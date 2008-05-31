/*
Copyright (C) 2008 Vana Development Team

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
#include "WorldServerAcceptHandler.h"
#include "WorldServerAcceptPlayerPacket.h"
#include "WorldServerAcceptPlayer.h"
#include "WorldServer.h"
#include "BufferUtilities.h"
#include "Channels.h"
#include "Players.h"
#include "ReadPacket.h"

void WorldServerAcceptHandler::playerChangeChannel(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	int playerid = packet->getInt();
	Channel *chan = Channels::Instance()->getChannel(packet->getInt());
	WorldServerAcceptPlayerPacket::newConnectable(chan->id, playerid);
	WorldServerAcceptPlayerPacket::playerChangeChannel(player, playerid, chan->ip, chan->port);
}

void WorldServerAcceptHandler::findPlayer(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	int finder = packet->getInt();
	string findee_name = packet->getString();

	Player *findee = Players::Instance()->getPlayerFromName(findee_name);
	if (findee->channel != -1)
		WorldServerAcceptPlayerPacket::findPlayer(player, finder, findee->channel, findee->name);
	else
		WorldServerAcceptPlayerPacket::findPlayer(player, finder, findee->channel, findee_name);
}

void WorldServerAcceptHandler::whisperPlayer(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	int whisperer = packet->getInt();
	string whisperee_name = packet->getString();
	string message = packet->getString();

	Player *whisperee = Players::Instance()->getPlayerFromName(whisperee_name);
	if (whisperee->channel != -1) {
		WorldServerAcceptPlayerPacket::findPlayer(player, whisperer, -1, whisperee->name, 1);
		WorldServerAcceptPlayerPacket::whisperPlayer(Channels::Instance()->getChannel(whisperee->channel)->player, whisperee->id, Players::Instance()->getPlayer(whisperer)->name, player->getChannel(),  message);
	}
	else
		WorldServerAcceptPlayerPacket::findPlayer(player, whisperer, whisperee->channel, whisperee_name);
}

void WorldServerAcceptHandler::registerPlayer(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	int id = packet->getInt();
	string name = packet->getString();
	Players::Instance()->registerPlayer(id, name, player->getChannel());
}

void WorldServerAcceptHandler::removePlayer(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	int id = packet->getInt();
	Players::Instance()->remove(id, player->getChannel());
}

void WorldServerAcceptHandler::scrollingHeader(WorldServerAcceptPlayer *player, ReadPacket *packet) {
	string message = packet->getString();
	WorldServer::Instance()->setScrollingHeader(message);
}
