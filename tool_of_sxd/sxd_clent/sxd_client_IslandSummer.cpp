#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class IslandSummerStoretype
{
public:
	static const int SUCCESS = 0;
	static const int NUM_LIMIT = 3;
	static const int FAILED = 7;
};
class IslandFishingtype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int NOT_ENOUGH_BAIT = 4;
	static const int NOT_COMPLETED = 9;
	static const int ORDINARY = 11;		//普通领取
	static const int DOUBLE = 12;		//双倍领取
	static const int IS_GET = 18;		//已领取
	static const int NOT_UNLOCK_PRIVILEGE = 19;		//未开通特权
};
class IslandSummerOrdertype
{
public:
	static const int SUCCESS = 0;
	static const int ALREADY_GET = 2;
	static const int NOT_ENOUGH_LEVEL = 3;
	static const int FAILED = 6;
};

void sxd_client::IslandSummer()
{
	IslandSummerStore();

	IslandSummerOrder();

	IslandFishing();
}
void sxd_client::IslandFishing()		//海岛垂钓
{
	Json::Value data = this->Mod_IslandFishing_Base_panel_info();
	//钓鱼列表
	Json::Value fishing_list = data[3];		
	for (Json::Value fishing : fishing_list)
	{
		int state = fishing[1].asInt();
		if (state == 2)
		{
			int player_fishing_id = fishing[0].asInt();
			Json::Value fishing_data = this->Mod_IslandFishing_Base_finish_fishing(player_fishing_id, IslandFishingtype::ORDINARY);
			if (fishing_data[0].asInt() == IslandFishingtype::SUCCESS)
			{
				common::log("【海岛垂钓】：收杆成功！！");
			}
		}
		else if (state == 1)
		{
			common::log("【海岛垂钓】：正在钓鱼中！！");
		}
		else if (state == 3)
		{
			common::log("【海岛垂钓】：正在排队中！！");
		}
	}

	Json::Value task_data = this->Mod_IslandFishing_Base_task_info();
	Json::Value task_list = task_data[1];
	for (Json::Value task : task_list)
	{
		int task_id = task[0].asInt();
		int is_get = task[2].asInt();
		if (!is_get)
		{
			Json::Value award_data = this->Mod_IslandFishing_Base_get_task_award(task_id);
			if (award_data[0].asInt() == IslandFishingtype::SUCCESS)
			{
				common::log("【海岛垂钓】：领取任务奖励成功！！");
			}
			else if(award_data[0].asInt() == IslandFishingtype::NOT_UNLOCK_PRIVILEGE)
			{
				common::log("【海岛垂钓】：未开通特权！！");
			}
		}
	}

	data = this->Mod_IslandFishing_Base_panel_info();
	int bait_num = data[0].asInt();
	int area = 1;
	while (bait_num > 0)
	{
		Json::Value fishing_data = this->Mod_IslandFishing_Base_fishing(area);
		if (fishing_data[0].asInt() == IslandFishingtype::SUCCESS)
		{
			common::log("【海岛垂钓】：放鱼钩成功！！");
			area++;
			bait_num--;
		}
		else
		{
			common::log("【海岛垂钓】：放鱼钩失败！！");
		}
	}
	Json::Value collect_data;
	for (int i = 1; i <= 6; i++)
	{
		collect_data = this->Mod_IslandFishing_Base_get_collect_award(i);
		if (collect_data[0].asInt() == IslandFishingtype::IS_GET)
		{
			common::log("【海岛垂钓】：收集奖励已领取！！",1);
			i++;
		}
		else if (collect_data[0].asInt() == IslandFishingtype::SUCCESS)
		{
			common::log("【海岛垂钓】：收集奖励领取成功！！");
			i++;
		}
		else if (collect_data[0].asInt() == IslandFishingtype::NOT_COMPLETED)
		{
			common::log("【海岛垂钓】：领取失败，未达到指定星级！！");
			return;
		}
	}
	
}

