#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class AncientRealmtype
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int FAILED = 3;
	static const int NOT_IN_RUNING_STATE = 21;
};

void sxd_client::AncientRealm()
{
	vector<std::string> type_list{ " 1:护境使者","2:玄境灵兽","3:加成格","4:奖励格","5:钥匙格","6:钥匙格","7:路障格","8:普通格","9:游商格","10:古境戍卫" };
	
	for (;;)
	{
		vector<vector<int>> array(9, vector<int>(11));
		array[1][1] = 1, array[1][2] = 1, array[2][1] = 1, array[2][2] = 1;
		array[1][8] = 1, array[1][9] = 1, array[2][8] = 1, array[2][9] = 1;
		array[6][1] = 1, array[6][2] = 1, array[7][1] = 1, array[7][2] = 1;
		array[6][8] = 1, array[6][9] = 1, array[7][8] = 1, array[7][9] = 1;

		common::log("【玄天古境】状态信息！！", 1);
		Json::Value data = this->Mod_AncientRealm_Base_get_state();

		//活动是否开启
		int is_func_open = data[2].asInt();
		if (!is_func_open)
		{
			common::log("【玄天古境】未开启！！");
			return;
		}

		common::log("【玄天古境】地图信息！！", 1);
		data = this->Mod_AncientRealm_Base_get_map_panel_info();

		//次数
		int ticket_num = 0;
		//是否挑战BOSS
		int is_fight_boss = data[3].asInt();
		int is_get_key1 = data[14].asInt();
		int is_get_key2 = data[15].asInt();
		common::log("【玄天古境】基本信息：");
		if(is_fight_boss)
			common::log("【玄天古境】BOSS已击杀！");
		else
			common::log("【玄天古境】BOSS未击杀！");
		if (is_get_key1)
			common::log("【玄天古境】钥匙【1】已获得！");
		else
			common::log("【玄天古境】钥匙【1】未获得！");
		if (is_get_key2)
			common::log("【玄天古境】钥匙【2】已获得！");
		else
			common::log("【玄天古境】钥匙【2】未获得！");

		for (Json::Value team_member : data[9])
		{
			//寻找自己的id
			if (team_member[8].asInt() == 1)
			{
				//获取次数
				ticket_num = team_member[7].asInt();
			}
		}
		if (ticket_num <= 0)
		{
			common::log("【玄天古境】没有次数！！");
			return;
		}

		Json::Value grid_info_list;
		for (Json::Value temp : data[11])
		{
			int x = temp[0].asInt();
			int y = temp[1].asInt();		
			int type = temp[2].asInt();
			int is_open = temp[3].asInt();
			int is_can_walk = temp[4].asInt();

			if (type != 7 && is_open && !is_can_walk)
			{
				array[x][y] = 1;
				grid_info_list.append(temp);
			}
		}

		common::log("\n\n\t 1.自动任务\n\t 2.挑战怪物\n\t 3.开启宝箱\n\t 4.开启格子\n\t 5.开启剩余格子\n\t 6.打BOSS\n\t 7.领取历练奖励\n\t 0.退出\n\t请选择相应的功能：");
		int flag;
		std::cin >> flag;
		if (flag == 1)
		{
			data = this->Mod_AncientRealm_Base_get_map_panel_info();
			//是否需要选择战利品
			int is_need_choose_spoil = data[2].asInt();
			if (is_need_choose_spoil)
			{
				int id, grid_id;
				common::log("【玄天古境】战利品信息！！");
				data = this->Mod_AncientRealm_Base_spoil_panel();

				for (Json::Value temp : data[0])
				{
					//龙珠
					if (temp[1].asInt() != 1)
					{
						id = temp[1].asInt();
						grid_id = temp[0].asInt();
						break;
					}
				}
				common::log("【玄天古境】选择战利品类型：\n\t1.龙珠\n\t2.觉醒\n\t3.法宝");
				int via = 1;
				std::cin >> via;
				common::log("玄天古境】选择战利品：\n\t1.\n\t2.\n\t3.");
				//common::log(boost::str(boost::format("【玄天古境】选择战利品：\n\t1.%1%\n\t2.%2%\n\t3.%3%") % db.get_code(version, "Item", data[via - 1][0][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][1][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][2][1].asInt())["text"]), iEdit);
				std::cin >> grid_id;

				data = this->Mod_AncientRealm_Base_choose_spoil(0, via, grid_id);
				if (data[0].asInt() == AncientRealmtype::SUCCESS)
				{
					//ticket_num = ticket_num - 1;
					common::log("【玄天古境】选择战利品成功！！");
				}
			}

			//先打怪
			common::log("【玄天古境】挑战怪物！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				int monster_team_id = temp[7].asInt();
				if (monster_team_id)
				{
					if (ticket_num <= 2)
					{
						common::log("【玄天古境】不足以挑战怪物！！");
						break;
					}

					int x = temp[0].asInt();
					int y = temp[1].asInt();
					//this->move(x, y);
					if (this->move(x, y))
					{
						Json::Value data_fight = this->Mod_AncientRealm_Base_fight_monster(x, y);
						if (data_fight[0].asInt() == AncientRealmtype::SUCCESS)
						{
							ticket_num = ticket_num - 3;
							//common::log("【玄天古境】挑战怪物成功！！");
							common::log(boost::str(boost::format("【玄天古境】挑战怪物【%1%】成功！！") % type_list[type - 1]));
						}
					}
					else
					{
						common::log("【玄天古境】无法移动到可挑战位置！！");
					}

				}
			}

			//开启宝箱
			common::log("【玄天古境】开启宝箱！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				if (type == 4)
				{
					int x = temp[0].asInt();
					int y = temp[1].asInt();
					//this->move(x, y);
					if (this->move(x, y))
					{
						Json::Value data_open = this->Mod_AncientRealm_Base_open_award_grid_box(x, y);
						if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
						{
							common::log("【玄天古境】开启宝箱成功！！");
						}
					}
					else
					{
						common::log("【玄天古境】无法移动到可挑战位置！！");
					}

				}
			}

			//开启格子
			common::log("【玄天古境】开启格子！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				if (ticket_num <= 0)
				{
					common::log("【玄天古境】次数不足！！");
					break;
				}

				int x = temp[0].asInt();
				int y = temp[1].asInt();
				//this->move(x, y);
				if (this->move(x, y))
				{
					Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(x, y);
					if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
					{
						ticket_num = ticket_num - 1;
						//common::log("【玄天古境】开启格子成功！！");
						//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % x % y), iEdit);
						common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % x % y % type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
					}
				}
				else
				{
					common::log("【玄天古境】无法移动到可挑战位置！！");

				}
			}

			//开启剩余格子
			int via = 1;
			while (via != 0)
			{
				common::log("【玄天古境】开启剩余格子,选择区域：\n\t1.3-5\n\t2.0-2\n\t3.6-8\n\t0.跳过");
				std::cin >> via;
				switch (via)
				{
				case 1:
					for (int i = 3; i <= 5; i++)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}
								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;
										//common::log("【玄天古境】开启格子成功！！");
										//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i% j% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				case 2:
					for (int i = 0; i <= 2; i++)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}

								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;
										//common::log("【玄天古境】开启格子成功！！");
										//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i% j% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				case 3:
					for (int i = 6; i <= 8; i++)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}

								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;
										//common::log("【玄天古境】开启格子成功！！");
										//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i% j% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				}
			}

			//打BOSS
			if (!is_fight_boss)
			{
				int fight_boss = 1;
				while (fight_boss != 0)
				{
					common::log("【玄天古境】是否挑战BOSS：\n\t1.是\n\t0.否");
					std::cin >> fight_boss;
					switch (fight_boss)
					{
					case 0:
						break;
					case 1:
					{
						int move_flag = 0;
						for (int i = 3; i <= 5; i++)
						{
							move_flag = this->move(i, 10);
							if (move_flag == 1)
							{
								break;
							}
						}
						if (move_flag == 1)
						{
							Json::Value data_fight_boss = this->Mod_AncientRealm_Base_fight_boss();
							if (data_fight_boss[0].asInt() == AncientRealmtype::SUCCESS)
							{
								common::log("【玄天古境】挑战boss成功！！");
								data = this->Mod_AncientRealm_Base_get_map_panel_info();
								is_fight_boss = data[3].asInt();
								if (is_fight_boss)
								{
									common::log("【玄天古境】boss已击杀！！");
									fight_boss = 0;
								}
							}
						}
						else
						{
							common::log("【玄天古境】无法移动到可挑战位置！！");
							fight_boss = 0;
						}
					}
					break;
					}
				}
			}
			else
			{
				common::log("【玄天古境】boss已击杀！！");
			}

			//领取历练奖励
			common::log("【玄天古境】领取玄天历练奖励！！");
			//开拓
			data = this->Mod_AncientRealm_Base_experience_panel(1);
			for (Json::Value temp : data[0])
			{
				int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
				if (is_finish && !is_get)
				{
					Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(1, mission_id);
					if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
					{
						common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
					}
				}
			}
			//破敌
			data = this->Mod_AncientRealm_Base_experience_panel(2);
			for (Json::Value temp : data[0])
			{
				int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
				if (is_finish && !is_get)
				{
					Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(2, mission_id);
					if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
					{
						common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
					}
				}
			}

			//acient_realm:int = 362

			//common::log("【玄天古境】隔断！！");
			data = this->Mod_AncientRealm_Base_get_map_panel_info();
			for (Json::Value team_member : data[9])
			{
				//寻找自己的id
				if (team_member[8].asInt() == 1)
				{
					//获取次数
					ticket_num = team_member[7].asInt();
				}
			}
			if (ticket_num <= 0)
			{
				common::log("【玄天古境】没有次数！！");
				return;
			}
		}
		else if (flag == 2)
		{
			data = this->Mod_AncientRealm_Base_get_map_panel_info();
			//是否需要选择战利品
			int is_need_choose_spoil = data[2].asInt();
			if (is_need_choose_spoil)
			{
				int id, grid_id;
				common::log("【玄天古境】战利品信息！！");
				data = this->Mod_AncientRealm_Base_spoil_panel();

				for (Json::Value temp : data[0])
				{
					//龙珠
					if (temp[1].asInt() != 1)
					{
						id = temp[1].asInt();
						grid_id = temp[0].asInt();
						break;
					}
				}
				common::log("【玄天古境】选择战利品类型：\n\t1.龙珠\n\t2.觉醒\n\t3.法宝");
				int via = 1;
				std::cin >> via;
				common::log("玄天古境】选择战利品：\n\t1.\n\t2.\n\t3.");
				//common::log(boost::str(boost::format("【玄天古境】选择战利品：\n\t1.%1%\n\t2.%2%\n\t3.%3%") % db.get_code(version, "Item", data[via - 1][0][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][1][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][2][1].asInt())["text"]), iEdit);
				std::cin >> grid_id;

				data = this->Mod_AncientRealm_Base_choose_spoil(0, via, grid_id);
				if (data[0].asInt() == AncientRealmtype::SUCCESS)
				{
					//ticket_num = ticket_num - 1;
					common::log("【玄天古境】选择战利品成功！！");
				}
			}

			//先打怪
			common::log("【玄天古境】挑战怪物！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				int monster_team_id = temp[7].asInt();
				if (monster_team_id)
				{
					if (ticket_num <= 2)
					{
						common::log("【玄天古境】玄霄令不足以挑战怪物！！");
						break;
					}

					int x = temp[0].asInt();
					int y = temp[1].asInt();
					//this->move(x, y);
					if (this->move(x, y))
					{
						Json::Value data_fight = this->Mod_AncientRealm_Base_fight_monster(x, y);
						if (data_fight[0].asInt() == AncientRealmtype::SUCCESS)
						{
							ticket_num = ticket_num - 3;
							//common::log("【玄天古境】挑战怪物成功！！");
							common::log(boost::str(boost::format("【玄天古境】挑战怪物【%1%】成功！！") % type_list[type-1]));
							
							//是否需要选择战利品
							data = this->Mod_AncientRealm_Base_get_map_panel_info();
							int is_need_choose_spoil = data[2].asInt();
							if (is_need_choose_spoil)
							{
								int id, grid_id;
								common::log("【玄天古境】战利品信息！！");
								data = this->Mod_AncientRealm_Base_spoil_panel();

								for (Json::Value temp : data[0])
								{
									//龙珠
									if (temp[1].asInt() != 1)
									{
										id = temp[1].asInt();
										grid_id = temp[0].asInt();
										break;
									}
								}
								common::log("【玄天古境】选择战利品类型：\n\t1.龙珠\n\t2.觉醒\n\t3.法宝");
								int via = 1;
								std::cin >> via;
								common::log("玄天古境】选择战利品：\n\t1.\n\t2.\n\t3.");
								//common::log(boost::str(boost::format("【玄天古境】选择战利品：\n\t1.%1%\n\t2.%2%\n\t3.%3%") % db.get_code(version, "Item", data[via - 1][0][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][1][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][2][1].asInt())["text"]), iEdit);
								std::cin >> grid_id;

								data = this->Mod_AncientRealm_Base_choose_spoil(0, via, grid_id);
								if (data[0].asInt() == AncientRealmtype::SUCCESS)
								{
									//ticket_num = ticket_num - 1;
									common::log("【玄天古境】选择战利品成功！！");
								}
							}
						}
					}
					else
					{
						common::log("【玄天古境】无法移动到可挑战位置！！");
					}

				}
			}
		}
		else if (flag == 3)
		{
			//开启宝箱
			common::log("【玄天古境】开启宝箱！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				if (type == 4)
				{
					int x = temp[0].asInt();
					int y = temp[1].asInt();
					//this->move(x, y);
					if (this->move(x, y))
					{
						Json::Value data_open = this->Mod_AncientRealm_Base_open_award_grid_box(x, y);
						if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
						{
							common::log("【玄天古境】开启宝箱成功！！");
						}
					}
					else
					{
						common::log("【玄天古境】无法移动到可挑战位置！！");
					}

				}
			}
		}
		else if (flag == 4)
		{
			//开启格子
			common::log("【玄天古境】开启格子！！");
			for (Json::Value temp : grid_info_list)
			{
				int type = temp[2].asInt();
				if (type == 5 || type == 6 || type == 8 || type == 9)
				{

					if (ticket_num <= 0)
					{
						common::log("【玄天古境】次数不足！！");
						break;
					}

					int x = temp[0].asInt();
					int y = temp[1].asInt();
					//this->move(x, y);
					if (this->move(x, y))
					{
						Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(x, y);
						if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
						{
							ticket_num = ticket_num - 1;
							//common::log("【玄天古境】开启格子成功！！");
							//common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功！！") % x% y% type_list[type - 1]), iEdit);
							common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % x% y% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
						}
					}
					else
					{
						common::log("【玄天古境】无法移动到可挑战位置！！");

					}
				}
			}
		}
		else if (flag == 5)
		{
			//开启剩余格子
			int via = 1;
			while (via != 0)
			{
				common::log("【玄天古境】开启剩余格子,选择区域：\n\t1.3-5\n\t2.0-2\n\t3.6-8\n\t0.跳过");
				std::cin >> via;
				switch (via)
				{
				case 1:
					for (int i = 3; i <= 5; i++)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}
								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;										
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i% j% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				case 2:
					for (int i = 2; i >= 0; i--)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}

								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;
										//common::log("【玄天古境】开启格子成功！！");
										//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i % j % type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				case 3:
					for (int i = 6; i <= 8; i++)
					{
						for (int j = 0; j <= 10; j++)
						{
							if (array[i][j] != 1)
							{
								if (ticket_num <= 0)
								{
									common::log("【玄天古境】次数不足！！");
									break;
								}

								//this->move(i, j);
								if (this->move(i, j))
								{
									Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
									if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
									{
										ticket_num = ticket_num - 1;
										//common::log("【玄天古境】开启格子成功！！");
										//common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
										common::log(boost::str(boost::format("开启格子（%1%，%2%）【%3%】成功，获得[%4%玄玉]！！") % i% j% type_list[data_open[1].asInt() - 1] % data_open[2].asInt()), iEdit);
									}
								}
								else
								{
									common::log("【玄天古境】无法移动到可挑战位置！！");

								}
							}
						}
					}
					break;
				}
			}
		}
		else if (flag == 6)
		{
			//打BOSS
			if (!is_fight_boss)
			{
				int fight_boss = 1;
				while (fight_boss != 0)
				{
					common::log("【玄天古境】是否挑战BOSS：\n\t1.是\n\t0.否");
					std::cin >> fight_boss;
					switch (fight_boss)
					{
					case 0:
						break;
					case 1:
					{
						int move_flag = 0;
						for (int i = 3; i <= 5; i++)
						{
							move_flag = this->move(i, 10);
							if (move_flag == 1)
							{
								break;
							}
						}
						if (move_flag == 1)
						{
							Json::Value data_fight_boss = this->Mod_AncientRealm_Base_fight_boss();
							if (data_fight_boss[0].asInt() == AncientRealmtype::SUCCESS)
							{
								common::log("【玄天古境】挑战boss成功！！");
								data = this->Mod_AncientRealm_Base_get_map_panel_info();
								is_fight_boss = data[3].asInt();
								if (is_fight_boss)
								{
									common::log("【玄天古境】boss已击杀！！");
									fight_boss = 0;
								}
							}
						}
						else
						{
							common::log("【玄天古境】无法移动到可挑战位置！！");
							fight_boss = 0;
						}
					}
					break;
					}
				}
			}

			data = this->Mod_AncientRealm_Base_get_map_panel_info();
			for (Json::Value team_member : data[9])
			{
				//寻找自己的id
				if (team_member[8].asInt() == 1)
				{
					//获取次数
					ticket_num = team_member[7].asInt();
				}
			}
			if (ticket_num <= 0)
			{
				common::log("【玄天古境】没有次数！！");
				return;
			}
		}
		else if (flag == 7)
		{
			//领取历练奖励
			common::log("【玄天古境】领取玄天历练奖励！！");
			//开拓
			data = this->Mod_AncientRealm_Base_experience_panel(1);
			for (Json::Value temp : data[0])
			{
				int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
				if (is_finish && !is_get)
				{
					Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(1, mission_id);
					if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
					{
						common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
					}
				}
			}
			//破敌
			data = this->Mod_AncientRealm_Base_experience_panel(2);
			for (Json::Value temp : data[0])
			{
				int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
				if (is_finish && !is_get)
				{
					Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(2, mission_id);
					if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
					{
						common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
					}
				}
			}
		}
		else if (flag == 0)
		{
			return;
		}
	}
	/*//是否需要选择战利品
	int is_need_choose_spoil = data[2].asInt();
	if (is_need_choose_spoil)
	{
		int id, grid_id;
		common::log("【玄天古境】战利品信息！！");
		data = this->Mod_AncientRealm_Base_spoil_panel();

		for (Json::Value temp : data[0])
		{
			//龙珠
			if (temp[1].asInt() != 1)
			{
				id = temp[1].asInt();
				grid_id = temp[0].asInt();
				break;
			}
		}
		common::log("【玄天古境】选择战利品类型：\n\t1.龙珠\n\t2.觉醒\n\t3.法宝");
		int via = 1;
		std::cin >> via;
		common::log("玄天古境】选择战利品：\n\t1.\n\t2.\n\t3.");
		//common::log(boost::str(boost::format("【玄天古境】选择战利品：\n\t1.%1%\n\t2.%2%\n\t3.%3%") % db.get_code(version, "Item", data[via - 1][0][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][1][1].asInt())["text"] % db.get_code(version, "Item", data[via - 1][2][1].asInt())["text"]), iEdit);
		std::cin >> grid_id;

		data = this->Mod_AncientRealm_Base_choose_spoil(0, via, grid_id);
		if (data[0].asInt() == AncientRealmtype::SUCCESS)
		{
			//ticket_num = ticket_num - 1;
			common::log("【玄天古境】选择战利品成功！！");
		}
	}

	//先打怪
	common::log("【玄天古境】挑战怪物！！");
	for (Json::Value temp : grid_info_list)
	{
		int monster_team_id = temp[7].asInt();
		if (monster_team_id)
		{
			if (ticket_num <= 2)
			{
				common::log("【玄天古境】不足以挑战怪物！！");
				break;
			}

			int x = temp[0].asInt();
			int y = temp[1].asInt();
			//this->move(x, y);
			if (this->move(x, y))
			{
				Json::Value data_fight = this->Mod_AncientRealm_Base_fight_monster(x, y);
				if (data_fight[0].asInt() == AncientRealmtype::SUCCESS)
				{
					ticket_num = ticket_num - 3;
					common::log("【玄天古境】挑战怪物成功！！");
				}
			}
			else
			{
				common::log("【玄天古境】无法移动到可挑战位置！！");
			}

		}
	}

	//开启宝箱
	common::log("【玄天古境】开启宝箱！！");
	for (Json::Value temp : grid_info_list)
	{
		int type = temp[2].asInt();
		if (type==4)
		{
			int x = temp[0].asInt();
			int y = temp[1].asInt();
			//this->move(x, y);
			if (this->move(x, y))
			{
				Json::Value data_open = this->Mod_AncientRealm_Base_open_award_grid_box(x, y);
				if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
				{
					common::log("【玄天古境】开启宝箱成功！！");
				}
			}
			else
			{
				common::log("【玄天古境】无法移动到可挑战位置！！");
			}

		}
	}

	//开启格子
	common::log("【玄天古境】开启格子！！");
	for (Json::Value temp : grid_info_list)
	{
		int type = temp[2].asInt();
		if (type == 5 || type == 6 || type == 8 || type == 9)
		{

			if (ticket_num <= 0)
			{
				common::log("【玄天古境】次数不足！！");
				break;
			}

			int x = temp[0].asInt();
			int y = temp[1].asInt();
			//this->move(x, y);
			if (this->move(x, y))
			{
				Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(x, y);
				if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
				{
					ticket_num = ticket_num - 1;
					//common::log("【玄天古境】开启格子成功！！");
					common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % x % y), iEdit);
				}
			}
			else
			{
				common::log("【玄天古境】无法移动到可挑战位置！！");

			}
		}
	}

	//开启剩余格子
	int via = 1;
	while (via != 0)
	{
		common::log("【玄天古境】开启剩余格子,选择区域：\n\t1.3-5\n\t2.0-2\n\t3.6-8\n\t0.跳过");
		std::cin >> via;
		switch (via)
		{
		case 1:
			for (int i = 3; i <= 5; i++)
			{
				for (int j = 0; j <= 10; j++)
				{
					if (array[i][j] != 1)
					{
						if (ticket_num <= 0)
						{
							common::log("【玄天古境】次数不足！！");
							break;
						}
						//this->move(i, j);
						if (this->move(i, j))
						{
							Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
							if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
							{
								ticket_num = ticket_num - 1;
								//common::log("【玄天古境】开启格子成功！！");
								common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
							}
						}
						else
						{
							common::log("【玄天古境】无法移动到可挑战位置！！");

						}
					}
				}
			}
			break;
		case 2:
			for (int i = 0; i <= 2; i++)
			{
				for (int j = 0; j <= 10; j++)
				{
					if (array[i][j] != 1)
					{
						if (ticket_num <= 0)
						{
							common::log("【玄天古境】次数不足！！");
							break;
						}

						//this->move(i, j);
						if (this->move(i, j))
						{
							Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
							if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
							{
								ticket_num = ticket_num - 1;
								//common::log("【玄天古境】开启格子成功！！");
								common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
							}
						}
						else
						{
							common::log("【玄天古境】无法移动到可挑战位置！！");

						}
					}
				}
			}
			break;
		case 3:
			for (int i = 6; i <= 8; i++)
			{
				for (int j = 0; j <= 10; j++)
				{
					if (array[i][j] != 1)
					{
						if (ticket_num <= 0)
						{
							common::log("【玄天古境】次数不足！！");
							break;
						}

						//this->move(i, j);
						if (this->move(i, j))
						{
							Json::Value data_open = this->Mod_AncientRealm_Base_open_grid(i, j);
							if (data_open[0].asInt() == AncientRealmtype::SUCCESS)
							{
								ticket_num = ticket_num - 1;
								//common::log("【玄天古境】开启格子成功！！");
								common::log(boost::str(boost::format("开启格子（%1%，%2%）成功！！") % i % j), iEdit);
							}
						}
						else
						{
							common::log("【玄天古境】无法移动到可挑战位置！！");

						}
					}
				}
			}
			break;
		}
	}

	//打BOSS
	if (!is_fight_boss)
	{
		int fight_boss = 1;
		while (fight_boss != 0)
		{
			common::log("【玄天古境】是否挑战BOSS：\n\t1.是\n\t0.否");
			std:cin >> fight_boss;
			switch (fight_boss)
			{
			case 0:
				break;
			case 1:
			{
				int move_flag = 0;
				for (int i = 3; i <= 5; i++)
				{
					move_flag = this->move(i, 10);
					if (move_flag == 1)
					{
						break;
					}
				}
				if (move_flag == 1)
				{
					Json::Value data_fight_boss = this->Mod_AncientRealm_Base_fight_boss();
					if (data_fight_boss[0].asInt() == AncientRealmtype::SUCCESS)
					{
						common::log("【玄天古境】挑战boss成功！！");
						data = this->Mod_AncientRealm_Base_get_map_panel_info();
						is_fight_boss = data[3].asInt();
						if (is_fight_boss)
						{
							common::log("【玄天古境】boss已击杀！！");
							fight_boss = 0;
						}
					}
				}
				else
				{
					common::log("【玄天古境】无法移动到可挑战位置！！");
					fight_boss = 0;
				}
			}				
				break;
			}
		}
	}

	//领取历练奖励
	common::log("【玄天古境】领取玄天历练奖励！！");
	//开拓
	data = this->Mod_AncientRealm_Base_experience_panel(1);
	for (Json::Value temp : data[0])
	{
		int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
		if (is_finish && !is_get)
		{
			Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(1, mission_id);
			if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
			{
				common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
			}
		}
	}
	//破敌
	data = this->Mod_AncientRealm_Base_experience_panel(2);
	for (Json::Value temp : data[0])
	{
		int mission_id = temp[0].asInt(), is_finish = temp[2].asInt(), is_get = temp[3].asInt();
		if (is_finish && !is_get)
		{
			Json::Value data_experience = this->Mod_AncientRealm_Base_get_experience_award(2, mission_id);
			if (data_experience[0].asInt() == AncientRealmtype::SUCCESS)
			{
				common::log("【玄天古境】领取玄天历练·开拓 奖励成功！！");
			}
		}
	}*/
	
	
	//acient_realm:int = 362

	//common::log("【玄天古境】隔断！！");
	/*data = this->Mod_AncientRealm_Base_get_map_panel_info();
	for (Json::Value team_member : data[9])
	{
		//寻找自己的id
		if (team_member[8].asInt() == 1)
		{
			//获取次数
			ticket_num = team_member[7].asInt();
		}
	}
	if (ticket_num <= 0)
	{
		common::log("【玄天古境】没有次数！！");
		return;
	}*/
	
	//AncientRealm();
}


