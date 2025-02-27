#include "stdafx.h"
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class SeekImmortalRoadType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 4;
	static const int IS_GET = 6;
};
void sxd_client::SeekImmortalRoad()
{
	//��ȡѰ��֮·��Ϣ
	Json::Value data = this->Mod_SeekImmortalRoad_Base_open_panel();

	int point = data[0].asInt();
	int box_count = data[12].asInt();
	int season = data[13].asInt();

	while (box_count > 0)
	{
		data = this->Mod_SeekImmortalRoad_Base_open_box();

		int result = data[0].asInt();
		if (result != SeekImmortalRoadType::SUCCESS)
		{
			common::log(boost::str(boost::format("��Ѱ��֮·��������Ѱ��֮·����ʧ�ܣ������룺%1%\n") % result));
			break;
		}
		else
		{
			box_count--;
			common::log("��Ѱ��֮·��������Ѱ��֮·����ɹ�����");
		}

	}

	if (point < 24000)
	{
		common::log("��Ѱ��֮·������Ծ��С��24000���ݲ���ȡ����");
		return;
	}

	if (season == 1)
	{
		for (int i = 91; i < 121; i++)
		{
			data = this->Mod_SeekImmortalRoad_Base_get_award(i);

			int result = data[0].asInt();
			if (result == SeekImmortalRoadType::IS_GET)
			{
				common::log("��Ѱ��֮·��������ȡ����", 0);
			}		
			else if (result != SeekImmortalRoadType::SUCCESS)
			{
				common::log(boost::str(boost::format("��Ѱ��֮·������ȡѰ��֮·����ʧ�ܣ������룺%1%\n") % result));
				return;
			}
			else
			{
				common::log("��Ѱ��֮·������ȡѰ��֮·�����ɹ�����");
			}
		}
		return;
	}

	if (season == 2)
	{
		for (int i = 361; i < 391; i++)
		{
			data = this->Mod_SeekImmortalRoad_Base_get_award(i);

			int result = data[0].asInt();
			if (result == SeekImmortalRoadType::IS_GET)
			{
				common::log("��Ѱ��֮·��������ȡ����", 0);
			}
			else if (result != SeekImmortalRoadType::SUCCESS)
			{
				common::log(boost::str(boost::format("��Ѱ��֮·������ȡѰ��֮·����ʧ�ܣ������룺%1%\n") % result));
				return;
			}
			else
			{
				common::log("��Ѱ��֮·������ȡѰ��֮·�����ɹ�����");
			}
		}
		return;
	}


}

//============================================================================
//  ��ȡѰ��֮·��Ϣ
// {module:466, action:1,
// request:
// Example	
//     [  ]
// 
// "response":
// Example
// 
// SeekImmortalRoadData.as L43
//		     oObject.list(param1,this._info,["point","need_ingot","upgrade_ingot","buy_box_ingot","charge_ingot","is_buy","is_upgrade","now_buy_box_times","buy_box_max_times",SeekImmortalRoadDataType.NOR_AWARD,SeekImmortalRoadDataType.ADV_AWARD,SeekImmortalRoadDataType.BOX_AWARD,"box_count","season","is_in_act","back_ingot","discount_ticket_id","discount_ticket_num","upgrade_discount_ticket_id","upgrade_discount_ticket_num","left_count","role_id","top_player"]);
//20241106   oObject.list(param1,this._info,["point","need_ingot","upgrade_ingot","buy_box_ingot","charge_ingot","is_buy","is_upgrade","now_buy_box_times","buy_box_max_times",SeekImmortalRoadDataType.NOR_AWARD,SeekImmortalRoadDataType.ADV_AWARD,SeekImmortalRoadDataType.BOX_AWARD,"box_count","season","is_in_act","back_ingot","discount_ticket_id","discount_ticket_num","upgrade_discount_ticket_id","upgrade_discount_ticket_num","left_count","role_id","top_player"]);
// SeekImmortalRoadType
//		line_id������·��һ��ѡ�ڶ������ɳ�֮·��
//		typed�����ͣ�1Ϊ��ͨ��2��3�ɽ硿
//		ÿ��·�������ͣ�ÿ��������30��������
//		�������͹�90������������·��9�����ͣ�270������
// ��һ����91-120��
// �ڶ�����361-390��
//============================================================================
Json::Value sxd_client::Mod_SeekImmortalRoad_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 466, 1);
}

//============================================================================
//  ��ȡ����
// {module:466, action:3,
// request:[Utils.IntUtil],
// Example	
//     [ award_id ]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// 
// SeekImmortalRoadData.as L43
//		this.info.result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_SeekImmortalRoad_Base_get_award(int award_id)
{
	Json::Value data;
	data.append(award_id);
	return this->send_and_receive(data, 466, 3);
}

//============================================================================
//  �򿪺���
// {module:466, action:6,
// request:
// Example	
//     [  ]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// 
// SeekImmortalRoadData.as L43
//		this.info.result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_SeekImmortalRoad_Base_open_box()
{
	Json::Value data;
	return this->send_and_receive(data, 466, 6);
}