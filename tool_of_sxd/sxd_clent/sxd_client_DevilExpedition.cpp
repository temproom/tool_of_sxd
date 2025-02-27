#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class DevilExpeditionType
{
public:
	static const int SUCCESS = 2;
	static const int FAILED = 3;
	static const int M5_FIGHT_MONSTER = 49;
	static const int M6_FIGHT_MONSTER = 50;
	static const int M7_FIGHT_MONSTER = 51;
	static const int MISSION_MONSTER_GOD = 67;
	static const int MISSION_YINYANG_HALL = 68;
	static const int MISSION_HERMIT_HOME = 69;
};

void sxd_client::DevilExpedition()
{
	//std::cout << boost::str(boost::format("【魔界远征】：选择进入第几关\n") );
	//common::log(boost::str(boost::format("\n\t【魔界远征】：选择进入第几关:") ));
	common::log("【魔界远征】：选择进入第几关\n\t\t1:第一关（不可用）\n\t\t2:第二关（不可用）\n\t\t3:第三关（不可用）\n\t\t4:挑战魔神伙伴\n\t\t5:挑战阴阳伙伴\n\t\t6:挑战隐仙伙伴\t\t请选择进入第几关：");
	//common::log("\n\t请选择进入第几关：");
	int floor;
	std::cin >> floor;
	Json::Value data;
	int result;
	switch (floor)
	{
	case 1:
		data = this->Mod_DevilExpedition_Base_enter_town(86);
		result = data[0].asInt();
		if (result == DevilExpeditionType::SUCCESS)
		{
			//成功
			common::log("【魔界远征】：进入副本成功！");
			//获取副本信息
			data = this->Mod_DevilExpedition_Base_get_mission_data_m1();
			common::log(boost::str(boost::format("【魔界远征】：副本信息：[%1%]\n") % data));
		}
		else
		{
			common::log(boost::str(boost::format("【魔界远征】：进入第一关副本失败，错误代码：[%1%]\n") % result));
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		data = this->Mod_DevilExpedition_Base_enter_town(91);
		result = data[0].asInt();
		if (result == DevilExpeditionType::SUCCESS)
		{
			//成功
			common::log("【魔界远征】：进入副本成功！");
			//获取副本信息
			//data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
			//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
			bool term = true;
			int type = 1;
			common::log("【魔界远征】： \n\t 1.帝俊\n\t 2.羲和\n\t 3.六耳\n\t 请选择相应的功能：");
			std::cin >> type;
			while (term)
			{
				data = this->Mod_DevilExpedition_Base_fight_monster_m5(type);
				//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("【魔界远征】：完成所有挑战！");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("【魔界远征】：进入副本失败，错误代码：[%1%]\n") % result));
		}
		break;
	case 5:
		//进入阴阳副本
		data = this->Mod_DevilExpedition_Base_enter_town(144);
		result = data[0].asInt();
		if (result == DevilExpeditionType::SUCCESS)
		{
			//成功
			common::log("【魔界远征】：进入副本成功！");
			//获取副本信息
			data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
			//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
			bool term = true;
			while (term)
			{
				data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
				//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("【魔界远征】：完成所有挑战！");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("【魔界远征】：进入阴阳副本失败，错误代码：[%1%]\n") % result));
		}
		break;
	case 6:
		//进入隐仙副本
		data = this->Mod_DevilExpedition_Base_enter_town(154);
		result = data[0].asInt();
		if(result == DevilExpeditionType::SUCCESS)
		{
			//成功
			common::log("【魔界远征】：进入副本成功！");
			//获取副本信息
			data = this->Mod_DevilExpedition_Base_get_mission_data_m7();
			//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
			bool term = true;
			while (term)
			{								
				data = this->Mod_DevilExpedition_Base_fight_monster_m7(1);
				//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("【魔界远征】：完成所有挑战！");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("【魔界远征】：进入隐仙副本失败，错误代码：[%1%]\n") % result));
		}
		break;
	}		
}

void sxd_client::Single_DevilExpedition()
{
	common::log("【魔界远征】：选择单人挑战第几关\n\t1:隐仙\n\t2:阴阳\n\t3:魔神\n\t请选择进入第几关：");
	int floor;
	int via;
	std::cin >> via;
	Json::Value data;
	int type = 1;		//魔神挑战，选择魔灵类型，默认帝俊
	floor = DevilExpeditionType::MISSION_HERMIT_HOME;			//默认挑战隐仙
	int mission_id = 154;

	if (via == 2)
	{
		//阴阳
		floor = DevilExpeditionType::MISSION_YINYANG_HALL;
		mission_id = 144;
	}	
	else if (via == 3)
	{
		//魔神
		floor = DevilExpeditionType::MISSION_MONSTER_GOD;
		mission_id = 91;;
		common::log(" 【魔界远征】：选择挑战魔神\n\t 1.帝俊\n\t 2.羲和\n\t 3.六耳\n\t 请选择相应的功能：");
		std::cin >> type;
	}

	data = this->Mod_DevilExpedition_Base_start_expedition_single(floor);

	//data = this->Mod_DevilExpedition_Base_enter_town(mission_id);
	int result = data[0].asInt();
	if (result == DevilExpeditionType::SUCCESS)
	{
		//成功
		common::log("【魔界远征】：进入副本成功！");

		data = this->Mod_DevilExpedition_Base_reject_buy_box();

		//获取副本信息
		if (via == 1)
		{
			//隐仙
			data = this->Mod_DevilExpedition_Base_get_mission_data_m7();
		}
		else if (via == 2)
		{
			//阴阳
			data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
		}	
		//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
		bool term = true;
		while (term)
		{
			if (via == 1)
			{
				//隐仙
				data = this->Mod_DevilExpedition_Base_fight_monster_m7(1);
			}
			else if (via == 2)
			{
				//阴阳
				data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
			}
			else if (via == 3)
			{
				//魔神
				data = this->Mod_DevilExpedition_Base_fight_monster_m5(type);
			}
			//data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
			//common::log(boost::str(boost::format("\n\t【魔界远征】：副本信息：[%1%]\n") % data));
			int end = data[0].asInt();
			if (end != DevilExpeditionType::SUCCESS)
			{
				term = false;
				data = this->Mod_DevilExpedition_Base_quit();
				common::log("【魔界远征】：完成所有挑战！");
			}
		}
	}
}

//86:["MoJieYuanZheng1","魔界远征1",{},5010,"魔界远征第一关城镇",0,0],
//87: ["MoJieYuanZheng2", "魔界远征2", {}, 5020, "魔界远征第二关城镇", 0, 0] ,
//88 : ["MoJieYuanZheng3", "魔界远征3", {}, 5030, "魔界远征第三关城镇", 0, 0] ,
//89 : ["MoWangDian", "魔王殿", {}, 5040, "魔界远征第四关城镇", 0, 0] ,
//91:["ShenYuanFuBen","深渊副本",{},5050,"魔界远征第五关城镇",0,0] 魔神
//144:["ShenYuanFuBen2","深渊副本2",{},5060,"魔界远征第五关城镇",0,0], 阴阳
//154: ["ShenYuanFuBen3", "深渊副本3", {}, 5070, "魔界远征第五关城镇", 0, 0] 隐仙
//============================================================================
//  进入魔界远征信息
// {module:429, action:13,
// request:[Utils.IntUtil],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil]
// 
// Example
// DevilExpeditionData.as
//     [ enterResult,is_weekend_revelry_discount]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_enter_town(int town_id)
{
	Json::Value data;
	data.append(town_id);
	return this->send_and_receive(data, 429, 13);
}

//============================================================================
//  离开魔界远征城镇
// {module:429, action:15,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil]
// 
// Example
// DevilExpeditionData.as
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_leave_town()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 15);
}

//============================================================================
//  退出魔界远征
// {module:429, action:29,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,Utils.IntUtil]
// 
// Example
// DevilExpeditionData.as
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_quit()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 29);
}

