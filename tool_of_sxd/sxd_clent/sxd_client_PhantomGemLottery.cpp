#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class PhantomGemLotteryType
{
public:
	static const int SUCCESS = 0;	
	static const int FAILED = 1;
	static const int NO_ENOUGTH_TIMES = 2;		//次数不足
	static const int NO_ENOUGTH_INGOT = 3;		//元宝不足
	static const int NO_ENOUGTH_DAOYUAN = 4;	//道缘不足
};

void sxd_client::PhantomGemLottery()
{
	common::log("【多宝山】：开始普通求宝！\n");
	//获取多宝山信息
	Json::Value data = this->Mod_PhantomGemLottery_Base_panel_info();
	int k = 0;
	//锁定3号翠青宝山
	for (k; k < 3; k++)
	{
		if (data[0][k][0].asInt() == 3)
		{
			break;
		}
	}
	//剩余普通求宝次数
	int remain_normal_times = data[0][k][2].asInt();
	if (remain_normal_times == 0)
	{
		common::log("【多宝山】：普通求宝次数已用完！");
		return;
	}
	else
	{
		data = this->Mod_PhantomGemLottery_Base_find(3, 0);
		int result = data[0].asInt();
		if (result == PhantomGemLotteryType::SUCCESS)
		{
			common::log("【多宝山】：普通求宝成功！");
			//return;
		}
		else if(result == PhantomGemLotteryType::NO_ENOUGTH_DAOYUAN)
		{
			common::log("【多宝山】：道源不足，普通求宝失败！！");		
			return;
		}	
		else if (result == PhantomGemLotteryType::NO_ENOUGTH_INGOT)
		{
			common::log("【多宝山】：元宝不足，普通求宝失败！！");
			return;
		}
		else if (result == PhantomGemLotteryType::NO_ENOUGTH_TIMES)
		{
			common::log("【多宝山】：次数不足，普通求宝失败！！");
			return;
		}
		else
		{
			common::log(boost::str(boost::format("【多宝山】：求宝结果：[%1%]\n") % data));
		}
	}
}

//============================================================================
//  获取隐仙副本信息
// {module:694, action:0,
// request:[],
// Example		
//     [  ]
// response:[[],[],[],[]]
// [[Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
// 
// Example
// DevilExpeditionData.as
// oObject.list(param1, this.info, ["mountain_list", "requite_list", "requite_value", "ticket_num"]);
//    mountain_list:  oObject.list(_loc2_[_loc3_],_loc5_,["mountain_id","normal_list","remain_normal_times","high_list","remain_high_times"]);
//    normal_list:	  oObject.list(_loc6_[_loc4_],_loc8_,["item_id","item_num","prob","is_high"]);
//											[8978,1,162,1]：[一阶黑宝石，数量一，概率16.2%，is_high]
// is_high==1,可以直接抽到。is_high==0,除富产以外，游戏页面概率除以8，就等于实际概率
//	  
//	  requite_list:   oObject.list(_loc2_[_loc3_],_loc5_,["requite_id","need_requite_value","requite_award_list","get_requite_award_id"]);
//    requite_award_list:   oObject.list(_loc6_[_loc4_],_loc8_,["requite_award_id","item_id","item_num","is_have"]);
//  [ ]
//============================================================================
Json::Value sxd_client::Mod_PhantomGemLottery_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 694, 0);
}

//============================================================================
//  获取隐仙副本信息
// {module:694, action:1,
// request:[Utils.ByteUtil,Utils.ByteUtil]
// Example		
// _data.call(Mod_PhantomGemLottery_Base.find, find_callback, [id, _loc1_]);
//     [  ]
// response:[]
// 
// Example
// 
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_PhantomGemLottery_Base_find(int id, int type)
{
	Json::Value data;
	data.append(id);
	data.append(type);
	return this->send_and_receive(data, 694, 1);
}

Json::Value sxd_client::Mod_PhantomGemLottery_Base_get_requite_award()
{
	Json::Value data;
	return this->send_and_receive(data, 694, 2);
}

