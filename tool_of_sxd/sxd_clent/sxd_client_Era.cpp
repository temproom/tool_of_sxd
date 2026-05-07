#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class EraInfiniteMissionType	//无量劫海挑战
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int IS_LOCK = 2;
	static const int ALREADY_PASS = 3;
	static const int ALREADY_END = 4;
	static const int FAILED = 5;
	static const int WRONG_ID = 6;
};

class EraRebirthLandAutoType	//轮回净地挂机
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int ALREADY_START = 2;
	static const int FAILED = 3;
	static const int NO_CAN_GET_GIFT = 4;//无可领取礼包
	static const int NO_FREE_STORAGE = 8;
	static const int NO_TIMES = 9;		
	static const int NO_PROFIT = 10;	//无收益
};

class EraTreasureOrderType	//纪元战令
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int ALREADY_GET = 2;
	static const int NOT_ENOUGH_LEVEL = 3;
	static const int FAILED = 6;
	static const int IS_GET = 11;
	static const int TASK_NOT_COMPLETED = 12;
};

void sxd_client::Era()
{
	//EraInfiniteMission();	// 无量劫海挑战
	EraRebirthLandAuto();	// 挂机
	EraRebirthLandAttr();	// 属性
	EraTreasureOrder();		// 战令
}

//============================================================================
// 无量劫海挑战
//============================================================================
void sxd_client::EraInfiniteMission()
{
	Json::Value data = this->Mod_EraInfiniteMission_Base_get_panel_info();
	int mission_level = data[1].asInt();
	int now_mission = mission_level + 1;

	data = this->Mod_EraInfiniteMission_Base_get_chapter_panel(4);

	int times = 1;
	while(times <= 100)
	{
		data = this->Mod_EraInfiniteMission_Base_fight(now_mission);
		int result = data[0].asInt();
		if (result != EraInfiniteMissionType::SUCCESS)
		{
			common::log(boost::str(boost::format("【无量劫海】：挑战错误:【%1%】") % result));
			return;
		}

		int flag = 1;
		for (Json::Value temp : data[1])
		{
			int is_win = temp[1].asInt();
			if (!is_win)
			{
				//有一个队伍失败，则挑战失败
				flag = is_win;
				break;
			}
		}
		if (flag)
		{
			//挑战成功
			common::log(boost::str(boost::format("【无量劫海】：第[%1%]次挑战第【%2%】关成功！！") % times % now_mission));
			now_mission++;
			times = 1;
		}
		else
		{
			//挑战失败
			common::log(boost::str(boost::format("【无量劫海】：第[%1%]次挑战第【%2%】关失败！！") % times % now_mission));
			times++;
		}
	}
}

//============================================================================
// 无量劫海挑战--获取当前赛季
// "module":853,"action":0,
// "request":[],
// "response":[Utils.IntUtil]
//
// EraInfiniteView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EEraInfiniteData.as:
//	get_auto_panel_info
//			oObject.list(param1,this._autoPanelInfo,["season","auto_level_gift_list","player_era_level","player_auto_level","player_last_open_level","is_start","has_auto_ts","auto_ts_limit","psychic_add_pro","base_pass_rate","psychic_add_rate","add_pass_rate","use_free_quick","use_ingot_quick","top1_info"]);
// 
// Example
//		[ 1 ] 
//============================================================================
Json::Value sxd_client::Mod_EraInfiniteMission_Base_get_now_season()
{
	Json::Value data;
	return this->send_and_receive(data, 853, 0);
}

//============================================================================
// 无量劫海挑战--面板信息
// "module":853,"action":1,
// "request":[],
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//
// EraInfiniteView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EEraInfiniteData.as:
//		oObject.list(param1,this._objEraInfinite,["era_level","mission_level","rank_perc","rank","fight_end_ts","era_zone"]);
// 
// Example
//			[ 81, 30, 0, 0, 1772466900, 1 ]
//			[ 93, 42, 0, 0, 1772466900, 2 ]
//============================================================================
Json::Value sxd_client::Mod_EraInfiniteMission_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 853, 1);
}

