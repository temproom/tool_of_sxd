#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Monopolytype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int TOP_GRADE = 6;
	static const int IS_GET = 13;
};


void sxd_client::Monopoly()
{
	vector<vector<int>> building_grid = { { 101, 127 }, { 101, 102 }, { 102, 103 }, { 103, 104 }, { 104, 105 } ,{ 105, 107 },{ 107, 108 },{ 108, 109 },{ 109, 111 },{ 111, 112 },
	{ 112, 113 } ,{ 113, 115 } ,{ 115, 116 } ,{ 116, 117 } ,{ 117, 118 } ,{ 118, 119 } ,{ 119, 121 } ,{ 121, 122 } ,{ 122, 123 }, {123, 125 }, {125, 126 },{ 126, 127 } };
	/* {
		vector<int> v1{ 101, 127 };
		vector<int> v2{ 101, 102 };
		vector<int> v3{ 102, 103 };
		vector<int> v4{ 103, 104 };
		vector<int> v5{ 104, 105 };
		vector<int> v6{ 105, 107 };
		vector<int> v7{ 107, 108 };
		vector<int> v8{ 108, 109 };
		vector<int> v9{ 109, 111 };
		vector<int> v10{ 111, 112 };
		vector<int> v11{ 112, 113 };
		vector<int> v12{ 113, 115 };
		vector<int> v13{ 115, 116 };
		vector<int> v14{ 116, 117 };
		vector<int> v15{ 117, 118 };
		vector<int> v16{ 118, 119 };
		vector<int> v17{ 119, 121 };
		vector<int> v18{ 121, 122 };
		vector<int> v19{ 122, 123 };
		vector<int> v20{ 123, 125 };
		vector<int> v21{ 125, 126 };
		vector<int> v22{ 126, 127 };

		building_grid.push_back(v1);
		building_grid.push_back(v2);
		building_grid.push_back(v3);
		building_grid.push_back(v4);
		building_grid.push_back(v5);
		building_grid.push_back(v6);
		building_grid.push_back(v7);
		building_grid.push_back(v8);
		building_grid.push_back(v9);
		building_grid.push_back(v10);
		building_grid.push_back(v11);
		building_grid.push_back(v12);
		building_grid.push_back(v13);
		building_grid.push_back(v14);
		building_grid.push_back(v15);
		building_grid.push_back(v16);
		building_grid.push_back(v17);
		building_grid.push_back(v18);
		building_grid.push_back(v19);
		building_grid.push_back(v20);
		building_grid.push_back(v21);
		building_grid.push_back(v22);
	}*/

	//总共建筑物数量
	vector<int> building_num{ 0,8,3,6,3,2,2,2,2 };

	//领取活跃度奖励
	Json::Value data_task = this->Mod_Monoploy_Base_get_task_award(1);
	{
		if (data_task[0].asInt() == Monopolytype::IS_GET)
		{
			common::log("【山河游历】活跃度奖励一，已领取！");
		}
		else if (data_task[0].asInt() == Monopolytype::SUCCESS)
		{
			common::log("【山河游历】活跃度奖励一，领取成功！");
		}

		data_task = this->Mod_Monoploy_Base_get_task_award(2);
		if (data_task[0].asInt() == Monopolytype::IS_GET)
		{
			common::log("【山河游历】活跃度奖励二，已领取！");
		}
		else if (data_task[0].asInt() == Monopolytype::SUCCESS)
		{
			common::log("【山河游历】活跃度奖励二，领取成功！");
		}

		data_task = this->Mod_Monoploy_Base_get_task_award(3);
		if (data_task[0].asInt() == Monopolytype::IS_GET)
		{
			common::log("【山河游历】活跃度奖励三，已领取！");
		}
		else if (data_task[0].asInt() == Monopolytype::SUCCESS)
		{
			common::log("【山河游历】活跃度奖励三，领取成功！");
		}
	}

	//面板信息
	Json::Value data = this->Mod_Monoploy_Base_main_panel();
	if (data[0].asInt() != Monopolytype::SUCCESS)
	{
		common::log(boost::str(boost::format("【山河游历】打开面板失败，代码： [%1%]") % data[0].asInt()), iEdit);
		return;
	}

	Json::Value buildings = data[1];

	//骰子数量
	int dice_num = data[3].asInt();

	//当前格子位置
	int now_grid_id = data[5].asInt();

	//是否可操纵，4
	//0：不可操作；1：可升级或建造
	int operation_flag = data[6].asInt();

	if (operation_flag == 1)
	{
		//可操作的格子id
		int grid_id1 = building_grid[now_grid_id-1][0];
		int grid_id2 = building_grid[now_grid_id-1][1];

		Json::Value building1, building2;

		//查找格子的建筑
		for (Json::Value building : buildings)
		{			
			//记录建筑物数量
			//每有一个，剩余可建数量减一
			building_num[building[1].asInt()]--;

			if (building[0].asInt() == grid_id1)
			{
				building1 = building;
			}

			if (building[0].asInt() == grid_id2)
			{
				building2 = building;
			}
		}

		//空格，建造建造
		if (!building1[1].asInt())
		{
			//w一号位是空的
			for (int i = 8; i > 0; i--)
			{
				if (building_num[i] != 0)
				{
					Json::Value data_build = this->Mod_Monoploy_Base_build(building1[0].asInt(), i);
					if (data_build[0].asInt() == Monopolytype::SUCCESS)
					{
						common::log("【山河游历】建造成功！");
						building_num[i]--;
					}
					else
					{
						common::log(boost::str(boost::format("【山河游历】建造失败，代码： [%1%]") % data_build[0].asInt()), iEdit);
						return;
					}					
				}
			}
		}
		else if(!building2[1].asInt())
		{
			//二号位是空的
			for (int i = 8; i > 0; i--)
			{
				if (building_num[i] != 0)
				{
					Json::Value data_build = this->Mod_Monoploy_Base_build(building2[0].asInt(), i);
					if (data_build[0].asInt() == Monopolytype::SUCCESS)
					{
						common::log("【山河游历】建造成功！");
						building_num[i]--;
					}
					else
					{
						common::log(boost::str(boost::format("【山河游历】建造失败，代码： [%1%]") % data_build[0].asInt()), iEdit);
						return;
					}				
				}
			}
		}

		//都不是空的
		//升级建筑
		upgrade_building(building1, building2);
	}

	while (dice_num > 0)
	{
		//投掷骰子
		data = this->Mod_Monoploy_Base_roll_dice();
		if (data[0].asInt() != Monopolytype::SUCCESS)
		{
			common::log(boost::str(boost::format("【山河游历】投掷骰子失败，代码： [%1%]") % data[0].asInt()), iEdit);
			return;
		}

		Json::Value data_panel = this->Mod_Monoploy_Base_main_panel();

		{
			now_grid_id = data_panel[5].asInt();
			//可操作的格子id
			int grid_id1 = building_grid[now_grid_id - 1][0];
			int grid_id2 = building_grid[now_grid_id - 1][1];

			Json::Value building1, building2;

			//查找格子的建筑
			for (Json::Value building : buildings)
			{
				if (building[0].asInt() == grid_id1)
				{
					building1 = building;
				}

				if (building[0].asInt() == grid_id2)
				{
					building2 = building;
				}
			}
			//空格，建造建造
			if (!building1[1].asInt())
			{
				//w一号位是空的
				for (int i = 8; i > 0; i--)
				{
					if (building_num[i] != 0)
					{
						Json::Value data_build = this->Mod_Monoploy_Base_build(building1[0].asInt(), i);
						if (data_build[0].asInt() == Monopolytype::SUCCESS)
						{
							common::log("【山河游历】建造成功！");
							building_num[i]--;
						}
						else
						{
							common::log(boost::str(boost::format("【山河游历】建造失败，代码： [%1%]") % data_build[0].asInt()), iEdit);
							return;
						}
					}
				}
			}
			else if (!building2[1].asInt())
			{
				//二号位是空的
				for (int i = 8; i > 0; i--)
				{
					if (building_num[i] != 0)
					{
						Json::Value data_build = this->Mod_Monoploy_Base_build(building2[0].asInt(), i);
						if (data_build[0].asInt() == Monopolytype::SUCCESS)
						{
							common::log("【山河游历】建造成功！");
							building_num[i]--;
						}
						else
						{
							common::log(boost::str(boost::format("【山河游历】建造失败，代码： [%1%]") % data_build[0].asInt()), iEdit);
							return;
						}
					}
				}
			}

			//都不是空的
			//升级建筑
			upgrade_building(building1, building2);
			dice_num--;
		}
	}

	data = this->Mod_Monoploy_Base_get_product();
	if (data[0].asInt() == Monopolytype::SUCCESS)
	{
		common::log("【山河游历】领取产出成功！");
	}
	else
	{
		common::log(boost::str(boost::format("【山河游历】领取产出失败，代码： [%1%]") % data[0].asInt()), iEdit);
		return;
	}
}

