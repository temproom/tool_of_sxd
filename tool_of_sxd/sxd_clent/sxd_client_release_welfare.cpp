#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_ReleaseWelfare
{
public:
	static const int TURE = 0;		//可领取
	static const int False = 1;		//不可领取
	static const int GOTTEN = 2;	//已领取
	static const int SUCCESS = 3;
};

//============================================================================
// R180 更新福利
//============================================================================
void sxd_client::release_welfare()
{
	auto data = this->Mod_ReleaseWelfare_Base_get_info();
	std::vector<Json::Value> welfares;
	//data[3]才是物品信息
	std::copy_if(data[3].begin(), data[3].end(), std::back_inserter(welfares), [](const Json::Value& x) { return x[7].asInt() == Mod_ReleaseWelfare::TURE; });
	if (!welfares.size())
		return;
	std::sort(welfares.begin(), welfares.end(), [](const Json::Value& x, const Json::Value& y) { return x[0].asInt() < y[0].asInt(); });

	for (const auto& welfare : welfares)
	{
		data = this->Mod_ReleaseWelfare_Base_get_welfare(welfare[0].asInt());
		if (data[0].asInt() != Mod_ReleaseWelfare::SUCCESS)
		{
			common::log(boost::str(boost::format("【更新福利】领取福利失败，result[%1%]") % data[0]), iEdit);
			return;
		}
		common::log(boost::str(boost::format("【更新福利】领取第 [%1%] 个福利") % welfare[0]), iEdit);
	}
}

//============================================================================
// R180 面板
// "module":240,"action":0,"request":[],"response":[Utils.IntUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.UByteUtil]]
// ReleaseWelfareData.as"
//     this._info.release = param1[0];
//     for each(_loc3_ in param1[1])
//     旧版    oObject.list(_loc3_,_loc4_,["day_seq","item_id","count","item_id2","count2","gift_item_id","gift_count","getable_falg"]);
//	   新版    oObject.list(_loc3_,_loc6_,["day_seq","item_id","count","item_id2","count2","gift_item_id","gift_count","getable_falg","extra_ingot","get_extra_ingot_flag"]);
// Example
//				[ 180, [ [ 5, 3345, 50, 5638, 1, 4062, 5, 0 ], [ 4, 4063, 5, 5638, 1, 4062, 3, 0 ], [ 3, 2016, 2, 5638, 1, 4062, 2, 0 ], [ 2, 2016, 2, 5638, 1, 4062, 1, 0 ], [ 1, 3345, 20, 5638, 1, 4062, 1, 2 ] ] ]
// 20241106     [ 342, 0, 0, [ [ 5, 3345, 40, 7732, 1, 7263, 2, 1, 60, 1 ], [ 4, 4063, 5, 7904, 10, 3345, 40, 1, 60, 1 ], [ 3, 2016, 2, 7892, 5, 3345, 40, 0, 60, 1 ], [ 2, 2016, 2, 5638, 5, 3345, 40, 0, 60, 1 ], [ 1, 3345, 40, 1411, 10, 2016, 2, 0, 60, 1 ] ], 0, [ [ 3345, 300 ], [ 7695, 1 ], [ 7965, 20 ] ] ] 
//			0:待领取, 1:不可领取, 2:已经领取
//============================================================================
Json::Value sxd_client::Mod_ReleaseWelfare_Base_get_info()
{
	Json::Value data;
	return this->send_and_receive(data, 240, 0);
}

//============================================================================
// R180 领取
// "module":240,"action":1,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil]
// ReleaseWelfareData.as"
//     this._result = param1[0];
// Example
//     [ 3 ]
//============================================================================
Json::Value sxd_client::Mod_ReleaseWelfare_Base_get_welfare(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 240, 1);
}
