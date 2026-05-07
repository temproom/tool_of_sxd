#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//中秋博饼
class MidAutuGameType
{
public:
	static const int CAN_PLAY = 0;
	static const int NOT_MORE_TIME = 1;
};

void sxd_client::MidAutuGame()
{
	Json::Value data = this->Mod_MidAutuGame_Base_get_midautu_game_info();
	int rest_times = data[3].asInt();
	//data = this->Mod_MidAutuGame_Base_play_games();
	while (rest_times > 0)
	{
		data = this->Mod_MidAutuGame_Base_play_games();
		if (data[0].asInt() == MidAutuGameType::NOT_MORE_TIME)
		{
			common::log("【中秋博饼】次数不足！");
			return;
		}
		else
		{
			common::log("【中秋博饼】博饼成功！");
			rest_times--;
		}
	}
}

//============================================================================
// 中秋博饼——获取信息
// "module":377,"action":0,
// "request":[]
// 
// "response":[Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],Utils.IntUtil]
//		 oObject.list(param1,this.gameObject,["active_num","today_max_times","interval","rest_times","place_list","per"]); 
// 
// Example
//			[ 1085, 10, 130, 0, [ [ 5, 20, [ [ 1740, 20 ] ] ], [ 3, 50, [ [ 2397, 20 ] ] ], [ 4, 40, [ [ 8161, 15 ] ] ], [ 7, 5, null ], [ 1, 100, [ [ 3345, 500 ] ] ], [ 2, 70, [ [ 8359, 1 ] ] ], [ 6, 10, [ [ 347, 50 ] ] ] ], 100 ]  
//============================================================================
Json::Value sxd_client::Mod_MidAutuGame_Base_get_midautu_game_info()
{
	Json::Value data;
	return this->send_and_receive(data, 377, 0);
}

//============================================================================
// 中秋博饼——博饼
// "module":377,"action":2,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		oObject.list(param1,this.gamePlayObject,["result","dices_list","place","get_award","rest_times","active_num","interval"]);
// 
// Example
//			[ 1, null, 0, null, 0, 1085, 130 ] 
//============================================================================
Json::Value sxd_client::Mod_MidAutuGame_Base_play_games()
{
	Json::Value data;
	return this->send_and_receive(data, 377, 2);
}