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
	static const int MAIN_TEAM = 0;				//主队
	static const int SOUL_TEAM = 1;				//灵结队
	static const int SUCCESS = 2;
	static const int FAILD = 3;
	static const int TOP_FLOOR = 4;				//到达最高层
	static const int BAG_FULL = 5;				//包满
	static const int CD_TIME = 6;				//cd中
	static const int NOT_ENOUGH_POWER = 7;		//体力不够
	static const int CHALLENGE_TIMES_LIMIT = 8;	//挑战次数限制
	static const int NO_COMPLETE = 12;	
	static const int IN_PRACTICE = 13;			//扫荡中
	static const int MATERIAL_NUM_LIMIT = 14;	//材料满
	static const int HEAVEN_FLOOR_ERROR = 15;
	static const int PRACTICE_END = 20;
};

int k=1;
//int challenge_times = 5;
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

void sxd_client::tong_tian_tower_challenge(int heaven, int floor, Json::Value data)
{
	//std::cout << "\n111\n";
	std::cout << boost::str(boost::format("【通天塔】：第[ %1% ]次挑战第[%2%]重[%3%]层\n") % k % heaven % floor);
	Json::Value data_re = this->Mod_TongTianTower_Base_challenge(data);
	if (data_re[0] == TongTianTower::NOT_ENOUGH_POWER)
	{
		std::cout << "【通天塔】：体力不足！！\n";
	}
	//std::cout << "\n222\n";
	//通天塔信息
	Json::Value info = this->Mod_TongTianTower_Base_panel_info();
	//std::cout << "\n333\n";
	//剩余挑战次数
	int times = info[4].asInt();
	int now_floor = info[1].asInt();	//层数

	if (now_floor != floor)
	{
		//层数不相同，那就是挑战成功
		std::cout << boost::str(boost::format("【通天塔】：第[%1%]重[%2%]层 挑战成功！！\n") % heaven % floor);
		k = 1;
		if (times != 0)
		{
			this->tong_tian_tower_challenge();
		}
	}
	else
	{
		k++;
		this->tong_tian_tower_challenge(heaven, floor, data);
	}
	/*if (data_re[0] == TongTianTower::SUCCESS)
	{
		std::cout << boost::str(boost::format("【通天塔】：第[%1%]重[%2%]层 挑战成功！！\n") % heaven % floor);
	}
	else
	{
		k++;
		this->tong_tian_tower_challenge(heaven, floor, data);
	}*/
}

void sxd_client::tong_tian_tower_challenge()
{
	//获取当前挑战信息
	Json::Value info = this->Mod_TongTianTower_Base_panel_info();

	int heaven = info[0].asInt();	//重数
	int floor = info[1].asInt();	//层数
	int challenge_times = info[4].asInt();	//剩余挑战次数

	if (challenge_times == 0)
	{
		common::log("【通天塔】今日挑战完毕", iEdit);
		return;
	}
	//cout << "challenge_times: " << challenge_times << "\n";
	//查表
	mss via_info = db.get_team_id(version, heaven, floor);

	int m_type = 0;	//是否灵结队伍，真为灵结，假为普通
	if (stoi(via_info["attr_prob"]) != 0)
	{
		m_type = 1;
	}

	//vector<int> temp_data;
	Json::Value temp_data;
	if (!m_type)
	{
		//如果是普通副本,只有一个team
		Json::Value temp_data1;
		int team_id = stoi(via_info["monster_team_id"]);
		temp_data1.append(TongTianTower::MAIN_TEAM);
		temp_data1.append(team_id);
		temp_data.append(temp_data1);
	}
	else
	{
		//如果是灵结副本,有两个team
		int team_id1 = stoi(via_info["monster_team_id"]);
		int team_id2 = stoi(via_info["soul_monster_team_id"]);
		Json::Value temp_data1, temp_data2;
		temp_data1.append(TongTianTower::MAIN_TEAM);
		temp_data1.append(team_id1);
		temp_data2.append(TongTianTower::SOUL_TEAM);
		temp_data2.append(team_id2);
		temp_data.append(temp_data1);
		temp_data.append(temp_data2);
	}
	this->tong_tian_tower_challenge(heaven, floor, temp_data);
}

