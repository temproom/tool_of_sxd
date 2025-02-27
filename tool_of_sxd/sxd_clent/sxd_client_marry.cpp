#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Marry_Base
{
public:
	static const int SUCCESS = 13;
};

//============================================================================
// R179 结婚
//============================================================================
void sxd_client::marry()
{
	//获取亲密度信息
	auto data_gift= this->Mod_Marry_Base_get_player_engagement_info();
	
	auto data = this->Mod_Marry_Base_get_marry_box();
	if (data[0].asInt() == Mod_Marry_Base::SUCCESS)
		common::log("【结婚】领取夫妻宝箱", iEdit);
}

void sxd_client::marry_gift()
{
	//获取亲密度信息
	auto data_gift1 = this->Mod_Marry_Base_get_player_engagement_info();

	//亲密度，30000满
	int count = data_gift1[1].asInt();
	if (count >= 30000)
	{
		common::log("【结婚】亲密度已满，无需赠送礼物！", iEdit);
		return;
	}

	common::log("【结婚】赠送铜钱礼物", iEdit);

	//获取亲密度信息
	auto data_gift = this->Mod_Marry_Base_get_player_engagement_info();

	auto gift_info = data_gift[6];

	for (auto gift : gift_info)
	{
		if (gift[0].asInt() == 1)
		{
			int times = gift[1].asInt();
			while (times < 5)
			{
				auto data = this->Mod_Marry_Base_give_gift(1);
				common::log("【结婚】赠送铜钱礼物【芍药】成功", iEdit);
				times++;
			}
		}

		if (gift[0].asInt() == 6 )
		{
			int times = gift[1].asInt();
			while (times < 5)
			{
				auto data = this->Mod_Marry_Base_give_gift(6);
				common::log("【结婚】赠送铜钱礼物【芍药】成功", iEdit);
				times++;
			}
		}

		if (gift[0].asInt() == 2)
		{
			int times = gift[1].asInt();
			while (times < 5)
			{
				auto data = this->Mod_Marry_Base_give_gift(2);
				common::log("【结婚】赠送铜钱礼物【同心结】成功", iEdit);
				times++;
			}
		}
		if (gift[0].asInt() == 7)
		{
			int times = gift[1].asInt();
			while (times < 5)
			{
				auto data = this->Mod_Marry_Base_give_gift(7);
				common::log("【结婚】赠送铜钱礼物【同心结】成功", iEdit);
				times++;
			}
		}
	}
	common::log("【结婚】今日赠送铜钱礼物完毕", iEdit);
}



//============================================================================
// R179 夫妻宝箱
// "module":206,"action":40,"request":[],"response":[Utils.UByteUtil]
// MarryData.as 265:
//     oObject.list(param1,this._engageIntimacyInfo,["result"]);
//============================================================================
Json::Value sxd_client::Mod_Marry_Base_get_marry_box()
{
	Json::Value data;
	return this->send_and_receive(data, 206, 40);
}

//============================================================================
//  获取亲密度信息
// "module":206,"action":7,
// "request":[],
// "response":[[Utils.ShortUtil,Utils.StringUtil],Utils.ShortUtil,Utils.UByteUtil,Utils.UByteUtil,Utils.IntUtil,Utils.ShortUtil,[Utils.ShortUtil,Utils.ShortUtil,Utils.IntUtil],Utils.ShortUtil,Utils.IntUtil,Utils.UByteUtil]
// 
// MarryData.as 265:
//     oObject.list(param1,this._engageIntimacyInfo,["lover_info","intimacy","status","is_booking","last_login_time","intimacy_id","gift_info","box_item_id","box_number","is_get_box"]);
// 
//	[ [ [ 507, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51" ], [ 512, "\u5929\u624d\u5e05\u5e05.s51" ] ], 17345, 4, 10, 1718418760, 9, [ [ 6, 0, 0 ], [ 9, 0, 0 ], [ 10, 0, 0 ], [ 7, 0, 0 ], [ 8, 0, 0 ] ], 3067, 0, 10 ] 
//  [[[ 511, "\u51af\u8bfa\u4f9d\u66fc" ], [512, "\u51af\u6d77\u68ee\u5821"]], 10020, 4, 10, 1718419321, 8, [[ 6, 5, 0 ], [9, 0, 0], [10, 0, 0], [7, 5, 0], [8, 0, 0]], 3067, 0, 10]


//============================================================================
Json::Value sxd_client::Mod_Marry_Base_get_player_engagement_info()
{
	Json::Value data;
	return this->send_and_receive(data, 206, 7);
}

//============================================================================
//  赠送礼物
// "module":206,"action":8,
// "request":[Utils.ShortUtil],
// "response":[Utils.UByteUtil,Utils.ShortUtil,Utils.UByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.IntUtil]
//	
// MarryIntimacyView.as
// _data.call(Mod_Marry_Base.give_gift,giveGiftBack,[giftID]);
// 
// MarryData.as 265:
//     oObject.list(param1,this._engageIntimacyInfo,["result"]);
//============================================================================
Json::Value sxd_client::Mod_Marry_Base_give_gift(int gift_id)
{
	Json::Value data;
	data.append(gift_id);
	return this->send_and_receive(data, 206, 8);
}