int sxd_client::move(int x, int y)
{
	Json::Value data = this->Mod_AncientRealm_Base_movement(x, y);
	int result = data[0].asInt();
	if (result == AncientRealmtype::SUCCESS)
	{
		common::log("【玄天古境】移动成功！！", 1);
		return 1;
	}

	data = this->Mod_AncientRealm_Base_movement(x - 1, y);
	result = data[0].asInt();
	if (result == AncientRealmtype::SUCCESS)
	{
		common::log("【玄天古境】移动成功！！", 1);
		return 2;
	}

	data = this->Mod_AncientRealm_Base_movement(x + 1, y);
	result = data[0].asInt();
	if (result == AncientRealmtype::SUCCESS)
	{
		common::log("【玄天古境】移动成功！！", 1);
		return 3;
	}

	data = this->Mod_AncientRealm_Base_movement(x, y - 1);
	result = data[0].asInt();
	if (result == AncientRealmtype::SUCCESS)
	{
		common::log("【玄天古境】移动成功！！", 1);
		return 4;
	}

	data = this->Mod_AncientRealm_Base_movement(x, y + 1);
	result = data[0].asInt();
	if (result == AncientRealmtype::SUCCESS)
	{
		common::log("【玄天古境】移动成功！！", 1);
		return 5;
	}
	return 0;
}