//============================================================================
// 无量劫海挑战--获取章节信息
// "module":853,"action":2,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil]],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//
// EraInfiniteView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EEraInfiniteData.as:
//	oObject.list(param1,this._objChapterPanel,["result","era_level","rule_list","now_level","lower_con","monster_list","best_player","fastest_player","rank_perc","rank","chapter_id"]);
// 
// Example
//		[ 0, 81 , [ [ 1 ] ], 31, 0, [ [ 1, 30641 ], [ 2, 30597 ], [ 3, 30601 ] ], [ [ 62295, "4399_s0117", "\u5756.s19", 510, 6, 7728, "4399_s0117@9x247.sxdweb.xd.com", [ [ 1, 1728511, "2025110701" ], [ 2, 1728512, "2025110701" ], [ 3, 1728513, "2025110701" ] ] ] ], [ [ 10855, "s0190", "\u5072\u8bd7.s378", 109, 8, 7564, "s0190@9x253.sxdweb.xd.com", [ [ 1, 1429125, "2025110701" ], [ 2, 1429126, "2025110701" ], [ 3, 1429127, "2025110701" ] ] ] ], 0, 0, 3 ] 
//		[ 0, 450, [ [ 1 ] ], 62, 0, [ [ 1, 30672 ], [ 2, 30638 ], [ 3, 30649 ] ], [ [ 0, "", "", 0, 0, 0, "", null ] ], [ [ 0, "", "", 0, 0, 0, "", null ] ], 91, 0, 4, 45, [ [ 10563, 0 ], [ 10564, 0 ], [ 10565, 0 ], [ 10566, 0 ] ] ] 
// 第一章【1-10】
// 第二章【11-30】
// 第三章【31-60】
// 第四章【61-300】
//============================================================================
Json::Value sxd_client::Mod_EraInfiniteMission_Base_get_chapter_panel(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 853, 2);
}

//============================================================================
// 无量劫海挑战--获取怪物队伍信息
// "module":853,"action":3,
// "request":[Utils.IntUtil,Utils.IntUtil],
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//
// EraInfiniteMissionDeployFightView.as:
//          _data.call(Mod_EraInfiniteMission_Base.get_monster_team_info,param1,[this._level,this._curDeployType]);
// 
// EEraInfiniteData.as:
//	get_auto_panel_info
//			oObject.list(param1,this._autoPanelInfo,["season","auto_level_gift_list","player_era_level","player_auto_level","player_last_open_level","is_start","has_auto_ts","auto_ts_limit","psychic_add_pro","base_pass_rate","psychic_add_rate","add_pass_rate","use_free_quick","use_ingot_quick","top1_info"]);
// 
// Example
// 
//============================================================================
Json::Value sxd_client::Mod_EraInfiniteMission_Base_get_monster_team_info(int id, int type)
{
	Json::Value data;
	data.append(id);
	data.append(type);
	return this->send_and_receive(data, 853, 3);
}

//============================================================================
// 无量劫海挑战--战斗
// "module":853,"action":4,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil]]
//20260218
// "request":[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],
// "response":[Utils.UByteUtil, [Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.StringUtil], Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil]]

// EraInfiniteChapterView.as:
//          _data.call(Mod_EraInfiniteMission_Base.fight,this.fightCallback,[_loc1_]);
// 20260218
//		    _data.call(Mod_EraInfiniteMission_Base.fight,this.fightCallback,[_loc2_,param1]);

// EEraInfiniteData.as:
//		this._result = param1[0];
//			oObject.list(_loc3_,_loc2_,["seq","is_win","war_report_id","report_version"]);
// 
// Example
//			[ 6, null ]
//			[ 0, [ [ 1, 1, 7484709, "2025110701" ], [ 2, 1, 7484710, "2025110701" ], [ 3, 0, 7484711, "2025110701" ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EraInfiniteMission_Base_fight(int id)
{
	Json::Value data;
	data.append(id);
	data.append(1);
	return this->send_and_receive(data, 853, 4);
}

