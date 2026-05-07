#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Assistant_Base
{
public:
	static const int SUCCESS = 0;
	static const int HAVE_DONE = 4;
};

//============================================================================
// R179 活跃度
//============================================================================
void sxd_client::assistant()
{
	auto data = this->Mod_Assistant_Base_info();
	auto info = data;
	for (unsigned sn = 1; sn <= 12; sn++)
	{
		data = this->Mod_Assistant_Base_get_award(sn);
		if (data[0].asInt() == Mod_Assistant_Base::SUCCESS)
		{
			common::log(boost::str(boost::format("【活跃度】领取第 [%1%] 个活跃度奖励") % sn), iEdit);

		}
		else if (data[0].asInt() == Mod_Assistant_Base::HAVE_DONE)
		{
			continue;
		}
	}
}

//============================================================================
// R179 小助手
// "module":52,"action":0,"request":[],"response":[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil]
// AssistantData.as 215:
//     this._aryBoxInfo = this.getBoxData(param1);
//     this._liveNum = param1[0] * 5;
//     this._boxInfo = this.renderBoxInfo(param1);
// Example
//     [ 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ] // 172*5=860(活跃度)
// Mod_Assistant_Base.get_award(1)后
//     [ 172, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
// 未领取每日1200活跃度奖励
// [ 131072, 0, 1, 0, 0, 4, -55, 0, 5, 0, 0, 0, 5, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 1 ] 
// 已领取1200
// [ 131072, 0, 1, 0, 0, 4, -85, 0, 5, 0, 0, 0, 5, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_Assistant_Base_info()
{
	Json::Value data;
	return this->send_and_receive(data, 52, 0);
}

//============================================================================
// R179 领取
// "module":52,"action":1,"request":[Utils.IntUtil],"response":[Utils.UByteUtil]
// AssistantData.as 223
//     oObject.list(param1,this._objGetAward,["result"]);
// Example
//     [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_Assistant_Base_get_award(int sn)
{
	Json::Value data;
	data.append(sn);
	return this->send_and_receive(data, 52, 1);
}
