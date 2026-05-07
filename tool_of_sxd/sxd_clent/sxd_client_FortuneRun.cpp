#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class FortuneRunType	//新春活动--福行万里
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int FAILED = 3;
	static const int ALREADY_GET = 6;
};

void sxd_client::FortuneRun()
{
	//一键获取和赠送礼物
	Json::Value data = this->Mod_FortuneRun_Base_get_and_send_all();
	int result = data[0].asInt();
	if (result == FortuneRunType::SUCCESS)
	{
		common::log("一键获取和赠送礼物成功！！");
	}

	//领取每日任务奖励
	for (int i = 1; i < 5; i++)
	{
		Json::Value data_task = this->Mod_FortuneRun_Base_get_daily_task(i);
		int result = data_task[0].asInt();
		if (result == FortuneRunType::SUCCESS)
		{
			common::log(boost::str(boost::format("【福行万里】：领取每日任务奖励【%1%】成功！！") % i));
		}			
	}
	//一键领取里程奖励
	data = this->Mod_FortuneRun_Base_one_click_get_award();
	result = data[0].asInt();
	if (result == FortuneRunType::SUCCESS)
	{
		common::log("一键领取里程奖励成功！！");
	}

	//面板信息
	data = this->Mod_FortuneRun_Base_get_panel_info();
	Json::Value item_list = data[0];

	std::unordered_map<int, std::string> item{ {10503,"草结"},{10504,"驿粮袋"}, {10505,"踏春良饲"}, {10506,"福星萝卜"}};
	
	for (Json::Value items : item_list)
	{
		int id = items[0].asInt();
		int num = items[1].asInt();

		if (num > 0)
		{
			Json::Value data_use = this->Mod_FortuneRun_Base_use_item(id, num);
			result = data_use[0].asInt();
			if (result == FortuneRunType::SUCCESS)
			{
				common::log(boost::str(boost::format("【福行万里】：使用【%1%】:[%2%]个！！") % item[id] % num));
			}
			else
			{
				common::log(boost::str(boost::format("【福行万里】：使用【%1%】错误，代码：[%2%]！！") % item[id] % result));

			}
		}		
	}
	

}

//============================================================================
// 福行万里--面板信息
// "module":865,"action":1,
// "request":[],
// "response":[[Utils.IntUtil,Utils.IntUtil],[Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
//
// FortuneRunView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// FortuneRunData.as:
//		oObject.list(param1,this._objPanelInfo,["item_list","show_player_list","speed"]);
// Example
//			[ [ [ 10505, 0 ], [ 10504, 0 ], [ 10506, 0 ], [ 10503, 0 ] ], [ [ "\u4f24\u5fc3\u4eba", "s954", 2, 1572, 0 ], [ "\u91d1\u9675\u72f1\u5352", "669ye_s2691", 2, 1783, 0 ], [ "\u51af\u8bfa\u4f9d\u66fc", "360_s313", 1, 1390, 1 ] ], 10 ] 
//============================================================================
Json::Value sxd_client::Mod_FortuneRun_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 865, 1);
}

//============================================================================
// 福行万里--使用物品
// "module":865,"action":2,
// "request":[Utils.IntUtil,Utils.IntUtil],
// "response":[Utils.UByteUtil]
//
// FortuneRunView.as:
//          _data.call(Mod_FortuneRun_Base.use_item,this.useItemCallback,[param1,param2]);
// 
// FortuneRunData.as:
//		oObject.list(param1,this._objPanelInfo,["item_list","show_player_list","speed"]);
// Example
//			[ [ [ 10505, 0 ], [ 10504, 0 ], [ 10506, 0 ], [ 10503, 0 ] ], [ [ "\u4f24\u5fc3\u4eba", "s954", 2, 1572, 0 ], [ "\u91d1\u9675\u72f1\u5352", "669ye_s2691", 2, 1783, 0 ], [ "\u51af\u8bfa\u4f9d\u66fc", "360_s313", 1, 1390, 1 ] ], 10 ] 
//============================================================================
Json::Value sxd_client::Mod_FortuneRun_Base_use_item(int id, int num)
{
	Json::Value data;
	data.append(id);
	data.append(num);
	return this->send_and_receive(data, 865, 2);
}
//============================================================================
// 福行万里--一键赠送收取
// "module":865,"action":6,
// "request":[],
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil]
//
// FortuneRunView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// FortuneRunData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_FortuneRun_Base_get_and_send_all()
{
	Json::Value data;
	return this->send_and_receive(data, 865, 6);
}

//============================================================================
// 福行万里--收取每日任务奖励
// "module":865,"action":8,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil]
//
// FortuneRunView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// FortuneRunData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_FortuneRun_Base_get_daily_task(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 865, 8);
}

//============================================================================
// 福行万里--一键领取里程奖励
// "module":865,"action":11,
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
//
// FortuneRunView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// FortuneRunData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_FortuneRun_Base_one_click_get_award()
{
	Json::Value data;
	return this->send_and_receive(data, 865, 11);
}