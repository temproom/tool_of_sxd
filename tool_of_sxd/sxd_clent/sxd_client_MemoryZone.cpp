#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class MemoryZoneType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int MAX_TIMES = 2;		//回溯次数满
	static const int NO_INGOT = 3;	//元宝不足
};

void sxd_client::MemoryZone()
{
	common::log("【回忆之境】：开始检查！");
	//活动伙伴轮回
	Json::Value data = this->Mod_MemoryZone_Base_back_panel(2);
	int result = data[0].asInt();
	if (result == MemoryZoneType::SUCCESS)
	{
		int today_back_times = data[2].asInt();
		int daily_free_times = data[3].asInt();

		//今日已回溯次数>=每日免费次数，
		if (today_back_times >= daily_free_times)
		{
			common::log("【回忆之境】：[活动伙伴轮回]：今日已免费回忆！");
		}
		else
		{
			vector<int> a;
			for (Json::Value partner_list : data[1])
			{
				if (partner_list[1].asInt() != partner_list[2].asInt())
				{
					a.push_back(partner_list[0].asInt());
				}
			}
			if (a.size() != 0)
			{
				//顺序是：454:盘古，324:伏羲，366:不死凤凰，382:神农，425:唐僧，
				vector<int> role_list{ 454,324,366,382,452 };
				for (auto role_id : role_list)
				{
					vector<int>::iterator i = find(a.begin(), a.end(), role_id);
					if (i != a.end())
					{
						data = this->Mod_MemoryZone_Base_backtrack(role_id);
						result = data[0].asInt();
						if (result == MemoryZoneType::SUCCESS)
						{
							string role_name;
							if (role_id == 454)
							{
								role_name = "盘古";
							}
							else if (role_id == 324)
							{
								role_name = "伏羲";
							}
							else if (role_id == 366)
							{
								role_name = "不死凤凰";
							}
							else if (role_id == 382)
							{
								role_name = "神农";
							}
							else if (role_id == 425)
							{
								role_name = "唐僧";
							}
							common::log(boost::str(boost::format("【回忆之境】：[活动伙伴轮回]：【%1%】回忆成功！") % role_name));
						}
						else if (result == MemoryZoneType::MAX_TIMES)
						{
							common::log("【回忆之境】：[活动伙伴轮回]：回溯次数满！");
						}
						else if (result == MemoryZoneType::NO_INGOT)
						{
							common::log("【回忆之境】：[活动伙伴轮回]：元宝不足！");
						}
						else if (result == MemoryZoneType::FAILED)
						{
							common::log("【回忆之境】：[活动伙伴轮回]：回溯失败！");
						}
						break;
					}
				}				
			}			
			
		}
	}

	//魔神伙伴
	Json::Value data_mo = this->Mod_MemoryZone_Base_back_panel(3);
	int result_mo = data_mo[0].asInt();
	if (result_mo == MemoryZoneType::SUCCESS)
	{
		int today_back_times_mo = data_mo[2].asInt();
		int daily_free_times_mo = data_mo[3].asInt();

		//今日已回溯次数>=每日免费次数，
		if (today_back_times_mo >= daily_free_times_mo)
		{
			common::log("【回忆之境】：[魔神伙伴]：今日已免费回忆！");
		}
		else
		{
			vector<int> a;
			for (Json::Value partner_list_mo : data_mo[1])
			{
				if (partner_list_mo[1].asInt() != partner_list_mo[2].asInt())
				{
					a.push_back(partner_list_mo[0].asInt());
				}
			}
			if (a.size() != 0)
			{
				//顺序是：486:圣帝俊,487:圣羲和,488:圣六耳猕猴	
				vector<int> role_list_mo{ 486,487,488 };
				for (auto role_id : role_list_mo)
				{
					vector<int>::iterator i = find(a.begin(), a.end(), role_id);
					if (i != a.end())
					{
						data_mo = this->Mod_MemoryZone_Base_backtrack(role_id);
						result_mo = data_mo[0].asInt();
						if (result_mo == MemoryZoneType::SUCCESS)
						{
							string role_name;
							if (role_id == 486)
							{
								role_name = "圣帝俊";
							}
							else if (role_id == 487)
							{
								role_name = "圣羲和";
							}
							else if (role_id == 488)
							{
								role_name = "圣六耳猕猴";
							}
							common::log(boost::str(boost::format("【回忆之境】：[魔神伙伴]：【%1%】回忆成功！") % role_name));						
						}
						else if (result_mo == MemoryZoneType::MAX_TIMES)
						{
							common::log("【回忆之境】：[魔神伙伴]：回溯次数满！");
						}
						else if (result_mo == MemoryZoneType::NO_INGOT)
						{
							common::log("【回忆之境】：[魔神伙伴]：元宝不足！");
						}
						else if (result_mo == MemoryZoneType::FAILED)
						{
							common::log("【回忆之境】：[魔神伙伴]：回溯失败！");
						}
						break;
					}
				}
			}

		}
	}
}

//============================================================================
//  回忆之境信息
// {module:526, action:2,
// request:[Utils.IntUtil],
// Example	
// MemoryZoneView.as	
//  if(this.info.curType == 1 || this.info.curType == 2 || this.info.curType == 3)
//{
//	_data.call(Mod_MemoryZone_Base.back_panel, param1, [this.info.curType]);
//}
//     1：活动伙伴, 2：活动伙伴轮回, 3：魔神伙伴,
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
// Example
// MemoryZoneData.as
// var _loc2_:int = 0;
/*this.info.result = param1[_loc2_++];
if (this.info.result == Mod_MemoryZone_Base.SUCCESS)
{
    this.info.partner_list = this.parsePartnerList(param1[_loc2_++]);
    this.info.curRole = null;
    this.info.today_back_times = param1[_loc2_++];
    this.info.daily_free_times = param1[_loc2_++];
    this.info.daily_ingot_times = param1[_loc2_++];
    this.info.cost_ingot = param1[_loc2_++];
}*/
//	partner_list：(_loc4_ = DataUtil.getRoleData(_loc3_[0],true,true)).have_scrap = _loc3_[1];
//					_loc4_.need_scrap = _loc3_[2];
// [角色id，现有碎片，总所需碎片]
//     [ result, partner_list, today_back_times, daily_free_times, daily_ingot_times, cost_ingot]
//============================================================================
Json::Value sxd_client::Mod_MemoryZone_Base_back_panel(int curtype)
{
	Json::Value data;
	data.append(curtype);
	return this->send_and_receive(data, 526, 2);
}

//============================================================================
//  进行回忆
// {module:526, action:4,
// request:[Utils.IntUtil],
// Example		
// MemoryZoneView.as
//             _data.call(Mod_MemoryZone_Base.backtrack,backTrackBack,[roleId]);
//     [  ]
// 324:伏羲,366:不死凤凰,382:神农,425:唐僧,454:盘古
// 486:圣帝俊,487:圣羲和,488:圣六耳猕猴		
//
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
// MemoryZoneData.as
// this.info.result = param1[_loc2_++];
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MemoryZone_Base_backtrack(int role_id)
{
	Json::Value data;
	data.append(role_id);
	return this->send_and_receive(data, 526, 4);
}