//============================================================================
// 玄天古境：获取状态
// {module:770, action:0, 
// request:[],
// 
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// AncientRealmData.as
//     oObject.list(param1,this._objGetState,["state_id","season","is_func_open","is_have_team"]);
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 0);
}

//============================================================================
//	玄天古境：获取地图信息
// "module":770,"action":12,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.mapPanelInfo,["result","is_first_come_layer","is_need_choose_spoil","is_fight_boss","team_id","layer_num","progress","goal_id","coin_num","team_member_list","layer_mission_list","grid_info_list","my_coin_num","boss_monster_team_id","is_get_key1","is_get_key2","is_experience_red_dot","sect_id","team_rank","is_have_store","is_order_red_dot"]);
//					oObject.list(_loc4_[_loc5_],_loc2_,["app_id","nickname","role_id","head_id","rim_id","leader_flag","title_id","ticket_num","is_self","grid_x","grid_y","game_player_id","ticket_limit"]);
//					this.mapPanelInfo.team_member_list.sortOn(["leader_flag","app_id"],[Array.NUMERIC | Array.DESCENDING,Array.NUMERIC]);
//					oObject.list(_loc4_[_loc5_],_loc2_,["x","y","grid_type_id","is_open","is_can_walk","is_locked","is_need_show","monster_team_id"]);
// 
// ["result","is_first_come_layer","is_need_choose_spoil","is_fight_boss","team_id","layer_num","progress","goal_id","coin_num",
// "team_member_list",
// "layer_mission_list",
// "grid_info_list",
//	"my_coin_num","boss_monster_team_id","is_get_key1","is_get_key2","is_experience_red_dot","sect_id","team_rank","is_have_store","is_order_red_dot"]);
// 
// Example
// [ 0, 0, 0, 1, 10254, 1, 89, 3, 1070, 
// [ [ 325, "\u51af\u6d77\u68ee\u5821", 207, 0, 7857, 0, 0, 26, 1, 4, 4, 211, 100 ], [ 19824, "\u5929\u624d\u5e05\u5e05.s51", 207, 12, 8120, 0, 0, 100, 0, 4, -1, 224623, 100 ], [ 1836, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51", 201, 11, 8120, 1, 0, 100, 0, 5, -1, 212358, 100 ] ],
// [ [ 3, 1 ], [ 4, 8 ], [ 1, 100 ], [ 2, 2 ] ], 
// [ [ 4, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 1, 8, 1, 1, 0, 0, 0 ], [ 5, 5, 7, 1, 0, 0, 0, 0 ], [ 4, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 4, 8, 1, 1, 0, 0, 0 ], [ 8, 3, 8, 0, 0, 1, 0, 0 ], [ 3, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 4, 4, 1, 1, 0, 0, 0 ], [ 3, 0, 8, 1, 1, 0, 0, 0 ], [ 8, 5, 3, 0, 0, 1, 0, 0 ], [ 0, 8, 10, 0, 0, 0, 0, 12360 ], [ 1, 10, 2, 0, 0, 1, 0, 12354 ], [ 5, 10, 8, 1, 1, 0, 0, 0 ], [ 1, 7, 8, 1, 1, 0, 0, 0 ], [ 7, 4, 10, 1, 0, 0, 0, 12360 ], [ 8, 6, 8, 0, 0, 1, 0, 0 ], [ 0, 1, 8, 1, 1, 0, 0, 0 ], [ 0, 3, 8, 1, 1, 0, 0, 0 ], [ 8, 7, 8, 0, 0, 1, 0, 0 ], [ 3, 2, 8, 1, 1, 0, 0, 0 ], [ 4, 8, 7, 1, 0, 0, 0, 0 ], [ 3, 6, 8, 1, 1, 0, 0, 0 ], [ 8, 4, 4, 0, 0, 1, 0, 0 ], [ 5, 6, 8, 1, 1, 0, 0, 0 ], [ 3, 3, 8, 1, 1, 0, 0, 0 ], [ 7, 7, 8, 1, 1, 0, 0, 0 ], [ 1, 5, 7, 1, 0, 0, 0, 0 ], [ 2, 7, 8, 1, 1, 0, 0, 0 ], [ 0, 0, 3, 1, 1, 0, 0, 0 ], [ 5, 2, 8, 1, 1, 0, 0, 0 ], [ 5, 0, 4, 1, 0, 0, 0, 0 ], [ 7, 10, 8, 0, 0, 1, 0, 0 ], [ 2, 10, 10, 1, 0, 0, 0, 12360 ], [ 2, 6, 3, 1, 1, 0, 0, 0 ], [ 4, 3, 4, 1, 1, 0, 0, 0 ], [ 5, 8, 8, 1, 1, 0, 0, 0 ], [ 2, 4, 8, 1, 1, 0, 0, 0 ], [ 0, 6, 4, 1, 1, 0, 0, 0 ], [ 6, 6, 3, 1, 1, 0, 0, 0 ], [ 3, 4, 7, 1, 0, 0, 0, 0 ], [ 4, 1, 8, 1, 1, 0, 0, 0 ], [ 7, 3, 1, 0, 0, 1, 0, 12353 ], [ 1, 4, 8, 1, 1, 0, 0, 0 ], [ 4, 10, 7, 1, 0, 0, 0, 0 ], [ 4, 6, 8, 1, 1, 0, 0, 0 ], [ 6, 3, 8, 1, 1, 0, 0, 0 ], [ 2, 5, 8, 1, 1, 0, 0, 0 ], [ 1, 6, 3, 1, 1, 0, 0, 0 ], [ 5, 7, 8, 1, 1, 0, 0, 0 ], [ 6, 5, 3, 0, 0, 1, 0, 0 ], [ 4, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 2, 5, 1, 1, 0, 1, 0 ], [ 3, 5, 8, 1, 1, 0, 0, 0 ], [ 2, 3, 6, 1, 1, 0, 1, 0 ], [ 4, 0, 4, 1, 1, 0, 0, 0 ], [ 5, 4, 2, 1, 1, 0, 0, 12354 ], [ 0, 2, 8, 1, 1, 0, 0, 0 ], [ 1, 3, 8, 1, 1, 0, 0, 0 ], [ 1, 0, 8, 1, 1, 0, 0, 0 ], [ 2, 0, 3, 1, 1, 0, 0, 0 ], [ 3, 8, 8, 1, 1, 0, 0, 0 ], [ 6, 7, 8, 1, 1, 0, 0, 0 ], [ 5, 3, 8, 1, 1, 0, 0, 0 ], [ 3, 1, 3, 1, 1, 0, 0, 0 ], [ 0, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 9, 4, 1, 1, 0, 0, 0 ], [ 3, 10, 10, 1, 1, 0, 0, 12360 ], [ 6, 10, 10, 1, 0, 0, 0, 12360 ], [ 6, 4, 8, 1, 1, 0, 0, 0 ], [ 7, 6, 10, 1, 0, 0, 0, 12360 ], [ 7, 5, 8, 0, 0, 1, 0, 0 ], [ 3, 7, 1, 1, 1, 0, 0, 12353 ] ],
// 3986, 12355, 1, 1, 1, 1, 1738, 0, 1 ]  
// 
// [ 0, 0, 0, 1, 10254, 1, 89, 3, 1070, 
// [ [ 325, "\u51af\u6d77\u68ee\u5821", 207, 0, 7857, 0, 0, 26, 0, 4, 4, 211, 100 ], [ 19824, "\u5929\u624d\u5e05\u5e05.s51", 207, 12, 8120, 0, 0, 100, 0, 4, -1, 224623, 100 ], [ 1836, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51", 201, 11, 8120, 1, 0, 100, 1, 5, -1, 212358, 100 ] ], 
// [ [ 3, 1 ], [ 4, 8 ], [ 1, 100 ], [ 2, 2 ] ], 
// [ [ 4, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 1, 8, 1, 1, 0, 0, 0 ], [ 5, 5, 7, 1, 0, 0, 0, 0 ], [ 4, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 4, 8, 1, 1, 0, 0, 0 ], [ 8, 3, 8, 0, 0, 1, 0, 0 ], [ 3, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 4, 4, 1, 1, 0, 0, 0 ], [ 3, 0, 8, 1, 1, 0, 0, 0 ], [ 8, 5, 3, 0, 0, 1, 0, 0 ], [ 0, 8, 10, 0, 0, 0, 0, 12360 ], [ 1, 10, 2, 0, 0, 1, 0, 12354 ], [ 5, 10, 8, 1, 1, 0, 0, 0 ], [ 1, 7, 8, 1, 1, 0, 0, 0 ], [ 7, 4, 10, 1, 0, 0, 0, 12360 ], [ 8, 6, 8, 0, 0, 1, 0, 0 ], [ 0, 1, 8, 1, 1, 0, 0, 0 ], [ 0, 3, 8, 1, 1, 0, 0, 0 ], [ 8, 7, 8, 0, 0, 1, 0, 0 ], [ 3, 2, 8, 1, 1, 0, 0, 0 ], [ 4, 8, 7, 1, 0, 0, 0, 0 ], [ 3, 6, 8, 1, 1, 0, 0, 0 ], [ 8, 4, 4, 0, 0, 1, 0, 0 ], [ 5, 6, 8, 1, 1, 0, 0, 0 ], [ 3, 3, 8, 1, 1, 0, 0, 0 ], [ 7, 7, 8, 1, 1, 0, 0, 0 ], [ 1, 5, 7, 1, 0, 0, 0, 0 ], [ 2, 7, 8, 1, 1, 0, 0, 0 ], [ 0, 0, 3, 1, 1, 0, 0, 0 ], [ 5, 2, 8, 1, 1, 0, 0, 0 ], [ 5, 0, 4, 1, 0, 0, 0, 0 ], [ 7, 10, 8, 0, 0, 1, 0, 0 ], [ 2, 10, 10, 1, 0, 0, 0, 12360 ], [ 2, 6, 3, 1, 1, 0, 0, 0 ], [ 4, 3, 4, 1, 1, 0, 0, 0 ], [ 5, 8, 8, 1, 1, 0, 0, 0 ], [ 2, 4, 8, 1, 1, 0, 0, 0 ], [ 0, 6, 4, 1, 1, 0, 0, 0 ], [ 6, 6, 3, 1, 1, 0, 0, 0 ], [ 3, 4, 7, 1, 0, 0, 0, 0 ], [ 4, 1, 8, 1, 1, 0, 0, 0 ], [ 7, 3, 1, 0, 0, 1, 0, 12353 ], [ 1, 4, 8, 1, 1, 0, 0, 0 ], [ 4, 10, 7, 1, 0, 0, 0, 0 ], [ 4, 6, 8, 1, 1, 0, 0, 0 ], [ 6, 3, 8, 1, 1, 0, 0, 0 ], [ 2, 5, 8, 1, 1, 0, 0, 0 ], [ 1, 6, 3, 1, 1, 0, 0, 0 ], [ 5, 7, 8, 1, 1, 0, 0, 0 ], [ 6, 5, 3, 0, 0, 1, 0, 0 ], [ 4, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 2, 5, 1, 1, 0, 1, 0 ], [ 3, 5, 8, 1, 1, 0, 0, 0 ], [ 2, 3, 6, 1, 1, 0, 1, 0 ], [ 4, 0, 4, 1, 1, 0, 0, 0 ], [ 5, 4, 2, 1, 1, 0, 0, 12354 ], [ 0, 2, 8, 1, 1, 0, 0, 0 ], [ 1, 3, 8, 1, 1, 0, 0, 0 ], [ 1, 0, 8, 1, 1, 0, 0, 0 ], [ 2, 0, 3, 1, 1, 0, 0, 0 ], [ 3, 8, 8, 1, 1, 0, 0, 0 ], [ 6, 7, 8, 1, 1, 0, 0, 0 ], [ 5, 3, 8, 1, 1, 0, 0, 0 ], [ 3, 1, 3, 1, 1, 0, 0, 0 ], [ 0, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 9, 4, 1, 1, 0, 0, 0 ], [ 3, 10, 10, 1, 1, 0, 0, 12360 ], [ 6, 10, 10, 1, 0, 0, 0, 12360 ], [ 6, 4, 8, 1, 1, 0, 0, 0 ], [ 7, 6, 10, 1, 0, 0, 0, 12360 ], [ 7, 5, 8, 0, 0, 1, 0, 0 ], [ 3, 7, 1, 1, 1, 0, 0, 12353 ] ], 
// 20986, 12355, 1, 1, 0, 1, 1774, 0, 0 ] 

