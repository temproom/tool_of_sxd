#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class AbyssClashtype
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int WRONG_STEP = 2;
	static const int FAILED = 3;
	static const int NO_FIGHT_NUM = 5;
	static const int BOSS_CAN_NOT_FIGHT = 6;
	static const int NUM_LIMIT = 7;
	static const int NOT_ENOUGH_SCORE = 8;
};

void sxd_client::AbyssClash(int step)
{
	if (step == 1)
	{
		Json::Value data = this->Mod_AbyssClash_Base_step1_panel_info();
		int fight_num = data[3].asInt();
		Json::Value monster_list = data[6];

		if (!fight_num)
		{
			common::log("【界渊之战】：阶段一，挑战次数不足！！");
			return;
		}
		while (fight_num)
		{
			for (Json::Value monster : monster_list)
			{
				int monster_id = monster[0].asInt();
				Json::Value fight_data = this->Mod_AbyssClash_Base_start_fight(monster_id);
				if (fight_data[0].asInt() == AbyssClashtype::SUCCESS)
				{
					common::log("【界渊之战】：阶段一，挑战成功！！");
					fight_num--;
				}
			}
		}
	}
	else if (step == 2)
	{
		Json::Value data = this->Mod_AbyssClash_Base_step2_panel_info();
		int fight_num = data[3].asInt();
		Json::Value monster_list = data[7];

		if (!fight_num)
		{
			common::log("【界渊之战】：阶段二，挑战次数不足！！");
		}
		while (fight_num)
		{
			for (Json::Value monster : monster_list)
			{
				int monster_id = monster[0].asInt();
				int is_boss = monster[2].asInt();
				if (!is_boss)
				{
					Json::Value fight_data = this->Mod_AbyssClash_Base_start_fight(monster_id);
					if (fight_data[0].asInt() == AbyssClashtype::SUCCESS)
					{
						common::log("【界渊之战】：阶段二，挑战成功！！");
						fight_num--;
					}
				}			
			}
		}

		//打BOSS
		data = this->Mod_AbyssClash_Base_step2_panel_info();
		monster_list = data[7];
		for (Json::Value monster : monster_list)
		{
			int monster_id = monster[0].asInt();
			int is_boss = monster[2].asInt();
			int boss_can_fight_num = monster[3].asInt();
			if (is_boss)
			{
				common::log(boost::str(boost::format("【界渊之战】：阶段二，BOOS可挑战次数：[%1%]。") % boss_can_fight_num));
				while (boss_can_fight_num)
				{
					Json::Value fight_data = this->Mod_AbyssClash_Base_start_fight(monster_id);
					if (fight_data[0].asInt() == AbyssClashtype::SUCCESS)
					{
						common::log("【界渊之战】：阶段二，挑战BOSS成功！！");
						boss_can_fight_num--;
					}
				}
				break;
			}
		}

		//积分兑换
		Json::Value shop_data = this->Mod_AbyssClash_Base_score_store_info();
		int score = shop_data[1].asInt();
		Json::Value score_exchange_list = shop_data[0];
		common::log(boost::str(boost::format("【界渊之战】：剩余积分：[%1%]。") % score));
		//auto it = std::find_if(score_exchange_list.begin(), score_exchange_list.end(), [](Json::Value& good) {return good[0].asInt() == 1; });
		
		int red_left_time = 10, gold_left_time = 20;
		for (Json::Value good : score_exchange_list)
		{
			if (good[0].asInt() == 1)
			{
				red_left_time -= good[1].asInt();
				common::log(boost::str(boost::format("【界渊之战】：红书碎片剩余可购买次数：[%1%]。") % red_left_time));
			}
			else if (good[0].asInt() == 2)
			{
				gold_left_time -= good[1].asInt();
				common::log(boost::str(boost::format("【界渊之战】：金书碎片剩余可购买次数：[%1%]。") % gold_left_time));
			}
		}

		while (red_left_time && score >= 800)
		{
			Json::Value buy_data = this->Mod_AbyssClash_Base_score_exchange(1, 1);
			if (buy_data[0].asInt() == AbyssClashtype::SUCCESS)
			{
				common::log("【界渊之战】：购买红书碎片成功！！");
				score -= 800;
				red_left_time--;
			}
			else if (buy_data[0].asInt() == AbyssClashtype::NUM_LIMIT)
			{
				common::log("【界渊之战】：红书碎片已售罄！！");
				break;
			}
			else if (buy_data[0].asInt() == AbyssClashtype::NOT_ENOUGH_SCORE)
			{
				common::log("【界渊之战】：积分不足！！");
				break;
			}
		}

		while(!red_left_time && gold_left_time && score >= 500)
		{
			Json::Value buy_data = this->Mod_AbyssClash_Base_score_exchange(2, 1);
			if (buy_data[0].asInt() == AbyssClashtype::SUCCESS)
			{
				common::log("【界渊之战】：购买金书碎片成功！！");
				score -= 500;
				gold_left_time--;
			}
			else if (buy_data[0].asInt() == AbyssClashtype::NUM_LIMIT)
			{
				common::log("【界渊之战】：金书碎片已售罄！！");
				break;
			}
			else if (buy_data[0].asInt() == AbyssClashtype::NOT_ENOUGH_SCORE)
			{
				common::log("【界渊之战】：积分不足！！");
				break;
			}
		}
	}
	
}

