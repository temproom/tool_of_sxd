#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class WanfaWorldtype
{
public:
	static const int SUCCESS = 0;
	static const int SERVER_CLOSED = 1;
	static const int FAILED = 2;
};

class TombArtifactstype
{
public:
	static const int DRAW_ONE = 0;
	static const int DRAW_TEN = 1;
	static const int SUCCESS = 2;
	static const int FAILED = 3;

	static const int xun_qi = 1;
	static const int zhao_ling = 2;
};

class EndlessExpeditiontype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
};

//诸法器冢
void sxd_client::TombArtifacts()
{
	//获取当前日历
	//获取当前时间戳
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	//使用localtime函数转换为本地时间
	struct tm local_time;
	localtime_s(&local_time, &now_c);
	int day = local_time.tm_mday;

	//偶数天为寻器，奇数天招灵
	int curid = day % 2 + 1;

	string cur;
	if (curid == 1)
	{
		cur = "寻器";
	}
	else
	{
		cur = "招灵";
	}

	//获取对应寻宝信息
	Json::Value data = this->Mod_TombArtifacts_Base_main_panel_info(curid);

	int free_times = data[1].asInt();
	if (free_times == 0)
	{
		common::log("【诸法器冢】今日免费次数已用完！");
	}
	else
	{
		data = this->Mod_TombArtifacts_Base_draw(curid, TombArtifactstype::DRAW_ONE);
		int result = data[0].asInt();
		if (result == TombArtifactstype::SUCCESS)
		{
			common::log(boost::str(boost::format("【诸法器冢】今日[ %1% ]成功！") % cur), iEdit);
			return;
		}
		else
		{
			common::log(boost::str(boost::format("【诸法器冢】今日[ %1% ]失败！代码：[%2%]") % cur % result), iEdit);
			return;
		}
	}
}

//诸法洞天
void sxd_client::EndlessExpedition()
{
	Json::Value data = this->Mod_EndlessExpedition_Base_mission_panel();
	
	int mission_id = data[2].asInt();
	//是否通过
	int pass_flag = data[3].asInt();
	if (pass_flag)
	{
		data = this->Mod_EndlessExpedition_Base_enter_next_mission();
		if (data[0].asInt() != EndlessExpeditiontype::SUCCESS)
		{
			common::log(boost::str(boost::format("【诸法洞天】进入下一个副本失败，代码[ %1% ]") % data[0].asInt()), iEdit);
			return;
		}
		data = this->Mod_EndlessExpedition_Base_mission_panel();

		mission_id = data[2].asInt();
	}
	
	int k = 1;
	while (k < 100)
	{
		data = this->Mod_EndlessExpedition_Base_challenge(mission_id);
		int result = data[0].asInt();
		
		Json::Value data_fight = this->Mod_EndlessExpedition_Base_mission_panel();
		int pass = data_fight[3].asInt();

		if (result == EndlessExpeditiontype::SUCCESS && pass)
		{
			common::log(boost::str(boost::format("【诸法洞天】第[ %1% ]次挑战第[ %2% ]层，成功！") % k % mission_id), iEdit);
			k = 1;

			//进入下一个副本
			data = this->Mod_EndlessExpedition_Base_enter_next_mission();
			if (data[0].asInt() != EndlessExpeditiontype::SUCCESS)
			{
				common::log(boost::str(boost::format("【诸法洞天】进入下一个副本失败，代码[ %1% ]") % data[0].asInt()), iEdit);
				return;
			}
			EndlessExpedition();
			return;
		}
		else
		{
			common::log(boost::str(boost::format("【诸法洞天】第[ %1% ]次挑战第[ %2% ]层，失败！") % k % mission_id), iEdit);
			k++;
		}
	}
}

