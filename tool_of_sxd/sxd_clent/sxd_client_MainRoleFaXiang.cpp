#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"
#include <ctime>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class MainRoleFaXiangType
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 6;
};

void sxd_client::MainRoleFaXiang()
{
	common::log("【主角法相】：开始检查！");

	//挑战守护魔兽
	MRFXExtraMo();

	//造物佛手
	MRFXExtraFo();
}

void sxd_client::MRFXExtraMo()
{
	//cd时间以时间戳计算
	//std::chrono 获取时间戳
	//auto now_time = std::chrono::system_clock::now();
	std::time_t now = time(nullptr);
	unsigned int now_time = now;

	//获取守护魔兽信息，
	Json::Value data = this->Mod_MainRoleFaXiang_Base_extra_mo_info();
	//common::log(boost::str(boost::format("\n\t\t【主角法相】：守护魔兽信息：%1%") % data));

	//cd时间
	auto next_can_fight_time = data[2].asUInt64();

	if (next_can_fight_time <= now_time)
	{
		common::log("【主角法相】：当前可挑战守护魔兽！");
		data = this->Mod_MainRoleFaXiang_Base_extra_mo_challenge();
		int result = data[0].asInt();
		if (result == MainRoleFaXiangType::SUCCESS)
		{
			common::log("【主角法相】：挑战守护魔兽成功！！！");
			return;
		}
		else
		{
			common::log(boost::str(boost::format("【主角法相】：挑战守护魔兽失败，result：%1%") % result));
			return;
		}
	}
	else
	{
		common::log("【主角法相】：cd时间，无法挑战守护魔兽×××");
		return;
	}
}

void sxd_client::MRFXExtraFo()
{
	//cd时间以时间戳计算
	//std::chrono 获取时间戳
	//auto now_time = std::chrono::system_clock::now();
	std::time_t now = time(nullptr);
	unsigned int now_time = now;


	//获取造物佛手信息，
	Json::Value data = this->Mod_MainRoleFaXiang_Base_extra_fo_info();
	//common::log(boost::str(boost::format("\n\t【主角法相】：造物佛手信息：%1%") % data));

	//cd时间
	auto refine_finish_time = data[4].asInt64();

	if (refine_finish_time == 0)
	{
		//cd时间为零
		common::log("【主角法相】：当前可祈愿！");

		if (!data[2])
		{
			//祈愿，本质是获取祈愿道具列表,
			//data[2]为祈愿道具列表,若为空，需要先祈愿一次
			data = this->Mod_MainRoleFaXiang_Base_extra_pray();
		}

		//获取祈愿信息，data[2]为祈愿道具列表
		data = this->Mod_MainRoleFaXiang_Base_extra_fo_info();

		vector<int> item_list;
		for (auto item : data[2])
		{
			item_list.push_back(item[0].asInt());
		}

		//按顺序祈愿
		vector<int> list = { 18,17,16,3,1,2,15,9,8,6,4,5,14 };

		for (auto id : list)
		{
			vector<int>::iterator i = find(item_list.begin(), item_list.end(), id);
			if (i != item_list.end())
			{
				//选择祈愿道具
				data = this->Mod_MainRoleFaXiang_Base_extra_choose_pray_award(id);
				int result = data[0].asInt();
				if(result== MainRoleFaXiangType::SUCCESS)
					common::log("【主角法相】：选择祈愿奖励成功！");
				else
					common::log(boost::str(boost::format("【主角法相】：祈愿错误，result：%1%") % result));
				break;
			}
		}
		return;
	}
	else if (refine_finish_time > now_time)
	{
		common::log("【主角法相】：CD时间，请等待祈愿时间结束！");
		return;
	}
	else if (refine_finish_time <= now_time)
	{
		common::log("【主角法相】：当前可领取祈愿奖励！");
		data = this->Mod_MainRoleFaXiang_Base_gain_pray_award();
		MRFXExtraFo();
		return;
	}
}

//============================================================================
//  主角法相
// {module:518, action:12,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil]
// Example
// MRFXExtraMoInfo.as
/*this.level = param1[_loc2_++]【0】;
this.cur_exp = param1[_loc2_++]【1】;
this.next_can_fight_time = param1[_loc2_++]【2】;
this.random_stunt_id = param1[_loc2_++]【3】;
this.random_supernatural_id = param1[_loc2_++]【4】;
this.random_special_pattern = param1[_loc2_++]【5】;*/
//     [ level, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_mo_info()
{
	Json::Value data;
	return this->send_and_receive(data, 518, 12);
}