//============================================================================
//	界渊之战：检查功能是否开启
// "module":818,"action":0,
// "request":[],
// 
// AbyssClashview.as
// 
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// 
// AbyssClashdata.as		
/*this._openFlag = param1[0];
         this._step = param1[1];
         this._playerSuperTownServerId = param1[2];*/
// Example
//			[ 1, 1, 12421 ]
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_is_func_open()
{
	Json::Value data;
	return this->send_and_receive(data, 818, 0);
}

//============================================================================
//	界渊之战：阶段一面板信息
// "module":818,"action":1,
// "request":[],
// 
// AbyssClashview.as
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// 
// AbyssClashdata.as		
//			oObject.list(param1,this._info,["result","is_first_open","score","fight_num","cost_power","next_step_ts","monster_list","map_player_list","zone_id","st_team_id","total_score"]);
//					oObject.list(_loc2_[_loc5_],_loc3_,["step_monster_id","monster_team_id","win_num","last_health","max_health","war_special_id"]);
//					oObject.list(_loc2_[_loc5_],_loc3_,["st_player_id","nickname","role_id","cloth_id","mount_id"]);
// Example
//			[ 0, 0, 3000, 4, 260, 1752595200, [ [ 32, 12937, 5, 129701191377752, 134400000000000, 63 ], [ 31, 12936, 6, 69647102747679, 134400000000000, 38 ], [ 33, 12938, 2, 127377493063013, 134400000000000, 37 ] ], [ [ 10531, "\u51af\u8bfa\u4f9d\u66fc", 205, 0, 7500 ], [ 6851, "\u96ea\u65e0\u75d5", 1, 0, 9647 ], [ 13884, "\u6797\u660a\u5929", 5, 6133, 0 ], [ 13474, "\u5f61\u5f61\u6653\u6708", 6, 1872, 0 ], [ 13185, "\u7ea2\u5854\u5c71", 507, 5000, 0 ], [ 15332, "\u67f3\u98de\u8fdf", 5, 0, 5519 ], [ 9634, "\u73cd\u73a5", 1, 0, 9323 ], [ 6735, "\u4e1c\u65b9\u32a5\u5251\u4ed9", 509, 0, 9647 ], [ 10633, "\u51af\u6d77\u68ee\u5821", 207, 0, 7500 ], [ 10633, "\u51af\u6d77\u68ee\u5821", 207, 0, 7500 ] ], 6, 50, 3000 ] 
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_step1_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 818, 1);
}

//============================================================================
//	界渊之战：阶段二面板信息
// "module":818,"action":2,
// "request":[],
// 
// AbyssClashview.as
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil],Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// 
// AbyssClashdata.as		
//			oObject.list(param1,this._info,["result","is_first_open","score","fight_num","cost_power","next_step_ts","player_win_num","monster_list","award_times","boss_top_3","map_player_list","zone_id","st_team_id","total_score"]);
//					 oObject.list(_loc2_[_loc5_],_loc3_,["step_monster_id","monster_team_id","is_boss","boss_can_fight_num","win_num","last_health","max_health"]);
//					oObject.list(_loc2_[_loc5_],_loc3_,["st_player_id","nickname","server_name","role_id","head_id","rim_id"]);
//					oObject.list(_loc2_[_loc5_],_loc3_,["st_player_id","nickname","role_id","cloth_id","mount_id"]);
// Example
//			[ 0, 1, 7000, 5, 250, 1752681600, 0, [ [ 34, 12939, 0, 0, 20, 67200000000000, 67200000000000 ], [ 36, 12941, 1, 0, 0, 268800000000000, 268800000000000 ], [ 35, 12940, 0, 0, 5, 67200000000000, 67200000000000 ] ], 100, [ [ 13904, "\u95fb\u4eba\u7eea", "s1609", 511, 0, 7857 ], [ 6770, "\u897f\u95e8\u2606\u5439\u96ea", "37wan_s816", 2, 8, 7150 ], [ 9634, "\u73cd\u73a5", "kuwan8_s1030", 1, 11, 7857 ] ], [ [ 13910, "\u534e\u9752\u6d77", 507, 0, 5376 ], [ 5152, "\u79bb\u6b4c", 510, 3049, 0 ], [ 7533, "\u7a7a\u767d", 512, 0, 5845 ], [ 9551, "\u662f\u9177\u9ca8\u9ca8\u5440\uff01", 1, 0, 9323 ], [ 12350, "\u4e1c\u65b9\u7b71\u6d45", 207, 1872, 0 ], [ 13187, "\u5c0f\u4e2b", 510, 7411, 0 ], [ 13474, "\u5f61\u5f61\u6653\u6708", 6, 1872, 0 ], [ 13185, "\u7ea2\u5854\u5c71", 507, 5000, 0 ], [ 9634, "\u73cd\u73a5", 1, 0, 9323 ], [ 13428, "\u98ce\u745f\u5e7b\u60f3", 197, 0, 6126 ], [ 13429, "\u51e4\u9e23\u4e5d\u5929", 6, 0, 5611 ], [ 13907, "\u515c\u515c\u6709\u7cd6", 508, 4924, 0 ], [ 13884, "\u6797\u660a\u5929", 5, 6133, 0 ], [ 6851, "\u96ea\u65e0\u75d5", 1, 0, 9647 ], [ 3691, "\u5df2\u4e0d\u518d\u662f\u5c11\u5e74", 510, 2406, 0 ], [ 13184, "\u963f\u8bd7\u739b", 508, 5000, 0 ], [ 13188, "\u4e09\u75af", 509, 7411, 0 ], [ 13904, "\u95fb\u4eba\u7eea", 511, 5159, 0 ], [ 6770, "\u897f\u95e8\u2606\u5439\u96ea", 2, 0, 0 ], [ 10633, "\u51af\u6d77\u68ee\u5821", 207, 0, 7500 ] ], 6, 50, 7000 ] 
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_step2_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 818, 2);
}