//============================================================================
//	海岛垂钓：面板信息
// "module":826,"action":0,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingview.as
// 
// "response":[Utils.IntUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// IslandFishingdata.as		
//			oObject.list(param1,this.objPanleInfo,["bait_num","is_privilege","fish_list","fishing_list","buy_bait_num","left_buy_bait_num"]);
//			已钓起		oObject.list(this.objPanleInfo.fish_list[_loc1_],_loc3_,["fish_id","weight","star"]);
//			正在钓		oObject.list(this.objPanleInfo.fishing_list[_loc1_],_loc2_,["player_fishing_id","state","end_time","seq","is_unlock","area_id"]);
//							state：1正在进行，2已完成，3排队中
// Example
//		正在钓鱼	[ 0, 0, [ [ 32, 101, 2 ], [ 33, 101, 2 ], [ 5, 71, 1 ], [ 8, 461, 2 ], [ 12, 1, 1 ], [ 6, 71, 1 ] ], [ [ 233, 1, 1752216784, 1, 1, 3 ], [ 234, 3, 0, 2, 1, 3 ], [ 235, 3, 0, 3, 1, 3 ], [ 0, 0, 0, 4, 0, 0 ], [ 0, 0, 0, 5, 0, 0 ] ], 0, 12, 92351 ] 
//		完成钓鱼	[ 0, 0, [ [ 5, 461, 2 ], [ 3, 101, 2 ], [ 10, 100, 1 ], [ 11, 101, 2 ] ], [ [ 14, 2, 1752137808, 1, 1, 3 ], [ 15, 2, 1752159408, 2, 1, 3 ], [ 16, 2, 1752181008, 3, 1, 3 ], [ 0, 0, 0, 4, 0, 0 ], [ 0, 0, 0, 5, 0, 0 ] ], 0, 12, 53723 ] 
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 826, 0);
}

//============================================================================
//	海岛垂钓：钓鱼
// "module":826,"action":1,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingview.as
//			钓鱼场，1,2,3，
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			this.result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_fishing(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 826, 1);
}

//============================================================================
//	海岛垂钓：获取钓鱼信息
// "module":826,"action":3,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingview.as
//			_data.call(Mod_IslandFishing_Base.get_fishing_info,this.get_fishing_info_call_back,[param1.player_fishing_id]);
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			oObject.list(param1,this.objGetFishingInfo,["result","fish_weight","fish_star","fish_id","rank","is_new_fish","is_new_weight","award_list","left_double_times","player_fishing_id","is_up_award"]);
//				oObject.list(this.objGetFishingInfo.award_list[_loc2_],_loc3_,["item_id","item_num"]);
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_get_fishing_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 826, 3);
}

//============================================================================
//	海岛垂钓：完成钓鱼
// "module":826,"action":5,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingview.as
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		_data.call(Mod_IslandFishing_Base.finish_fishing,this.finish_fishing_call_back,[param1,Mod_IslandFishing_Base.ORDINARY]);
// 
// IslandFishingdata.as		
//			this.result = param1[0];
//				oObject.list(param1[1][_loc2_],_loc3_,["item_id","item_num"]);
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_finish_fishing(int type, int id)
{
	Json::Value data;
	data.append(type);
	data.append(id);
	return this->send_and_receive(data, 826, 5);
}

//============================================================================
//	海岛垂钓：任务信息
// "module":826,"action":7,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingTaskview.as
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			oObject.list(param1,this._taskInfo,["is_privilege","task_list","charge_ingot","award_bait_num"]);
//					oObject.list(_loc2_[_loc4_],_loc3_,["task_id","cur_value","is_get"]);
//							this._taskInfo.task_list.sortOn(["is_get","task_id"],[Array.NUMERIC,Array.NUMERIC]);
// Example
//			[ 0, [ [ 5, 600, 0 ], [ 3, 1, 1 ], [ 4, 1, 0 ], [ 1, 1, 1 ], [ 2, 800, 1 ] ], 0, 6 ] 
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_task_info()
{
	Json::Value data;
	return this->send_and_receive(data, 826, 7);
}

//============================================================================
//	海岛垂钓：获取任务奖励
// "module":826,"action":8,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingTaskview.as
//				_data.call(Mod_IslandFishing_Base.get_task_award,this.get_task_award_callback,[param1]);
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			this.result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_get_task_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 826, 8);
}

//============================================================================
//	海岛垂钓：收集奖励信息
// "module":826,"action":9,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingCollectview.as
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			oObject.list(param1[0][_loc4_],_loc3_,["fish_id","weight","star","is_new","rank"]);
		 /*if(param1[1][_loc5_][0] == this._collectInfo.collect_list[_loc4_].id)
			{
			this._collectInfo.collect_list[_loc4_].is_get = true;
			break;
			}*/
// Example
//			[ [ [ 3, 1, 1, 1, 0 ], [ 26, 1, 1, 1, 0 ], [ 11, 2601, 3, 0, 0 ], [ 19, 1001, 3, 1, 0 ], [ 6, 101, 2, 0, 0 ], [ 28, 1, 1, 1, 0 ], [ 12, 1, 1, 0, 0 ], [ 23, 461, 2, 1, 0 ], [ 35, 2601, 3, 0, 0 ], [ 20, 1, 1, 1, 0 ], [ 24, 101, 2, 1, 0 ], [ 10, 461, 2, 0, 0 ], [ 33, 1, 1, 1, 0 ], [ 36, 1, 1, 1, 0 ], [ 22, 1, 1, 0, 0 ] ], [ [ 3 ], [ 1 ], [ 2 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_collect_info()
{
	Json::Value data;
	return this->send_and_receive(data, 826, 9);
}