//============================================================================
// 轮回净地挂机
//============================================================================
void sxd_client::EraRebirthLandAuto()
{
	Json::Value data = this->Mod_EraRebirthLandAuto_Base_get_panel_info();
	//已使用免费挂机次数
	int use_free_quick = data[12].asInt();
	if (use_free_quick < 2)
	{
		int i = 2 - use_free_quick;
		while (i > 0)
		{
			common::log("【轮回净地·挂机】：免费快速挂机成功！！");
			data = this->Mod_EraRebirthLandAuto_Base_quick_auto();
			i--;
		}		
	}
	//领取等级奖励
	data = this->Mod_EraRebirthLandAuto_Base_get_level_gift();
	common::log("【轮回净地·挂机】：领取等级奖励成功！！");
	//领取挂机奖励
	data = this->Mod_EraRebirthLandAuto_Base_get_auto_profit();
	common::log("【轮回净地·挂机】：领取挂机奖励成功！！");
	//data = this->Mod_EraRebirthLandAuto_Base_quick_auto();
}

//============================================================================
// 轮回净地挂机--面板信息
// "module":852,"action":1,
// "request":[],
// "response":[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//	get_auto_panel_info
//			oObject.list(param1,this._autoPanelInfo,["season","auto_level_gift_list","player_era_level","player_auto_level","player_last_open_level","is_start","has_auto_ts","auto_ts_limit","psychic_add_pro","base_pass_rate","psychic_add_rate","add_pass_rate","use_free_quick","use_ingot_quick","top1_info"]);
// 
// Example
//			[ 1, [ [ 19, 0 ], [ 5, 1 ], [ 20, 0 ], [ 26, 0 ], [ 8, 1 ], [ 3, 1 ], [ 4, 1 ], [ 7, 1 ], [ 12, 0 ], [ 21, 0 ], [ 17, 0 ], [ 1, 1 ], [ 24, 0 ], [ 11, 0 ], [ 25, 0 ], [ 13, 0 ], [ 28, 0 ], [ 23, 0 ], [ 16, 0 ], [ 10, 0 ], [ 9, 0 ], [ 14, 0 ], [ 18, 0 ], [ 2, 1 ], [ 22, 0 ], [ 15, 0 ], [ 27, 0 ], [ 6, 1 ] ], 81, 84, 82, 1, 25606, 172800, 0, 100, 0, 40, 0, 0, [ [ "\u591c\u5e55\u5b64\u661f.s88", "xdwan_s0266", 107, 21, 8194, 324 ] ] ] 
//			[ 1, [ [ 19, 0 ], [ 5, 1 ], [ 20, 0 ], [ 26, 0 ], [ 8, 1 ], [ 3, 1 ], [ 4, 1 ], [ 7, 1 ], [ 12, 0 ], [ 21, 0 ], [ 17, 0 ], [ 1, 1 ], [ 24, 0 ], [ 11, 0 ], [ 25, 0 ], [ 13, 0 ], [ 28, 0 ], [ 23, 0 ], [ 16, 0 ], [ 10, 0 ], [ 9, 0 ], [ 14, 0 ], [ 18, 0 ], [ 2, 1 ], [ 22, 0 ], [ 15, 0 ], [ 27, 0 ], [ 6, 1 ] ], 82, 84, 84, 1, 3634, 172800, 0, 100, 0, 100, 1, 0, [ [ "\u8427\u5929.s52", "ledu_s034", 99, 21, 8123, 325 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 1);
}

//============================================================================
// 轮回净地挂机--开始挂机
// "module":852,"action":2,
// "request":[],
// "response":[Utils.UByteUtil]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
// 
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_start_auto()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 2);
}

//============================================================================
// 轮回净地挂机--领取等级礼包
// "module":852,"action":3,
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
//			[ 4, null ]
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_get_level_gift()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 3);
}

