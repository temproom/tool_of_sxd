#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class ShanhaiType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 2;
	static const int IS_FULL = 3;		//频道人数已达上限
	static const int SERVER_CLOSED = 1;	//服务器关闭
};

class TongTianTower
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 1;
	static const int TOP_FLOOR = 2;				//到达最高层
	static const int BAG_FULL = 3;				//包满
	static const int CD_TIME = 4;				//cd中
	static const int NOT_ENOUGH_POWER = 5;		//体力不够
	static const int CHALLENGE_TIMES_LIMIT = 6;	//挑战次数限制
	static const int NO_COMPLETE = 8;	
	static const int IN_PRACTICE = 9;			//扫荡中
	static const int MATERIAL_NUM_LIMIT = 10;	//材料满
	static const int HEAVEN_FLOOR_ERROR = 11;
};
int k=1;
int sxd_client::shanhaiworld_login(sxd_client* sxd_client_town)
{
	//1. 获取登录山海界的信息
	Json::Value data = sxd_client_town->Mod_ShanhaiWorld_Base_get_login_info(2);
	std::string host = data[1].asString();
	std::string port = data[2].asString();
	std::string server_name = data[3].asString();
	int time = data[4].asInt();
	std::string pass_code = data[5].asString();
	this->user_id = sxd_client_town->user_id;

	// 2. 山海界的id是153
	int town_map_id = 153;

	// 3. 连接服务器
	this->connect(host, port);
	common::log(boost::str(boost::format("【山海界】连接服务器 [%1%:%2%] 成功") % host % port), iEdit);

	// 4. 登录
	data = this->Mod_ShanhaiWorld_Base_login(server_name, sxd_client_town->player_id, time, pass_code);
	if (data[0].asInt() != 0)
	{
		std::cout << data[0].asInt() << "\n" << ShanhaiType::SUCCESS << "\n";
		common::log(boost::str(boost::format("【山海界】玩家登录 [山海界] 失败，result[%1%]") % data[0]), iEdit);
		return 1;
	}
	int room_id = data[3].asInt();
	common::log(boost::str(boost::format("【山海界】登录成功，player_id[%1%]") % player_id), iEdit);

	//5. 进入山海界城镇
	data = this->Mod_ShanhaiWorld_Base_enter_town(room_id);
	if (data[0].asInt() != 0)
	{
		std::cout << data[0].asInt() << "\n" << ShanhaiType::SUCCESS << "\n";
		common::log(boost::str(boost::format("【山海界】玩家进入城镇 [山海界] 失败，result[%1%]") % data[0]), iEdit);
		return 2;
	}
	common::log("【山海界】玩家进入 [山海界]", iEdit);

	bLogin = 1;
	return 0;
}

void sxd_client::tong_tian_tower()
{
	std::cout << "第" <<k<< "次挑战\n";
	Json::Value data = this->Mod_ShanhaiWorld_Base_challenge();
	if (data[0] == TongTianTower::CHALLENGE_TIMES_LIMIT)
	{
		std::cout << "挑战通天塔成功\n";
	}
	else
	{
		k++;
		this->tong_tian_tower();
	}

	/*Json::Value data = this->Mod_ShanhaiWorld_Base_get_room_list();
	if (data[0] == ShanhaiType::SUCCESS)
	{

	}*/
}

//============================================================================
//  山海界登录信息
// {module:645, action:1, request:[Utils.IntUtil],
// Example:
//			get_login_info_callback,[this._nodeId])
// response:[Utils.UByteUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil]
// Example	
//			["result","host","port","server_name","time","pass_code"]
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_get_login_info(int node_id)
{
	Json::Value data;
	data.append(node_id);
	return this->send_and_receive(data, 645, 1);
}

//============================================================================
//  登录山海界
// {module:645, action:2,
// request:[Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil],
// Example		shanhaiWorld.mainPlayerInfo
//   view.as   this._data.call(Mod_ShanhaiWorld_Base.login,common_login_callback,[_ctrl.shanhaiWorld.loginInfo.server_name,_ctrl.player.playerId,_ctrl.shanhaiWorld.loginInfo.time,_ctrl.shanhaiWorld.loginInfo.pass_code],true,DataBase.SOCKET_SYS,name);
//		[server_name, playerId, time, pass_code]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//     [ result ,regId ,? ,roomId ]
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_login(const std::string& server_name, int player_id, int time, const std::string& pass_code)
{
	Json::Value data;
	data.append(server_name);
	data.append(player_id);
	data.append(time);
	data.append(pass_code);
	return this->send_and_receive(data, 645, 2);
}

//============================================================================
//  山海界房间信息
// {module:645, action:3,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// Example
//     [ result, nodeId, roomId, [node_id, room_id, player_num] ]
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_get_room_list()
{
	Json::Value data;
	return this->send_and_receive(data, 645, 3);
}

//============================================================================
//  进入山海界城镇
// {module:645, action:4,
// request:[Utils.IntUtil,Utils.IntUtil]
// Example		
//	shanhaiworldmapview.as	692		 _data.call(Mod_ShanhaiWorld_Base.enter_town,this.enter_town_back,[_id,_loc1_],true,DataBase.SOCKET_SYS,CommonSocketForm.SOCKET_SHJJ);
//     [ 153,room_id ]	_id = TownType.getId(TownType.ShanHaiJiuJie);
// response:[Utils.UByteUtil]
// Example
//     [ result ]
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_enter_town(int room_id)
{
	Json::Value data;
	data.append(153);
	data.append(room_id);
	return this->send_and_receive(data, 645, 4);
}

//============================================================================
//  通天塔信息
// {module:648, action:0,
// request:[]
// Example		
//
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//	TongTianTowerData.as 82 
//	oObject.list(param1,this._panelData,["heaven","floor","get_ingot","award_ingot_limit","total_pass_floor","is_break_up","difficulty","completed_num","break_up_time_left","cur_heaven","cur_floor","material_dop_num"]);
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 648, 0);
}

//============================================================================
//  通天塔挑战
// {module:648, action:1,
// request:[]
// Example		
//
// response:
// Example
//	TongTianTowerData.as 82
//    this._result = list[0];
//============================================================================
Json::Value sxd_client::Mod_ShanhaiWorld_Base_challenge()
{
	Json::Value data;
	return this->send_and_receive(data, 648, 1);
}