//无尽模式
void sxd_client::Endlesschallenge()
{
	Json::Value data = this->Mod_EndlessExpedition_Base_endless_panel();

	int layer_id = data[2].asInt();
	int pass_flag = data[3].asInt();

	while (pass_flag)
	{
		//已通关，前往下一层
		data = this->Mod_EndlessExpedition_Base_endless_enter_next_layer();

		data = this->Mod_EndlessExpedition_Base_endless_panel();
		layer_id = data[2].asInt();
		pass_flag = data[3].asInt();
	}

	//data = this->Mod_EndlessExpedition_Base_get_monster_team_info(layer_id);

	//记录挑战次数
	int k = 1;

	while (!pass_flag)
	{
		Json::Value data_fight = this->Mod_EndlessExpedition_Base_endless_challenge(layer_id);

		int result = data_fight[0].asInt();
		if (result != EndlessExpeditiontype::SUCCESS)
		{
			common::log(boost::str(boost::format("【诸法洞天：无尽模式】挑战失败，代码[ %1% ]") % result), iEdit);
			return;
		}
		else if (result == EndlessExpeditiontype::SUCCESS)
		{
			data = this->Mod_EndlessExpedition_Base_endless_panel();

			if (data[3].asInt() == 1)
			{
				common::log(boost::str(boost::format("【诸法洞天：无尽模式】第[ %1% ]次挑战第[ %2% ]层，成功！") % k % layer_id), iEdit);

				//通关，前往下一层
				data = this->Mod_EndlessExpedition_Base_endless_enter_next_layer();
				data = this->Mod_EndlessExpedition_Base_endless_panel();
				layer_id = data[2].asInt();
				pass_flag = data[3].asInt();
				//重置挑战次数
				k = 1;
			}
			else
			{
				//未通关，继续挑战
				//已挑战100次，退出。
				common::log(boost::str(boost::format("【诸法洞天：无尽模式】第[ %1% ]次挑战第[ %2% ]层，失败！") % k % layer_id), iEdit);

				//挑战次数+1
				k++;

				if (k == 100)
				{
					return;
				}
			}
		}
		Sleep(5000);
	}

}

//============================================================================
//获取万法界登录信息
// "module":794,"action":1,
// "request":[]
// 
// [ 0, "9x288.sxdweb.xd.com", "8213", "360_s313", 1733130237, "a488a811d6f0c2c1ef92d27cc514505e" ] 
// 
// "response":[Utils.UByteUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil]
//oObject.list(param1,this._wfjLoginInfo,["result","host","port","server_name","time","pass_code"]);
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_WanfaWorld_Base_get_login_info()
{
	Json::Value data;
	return this->send_and_receive(data, 794, 1);
}

//============================================================================
//登录万法界
// 
// view.as
// this.commonConect(_ctrl.sectLogin.currentGroupInfo.socketName,_ctrl.sectLogin.currentGroupInfo.host,_ctrl.sectLogin.currentGroupInfo.port,this.sectLoginCallback,true);
// 需要先connect，再登录
// 
// CommonSocketForm.as
 /*      public static const SOCKET_SCET:String = "SOCKET_SCET";
      
      public static const SOCKET_PP:String = "SOCKET_PP";
      
      public static const SOCKET_SHJJ:String = "SOCKET_SHJJ";
      
      public static const SOCKET_WFJ:String = "SOCKET_WFJ";
*/
// "module":794,"action":2,
// "request":[Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil],
//this._data.call(Mod_WanfaWorld_Base.login, common_login_callback, [_ctrl.wanfaWorld.loginInfo.server_name, _ctrl.player.playerId, _ctrl.wanfaWorld.loginInfo.time, _ctrl.wanfaWorld.loginInfo.pass_code], true, DataBase.SOCKET_SYS, name);
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_WanfaWorld_Base_login(const std::string& server_name, int playerId, int time, const std::string& pass_code)
{
	Json::Value data;
	data.append(server_name);
	data.append(playerId);
	data.append(time);
	data.append(pass_code);
	return this->send_and_receive(data, 794, 2);
}

//============================================================================
//登录万法界
// "module":794,"action":3,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_WanfaWorld_Base_enter_town(int town_id)
{
	Json::Value data;
	data.append(town_id);
	return this->send_and_receive(data, 794, 3);
}

//============================================================================
// 
// "module":801,"action":0,
// "request":[]
// 
// "response":[Utils.ByteUtil]
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_TombArtifacts_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 801, 0);
}

//============================================================================
// 获取诸法器冢信息
// "module":801,"action":1,
// "request":[Utils.IntUtil]
//	TombArtifactsview.as
//		[curid=1]
// TombArtifactsview.as
//		_curId = TombArtifactsForm.xun_qi;
// 
// "response":[Utils.IntUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// TombArtifactsdata.as
//		oObject.list(param1,this._objTa,["res_num","free_times","protections","buy_times","draw_times","type_id"]);
// Example
//     [ 0, 0, [ [ 2, 3, 119 ], [ 1, 1, 199 ], [ 3, 5, 19 ] ], 0, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_TombArtifacts_Base_main_panel_info(int curid)
{
	Json::Value data;
	data.append(curid);
	return this->send_and_receive(data, 801, 1);
}

//============================================================================
//	诸法器冢：取灵
// "module":801,"action":2,
// "request":[Utils.IntUtil,Utils.UByteUtil],
// 
// TombArtifactsview.as
//			_curId = TombArtifactsForm.xun_qi;
//			_loc2_ = Mod_TombArtifacts_Base.DRAW_ONE;		_loc2_:int = Mod_TombArtifacts_Base.DRAW_TEN;
//		_data.call(Mod_TombArtifacts_Base.draw,this.drawCallback,[this._curId,_loc2_]);
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil]]
//				
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_TombArtifacts_Base_draw(int curid, int type)
{
	Json::Value data;
	data.append(curid);
	data.append(type);
	return this->send_and_receive(data, 801, 2);
}


