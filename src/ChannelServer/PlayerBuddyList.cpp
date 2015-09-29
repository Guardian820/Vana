/*
Copyright (C) 2008-2015 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "PlayerBuddyList.hpp"
#include "Common/Algorithm.hpp"
#include "Common/Database.hpp"
#include "Common/GameConstants.hpp"
#include "Common/MiscUtilities.hpp"
#include "Common/StringUtilities.hpp"
#include "ChannelServer/BuddyListPacket.hpp"
#include "ChannelServer/ChannelServer.hpp"
#include "ChannelServer/Player.hpp"
#include "ChannelServer/SyncPacket.hpp"
#include <algorithm>

namespace Vana {
namespace ChannelServer {

PlayerBuddyList::PlayerBuddyList(Player *player) :
	m_player{player}
{
	load();
}

auto PlayerBuddyList::load() -> void {
	auto &db = Database::getCharDb();
	auto &sql = db.getSession();

	soci::rowset<> rs = (sql.prepare
		<< "SELECT bl.id, bl.buddy_character_id, bl.name AS name_cache, c.name, bl.group_name, CASE WHEN c.online = 1 THEN u.online ELSE 0 END AS `online` "
		<< "FROM " << db.makeTable("buddylist") << " bl "
		<< "LEFT JOIN " << db.makeTable("characters") << " c ON bl.buddy_character_id = c.character_id "
		<< "LEFT JOIN " << db.makeTable("accounts") << " u ON c.account_id = u.account_id "
		<< "WHERE bl.character_id = :char",
		soci::use(m_player->getId(), "char"));

	for (const auto &row : rs) {
		addBuddy(db, row);
	}

	rs = (sql.prepare
		<< "SELECT p.* "
		<< "FROM " << db.makeTable("buddylist_pending") << " p "
		<< "LEFT JOIN " << db.makeTable("characters") << " c ON c.character_id = p.inviter_character_id "
		<< "WHERE c.world_id = :world AND p.character_id = :char ",
		soci::use(m_player->getId(), "char"),
		soci::use(ChannelServer::getInstance().getWorldId(), "world"));

	BuddyInvite invite;
	for (const auto &row : rs) {
		invite = BuddyInvite{};
		invite.id = row.get<player_id_t>("inviter_character_id");
		invite.name = row.get<string_t>("inviter_name");
		m_pendingBuddies.push_back(invite);
	}
}

auto PlayerBuddyList::addBuddy(const string_t &name, const string_t &group, bool invite) -> uint8_t {
	if (listSize() >= m_player->getBuddyListSize()) {
		// Buddy list full
		return Packets::Buddy::Errors::BuddyListFull;
	}

	if (!ext::in_range_inclusive<size_t>(name.size(), Characters::MinNameSize, Characters::MaxNameSize) || group.size() > Buddies::MaxGroupNameSize) {
		// Invalid name or group length
		return Packets::Buddy::Errors::UserDoesNotExist;
	}

	auto &db = Database::getCharDb();
	auto &sql = db.getSession();
	soci::row row;

	sql.once
		<< "SELECT c.character_id, c.name, u.gm_level, u.admin, c.buddylist_size AS buddylist_limit, ("
		<< "	SELECT COUNT(b.id) "
		<< "	FROM " << db.makeTable("buddylist") << " b "
		<< "	WHERE b.character_id = c.character_id"
		<< ") AS buddylist_size "
		<< "FROM " << db.makeTable("characters") << " c "
		<< "INNER JOIN " << db.makeTable("accounts") << " u ON c.account_id = u.account_id "
		<< "WHERE c.name = :name AND c.world_id = :world ",
		soci::use(name, "name"),
		soci::use(ChannelServer::getInstance().getWorldId(), "world"),
		soci::into(row);

	if (!sql.got_data()) {
		// Name does not exist
		return Packets::Buddy::Errors::UserDoesNotExist;
	}

	if (row.get<int32_t>("gm_level") > 0 && !m_player->isGm()) {
		// GM cannot be in buddy list unless the player is a GM
		return Packets::Buddy::Errors::NoGms;
	}

	if (row.get<bool>("admin") && !m_player->isAdmin()) {
		return Packets::Buddy::Errors::NoGms;
	}

	if (row.get<int64_t>("buddylist_size") >= row.get<int32_t>("buddylist_limit")) {
		// Opposite-end buddy list full
		return Packets::Buddy::Errors::TargetListFull;
	}

	player_id_t charId = row.get<player_id_t>("character_id");

	if (m_buddies.find(charId) != std::end(m_buddies)) {
		if (m_buddies[charId]->groupName == group) {
			// Already in buddy list
			return Packets::Buddy::Errors::AlreadyInList;
		}
		else {
			sql.once
				<< "UPDATE " << db.makeTable("buddylist") << " "
				<< "SET group_name = :name "
				<< "WHERE buddy_character_id = :buddy AND character_id = :owner ",
				soci::use(group, "name"),
				soci::use(charId, "buddy"),
				soci::use(m_player->getId(), "owner");

			m_buddies[charId]->groupName = group;
		}
	}
	else {
		sql.once
			<< "INSERT INTO " << db.makeTable("buddylist") << " (character_id, buddy_character_id, name, group_name) "
			<< "VALUES (:owner, :buddy, :name, :group)",
			soci::use(name, "name"),
			soci::use(group, "group"),
			soci::use(charId, "buddy"),
			soci::use(m_player->getId(), "owner");

		int32_t rowId = db.getLastId<int32_t>();

		sql.once
			<< "SELECT bl.id, bl.buddy_character_id, bl.name AS name_cache, c.name, bl.group_name, CASE WHEN c.online = 1 THEN u.online ELSE 0 END AS `online` "
			<< "FROM " << db.makeTable("buddylist") << " bl "
			<< "LEFT JOIN " << db.makeTable("characters") << " c ON bl.buddy_character_id = c.character_id "
			<< "LEFT JOIN " << db.makeTable("accounts") << " u ON c.account_id = u.account_id "
			<< "WHERE bl.id = :row",
			soci::use(rowId, "row"),
			soci::into(row);

		addBuddy(db, row);

		sql.once
			<< "SELECT id "
			<< "FROM " << db.makeTable("buddylist") << " "
			<< "WHERE character_id = :char AND buddy_character_id = :buddy",
			soci::use(charId, "char"),
			soci::use(m_player->getId(), "buddy"),
			soci::into(rowId);

		if (!sql.got_data()) {
			if (invite) {
				ChannelServer::getInstance().sendWorld(Packets::Interserver::Buddy::buddyInvite(m_player->getId(), charId));
			}
		}
		else {
			ChannelServer::getInstance().sendWorld(Packets::Interserver::Buddy::readdBuddy(m_player->getId(), charId));
		}
	}

	m_player->send(Packets::Buddy::update(m_player, Packets::Buddy::ActionTypes::Add));
	return Packets::Buddy::Errors::None;
}

auto PlayerBuddyList::removeBuddy(player_id_t charId) -> void {
	if (m_pendingBuddies.size() != 0 && m_sentRequest) {
		BuddyInvite invite = m_pendingBuddies.front();
		if (invite.id == charId) {
			removePendingBuddy(charId, false);
		}
		return;
	}

	if (m_buddies.find(charId) == std::end(m_buddies)) {
		// Hacking
		return;
	}

	ChannelServer::getInstance().sendWorld(Packets::Interserver::Buddy::removeBuddy(m_player->getId(), charId));
	m_buddies.erase(charId);

	auto &db = Database::getCharDb();
	auto &sql = db.getSession();
	sql.once
		<< "DELETE FROM " << db.makeTable("buddylist") << " "
		<< "WHERE character_id = :char AND buddy_character_id = :buddy",
		soci::use(m_player->getId(), "char"),
		soci::use(charId, "buddy");

	m_player->send(Packets::Buddy::update(m_player, Packets::Buddy::ActionTypes::Remove));
}

auto PlayerBuddyList::addBuddy(Database &db, const soci::row &row) -> void {
	player_id_t charId = row.get<player_id_t>("buddy_character_id");
	int32_t rowId = row.get<int32_t>("id");
	opt_string_t name = row.get<opt_string_t>("name");
	opt_string_t group = row.get<opt_string_t>("group_name");
	string_t cache = row.get<string_t>("name_cache");

	auto &sql = db.getSession();
	if (name.is_initialized() && name.get() != cache) {
		// Outdated name cache, i.e. character renamed
		sql.once
			<< "UPDATE " << db.makeTable("buddylist") << " "
			<< "SET name = :name "
			<< "WHERE id = :id ",
			soci::use(name.get(), "name"),
			soci::use(rowId, "id");
	}

	ref_ptr_t<Buddy> buddy = make_ref_ptr<Buddy>();
	buddy->charId = charId;

	// Note that the cache is for displaying the character name when the character in question is deleted
	buddy->name = name.get(cache);

	if (!group.is_initialized()) {
		buddy->groupName = "Default Group";
		sql.once
			<< "UPDATE " << db.makeTable("buddylist") << " "
			<< "SET group_name = :name "
			<< "WHERE buddy_character_id = :buddy AND character_id = :owner ",
			soci::use(buddy->groupName, "name"),
			soci::use(charId, "buddy"),
			soci::use(m_player->getId(), "owner");
	}
	else {
		buddy->groupName = group.get();
	}

	sql.once
		<< "SELECT id "
		<< "FROM " << db.makeTable("buddylist") << " "
		<< "WHERE character_id = :char AND buddy_character_id = :buddy ",
		soci::use(charId, "char"),
		soci::use(m_player->getId(), "buddy"),
		soci::into(rowId);

	if (sql.got_data()) {
		buddy->oppositeStatus = Packets::Buddy::OppositeStatus::Registered;
	}
	else {
		buddy->oppositeStatus = Packets::Buddy::OppositeStatus::Unregistered;
	}

	m_buddies[charId] = buddy;
}

auto PlayerBuddyList::addBuddies(PacketBuilder &builder) -> void {
	auto &provider = ChannelServer::getInstance().getPlayerDataProvider();
	for (const auto &kvp : m_buddies) {
		const ref_ptr_t<Buddy> &buddy = kvp.second;
		auto data = provider.getPlayerData(buddy->charId);

		builder.add<int32_t>(buddy->charId);
		builder.add<string_t>(buddy->name, 13);
		builder.add<uint8_t>(buddy->oppositeStatus);

		if (buddy->oppositeStatus == Packets::Buddy::OppositeStatus::Unregistered) {
			builder.add<int16_t>(0x00);
			builder.add<uint8_t>(0xF0);
			builder.add<uint8_t>(0xB2);
		}
		else if (data == nullptr) {
			builder.add<int32_t>(-1);
		}
		else {
			builder.add<int32_t>(data->channel.get(-1));
		}

		builder.add<string_t>(buddy->groupName, 13);
		builder.add<int8_t>(0x00);
		builder.add<int8_t>(20); // Seems to be the amount of buddy slots for the character...
		builder.add<uint8_t>(0xFD);
		builder.add<uint8_t>(0xBA);
	}
}

auto PlayerBuddyList::checkForPendingBuddy() -> void {
	if (m_pendingBuddies.size() == 0 || m_sentRequest) {
		// No buddies pending or request sent (didn't receive answer yet)
		return;
	}

	m_player->send(Packets::Buddy::invitation(m_pendingBuddies.front()));
	m_sentRequest = true;
}

auto PlayerBuddyList::buddyAccepted(player_id_t buddyId) -> void {
	m_buddies[buddyId]->oppositeStatus = Packets::Buddy::OppositeStatus::Registered;
	m_player->send(Packets::Buddy::update(m_player, Packets::Buddy::ActionTypes::Add));
}

auto PlayerBuddyList::removePendingBuddy(player_id_t id, bool accepted) -> void {
	if (m_pendingBuddies.size() == 0 || !m_sentRequest) {
		// Hacking
		return;
	}

	BuddyInvite invite = m_pendingBuddies.front();
	if (invite.id != id) {
		// Hacking
		ChannelServer::getInstance().log(LogType::Warning, [&](out_stream_t &log) {
			log << "Player tried to accept a player with player ID " << id
				<< " but the sent player ID was " << invite.id
				<< ". Player: " << m_player->getName();
		});
		return;
	}

	if (accepted) {
		int8_t error = addBuddy(invite.name, "Default Group", false);
		if (error != Packets::Buddy::Errors::None) {
			m_player->send(Packets::Buddy::error(error));
		}

		auto &db = Database::getCharDb();
		auto &sql = db.getSession();
		sql.once
			<< "DELETE FROM " << db.makeTable("buddylist_pending") << " "
			<< "WHERE character_id = :char AND inviter_character_id = :buddy",
			soci::use(m_player->getId(), "char"),
			soci::use(id, "buddy");

		ChannelServer::getInstance().sendWorld(Packets::Interserver::Buddy::acceptBuddyInvite(m_player->getId(), id));
	}

	m_player->send(Packets::Buddy::update(m_player, Packets::Buddy::ActionTypes::First));

	m_pendingBuddies.pop_front();
	m_sentRequest = false;
	checkForPendingBuddy();
}

auto PlayerBuddyList::getBuddyIds() -> vector_t<player_id_t> {
	vector_t<player_id_t> ids;
	for (const auto &kvp : m_buddies) {
		if (kvp.second->oppositeStatus == Packets::Buddy::OppositeStatus::Registered) {
			ids.push_back(kvp.second->charId);
		}
	}

	return ids;
}

}
}