//============================================================================
//	海岛垂钓：获取收集奖励
// "module":826,"action":10,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandFishingCollectview.as
//			 _data.call(Mod_IslandFishing_Base.get_collect_award,this.get_collect_award_callback,[param1]);
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandFishingdata.as		
//			this.result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandFishing_Base_get_collect_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 826, 10);
}


void sxd_client::IslandSummerOrder()		//海岛战令
{
	Json::Value data = this->Mod_IslandSummerOrder_Base_one_click_get_award();
	if (data[0].asInt() == IslandSummerOrdertype::SUCCESS)
	{
		common::log("【海岛战令】：一键领取成功！");
		return;
	}
}

//============================================================================
//	海岛战令，面板信息
// "module":829,"action":1,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandSummerStoreview.as
// 
// "response":[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//		
// IslandSummerStoredata.as		
//			oObject.list(param1,this._info,["player_level","player_exp","award_list","is_unlock_ingot","is_unlock_charge_ingot","player_ingot","player_charge_ingot","task_list"]);
//					 oObject.list(this._info.task_list[_loc6_],_loc4_,["id","player_value","finish_num"]);
//
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandSummerOrder_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 829, 1);
}
//============================================================================
//	海岛战令：一键领取奖励
// "module":829,"action":3,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandSummerStoreview.as
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandSummerStoredata.as		
//			this._result = list[0];
//				this._itemList = Tool2Data.parse2List(list[1], ["item_id", "item_num"], function(param1:Object) :void
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandSummerOrder_Base_one_click_get_award()
{
	Json::Value data;
	return this->send_and_receive(data, 829, 3);
}


void sxd_client::IslandSummerStore()		//海岛商店
{
	common::log("【海岛商店】：");
	Json::Value data = this->Mod_IslandSummerStore_Base_panel_info();
	for (Json::Value award : data[2])
	{
		if (award[0].asInt() == 1)
		{
			if (!award[1].asInt())
			{
				Json::Value data_award = this->Mod_IslandSummerStore_Base_exchange_award(1, 1);
				if (data_award[0].asInt() == IslandSummerStoretype::SUCCESS)
				{
					common::log("【海岛商店】：领取每日奖励成功！");
					return;
				}
			}
			else
			{
				common::log("【海岛商店】：每日奖励已领取！");
				return;
			}
		}

	}
	
}

//============================================================================
//	海岛商店：面板信息
// "module":830,"action":1,
// "request":[]
// 
// IslandSummerStoreview.as
// 
// "response":[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// IslandSummerStoredata.as		
//		oObject.list(param1,this.panelInfo,["coin_num","total_cost_coin","award_list","add_coin_num","rest_add_num","max_add_num"]);
//			oObject.list(this.panelInfo.award_list[_loc1_],_loc2_,["award_id","exchange_num","limit","is_have_pre_item"]);
// 
// Example
//			[ 20, 0, [ [ 19, 0, 100, 1 ], [ 32, 0, 100, 1 ], [ 5, 0, 10, 0 ], [ 20, 0, 30, 1 ], [ 26, 0, 5, 1 ], [ 8, 0, 10, 0 ], [ 3, 0, 10, 0 ], [ 4, 0, 10, 0 ], [ 7, 0, 10, 0 ], [ 30, 0, 50, 1 ], [ 12, 0, 10, 0 ], [ 21, 0, 5, 1 ], [ 17, 0, 5, 1 ], [ 1, 1, 1, 1 ], [ 29, 0, 20, 1 ], [ 24, 0, 15, 1 ], [ 11, 0, 10, 0 ], [ 25, 0, 5, 1 ], [ 13, 0, 10, 0 ], [ 28, 0, 10, 1 ], [ 23, 0, 3, 1 ], [ 16, 0, 1, 1 ], [ 10, 0, 10, 0 ], [ 9, 0, 10, 0 ], [ 31, 0, 30, 1 ], [ 14, 0, 10, 0 ], [ 18, 0, 1, 1 ], [ 2, 0, 1, 1 ], [ 22, 0, 2, 1 ], [ 15, 0, 1, 0 ], [ 27, 0, 10, 1 ], [ 6, 0, 10, 0 ] ], 0, 600, 9150 ] 
//============================================================================
Json::Value sxd_client::Mod_IslandSummerStore_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 830, 1);
}

//============================================================================
//	海岛商店：兑换奖励
// "module":830,"action":1,
// "request":[Utils.IntUtil,Utils.IntUtil]
// 
// IslandSummerStoreview.as
//				_data.call(Mod_IslandSummerStore_Base.exchange_award,this.exchange_call_back,[param1.award_id,1]);
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// IslandSummerStoredata.as		
//			         this.result = param1[0];
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_IslandSummerStore_Base_exchange_award(int award_id, int id)
{
	Json::Value data;
	data.append(award_id);
	data.append(id);
	return this->send_and_receive(data, 830, 2);
}