void sxd_client::upgrade_building(Json::Value building1, Json::Value building2)
{
	//building[2]越大，建筑品质越好
	if (building1[2].asInt() < 20 && building2[2].asInt() < 20)
	{
		if (building1[1].asInt() > building2[1].asInt())
		{
			Json::Value data_upgrade = this->Mod_Monoploy_Base_upgrade(building1[0].asInt());
			if(data_upgrade[0].asInt() == Monopolytype::SUCCESS)
			{
				common::log("【山河游历】升级成功！");
			}
			else
			{
				common::log(boost::str(boost::format("【山河游历】升级失败，代码： [%1%]") % data_upgrade[0].asInt()), iEdit);
			}
			return;
		}
		else
		{
			Json::Value data_upgrade = this->Mod_Monoploy_Base_upgrade(building2[0].asInt());
			if (data_upgrade[0].asInt() == Monopolytype::SUCCESS)
			{
				common::log("【山河游历】升级成功！");
			}
			else
			{
				common::log(boost::str(boost::format("【山河游历】升级失败，代码： [%1%]") % data_upgrade[0].asInt()), iEdit);
			}
			return;
		}
	}
	else if (building1[2].asInt() < 20)
	{
		Json::Value data_upgrade = this->Mod_Monoploy_Base_upgrade(building1[0].asInt());
		if (data_upgrade[0].asInt() == Monopolytype::SUCCESS)
		{
			common::log("【山河游历】升级成功！");
		}
		else
		{
			common::log(boost::str(boost::format("【山河游历】升级失败，代码： [%1%]") % data_upgrade[0].asInt()), iEdit);
		}
		return;
	}
	else if (building2[2].asInt() < 20)
	{
		Json::Value data_upgrade = this->Mod_Monoploy_Base_upgrade(building2[0].asInt());
		if (data_upgrade[0].asInt() == Monopolytype::SUCCESS)
		{
			common::log("【山河游历】升级成功！");
		}
		else
		{
			common::log(boost::str(boost::format("【山河游历】升级失败，代码： [%1%]") % data_upgrade[0].asInt()), iEdit);
		}
		return;
	}
	else
	{
		common::log("【山河游历】两个格子都已满级！");
		return;
	}
}