void sxd_client::tong_tian_tower_practice()
{
	Json::Value data = this->Mod_TongTianTower_Base_panel_info();
	int cur_heaven = data[9].asInt();		//当前重数
	int cur_floor = data[10].asInt();		//当前层数
	data = this->Mod_Player_Base_get_player_info();
	int ti_li = data[6].asInt();
	if (ti_li < 10)
	{
		common::log("【通天塔】：体力不足！");
		return;
	}
	int times = ti_li / 10;
	common::log(boost::str(boost::format("【通天塔】：当前第[%1%]重，第[%2%]层，体力：[%3%]") % cur_heaven % cur_floor % ti_li));
	common::log("请输入扫荡次数：");
	int k;
	std::cin >> k;
	if (k > 0 && k <= times)
	{
		common::log(boost::str(boost::format("【通天塔】：扫荡[%1%]次\n") % k));
		data = this->Mod_TongTianTower_Base_practice(cur_heaven, cur_floor, k);
	}
	else
	{
		common::log(boost::str(boost::format("【通天塔】：扫荡[%1%]次\n") % times));
		data = this->Mod_TongTianTower_Base_practice(cur_heaven, cur_floor, times);
	}		
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
//	         oObject.list(param1,this._panelData,["heaven","floor","get_ingot","award_ingot_limit","total_pass_floor","is_break_up","difficulty","completed_num","break_up_time_left","cur_heaven","cur_floor","material_dop_num"]);
//									new!         ["heaven","floor","get_ingot","award_ingot_limit","challenge_times", "is_break_up","difficulty","completed_num","break_up_time_left","cur_heaven","cur_floor","material_dop_num"]);
//  20231113 oObject.list(param1,this._panelData,["heaven","floor","get_ingot","award_ingot_limit","challenge_times", "is_break_up","difficulty","completed_num","break_up_time_left","cur_heaven","cur_floor","material_dop_num","is_weekend_discount","top_rank","top_time","hire_role_id"]);
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_TongTianTower_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 648, 0);
}

//============================================================================
//  通天塔挑战
// {module:648, action:1,
// request:[[Utils.UByteUtil,Utils.IntUtil]],
// TongTianTowerView.as 305
//		_data.call(Mod_TongTianTower_Base.challenge,startChallengeBack,[_view.commenMonsterDeploy.getTeamArray(_isHaveProb)]);
// CommenMonsterDeployView.as 273
// public function getTeamArray(param1:Boolean) : Array
//		var _loc2_:Object = this._ttData[CommonDeployType.LandContractTeam1];
//		if (!param1)
//		{
//			return [[_loc2_.team_type, _loc2_.monster_team_id]];
//		}
//		var _loc3_ : Object = this._ttData[CommonDeployType.LandContractTeam2];
//		return [[_loc2_.team_type, _loc2_.monster_team_id], [_loc3_.team_type, _loc3_.monster_team_id]];
// Example		
//			[[0,]]
// response:
// 20231113 response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil]]
// old      response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]]],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil]]
// 20240422 response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil]],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil]]
// Example
//	TongTianTowerData.as 116
//    this._result = list[0];
//============================================================================
Json::Value sxd_client::Mod_TongTianTower_Base_challenge(Json::Value data_s)
{
	Json::Value data;
	data.append(data_s);
	return this->send_and_receive(data, 648, 1);
}

//============================================================================
//  通天塔扫荡
// {module:648, action:2,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],
//	MissionPracticeTTTView.as 97	
//			_data.call(Mod_TongTianTower_Base.practice,this.startPracticeCallBack,[this._towerData.cur_heaven,this._towerData.cur_floor,this._battleTimer]);
// Example	
//			[要扫荡的重数，要扫荡的层数，次数]
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//	TongTianTowerData.as 125
//    this._result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_TongTianTower_Base_practice(int cur_heaven,int cur_floor,int times)
{
	Json::Value data;
	data.append(cur_heaven);
	data.append(cur_floor);
	data.append(times);
	return this->send_and_receive(data, 648, 2);
}