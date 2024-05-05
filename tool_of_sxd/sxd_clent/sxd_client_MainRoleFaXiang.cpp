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
	common::log("�����Ƿ��ࡿ����ʼ��飡");

	//��ս�ػ�ħ��
	MRFXExtraMo();

	//�������
	MRFXExtraFo();
}

void sxd_client::MRFXExtraMo()
{
	//cdʱ����ʱ�������
	//std::chrono ��ȡʱ���
	//auto now_time = std::chrono::system_clock::now();
	std::time_t now = time(nullptr);
	unsigned int now_time = now;

	//��ȡ�ػ�ħ����Ϣ��
	Json::Value data = this->Mod_MainRoleFaXiang_Base_extra_mo_info();
	//common::log(boost::str(boost::format("\n\t\t�����Ƿ��ࡿ���ػ�ħ����Ϣ��%1%") % data));

	//cdʱ��
	auto next_can_fight_time = data[2].asUInt64();

	if (next_can_fight_time <= now_time)
	{
		common::log("�����Ƿ��ࡿ����ǰ����ս�ػ�ħ�ޣ�");
		data = this->Mod_MainRoleFaXiang_Base_extra_mo_challenge();
		int result = data[0].asInt();
		if (result == MainRoleFaXiangType::SUCCESS)
		{
			common::log("�����Ƿ��ࡿ����ս�ػ�ħ�޳ɹ�������");
			return;
		}
		else
		{
			common::log(boost::str(boost::format("�����Ƿ��ࡿ����ս�ػ�ħ��ʧ�ܣ�result��%1%") % result));
			return;
		}
	}
	else
	{
		common::log("�����Ƿ��ࡿ��cdʱ�䣬�޷���ս�ػ�ħ�ޡ�����");
		return;
	}
}

void sxd_client::MRFXExtraFo()
{
	//cdʱ����ʱ�������
	//std::chrono ��ȡʱ���
	//auto now_time = std::chrono::system_clock::now();
	std::time_t now = time(nullptr);
	unsigned int now_time = now;


	//��ȡ���������Ϣ��
	Json::Value data = this->Mod_MainRoleFaXiang_Base_extra_fo_info();
	//common::log(boost::str(boost::format("\n\t�����Ƿ��ࡿ�����������Ϣ��%1%") % data));

	//cdʱ��
	auto refine_finish_time = data[4].asInt64();

	if (refine_finish_time == 0)
	{
		//cdʱ��Ϊ��
		common::log("�����Ƿ��ࡿ����ǰ����Ը��");

		if (!data[2])
		{
			//��Ը�������ǻ�ȡ��Ը�����б�,
			//data[2]Ϊ��Ը�����б�,��Ϊ�գ���Ҫ����Ըһ��
			data = this->Mod_MainRoleFaXiang_Base_extra_pray();
		}

		//��ȡ��Ը��Ϣ��data[2]Ϊ��Ը�����б�
		data = this->Mod_MainRoleFaXiang_Base_extra_fo_info();

		vector<int> item_list;
		for (auto item : data[2])
		{
			item_list.push_back(item[0].asInt());
		}

		//��˳����Ը
		vector<int> list = { 18,17,16,3,1,2,15,9,8,6,4,5,14 };

		for (auto id : list)
		{
			vector<int>::iterator i = find(item_list.begin(), item_list.end(), id);
			if (i != item_list.end())
			{
				//ѡ����Ը����
				data = this->Mod_MainRoleFaXiang_Base_extra_choose_pray_award(id);
				int result = data[0].asInt();
				if(result== MainRoleFaXiangType::SUCCESS)
					common::log("�����Ƿ��ࡿ��ѡ����Ը�����ɹ���");
				else
					common::log(boost::str(boost::format("�����Ƿ��ࡿ����Ը����result��%1%") % result));
				break;
			}
		}
		return;
	}
	else if (refine_finish_time > now_time)
	{
		common::log("�����Ƿ��ࡿ��CDʱ�䣬��ȴ���Ըʱ�������");
		return;
	}
	else if (refine_finish_time <= now_time)
	{
		common::log("�����Ƿ��ࡿ����ǰ����ȡ��Ը������");
		data = this->Mod_MainRoleFaXiang_Base_gain_pray_award();
		MRFXExtraFo();
		return;
	}
}

//============================================================================
//  ���Ƿ���
// {module:518, action:12,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil]
// Example
// MRFXExtraMoInfo.as
/*this.level = param1[_loc2_++]��0��;
this.cur_exp = param1[_loc2_++]��1��;
this.next_can_fight_time = param1[_loc2_++]��2��;
this.random_stunt_id = param1[_loc2_++]��3��;
this.random_supernatural_id = param1[_loc2_++]��4��;
this.random_special_pattern = param1[_loc2_++]��5��;*/
//     [ level, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_mo_info()
{
	Json::Value data;
	return this->send_and_receive(data, 518, 12);
}

//============================================================================
//  ���Ƿ���
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
//  ���Ƿ��� ���������Ϣ
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
// 1,�ֻ�ʯ, 2,Ѫ��ʯ, 3,��ʯ, 4,����ʯ, 5,�����ʯ, 6,�߼���ʯ, 8,��Ҷ��, 9,�����
// 12,�ƽ�ʥˮ, 14,����, 15,����, 16,�����鱦, 17,����ħʯ, 18,��Ʒ������
// {18,17,16,3,1,2,15,9,8,6,4,5,14}
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_fo_info()		//��Ը��Ϣ
{
	Json::Value data;
	return this->send_and_receive(data, 518, 11);
}

//============================================================================
//  ���Ƿ���
// {module:518, action:13,
// request:[],
// Example		
//     [  ]
//
//"response":[Utils.UByteUtil,[Utils.IntUtil]]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_pray()			//��Ը
{
	Json::Value data;
	return this->send_and_receive(data, 518, 13);
}

//============================================================================
//  ���Ƿ���
// {module:518, action:14,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil,[Utils.IntUtil]]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_refresh_pray()	//ˢ����Ը����
{
	Json::Value data;
	return this->send_and_receive(data, 518, 14);
}

//============================================================================
//  ���Ƿ���
// {module:518, action:15,
// request:[Utils.IntUtil]
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil,Utils.IntUtil]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_extra_choose_pray_award(int item_id)	//ѡ����Ը����
{
	Json::Value data;
	data.append(item_id);
	return this->send_and_receive(data, 518, 15);
}

//============================================================================
//  ���Ƿ���
// {module:518, action:17,
// request:[],
// Example		
//     [  ]
//
//"response": [Utils.UByteUtil]
// Example
//     [ result, ...]
//============================================================================
Json::Value sxd_client::Mod_MainRoleFaXiang_Base_gain_pray_award()			//�ջ���Ը����
{
	Json::Value data;
	return this->send_and_receive(data, 518, 17);
}