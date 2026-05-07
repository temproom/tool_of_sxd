#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_PartnerPresent_Base
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int DAY_GIFT = 2;
	static const int WEEK_GIFT = 3;
	static const int ROLE_GIFT = 4;
	static const int IS_GET = 5;
};

void sxd_client::PartnerPresent()
{
	Json::Value data = this->Mod_PartnerPresent_Base_open_panel();
	int is_have_role = data[2].asInt();
	int is_get_day = data[5].asInt();
	int is_get_week = data[7].asInt();
	int is_get_role = data[9].asInt();
	if (!is_get_day)
	{
		Json::Value data_gift = this->Mod_PartnerPresent_Base_get_free_gift(2);
		if (data_gift[0].asInt() == Mod_PartnerPresent_Base::SUCCESS)
		{
			common::log("【好礼馈赠】领取每日奖励成功！！");
		}
		else if(data_gift[0].asInt() == Mod_PartnerPresent_Base::IS_GET)
		{
			common::log("【好礼馈赠】每日奖励已领取！！", 0);
		}
	}
	if (!is_get_week)
	{
		Json::Value data_gift = this->Mod_PartnerPresent_Base_get_free_gift(3);
		if (data_gift[0].asInt() == Mod_PartnerPresent_Base::SUCCESS)
		{
			common::log("【好礼馈赠】领取每周奖励成功！！");
		}
		else if (data_gift[0].asInt() == Mod_PartnerPresent_Base::IS_GET)
		{
			common::log("【好礼馈赠】每周奖励已领取！！", 0);
		}
	}
	if (!is_get_role && is_have_role)
	{
		Json::Value data_gift = this->Mod_PartnerPresent_Base_get_free_gift(4);
		if (data_gift[0].asInt() == Mod_PartnerPresent_Base::SUCCESS)
		{
			common::log("【好礼馈赠】领取伙伴奖励成功！！");
		}
		else if (data_gift[0].asInt() == Mod_PartnerPresent_Base::IS_GET)
		{
			common::log("【好礼馈赠】伙伴奖励已领取！！", 0);
		}
	}	
}

//============================================================================
//  好礼馈赠 面板信息
// {module:538, action:1, 
// request:[], 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// PartnerPresentData.as 40:
//				oObject.list(param1,this._panelObject,["result","role_id","is_have_role","next_time","day_award","is_get_day","week_award","is_get_week","role_award","is_get_role","buy_award1","last_ingot1","now_ingot1","is_buy1","buy_limit1","buy_times1","buy_award2","last_ingot2","now_ingot2","is_buy2","buy_limit2","buy_times2","refresh_limit","refresh_times","role_soul_id"]);
// 
// Example
//    [ 0, 552, 1, 1756051200, [ [ 1747, 1500000 ], [ 2343, 10 ] ], 0, [ [ 4975, 10 ], [ 2407, 5 ], [ 5183, 20 ] ], 0, [ [ 2263, 30 ], [ 6638, 2 ], [ 7087, 10 ] ], 0, [ [ 1743, 666 ], [ 4099, 25 ], [ 3050, 800 ] ], 1288, 588, 0, 2, 0, [ [ 2471, 80 ], [ 4099, 25 ], [ 3706, 500 ] ], 1288, 499, 0, 1, 0, 2, 0, 68 ] 
//============================================================================
Json::Value sxd_client::Mod_PartnerPresent_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 538, 1);
}

//============================================================================
//  好礼馈赠 面板信息
// {module:538, action:2, 
// request:[Utils.UByteUtil], 
// 
// PartnerPresentView.as
//		data.call(Mod_PartnerPresent_Base.get_free_gift,this.get_free_gift_callback,[param1]);
// 
// response:[Utils.UByteUtil],
// 
// PartnerPresentData.as 40:
// 
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_PartnerPresent_Base_get_free_gift(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 538, 2);
}