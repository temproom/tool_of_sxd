#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>

#include <regex.hpp>
#include <algorithm/string.hpp>
#include <algorithm/string/regex.hpp>
#include <lexical_cast.hpp>

#include <format.hpp>

#include "sxd_web.h"
#include "database.h"
#include "sxd_client.h"
#include "common.h"
#include "protocol.h"
#include "sxd.h"
//#include "threadpool/threadpool.h"

std::string player_name = "助手";

sxd::sxd()
{}

sxd::~sxd()
{}

//connection_pool *m_sql_pool;

//void sxd::init(int close_log , int log_write , int sql_num )
//{
//	m_close_log = close_log;
//	m_log_write = log_write;
//	m_sql_num = sql_num;
//}

void sxd::log_write(const std::string& player_name, int close_log, int max_queue_size )
{
	//初始化日志
	Log::get_instance()->init(player_name, close_log, max_queue_size);
}

void sxd::sql_pool()
{
	//m_sql_pool = connection_pool::GetInstance();
	connection_pool::GetInstance()->init(10, 0);
}
//void sxd::thread_pool()
//{
//	m_thread_pool = new threadpool<sxd_client>(m_sql_pool,10, 100);
//}

void sxd::run(std::string arg, bool auto_exit)
{
	//删除三天前的日志文件
	//system(("del /q /f log\\" + common::to_string(std::time(0) - 3 * 24 * 60 * 60, "%Y-%m-%d*")).c_str());

	//在当前文件夹打开user.ini
	std::string user_ini = common::read_file("user.ini");

	//正则表达式读取文件信息，获取cookies
	boost::regex user_regex("\\[(.*?)\\]\r\nurl=(.*?)\ncode=(.*?)\r\ntime1=(.*?)\r\nhash1=(.*?)\r\ntime=(.*?)\r\nhash=(.*?)\r\nname=(.*?)\r\n");

	if (arg == "menu")
	{
		int i = 0;
		for (auto it = boost::sregex_iterator(user_ini.begin(), user_ini.end(), user_regex); it != boost::sregex_iterator(); it++)
		{
			//Log::get_instance()->init((*it)[9], 1, 100);
			//Log::get_instance()->write_log(boost::str(boost::format("%1%. %2%") % (++i) % (*it)[9]), -1, 0, 0)
			common::log(boost::str(boost::format("%1%. %2%") % (++i) % (*it)[8]), -1, 0, 0);
		}
		common::log("请选择(输入0表示运行所有)：", -1, 0, 0);
		std::getline(std::cin, arg);
		sxd::run(arg, auto_exit);
		return;
	}
	else if (arg == "login")
	{
		sxd::login();
	}
	//else if (arg == "analyze")
	//{
	//	sxd::analyze();
	//}
	else if (arg == "collect")
	{
		sxd::collect();
	}
	else
	{
		int index;
		try
		{
			index = std::stoi(arg);
		}
		catch (const std::exception& ex)
		{
			common::log("无效输入", -1, 0, 0);
			sxd::run("menu", auto_exit);
			return;
		}
		int i = 0;
		for (auto it = boost::sregex_iterator(user_ini.begin(), user_ini.end(), user_regex); it != boost::sregex_iterator(); it++)
		{
			try
			{
				if (index && index != ++i)
					continue;
				common::log("", -1, 1, 0);

				player_name = (*it)[8];

				//初始化日志
				sxd::log_write(player_name,0,100);

				//Log::get_instance()->write_log(player_name, " 开始...");
				common::log(boost::str(boost::format("【%1%】开始...") % player_name));

				std::ostringstream oss;
				oss << "Cookie: user=" << (*it)[3] << ";";
				oss << "_time=" << (*it)[6] << ";_hash=" << (*it)[7] << ";";
				oss << "login_time_sxd_xxxxxxxx=" << (*it)[4] << ";login_hash_sxd_xxxxxxxx=" << (*it)[5] << "\r\n";
				std::string user_id = (*it)[1];
				std::string url = (*it)[2];
				std::string version = "2022093001";

				database* m_db = NULL;
				connectionRAII sqlcon(&m_db, connection_pool::GetInstance());
				std::string max_version = m_db->get_max_version();

				//Log::get_instance()->write_log(player_name, boost::str(boost::format("当前版本 [%1%]	最新版本 [%2%]") % version.c_str() % max_version.c_str()));
				std::cout << "当前版本:" << max_version.c_str() << max_version.length() << "\n" << "最新版本:" << version.c_str() << version.length() << "\n";
				if (version != max_version)
				{
					//Log::get_instance()->write_log(player_name, boost::str(boost::format("当前版本 [%1%]，最新版本 [%2%]，请及时更新版本") % version.c_str() % max_version.c_str()));
					common::log(common::sprintf("当前版本 [%s]，最新版本 [%s]，请及时更新版本", version.c_str(), max_version.c_str()));
					version = max_version;
				}
				std::string cookie = oss.str();
				//sxd::batch_fate(version, user_id, url, cookie);

				//std::thread tid(auto_play,(version, user_id, url, cookie));
				sxd::auto_play(version, user_id, url, cookie);
				//std::thread thread([url, cookie]() {sxd::play(version, url, cookie);});
			}
			catch (const std::exception& ex)
			{
				Log::get_instance()->write_log(player_name, boost::str(boost::format("发现错误(run)：%1%") % ex.what()));
				common::log(boost::str(boost::format("发现错误(run)：%1%") % ex.what()));
			}
		}
	}
	if (!auto_exit)
		sxd::run("menu", auto_exit);
}