//============================================================================
// 轮回净地挂机--找回快速挂机面板
// "module":852,"action":4,
// "request":[],
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
//		[ [ [ 1763135700, 2, 0 ], [ 1762876500, 2, 0 ], [ 1763308500, 2, 0 ], [ 1763049300, 2, 0 ], [ 1762962900, 2, 0 ], [ 1763222100, 2, 0 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_quick_auto_back_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 4);
}

//============================================================================
// 轮回净地挂机--一键领取免费快速挂机奖励
// "module":852,"action":6,
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.LongUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
//		[ 8, null ]
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_one_click_quick_free_back()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 6);
}

//============================================================================
// 轮回净地挂机--快速挂机
// "module":852,"action":8,
// "request":[],
// "response":Utils.UByteUtil,[Utils.IntUtil,Utils.LongUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
// 
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_quick_auto()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 8);
}
//============================================================================
// 轮回净地挂机--领取挂机收益
// "module":852,"action":9,
// "request":[],
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.LongUtil]]
//
// EraRebirthLandView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// 
// EraRebirthLandData.as:
//			
// Example
//		[ 0, 25607, [ [ 10383, 2626880 ], [ 10384, 10507520 ], [ 10385, 204856 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAuto_Base_get_auto_profit()
{
	Json::Value data;
	return this->send_and_receive(data, 852, 9);
}

//============================================================================
//	轮回净地属性
//============================================================================
void sxd_client::EraRebirthLandAttr() 
{
	for(;;)
	{
		Json::Value data = this->Mod_EraRebirthLandAttr_Base_level_up();
		if (data[0].asInt() == 0)
		{
			common::log("【轮回净地】：升级成功！！");
		}
		else if (data[0].asInt() != 0)
		{
			common::log("【轮回净地】：升级失败！！");
			return;
		}
	}
}
//============================================================================
// 轮回净地属性--升级
// "module":851,"action":3,
// "request":[],
// "response":[Utils.UByteUtil]//
// SEraTreasureOrderView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
//
// EraTreasureOrderData.as:
//			oObject.list(param1,this.panelInfo,["player_level","player_exp","award_list","is_unlock_ingot","is_unlock_charge_ingot","player_ingot","player_charge_ingot","season"]);
// Example
//			[ 0 ]
//============================================================================
Json::Value sxd_client::Mod_EraRebirthLandAttr_Base_level_up()
{
	Json::Value data;
	return this->send_and_receive(data, 851, 3);
}

//============================================================================
// 纪元战令
//============================================================================
void sxd_client::EraTreasureOrder()
{
	//每周任务【1-8】，赛季等级【9-17】，赛季关卡【18-23】，赛季劫海【24-30】
	for (int i = 1; i <= 8; i++)
	{
		Json::Value data_task = this->Mod_EraTreasureOrder_Base_get_task(i);
		if (data_task[0].asInt() == EraTreasureOrderType::SUCCESS)
		{
			common::log(boost::str(boost::format("【纪元战令】：领取每周任务【%1%】成功！！") % i));
		}
		else if (data_task[0].asInt() == EraTreasureOrderType::TASK_NOT_COMPLETED)
		{
			common::log(boost::str(boost::format("【纪元战令】：每周任务【%1%】未完成！！") % i));
			break;
		}
		
	}

	for (int i = 9; i <= 17; i++)
	{
		Json::Value data_task = this->Mod_EraTreasureOrder_Base_get_task(i);
		if (data_task[0].asInt() == EraTreasureOrderType::SUCCESS)
		{
			common::log(boost::str(boost::format("【纪元战令】：领取赛季关卡任务【%1%】成功！！") % i));
		}
		else if (data_task[0].asInt() == EraTreasureOrderType::TASK_NOT_COMPLETED)
		{
			common::log(boost::str(boost::format("【纪元战令】：赛季关卡任务【%1%】未完成！！") % i));
			break;
		}

	}
	for (int i = 18; i <= 30; i++)
	{
		Json::Value data_task = this->Mod_EraTreasureOrder_Base_get_task(i);
		if (data_task[0].asInt() == EraTreasureOrderType::SUCCESS)
		{
			common::log(boost::str(boost::format("【纪元战令】：领取赛季劫海任务【%1%】成功！！") % i));
		}
		else if (data_task[0].asInt() == EraTreasureOrderType::TASK_NOT_COMPLETED)
		{
			common::log(boost::str(boost::format("【纪元战令】：赛季劫海任务【%1%】未完成！！") % i));
			break;
		}

	}

	Json::Value data = this->Mod_EraTreasureOrder_Base_one_click_get_award();
	common::log("【纪元战令】：一键领取战令奖励成功！！");
}

