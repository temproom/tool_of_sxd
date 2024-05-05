#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class PhantomGemLotteryType
{
public:
	static const int SUCCESS = 0;	
	static const int FAILED = 1;
	static const int NO_ENOUGTH_TIMES = 2;		//��������
	static const int NO_ENOUGTH_INGOT = 3;		//Ԫ������
	static const int NO_ENOUGTH_DAOYUAN = 4;	//��Ե����
};

void sxd_client::PhantomGemLottery()
{
	common::log("���౦ɽ������ʼ��ͨ�󱦣�\n");
	//��ȡ�౦ɽ��Ϣ
	Json::Value data = this->Mod_PhantomGemLottery_Base_panel_info();
	int k = 0;
	//����3�Ŵ��౦ɽ
	for (k; k < 3; k++)
	{
		if (data[0][k][0].asInt() == 3)
		{
			break;
		}
	}
	//ʣ����ͨ�󱦴���
	int remain_normal_times = data[0][k][2].asInt();
	if (remain_normal_times == 0)
	{
		common::log("���౦ɽ������ͨ�󱦴��������꣡");
		return;
	}
	else
	{
		data = this->Mod_PhantomGemLottery_Base_find(3, 0);
		int result = data[0].asInt();
		if (result == PhantomGemLotteryType::SUCCESS)
		{
			common::log("���౦ɽ������ͨ�󱦳ɹ���");
			//return;
		}
		else if(result == PhantomGemLotteryType::NO_ENOUGTH_DAOYUAN)
		{
			common::log("���౦ɽ������Դ���㣬��ͨ��ʧ�ܣ���");		
			return;
		}	
		else if (result == PhantomGemLotteryType::NO_ENOUGTH_INGOT)
		{
			common::log("���౦ɽ����Ԫ�����㣬��ͨ��ʧ�ܣ���");
			return;
		}
		else if (result == PhantomGemLotteryType::NO_ENOUGTH_TIMES)
		{
			common::log("���౦ɽ�����������㣬��ͨ��ʧ�ܣ���");
			return;
		}
		else
		{
			common::log(boost::str(boost::format("���౦ɽ�����󱦽����[%1%]\n") % data));
		}
	}
}

//============================================================================
//  ��ȡ���ɸ�����Ϣ
// {module:694, action:0,
// request:[],
// Example		
//     [  ]
// response:[[],[],[],[]]
// [[Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
// 
// Example
// DevilExpeditionData.as
// oObject.list(param1, this.info, ["mountain_list", "requite_list", "requite_value", "ticket_num"]);
//    mountain_list:  oObject.list(_loc2_[_loc3_],_loc5_,["mountain_id","normal_list","remain_normal_times","high_list","remain_high_times"]);
//    normal_list:	  oObject.list(_loc6_[_loc4_],_loc8_,["item_id","item_num","prob","is_high"]);
//											[8978,1,162,1]��[һ�׺ڱ�ʯ������һ������16.2%��is_high]
// is_high==1,����ֱ�ӳ鵽��is_high==0,���������⣬��Ϸҳ����ʳ���8���͵���ʵ�ʸ���
//	  
//	  requite_list:   oObject.list(_loc2_[_loc3_],_loc5_,["requite_id","need_requite_value","requite_award_list","get_requite_award_id"]);
//    requite_award_list:   oObject.list(_loc6_[_loc4_],_loc8_,["requite_award_id","item_id","item_num","is_have"]);
//  [ ]
//============================================================================
Json::Value sxd_client::Mod_PhantomGemLottery_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 694, 0);
}

//============================================================================
//  ��ȡ���ɸ�����Ϣ
// {module:694, action:1,
// request:[Utils.ByteUtil,Utils.ByteUtil]
// Example		
// _data.call(Mod_PhantomGemLottery_Base.find, find_callback, [id, _loc1_]);
//     [  ]
// response:[]
// 
// Example
// 
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_PhantomGemLottery_Base_find(int id, int type)
{
	Json::Value data;
	data.append(id);
	data.append(type);
	return this->send_and_receive(data, 694, 1);
}

Json::Value sxd_client::Mod_PhantomGemLottery_Base_get_requite_award()
{
	Json::Value data;
	return this->send_and_receive(data, 694, 2);
}

