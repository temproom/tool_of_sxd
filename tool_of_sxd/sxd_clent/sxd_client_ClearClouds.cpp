#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"
#include <ctime>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class ClearCloudsType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
};

void sxd_client::ClearClouds()
{
	common::log("【拨云见日】：开始检查！");

	Json::Value data = this->Mod_ClearClouds_Base_open_buy_panel();
	//common::log(boost::str(boost::format("\n\t\t【拨云见日】：拨云见日礼包信息：%1%") % data));

	if (!data[4]||data[4][0][0].asInt()>data[4][0][1].asInt())
	{
		data = this->Mod_ClearClouds_Base_buy_gift(1);
		common::log("【拨云见日】：领取每日免费礼包成功！");
		return;
	}
	else
	{
		common::log("【拨云见日】：每日免费礼包已领取，请明日再来！");
		return;
	}
}

//============================================================================
//  拨云见日 礼包信息
// {module:608, action:7,
// request:[],
// Example		
//     [  ]
//
//"response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// ClearCloundsData.as
//        this._giftData = this.parseObject(param1,["result","active_degree","charge_ingot","cost_ingot","buy_gift_list"]);
//     buy_gift_list，[礼包id，购买次数]，若为空，则表示没有买过
//		礼包id：1为免费礼包
// [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_ClearClouds_Base_open_buy_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 608, 7);
}

//============================================================================
//  拨云见日 领取每日免费礼包
// {module:608, action:8,
// request:[Utils.IntUtil],
// Example		
//     [  ]
//
//"response":[Utils.UByteUtil,Utils.IntUtil]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_ClearClouds_Base_buy_gift(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 608, 8);
}