//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_map_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 12);
}

//============================================================================
//	玄天古境：玄天历练
// "module":770,"action":14,
// "request":[Utils.IntUtil],
// 
// AncientRealmTaskView.as
//			_data.call(Mod_AncientRealm_Base.experience_panel,param1,[this._taskTpe],true,DataBase.SOCKET_SYS,_ctrl.sectLogin.currentGroupInfo.socketName);
// 
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
// 
// AncientRealmData.as		
//			         oObject.list(param1,this._objTask,["experience_mission_list","is_active_title"]);
//					 oObject.list(_loc4_[_loc2_],_loc3_,["mission_id","now_value","is_finish","is_get"]);
// Example
//		id=1，开拓	[ [ [ 4, 2, 1, 1 ], [ 2, 60, 1, 1 ], [ 5, 5, 1, 1 ], [ 3, 90, 1, 1 ], [ 1, 30, 1, 1 ] ], 1 ] 
//		id=2，破敌	[ [ [ 6, 30, 1, 1 ], [ 9, 498, 1, 1 ], [ 10, 2, 0, 0 ], [ 8, 33, 0, 0 ], [ 7, 33, 0, 0 ] ], 0 ] 
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_experience_panel(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 14);
}

//============================================================================
//	玄天古境：获取经验奖励
// "module":770,"action":15,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmTaskView.as
//			_data.call(Mod_AncientRealm_Base.get_experience_award,this.get_experience_award_call_back,[param1.type_id,param1.mission_id],true,DataBase.SOCKET_SYS,_ctrl.sectLogin.currentGroupInfo.socketName);
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_experience_award(int type, int id)
{
	Json::Value data;
	data.append(type);
	data.append(id);
	return this->send_and_receive(data, 770, 15);
}

