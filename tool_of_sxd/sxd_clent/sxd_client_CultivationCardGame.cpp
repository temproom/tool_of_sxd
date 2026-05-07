#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class CultivationCardGameType	//神仙收藏册
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int ACT_NOT_OPEN = 2;
	static const int NOT_COMPLETE = 9;
	static const int HAD_GET = 10;
	static const int BUY_LIMIT = 15;
};

void sxd_client::CultivationCardGame()
{
	Json::Value data = this->Mod_CultivationCardGame_Base_panel_info();

	//当前阶段
	int now_step_id = data[0].asInt();
	data = this->Mod_CultivationCardGame_Base_task_info(now_step_id);

	for (Json::Value step_task_list : data[1])
	{
		//周任务1,2,3,4，阶段任务5,7,11
		int task_id = step_task_list[0].asInt();
		if (step_task_list[2].asInt() == 0)
		{
			//未领取
			Json::Value data_task = this->Mod_CultivationCardGame_Base_get_task_award(task_id);
			int result = data_task[0].asInt();
			if (result == CultivationCardGameType::SUCCESS)
			{
				common::log(boost::str(boost::format("【神仙收藏册】：领取任务【%1%】成功！！") % task_id));
			}
			else if (result == CultivationCardGameType::NOT_COMPLETE)
			{
				common::log(boost::str(boost::format("【神仙收藏册】：任务【%1%】未完成！！") % task_id), 1);
			}
		}
	}

	Json::Value data_shop = this->Mod_CultivationCardGame_Base_shop_info();

	data_shop = this->Mod_CultivationCardGame_Base_buy_shop(1);
	int result = data_shop[0].asInt();
	if (result == CultivationCardGameType::SUCCESS)
	{
		common::log("【神仙收藏册】：[铜钱]购买【普通】春卡成功！！");
	}
	else if (result == CultivationCardGameType::BUY_LIMIT)
	{
		common::log("【神仙收藏册】：本周【普通】春卡已购买！！");
	}

	data_shop = this->Mod_CultivationCardGame_Base_buy_shop(3);
	result = data_shop[0].asInt();
	if (result == CultivationCardGameType::SUCCESS)
	{
		common::log("【神仙收藏册】：[铜钱]购买【高级】春卡成功！！");
	}
	else if (result == CultivationCardGameType::BUY_LIMIT)
	{
		common::log("【神仙收藏册】：本周【高级】春卡已购买！！");
	}
}

//============================================================================
// 神仙收藏册--面板信息
// "module":859,"action":0,
// "request":[],
// "response":[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,[Utils.IntUtil],[Utils.IntUtil,Utils.ByteUtil]],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//
// CultivationCardGameView.as:   
// 
// CultivationCardGameData.as:
//		oObject.list(param1,this.objInfo,["now_step_id","card_list","award_group_info","open_bag_type_num","chen_xin_dian","shen_xian_ka_ling"]);
// Example
//			[ 1, null, [ [ 5, null, [ [ 22, 0 ], [ 23, 0 ], [ 21, 0 ], [ 20, 0 ] ] ], [ 3, null, [ [ 15, 0 ], [ 14, 0 ], [ 13, 0 ], [ 11, 0 ], [ 12, 0 ] ] ], [ 4, null, [ [ 18, 0 ], [ 16, 0 ], [ 17, 0 ], [ 19, 0 ] ] ], [ 1, null, [ [ 2, 0 ], [ 1, 0 ], [ 4, 0 ], [ 3, 0 ], [ 5, 0 ] ] ], [ 2, null, [ [ 6, 0 ], [ 9, 0 ], [ 10, 0 ], [ 7, 0 ], [ 8, 0 ] ] ] ], 0, 0, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_CultivationCardGame_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 859, 0);
}

//============================================================================
// 神仙收藏册--任务信息
// "module":859,"action":5,
// "request":[Utils.IntUtil],
// "response":[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil]]
//
// CultivationCardTaskAndShopView.as:
//			_data.call(Mod_CultivationCardGame_Base.task_info,param1,[this._curTaskStep]);
//		type：0: 周任务,task_id:1,2,3,4
//			  1：阶段累计任务,task_id:5,7,11
// 
// CultivationCardGameData.as:
//		
//		oObject.list(param1,this.taskInfo,["now_step_id","step_task_list"]);
//				oObject.list(_loc4_[_loc5_],_loc3_,["step_task_id","value","is_get"]);
// Example
//			[ 1, [ [ 2, 3290, 0 ], [ 11, 0, 0 ], [ 1, 3, 0 ], [ 7, 0, 0 ], [ 4, 0, 0 ], [ 3, 2300, 1 ], [ 5, 3, 0 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_CultivationCardGame_Base_task_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 859, 5);
}

//============================================================================
// 神仙收藏册--领取任务奖励
// "module":859,"action":6,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil]
//
// CultivationCardTaskAndShopView.as:
//         _data.call(Mod_CultivationCardGame_Base.get_task_award,this.updatePanelCallback,[param1]);
// 
// CultivationCardGameData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_CultivationCardGame_Base_get_task_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 859, 6);
}

//============================================================================
// 神仙收藏册--商店信息
// "module":859,"action":7,
// "request":[],
// "response":[[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
//
// CultivationCardTaskAndShopView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// CultivationCardGameData.as:
//		oObject.list(param1[0][_loc4_],_loc3_,["shop_id","left_number"]);
// Example
//			[ [ [ 5, 20 ], [ 3, 0 ], [ 4, 20 ], [ 1, 0 ], [ 2, 20 ], [ 6, 3 ] ], 0 ] 
//============================================================================
Json::Value sxd_client::Mod_CultivationCardGame_Base_shop_info()
{
	Json::Value data;
	return this->send_and_receive(data, 859, 7);
}

//============================================================================
// 神仙收藏册--购买物品
// "module":859,"action":8,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil]
//
// CultivationCardTaskAndShopView.as:
//          _data.call(Mod_CultivationCardGame_Base.buy_shop,updatePanelCallback,[data.id]);
// 
// CultivationCardGameData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_CultivationCardGame_Base_buy_shop(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 859, 8);
}