//============================================================================
//	界渊之战：获取怪物队伍信息
// "module":818,"action":3,
// "request":[Utils.IntUtil],
// 
// AbyssClashview.as
//			 _data.call(Mod_AbyssClash_Base.get_monster_team_info,param1,[this._monsterInfo.info.step_monster_id],true,DataBase.SOCKETSERVERTOWN);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//		
// 
// AbyssClashdata.as		
//			oObject.list(param1,this._monsterInfo,["result","zone_id","monster_team_id","monster_level","war_special_id","monsters"]);
//					oObject.list(_loc3_,_loc4_,["monster_id","grid_id","stunt_id","supernatural_id","rebirth_skill_id"]);
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_get_monster_team_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 818, 3);
}

//============================================================================
//	界渊之战：战斗
// "module":818,"action":4,
// "request":[Utils.IntUtil],
// 
// AbyssClashview.as
//		_data.call(Mod_AbyssClash_Base.start_fight,this.refresh_callback,[this._monsterInfo.info.step_monster_id],true,DataBase.SOCKETSERVERTOWN);
// 
// "response":
//		
// 
// AbyssClashdata.as		
//			this._result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_start_fight(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 818, 4);
}

//============================================================================
//	界渊之战：积分商店
// "module":818,"action":7,
// "request":[],
// 
// AbyssClashview.as
//		_data.call(Mod_AbyssClash_Base.start_fight,this.refresh_callback,[this._monsterInfo.info.step_monster_id],true,DataBase.SOCKETSERVERTOWN);
// 
// "response":
//		[[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]

// 
// AbyssClashdata.as		
//			oObject.list(param1,this._scoreStoreInfo,["score_exchange_list","score","end_ts"]);
// Example
//			[ [ [ 5, 0 ], [ 8, 0 ], [ 3, 0 ], [ 4, 0 ], [ 7, 0 ], [ 1, 10 ], [ 2, 4 ], [ 6, 0 ] ], 0, 1753026600 ] 
// 
//			"id":1,,"item_id":10304,,"item_num" : 1,,"need_score" : 800,,"limit" : 10
//			1,10304,800,10 红书碎片
//			2,10305,500,20 金书碎片
//			3,10306,300,50 蓝书碎片
//			4,1411,200,20  女娲石碎片
//			5,8643,400,50  随机功法残片
//			6,3050,4,5000  魔石碎片
//			7,7892,400,20  盘古魂碎片
//			8,1842,600,50  太极灵宝
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_score_store_info()
{
	Json::Value data;
	return this->send_and_receive(data, 818, 7);
}

//============================================================================
//	界渊之战：积分兑换
// "module":818,"action":8,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// AbyssClashShopview.as
//		_data.call(Mod_AbyssClash_Base.score_exchange,this.score_exchange_callback,[param1.id,param2],true,DataBase.SOCKETSERVERTOWN);
// 
// "response":
//		[Utils.UByteUtil]
// 
// AbyssClashdata.as		
//			this._result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_AbyssClash_Base_score_exchange(int id, int num)
{
	Json::Value data;
	data.append(id);
	data.append(num);
	return this->send_and_receive(data, 818, 8);
}