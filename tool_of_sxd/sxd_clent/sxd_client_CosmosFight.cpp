#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class CosmosFightType
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int WRONG_ID = 2;
	static const int FAILED = 4;
	static const int NO_OPPONENT = 8;
	static const int NO_TIMES = 10;
};

void sxd_client::CosmosFight()
{
	//获取状态
	Json::Value data = this->Mod_CosmosFight_Base_get_state();
	time_t nowtime;
	time(&nowtime);
	int now_time = (int)nowtime;
	if (now_time > data[1].asInt() && now_time < data[2].asInt())
	{
		common::log("【寰宇乱斗】：活动开启！！");
	}
	else
	{
		common::log("【寰宇乱斗】：活动关闭！！");
		return;
	}

	//获取礼包面板
	data = this->Mod_CosmosFight_Base_gift_box_panel();
	int free_gift = data[0][0][2].asInt();
	if (free_gift > 0)
	{
		//可领取免费礼包
		data = this->Mod_CosmosFight_Base_get_or_buy_gift_box(1, 1);
		int result = data[0].asInt();
		if (result == CosmosFightType::SUCCESS)
		{
			common::log("【寰宇乱斗】：领取活跃度礼包成功！！");
		}
	}
	else
	{
		common::log("【寰宇乱斗】：无可领取活跃度礼包！！");
	}

	//获取信息面板
	data = this->Mod_CosmosFight_Base_get_panel_info();

	int is_can_fight = data[0].asInt();
	if (!is_can_fight)
	{
		common::log("【寰宇乱斗】：非挑战时间！！");
		return;
	}

	//活力点数（可挑战次数）
	int player_activity = data[11].asInt();

	if (player_activity == 0)
	{
		common::log("【寰宇乱斗】：活力点数不足！！");
		return;
	}

	vector<int> bless_info{
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3 };

	if (data[1].asInt() == 1)
	{/*
		//选择祝福
		Json::Value data_temp = this->Mod_CosmosFight_Base_bless_or_treasure_panel_info(1);
		vector<int>bless_id;
		for (Json::Value  bless_lists : data_temp[1])
		{
			bless_id.push_back(bless_lists[3].asInt());
		}
		//按表降序
		std::sort(bless_id.begin(), bless_id.end(), [&](const int& a, const int& b)->bool {return bless_info[a] > bless_info[b]; });

		data_temp = this->Mod_CosmosFight_Base_choose_bless_or_treasure(1, bless_id[0]);
		int result = data_temp[0].asInt();
		if (result == CosmosFightType::SUCCESS)
		{
			common::log("【寰宇乱斗】：选择祝福成功成功！！");
		}
		*/
		choosebless();
	}

	if (data[2].asInt() == 1)
	{
		//选择奇物
		//Json::Value data_temp = this->Mod_CosmosFight_Base_bless_or_treasure_panel_info(2);
		common::log("【寰宇乱斗】：请自行选取奇物！！！");
		return;
	}

	while (player_activity > 0)
	{
		//匹配对手
		data = this->Mod_CosmosFight_Base_find_opponent();
		//挑战
		data = this->Mod_CosmosFight_Base_fight();
		int fight_result = data[1].asInt();
		if (fight_result == 1)
		{
			//战斗失败，退出战斗，匹配下一个对手
			common::log("【寰宇乱斗】：战斗失败，逃离！！！");
			data = this->Mod_CosmosFight_Base_exit_fight();
		}

		//获取信息面板
		data = this->Mod_CosmosFight_Base_get_panel_info();

		//活力点数（可挑战次数）
		player_activity = data[11].asInt();

		if (data[1].asInt() == 1)
		{
			choosebless();
		}
		if (data[2].asInt() == 1)
		{
			//选择奇物
			//Json::Value data_temp = this->Mod_CosmosFight_Base_bless_or_treasure_panel_info(2);
			common::log("【寰宇乱斗】：请自行选取奇物！！！");
			return;
		}
	}	
}

void sxd_client::choosebless()
{
	vector<int> bless_info{
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3,
	1,1,1,1,2,2,2,3,3 };

	//选择祝福
	Json::Value data_temp = this->Mod_CosmosFight_Base_bless_or_treasure_panel_info(1);
	//vector<int>bless_id;
	int id, bless_id = 1;
	for (Json::Value bless_lists : data_temp[1])
	{
		//祝福原id
		int temp = bless_lists[3].asInt();

		//查表比较大小
		if (bless_info[bless_id] <= bless_info[temp])
		{
			bless_id = temp;
			id = bless_lists[0].asInt();
		}

		//bless_id.push_back(bless_lists[3].asInt());
	}
	//按表降序
	//std::sort(bless_id.begin(), bless_id.end(), [&](const int& a, const int& b)->bool {return bless_info[a] > bless_info[b]; });

	data_temp = this->Mod_CosmosFight_Base_choose_bless_or_treasure(1, id);
	int result = data_temp[0].asInt();
	if (result == CosmosFightType::SUCCESS)
	{
		common::log("【寰宇乱斗】：选择祝福成功！！");
	}
}

//============================================================================
//  寰宇乱斗-状态
// {module:763, action:0,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//     oObject.list(param1,this._objState,["state","end_apply_ts","end_fight_ts","is_apply"]);
//			[ 2, 1736870400, 1737734399, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_CosmosFight_Base_get_state()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 0);
}