//============================================================================
//  主角法相
// {module:518, action:16,
// request:[],
// Example		
//     [  ]
//
//"response" : [Utils.UByteUtil, Utils.IntUtil, [Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.StringUtil, [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.IntUtil, Utils.IntUtil], Utils.ByteUtil, [Utils.IntUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil]], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.IntUtil, Utils.IntUtil], Utils.ByteUtil, [Utils.IntUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil]], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.IntUtil, Utils.IntUtil], Utils.ByteUtil, [Utils.IntUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil]], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.IntUtil, Utils.IntUtil], Utils.ByteUtil, [Utils.IntUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil]], [Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil], Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.IntUtil, Utils.StringUtil, Utils.IntUtil, Utils.IntUtil, [Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, [Utils.IntUtil]], Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil], Utils.ByteUtil, [Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil]], Utils.ByteUtil, [Utils.IntUtil], Utils.IntUtil, [Utils.StringUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ByteUtil, [Utils.ShortUtil, Utils.IntUtil], [Utils.ShortUtil, Utils.ByteUtil]], Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil], [Utils.IntUtil], Utils.IntUtil, Utils.IntUtil], [[Utils.ByteUtil, Utils.StringUtil, [Utils.IntUtil, Utils.StringUtil], [Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], Utils.StringUtil], [Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil]], [Utils.ByteUtil, Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.IntUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.ShortUtil], [Utils.IntUtil, Utils.ShortUtil, [Utils.UByteUtil, Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.IntUtil], [Utils.ShortUtil], [Utils.ShortUtil], Utils.IntUtil, Utils.IntUtil, Utils.LongUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil], [Utils.IntUtil, Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil], [Utils.IntUtil, Utils.StringUtil]], [Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, [Utils.UByteUtil, Utils.ShortUtil, Utils.ByteUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.IntUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.ShortUtil], Utils.IntUtil, Utils.ShortUtil, Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.StringUtil, Utils.IntUtil, Utils.UByteUtil, [Utils.IntUtil, Utils.ShortUtil, [Utils.UByteUtil, Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.IntUtil], [Utils.ShortUtil], [Utils.ShortUtil], Utils.IntUtil, Utils.IntUtil, Utils.LongUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil], [Utils.IntUtil, Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil], [Utils.IntUtil, Utils.StringUtil], Utils.IntUtil, Utils.IntUtil, Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil], [Utils.IntUtil, Utils.StringUtil, [Utils.IntUtil, Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil]], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil, Utils.LongUtil]], Utils.ShortUtil, Utils.IntUtil, Utils.IntUtil]]
// Example
// 
// MainRoleFaXiangData.as
// 
// public function extra_mo_challenge(param1:Array) : void
//		{this.fight(param1, true);}
// 
// public function fight(param1:Array, param2:Boolean = false) : void
// this.challengeInfo.result = param1[_loc3_++];
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_mo_challenge()
{
	Json::Value data;
	return this->send_and_receive(data, 518, 16);
}

//============================================================================
//  主角法相 造物佛手信息
// {module:518, action:11,
// request:[],
// Example		
//     [  ]
//
// "response": [Utils.IntUtil, Utils.IntUtil, [Utils.IntUtil], Utils.IntUtil, Utils.IntUtil]
// Example
// MRFXExtraFoInfo.as
/*		 this.level = param1[_loc2_++];
         this.cur_exp = param1[_loc2_++];
         this.parseAwardlist(param1[_loc2_++]);
         this.curPrayAward = this.getAwardData(param1[_loc2_++]);
         this.refine_finish_time = param1[_loc2_++];*/
// [ 1, 320, [ [ 14 ], [ 9 ], [ 6 ] ], 0, 0 ] 
// 
// MainRoleFaXiangType.as
// 1,轮回石, 2,血魂石, 3,星石, 4,大经验石, 5,大经验符石, 6,高级灵石, 8,茶叶蛋, 9,境界点
// 12,黄金圣水, 14,玄铁, 15,命宫, 16,惊鸿灵宝, 17,六级魔石, 18,二品神器包
// {18,17,16,3,1,2,15,9,8,6,4,5,14}
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_fo_info()		//祈愿信息
{
	Json::Value data;
	return this->send_and_receive(data, 518, 11);
}

//============================================================================
//  主角法相
// {module:518, action:13,
// request:[],
// Example		
//     [  ]
//
//"response":[Utils.UByteUtil,[Utils.IntUtil]]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_pray()			//祈愿
{
	Json::Value data;
	return this->send_and_receive(data, 518, 13);
}

//============================================================================
//  主角法相
// {module:518, action:14,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil,[Utils.IntUtil]]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_refresh_pray()	//刷新祈愿道具
{
	Json::Value data;
	return this->send_and_receive(data, 518, 14);
}

//============================================================================
//  主角法相
// {module:518, action:15,
// request:[Utils.IntUtil]
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil,Utils.IntUtil]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_choose_pray_award(int item_id)	//选择祈愿道具
{
	Json::Value data;
	data.append(item_id);
	return this->send_and_receive(data, 518, 15);
}

//============================================================================
//  主角法相
// {module:518, action:17,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_gain_pray_award()			//收获祈愿道具
{
	Json::Value data;
	return this->send_and_receive(data, 518, 17);
}