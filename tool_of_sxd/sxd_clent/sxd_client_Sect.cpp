#include "stdafx.h"
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class SectLoginType
{
	//宗门仙境登录
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
};
class SectType
{
public:
	static const int SECT_NOT_OPEN = 0;
	static const int SECT_OPEN = 1;
	static const int SUCCESS = 2;
};
class SectBonusType	
{
	//赏金堂
public:
	static const int SUCCESS = 0;
	static const int FAILED = 2;
	static const int LESS_TIMES = 3;
};
class NewSectShopType
{
	//商店
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int LOW_CONTRIBUTE = 2;
	static const int MAX_BUY_TIMES = 6;
};
class SectMonsterType
{
	//宗门灵兽
public:
	static const int SUCCESS = 0;
	static const int HAS_PUSHED = 1;
	static const int FAILED = 4;
	static const int FEED_NUM_LIMIT = 5;
};

int sxd_client::login_Sect_area(sxd_client* sxd_client_town)
{

	// 1. get group
	Json::Value data = sxd_client_town->Mod_Sect_Base_panel_info();

	int group = data[9].asInt();
	int sect_id = data[6].asInt();

	this->user_id = sxd_client_town->user_id;

	// 2. get login information: [ appId, playerId, serverName, node, time, passCode]
	data = sxd_client_town->Mod_SectLogin_Base_get_login_info(group);

	std::string host = data[1].asString();
	std::string port = data[2].asString();
	int appid = data[3].asInt();
	std::string node = data[4].asString();
	std::string server_name = data[5].asString();
	int time = data[6].asInt();
	std::string pass_code = data[7].asString();

	if (host.size() == 0)
	{
		common::log("【宗门仙境】玩家未开启宗门功能", iEdit);
		return 2;
	}

	// 3. connect
	this->connect(host, port);
	common::log(boost::str(boost::format("【宗门仙境】连接服务器 [%1%:%2%] 成功") % host % port), iEdit);

	// 4. login
	data = this->Mod_SectLogin_Base_login(appid, sxd_client_town->player_id, server_name, node, time, pass_code);

	if (data[0].asInt() != SectLoginType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门仙境】登录失败，result[%1%]") % data[0]), iEdit);
		return 3;
	}

	common::log("【宗门仙境】登录宗门仙境成功！！", iEdit);

	// 5. enter town
	data = this->Mod_SectLogin_Base_enter_town(148, sect_id);
	if (data[0].asInt() != SectLoginType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门仙境】玩家进入 [宗门仙境] 失败，result[%1%]") % data[0]), iEdit);
		return 4;
	}
	common::log("【宗门仙境】玩家进入 [宗门仙境]", iEdit);

	bLogin = 1;
	
	return 0;
}


void sxd_client::Sect()
{
	common::log("【宗门】：开始宗门任务！！");

	//获取宗门信息
	Json::Value data = this->Mod_Sect_Base_panel_info();

	/*int result = data[0].asInt();
	if (result != SectType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门】：获取宗门信息失败！！代码：%1%\n") % result));
		return;
	}*/

	//宗门id
	int sect_id = data[6].asInt();

	//data = this->Mod_SectLogin_Base_enter_town(148, 1);

	//宗门赏金堂
	this->SectBonus(1);

	//宗门礼包
	this->NewSectShop();

	//宗门灵兽
	this->SectMonster(1);
}

void sxd_client::SectBonus(int sect_id)
{
	//获取赏金堂信息
	Json::Value data = this->Mod_SectBonus_Base_open_panel(sect_id);
	
	int result = data[0].asInt();
	if (result != SectBonusType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门赏金堂】：获取宗门赏金堂失败！！代码：%1%\n") % result));
		return;
	}
	
	//剩余获取礼物次数
	int left_get_times = data[2].asInt();

	if (left_get_times == 0)
	{
		common::log("【宗门赏金堂】：无次数！！");
		return;
	}

	while (left_get_times > 0)
	{
		data = this->Mod_SectBonus_Base_get_gift(sect_id);

		result = data[0].asInt();;
		if (result != SectBonusType::SUCCESS)
		{
			common::log(boost::str(boost::format("【宗门赏金堂】：获取宗门赏金堂失败！！代码：%1%\n") % result));
			return;
		}
		else 
		{
			common::log("【宗门赏金堂】：获取礼物成功！！");
			left_get_times--;
		}
	}
}