//============================================================================
//  寰宇乱斗-信息
// {module:763, action:2,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//     oObject.list(param1,this._objPanelInfo,["is_can_fight","is_need_choose_bless","is_need_choose_treasure","law_id_list","nickname","stage_name","server_name","win_num","lose_num","player_score","player_coin","player_activity","can_get_award_box_num","is_skip_fight","battle_player_info","opponent_star","opponent_weapon_elem_info","opponent_ball_elem_info","opponent_awake_elem_info","last_fight_result","rank_list"]);
//		[ 1, 0, 0, null, "\u51af\u8bfa\u4f9d\u66fc", "360", "360_s313", 0, 0, 1000, 0, 32, 0, 1, [ [ 2, 0, 0, 0, 0, 0, 205, 11, 8123, [ [ 854, 7 ], [ 858, 1 ], [ 709, 4 ], [ 205, 8 ], [ 552, 51 ], [ 633, 9 ] ] ] ], 0, null, null, null, 0, null ] 
//		[ 1, 1, 0, null, "\u51af\u8bfa\u4f9d\u66fc", "360", "360_s313", 2, 0, 1020, 60, 30, 1, 1, [ [ 2, 0, 0, 0, 1, 6, 205, 11, 8123, [ [ 854, 7 ], [ 858, 1 ], [ 709, 4 ], [ 205, 8 ], [ 552, 51 ], [ 633, 9 ] ] ] ], 0, null, null, null, 0, null ] 
//============================================================================
Json::Value sxd_client::Mod_CosmosFight_Base_get_panel_info()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 2);
}

//============================================================================
//  寰宇乱斗-寻找对手
// {module:763, action:3,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//     this.result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_CosmosFight_Base_find_opponent()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 3);
}

//============================================================================
//  寰宇乱斗-战斗
// {module:763, action:4,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//     oObject.list(param1,this.objFight,["result","fight_result","war_report_id","report_version","score","experience","add_coin"]);
// [ 0, 2, 617014, "2025011001", 10, 12, 30 ] 
// fight_result：
//			1战斗失败，2三星胜利，3二星胜利，4一星胜利
//============================================================================
Json::Value sxd_client::Mod_CosmosFight_Base_fight()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 4);
}

//============================================================================
//  寰宇乱斗-退出战斗
// {module:763, action:5,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//    this.result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_CosmosFight_Base_exit_fight()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 5);
}

//【非攻】1-4低级，5-7中级，8-9高级
//【锋锐】10-13低级，14-16中级，17-18高级
//【气盛】19-22低级，23-25中级。26-27高级
//【方休】28-31低级，32-34中级，35-36高级
//============================================================================
//  寰宇乱斗-祝福面板
// {module:763, action:8,
// request:[Utils.IntUtil],
// Example		
/*            if(_ctrl.cosmosFight.getObjPanelInfo.is_need_choose_bless)
				{
					this._type = 1;
				}
			else if (_ctrl.cosmosFight.getObjPanelInfo.is_need_choose_treasure)
				{
				this._type = 2;
				}*/
//			_data.call(Mod_CosmosFight_Base.bless_or_treasure_panel_info,this.bless_or_treasure_panel_info_call_back,[this._type]);
// 
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//    this.result = param1[0];
// oObject.list(param1,this._objTreasurePanelInfo,["result","arr_info","coin_num","type"]);
// oObject.list(this._objTreasurePanelInfo.arr_info[_loc2_],_loc3_,["id","replace_num","can_replace_num","ble_or_tre_id","star_id","replace_need_coin"]);
// 祝福面板
//			[ 0, [ [ 1, 0, 1, 12, 1, 10 ], [ 3, 0, 1, 3, 0, 10 ], [ 2, 0, 1, 36, 0, 10 ] ], 90, 1 ] 
//============================================================================
Json::Value  sxd_client::Mod_CosmosFight_Base_bless_or_treasure_panel_info(int type)
{
	Json::Value data;
	data.append(type);
	return this->send_and_receive(data, 763, 8);
}

//============================================================================
//  寰宇乱斗-选择祝福或奇物
// {module:763, action:10,
// request:[Utils.IntUtil,Utils.IntUtil],
// Example		
//     _data.call(Mod_CosmosFight_Base.choose_bless_or_treasure,this.choose_bless_or_treasure_call_back,[this._type,param1]);
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//    this.result = param1[0];
//============================================================================
Json::Value  sxd_client::Mod_CosmosFight_Base_choose_bless_or_treasure(int type, int id)
{
	Json::Value data;
	data.append(type);
	data.append(id);
	return this->send_and_receive(data, 763, 10);
}

//============================================================================
//  寰宇乱斗-礼包信息
// {module:763, action:14,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//    oObject.list(param1,this._objGiftBoxPanel,["putong_gift","buy_gift","charge_ingot"]);
//  oObject.list(this._objGiftBoxPanel.putong_gift[_loc2_],_loc3_,["id","already_get_num","can_get_num"]);
// [ [ [ 1, 4, 0 ] ], [ [ 5, 0 ], [ 3, 0 ], [ 4, 0 ], [ 1, 0 ], [ 2, 0 ], [ 6, 0 ] ], 42629 ] 
//============================================================================
Json::Value  sxd_client::Mod_CosmosFight_Base_gift_box_panel()
{
	Json::Value data;
	//data.append(town_id);
	return this->send_and_receive(data, 763, 14);
}

//============================================================================
//  寰宇乱斗-领取礼包
// {module:763, action:15,
// request:[],
// Example		
//     _data.call(Mod_CosmosFight_Base.get_or_buy_gift_box,this.get_or_buy_gift_box_call_back,[this._obj.gift_type,this._obj.id]);
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// CosmosFightData.as
//    this.result = param1[0];
//============================================================================
Json::Value  sxd_client::Mod_CosmosFight_Base_get_or_buy_gift_box(int gift_type, int id)
{
	Json::Value data;
	data.append(gift_type);
	data.append(id);
	return this->send_and_receive(data, 763, 15);
}