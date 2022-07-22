﻿#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "database.h"
#include <D:\boost_1_78_0\boost_1_78_0\boost/lexical_cast.hpp>
#include "common.h"
#include "sqlite3.h"

database::database() {
	if (sqlite3_open_v2(".\\sxd.sqlite", &db, SQLITE_OPEN_READWRITE, NULL) == 0)
		return;
	if (sqlite3_open_v2("..\\sxd.sqlite", &db, SQLITE_OPEN_READWRITE, NULL) == 0)
		return;
	std::cerr << sqlite3_errmsg(db) << std::endl;
	exit(1);
} //database::database

database::~database(void) {
	sqlite3_close(db);
} //database::~database

void database::execute(const std::string& sql) {
	this->execute(sql.c_str());
}

void database::execute(const char* sql) {
	char* zErrMsg = nullptr;
	if (sqlite3_exec(db, sql, 0, 0, &zErrMsg)) {
		std::ostringstream err;
		err << "SQL error: " << zErrMsg;
		sqlite3_free(zErrMsg);
		throw std::runtime_error(err.str());
	}
}

std::list<mss> database::get_records(const std::string& table, const std::string& where_clause) {
	return this->get_records(table.c_str(), where_clause.c_str());
}

std::list<mss> database::get_records(const char* table, const char* where_clause) {
	std::list<mss> items;
	std::ostringstream sql;
	sql << "select * from " << table << " where " << where_clause;
	char* zErrMsg = nullptr;
	if (sqlite3_exec(db, sql.str().c_str(), &database::callback, static_cast<void *>(&items), &zErrMsg)) {
		std::ostringstream err;
		err << "SQL error: " << zErrMsg;
		sqlite3_free(zErrMsg);
		throw std::runtime_error(err.str());
	}
	return items;
}

mss database::get_record(const std::string& table, const std::string& where_clause) {
	return this->get_record(table.c_str(), where_clause.c_str());
}

mss database::get_record(const char* table, const char* where_clause) {
	auto items = get_records(table, where_clause);
	if (items.size() == 0) {
		std::ostringstream err;
		err << "Not found record in " << table << " with " << where_clause;
		throw std::runtime_error(err.str());
	}
	else if (items.size() > 1) {
		std::ostringstream err;
		err << "Find too many records in " << table << " with " << where_clause;
		throw std::runtime_error(err.str());
	}
	return *items.begin();
}

// table user
std::list<mss> database::get_all_users() {
	return get_records("user", "1=1");
}

// table protocol
mss database::get_protocol(const char* version, short module, short action) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and module=" << module << " and action=" << action;
	return this->get_record("protocol", where_clause.str());
}

// table config
std::string database::get_config(const char* user_id, const char* name) {
	std::ostringstream where_clause;
	where_clause << "user='" << user_id << "' and name='" << name << "'";
	auto items = get_records("config", where_clause.str().c_str());
	if (items.size() == 0) {
		where_clause.str("");
		where_clause << "user='default' and name='" << name << "'";
		items = get_records("config", where_clause.str().c_str());
		if (items.size() == 0) {
			std::ostringstream err;
			err << "Not found config with " << where_clause.str();
			throw std::runtime_error(err.str());
		}
		else if (items.size() > 1) {
			std::ostringstream err;
			err << "Find too many config with " << where_clause.str();
			throw std::runtime_error(err.str());
		}
		return (*items.begin())["value"];
	}
	else if (items.size() > 1) {
		std::ostringstream err;
		err << "Find too many config with " << where_clause.str();
		throw std::runtime_error(err.str());
	}
	return (*items.begin())["value"];
}

// table lucky_shop_item
mss database::get_lucky_shop_item(const std::string& version, int id) {
	return this->get_lucky_shop_item(version.c_str(), id);
}

mss database::get_lucky_shop_item(const char* version, int id) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and id=" << id;
	mss item = this->get_record("lucky_shop_item", where_clause.str());
	int item_id = boost::lexical_cast<int>(item["item_id"]);
	item.insert(std::make_pair("item_name", this->get_code(version, "Item", item_id)["text"]));
	item.insert(std::make_pair("item_comment", this->get_code(version, "Item", item_id)["comment"]));
	return item;
}

// table code
mss database::get_code(const std::string& version, const std::string& type, int value) {
	return this->get_code(version.c_str(), type.c_str(), value);
}

mss database::get_code(const char* version, const char* type, int value) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and type='" << type << "' and value='" << value << "'";
	return this->get_record("code", where_clause.str());
}

mss database::get_code(const std::string& version, const std::string& type, const std::string& text) {
	return this->get_code(version.c_str(), type.c_str(), text.c_str());
}

mss database::get_code(const char* version, const char* type, const char* text) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and type='" << type << "' and text='" << text << "'";
	return this->get_record("code", where_clause.str());
}

std::list<mss> database::get_facture_reel(const std::string& version, int item_id) {
	return this->get_facture_reel(version.c_str(), item_id);
}

std::list<mss> database::get_facture_reel(const char* version, int item_id) {
	std::ostringstream where_clause;
	// ÅÅ³ý33¸öÌØÊâ¾íÖá
	where_clause << "version='" << version << "' and item_id=" << item_id << " and reel_id not in (1550, 1551, 1552, 1553, 1554, 1555, 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576, 1577, 1578, 1579, 1580, 1581, 1582)";
	mss item = this->get_record("facture_reel", where_clause.str());
	int reel_id = boost::lexical_cast<int>(item["reel_id"]);

	where_clause.str("");
	where_clause << "version='" << version << "' and reel_id=" << reel_id << " and item_id<>" << item_id;
	return this->get_records("facture_reel", where_clause.str());
}

std::string database::get_max_version() {
	std::list<mss> items;
	char* zErrMsg = nullptr;
	if (sqlite3_exec(db, "select max(version) mv from protocol", &database::callback, static_cast<void *>(&items), &zErrMsg)) {
		std::ostringstream err;
		err << "SQL error: " << zErrMsg;
		sqlite3_free(zErrMsg);
		throw std::runtime_error(err.str());
	}
	return (*items.begin())["mv"];
}

int database::callback(void* p, int argc, char** argv, char** azColName) {
	auto pitems = static_cast<std::list<mss>*>(p);
	mss item;
	for (auto i = 0; i < argc; i++)
		item.insert(std::make_pair(azColName[i], argv[i] ? common::utf2gbk(argv[i]) : "NULL"));
	pitems->push_back(item);
	return 0;
} //database::callback_user

// table mission
mss database::get_mission(const std::string& version, int mission_id) {
	return this->get_mission(version.c_str(), mission_id);
}
mss database::get_mission(const char* version ,int mission_id) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and mission_id=" << mission_id;
	return this->get_record("mission", where_clause.str());
}

// table mission_monster
std::list<mss> database::get_mission_teamid_by_monsterid(const std::string& version, int monster_id)
{
	return this->get_mission_teamid_by_monsterid(version.c_str(), monster_id);
}
std::list<mss> database::get_mission_teamid_by_monsterid(const char* version, int monster_id)
{
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and monster_id=" << monster_id;
	return this->get_records("mission_monster", where_clause.str());
}

// table quest
mss database::get_quest(const std::string& version, int quest_id) {
	return this->get_quest(version.c_str(), quest_id);
}
mss database::get_quest(const char* version, int quest_id) {
	std::ostringstream where_clause;
	where_clause << "version='" << version << "' and quest_id=" << quest_id;
	return this->get_record("quest", where_clause.str());
}

database db;