//============================================================================
//  获取隐仙副本信息 第一关
// {module:429, action:24,
// request:[],
// Example		
//     [  ]
// response:[]
// 
// Example
// DevilExpeditionData.as
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_get_mission_data_m1()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 24);
}

//============================================================================
//  获取隐仙副本信息 隐仙
// {module:429, action:74,
// request:[],
// Example		
//     [  ]
// response:[]
// 
// Example
// DevilExpeditionData.as
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_get_mission_data_m7()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 74);
}

//============================================================================
//  战斗 隐仙
// {module:429, action:75,
// request:[Utils.IntUtil],
// Example		
//     [ monster_id ]
// response:[]
// Example
// DevilExpeditionData.as
// this.stageUIInfo.warResult = param1[_loc2_++];
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_fight_monster_m7(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 429, 75);
}

//============================================================================
//  获取隐仙副本信息 隐仙
// {module:429, action:76,
// request:[],
// Example		
//     [  ]
// response:[]
// Example
// DevilExpeditionData.as
// this.myRecordInfo.result = param1[_loc2_++];
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_notify_mission_data_change_m7()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 76);
}

//============================================================================
//  获取隐仙副本奖励
// {module:429, action:77,
// request:[],
// Example		
//     [  ]
// response:[]
// Example
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_get_fight_hermit_record()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 77);
}

//============================================================================
//  获取隐仙副本信息 阴阳
// {module:429, action:68,
// request:[],
// Example		
//     [  ]
// response:[]
// 
// Example
// DevilExpeditionData.as
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_get_mission_data_m6()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 68);
}

//============================================================================
//  战斗 阴阳
// {module:429, action:69,
// request:[Utils.IntUtil],
// Example		
//     [ monster_id ]
// response:[]
// Example
// DevilExpeditionData.as
// this.stageUIInfo.warResult = param1[_loc2_++];
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_fight_monster_m6(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 429, 69);
}

//============================================================================
//  战斗 魔神
// {module:429, action:58,
// request:[Utils.IntUtil],
// Example		
//     [ monster_id ]
// response:[]
// Example
// DevilExpeditionData.as
// this.stageUIInfo.warResult = param1[_loc2_++];
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_fight_monster_m5(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 429, 58);
}
//============================================================================
//  单人快速挑战，
// {module:429, action:63,
// request:[Utils.UByteUtil],
// Example
// DevilExpeditionSignInView.as 	188	
/*var _loc3_:int = Mod_DevilExpedition_Base.MISSION_YINYANG_HALL;
if (param2[0] == 2)
{
	_loc3_ = Mod_DevilExpedition_Base.MISSION_MONSTER_GOD;
}
else if (param2[0] == 3)
{
	_loc3_ = Mod_DevilExpedition_Base.MISSION_HERMIT_HOME;
}*/
//     _data.call(Mod_DevilExpedition_Base.start_expedition_single,onSingleExpeditionBack,[_loc3_]);
// response:[Utils.UByteUtil]
// 
// Example
// DevilExpeditionData.as
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_start_expedition_single(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 429, 63);
}


//============================================================================
//  拒绝元宝买找回宝箱
// {module:429, action:66,
// request:[Utils.IntUtil],
// Example		
//     []
// response:[]
// Example
//============================================================================
Json::Value sxd_client::Mod_DevilExpedition_Base_reject_buy_box()
{
	Json::Value data;
	return this->send_and_receive(data, 429, 66);
}
