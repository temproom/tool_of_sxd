#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

#include <D:\boost_1_78_0\boost_1_78_0\boost/regex.hpp>
#include <D:\boost_1_78_0\boost_1_78_0\boost/algorithm/string.hpp>
#include <D:\boost_1_78_0\boost_1_78_0\boost/algorithm/string/regex.hpp>
#include <D:\boost_1_78_0\boost_1_78_0\boost/lexical_cast.hpp>

#include <D:\boost_1_78_0\boost_1_78_0\boost/format.hpp>

#include "database.h"
#include "sxd_web.h"
#include "sxd_client.h"
#include "common.h"
#include "protocol.h"
#include "sxd.h"

sxd::sxd() {
}

sxd::~sxd() {
}

void sxd::run(std::string arg, bool auto_exit) {
	//ɾ������ǰ����־�ļ�
	system(("del /q /f log\\" + common::to_string(std::time(0) - 3 * 24 * 60 * 60, "%Y-%m-%d*")).c_str());
	std::string user_ini = common::read_file("D:\\����\\sxd����\\user.ini");
	boost::regex user_regex("userid=(.*?)\r\nurl=(.*?)\ncode=(.*?)\r\ntime1=(.*?)\r\nhash1=(.*?)\r\ntime=(.*?)\r\nhash=(.*?)\r\nversion=(.*?)\r\nname=(.*?)\r\n");

	if (arg == "menu") {
		int i = 0;
		for (auto it = boost::sregex_iterator(user_ini.begin(), user_ini.end(), user_regex); it != boost::sregex_iterator(); it++)
		{
			common::log(boost::str(boost::format("%3d. %s") % (++i) % (*it)[9]), -1, 0, 0);
		}
		common::log("��ѡ��(����0��ʾ��������)��", -1, 0, 0);
		std::getline(std::cin, arg);
		sxd::run(arg, auto_exit);
		return;
	}
	else if (arg == "login") {
		sxd::login();
	}
	else if (arg == "analyze") {
		sxd::analyze();
	}
	else if (arg == "collect") {
		sxd::collect();
	}
	else {
		int index;
		try {
			index = std::stoi(arg);
		}
		catch (const std::exception& ex) {
			common::log("��Ч����", -1, 0, 0);
			sxd::run("menu", auto_exit);
			return;
		}
		int i = 0;
		for (auto it = boost::sregex_iterator(user_ini.begin(), user_ini.end(), user_regex); it != boost::sregex_iterator(); it++) {
			try {
				if (index && index != ++i)
					continue;
				common::log("", -1, 1, 0);
				common::log(boost::str(boost::format("��%1%����ʼ...") % (*it)[9]));
				std::ostringstream oss;
				oss << "Cookie: user=" << (*it)[3] << ";";
				oss << "_time=" << (*it)[6] << ";_hash=" << (*it)[7] << ";";
				oss << "login_time_sxd_xxxxxxxx=" << (*it)[4] << ";login_hash_sxd_xxxxxxxx=" << (*it)[5] << "\r\n";
				std::string user_id = (*it)[1];
				std::string url = (*it)[2];
				std::string version = (*it)[8];
				std::string max_version = db.get_max_version();
				std::cout << "��ǰ�汾:" << max_version.c_str() << max_version.length()<< "\n" << "���°汾:" << version.c_str() << version.length()<< "\n";
				if (version != max_version) {
					common::log(common::sprintf("��ǰ�汾 [%s]�����°汾 [%s]���뼰ʱ���°汾", version.c_str(), max_version.c_str()));
					version = max_version;
				}
				std::string cookie = oss.str();
				//sxd::batch_fate(version, user_id, url, cookie);
				sxd::auto_play(version, user_id, url, cookie);
				//std::thread thread([url, cookie]() {sxd::play(version, url, cookie);});
				}
				catch (const std::exception& ex) {
					common::log(boost::str(boost::format("���ִ���(run)��%1%") % ex.what()));
				}
		}
	}
	if (!auto_exit)
		sxd::run("menu", auto_exit);
}

