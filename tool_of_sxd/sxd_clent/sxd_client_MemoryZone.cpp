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
	static const int MAX_TIMES = 2;		//���ݴ�����
	static const int NO_INGOT = 3;	//Ԫ������
};

void sxd_client::MemoryZone()
{
	common::log("������֮��������ʼ��飡");
	//�����ֻ�
	Json::Value data = this->Mod_MemoryZone_Base_back_panel(2);
	int result = data[0].asInt();
	if (result == MemoryZoneType::SUCCESS)
	{
		int today_back_times = data[2].asInt();
		int daily_free_times = data[3].asInt();

		//�����ѻ��ݴ���>=ÿ����Ѵ�����
		if (today_back_times >= daily_free_times)
		{
			common::log("������֮������[�����ֻ�]����������ѻ��䣡");
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
				//˳���ǣ�454:�̹ţ�324:���ˣ�366:������ˣ�382:��ũ��425:��ɮ��
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
								role_name = "�̹�";
							}
							else if (role_id == 324)
							{
								role_name = "����";
							}
							else if (role_id == 366)
							{
								role_name = "�������";
							}
							else if (role_id == 382)
							{
								role_name = "��ũ";
							}
							else if (role_id == 425)
							{
								role_name = "��ɮ";
							}
							common::log(boost::str(boost::format("������֮������[�����ֻ�]����%1%������ɹ���") % role_name));
						}
						else if (result == MemoryZoneType::MAX_TIMES)
						{
							common::log("������֮������[�����ֻ�]�����ݴ�������");
						}
						else if (result == MemoryZoneType::NO_INGOT)
						{
							common::log("������֮������[�����ֻ�]��Ԫ�����㣡");
						}
						else if (result == MemoryZoneType::FAILED)
						{
							common::log("������֮������[�����ֻ�]������ʧ�ܣ�");
						}
						break;
					}
				}				
			}			
			
		}
	}

	//ħ����
	Json::Value data_mo = this->Mod_MemoryZone_Base_back_panel(3);
	int result_mo = data_mo[0].asInt();
	if (result_mo == MemoryZoneType::SUCCESS)
	{
		int today_back_times_mo = data_mo[2].asInt();
		int daily_free_times_mo = data_mo[3].asInt();

		//�����ѻ��ݴ���>=ÿ����Ѵ�����
		if (today_back_times_mo >= daily_free_times_mo)
		{
			common::log("������֮������[ħ����]����������ѻ��䣡");
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
				//˳���ǣ�486:ʥ�ۿ�,487:ʥ�˺�,488:ʥ����⨺�	
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
								role_name = "ʥ�ۿ�";
							}
							else if (role_id == 487)
							{
								role_name = "ʥ�˺�";
							}
							else if (role_id == 488)
							{
								role_name = "ʥ����⨺�";
							}
							common::log(boost::str(boost::format("������֮������[ħ����]����%1%������ɹ���") % role_name));						
						}
						else if (result_mo == MemoryZoneType::MAX_TIMES)
						{
							common::log("������֮������[ħ����]�����ݴ�������");
						}
						else if (result_mo == MemoryZoneType::NO_INGOT)
						{
							common::log("������֮������[ħ����]��Ԫ�����㣡");
						}
						else if (result_mo == MemoryZoneType::FAILED)
						{
							common::log("������֮������[ħ����]������ʧ�ܣ�");
						}
						break;
					}
				}
			}

		}
	}
}

//============================================================================
//  ����֮����Ϣ
// {module:526, action:2,
// request:[Utils.IntUtil],
// Example	
// MemoryZoneView.as	
//  if(this.info.curType == 1 || this.info.curType == 2 || this.info.curType == 3)
//{
//	_data.call(Mod_MemoryZone_Base.back_panel, param1, [this.info.curType]);
//}
//     1������, 2�������ֻ�, 3��ħ����,
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
//	partner_list��(_loc4_ = DataUtil.getRoleData(_loc3_[0],true,true)).have_scrap = _loc3_[1];
//					_loc4_.need_scrap = _loc3_[2];
// [��ɫid��������Ƭ����������Ƭ]
//     [ result, partner_list, today_back_times, daily_free_times, daily_ingot_times, cost_ingot]
//============================================================================
Json::Value sxd_client::Mod_MemoryZone_Base_back_panel(int curtype)
{
	Json::Value data;
	data.append(curtype);
	return this->send_and_receive(data, 526, 2);
}

//============================================================================
//  ���л���
// {module:526, action:4,
// request:[Utils.IntUtil],
// Example		
// MemoryZoneView.as
//             _data.call(Mod_MemoryZone_Base.backtrack,backTrackBack,[roleId]);
//     [  ]
// 324:����,366:�������,382:��ũ,425:��ɮ,454:�̹�
// 486:ʥ�ۿ�,487:ʥ�˺�,488:ʥ����⨺�		
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