//============================================================================
//	玄天古境：打开地图格子
// "module":770,"action":18,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmMapView.as
//          this._curOpenGridPos.x = param1;
//			this._curOpenGridPos.y = param2;
//			_data.call(Mod_AncientRealm_Base.open_grid,this.open_grid_callback,[param1 - 1,param2 - 1],true,DataBase.SOCKET_SYS,this._groupInfo.socketName);
//
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil]
//		
// AncientRealmData.as		
//			         this.result = param1[0];
//						this.openGridInfo = Tool2Copy.copyO(AncientRealmType.getAncientRealmMapGridTypeData(param1[1]));
//						this.openGridInfo.get_coin_num = param1[2];
// Example
//============================================================================
//
// 1.护境使者；2.玄境灵兽；3.加成格；4.奖励格；5.钥匙格1；6.钥匙格2；7.路障格；8.普通格；9.游商格；10.古境戍卫
// 
Json::Value sxd_client::Mod_AncientRealm_Base_open_grid(int x, int y)
{
	Json::Value data;
	data.append(x);
	data.append(y);
	return this->send_and_receive(data, 770, 18);
}

//============================================================================
//	玄天古境：挑战怪物
// "module":770,"action":19,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmMonsterInfoView.as
//			_data.call(Mod_AncientRealm_Base.fight_monster,this.challenge_callBack,[this._monsterInfo.x,this._monsterInfo.y],true,DataBase.SOCKET_SYS,this._groupInfo.socketName);
// 
// "response":
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_fight_monster(int floor, int id)
{
	Json::Value data;
	data.append(floor);
	data.append(id);
	return this->send_and_receive(data, 770, 19);
}

