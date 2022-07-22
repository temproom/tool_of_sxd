#ifndef DATABASE_H_
#define DATABASE_H_

#include <list>
#include <unordered_map>
#include "sqlite3.h"

typedef std::unordered_map<std::string, std::string> mss;

class database {
public:
	explicit database();
	~database(void);
	void execute(const std::string& sql);
	void execute(const char* sql);
	std::list<mss> get_records(const std::string& table, const std::string& where_clause);
	std::list<mss> get_records(const char* table, const char* where_clause);
	mss get_record(const std::string& table, const std::string& where_clause);
	mss get_record(const char* table, const char* where_clause);
	
	// table user
	std::list<mss> get_all_users();
	// table protocol
	mss get_protocol(const char* version, short module, short action);
	// table config
	std::string get_config(const char* user_id, const char* name);
	// table lucky_shop_item
	mss get_lucky_shop_item(const std::string& version, int id);
	mss get_lucky_shop_item(const char* version, int id);
	// table code
	mss get_code(const std::string& version, const std::string& type, int value);
	mss get_code(const char* version, const char* type, int value);
	mss get_code(const std::string& version, const std::string& type, const std::string& text);
	mss get_code(const char* version, const char* type, const char* text);
	// table facture_reel
	std::list<mss> get_facture_reel(const std::string& version, int item_id);
	std::list<mss> get_facture_reel(const char* version, int item_id);
	// max version
	std::string get_max_version();

	// table mission
	mss get_mission(const std::string& version, int mission_id);
	mss get_mission(const char* version, int mission_id);

	// table mission_monster
	std::list<mss> get_mission_teamid_by_monsterid(const std::string& version, int monster_id);
	std::list<mss> get_mission_teamid_by_monsterid(const char* version, int monster_id);

	// table quest
	mss get_quest(const std::string& version, int quest_id);
	mss get_quest(const char* version, int quest_id);

private:
	static int callback(void* p, int argc, char** argv, char** azColName);

	sqlite3* db;
};

extern database db;

#endif /* DATABASE_H_ */#pragma once
