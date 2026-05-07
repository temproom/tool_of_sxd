#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class WorldPkRankingtype
{
public:
	static const int ON = 0;
	static const int OF = 1;
	static const int SUPER_TOWN_CLOSE = 2;		//仙界未开放
	static const int SUCCESS = 3;
	static const int ACTIVITY_NOT_OPEN = 4;		//活动未开启
	static const int FAILED = 5;
	static const int NOT_COMPLETED = 21;
	static const int LOW_LEVEL = 22;
}; 

void sxd_client::WorldPkRanking()
{
	//common::log("【阶位赛】任务面板！！");
	Json::Value data = this->Mod_WorldPkRanking_Base_open_task_panel();

	Json::Value week_task_list = data[1];
	Json::Value season_task_list = data[2];
	int cur_level = data[3].asInt();
	Json::Value level_award_listt = data[7];

	//每周任务奖励
	for (Json::Value temp : week_task_list)
	{
		int task_id = temp[0].asInt();
		int complete_val = temp[1].asInt();
		int get_flag = temp[2].asInt();
		
		if (!get_flag)
		{
			Json::Value data_task = this->Mod_WorldPkRanking_Base_get_task_award(task_id);
			if (data_task[0].asInt() == WorldPkRankingtype::SUCCESS)
			{
				common::log("【阶位赛】领取每周任务奖励成功！！");
			}
		}
	}

	//赛季任务奖励
	for (Json::Value temp : season_task_list)
	{
		int task_id = temp[0].asInt();
		int complete_val = temp[1].asInt();
		int get_flag = temp[2].asInt();

		if (!get_flag)
		{
			Json::Value data_task = this->Mod_WorldPkRanking_Base_get_task_award(task_id);
			if (data_task[0].asInt() == WorldPkRankingtype::SUCCESS)
			{
				common::log("【阶位赛】领取赛季任务奖励成功！！");
			}
		}
	}

	//免费等级奖励
	for (Json::Value temp : level_award_listt)
	{
		int level = temp[0].asInt();
		int free_get_flag = temp[2].asInt();

		if (!free_get_flag)
		{
			Json::Value data_task = this->Mod_WorldPkRanking_Base_get_level_free_award(level);
			if (data_task[0].asInt() == WorldPkRankingtype::SUCCESS)
			{
				common::log("【阶位赛】免费等级奖励成功！！");
			}
		}
	}
}

//============================================================================
// 阶位赛：获取状态
// {module:349, action:0, 
// request:[],
// 
// WorldPkRankingView.as
// 
// 
// response:[Utils.UByteUtil,Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this.activityObject,["status","season","open_time","close_time"]);
// Example
//     [ 1, 78, 1746979200, 1749398340 ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_get_status()
{
	Json::Value data;
	return this->send_and_receive(data, 349, 0);
}

//============================================================================
// 阶位赛：打开面板
// {module:349, action:2, 
// request:[],
// 
// WorldPkRankingView.as
// 
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,[Utils.IntUtil,Utils.LongUtil],Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this.panelObject,["apply_id","nickname","servername","stagename","role_id","equip_item_id","player_level","score","highest_score","step_id","fight_times","win_times","pk_coins","medalCount","theme_id","special_id","now_value","award_list","award_is_get","remain_time","season_remain_time","auto_match","skip_war","cd_time","left_skip_time","self_rank","cur_level","award_item_id","award_item_num","have_award"]);
// Example
//     [ 3, 0, "\u51af\u6d77\u68ee\u5821", "360_s313", "360", 207, 8091, 300, 5303, 5619, 6, 236, 119, 35, 3, 4, 9, 0, [ [ 5425, 20 ], [ 4063, 3 ], [ 6422, 1 ] ], 0, 374068, 2188408, 0, 0, 1747057801, 10, 0, 4, 5116, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 349, 2);
}

//============================================================================
// 阶位赛：奖励面板
// {module:349, action:25, 
// request:[],
// 
// WorldPkRankingView.as
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this.ruleInfo,["result","top_score","top_step","now_score","now_step"]);
// Example
//    [ 3, 5619, 6, 5303, 6 ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_award_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 349, 25);
}

//============================================================================
// 阶位赛：任务面板
// {module:349, action:26, 
// request:[],
// 
// WorldPkRankingView.as
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this.taskInfo,["result","week_task_list","season_task_list","cur_level","cur_exp","today_charge_val","left_charge_val","level_award_list"]);
//		_loc6_.task_id = param1[_loc5_][0];
//		_loc6_.complete_val = param1[_loc5_][1];
//		_loc6_.get_flag = param1[_loc5_][2];
// 
//		_loc5_.level = param1[_loc4_][0];
//		_loc5_.tran_flag = param1[_loc4_][1];
//		_loc5_.free_get_flag = param1[_loc4_][2];
//		_loc5_.gift_buy_flag = param1[_loc4_][3];
// Example
//      [ 3, 
//		[ [ 13, 0, 0 ], [ 4, 600000000, 1 ], [ 10, 0, 0 ], [ 9, 0, 0 ], [ 1, 20, 1 ], [ 11, 3, 1 ] ], 
//		[ [ 22, 3, 1 ], [ 17, 686, 0 ], [ 14, 500, 1 ], [ 21, 0, 0 ], [ 18, 686, 0 ], [ 24, 5, 0 ], [ 20, 686, 0 ], [ 23, 5, 1 ], [ 19, 686, 0 ], [ 16, 686, 0 ], [ 15, 686, 0 ] ], 
//		4, 200, 0, 0, 
//		[ [ 1, 0, 1, 0 ], [ 2, 0, 1, 0 ], [ 3, 0, 1, 0 ], [ 4, 0, 1, 0 ], [ 5, 0, 0, 0 ], [ 6, 0, 0, 0 ], [ 7, 0, 0, 0 ], [ 8, 0, 0, 0 ], [ 9, 1, 0, 0 ], [ 10, 1, 0, 0 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_open_task_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 349, 26);
}

//============================================================================
// 阶位赛：获取任务奖励
// {module:349, action:27, 
// request:[Utils.IntUtil],
// 
// WorldPkRankingView.as
// response:[Utils.UByteUtil]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this._objGetState,["state_id","season","is_func_open","is_have_team"]);
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_get_task_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 349, 27);
}

//============================================================================
// 阶位赛：获取等级免费奖励
// {module:349, action:28, 
// request:[Utils.IntUtil],
// 
// WorldPkRankingView.as
// response:[Utils.UByteUtil]
// 
// WorldPkRankingData.as
//     oObject.list(param1,this._objGetState,["state_id","season","is_func_open","is_have_team"]);
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_WorldPkRanking_Base_get_level_free_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 349, 28);
}