//============================================================================
// 面板信息
// "module":748,"action":1,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[[Utils.IntUtil,Utils.LongUtil,Utils.LongUtil],Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//oObject.list(param1,_loc2_,["result","buildings","products","dice_num","next_get_time","now_grid_id","operation_flag","first_flag","max_dice_num","privilege_flag","extra_dice_num"])      };
// 
// buildings:["grid_id","building_id","grade","cd_time"]
// Example
//     
/*
1:[101,127,128],
2: [101, 102] ,
3 : [102, 103] ,
4 : [103, 104] ,
5 : [104, 105] ,
6 : [105, 106, 107] ,
7 : [107, 108] ,
8 : [108, 109] ,
9 : [109, 110, 111] ,
10 : [111, 112] ,
11 : [112, 113] ,
12 : [113, 114, 115] ,
13 : [115, 116] ,
14 : [116, 117] ,
15 : [117, 118] ,
16 : [118, 119] ,
17 : [119, 120, 121] ,
18 : [121, 122] ,
19 : [122, 123] ,
20 : [123, 124, 125] ,
21 : [125, 126] ,
22 : [126, 127] */
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_main_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 748, 1);
}

//============================================================================
// 投掷骰子
// "module":748,"action":2,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);

// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_roll_dice()
{
	Json::Value data;
	return this->send_and_receive(data, 748, 2);
}

//============================================================================
// 建造
// "module":748,"action":3,
// "request":[Utils.IntUtil,Utils.IntUtil],
//this._gridId = param1;
//this._buildingId = param2;
// 
// "response":
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_build(int gridId, int buildingId)
{
	Json::Value data;
	data.append(gridId);
	data.append(buildingId);
	return this->send_and_receive(data, 748, 3);
}

//============================================================================
// 升级
// "module":748,"action":4,
// "request"[Utils.IntUtil],
//this._gridId = param1; :
// 
// "response":
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_upgrade(int gridId)
{
	Json::Value data;
	data.append(gridId);
	return this->send_and_receive(data, 748, 4);
}

//============================================================================
// 获取产出
// "module":748,"action":7,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_get_product()
{
	Json::Value data;
	return this->send_and_receive(data, 748, 7);
}

//============================================================================
// 任务面板信息
// "module":748,"action":8,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil]]]
// result=data[0]
// data[1]:
//oObject.list(_loc5_, _loc4_, ["task_type", "value", "lock_flag", "get_award_list"]);

// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_task_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 748, 8);
}

//============================================================================
// 获取任务奖励
// "module":748,"action":9,
// "request":[Utils.IntUtil],
//this._missionId = param1;
// 
// "response":
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_get_task_award(int missionId)
{
	Json::Value data;
	data.append(missionId);
	return this->send_and_receive(data, 748, 9);
}

//============================================================================
// 获取所有产出
// "module":748,"action":11,
// "request":
// "response":
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Monoploy_Base_get_total_product()
{
	Json::Value data;
	return this->send_and_receive(data, 748, 11);
}