//============================================================================
//	诸法洞天：副本概况
// "module":797,"action":1,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// endlessexpeditiondata.as		
//			oObject.list(param1,this._missionPanel,["result","reg_id","mission_id","pass_flag","get_award_flag","quick_flag","left_award_num"]);
// Example
//			[ 0, 1100, 1, 0, 0, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_mission_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 797, 1);
}

//============================================================================
//	诸法洞天：副本信息
// "module":797,"action":2,
// "request":[Utils.IntUtil],
// 
// EndlessExpeditionMapView.as
//		 _data.call(Mod_EndlessExpedition_Base.mission_info,this.mission_info_call_back,[this._missionId],true,DataBase.SOCKET_SYS,CommonSocketForm.SOCKET_WFJ);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
//				
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_mission_info(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 797, 2);
}

//============================================================================
//	诸法洞天：挑战
// "module":797,"action":4,
// "request":[Utils.IntUtil],
// 
// EndlessExpeditionMapView.as
//		_data.call(Mod_EndlessExpedition_Base.challenge,this.fight_monster_callback,[this._missionId]);
// 
// "response":[Utils.UByteUtil,[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil]],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil]]
//				
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_challenge(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 797, 4);
}

//============================================================================
//	诸法洞天：前往下一个副本
// "module":797,"action":14,
// "request":[]
// 
// "response":[Utils.UByteUtil]
//				
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_enter_next_mission()
{
	Json::Value data;
	return this->send_and_receive(data, 797, 14);
}


//============================================================================
//	诸法洞天：无尽模式副本概况
// "module":797,"action":21,
// "request":[]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil]]
//		
// endlessexpeditiondata.as		
//			oObject.list(param1,this.endlessPanel,["result","reg_id","layer","pass_flag","monster_id","role_id","head_id","rim_id","rank","rank_perc","need_layer","pass_mission","rule_list"]);
// Example
//			[ 0, 1112, 1, 0, 28997, 507, 11, 8120, 0, 0, 20, 300, [ [ 10 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_endless_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 797, 21);
}

//============================================================================
//	诸法洞天：无尽模式 怪物队伍
// "module":797,"action":22,
// "request":[Utils.IntUtil]
// 
// EndlessExpeditionMapView.as
//				_data.call(Mod_EndlessExpedition_Base.get_monster_team_info,this.get_monster_team_info_callback,[this._layer]);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.IntUtil]]
//		
// endlessexpeditiondata.as		
//			this.result = param1[0];
//			this.warSpecialId = param1[1];
//			for each(_loc2_ in param1[2])
//				oObject.list(_loc2_,_loc3_,["monster_id","grid_id","stunt_id","supernatural_id","max_health","left_health","ratio"]);
// Example
//			[ 0, 9, [ [ 28989, 4, 256, 173, 5321748540, 5321748540, 0 ], [ 29001, 5, 281, 75, 5321748540, 5321748540, 0 ], [ 28992, 3, 282, 173, 5321748540, 5321748540, 0 ], [ 29000, 2, 518, 192, 5321748540, 5321748540, 0 ], [ 28997, 6, 395, 161, 5321748540, 5321748540, 0 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_get_monster_team_info(int layer_id)
{
	Json::Value data;
	data.append(layer_id);
	return this->send_and_receive(data, 797, 22);
}

//============================================================================
//	诸法洞天：无尽模式 挑战
// "module":797,"action":23,
// "request":[Utils.IntUtil]
// 
// EndlessExpeditionMapView.as
//				_data.call(Mod_EndlessExpedition_Base.endless_challenge,this.endless_challenge_callback,[this._layer]);
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// endlessexpeditiondata.as		
//			oObject.list(param1,this._missionPanel,["result","reg_id","mission_id","pass_flag","get_award_flag","quick_flag","left_award_num"]);
// Example
//			[ 0, 1100, 1, 0, 0, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_endless_challenge(int layer_id)
{
	Json::Value data;
	data.append(layer_id);
	return this->send_and_receive(data, 797, 23);
}

//============================================================================
//	诸法洞天：无尽模式 前往下一层
// "module":797,"action":30,
// "request":[]
// 
// "response":[Utils.UByteUtil]
//		
// endlessexpeditiondata.as		
//			oObject.list(param1,this._missionPanel,["result","reg_id","mission_id","pass_flag","get_award_flag","quick_flag","left_award_num"]);
// Example
//			[ 0, 1100, 1, 0, 0, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_EndlessExpedition_Base_endless_enter_next_layer()
{
	Json::Value data;
	return this->send_and_receive(data, 797, 30);
}