void sxd::login() {
		std::string response_http_version;
		unsigned int status_code;
		std::string status_message;
		std::string response_header;
		std::string response_body;

		auto users = db.get_all_users();
		for (auto& user : users) {
			try {
				common::log(boost::str(boost::format("%1% [%2%]") % user["name"] % user["id"]));
				std::string id = user["id"];
				std::string host1 = user["host"];
				std::string port1 = user["port"];
				std::string url1 = user["url"];
				std::string cookie1 = user["cookie"];

				sxd_web web1;
				web1.connect(host1, port1);
				web1.service("GET", url1, "HTTP/1.1", "Cookie: " + cookie1 + "\r\n", "", response_http_version, status_code, status_message, response_header, response_body);
				boost::regex location_regex("Location: https?://(.*?)[?](.*?)\r\n");
				boost::smatch match;
				std::cout << boost::str(boost::format("response_header��%1%") % response_header);
				//if (!regex_search(response_header, match, location_regex))
					//throw std::runtime_error("[Location]ƥ�����");

				//std::string host2 = match[1];
				std::string host2 = "wan.360.cn";
				std::string port2("80");
				//std::string url2("/" + match[2]);
				std::string url2("/game/sxd");

				sxd_web web2;
				web2.connect(host2, port2);
				web2.service("GET", url2, "HTTP/1.1", "", "", response_http_version, status_code, status_message, response_header, response_body);
				std::list<std::string> cookies;
				boost::regex set_cookie_regex("Set-Cookie: (.*?);");
				
				std::cout << boost::str(boost::format("response_header��%1%") % response_header);

				if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: user=(.*?);")))
					throw std::runtime_error("[Set-Cookie]ƥ�����");
				std::string code = match[1];
				if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: _time=(.*?);")))
					throw std::runtime_error("[Set-Cookie]ƥ�����");
				std::string time = match[1];
				if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: _hash=(.*?);")))
					throw std::runtime_error("[Set-Cookie]ƥ�����");
				std::string hash = match[1];
				if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: login_time_sxd_xxxxxxxx=(.*?);")))
					throw std::runtime_error("[Set-Cookie]ƥ�����");
				std::string time1 = match[1];
				if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: login_hash_sxd_xxxxxxxx=(.*?);")))
					throw std::runtime_error("[Set-Cookie]ƥ�����");
				std::string hash1 = match[1];

				std::string user_ini = common::read_file("user.ini");
				boost::regex user_regex("\\[" + id + "\\]\r\nurl=(?<url>.*?)\r\ncode=(?<user>.*?)\r\ntime=(?<_time>.*?)\r\nhash=(?<_hash>.*?)\r\ntime1=(?<login_time_sxd_xxxxxxxx>.*?)\r\nhash1=(?<login_hash_sxd_xxxxxxxx>.*?)\r\nname=(?<name>.*?)\r\n");
				std::string user_replace("[" + id + "]\r\nurl=$1\r\ncode=" + code + "\r\ntime=" + time + "\r\nhash=" + hash + "\r\ntime1=" + time1 + "\r\nhash1=" + hash1 + "\r\nname=$7\r\n");
				user_ini = boost::regex_replace(user_ini, user_regex, user_replace);
				common::write_file("user.ini", user_ini);
			}
			catch (const std::exception& ex) {
				common::log(boost::str(boost::format("���ִ���(login)��%1%") % ex.what()));
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		} 
	}

void sxd::analyze() {
	// read file to be analyzed
	std::cout << "�����������ļ�����" << std::endl << std::flush;
	std::string file_name;
	std::cin >> file_name;
	common::log(boost::str(boost::format("���������ļ���%1%") % file_name));
	std::string str = common::read_file(file_name);
	// read max version
	std::string version = db.get_max_version();
	// split items and loop
	std::vector<std::string> times;
	for (auto it = boost::sregex_iterator(str.begin(), str.end(), boost::regex("\r\n(?<time>\\[\\d{4}.*?\\])\r\n"), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++)
		times.push_back((*it)[1]);
	std::vector<std::string> blocks;
	boost::algorithm::split_regex(blocks, str, boost::regex("\r\n(?<time>\\[\\d{4}.*?\\])\r\n"));

	for (unsigned i = 1; i < blocks.size(); i++) {
		std::stringstream ss;
		int block_size = 0;
		auto block = blocks[i];
		common::log(times[i - 1]);

		// supply ss line by line
		std::vector<std::string> lines;
		boost::algorithm::split_regex(lines, block, boost::regex("\r\n"));
		for (auto& line : lines) {
			if (line.size() == 0)
				continue;
			line = line.substr(10, 49);
			for (auto it = boost::sregex_iterator(line.begin(), line.end(), boost::regex("([0-9A-F]{2}) "), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++) {
				std::istringstream is((*it)[1]);
				int temp;
				is >> std::hex >> temp;
				common::write_int8(ss, temp);
				block_size++;
			}
		}

		// analyze a block with frame by frame
		int size = 0;
		while (size < block_size) {
			// read frame size
			int frame_size = common::read_int32(ss);
			size += frame_size + 4;
			// read module and action
			short module = common::read_int16(ss);
			short action = common::read_int16(ss);
			// protocol and pattern
			mss protocol;
			Json::Value pattern;
			Json::Value data;
			if (module == 0x789C) {
				// stream to memory
				std::ostringstream oss;
				common::write_int16(oss, module);
				common::write_int16(oss, action);
				oss << common::read_string(ss, frame_size - 4);
				std::string str = oss.str();
				// decompress
				boost::iostreams::array_source as(str.c_str(), str.size());
				boost::iostreams::filtering_istream fis;
				fis.push(boost::iostreams::zlib_decompressor());
				fis.push(as);
				// re-read module and action
				module = common::read_int16(fis);
				action = common::read_int16(fis);

				// get response pattern from database corresponding to module and action
				protocol = db.get_protocol(version.c_str(), module, action);
				std::istringstream(protocol[i % 2 ? "request" : "response"]) >> pattern;
				// decode frame
				protocol::decode_frame(fis, data, pattern);
			}
			else {
				// get response pattern from database corresponding to module and action
				protocol = db.get_protocol(version.c_str(), module, action);
				std::istringstream(protocol[i % 2 ? "request" : "response"]) >> pattern;
				// decode frame
				protocol::decode_frame(ss, data, pattern);
				if ((i % 2) && (module != 0 || action != 0) && (module != 94 || action != 0) && (module != 293 || action != 0) && (module != 336 || action != 12))
					common::read_int32(ss);
			}
			// log
			common::log(boost::str(boost::format("  %1% method:  %2%.%3%(%4%,%5%)") % (i % 2 ? "   Send" : "Receive") % protocol["class"] % protocol["method"] % protocol["module"] % protocol["action"]));
			//common::log(boost::str(boost::format("  %1% pattern: %2%") % (i % 2 ? "   Send" : "Receive") % protocol[i % 2 ? "request" : "response"]));
			common::log(boost::str(boost::format("  %1% data:    %2%") % (i % 2 ? "   Send" : "Receive") % boost::regex_replace(data.toStyledString(), boost::regex("[\n\t]+"), " ")));
		}
	} // for items
}

void sxd::batch_fate(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie) {
	// initialize town clients
	sxd_client sxd_client_town(version);
	Json::Value data;

	// get web page from url and cookie
	std::string web_page;
	{
		boost::smatch match;
		if (!regex_search(url, match, boost::regex("http://(.*?)(:\\d*)?/(.*)")))
			throw std::runtime_error("[url] ƥ�����");
		std::string host(match[1]);
		std::string port(match[2]);
		port = port.size() ? port.substr(1) : "80";
		sxd_web web;
		web.connect(host, port);
		web_page = web.get(url, cookie);
	}
	// login town
	if (sxd_client_town.login_town(web_page))
		return;

	sxd_client_town.batch_fate(INT_MAX);

}

void sxd::auto_play(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie) {

	/*Json::Value config;
	std::istringstream("[\"\u60c5\u7fa9\u4e4b\u5dd4\u00b0\u27aa.s156\"]") >> config;
	std::string u = config[0].asString();
	std::string g = common::utf2gbk(u);
	common::log(u, 0);
	common::log(g, 0);
	exit(1);*/

	// initialize four clients
	sxd_client sxd_client_town(version);
	sxd_client sxd_client_super_town(version);
	sxd_client sxd_client_saint_area(version);
	sxd_client sxd_client_chat_room(version);
	Json::Value data;

	// get web page from url and cookie
	std::string web_page;
	{
		boost::smatch match;
		if (!regex_search(url, match, boost::regex("https?://([^/]+)/")))
			throw std::runtime_error("[url] ƥ�����");
		std::string host(match[1]);
		std::string port("80");
		sxd_web web;
		web.connect(host, port);
		web_page = web.get(url, cookie);
	}

	// login town
	if (sxd_client_town.login_town(web_page))
		return;

	// bug
	if (0)
		sxd_client_town.bug();

	// get player functions
	std::vector<std::string> function_names;
	std::vector<std::string> function_ids;
	{
		data = sxd_client_town.Mod_Player_Base_get_player_function();
		common::log(boost::str(boost::format("����¼������ѿ��� [%1%] ���") % data[0].size()), 0);
		for (const auto& item : data[0]) {
			try {
				function_ids.push_back(item[0].asString());
				std::string function_name = db.get_code(version, "Function", item[0].asInt())["text"];
				common::log(boost::str(boost::format("��Function��[%1%(%2%)]") % function_name % item[0]), 0);
				function_names.push_back(function_name);
			}
			catch (const std::exception& ex) {
				common::log(boost::str(boost::format("���ִ���(function)��%1%") % ex.what()));
			}
		}
	}
	//�Զ�����
	std::cout << "�Զ�����\n";
	sxd_client_town.auto_quest();





	/*for (int i = 0;i < function_names.size();i++)
		std::cout << function_names[i] << "\n";*/
	/*
	// gift
	sxd_client_town.gift3();                                    // ��ʯ, ٺ», ����
	sxd_client_town.function_end();                             // ������
	sxd_client_town.gift();                                     // �������
	sxd_client_town.Mod_HeroesWar_Base_get_end_gift();          // ��Ӫս���
	sxd_client_town.Mod_StChallenge_Base_get_end_li_bao();      // �Զ�����ս���
	sxd_client_town.Mod_UnlimitChallenge_Base_get_end_award();  // ������ս����
																// lucky shop
	if (!common::contain(function_names, "��������"))
		common::log("���������ˡ�δ����", 0);
	else {
		sxd_client_town.item_sell();                            // ��Ʒ����
		sxd_client_town.lucky_shop();                           // ��������
		sxd_client_town.black_shop();                           // �����챦
		sxd_client_town.item_reel();                            // ����ϳ�
		sxd_client_town.item_use();                             // ��Ʒʹ��
	}
	// release welfare�����¸���
	sxd_client_town.release_welfare();

	// equipment
	if (!common::contain(function_names, "ǿ��"))
		common::log("��ǿ����δ����", 0);
	else {
		sxd_client_town.equipment();
	}

	// library
	if (!common::contain(function_names, "�ؾ���"))
		common::log("���ؾ���δ����", 0);
	else {
		sxd_client_town.library();
	}

	// research ����
	if (!common::contain(function_names, "����"))
		common::log("��������δ����", 0);
	else {
		sxd_client_town.research();
	}

	// rune �в����
	if (!common::contain(function_names, "�в����"))
		common::log("���в������δ����", 0);
	else {
		sxd_client_town.rune();
	}

	// get peach
	if (!common::contain(function_names, "ժ����"))
		common::log("��ժ���ҡ�δ����", 0);
	else {
		sxd_client_town.get_peach();
	}

	// farm
	if (!common::contain(function_names, "ҩ԰"))
		common::log("��ҩ԰��δ����", 0);
	else {
		sxd_client_town.harvest();
		if (!common::contain(function_names, "������"))
			common::log("��ҩ԰��δ���� [������]", 0);
		else {
			sxd_client_town.plant();
			sxd_client_town.harvest();
		}
	}

	// ����
	if (!common::contain(function_names, "����"))
		common::log("��������δ����", 0);
	else {
		// sign in��ǩ��
		sxd_client_town.sign_in();
		// wager shop�����ֳǶķ�
		sxd_client_town.wager_shop();
		// hide treasure map���ر�ͼ
		sxd_client_town.hide_treasure_map();
		// dice messenger������ʹ��
		sxd_client_town.dice_messenger();
		// dunhuang treasure���ػ��ر�
		sxd_client_town.dunhuang_treasure();
		// spring carnival���´����껪
		sxd_client_town.spring_carnival();
		// regression���ع�
		sxd_client_town.regression();
		// find treasure������̽��
		sxd_client_town.find_treasure();
	}

	// link fate
	if (!common::contain(function_names, "��Ե"))
		common::log("����Ե��δ����", 0);
	else {
		sxd_client_town.link_fate();
	}

	// training
	if (!common::contain(function_names, "����"))
		common::log("��������δ����", 0);
	else {
		sxd_client_town.training();
	}

	// chaos equipment
	if (!common::contain(function_names, "�������"))
		common::log("��������ա�δ����", 0);
	else {
		sxd_client_town.space_find();           // �������
		sxd_client_town.chaos_equipment();      // ��������
	}

	// email
	if (!common::contain(function_names, "����"))
		common::log("�����䡿δ����", 0);
	else {
		sxd_client_town.email();
	}

	// courtyard pet
	if (!common::contain(function_names, "����"))
		common::log("�����δ����", 0);
	else {
		sxd_client_town.courtyard_pet();        // ���﷿
		sxd_client_town.courtyard_pet_quest();  // ��������
	}

	// pot world
	if (!common::contain(function_names, "���н�"))
		common::log("�����н硿δ����", 0);
	else {
		sxd_client_town.pot_world();
	}

	// beelzebub trials
	if (!common::contain(function_names, "ħ������"))
		common::log("��ħ��������δ����", 0);
	else {
		sxd_client_town.beelzebub_trials();
	}

	// pet animal
	if (!common::contain(function_names, "Ҷ������"))
		common::log("��Ҷ��������δ����", 0);
	else {
		sxd_client_town.pet_animal();
	}

	// travel event
	if (!common::contain(function_names, "������Ե"))
		common::log("��������Ե��δ����", 0);
	else {
		sxd_client_town.travel_event();
	}

	// hunt demon
	if (!common::contain(function_names, "����"))
		common::log("��������δ����", 0);
	else {
		sxd_client_town.hunt_demon();
	}

	// awake
	if (!common::contain(function_names, "����"))
		common::log("�����ѡ�δ����", 0);
	else {
		sxd_client_town.awake();
	}

	// bai lian qian kun
	if (!common::contain(function_names, "����Ǭ��"))
		common::log("������Ǭ����δ����", 0);
	else {
		sxd_client_town.bai_lian_qian_kun();
	}

	// five elements laba
	if (!common::contain(function_names, "��������"))
		common::log("���������ǡ�δ����", 0);
	else {
		sxd_client_town.five_elements_laba();
	}

	// roll cake
	if (!common::contain(function_names, "���Ǹ���"))
		common::log("�����Ǹ��ա�δ����", 0);
	else {
		sxd_client_town.roll_cake();
	}

	// send flower
	if (!common::contain(function_names, "�ͻ�"))
		common::log("���ͻ���δ����", 0);
	else {
		sxd_client_town.send_flower();
	}

	// fate
	if (!common::contain(function_names, "����"))
		common::log("��������δ����", 0);
	else {
		sxd_client_town.fate();
	}

	// find immortal
	if (!common::contain(function_names, "ϲ���콵"))
		common::log("��ϲ���콵��δ����", 0);
	else {
		sxd_client_town.find_immortal();      // ϲ���콵
		sxd_client_town.find_immortal2();     // �帣����
	}

	// rob money
	if (!common::contain(function_names, "����"))
		common::log("�����ڡ�δ����", 0);
	else {
		sxd_client_town.rob_money();
	}

	// nine regions���ſ��޽�
	sxd_client_town.nine_regions();

	// dance
	if (!common::contain(function_names, "�赸����"))
		common::log("��Ⱥ�����衿δ����", 0);
	else {
		sxd_client_town.dance();
	}

	// marry
	if (!common::contain(function_names, "���"))
		common::log("����顿δ����", 0);
	else {
		sxd_client_town.marry();
	}

	// coin mission
	if (!common::contain(function_names, "ͭǮ����"))
		common::log("��ͭǮ������δ����", 0);
	else {
		sxd_client_town.coin_mission();
	}

	// hero mission
	if (!common::contain(function_names, "Ӣ��ɨ��"))
		common::log("��Ӣ��ɨ����δ����", 0);
	else {
		sxd_client_town.hero_mission();
	}

	// lucky super number�����˴��ƴ
	sxd_client_town.lucky_super_number();

	// sunday fruit����ĩˮ����
	sxd_client_town.sunday_fruit();

	// partner link
	if (!common::contain(function_names, "��Եϵͳ"))
		common::log("����Եϵͳ��δ����", 0);
	else {
		sxd_client_town.partner_link();     // Ե�걦��
	}

	// dragonball
	if (!common::contain(function_names, "�����Ϲ�"))
		common::log("�������Ϲ���δ����", 0);
	else {
		sxd_client_town.dragonball();
	}

	// npc friendship
	if (!common::contain(function_names, "NPC�ύ"))
		common::log("��NPC�ύ��δ����", 0);
	else {
		//sxd_client_town.npc_friendship();
	}

	// tower
	if (!common::contain(function_names, "����"))
		common::log("�������ֻء�δ����", 0);
	else {
		sxd_client_town.tower();
	}

	// faction
	if (!common::contain(function_names, "����"))
		common::log("�����ɡ�δ����", 0);
	else {
		std::string faction_name = sxd_client_town.get_faction_name();
		if (faction_name.size() == 0)
			common::log("�����ɡ�δ�������", 0);
		else {
			sxd_client_town.faction_god();              // ���ɼ���
			sxd_client_town.seal_satan();               // ���Ƿ�ħ
			//sxd_client_town.faction_roll_cake();        // ���ɼ��Ǹ���
			sxd_client_town.faction_lucky_wheel();      // ����ת��
			sxd_client_town.faction_join_feast();       // ������
			sxd_client_town.faction_approve();          // ���
			sxd_client_town.faction_war();              // ����ս
		}
	}

	// assistant
	if (!common::contain(function_names, "��Ծ��"))
		common::log("����Ծ�ȡ�δ����", 0);
	else {
		sxd_client_town.assistant();
	}

	// fish
	if (!common::contain(function_names, "����"))
		common::log("�����㡿δ����", 0);
	else {
		sxd_client_town.fish();
	}

	// super town
	try {
		if (!common::contain(function_names, "�ɽ�"))
			common::log("���ɽ硿δ����", 0);
		else if (!sxd_client_super_town.login_super_town(&sxd_client_town)) {

			// st_union
			if (!common::contain(function_names, "����"))
				common::log("�����ˡ�δ����", 0);
			else {
				std::string st_union_name = sxd_client_super_town.get_st_union_name();
				if (st_union_name.size() == 0)
					common::log("�����ˡ�δ��������", 0);
				else {
					common::log(boost::str(boost::format("�����ˡ��������� [%1%]") % st_union_name), 0);
					sxd_client_super_town.st_union_god_incense();       // ��������
					sxd_client_super_town.st_union_activity();          // ����֮��
					sxd_client_super_town.st_union_task();              // ħ����ս
					sxd_client_super_town.st_union_approve();           // �������
					sxd_client_super_town.st_union_nimal();             // ��������
				}
			}

			// wish pool
			if (!common::contain(function_names, "��Ը��"))
				common::log("����Ը�ء�δ����", 0);
			else {
				sxd_client_super_town.wish_pool();
			}

			// furniture effect
			if (!common::contain(function_names, "��԰"))
				common::log("����԰��δ����", 0);
			else {
				sxd_client_super_town.furniture_effect();
			}

			// st arena
			if (!common::contain(function_names, "�ɽ��̵�"))
				common::log("���ɽ��̵꡿δ����", 0);
			else {
				sxd_client_super_town.st_daoyuan_shop();
			}

			// st big turntable
			if (!common::contain(function_names, "����"))
				common::log("��������δ����", 0);
			else {
				sxd_client_super_town.st_big_turntable();           // ���ֳǴ�ת��
			}

			// st altar
			if (!common::contain(function_names, "�ɽ���̳"))
				common::log("���ɽ���̳��δ����", 0);
			else {
				sxd_client_super_town.st_altar();
			}

			// st mine
			if (!common::contain(function_names, "��ɽϵͳ"))
				common::log("����ɽϵͳ��δ����", 0);
			else {
				sxd_client_super_town.st_mine();
			}

			// st practice room
			if (!common::contain(function_names, "�ɽ�������"))
				common::log("���ɽ���������δ����", 0);
			else {
				if (common::contain(function_names, "���Ƿ���"))
					common::log("���ɽ��������������ѳ�ʥ������Ϊ [ʥ��������]", 0);
				else {
					sxd_client_super_town.st_practice_room();
				}
			}

			// st arena
			if (!common::contain(function_ids, "132"))
				common::log("���ɽ羺������δ����", 0);
			else {
				sxd_client_super_town.st_arena();           // ��ս
				sxd_client_town.exploit_shop();             // �����̵����ڵ�
			}

			// st take bible
			if (!common::contain(function_names, "���ȡ��"))
				common::log("���ɽ�ȡ����δ����", 0);
			else {
				if (common::contain(function_names, "���Ƿ���"))
					common::log("���ɽ�ȡ���������ѳ�ʥ������Ϊ [ʥ��ȡ��]", 0);
				else {
					sxd_client_super_town.st_take_bible();                      // ����ȡ��
					sxd_client_super_town.st_rob_bible(&sxd_client_town);       // ����ȡ��
				}
			}

			// st super sport
			if (!common::contain(function_ids, "93"))
				common::log("����ħ������δ����", 0);
			else {
				if (common::contain(function_names, "���Ƿ���"))
					common::log("����ħ�����������ѳ�ʥ������Ϊ [ʥ�򾺼���]", 0);
				else {
					sxd_client_super_town.point_race(&sxd_client_town);         // ������
					sxd_client_super_town.war_race(&sxd_client_town);           // ��ħ��ս
					sxd_client_super_town.get_rank_award(&sxd_client_town);     // ��������
					sxd_client_super_town.get_score_award();                    // ��ħ����
				}
			}
		}
	}
	catch (const std::exception& ex) {
		common::log(boost::str(boost::format("���ִ���(super town)��%1%") % ex.what()));
	}

	// saint area
	try {
		if (!common::contain(function_names, "ʥ��"))
			common::log("��ʥ��δ����", 0);
		else if (!sxd_client_saint_area.login_saint_area(&sxd_client_town)) {
			sxd_client_saint_area.sa_take_bible();                  // ʥ��ȡ��
			sxd_client_saint_area.sa_super_sport();                 // ʥ�򾺼���
		}
	}
	catch (const std::exception& ex) {
		common::log(boost::str(boost::format("���ִ���(saint area)��%1%") % ex.what()));
	}

	// server chat room
	try {
		if (!common::contain(function_names, "������"))
			common::log("��ȫ�����졿δ����", 0);
		else if (!sxd_client_chat_room.login_server_chat(&sxd_client_town)) {
			sxd_client_chat_room.pet_escort(&sxd_client_town);
		}
	}
	catch (const std::exception& ex) {
		common::log(boost::str(boost::format("���ִ���(server chat room)��%1%") % ex.what()));
	}

	// super sport
	if (!common::contain(function_names, "������"))
		common::log("����������δ����", 0);
	else {
		sxd_client_town.super_sport();
	}
	std::cout << "����ִ��\n";
	*/
}

void sxd::collect() {
	try {
		std::cout << "������汾�ţ�" << std::endl << std::flush;
		std::string version;
		std::cin >> version;
		//sxd::collect_protocol(version, "d:\\����\\Э��\\" + version + "\\com\\protocols");
		//sxd::collect_end_function_gift(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\gifttypedata.as");
		//sxd::collect_function(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\functiontypedata.as");
		//sxd::collect_gift(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\gifttypedata.as");
		//sxd::collect_item(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\itemtypedata.as");
		//sxd::collect_lucky_shop_item(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\itemtypedata.as");
		//sxd::collect_role(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\roletype.as");
		//sxd::collect_town(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\source\\towntypedata.as");
		//sxd::collect_facture_reel(version, "d:\\����\\Э��\\" + version + "\\com\\assist\\server\\facturereeltype.as");
		//sxd::collect_mission_monster(version, "d:\\����\\Э��\\missionmonstertypedata.as");
		//sxd::collect_mission_team(version, "d:\\����\\Э��\\missionmonstertypedata.as");
		//sxd::collect_quest(version, "d:\\����\\Э��\\questtypedata.as");
		//sxd::collect_mission(version, "d:\\����\\Э��\\missiontypedata.as");
		
	}
	catch (const std::exception& ex) {
		std::cerr << boost::str(boost::format("���ִ���(collect)��%1%") % ex.what()) << std::endl;
	}
}

void sxd::collect_end_function_gift(const std::string& version, const std::string& path) {
	std::string type = "EndFunctionGift";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("endFunctionGift.*?\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?), \"(.*?)\", \"(.*?)\".*?\\]");
	boost::regex regex2("\\[(\\d+?),\"(.*?)\",\"(.*?)\".*?\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_function(const std::string& version, const std::string& path) {
	std::string type = "Function";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("List:Object = .*?;");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\".*?\",\"(.*?)\",\"(.*?)\"\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_gift(const std::string& version, const std::string& path) {
	std::string type = "Gift";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("GiftTypes.*?\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?), \"(.*?)\", \"(.*?)\".*?\\]");
	boost::regex regex2("\\[(\\d+?),\"(.*?)\",\"(.*?)\".*?\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_item(const std::string& version, const std::string& path) {
	std::string type = "Item";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Items:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\\d*?,\".*?\",\"(.*?)\".*?]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[2] % "" % match[3];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_lucky_shop_item(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	db.execute("DELETE FROM lucky_shop_item where version='" + version + "'");
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("AryShopItem:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(?<id>\\d+?):\\[(?<item_id>\\d*?),(?<count>\\d*?),(?<ingot>\\d*?),(?<coin>\\d*?)\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO lucky_shop_item(version, id, item_id, count, ingot, coin) VALUES('%1%', %2%, %3%, %4%, %5%, %6%)");
		sql % version % match["id"] % match["item_id"] % match["count"] % match["ingot"] % match["coin"];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str());
		content = match.suffix();
	}
	db.execute("COMMIT");

}

void sxd::collect_role(const std::string& version, const std::string& path) {
	std::string type = "Role";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Roles:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[(.*?),\"(.*?)\",.*?]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_town(const std::string& version, const std::string& path) {
	std::string type = "Town";
	db.execute("BEGIN");
	db.execute(("DELETE FROM code where version='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Towns:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\"(.*?)\",\\{(.*?)\\},(\\d+?),\"(.*?)\",.*?\r\n");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[6] % match[2] % match[3];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_facture_reel(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	db.execute("DELETE FROM facture_reel where version='" + version + "'");
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("_factureReelList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d*?),(\\d*?),(\\d*?),\"(.*?)\",(\\d*?)\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO facture_reel(version, reel_id, item_id, count, position, mission_id) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % match[1] % match[2] % match[3] % match[4] % match[5];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_protocol(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	db.execute(("DELETE FROM protocol where version='" + version + "'").c_str());

	boost::filesystem::directory_iterator ite;
	for (boost::filesystem::directory_iterator it(path); it != ite; ++it) {
		std::string file = it->path().string();
		std::string filename = it->path().filename().string();
		if (!boost::regex_match(filename, boost::regex("Mod_.*_Base\\.as")))
			continue;

		std::string content = common::read_file(file);
		boost::smatch match;
		//boost::regex_search(content, match, boost::regex("public class (?<class>.*?) extends Object"));
		boost::regex_search(content, match, boost::regex("public class (?<class>.*?)\r\n"));
		common::log(boost::str(boost::format("class: %1%") % match["class"]));

		//INSERT INTO protocol(version, module, class, action, method, request, response) VALUES('2022042801',563,'Mod_WishTemple_Base',2,'panel','[]','[]')
		//for (auto it = boost::sregex_iterator(content.begin(), content.end(), boost::regex("public static const (?<method>.*?):Object = {module:(?<module>\\d*?), action:(?<action>\\d*?), request:(?<request>.*?), response:(?<response>.*?)};"), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++) {
		for (auto it = boost::sregex_iterator(content.begin(), content.end(), boost::regex("public static const (?<method>.*?):Object = {\\s*\"module\":(?<module>\\d*?),\\s*\"action\":(?<action>\\d*?),\\s*\"request\":(?<request>.*?),\\s*\"response\":(?<response>.*?)\\s*};"), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++) {
			std::ostringstream sql;
			sql << "INSERT INTO protocol(version, module, class, action, method, request, response) VALUES(";
			sql << "'" << version << "', ";
			sql << (*it)["module"] << ", ";
			sql << "'" << match["class"] << "', ";
			sql << (*it)["action"] << ", ";
			sql << "'" << (*it)["method"] << "', ";
			sql << "'" << boost::regex_replace(std::string((*it)["request"]), boost::regex("Utils(.*?)Util"), "\"$&\"") << "', ";
			sql << "'" << boost::regex_replace(std::string((*it)["response"]), boost::regex("Utils(.*?)Util"), "\"$&\"") << "'";
			sql << ")";
			db.execute(sql.str().c_str());
		}
	}
	db.execute("COMMIT");
} //sxd::collect_protocols

void sxd::collect_mission_monster(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	//db.execute(("DELETE FROM code where version='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("missionMonsterTeamList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO mission_monster(version, team_id, scene_id, monster_id) VALUES('%1%', '%2%', '%3%', '%4%')");
		sql % version % match[1] % match[2] % match[4];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_mission(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	//db.execute(("DELETE FROM code where version='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Missions:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",(\\d+?),(\\d+?),(\\d+?)\\],\r\n");

	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO mission(version, mission_id, scene_id, boss_id, mission_name, hero_mission, boss_mission) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%', '%7%')");
		sql % version % match[1] % match[2] % match[8] % match[9] % match[10] % match[11];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_quest(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	//db.execute(("DELETE FROM code where version='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Quests:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	boost::regex regex2("(\\d+?):\\[(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?)\\],\r\n");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO quest(version, quest_id, quest_type, level, quest_name, mission_id) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % match[1] % match[2] % match[4] % match[5] % match[18];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}

void sxd::collect_mission_team(const std::string& version, const std::string& path) {
	db.execute("BEGIN");
	//db.execute(("DELETE FROM code where version='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("missionMonsterList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	while (boost::regex_search(content, match, regex2)) {
		auto sql = boost::format("INSERT INTO mission_team(version, _id, team_id, monster_id) VALUES('%1%', '%2%', '%3%', '%4%')");
		sql % version % match[1] % match[2] % match[3];
		common::log(common::utf2gbk(sql.str()));
		db.execute(sql.str().c_str());
		content = match.suffix();
	}
	db.execute("COMMIT");
}
