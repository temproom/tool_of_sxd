#include "stdafx.h"
#include <vector>
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class XiuZhenRealmType
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 1;
	static const int NO_AWARD = 6;
};

void sxd_client::XiuZhenRealm()
{
	Json::Value data = this->Mod_XiuZhenRealm_Base_get_extra_award();
	if (data[0].asInt() == XiuZhenRealmType::SUCCESS)
	{
		common::log("【修真境界】：领取奖励成功！");
	}
	else if (data[0].asInt() == XiuZhenRealmType::NO_AWARD)
	{
		common::log("【修真境界】：无奖励可领取！");
	}
}

//============================================================================
//  修真境界-获取奖励
// {module:552, action:5,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]]
// 
// Example
// XiuZhenRealmData.as
//     this.result = param1[0];
//			[ 0, [ [ 8808, 1 ] ], [ [ 11, 0, [ [ 8808, 0 ] ] ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_XiuZhenRealm_Base_get_extra_award()
{
	Json::Value data;
	return this->send_and_receive(data, 552, 5);
}