void sxd_client::NewSectShop()
{
	//获取商店信息
	Json::Value data = this->Mod_NewSectShop_Base_gift_shop_panel();

	int result = data[0].asInt();
	if (result != NewSectShopType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门商店】：获取宗门商店信息失败！！代码：%1%\n") % result));
		return;
	}

	int contribute_val = data[1].asInt();
	if (contribute_val < 600)
	{
		common::log("【宗门商店】：贡献不足，无法领取礼包！！");
		return;
	}
	else
	{
		data = this->Mod_NewSectShop_Base_buy_gift_shop_item(1);

		int result = data[0].asInt();
		if (result == NewSectShopType::MAX_BUY_TIMES)
		{
			common::log("【宗门商店】：本周已领取！！");
			return;
		}
		else if (result != NewSectShopType::SUCCESS)
		{
			common::log(boost::str(boost::format("【宗门商店】：领取宗门礼包失败！！代码：%1%\n") % result));
			return;
		}		
		else
		{
			common::log("【宗门商店】：领取礼包成功！！");
			return;
		}
	}
}

void sxd_client::SectMonster(int sect_id)
{
	//获取灵兽信息
	Json::Value data = this->Mod_SectMonster_Base_get_monster_list(sect_id);

	/*int result = data[0].asInt();
	if (result != SectMonsterType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门灵兽】：获取灵兽信息失败！！代码：%1%\n") % result));
		return;
	}*/

	int free_feed_num = data[1].asInt();
	if (free_feed_num > 0)
	{
		common::log("【宗门灵兽】：免费喂养次数已用完！！");
		return;
	}
	
	data = this->Mod_SectMonster_Base_feed(1, 1);

	int result = data[0].asInt();
	if (result != SectMonsterType::SUCCESS)
	{
		common::log(boost::str(boost::format("【宗门灵兽】：喂养失败！！代码：%1%\n") % result));
		return;
	}
	else
	{
		common::log("【宗门灵兽】：免费喂养成功！！");
		return;
	}

}

//============================================================================
//  获取宗门仙境信息
// {module:575, action:1,
// request:[Utils.IntUtil],
// Example	
//		view.as
//			_data.call(Mod_SectLogin_Base.get_login_info,this.getSectLoginInfoCallback,[SectLoginInfo.currentGroup]);
// 也就是 Mod_Sect_Base_panel_info()的my_sect_group
// [sect_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil]
// Example
// 
// SectLoginData.as L43
//		oObject.list(param1,_loc2_,["result","host","port","appId","node","serverName","time","passCode","group"]);
//============================================================================
Json::Value sxd_client::Mod_SectLogin_Base_get_login_info(int group)
{
	Json::Value data;
	data.append(group);
	return this->send_and_receive(data, 575, 1);
}

//============================================================================
//  登录宗门仙境
// {module:575, action:2,
// request:[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil],
// Example	
//		groupinfo.as
/*		public var host:String = "127.0.0.1";
      public var port:int = 8888;      
      public var appId:int = 0;      
      public var node:String = "";      
      public var serverName:String = "trunk";      
      public var time:int = 0;     
      public var passCode:String = "abcdefghijlklmn";      
      public var group:int = 0;   
      public var sectId:int = 0;*/
//		view.as
//			this._data.call(Mod_SectLogin_Base.login,common_login_callback,[groupInfo.appId,this.ctrl.player.playerId,groupInfo.serverName,groupInfo.node,groupInfo.time,groupInfo.passCode],true,DataBase.SOCKET_SYS,name);
//     [ appId, playerId, serverName, node, time, passCode]
// 
// "response":[Utils.UByteUtil]
// Example
// 
// SectLoginData.as L43
//		this._result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_SectLogin_Base_login(int appId, int playerId, const std::string& serverName, const std::string& node, int time, const std::string& passCode)
{
	Json::Value data;
	data.append(appId);
	data.append(playerId);
	data.append(serverName);
	data.append(node);
	data.append(time);
	data.append(passCode);
	return this->send_and_receive(data, 575, 2);
}

//============================================================================
//  进入宗门地图
// {module:575, action:3,
// request:[Utils.IntUtil,Utils.IntUtil],
// Example	
// SectTownMapView.as 
//		_data.call(Mod_SectLogin_Base.enter_town,this.enter_town_back,[_id,SectLoginInfo.currentSectId],true,DataBase.SOCKET_SYS,this._groupInfo.socketName);
//		id=宗门地图
//		
// "response":[Utils.UByteUtil]
// Example
// this._result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_SectLogin_Base_enter_town(int id, int sect_id)
{
	Json::Value data;
	data.append(id);
	data.append(sect_id);
	return this->send_and_receive(data, 575, 3);
}

Json::Value sxd_client::Mod_SectLogin_Base_get_player_sect_info()
{
	Json::Value data;
	return this->send_and_receive(data, 575, 10);
}

 //============================================================================