//============================================================================
//	玄天古境：战利品信息
// "module":770,"action":20,
// "request":[]
// 
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.spoilPanelInfo,["ball_list","awake_list","weapon_list"]);
//					var _loc6_:Array = ["grid_id","id","level","grade"];
// Example
//			[ [ [ 2, 67, 0, 3 ], [ 3, 56, 0, 3 ], [ 1, 66, 0, 3 ] ], 
//			[ [ 2, 5528, 5, 3 ], [ 3, 7143, 5, 3 ], [ 1, 5325, 5, 3 ] ], 
//			[ [ 3, 19, 0, 3 ], [ 2, 18, 0, 3 ], [ 1, 22, 0, 3 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_spoil_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 20);
}

//============================================================================
//	玄天古境：选择战利品
// "module":770,"action":21,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmSpoilView.as
/*				this._curItem = param2;
				if (param2.is_trans_point)
				{
					_data.call(Mod_AncientRealm_Base.choose_spoil, this.choose_spoil_callback, [1, 0, 0], true, DataBase.SOCKET_SYS, this._groupInfo.socketName);
				}
				else
				{
					_data.call(Mod_AncientRealm_Base.choose_spoil, this.choose_spoil_callback, [0, param1, param2.grid_id], true, DataBase.SOCKET_SYS, this._groupInfo.socketName);
				}*/
