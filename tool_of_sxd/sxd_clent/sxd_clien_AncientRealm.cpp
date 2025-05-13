#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class AncientRealmtype
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
	static const int FAILED = 3;
	static const int NOT_IN_RUNING_STATE = 21;
};

void sxd_client::AncientRealm()
{
	common::log("【玄天古境】状态信息！！");
	Json::Value data = this->Mod_AncientRealm_Base_get_state();
	common::log("【玄天古境】地图信息！！");
	data = this->Mod_AncientRealm_Base_get_map_panel_info();
	common::log("【玄天古境】战利品信息！！");
	//data = this->Mod_AncientRealm_Base_spoil_panel();
}

//============================================================================
// 玄天古境：获取状态
// {module:770, action:0, 
// request:[],
// 
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// AncientRealmData.as
//     oObject.list(param1,this._objGetState,["state_id","season","is_func_open","is_have_team"]);
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 0);
}

//============================================================================
//	玄天古境：获取地图信息
// "module":770,"action":12,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.mapPanelInfo,["result","is_first_come_layer","is_need_choose_spoil","is_fight_boss","team_id","layer_num","progress","goal_id","coin_num","team_member_list","layer_mission_list","grid_info_list","my_coin_num","boss_monster_team_id","is_get_key1","is_get_key2","is_experience_red_dot","sect_id","team_rank","is_have_store","is_order_red_dot"]);
// Example
//			[ 0, 0, 0, 1, 10254, 1, 89, 3, 1070, 
// [ [ 325, "\u51af\u6d77\u68ee\u5821", 207, 0, 7857, 0, 0, 26, 1, 4, 4, 211, 100 ], [ 19824, "\u5929\u624d\u5e05\u5e05.s51", 207, 12, 8120, 0, 0, 100, 0, 4, -1, 224623, 100 ], [ 1836, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51", 201, 11, 8120, 1, 0, 100, 0, 5, -1, 212358, 100 ] ],
// [ [ 3, 1 ], [ 4, 8 ], [ 1, 100 ], [ 2, 2 ] ], 
// [ [ 4, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 1, 8, 1, 1, 0, 0, 0 ], [ 5, 5, 7, 1, 0, 0, 0, 0 ], [ 4, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 4, 8, 1, 1, 0, 0, 0 ], [ 8, 3, 8, 0, 0, 1, 0, 0 ], [ 3, 9, 8, 1, 1, 0, 0, 0 ], [ 0, 4, 4, 1, 1, 0, 0, 0 ], [ 3, 0, 8, 1, 1, 0, 0, 0 ], [ 8, 5, 3, 0, 0, 1, 0, 0 ], [ 0, 8, 10, 0, 0, 0, 0, 12360 ], [ 1, 10, 2, 0, 0, 1, 0, 12354 ], [ 5, 10, 8, 1, 1, 0, 0, 0 ], [ 1, 7, 8, 1, 1, 0, 0, 0 ], [ 7, 4, 10, 1, 0, 0, 0, 12360 ], [ 8, 6, 8, 0, 0, 1, 0, 0 ], [ 0, 1, 8, 1, 1, 0, 0, 0 ], [ 0, 3, 8, 1, 1, 0, 0, 0 ], [ 8, 7, 8, 0, 0, 1, 0, 0 ], [ 3, 2, 8, 1, 1, 0, 0, 0 ], [ 4, 8, 7, 1, 0, 0, 0, 0 ], [ 3, 6, 8, 1, 1, 0, 0, 0 ], [ 8, 4, 4, 0, 0, 1, 0, 0 ], [ 5, 6, 8, 1, 1, 0, 0, 0 ], [ 3, 3, 8, 1, 1, 0, 0, 0 ], [ 7, 7, 8, 1, 1, 0, 0, 0 ], [ 1, 5, 7, 1, 0, 0, 0, 0 ], [ 2, 7, 8, 1, 1, 0, 0, 0 ], [ 0, 0, 3, 1, 1, 0, 0, 0 ], [ 5, 2, 8, 1, 1, 0, 0, 0 ], [ 5, 0, 4, 1, 0, 0, 0, 0 ], [ 7, 10, 8, 0, 0, 1, 0, 0 ], [ 2, 10, 10, 1, 0, 0, 0, 12360 ], [ 2, 6, 3, 1, 1, 0, 0, 0 ], [ 4, 3, 4, 1, 1, 0, 0, 0 ], [ 5, 8, 8, 1, 1, 0, 0, 0 ], [ 2, 4, 8, 1, 1, 0, 0, 0 ], [ 0, 6, 4, 1, 1, 0, 0, 0 ], [ 6, 6, 3, 1, 1, 0, 0, 0 ], [ 3, 4, 7, 1, 0, 0, 0, 0 ], [ 4, 1, 8, 1, 1, 0, 0, 0 ], [ 7, 3, 1, 0, 0, 1, 0, 12353 ], [ 1, 4, 8, 1, 1, 0, 0, 0 ], [ 4, 10, 7, 1, 0, 0, 0, 0 ], [ 4, 6, 8, 1, 1, 0, 0, 0 ], [ 6, 3, 8, 1, 1, 0, 0, 0 ], [ 2, 5, 8, 1, 1, 0, 0, 0 ], [ 1, 6, 3, 1, 1, 0, 0, 0 ], [ 5, 7, 8, 1, 1, 0, 0, 0 ], [ 6, 5, 3, 0, 0, 1, 0, 0 ], [ 4, 7, 4, 1, 1, 0, 0, 0 ], [ 4, 2, 5, 1, 1, 0, 1, 0 ], [ 3, 5, 8, 1, 1, 0, 0, 0 ], [ 2, 3, 6, 1, 1, 0, 1, 0 ], [ 4, 0, 4, 1, 1, 0, 0, 0 ], [ 5, 4, 2, 1, 1, 0, 0, 12354 ], [ 0, 2, 8, 1, 1, 0, 0, 0 ], [ 1, 3, 8, 1, 1, 0, 0, 0 ], [ 1, 0, 8, 1, 1, 0, 0, 0 ], [ 2, 0, 3, 1, 1, 0, 0, 0 ], [ 3, 8, 8, 1, 1, 0, 0, 0 ], [ 6, 7, 8, 1, 1, 0, 0, 0 ], [ 5, 3, 8, 1, 1, 0, 0, 0 ], [ 3, 1, 3, 1, 1, 0, 0, 0 ], [ 0, 5, 8, 1, 1, 0, 0, 0 ], [ 5, 9, 4, 1, 1, 0, 0, 0 ], [ 3, 10, 10, 1, 1, 0, 0, 12360 ], [ 6, 10, 10, 1, 0, 0, 0, 12360 ], [ 6, 4, 8, 1, 1, 0, 0, 0 ], [ 7, 6, 10, 1, 0, 0, 0, 12360 ], [ 7, 5, 8, 0, 0, 1, 0, 0 ], [ 3, 7, 1, 1, 1, 0, 0, 12353 ] ],
// 3986, 12355, 1, 1, 1, 1, 1738, 0, 1 ]  
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_map_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 12);
}