//  获取宗门信息
// {module:571, action:0,
// request:[],
// Example		
//     [  ]
// 
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil]
// Example
//			[ 12, 1, 1, 1, 0, 2, 44732, 2, 0 ] 
// 
// SectData.as L41
//		["begin_create_time","end_create_time","begin_vote_time","end_vote_time","sect_location_list","my_appid","my_sect_id","god_rank_job","salary","my_sect_group","my_sect_lv","my_sect_name"]);
//		 _loc2_ = this.parseObject(_loc3_,["sect_location_id","sect_id","sect_name","leader_name","vote_appid","apply_list","group","sect_lv"]);
//============================================================================
Json::Value sxd_client::Mod_Sect_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 571, 0);
}

Json::Value sxd_client::Mod_Sect_Base_sect_list()
{
	Json::Value data;
	return this->send_and_receive(data, 571, 1);
}

//============================================================================
//  获取赏金堂信息
// {module:591, action:1,
// request:[Utils.IntUtil],
// Example	
// [sect_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
// 
// SectBonusData.as L43
//		["result","gift_list","left_get_times","today_self_cont","sect_cont_limit","sect_cont"]);
//============================================================================
Json::Value sxd_client::Mod_SectBonus_Base_open_panel(int sect_id)
{
	Json::Value data;
	data.append(sect_id);
	return this->send_and_receive(data, 591, 1);
}

//============================================================================
//  收取赏金堂礼物
// {module:591, action:3,
// request:[Utils.IntUtil],
// Example	
// [sect_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil]
// Example
// 
// SectBonusData L107
//		["result","record_list"])
//============================================================================
Json::Value sxd_client::Mod_SectBonus_Base_get_gift(int sect_id)
{
	Json::Value data;
	data.append(sect_id);
	return this->send_and_receive(data, 591, 3);
}

//============================================================================
//  宗门商店信息
// {module:765, action:,
// request:[],
// Example	
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// 
// NewSectShopData L35
//		["result","contribute_val","charge_ingot","shop_item_list"]);
//============================================================================
Json::Value sxd_client::Mod_NewSectShop_Base_gift_shop_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 765, 1);
}

//============================================================================
//  宗门商店信息
// {module:765, action:2
// request:[Utils.IntUtil]
// Example	
// [item_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil,Utils.IntUtil]
// Example
// 
// NewSectShopData L102
//		this.result = param1[0]
//============================================================================
Json::Value sxd_client::Mod_NewSectShop_Base_buy_gift_shop_item(int item_id)
{
	Json::Value data;
	data.append(item_id);
	return this->send_and_receive(data, 765, 2);
}

//============================================================================
//  宗门灵兽
// {module:576, action:0
// request:[Utils.IntUtil]
// Example	
// [item_id	]
//     [  ]
// 
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
// 
// SectMonsterData L120
//		["grow_value","push_soul","new_monster_type","new_monster_quality","job_id","old_monster_quality"]);
//============================================================================
Json::Value sxd_client::Mod_SectMonster_Base_egg_panel_info(int sect_id)
{
	Json::Value data;
	data.append(sect_id);
	return this->send_and_receive(data, 576, 0);
}

//============================================================================
//  宗门灵兽
// {module:576, action:1
// request:[Utils.IntUtil]
// Example	
// [item_id	]
//     [  ]
// 
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// Example
// 
// SectMonsterData L140
//		["monster_list","free_feed_num","high_feed_num","job_id","week_add_monster_exp","common_monster_list"]);
//		monster_list:["monster_type","level","quality","grow_value"]);
//============================================================================
Json::Value sxd_client::Mod_SectMonster_Base_get_monster_list(int sect_id)
{
	Json::Value data;
	data.append(sect_id);
	return this->send_and_receive(data, 576, 1);
}

//============================================================================
//  宗门灵兽
// {module:576, action:2
// request:
// Example	
// [item_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil]
// Example
// 
// SectMonsterData 
//		this.result = param1[0]
//============================================================================
Json::Value sxd_client::Mod_SectMonster_Base_push_soul()
{
	Json::Value data;
	return this->send_and_receive(data, 576, 2);
}

//============================================================================
//  宗门灵兽
// {module:576, action:3
// request:[Utils.IntUtil,Utils.IntUtil],
// Example	
// [item_id	]
//     [  ]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// 
// SectMonsterData 
//		this.result = param1[0]
//============================================================================
Json::Value sxd_client::Mod_SectMonster_Base_feed(int monster_id, int type)
{
	Json::Value data;
	data.append(monster_id);
	data.append(type);
	return this->send_and_receive(data, 576, 3);
}