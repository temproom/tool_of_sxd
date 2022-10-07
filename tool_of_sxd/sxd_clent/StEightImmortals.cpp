#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"
#include <windows.h>
#include <regex.hpp>
#include <algorithm/string.hpp>
#include <algorithm/string/regex.hpp>

class EightImmortalsType
{
public:
	static const int ONE = 7;
	static const int TWO = 8;
	static const int THREE = 9;

	static const int LevelLimit = 13;
	static const int SUCCESS = 10;
	static const int FAILURE = 38;
	static const int EMPTY_HEALTH = 17;
	static const int MISSION_COMPLETE = 33;
};

void sxd_client::StEightImmortals()
{
	Json::Value data;
	data = this->Mod_StEightImmortals_Base_get_finished_missions(7);
	for (int i = 5; i < 73; i++)
	{
		data = this->Mod_StEightImmortals_Base_enter_mission(i);
		if (data[0].asInt() == 10)
		{
			int k = 0;
			while (k != 21)
			{
				data = this->Mod_StEightImmortals_Base_fight();
				k = data[0].asInt();
				std::cout << k << "\n";
				if (k == 19)
				{
					Json::Value datak;
					this->send_and_receive(datak, 129, 5);
				}
				if (k == 17)
				{
					i--;
					break;
				}
			}
		}
	}
	data = this->Mod_StEightImmortals_Base_enter_mission(4);
	std::cout << "看看看\n";
	int k = 0;
	//data = this->Mod_StEightImmortals_Base_fight();
	while (k!=21)
	{
		data = this->Mod_StEightImmortals_Base_fight();
		k = data[0].asInt();
	}
}

//============================================================================
//  获取八仙过海副本信息
// {module:129, action:0,
// request:[Utils.UByteUtil],
// StEightCardView.sa 67.
//   _data.call(Mod_StEightImmortals_Base.get_finished_missions,param1,[Mod_StEightImmortals_Base.ONE],true,1);
// Example
//     [ 4148 ]
// response:[[Utils.ByteUtil,Utils.ByteUtil,Utils.UByteUtil,Utils.UByteUtil,Utils.UByteUtil]]
// StEightImmortalsData.as 57:
//     oObject.list(_loc5_,_loc8_,["mission_seq","star","star1","star2","star3"]);
// Example
// [["mission_seq","star","star1","star2","star3"],[,,,,]]
//============================================================================
Json::Value sxd_client::Mod_StEightImmortals_Base_get_finished_missions(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 129, 0);
}

//============================================================================
//  进入八仙过海副本
// {module:129, action:1,
// request:[Utils.ByteUtil],
// Example
//     [ 33 ]
// response:[Utils.UByteUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil],[Utils.ByteUtil],Utils.ByteUtil,Utils.ByteUtil]
// StEightImmortalsData.as 117:
//     oObject.list(param1,this.enter_mission_info,["result","next_wave_type","assistant_list","enemy_list","item_list","health_prop_a","health_prop_b"]);
// Example
// status=1, 已通关, 可扫荡; status=0, 未通关
//if (_loc2_.isBoss == 0){_loc2_.monsterLen = 3;}else{_loc2_.monsterLen = 1;}
//     [ [ [ 866, 1, 74, 0, 0, 0 ], [ 867, 1, 75, 0, 0, 0 ], [ 868, 1, 74, 1, 0, 0 ], [ 869, 1, 74, 0, 0, 0 ], [ 870, 1, 74, 0, 0, 0 ], [ 871, 1, 74, 1, 0, 0 ], [ 872, 1, 74, 0, 0, 0 ], [ 873, 1, 74, 0, 0, 0 ], [ 874, 1, 74, 1, 0, 0 ], [ 875, 1, 74, 0, 0, 0 ], [ 876, 1, 76, 0, 0, 0 ], [ 877, 1, 76, 1, 0, 0 ], [ 878, 1, 76, 0, 0, 0 ], [ 879, 1, 76, 0, 0, 0 ], [ 880, 1, 76, 1, 0, 0 ], [ 881, 1, 76, 0, 0, 0 ], [ 882, 1, 76, 0, 0, 0 ], [ 883, 1, 76, 1, 0, 0 ], [ 884, 1, 76, 0, 0, 0 ], [ 885, 1, 76, 0, 0, 0 ], [ 886, 1, 76, 1, 0, 0 ], [ 887, 1, 76, 0, 0, 0 ], [ 888, 1, 76, 0, 0, 0 ], [ 889, 1, 76, 1, 0, 0 ], [ 890, 0, 76, 0, 0, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_StEightImmortals_Base_enter_mission(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 129, 1);
}

//============================================================================
//  进入八仙过海副本
// {module:129, action:2,
// request:[Utils.IntUtil],
// Example
//     [ 33 ]
// response:[[Utils.IntUtil, Utils.ByteUtil, Utils.UByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil]]};
// StEightImmortalsData.as 144:
//     oObject.list(param1,this.enter_mission_info,["result","war_result_list","star","new_stage_clear_flag","next_wave_type","assistant_list","enemy_list","item_list","award_coin","award_aura"]);
// Example
// status=1, 已通关, 可扫荡; status=0, 未通关
//if (_loc2_.isBoss == 0){_loc2_.monsterLen = 3;}else{_loc2_.monsterLen = 1;}
//     [ [ [ 866, 1, 74, 0, 0, 0 ], [ 867, 1, 75, 0, 0, 0 ], [ 868, 1, 74, 1, 0, 0 ], [ 869, 1, 74, 0, 0, 0 ], [ 870, 1, 74, 0, 0, 0 ], [ 871, 1, 74, 1, 0, 0 ], [ 872, 1, 74, 0, 0, 0 ], [ 873, 1, 74, 0, 0, 0 ], [ 874, 1, 74, 1, 0, 0 ], [ 875, 1, 74, 0, 0, 0 ], [ 876, 1, 76, 0, 0, 0 ], [ 877, 1, 76, 1, 0, 0 ], [ 878, 1, 76, 0, 0, 0 ], [ 879, 1, 76, 0, 0, 0 ], [ 880, 1, 76, 1, 0, 0 ], [ 881, 1, 76, 0, 0, 0 ], [ 882, 1, 76, 0, 0, 0 ], [ 883, 1, 76, 1, 0, 0 ], [ 884, 1, 76, 0, 0, 0 ], [ 885, 1, 76, 0, 0, 0 ], [ 886, 1, 76, 1, 0, 0 ], [ 887, 1, 76, 0, 0, 0 ], [ 888, 1, 76, 0, 0, 0 ], [ 889, 1, 76, 1, 0, 0 ], [ 890, 0, 76, 0, 0, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_StEightImmortals_Base_fight()
{
	Json::Value data;
	return this->send_and_receive(data, 129, 2);
}