//
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_choose_spoil(int id1, int id2, int id3)
{
	Json::Value data;
	data.append(id1);
	data.append(id2);
	data.append(id3);
	return this->send_and_receive(data, 770, 21);
}

//============================================================================
//	玄天古境：移动位置
// "module":770,"action":22,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmSpoilView.as
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_movement(int x, int y)
{
	Json::Value data;
	data.append(x);
	data.append(y);
	return this->send_and_receive(data, 770, 22);
}

//============================================================================
//	玄天古境：挑战BOSS
// "module":770,"action":23,
// "request":[]
// 
// "response":
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_fight_boss()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 23);
}

//============================================================================
//	玄天古境：前往下一层
// "module":770,"action":24,
// "request":[]
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_click_pass()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 24);
}

//============================================================================
//	玄天古境：获取怪物团信息
// "module":770,"action":27,
// "request":[Utils.IntUtil],
// 
// AncientRealmMonsterInfoView.as
//			_data.call(Mod_AncientRealm_Base.get_monster_team_info,this.get_monster_team_info_callback,[this._monsterInfo.monster_team_id],true,DataBase.SOCKET_SYS,this._groupInfo.socketName);
//
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_monster_team_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 27);
}

//============================================================================
//	玄天古境：打开格子宝箱
// "module":770,"action":30,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AncientRealmMapView.as
/*			this._curOpenGridPos.x = param1;
			this._curOpenGridPos.y = param2;
			_data.call(Mod_AncientRealm_Base.open_award_grid_box, this.open_award_grid_box_callback, [param1 - 1, param2 - 1], true, DataBase.SOCKET_SYS, this._groupInfo.socketName);
*/
// 
// "response":[Utils.UByteUtil,Utils.IntUtil]
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_open_award_grid_box(int x, int y)
{
	Json::Value data;
	data.append(x);
	data.append(y);
	return this->send_and_receive(data, 770, 30);
}