//============================================================================
// 纪元战令--面板信息
// "module":848,"action":1,
// "request":[],
// "response":[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//
// SEraTreasureOrderView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
//
// EraTreasureOrderData.as:
//			oObject.list(param1,this.panelInfo,["player_level","player_exp","award_list","is_unlock_ingot","is_unlock_charge_ingot","player_ingot","player_charge_ingot","season"]);
// Example
//			[ 14, 13000, 
//				[ [ 64, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 76, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 69, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 109, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 98, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 101, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 106, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 91, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 19, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 32, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 99, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 5, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 100, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 20, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 26, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 119, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 65, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 8, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 3, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 111, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 52, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 61, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 79, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 60, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 62, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 71, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 4, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 110, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 54, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 7, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 46, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 30, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 68, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 12, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 115, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 57, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 70, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 118, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 37, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 39, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 41, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 49, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 108, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 21, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 114, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 90, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 56, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 116, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 77, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 92, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 17, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 34, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 67, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 36, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 1, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 102, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 44, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 29, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 35, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 47, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 24, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 11, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 25, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 13, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 82, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 95, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 53, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 59, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 28, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 23, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 103, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 48, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 88, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 16, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 112, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 50, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 113, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 40, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 10, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 105, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 9, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 81, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 31, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 83, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 63, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 93, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 120, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 97, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 74, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 86, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 43, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 94, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 117, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 107, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 84, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 42, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 14, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 51, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 72, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 18, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 85, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 66, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 2, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 22, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 104, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 55, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 45, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 15, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 27, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 87, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 78, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 96, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 80, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 73, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 33, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 6, [ [ 3, 0 ], [ 1, 1 ], [ 2, 0 ] ] ], [ 58, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 38, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 89, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ], [ 75, [ [ 3, 0 ], [ 1, 0 ], [ 2, 0 ] ] ] ], 
//					0, 0, 112888, 0, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_EraTreasureOrder_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 848, 1);
}

//============================================================================
// 纪元战令--一键领取奖励
// "module":848,"action":3,
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//
// EraTreasureOrderView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
//
// EraTreasureOrderData.as:
//			
// Example
//		[ 0, [ [ 9085, 1 ], [ 8358, 1 ], [ 10385, 200000 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_EraTreasureOrder_Base_one_click_get_award()
{
	Json::Value data;
	return this->send_and_receive(data, 848, 3);
}

//============================================================================
// 纪元战令--任务信息
// "module":848,"action":6,
// "request":[],
// "response":[[Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil]]
//
// EraTreasureTaskViewView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
//
// EraTreasureOrderData.as:
//			oObject.list(param1[0][_loc2_],_loc3_,["task_id","is_get","task_value"]);
//
// Example
//		[ [ [ 2, 0, 1060 ], [ 6, 1, 5 ], [ 7, 0, 1 ], [ 8, 1, 42 ], [ 9, 0, 82 ], [ 19, 0, 84 ], [ 26, 0, 30 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EraTreasureOrder_Base_task_info()
{
	Json::Value data;
	return this->send_and_receive(data, 848, 6);
}

//============================================================================
// 纪元战令--领取任务奖励
// "module":848,"action":7,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil]
//
// EraTreasureTaskViewView.as:
//          _data.call(Mod_EraTreasureOrder_Base.get_task,this.get_task_call_back,[param1.task_id]);
//
// EraTreasureOrderData.as:
//			
// Example
//		[ 11 ] 
//============================================================================
Json::Value sxd_client::Mod_EraTreasureOrder_Base_get_task(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 848, 7);
}