//============================================================================
//	玄天古境：经验信息
// "module":770,"action":14,
// "request":[Utils.IntUtil],
// 
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
// AncientRealmData.as		
//			         oObject.list(param1,this._objTask,["experience_mission_list","is_active_title"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_experience_panel(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 14);
}

//============================================================================
//	玄天古境：获取经验奖励
// "module":770,"action":15,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_experience_award(int type, int id)
{
	Json::Value data;
	data.append(type);
	data.append(id);
	return this->send_and_receive(data, 770, 15);
}

//============================================================================
//	玄天古境：打开地图格子
// "module":770,"action":18,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
//
// 1.护境使者；2.玄境灵兽；3.加成格；4.奖励格；5.钥匙格1；6.钥匙格2；7.路障格；8.普通格；9.游商格；10.古境戍卫
// 
// 
//
Json::Value sxd_client::Mod_AncientRealm_Base_open_grid(int floor, int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 18);
}

//============================================================================
//	玄天古境：挑战怪物
// "module":770,"action":19,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// "response":
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_fight_monster(int floor, int id)
{
	Json::Value data;
	data.append(floor);
	data.append(id);
	return this->send_and_receive(data, 770, 19);
}

//============================================================================
//	玄天古境：战利品信息
// "module":770,"action":20,
// "request":[]
// 
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_spoil_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 20);
}

//============================================================================
//	玄天古境：选择战利品
// "module":770,"action":21,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_choose_spoil(int id1, int id2, int id3)
{
	Json::Value data;
	data.append(id1);
	data.append(id2);
	data.append(id3);
	return this->send_and_receive(data, 770, 21);
}

//============================================================================
//	玄天古境：挑战BOSS
// "module":770,"action":23,
// "request":[]
// 
// "response":
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_fight_boss()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 23);
}

//============================================================================
//	玄天古境：前往下一层
// "module":770,"action":24,
// "request":[]
// 
// "response":[Utils.UByteUtil]
//		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_click_pass()
{
	Json::Value data;
	return this->send_and_receive(data, 770, 24);
}

//============================================================================
//	玄天古境：获取怪物团信息
// "module":770,"action":27,
// "request":[Utils.IntUtil],
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_get_monster_team_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 27);
}

//============================================================================
//	玄天古境：打开格子宝箱
// "module":770,"action":30,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
// "response":[Utils.UByteUtil,Utils.IntUtil]
// 		
// AncientRealmData.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//============================================================================
Json::Value sxd_client::Mod_AncientRealm_Base_open_award_grid_box(int floor, int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 770, 30);
}