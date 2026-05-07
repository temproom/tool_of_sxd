#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//ÏÉ¼£Ìì°ñ
class SeasonRankingtType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int IS_GET = 2;
};

void sxd_client::SeasonRanking()
{
	//Json::Value data = this->Mod_SeasonRanking_Base_get_self_info();
	//data = this->Mod_SeasonRanking_Base_get_blessing_info();

	for (int i = 2; i <= 10; i++)
	{
		Json::Value data_award = this->Mod_SeasonRanking_Base_get_blessing_award(i);
		if (data_award[0].asInt() == SeasonRankingtType::SUCCESS)
		{
			common::log(boost::str(boost::format("¡¾ÏÉ¼£Ìì°ñ¡¿ÁìÈ¡×£¸£µÇ¼Ç½±Àø [%1%] ³É¹¦£¡") % i));
		}
		else if (data_award[0].asInt() == SeasonRankingtType::IS_GET)
		{
			common::log(boost::str(boost::format("¡¾ÏÉ¼£Ìì°ñ¡¿×£¸£½±Àø [%1%] ÒÑÁìÈ¡£¡") % i), iEdit);
		}
		else if (data_award[0].asInt() == SeasonRankingtType::FAILED)
		{
			common::log(boost::str(boost::format("¡¾ÏÉ¼£Ìì°ñ¡¿×£¸£½±Àø [%1%] ÁìÈ¡Ê§°Ü£¡") % i), iEdit);
			break;
		}
	}
	
}

//============================================================================
// ÏÉ¼£Ìì°ñ¡ª¡ª¸öÈËÐÅÏ¢
// "module":739,"action":1,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
//		oObject.list(param1,this.objSelfRank,["result","season","round","self_war_zone","score_list","blessing_rate","top_rank"]);
// 
// Example
//		[ 0, 5, 9, 3, [ [ 64, 0 ], [ 69, 0 ], [ 19, 3600 ], [ 32, 8202 ], [ 5, 10000 ], [ 20, 156925 ], [ 26, 58320 ], [ 65, 0 ], [ 8, 0 ], [ 3, 1505 ], [ 52, 0 ], [ 61, 0 ], [ 62, 3000 ], [ 71, 0 ], [ 4, 8020 ], [ 54, 0 ], [ 7, 0 ], [ 46, 0 ], [ 30, 8020 ], [ 68, 0 ], [ 12, 1048 ], [ 57, 2555 ], [ 70, 0 ], [ 37, 20958 ], [ 39, 0 ], [ 41, 0 ], [ 49, 0 ], [ 21, 52096 ], [ 56, 0 ], [ 17, 4500 ], [ 34, 15460 ], [ 67, 0 ], [ 36, 10800 ], [ 1, 2440 ], [ 44, 0 ], [ 29, 43539 ], [ 35, 35000 ], [ 47, 0 ], [ 24, 2914 ], [ 11, 2925 ], [ 25, 4140 ], [ 13, 1000 ], [ 53, 0 ], [ 28, 41566 ], [ 23, 2717 ], [ 48, 0 ], [ 16, 5000 ], [ 50, 0 ], [ 40, 0 ], [ 10, 0 ], [ 9, 0 ], [ 31, 27000 ], [ 63, 2200 ], [ 74, 0 ], [ 43, 0 ], [ 42, 0 ], [ 14, 2000 ], [ 51, 0 ], [ 72, 0 ], [ 18, 11000 ], [ 66, 0 ], [ 2, 1968 ], [ 22, 6454 ], [ 55, 0 ], [ 45, 0 ], [ 15, 3000 ], [ 27, 26307 ], [ 73, 0 ], [ 33, 34800 ], [ 6, 0 ], [ 58, 2200 ], [ 38, 0 ], [ 75, 0 ] ], 135, 117 ] 
//============================================================================
Json::Value sxd_client::Mod_SeasonRanking_Base_get_self_info()
{
	Json::Value data;
	return this->send_and_receive(data, 739, 1);
}

//============================================================================
// ÏÉ¼£Ìì°ñ¡ª¡ª×£¸£ÐÅÏ¢
// "module":739,"action":4,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil]]
//		oObject.list(param1,this.objBlessingInfo,["result","total_exp","get_list"]);
// 
// Example
//			[ 0, 89430, [ [ 10 ], [ 8 ], [ 3 ], [ 7 ], [ 5 ], [ 4 ], [ 6 ], [ 9 ], [ 2 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_SeasonRanking_Base_get_blessing_info()
{
	Json::Value data;
	return this->send_and_receive(data, 739, 4);
}

//============================================================================
// ÏÉ¼£Ìì°ñ¡ª¡ª»ñÈ¡×£¸£½±Àø
// "module":739,"action":5,
// "request":[Utils.IntUtil],
//			_data.call(Mod_SeasonRanking_Base.get_blessing_award,this.get_blessing_award_callback,[param1]);
// 
// "response":[Utils.UByteUtil]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
//============================================================================
Json::Value sxd_client::Mod_SeasonRanking_Base_get_blessing_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 739, 5);
}