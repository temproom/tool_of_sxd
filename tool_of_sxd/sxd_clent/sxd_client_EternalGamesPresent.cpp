#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//万古神墟
class EternalGamesPresentType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int FUNC_NOT_OPEN = 2;
	static const int HAS_WORSHIPED = 3;
	static const int NOT_AWARD = 4;
};

void sxd_client::EternalGamesPresent()
{
	Json::Value data = this->Mod_EternalGamesPresent_Base_main_info();
	vector<std::string> list = { "神仙联赛","殿堂赛","封神榜","封神之战","阶位赛" };

	for (int i = 1; i <= 5; i++)
	{
		data = this->Mod_EternalGamesPresent_Base_worship(i);
		if (data[0].asInt() == EternalGamesPresentType::SUCCESS)
		{
			//common::log("【万古神墟】膜拜成功！");
			common::log(boost::str(boost::format("【万古神墟】%1% 膜拜成功！") % list[i - 1]));
		}
		else if (data[0].asInt() == EternalGamesPresentType::HAS_WORSHIPED)
		{
			//common::log("【万古神墟】本周已膜拜！");
			common::log(boost::str(boost::format("【万古神墟】%1% 本周已膜拜！") % list[i - 1]));
		}
	}
	
}

//============================================================================
// 万古神墟——面板信息
// "module":840,"action":0,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],Utils.LongUtil]
//		oObject.list(param1,this._info,["result","best_player_info","can_get_coin"]);
// 
// Example
//			[ 0, [ [ 5, 1, 507, 5297, "56uu_s070", "\u6768\u5c0f\u4e8c.s60", 101, 8188, 1370981116, 1, 1237, [ [ 1, 506 ], [ 1, 507 ] ], 100000000, 100000000 ], [ 3, 11, 0, 6, "669ye_s1489", "\u866b\u5b50", 4, 8179, 878452118, 1, 74036, [ [ 1, 0 ], [ 2, 0 ], [ 3, 0 ], [ 4, 0 ], [ 5, 0 ], [ 6, 0 ], [ 7, 0 ], [ 8, 0 ], [ 9, 0 ], [ 10, 0 ], [ 11, 0 ] ], 200000000, 1000000000 ], [ 4, 18, 0, 5297, "56uu_s070", "\u6768\u5c0f\u4e8c.s60", 2, 8188, 883232462, 1, 75184, [ [ 1, 0 ], [ 2, 0 ], [ 3, 0 ], [ 4, 0 ], [ 5, 0 ], [ 6, 0 ], [ 7, 0 ], [ 8, 0 ], [ 9, 0 ], [ 10, 0 ], [ 11, 0 ], [ 12, 0 ], [ 13, 0 ], [ 14, 0 ], [ 15, 0 ], [ 16, 0 ], [ 17, 0 ], [ 18, 0 ] ], 100000000, 500000000 ], [ 1, 1, 0, 0, "", "", 0, 0, 0, 0, 0, [ [ 1, 0 ] ], 0, 0 ], [ 2, 2, 0, 91553, "57k_s0212", "\u53f8\u7a7a\u541f\u6708.s35", 4, 0, 1361537169, 1, 44538, [ [ 1, 0 ], [ 2, 0 ] ], 500000000, 2147483647 ] ], 0 ] 
//============================================================================
Json::Value sxd_client::Mod_EternalGamesPresent_Base_main_info()
{
	Json::Value data;
	return this->send_and_receive(data, 840, 0);
}

//============================================================================
// 万古神墟——膜拜
// "module":840,"action":1,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],Utils.LongUtil]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
// 【1】万古神墟-神仙联赛
// 【2】万古神墟-殿堂赛
// 【3】万古神墟-封神榜
// 【4】万古神墟-封神之战
// 【5】万古神墟-阶位赛
//============================================================================
Json::Value sxd_client::Mod_EternalGamesPresent_Base_worship(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 840, 1);
}

//============================================================================
// 万古神墟——获取奖励
// "module":840,"action":2,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],Utils.LongUtil]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
//============================================================================
Json::Value sxd_client::Mod_EternalGamesPresent_Base_get_award()
{
	Json::Value data;
	return this->send_and_receive(data, 840, 2);
}