void sxd::login()
{
	std::string response_http_version;
	unsigned int status_code;
	std::string status_message;
	std::string response_header;
	std::string response_body;

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());
	auto users = m_db->get_all_users();
	for (auto& user : users)
	{
		try
		{
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
			std::cout << boost::str(boost::format("response_header：%1%") % response_header);
			//if (!regex_search(response_header, match, location_regex))
				//throw std::runtime_error("[Location]匹配出错");

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

			std::cout << boost::str(boost::format("response_header：%1%") % response_header);

			if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: user=(.*?);")))
				throw std::runtime_error("[Set-Cookie]匹配出错");
			std::string code = match[1];
			if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: _time=(.*?);")))
				throw std::runtime_error("[Set-Cookie]匹配出错");
			std::string time = match[1];
			if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: _hash=(.*?);")))
				throw std::runtime_error("[Set-Cookie]匹配出错");
			std::string hash = match[1];
			if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: login_time_sxd_xxxxxxxx=(.*?);")))
				throw std::runtime_error("[Set-Cookie]匹配出错");
			std::string time1 = match[1];
			if (!boost::regex_search(response_header, match, boost::regex("Set-Cookie: login_hash_sxd_xxxxxxxx=(.*?);")))
				throw std::runtime_error("[Set-Cookie]匹配出错");
			std::string hash1 = match[1];

			std::string user_ini = common::read_file("user.ini");
			boost::regex user_regex("\\[" + id + "\\]\r\nurl=(?<url>.*?)\r\ncode=(?<user>.*?)\r\ntime=(?<_time>.*?)\r\nhash=(?<_hash>.*?)\r\ntime1=(?<login_time_sxd_xxxxxxxx>.*?)\r\nhash1=(?<login_hash_sxd_xxxxxxxx>.*?)\r\nname=(?<name>.*?)\r\n");
			std::string user_replace("[" + id + "]\r\nurl=$1\r\ncode=" + code + "\r\ntime=" + time + "\r\nhash=" + hash + "\r\ntime1=" + time1 + "\r\nhash1=" + hash1 + "\r\nname=$7\r\n");
			user_ini = boost::regex_replace(user_ini, user_regex, user_replace);
			common::write_file("user.ini", user_ini);
		}
		catch (const std::exception& ex)
		{
			common::log(boost::str(boost::format("发现错误(login)：%1%") % ex.what()));
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void sxd::analyze()
{
	// read file to be analyzed
	std::cout << "请输入数据文件名：" << std::endl << std::flush;
	std::string file_name;
	std::cin >> file_name;
	common::log(boost::str(boost::format("分析数据文件：%1%") % file_name));
	std::string str = common::read_file(file_name);
	// read max version
	std::string version = db.get_max_version();
	// split items and loop
	std::vector<std::string> times;
	for (auto it = boost::sregex_iterator(str.begin(), str.end(), boost::regex("\r\n(?<time>\\[\\d{4}.*?\\])\r\n"), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++)
		times.push_back((*it)[1]);
	std::vector<std::string> blocks;
	boost::algorithm::split_regex(blocks, str, boost::regex("\r\n(?<time>\\[\\d{4}.*?\\])\r\n"));

	for (unsigned i = 1; i < blocks.size(); i++)
	{
		std::stringstream ss;
		int block_size = 0;
		auto block = blocks[i];
		common::log(times[i - 1]);

		// supply ss line by line
		std::vector<std::string> lines;
		boost::algorithm::split_regex(lines, block, boost::regex("\r\n"));
		for (auto& line : lines)
		{
			if (line.size() == 0)
				continue;
			line = line.substr(10, 49);
			for (auto it = boost::sregex_iterator(line.begin(), line.end(), boost::regex("([0-9A-F]{2}) "), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++)
			{
				std::istringstream is((*it)[1]);
				int temp;
				is >> std::hex >> temp;
				common::write_int8(ss, temp);
				block_size++;
			}
		}

		// analyze a block with frame by frame
		int size = 0;
		while (size < block_size)
		{
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
			if (module == 0x789C)
			{
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
			else
			{
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

void sxd::batch_fate(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie)
{
	// initialize town clients
	sxd_client sxd_client_town(version);
	Json::Value data;

	// get web page from url and cookie
	std::string web_page;
	{
		boost::smatch match;
		if (!regex_search(url, match, boost::regex("http://(.*?)(:\\d*)?/(.*)")))
			throw std::runtime_error("[url] 匹配出错");
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

void sxd::auto_play(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie)
{

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
	sxd_client sxd_client_shanhai_world(version);
	sxd_client sxd_client_sect_area(version);			//宗门仙境
	Json::Value data;

	// get web page from url and cookie
	std::string web_page;
	{
		boost::smatch match;
		if (!regex_search(url, match, boost::regex("https?://([^/]+)/")))
			throw std::runtime_error("[url] 匹配出错");
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
		common::log(boost::str(boost::format("【登录】玩家已开启 [%1%] 项功能") % data[0].size()), 0);
		for (const auto& item : data[0])
		{
			try
			{
				function_ids.push_back(item[0].asString());

				database* m_db = NULL;
				connectionRAII sqlcon(&m_db, connection_pool::GetInstance());
				std::string function_name = m_db->get_code(version, "Function", item[0].asInt())["text"];

				common::log(boost::str(boost::format("【Function】[%1%(%2%)]") % function_name % item[0]), 0);
				function_names.push_back(function_name);
			}
			catch (const std::exception& ex)
			{
				common::log(boost::str(boost::format("发现错误(function)：%1%") % ex.what()));
			}
		}
	}


	//自动任务
	//std::cout << "自动任务：\n";
	//sxd_client_town.auto_quest();

	//common::log("【主页】请选择相应的功能" , 0);
	//common::log(boost::str(boost::format("1.日常任务") % data[0].size()), 0);
	for (;;)
	{
		common::log("\n\n\t 1.日常任务\n\t 2.城镇任务\n\t 3.副本挑战\n\t 4.挂机任务\n\t 5.测试\n\t 0.退出\n\t请选择相应的功能：");
		int fun_id;
		std::cin >> fun_id;
		if (fun_id == 1)
		{
			common::log("\n\n\t 1.小分队补充任务\n\t 2.自动任务\n\t 3.周末魔神远征\n\t 4.仙幽幻境\n\t请选择相应的功能：");
			int fen_dui;

			std::cin >> fen_dui;
			if (fen_dui == 1)
			{
				sxd_client_town.ClearClouds();			// 拨云见日，领取免费礼包
				sxd_client_town.WuFaField();			// 悟法领域
				sxd_client_town.PhantomGemLottery();	// 多宝山，道源求宝
				sxd_client_town.MemoryZone();			// 回忆之境，免费一次
				sxd_client_town.MainRoleFaXiang();		// 主角法相，守护魔兽和造物佛手
				sxd_client_town.marry_gift();			// 结婚，亲密度礼物
				sxd_client_town.RaceOrder();			// 赛事战令
				sxd_client_town.SeekImmortalRoad();		// 寻仙之路
				sxd_client_town.StVoid();				// 仙界虚空点赞
				sxd_client_town.Sect();					// 宗门任务	
				sxd_client_town.release_welfare();		// 更新福利
				sxd_client_town.LinlangPavilion();		//琳琅阁
				sxd_client_town.black_shop();			//秘宝商铺奇珍异宝
				sxd_client_town.XiuZhenRealm();			//修真境界				
				//sxd_client_town.Monopoly();			// 山河游历，骰子
				//sxd_client_town.sa_take_bible();      // 圣域取经
				sxd_client_town.TombArtifacts();		// 诸法器冢
				//sxd_client_town.GlazePavilion();		// 琉璃宝阁
				sxd_client_town.SoulHuntBlessGif();		//灵域猎妖-灵域福赠
				sxd_client_town.HeroesBattleScoreRace();//群英战积分赛
				sxd_client_town.CosmosFight();			//寰宇乱斗			
				sxd_client_town.hide_treasure_map();	// hide treasure map，藏宝图
				sxd_client_town.spring_big_run();		//新春大放送

				if (!sxd_client_super_town.login_super_town(&sxd_client_town))
				{
					sxd_client_super_town.st_union_task();              // 魔神挑战
					//sxd_client_super_town.st_arena();					// 仙界竞技场

				}

				// 宠物派遣
				{
					try
					{
						if (!common::contain(function_names, "聊天室"))
							common::log("【全网聊天】未开启", 0);
						else if (!sxd_client_chat_room.login_server_chat(&sxd_client_town))
						{
							sxd_client_chat_room.pet_escort(&sxd_client_town);	// 宠物派遣
						}
					}
					catch (const std::exception& ex)
					{
						common::log(boost::str(boost::format("发现错误(server chat room)：%1%") % ex.what()));
					}
				}

				if (!sxd_client_saint_area.login_saint_area(&sxd_client_town))
				{
					sxd_client_saint_area.sa_take_bible();              // 圣域取经
					sxd_client_saint_area.sa_super_sport();             // 圣域竞技场
				}

				/*{
					if (!sxd_client_sect_area.login_Sect_area(&sxd_client_town))
					{
						sxd_client_sect_area.Sect();
					}

				}*/
			}
			else if (fen_dui == 2)
			{
				//日常任务

				// gift start 礼包
				{
					sxd_client_town.gift3();                                    // 灵石, 俸禄, 仙令
					sxd_client_town.function_end();                             // 随机礼包
					sxd_client_town.gift();                                     // 各种礼包
					sxd_client_town.Mod_HeroesWar_Base_get_end_gift();          // 阵营战礼包
					sxd_client_town.Mod_StChallenge_Base_get_end_li_bao();      // 自定义挑战礼包
					sxd_client_town.Mod_UnlimitChallenge_Base_get_end_award();  // 极限挑战宝箱}
				}

				//lucky shop 神秘商人
				{
					//sxd_client_town.lucky_shop();                           // 神秘商人
					//sxd_client_town.black_shop();                           // 珍奇异宝
				}

				// release welfare，更新福利
				{
					sxd_client_town.release_welfare();
				}

				// get peach 摘仙桃
				{
					if (!common::contain(function_names, "摘仙桃"))
						common::log("【摘仙桃】未开启", 0);
					else
					{
						sxd_client_town.get_peach();
					}
				}

				// farm 药园
				{
					if (!common::contain(function_names, "药园"))
						common::log("【药园】未开启", 0);
					else
					{
						sxd_client_town.harvest();
						if (!common::contain(function_names, "发财树"))
							common::log("【药园】未开启 [发财树]", 0);
						else
						{
							sxd_client_town.plant();
							sxd_client_town.harvest();
						}
					}
				}

				// link fate 结缘
				{
					if (!common::contain(function_names, "结缘"))
						common::log("【结缘】未开启", 0);
					else
					{
						sxd_client_town.link_fate();
					}
				}

				// chaos equipment 混沌虚空
				{
					if (!common::contain(function_names, "混沌虚空"))
						common::log("【混沌虚空】未开启", 0);
					else
					{
						sxd_client_town.space_find();           // 混沌虚空
						sxd_client_town.chaos_equipment();      // 混沌异兽
					}
				}

				// email 邮箱
				{
					if (!common::contain(function_names, "邮箱"))
						common::log("【邮箱】未开启", 0);
					else
					{
						sxd_client_town.email();
					}
				}

				// courtyard pet 宠物
				{
					if (!common::contain(function_names, "宠物"))
						common::log("【宠物】未开启", 0);
					else
					{
						sxd_client_town.courtyard_pet();        // 宠物房
						sxd_client_town.courtyard_pet_quest();  // 宠物任务
					}
				}

				// pot world 壶中界
				{
					if (!common::contain(function_names, "壶中界"))
						common::log("【壶中界】未开启", 0);
					else
					{
						sxd_client_town.pot_world();
					}
				}

				// pet animal 叶公好龙
				{
					if (!common::contain(function_names, "叶公好龙"))
						common::log("【叶公好龙】未开启", 0);
					else
					{
						sxd_client_town.pet_animal();
					}
				}

				// travel event 仙旅奇缘
				{
					if (!common::contain(function_names, "仙旅奇缘"))
						common::log("【仙旅奇缘】未开启", 0);
					else
					{
						sxd_client_town.travel_event();
					}
				}

				// hunt demon 猎妖
				{
					if (!common::contain(function_names, "猎妖"))
						common::log("【猎妖】未开启", 0);
					else
					{
						sxd_client_town.hunt_demon();
					}
				}

				// awake 觉醒
				{
					if (!common::contain(function_names, "觉醒"))
						common::log("【觉醒】未开启", 0);
					else
					{
						sxd_client_town.awake();
					}
				}

				// bai lian qian kun 百炼乾坤
				{
					if (!common::contain(function_names, "百炼乾坤"))
						common::log("【百炼乾坤】未开启", 0);
					else
					{
						sxd_client_town.bai_lian_qian_kun();
					}
				}

				// five elements laba 五行天仪
				{
					if (!common::contain(function_names, "五行天仪"))
						common::log("【五行天仪】未开启", 0);
					else
					{
						sxd_client_town.five_elements_laba();
					}
				}

				// roll cake 吉星高照
				{
					if (!common::contain(function_names, "吉星高照"))
						common::log("【吉星高照】未开启", 0);
					else
					{
						sxd_client_town.roll_cake();
					}
				}

				// send flower 送花
				{
					if (!common::contain(function_names, "送花"))
						common::log("【送花】未开启", 0);
					else
					{
						sxd_client_town.send_flower();
					}
				}

				// fate 猎命
				{
					if (!common::contain(function_names, "猎命"))
						common::log("【猎命】未开启", 0);
					else
					{
						sxd_client_town.fate();
					}
				}

				// find immortal 喜从天降
				{
					if (!common::contain(function_names, "喜从天降"))
						common::log("【喜从天降】未开启", 0);
					else
					{
						sxd_client_town.find_immortal();      // 喜从天降
						sxd_client_town.find_immortal2();     // 五福临门
					}
				}

				// rob money 劫镖
				{
					if (!common::contain(function_names, "劫镖"))
						common::log("【劫镖】未开启", 0);
					else
					{
						sxd_client_town.rob_money();
					}
				}

				// hero mission 英雄扫荡
				{
					if (!common::contain(function_names, "英雄扫荡"))
						common::log("【英雄扫荡】未开启", 0);
					else
					{
						sxd_client_town.hero_mission();
					}
				}

				// sunday fruit，周末水果机
				{
					sxd_client_town.sunday_fruit();
				}

				// partner link 缘魂宝箱
				{
					if (!common::contain(function_names, "天缘系统"))
						common::log("【天缘系统】未开启", 0);
					else
					{
						sxd_client_town.partner_link();     // 缘魂宝箱
					}
				}

				// dragonball 神龙上供
				{
					if (!common::contain(function_names, "神龙上供"))
						common::log("【神龙上供】未开启", 0);
					else
					{
						sxd_client_town.dragonball();
					}
				}

				// tower 爬塔
				{
					if (!common::contain(function_names, "爬塔"))
						common::log("【六道轮回】未开启", 0);
					else
					{
						sxd_client_town.tower();
					}
				}

				// faction 帮派
				{
					if (!common::contain(function_names, "帮派"))
						common::log("【帮派】未开启", 0);
					else
					{
						std::string faction_name = sxd_client_town.get_faction_name();
						if (faction_name.size() == 0)
							common::log("【帮派】未加入帮派", 0);
						else
						{
							sxd_client_town.faction_god();              // 帮派祭神
							sxd_client_town.seal_satan();               // 七星封魔
							//sxd_client_town.faction_roll_cake();      // 帮派吉星高照
							sxd_client_town.faction_lucky_wheel();      // 帮派转盘
							sxd_client_town.faction_join_feast();       // 吃仙宴
							sxd_client_town.faction_approve();          // 审核
							sxd_client_town.faction_war();              // 帮派战
						}
					}
				}

				// assistant 活跃度
				{
					if (!common::contain(function_names, "活跃度"))
						common::log("【活跃度】未开启", 0);
					else
					{
						sxd_client_town.assistant();
					}
				}

				// fish 钓鱼
				{
					if (!common::contain(function_names, "钓鱼"))
						common::log("【钓鱼】未开启", 0);
					else
					{
						sxd_client_town.fish();
					}
				}

				// super town 仙界
				{
					try
					{
						if (!common::contain(function_names, "仙界"))
							common::log("【仙界】未开启", 0);
						else if (!sxd_client_super_town.login_super_town(&sxd_client_town))
						{

							// st_union
							if (!common::contain(function_names, "仙盟"))
								common::log("【仙盟】未开启", 0);
							else
							{
								std::string st_union_name = sxd_client_super_town.get_st_union_name();
								if (st_union_name.size() == 0)
									common::log("【仙盟】未加入仙盟", 0);
								else
								{
									common::log(boost::str(boost::format("【仙盟】进入仙盟 [%1%]") % st_union_name), 0);
									sxd_client_super_town.st_union_god_incense();       // 仙盟上香
									sxd_client_super_town.st_union_activity();          // 仙盟之树
									sxd_client_super_town.st_union_task();              // 魔神挑战
									sxd_client_super_town.st_union_approve();           // 仙盟审核
									sxd_client_super_town.st_union_nimal();             // 仙盟神兽
								}
							}

							// wish pool
							if (!common::contain(function_names, "许愿池"))
								common::log("【许愿池】未开启", 0);
							else
							{
								sxd_client_super_town.wish_pool();
							}

							// furniture effect
							if (!common::contain(function_names, "家园"))
								common::log("【家园】未开启", 0);
							else
							{
								sxd_client_super_town.furniture_effect();
							}

							// st arena
							if (!common::contain(function_names, "仙界商店"))
								common::log("【仙界商店】未开启", 0);
							else
							{
								sxd_client_super_town.st_daoyuan_shop();
							}

							// st big turntable
							if (!common::contain(function_names, "周年活动"))
								common::log("【周年活动】未开启", 0);
							else
							{
								sxd_client_super_town.st_big_turntable();           // 游乐城大转盘
							}

							// st altar
							if (!common::contain(function_names, "仙界神坛"))
								common::log("【仙界神坛】未开启", 0);
							else
							{
								sxd_client_super_town.st_altar();
							}

							// st mine
							if (!common::contain(function_names, "矿山系统"))
								common::log("【矿山系统】未开启", 0);
							else
							{
								sxd_client_super_town.st_mine();
							}

							// st practice room
							if (!common::contain(function_names, "仙界练功房"))
								common::log("【仙界练功房】未开启", 0);
							else
							{
								if (common::contain(function_names, "主角飞升"))
									common::log("【仙界练功房】主角已成圣，升级为 [圣域练功房]", 0);
								else
								{
									sxd_client_super_town.st_practice_room();
								}
							}

							// st arena
							if (!common::contain(function_ids, "132"))
								common::log("【仙界竞技场】未开启", 0);
							else
							{
								sxd_client_super_town.st_arena();           // 挑战
								//sxd_client_town.exploit_shop();             // 荣誉商店买内丹
							}

							// st take bible
							if (!common::contain(function_names, "跨服取经"))
								common::log("【仙界取经】未开启", 0);
							else
							{
								if (common::contain(function_names, "主角飞升"))
									common::log("【仙界取经】主角已成圣，升级为 [圣域取经]", 0);
								else
								{
									sxd_client_super_town.st_take_bible();                      // 护送取经
									sxd_client_super_town.st_rob_bible(&sxd_client_town);       // 拦截取经
								}
							}

							// st super sport
							if (!common::contain(function_ids, "93"))
								common::log("【神魔竞技】未开启", 0);
							else
							{
								if (common::contain(function_names, "主角飞升"))
									common::log("【神魔竞技】主角已成圣，升级为 [圣域竞技场]", 0);
								else
								{
									sxd_client_super_town.point_race(&sxd_client_town);         // 积分赛
									sxd_client_super_town.war_race(&sxd_client_town);           // 神魔大战
									sxd_client_super_town.get_rank_award(&sxd_client_town);     // 排名奖励
									sxd_client_super_town.get_score_award();                    // 神魔大礼
								}
							}
						}
					}
					catch (const std::exception& ex)
					{
						common::log(boost::str(boost::format("发现错误(super town)：%1%") % ex.what()));
					}
				}

				// saint area 圣域		
				{
					try
					{
						if (!common::contain(function_names, "圣域"))
							common::log("【圣域】未开启", 0);
						else if (!sxd_client_saint_area.login_saint_area(&sxd_client_town))
						{
							sxd_client_saint_area.sa_take_bible();                  // 圣域取经
							sxd_client_saint_area.sa_super_sport();                 // 圣域竞技场
						}
					}
					catch (const std::exception& ex)
					{
						common::log(boost::str(boost::format("发现错误(saint area)：%1%") % ex.what()));
					}
				}

				// server chat room 聊天室
				{
					try
					{
						if (!common::contain(function_names, "聊天室"))
							common::log("【全网聊天】未开启", 0);
						else if (!sxd_client_chat_room.login_server_chat(&sxd_client_town))
						{
							sxd_client_chat_room.pet_escort(&sxd_client_town);
						}
					}
					catch (const std::exception& ex)
					{
						common::log(boost::str(boost::format("发现错误(server chat room)：%1%") % ex.what()));
					}
				}

				// super sport 竞技场
				{
					if (!common::contain(function_names, "竞技场"))
					{
						common::log("【竞技场】未开启", 0);
					}
					else
					{
						sxd_client_town.super_sport();
					}
				}
			}
			else if (fen_dui == 3)
			{
				sxd_client_town.Single_DevilExpedition();			//周末魔神远征			
			}
			else if (fen_dui == 4)
			{
				sxd_client_town.MysticCapture();					//仙幽猎境（幻灵）
			}
		}
		else if (fun_id == 2)
		{
			sxd_client_town.auto_quest();		//自动任务
		}
		else if (fun_id == 3)
		{
			for (;;)
			{
				common::log(" \n\t 1.通天塔\n\t 2.仙界挑战\n\t 3.圣域挑战\n\t 4,圣域秘境\n\t 5,宗师修行\n\t 6,魔界远征\n\t 7,五行天宫\n\t 8,九霄云巅\n\t 9,锁妖塔\n\t 10,圣域秘境塔\n\t 11,诸法洞天\n\t 12,诸法洞天：无尽模式\n\t 0,退出\n\t 请选择相应的功能：");
				int fun;
				std::cin >> fun;
				if (fun == 1)
				{
					sxd_client_town.tong_tian_tower_challenge();			// 通天塔挑战
					//sxd_client_town.tong_tian_tower_practice();				// 通天塔扫荡
				}
				else if (fun == 2)
				{
					if (!common::contain(function_names, "仙界"))
						common::log("【仙界】未开启", 0);
					else if (!sxd_client_super_town.login_super_town(&sxd_client_town))
					{
						common::log(" \n\t 1.八仙过海\n\t 2.魔神挑战\n\t 3.仙界竞技场\n\t 请选择相应的功能：");
						int i;
						std::cin >> i;
						if (i == 1)
						{
							sxd_client_super_town.StEightImmortals();            // 八仙过海
						}
						if (i == 2)
						{
							sxd_client_super_town.st_union_task();              // 魔神挑战
						}
						if (i == 3)
						{
							sxd_client_super_town.st_arena();					// 仙界竞技场
						}
					}
				}
				else if (fun == 3)
				{
					if (!common::contain(function_names, "圣域"))
						common::log("【圣域】未开启", 0);
					else if (!sxd_client_saint_area.login_saint_area(&sxd_client_town))
					{
						common::log(" \n\t 1.圣域取经\n\t 2.圣域竞技场\n\t 3.无尽龙域 4.圣域秘境\n\t 请选择相应的功能：");
						int i;
						std::cin >> i;
						if (i == 1)
						{
							sxd_client_saint_area.sa_take_bible();              // 圣域取经
						}
						if (i == 2)
						{
							sxd_client_saint_area.sa_super_sport();             // 圣域竞技场
						}
						if (i == 3)
						{
							//sxd_client_super_town.sa_dragon_area();					// 无尽龙域sa_dragon_area
						}
						if (i == 4)
						{
							sxd_client_super_town.sa_trials();					// 圣域秘境sa_trials
						}
					}
				}
				else if (fun == 4)
				{
					sxd_client_town.sa_trials();					// 圣域秘境sa_trials
				}
				else if (fun == 5)
				{
					sxd_client_town.MasterPractice();					// 宗师修行
				}
				else if (fun == 6)
				{
					sxd_client_town.DevilExpedition();					// 魔界远征
				}
				else if (fun == 7)
				{
					sxd_client_town.FiveElementsPalace();					// 五行天宫
				}
				else if (fun == 8)
				{
					sxd_client_town.Fairyland();					// 九霄云巅
				}
				else if (fun == 9)
				{
					sxd_client_town.RepressDemonTower();					// 锁妖塔
				}
				else if (fun == 10)
				{
					sxd_client_town.satrials();					// 圣域秘境
				}
				else if (fun == 11)
				{
					sxd_client_town.EndlessExpedition();		//诸法洞天
				}
				else if (fun == 12)
				{
					sxd_client_town.Endlesschallenge();	//诸法洞天 无尽模式
				}
				else if (fun == 0)
				{
					break;
				}
			}
		}
		else if (fun_id == 4)
		{
			for (;;)
			{
				common::log("\n\n\t 1.强化降魔战魂至太初十\n\t 2.制作逍遥战魂\n\t 0.退出\n\t请选择相应的功能：");

				int fun4;
				std::cin >> fun4;

				if (fun4 == 1)
				{
					sxd_client_town.upgrade();
				}
				else if (fun4 == 2)
				{
					sxd_client_town.equip_use_reel();
				}
				else
				{
					break;
				}
			}
		}
		else if (fun_id == 5)
		{
		common::log("【测试1】开启");
		sxd_client_town.spring_carnival();	//新春嘉年华
		}
		else if (fun_id == 0)
		{
			break;
		}

	}
	/*		
		// equipment
		if (!common::contain(function_names, "强化"))
			common::log("【强化】未开启", 0);
		else {
			sxd_client_town.equipment();
		}

		// library
		if (!common::contain(function_names, "藏经阁"))
			common::log("【藏经阁】未开启", 0);
		else {
			sxd_client_town.library();
		}

		// research 奇术
		if (!common::contain(function_names, "奇术"))
			common::log("【奇术】未开启", 0);
		else {
			sxd_client_town.research();
		}

		// rune 招财神符
		if (!common::contain(function_names, "招财神符"))
			common::log("【招财神符】未开启", 0);
		else {
			sxd_client_town.rune();
		}

		// 周年活动
		if (!common::contain(function_names, "周年活动"))
			common::log("【周年活动】未开启", 0);
		else {
			// sign in，签到
			sxd_client_town.sign_in();
			// wager shop，游乐城赌坊
			sxd_client_town.wager_shop();
			// hide treasure map，藏宝图
			sxd_client_town.hide_treasure_map();
			// dice messenger，骰子使者
			sxd_client_town.dice_messenger();
			// dunhuang treasure，敦煌秘宝
			sxd_client_town.dunhuang_treasure();
			// spring carnival，新春嘉年华
			sxd_client_town.spring_carnival();
			// regression，回归
			sxd_client_town.regression();
			// find treasure，龙宫探宝
			sxd_client_town.find_treasure();
		}
	
		// training
		if (!common::contain(function_names, "培养"))
			common::log("【培养】未开启", 0);
		else {
			sxd_client_town.training();
		}	

		// beelzebub trials
		if (!common::contain(function_names, "魔王试炼"))
			common::log("【魔王试炼】未开启", 0);
		else {
			sxd_client_town.beelzebub_trials();
		}		

		// nine regions，九空无界
		sxd_client_town.nine_regions();

		// dance
		if (!common::contain(function_names, "舞蹈动作"))
			common::log("【群仙乱舞】未开启", 0);
		else {
			sxd_client_town.dance();
		}

		// marry
		if (!common::contain(function_names, "结婚"))
			common::log("【结婚】未开启", 0);
		else {
			sxd_client_town.marry();
		}

		// coin mission
		if (!common::contain(function_names, "铜钱副本"))
			common::log("【铜钱副本】未开启", 0);
		else {
			sxd_client_town.coin_mission();
		}	

		// lucky super number，幸运大比拼
		sxd_client_town.lucky_super_number();
	
		// npc friendship
		if (!common::contain(function_names, "NPC结交"))
			common::log("【NPC结交】未开启", 0);
		else {
			//sxd_client_town.npc_friendship();
		}		
		std::cout << "完整执行\n";
		*/
}

void sxd::collect()
{
	try
	{
		std::cout << "请输入版本号：" << std::endl << std::flush;
		std::string version;
		std::cin >> version;
		sxd::collect_protocol(version, "d:\\随心\\协议\\" + version + "\\com\\protocols");
		sxd::collect_end_function_gift(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\gifttypedata.as");
		sxd::collect_function(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\functiontypedata.as");
		sxd::collect_gift(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\gifttypedata.as");
		sxd::collect_item(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\itemtypedata.as");
		sxd::collect_lucky_shop_item(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\itemtypedata.as");
		sxd::collect_role(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\roletype.as");
		sxd::collect_town(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\towntypedata.as");
		sxd::collect_facture_reel(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\facturereeltype.as");
		sxd::collect_mission_monster(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\missionmonstertypedata.as");
		sxd::collect_mission_team(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\missionmonstertypedata.as");
		sxd::collect_quest(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\questtypedata.as");
		sxd::collect_mission(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\missiontypedata.as");
		sxd::collect_mission_to_monster(version, "d:\\随心\\协议\\" + version + "\\mission.txt"); 
		sxd::collect_tongtiantower(version, "d:\\随心\\协议\\" + version + "\\com\\assist\\server\\source\\TongTianTowerTypeData.as");
	}
	catch (const std::exception& ex)
	{
		std::cerr << boost::str(boost::format("发现错误(collect)：%1%") % ex.what()) << std::endl;
	}
}

void sxd::collect_end_function_gift(const std::string& version, const std::string& path)
{
	std::string type = "EndFunctionGift";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("endFunctionGift.*?\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?), \"(.*?)\", \"(.*?)\".*?\\]");
	boost::regex regex2("\\[(\\d+?),\"(.*?)\",\"(.*?)\".*?\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_function(const std::string& version, const std::string& path)
{
	std::string type = "Function";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("List:Object = .*?;");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\".*?\",\"(.*?)\",\"(.*?)\"\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_gift(const std::string& version, const std::string& path)
{
	std::string type = "Gift";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("GiftTypes.*?\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?), \"(.*?)\", \"(.*?)\".*?\\]");
	boost::regex regex2("\\[(\\d+?),\"(.*?)\",\"(.*?)\".*?\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_item(const std::string& version, const std::string& path)
{
	std::string type = "Item";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Items:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\\d*?,\".*?\",\"(.*?)\".*?]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[2] % "" % match[3];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_lucky_shop_item(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute("DELETE FROM lucky_shop_item where version!='" + version + "'");
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("AryShopItem:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(?<id>\\d+?):\\[(?<item_id>\\d*?),(?<count>\\d*?),(?<ingot>\\d*?),(?<coin>\\d*?)\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO lucky_shop_item(version, id, item_id, count, ingot, coin) VALUES('%1%', %2%, %3%, %4%, %5%, %6%)");
		sql % version % match["id"] % match["item_id"] % match["count"] % match["ingot"] % match["coin"];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");

}

void sxd::collect_role(const std::string& version, const std::string& path)
{
	std::string type = "Role";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Roles:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[(.*?),\"(.*?)\",.*?]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[3] % match[2] % "";
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_town(const std::string& version, const std::string& path)
{
	std::string type = "Town";

	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM code where version!='" + version + "' and type='" + type + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Towns:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[\"(.*?)\",\"(.*?)\",\\{(.*?)\\},(\\d+?),\"(.*?)\",.*?\r\n");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO code(version, type, value, text, sign, comment) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % type % match[1] % match[6] % match[2] % match[3];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_facture_reel(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute("DELETE FROM facture_reel where version!='" + version + "'");
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("_factureReelList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d*?),(\\d*?),(\\d*?),\"(.*?)\",(\\d*?)\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO facture_reel(version, reel_id, item_id, count, position, mission_id) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % match[1] % match[2] % match[3] % match[4] % match[5];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_protocol(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM protocol where version!='" + version + "'").c_str());

	boost::filesystem::directory_iterator ite;
	for (boost::filesystem::directory_iterator it(path); it != ite; ++it)
	{
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
		for (auto it = boost::sregex_iterator(content.begin(), content.end(), boost::regex("public static const (?<method>.*?):Object = {\\s*\"module\":(?<module>\\d*?),\\s*\"action\":(?<action>\\d*?),\\s*\"request\":(?<request>.*?),\\s*\"response\":(?<response>.*?)\\s*};"), boost::match_not_dot_newline); it != boost::sregex_iterator(); it++)
		{
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
			m_db->execute(sql.str().c_str());
		}
	}
	m_db->execute("COMMIT");
} //sxd::collect_protocols

void sxd::collect_mission_monster(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM mission_monster where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("missionMonsterTeamList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO mission_monster(version, team_id, scene_id, monster_id) VALUES('%1%', '%2%', '%3%', '%4%')");
		sql % version % match[1] % match[2] % match[4];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_mission(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM mission where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Missions:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("(\\d+?):\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",(\\d+?),(\\d+?),(\\d+?)\\],\r\n");

	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO mission(version, mission_id, scene_id, boss_id, mission_name, hero_mission, boss_mission) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%', '%7%')");
		sql % version % match[1] % match[2] % match[8] % match[9] % match[10] % match[11];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_quest(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM quest where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("Quests:Object = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	boost::regex regex2("(\\d+?):\\[(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?)\\],\r\n");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO quest(version, quest_id, quest_type, level, quest_name, mission_id) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql % version % match[1] % match[2] % match[4] % match[5] % match[18];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_mission_team(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM mission_team where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("missionMonsterList:Array = .*?;\r\n");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	boost::regex regex2("\\[(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?)\\]");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO mission_team(version, _id, team_id, monster_id) VALUES('%1%', '%2%', '%3%', '%4%')");
		sql % version % match[1] % match[2] % match[3];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_mission_to_monster(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM mission_to_monster where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	//boost::regex regex1("\"(\\d+?)\":\{\"(.*?)\":\"(.*?)\",\"(.*?)\":(\\d+?),\"(.*?)\":\\[(.*?)\\](.*?)\},");
	//boost::regex_search(content, match, regex1);
	//std::cout << "match\n" ;
	//std::cout << match << "\n";
	//content = match.str();
	// regex2
	boost::regex regex2("\"(\\d+?)\":\\{\"(\\S*)\":\"(\\S*)\",\"(\\S*)\":(\\d+?),\"(\\S*)\":\\[(\\S*)\\](\\S*)\\},\r\n");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO mission_to_monster(version, mission_id, mission_name, map_id, mission_team) VALUES('%1%', '%2%', '%3%', '%4%', '%5%')");
		sql% version% match[1] % match[3] % match[5] % match[7];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}

void sxd::collect_tongtiantower(const std::string& version, const std::string& path)
{
	database* m_db = NULL;
	connectionRAII sqlcon(&m_db, connection_pool::GetInstance());

	m_db->execute("BEGIN");
	m_db->execute(("DELETE FROM tongtiantower where version!='" + version + "'").c_str());
	std::string content = common::read_file(path);
	// regex 1
	boost::smatch match;
	boost::regex regex1("TowerArray:Array = \\[.*?\\];");
	//boost::regex regex1("TowerArray:Array = \\[\\{.*?\\},");
	boost::regex_search(content, match, regex1);
	content = match.str();
	// regex2
	//boost::regex regex2("TowerArray:Array = \\{.*?\\},");
	//boost::regex regex2("\\{(.*?)\"(.*?)\":(?<heaven>\\d+?),(.*?)\"(.*?)\":(?<floor>\\d+?),");
	boost::regex regex2("\\{(.*?)\"(.*?)\":(?<heaven>\\d+?),(.*?)\"(.*?)\":(?<floor>\\d+?),(.*?)\"(.*?)\":(?<next_heaven>\\d+?),(.*?)\"(.*?)\":(?<next_floor>\\d+?),(.*?)\"(.*?)\":(?<monster_team_id>\\d+?),(.*?)\"(.*?)\":(?<special_pattern>\\d+?),(.*?)\"(.*?)\":(?<is_boss>\\d+?),(.*?)\"(.*?)\":(?<before_video>\\d+?),(.*?)\"(.*?)\":(?<end_video>\\d+?),(.*?)\"(.*?)\":\"(?<desc>.*?)\",(.*?)\"(.*?)\":(?<soul_monster_team_id>\\d+?),(.*?)\"(.*?)\":(?<soul_special_pattern>\\d+?),(.*?)\"(.*?)\":(?<attr_prob>\\d+?)(.*?)\\}");
	//boost::regex regex2("(\\d+?):\\[(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),(\\d+?),\"(.*?)\",\"(.*?)\",\"(.*?)\",(\\d+?),(\\d+?)\\],\r\n");
	while (boost::regex_search(content, match, regex2))
	{
		auto sql = boost::format("INSERT INTO tongtiantower(version, heaven, floor, monster_team_id, soul_monster_team_id, attr_prob) VALUES('%1%', '%2%', '%3%', '%4%', '%5%', '%6%')");
		sql% version% match["heaven"] % match["floor"] % match["monster_team_id"] % match["soul_monster_team_id"] % match["attr_prob"];
		common::log(common::utf2gbk(sql.str()));
		m_db->execute(sql.str().c_str());
		content = match.suffix();
	}
	m_db->execute("COMMIT");
}