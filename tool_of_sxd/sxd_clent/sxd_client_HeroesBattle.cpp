#include "stdafx.h"
#include <vector>
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//积分赛
class HeroesBattleScoreRaceType
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int NOT_SCORE_RACE_STEP = 2;
	static const int FAILED = 3;
	static const int NOT_ENOUGH_VALUE = 7;
	static const int ALREADY_GET = 8;

};

void sxd_client::HeroesBattleScoreRace()
{
	common::log("【群英战积分赛】：！");
	Json::Value data = this->Mod_HeroesBattleScoreRace_Base_score_race_panel();
	int fight_num = data[13].asInt();
	while (fight_num)
	{
		data = this->Mod_HeroesBattleScoreRace_Base_find_opponent();
		data = this->Mod_HeroesBattleScoreRace_Base_fight();
		if (data[0].asInt() == HeroesBattleScoreRaceType::SUCCESS)
		{
			common::log("【群英战积分赛】：挑战成功！");
		}
		else if (data[0].asInt() == HeroesBattleScoreRaceType::FAILED)
		{
			common::log("【群英战积分赛】：挑战失败！");
		}
		data = this->Mod_HeroesBattleScoreRace_Base_score_race_panel();
		fight_num = data[13].asInt();
	}

	data = this->Mod_HeroesBattleScoreRace_Base_award_panel();

	vector<int> fight_award_list, score_award_list;

	//挑战奖励
	for (Json::Value temp : data[4])
	{
		if (!temp[1].asInt())
		{
			fight_award_list.push_back(temp[0].asInt());
		}
	}

	//段位奖励
	for (Json::Value temp : data[5])
	{
		if (!temp[1].asInt())
		{
			score_award_list.push_back(temp[0].asInt());
		}
	}

	//领取挑战奖励
	if (!fight_award_list.size())
	{
		common::log("【群英战积分赛】：挑战奖励已领取！");
	}
	else
	{
		for (int id : fight_award_list)
		{
			data = this->Mod_HeroesBattleScoreRace_Base_get_award(1, id);
			if (data[0].asInt() == HeroesBattleScoreRaceType::SUCCESS)
			{
				common::log("【群英战积分赛】：领取挑战奖励成功！");
			}
			else if (data[0].asInt() == HeroesBattleScoreRaceType::ALREADY_GET)
			{
				common::log("【群英战积分赛】：挑战奖励已领取！");
			}			
		}
	}

	//领取段位奖励
	if (!score_award_list.size())
	{
		common::log("【群英战积分赛】：挑战奖励已领取！");
	}
	else
	{
		for (int id : score_award_list)
		{
			data = this->Mod_HeroesBattleScoreRace_Base_get_award(2, id);
			if (data[0].asInt() == HeroesBattleScoreRaceType::SUCCESS)
			{
				common::log("【群英战积分赛】：领取段位奖励成功！");
			}
			else if (data[0].asInt() == HeroesBattleScoreRaceType::ALREADY_GET)
			{
				common::log("【群英战积分赛】：挑战段位已领取！");
			}
			else if (data[0].asInt() == HeroesBattleScoreRaceType::NOT_ENOUGH_VALUE)
			{
				common::log("【群英战积分赛】：积分不足！");
			}
		}
	}
}

//============================================================================
//  群英战-积分赛-面板
// {module:792, action:1,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// 
// Example
// HeroesBattleScoreRaceData.as
//     oObject.list(param1,_loc2_,["","reg_id","war_zone","flower_num","post_num","buy_post_num","is_award_red_dot","team_id","team_name","score","rank","need_up_score","win_rate","fight_num","member_list","opponent_list"]);
//			[ 0, 18593, 2, 10, 0, 0, 0, 18593, "\u6d77\u68ee\u5821", 1160, 0, 0, 86, 10, [ [ 18593, 1, "360_s313", 211, "\u51af\u6d77\u68ee\u5821", 0, 0, 69854528, 207, 7412, 0, 0, "", "\u6d77\u68ee\u5821", 1, 0, 7857 ], [ 23090, 0, "2133_s026", 163190, "\u4e1c\u65b9\u72b9\u5c06.s3", 0, 0, 123161275, 511, 6133, 0, 0, "", "\u6d77\u68ee\u5821", 3, 7, 7150 ], [ 19860, 0, "plu_s01", 6579, "\u90ac\u911e\u8d85.s1", 0, 0, 118442957, 509, 8883, 0, 0, "", "\u6d77\u68ee\u5821", 2, 9, 7564 ] ], null ] 
//============================================================================
Json::Value sxd_client::Mod_HeroesBattleScoreRace_Base_score_race_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 792, 1);
}

//============================================================================
//  群英战-积分赛-奖励面板
// {module:792, action:4,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]]
// 
// Example
// HeroesBattleScoreRaceData.as
//     oObject.list(param1,this._awardPanelInfo,["result","today_fight_num","today_win_num","team_highest_score","fight_award_list","score_award_list"]);
//			[ 0, 10, 4, 1215, [ [ 3, 1 ], [ 4, 1 ], [ 1, 1 ], [ 2, 1 ] ], [ [ 5, 0 ], [ 3, 0 ], [ 4, 0 ], [ 1, 1 ], [ 2, 0 ], [ 6, 0 ] ] ] 
//			[ 0, 10, 4, 1235, [ [ 3, 0 ], [ 4, 0 ], [ 1, 0 ], [ 2, 0 ] ], [ [ 5, 0 ], [ 3, 0 ], [ 4, 0 ], [ 1, 0 ], [ 2, 0 ], [ 6, 0 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_HeroesBattleScoreRace_Base_award_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 792, 4);
}

//============================================================================
//  群英战-积分赛-领取奖励
// {module:792, action:5,
// request:[Utils.IntUtil,Utils.IntUtil],
// Example		
//		this._getId = param2;
//     _data.call(Mod_HeroesBattleScoreRace_Base.get_award,this.get_award_callBack,[param1,param2]);
// response:[Utils.UByteUtil]
// 
// Example
// HeroesBattleScoreRaceData.as
//     this.result = param1[0];
//			[ 2, 1736870400, 1737734399, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_HeroesBattleScoreRace_Base_get_award(int type, int id)
{
	Json::Value data;
	data.append(type);
	data.append(id);
	return this->send_and_receive(data, 792, 5);
}


//============================================================================
//  群英战-积分赛-寻找对手
// {module:792, action:6,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil]
// 
// Example
// HeroesBattleScoreRaceData.as
//     this.result = param1[0];
//			[ 2, 1736870400, 1737734399, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_HeroesBattleScoreRace_Base_find_opponent()
{
	Json::Value data;
	return this->send_and_receive(data, 792, 6);
}

//============================================================================
//  群英战-积分赛-战斗
// {module:792, action:8,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// HeroesBattleScoreRaceData.as
//     this.result = param1[0];
//		
//============================================================================
Json::Value sxd_client::Mod_HeroesBattleScoreRace_Base_fight()
{
	Json::Value data;
	return this->send_and_receive(data, 792, 8);
}