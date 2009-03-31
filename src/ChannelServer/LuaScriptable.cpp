/*
Copyright (C) 2008-2009 Vana Development Team

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
#include "LuaScriptable.h"
#include "Inventory.h"
#include "Player.h"
#include "Players.h"
#include "Maps.h"
#include "MapPacket.h"
#include "Mobs.h"
#include "NPCs.h"
#include "Reactors.h"
#include "Quests.h"
#include "Levels.h"
#include "ShopDataProvider.h"
#include "TimeUtilities.h"
#include "PlayerPacket.h"
#include "InventoryPacket.h"
#include "Randomizer.h"
#include <iostream>

LuaScriptable::LuaScriptable(const string &filename, int32_t playerid) : filename(filename), playerid(playerid), luaVm(lua_open()) {
	initialize();
}

LuaScriptable::~LuaScriptable() {
	lua_close(luaVm);
}

void LuaScriptable::initialize() {
	luaopen_base(luaVm);
	setVariable("playerid", playerid); // Pushing id for reference from static functions

	// Miscellanous
	lua_register(luaVm, "getRandomNumber", &LuaExports::getRandomNumber);
	lua_register(luaVm, "runNPC", &LuaExports::runNPC);
	lua_register(luaVm, "getChannel", &LuaExports::getChannel);
	lua_register(luaVm, "showShop", &LuaExports::showShop);

	// Buddy
	lua_register(luaVm, "addBuddySlots", &LuaExports::addBuddySlots);
	lua_register(luaVm, "getBuddySlots", &LuaExports::getBuddySlots);

	// Skill
	lua_register(luaVm, "addSkillLevel", &LuaExports::addSkillLevel);
	lua_register(luaVm, "getSkillLevel", &LuaExports::getSkillLevel);

	// Inventory
	lua_register(luaVm, "addSlots", &LuaExports::addSlots);
	lua_register(luaVm, "addStorageSlots", &LuaExports::addStorageSlots);
	lua_register(luaVm, "getItemAmount", &LuaExports::getItemAmount);
	lua_register(luaVm, "hasOpenSlotsFor", &LuaExports::hasOpenSlotsFor);
	lua_register(luaVm, "getOpenSlots", &LuaExports::getOpenSlots);
	lua_register(luaVm, "useItem", &LuaExports::useItem);
	lua_register(luaVm, "giveItem", &LuaExports::giveItem);
	lua_register(luaVm, "getMesos", &LuaExports::getMesos);
	lua_register(luaVm, "giveMesos", &LuaExports::giveMesos);

	// Player
	lua_register(luaVm, "getID", &LuaExports::getID);
	lua_register(luaVm, "getSP", &LuaExports::getSP);
	lua_register(luaVm, "getAP", &LuaExports::getAP);
	lua_register(luaVm, "getSTR", &LuaExports::getSTR);
	lua_register(luaVm, "getDEX", &LuaExports::getDEX);
	lua_register(luaVm, "getINT", &LuaExports::getINT);
	lua_register(luaVm, "getLUK", &LuaExports::getLUK);
	lua_register(luaVm, "getHPMPAP", &LuaExports::getHPMPAP);
	lua_register(luaVm, "getJob", &LuaExports::getJob);
	lua_register(luaVm, "getLevel", &LuaExports::getLevel);
	lua_register(luaVm, "getGender", &LuaExports::getGender);
	lua_register(luaVm, "getMap", &LuaExports::getMap);
	lua_register(luaVm, "getFH", &LuaExports::getFH);
	lua_register(luaVm, "getPosX", &LuaExports::getPosX);
	lua_register(luaVm, "getPosY", &LuaExports::getPosY);
	lua_register(luaVm, "getEXP", &LuaExports::getEXP);
	lua_register(luaVm, "getHP", &LuaExports::getHP);
	lua_register(luaVm, "getMHP", &LuaExports::getMHP);
	lua_register(luaVm, "getRMHP", &LuaExports::getRMHP);
	lua_register(luaVm, "getMP", &LuaExports::getMP);
	lua_register(luaVm, "getMMP", &LuaExports::getMMP);
	lua_register(luaVm, "getRMMP", &LuaExports::getRMMP);
	lua_register(luaVm, "getHair", &LuaExports::getHair);
	lua_register(luaVm, "getSkin", &LuaExports::getSkin);
	lua_register(luaVm, "getEyes", &LuaExports::getEyes);
	lua_register(luaVm, "getName", &LuaExports::getName);
	lua_register(luaVm, "getGMLevel", &LuaExports::getGMLevel);
	lua_register(luaVm, "isGM", &LuaExports::isGM);
	lua_register(luaVm, "giveEXP", &LuaExports::giveEXP);
	lua_register(luaVm, "giveSP", &LuaExports::giveSP);
	lua_register(luaVm, "giveAP", &LuaExports::giveAP);
	lua_register(luaVm, "getPlayerVariable", &LuaExports::getPlayerVariable);
	lua_register(luaVm, "setPlayerVariable", &LuaExports::setPlayerVariable);
	lua_register(luaVm, "deletePlayerVariable", &LuaExports::deletePlayerVariable);
	lua_register(luaVm, "setStyle", &LuaExports::setStyle);
	lua_register(luaVm, "setMap", &LuaExports::setMap);
	lua_register(luaVm, "setEXP", &LuaExports::setEXP);
	lua_register(luaVm, "setHP", &LuaExports::setHP);
	lua_register(luaVm, "setMHP", &LuaExports::setMHP);
	lua_register(luaVm, "setRMHP", &LuaExports::setRMHP);
	lua_register(luaVm, "setMP", &LuaExports::setMP);
	lua_register(luaVm, "setMMP", &LuaExports::setMMP);
	lua_register(luaVm, "setRMMP", &LuaExports::setRMMP);
	lua_register(luaVm, "setSP", &LuaExports::setSP);
	lua_register(luaVm, "setAP", &LuaExports::setAP);
	lua_register(luaVm, "setSTR", &LuaExports::setSTR);
	lua_register(luaVm, "setDEX", &LuaExports::setDEX);
	lua_register(luaVm, "setINT", &LuaExports::setINT);
	lua_register(luaVm, "setLUK", &LuaExports::setLUK);
	lua_register(luaVm, "setJob", &LuaExports::setJob);
	lua_register(luaVm, "setLevel", &LuaExports::setLevel);

	// Map
	lua_register(luaVm, "showMessage", &LuaExports::showMessage);
	lua_register(luaVm, "showMapMessage", &LuaExports::showMapMessage);
	lua_register(luaVm, "showMapEvent", &LuaExports::showMapEvent);
	lua_register(luaVm, "showMapTimer", &LuaExports::showMapTimer);
	lua_register(luaVm, "playSoundMap", &LuaExports::playSoundMap);
	lua_register(luaVm, "playSoundPlayer", &LuaExports::playSoundPlayer);
	lua_register(luaVm, "showInstructionBubble", &LuaExports::showInstructionBubble);
	lua_register(luaVm, "spawnMob", &LuaExports::spawnMob);
	lua_register(luaVm, "spawnMobPos", &LuaExports::spawnMobPos);
	lua_register(luaVm, "getNumPlayers", &LuaExports::getNumPlayers);
	lua_register(luaVm, "getReactorState", &LuaExports::getReactorState);
	lua_register(luaVm, "setReactorsState", &LuaExports::setReactorsState);
	lua_register(luaVm, "killMob", &LuaExports::killMob);
	lua_register(luaVm, "countMobs", &LuaExports::countMobs);
	lua_register(luaVm, "clearMobs", &LuaExports::clearMobs);
	lua_register(luaVm, "clearDrops", &LuaExports::clearDrops);
	lua_register(luaVm, "setMusic", &LuaExports::setMusic);

	// Time
	lua_register(luaVm, "getChannelClock", &LuaExports::getChannelClock);
	lua_register(luaVm, "getWorldClock", &LuaExports::getWorldClock);
	lua_register(luaVm, "getTime", &LuaExports::getTime);
	lua_register(luaVm, "getDay", &LuaExports::getDay);
	lua_register(luaVm, "getDate", &LuaExports::getDate);
	lua_register(luaVm, "getMonth", &LuaExports::getMonth);
	lua_register(luaVm, "getYear", &LuaExports::getYear);
	lua_register(luaVm, "getHour", &LuaExports::getHour);
	lua_register(luaVm, "getMinute", &LuaExports::getMinute);
	lua_register(luaVm, "getSecond", &LuaExports::getSecond);
	lua_register(luaVm, "getDST", &LuaExports::getDST);

	// Rates
	lua_register(luaVm, "getEXPRate", &LuaExports::getEXPRate);
	lua_register(luaVm, "getMesoRate", &LuaExports::getMesoRate);
	lua_register(luaVm, "getQuestEXPRate", &LuaExports::getQuestEXPRate);
	lua_register(luaVm, "getDropRate", &LuaExports::getDropRate);
}

bool LuaScriptable::run() {
	if (luaL_dofile(luaVm, filename.c_str())) {
		// Error in lua script
		string error = lua_tostring(luaVm, -1);
		std::cout << error << std::endl;

		Player *player = Players::Instance()->getPlayer(playerid);

		if (player->isGM()) {
			PlayerPacket::showMessage(player, error, 6);
		}
		else {
			PlayerPacket::showMessage(player, "There is an error in the script '" + filename +"'", 6);
		}
		return false;
	}
	return true;
}

void LuaScriptable::setVariable(const string &name, int32_t val) {
	lua_pushinteger(luaVm, val);
	lua_setglobal(luaVm, name.c_str());
}

void LuaScriptable::setVariable(const string &name, const string &val) {
	lua_pushstring(luaVm, val.c_str());
	lua_setglobal(luaVm, name.c_str());
}

Player * LuaExports::getPlayer(lua_State *luaVm) {
	lua_getglobal(luaVm, "playerid");
	return Players::Instance()->getPlayer(lua_tointeger(luaVm, -1));
}

// Miscellaneous
int LuaExports::getRandomNumber(lua_State *luaVm) {
	int32_t number = lua_tointeger(luaVm, -1);
	lua_pushinteger(luaVm, Randomizer::Instance()->randInt(number - 1) + 1);
	return 1;
}

int LuaExports::runNPC(lua_State *luaVm) {
	int32_t npcid = lua_tointeger(luaVm, -1);
	NPC *npc = new NPC(npcid, getPlayer(luaVm));
	npc->run();
	return 0;
}

int LuaExports::getChannel(lua_State *luaVm) {
	lua_pushnumber(luaVm, ChannelServer::Instance()->getChannel() + 1);
	return 1;
}

int LuaExports::showShop(lua_State *luaVm) {
	int32_t shopid = lua_tointeger(luaVm, -1);
	ShopDataProvider::Instance()->showShop(getPlayer(luaVm), shopid);
	return 0;
}

// Buddy
int LuaExports::addBuddySlots(lua_State *luaVm) {
	Player *p = getPlayer(luaVm);
	uint8_t csize = p->getBuddyListSize();
	int8_t mod = lua_tointeger(luaVm, 1);
	p->setBuddyListSize(csize + mod);
	return 0;
}

int LuaExports::getBuddySlots(lua_State *luaVm) {
	lua_pushinteger(luaVm, getPlayer(luaVm)->getBuddyListSize());
	return 1;
}

// Skills
int LuaExports::addSkillLevel(lua_State *luaVm) {
	int32_t skillid = lua_tointeger(luaVm, -2);
	uint8_t level = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->getSkills()->addSkillLevel(skillid, level);
	return 0;
}

int LuaExports::getSkillLevel(lua_State *luaVm) {
	int32_t skillid = lua_tointeger(luaVm, -1);
	lua_pushnumber(luaVm, getPlayer(luaVm)->getSkills()->getSkillLevel(skillid));
	return 1;
}

// Inventory
int LuaExports::addSlots(lua_State *luaVm) {
	int8_t inventory = lua_tointeger(luaVm, -2);
	int8_t rows = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->getInventory()->addMaxSlots(inventory, rows);
	return 0;
}

int LuaExports::addStorageSlots(lua_State *luaVm) {
	int8_t slots = lua_tointeger(luaVm, 1);
	getPlayer(luaVm)->getStorage()->setSlots(getPlayer(luaVm)->getStorage()->getSlots() + slots);
	return 0;
}

int LuaExports::giveItem(lua_State *luaVm) {
	int32_t itemid = lua_tointeger(luaVm, -2);
	int16_t amount = lua_tointeger(luaVm, -1);
	bool success = Quests::giveItem(getPlayer(luaVm), itemid, amount);
	lua_pushboolean(luaVm, success);
	return 1;
}

int LuaExports::giveMesos(lua_State *luaVm) {
	int32_t mesos = lua_tointeger(luaVm, -1);
	bool success = Quests::giveMesos(getPlayer(luaVm), mesos);
	lua_pushboolean(luaVm, success);
	return 1;
}

int LuaExports::hasOpenSlotsFor(lua_State *luaVm) {
	int32_t itemid = lua_tointeger(luaVm, 1);
	int16_t amount = 1;
	if (lua_isnumber(luaVm, 2))
		amount = lua_tointeger(luaVm, 2);
	lua_pushboolean(luaVm, getPlayer(luaVm)->getInventory()->hasOpenSlotsFor(itemid, amount));
	return 1;
}

int LuaExports::getOpenSlots(lua_State *luaVm) {
	int8_t inv = lua_tointeger(luaVm, -1);
	lua_pushnumber(luaVm, getPlayer(luaVm)->getInventory()->getOpenSlotsNum(inv));
	return 1;
}

int LuaExports::getItemAmount(lua_State *luaVm) {
	int32_t itemid = lua_tointeger(luaVm, -1);
	lua_pushnumber(luaVm, getPlayer(luaVm)->getInventory()->getItemAmount(itemid));
	return 1;
}

int LuaExports::getMesos(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getInventory()->getMesos());
	return 1;
}

int LuaExports::useItem(lua_State *luaVm) {
	int32_t itemid = lua_tointeger(luaVm, -1);
	Inventory::useItem(getPlayer(luaVm), itemid);
	return 0;
}

// Player
int LuaExports::getID(lua_State *luaVm) {
	lua_pushinteger(luaVm, getPlayer(luaVm)->getId());
	return 1;
}

int LuaExports::giveEXP(lua_State *luaVm) {
	int32_t exp = lua_tointeger(luaVm, -1);
	Levels::giveEXP(getPlayer(luaVm), exp, true);
	return 0;
}

int LuaExports::giveSP(lua_State *luaVm) {
	int16_t sp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setSP(getPlayer(luaVm)->getSP() + sp);
	return 0;
}

int LuaExports::giveAP(lua_State *luaVm) {
	int16_t ap = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setAP(getPlayer(luaVm)->getAP() + ap);
	return 0;
}

int LuaExports::getSP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getSP());
	return 1;
}

int LuaExports::getAP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getAP());
	return 1;
}

int LuaExports::getSTR(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getStr());
	return 1;
}

int LuaExports::getDEX(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getDex());
	return 1;
}

int LuaExports::getINT(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getInt());
	return 1;
}

int LuaExports::getLUK(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getLuk());
	return 1;
}

int LuaExports::getHPMPAP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getHPMPAP());
	return 1;
}

int LuaExports::getJob(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getJob());
	return 1;
}

int LuaExports::getLevel(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getLevel());
	return 1;
}

int LuaExports::getGender(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getGender());
	return 1;
}

int LuaExports::getMap(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getMap());
	return 1;
}

int LuaExports::getFH(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getFH());
	return 1;
}

int LuaExports::getPosX(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getPos().x);
	return 1;
}

int LuaExports::getPosY(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getPos().y);
	return 1;
}

int LuaExports::getEXP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getExp());
	return 1;
}

int LuaExports::getHP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getHP());
	return 1;
}

int LuaExports::getMHP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getMHP());
	return 1;
}

int LuaExports::getRMHP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getRMHP());
	return 1;
}

int LuaExports::getMP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getMP());
	return 1;
}

int LuaExports::getMMP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getMMP());
	return 1;
}

int LuaExports::getRMMP(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getRMMP());
	return 1;
}

int LuaExports::getHair(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getHair());
	return 1;
}

int LuaExports::getSkin(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getSkin());
	return 1;
}

int LuaExports::getEyes(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getEyes());
	return 1;
}

int LuaExports::getName(lua_State *luaVm) {
	lua_pushstring(luaVm, getPlayer(luaVm)->getName().c_str());
	return 1;
}

int LuaExports::isGM(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->isGM());
	return 1;
}

int LuaExports::getGMLevel(lua_State *luaVm) {
	lua_pushnumber(luaVm, getPlayer(luaVm)->getGMLevel());
	return 1;
}

int LuaExports::setStyle(lua_State *luaVm) {
	int32_t id = lua_tointeger(luaVm, -1);
	if (id/10000 == 0) {
		getPlayer(luaVm)->setSkin((int8_t)id);
	}
	else if (id/10000 == 2) {
		getPlayer(luaVm)->setEyes(id);
	}
	else if (id/10000 == 3) {
		getPlayer(luaVm)->setHair(id);
	}
	InventoryPacket::updatePlayer(getPlayer(luaVm));
	return 0;
}

int LuaExports::setMap(lua_State *luaVm) {
	PortalInfo *portal = 0;

	int32_t mapid = lua_tointeger(luaVm, 1);

	if (lua_isstring(luaVm, 2)) { // Optional portal parameter
		string to = lua_tostring(luaVm, 2);
		portal = Maps::getMap(mapid)->getPortal(to);
	}

	if (Maps::getMap(mapid))
		Maps::changeMap(getPlayer(luaVm), mapid, portal);
	return 0;
}

int LuaExports::setEXP(lua_State *luaVm) {
	int32_t exp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setExp(exp);
	return 0;
}

int LuaExports::setHP(lua_State *luaVm) {
	uint16_t hp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setHP(hp);
	return 0;
}

int LuaExports::setMHP(lua_State *luaVm) {
	uint16_t hp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setMHP(hp);
	return 0;
}

int LuaExports::setRMHP(lua_State *luaVm) {
	uint16_t hp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setRMHP(hp);
	return 0;
}

int LuaExports::setMP(lua_State *luaVm) {
	uint16_t mp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setMP(mp);
	return 0;
}

int LuaExports::setMMP(lua_State *luaVm) {
	uint16_t mp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setMMP(mp);
	return 0;
}

int LuaExports::setRMMP(lua_State *luaVm) {
	uint16_t mp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setRMMP(mp);
	return 0;
}

int LuaExports::setSP(lua_State *luaVm) {
	int16_t sp = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setSP(sp);
	return 0;
}

int LuaExports::setAP(lua_State *luaVm) {
	int16_t ap = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setAP(ap);
	return 0;
}

int LuaExports::setSTR(lua_State *luaVm) {
	int16_t str = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setStr(str);
	return 0;
}

int LuaExports::setDEX(lua_State *luaVm) {
	int16_t dex = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setDex(dex);
	return 0;
}

int LuaExports::setINT(lua_State *luaVm) {
	int16_t intt = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setInt(intt);
	return 0;
}

int LuaExports::setLUK(lua_State *luaVm) {
	int16_t luk = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setLuk(luk);
	return 0;
}

int LuaExports::setJob(lua_State *luaVm) {
	int16_t job = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setJob(job);
	return 0;
}

int LuaExports::setLevel(lua_State *luaVm) {
	uint8_t level = lua_tointeger(luaVm, -1);
	getPlayer(luaVm)->setLevel(level);
	return 0;
}

int LuaExports::getPlayerVariable(lua_State *luaVm) {
	string key = string(lua_tostring(luaVm, -1));
	lua_pushstring(luaVm, getPlayer(luaVm)->getVariable(key).c_str());
	return 1;
}

int LuaExports::deletePlayerVariable(lua_State *luaVm) {
	string key = string(lua_tostring(luaVm, -1));
	getPlayer(luaVm)->deleteVariable(key);
	return 0;
}

int LuaExports::setPlayerVariable(lua_State *luaVm) {
	string value = string(lua_tostring(luaVm, -1));
	string key = string(lua_tostring(luaVm, -2));
	getPlayer(luaVm)->setVariable(key, value);
	return 0;
}

// Map
int LuaExports::getNumPlayers(lua_State *luaVm) {
	int32_t mapid = lua_tointeger(luaVm, -1);
	lua_pushinteger(luaVm, Maps::getMap(mapid)->getNumPlayers());
	return 1;
}

int LuaExports::getReactorState(lua_State *luaVm) {
	int32_t mapid = lua_tointeger(luaVm, -2);
	int32_t reactorid = lua_tointeger(luaVm, -1);
	for (uint32_t i = 0; i < Maps::getMap(mapid)->getNumReactors(); i++) {
		if (Maps::getMap(mapid)->getReactor(i)->getReactorID() == reactorid) {
			lua_pushinteger(luaVm, Maps::getMap(mapid)->getReactor(i)->getState());
			return 1;
		}
	}
	lua_pushinteger(luaVm, 0);
	return 1;
}

int LuaExports::killMob(lua_State *luaVm) {
	int32_t mobid = lua_tointeger(luaVm, 1);
	int32_t mapid = getPlayer(luaVm)->getMap();
	bool playerkill = true;
	if (lua_isboolean(luaVm, 2))
		playerkill = (lua_toboolean(luaVm, 2) == 1 ? true : false);
	int32_t killed = Maps::getMap(mapid)->killMobs(getPlayer(luaVm), mobid, playerkill, true);
	lua_pushinteger(luaVm, killed);
	return 1;
}

int LuaExports::clearMobs(lua_State *luaVm) {
	int32_t mapid = getPlayer(luaVm)->getMap();
	Maps::getMap(mapid)->killMobs(0, 0, false, false);
	return 0;
}

int LuaExports::clearDrops(lua_State *luaVm) {
	int32_t mapid = getPlayer(luaVm)->getMap();
	Maps::getMap(mapid)->clearDrops(false);
	return 0;
}

int LuaExports::countMobs(lua_State *luaVm) {
	int32_t mapid = lua_tointeger(luaVm, 1);
	int32_t mobid = 0;
	if (lua_isnumber(luaVm, 2))
		mobid = lua_tointeger(luaVm, 2);
	lua_pushinteger(luaVm, Maps::getMap(mapid)->countMobs(mobid));
	return 1;
}

int LuaExports::setMusic(lua_State *luaVm) {
	Maps::getMap(getPlayer(luaVm)->getMap())->setMusic(lua_tostring(luaVm, -1));
	return 0;
}

int LuaExports::setReactorsState(lua_State *luaVm) {
	int32_t mapid = lua_tointeger(luaVm, -3);
	int32_t reactorid = lua_tointeger(luaVm, -2);
	uint8_t state = lua_tointeger(luaVm, -1);
	for (size_t i = 0; i < Maps::getMap(mapid)->getNumReactors(); i++) {
		Reactor *reactor = Maps::getMap(mapid)->getReactor(i);
		if (reactor->getReactorID() == reactorid) {
			reactor->setState(state, true);
			break;
		}
	}
	return 0;
}

int LuaExports::showMessage(lua_State *luaVm) {
	string msg = lua_tostring(luaVm, -2);
	uint8_t type = lua_tointeger(luaVm, -1);
	PlayerPacket::showMessage(getPlayer(luaVm), msg, type);
	return 0;
}

int LuaExports::showMapMessage(lua_State *luaVm) {
	string msg = lua_tostring(luaVm, -2);
	uint8_t type = lua_tointeger(luaVm, -1);
	int32_t map = getPlayer(luaVm)->getMap();
	Maps::getMap(map)->showMessage(msg, type);
	return 0;
}

int LuaExports::showMapEvent(lua_State *luaVm) {
	string val = lua_tostring(luaVm, -1);
	MapPacket::sendEvent(getPlayer(luaVm)->getMap(), val);
	return 0;
}

int LuaExports::playSoundMap(lua_State *luaVm) {
	string val = lua_tostring(luaVm, -1);
	MapPacket::sendSound(getPlayer(luaVm)->getMap(), val);
	return 0;
}

int LuaExports::playSoundPlayer(lua_State *luaVm) {
	string val = lua_tostring(luaVm, -1);
	PlayerPacket::sendSound(getPlayer(luaVm), val);
	return 0;
}

int LuaExports::showInstructionBubble(lua_State *luaVm) {
	string msg = lua_tostring(luaVm, 1);
	int16_t width = lua_tointeger(luaVm, 2);
	int16_t height = lua_tointeger(luaVm, 3);

	if (width == 0) {
		width = -1;
	}
	if (height == 0) {
		height = 5;
	}

	PlayerPacket::instructionBubble(getPlayer(luaVm), msg, width, height);
	return 0;
}

int LuaExports::showMapTimer(lua_State *luaVm) {
	int32_t mapid = lua_tointeger(luaVm, 1);
	int32_t time = lua_tointeger(luaVm, 2);
	Maps::getMap(mapid)->setMapTimer(time);
	return 0;
}

int LuaExports::spawnMob(lua_State *luaVm) {
	int32_t mobid = lua_tointeger(luaVm, -1);
	Player *player = getPlayer(luaVm);
	Maps::getMap(player->getMap())->spawnMob(mobid, player->getPos());
	return 0;
}

int LuaExports::spawnMobPos(lua_State *luaVm) {
	int32_t mobid = lua_tointeger(luaVm, 1);
	int16_t x = lua_tointeger(luaVm, 2);
	int16_t y = lua_tointeger(luaVm, 3);
	int16_t fh = 0;
	if (lua_isnumber(luaVm, 4))
		fh = lua_tointeger(luaVm, 4);
	Maps::getMap(getPlayer(luaVm)->getMap())->spawnMob(mobid, Pos(x, y), -1, fh);
	return 0;
}

// Time
int LuaExports::getChannelClock(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::clock_in_ms());
	return 1;
}

int LuaExports::getWorldClock(lua_State *luaVm) {
	lua_pushinteger(luaVm, ChannelServer::Instance()->getWorldClock() + TimeUtilities::clock_in_ms());
	return 1;
}

int LuaExports::getTime(lua_State *luaVm) {
	lua_pushinteger(luaVm, (lua_Integer) time(0)); // Here's to hoping that lua_Integer is an 8-byte type on most platforms!
	return 1;
}

int LuaExports::getDate(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getDate());
	return 1;
}

int LuaExports::getDay(lua_State *luaVm) {
	bool stringreturn = false;
	if (lua_isboolean(luaVm, -1))
		stringreturn = (lua_toboolean(luaVm, -1) != 0 ? true : false);
	if (stringreturn)
		lua_pushstring(luaVm, TimeUtilities::getDayString().c_str());
	else
		lua_pushinteger(luaVm, TimeUtilities::getDay());
	return 1;
}

int LuaExports::getMonth(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getMonth());
	return 1;
}

int LuaExports::getYear(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getYear());
	return 1;
}

int LuaExports::getHour(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getHour());
	return 1;
}

int LuaExports::getMinute(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getMinute());
	return 1;
}

int LuaExports::getSecond(lua_State *luaVm) {
	lua_pushinteger(luaVm, TimeUtilities::getSecond());
	return 1;
}

int LuaExports::getDST(lua_State *luaVm) {
	lua_pushboolean(luaVm, TimeUtilities::getDST());
	return 1;
}

// Rates
int LuaExports::getEXPRate(lua_State *luaVm) {
	lua_pushnumber(luaVm, ChannelServer::Instance()->getExprate());
	return 1;
}

int LuaExports::getQuestEXPRate(lua_State *luaVm) {
	lua_pushnumber(luaVm, ChannelServer::Instance()->getQuestExprate());
	return 1;
}

int LuaExports::getMesoRate(lua_State *luaVm) {
	lua_pushnumber(luaVm, ChannelServer::Instance()->getMesorate());
	return 1;
}

int LuaExports::getDropRate(lua_State *luaVm) {
	lua_pushnumber(luaVm, ChannelServer::Instance()->getDroprate());
	return 1;
}