#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

//�����Ծ�������ʦ��

class MysticCaptureType
{
public:
	static const int SUCCESS = 0;
	static const int NO_TIMES = 2;
	static const int FAILED = 3;
	static const int WRONG_HUNT_SOUL_ID = 6;
};

void sxd_client::MysticCapture()
{
	//���������Ծ�
	Json::Value data = this->Mod_MysticCapture_Base_click_enter_hunt();

	//��«ư����
	int num = data[7].asInt();
	if (num < 2)
	{
		common::log("�������Ծ�������ǰ������«ưС��2���޷���׽��");
		return;
	}

	data = this->Mod_MysticCapture_Base_enter_hunt();

	int result = data[0].asInt();
	if (result != MysticCaptureType::SUCCESS)
	{
		if (result == MysticCaptureType::NO_TIMES)
		{
			common::log("�������Ծ����������Ծ�ʧ�ܣ���ǰ�޴�������");
			return;
		}
		common::log(boost::str(boost::format("�������Ծ����������Ծ�ʧ�ܣ������룺%1%\n") % result));
		return;
	}

	//������Ϣ
	Json::Value souls_info = data[1];
	//��«ư��Ϣ
	//Json::Value calabashes = data[2];

	//����«ư����
	/*int num;
	for (Json::Value data_calabashes : calabashes)
	{
		int calabashes_id = data_calabashes[0].asInt();
		int calabashes_num = data_calabashes[1].asInt();
		if (calabashes_id == 9765)
		{
			if (calabashes_num < 2)
			{
				common::log("�������Ծ�������ǰ��«ưС��2���޷���׽��");
				return;
			}
			num = calabashes_num;
		}
	}*/
	vector<int> souls_id;
	for (Json::Value data_souls : souls_info)
	{
		int id = data_souls[0].asInt();
		int soul_id = data_souls[1].asInt();
		//26-40������һ��
		if (soul_id > 25 && soul_id < 41)
		{
			souls_id.push_back(id);
		}
	}

	//��׽һ�ǻ���
	for (int id : souls_id)
	{
		bool flag = true;
		while (num >= 2 && flag)
		{
			Json::Value data_hunt = this->Mod_MysticCapture_Base_capture_soul(id, 9765);
			if (data_hunt[0].asInt() == MysticCaptureType::SUCCESS)
			{
				num--;
				data_hunt = this->Mod_MysticCapture_Base_capture_soul(id, 9765);
				if (data_hunt[0].asInt() == MysticCaptureType::SUCCESS)
				{
					common::log("�������Ծ�������׽�ɹ���");
					num--;
				}
				else
				{
					common::log(boost::str(boost::format("�������Ծ�������׽ʧ�ܣ������룺%1%\n") % data_hunt[0].asInt()));
					return;
				}
			}
			else
			{
				common::log(boost::str(boost::format("�������Ծ�������׽ʧ�ܣ������룺%1%\n") % data_hunt[0].asInt()));
				return;
			}
			flag = false;
		}
	}
}

//============================================================================
//  ���������Ծ�
// {module:762, action:0,
// request:[],
// Example		
//     [  ]
// 
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//			[ 12, 1, 1, 1, 0, 2, 44732, 2, 0 ] 
// �������������ɽ�������������ѽ���������Ƿ�������ղ�׽���Ƿ����ڲ�׽����ҵȼ����ȼ�������������«ư������������Ʒ������
// MysticCaptureData.as L48
//          oObject.list(param1,this._objTenterHunt,["all_souls_num","have_enter_times","today_enter_times","is_enter_novice_hunt","is_in_hunt","player_level","player_score","chuji_calabash_num","mystic_item_num"]);
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_MysticCapture_Base_click_enter_hunt()
{
	Json::Value data;
	return this->send_and_receive(data, 762, 0);
}

//============================================================================
//  ���������Ծ�
// {module:762, action:1,
// request:[],
// Example		
//     [  ]
// 
// response::[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
//			 [ 0, [ [ 1, 25, 0 ], [ 5, 32, 0 ], [ 3, 16, 0 ], [ 4, 30, 0 ], [ 2, 33, 0 ] ], [ [ 9765, 4 ], [ 9767, 0 ], [ 9768, 0 ], [ 9766, 1 ], [ 9769, 0 ] ], 2, 46378, 0, 0 ] 
//		�������������Ϣ����«ư����ҵȼ�����ҷ������Ƿ��������Ƿ�������ֽ̡̳�
// ������Ϣ����ǰ����id����������id��������
// ��1-10�����ǡ�����30
// ��11-25�����ǡ�����16
// ��26-40��һ�ǡ�����8
// ��9765���ճ���«ư��
// 20241106������
// ��41-45�����ǡ�����30
// 
// MysticCaptureData.as L115 L178
//             oObject.list(param1,this._objEnterHunt,["result","souls_info","calabashes","player_level","player_score","is_skip","is_enter_novice_hunt"]);
//			 oObject.list(param1[_loc2_],_loc3_,["hunt_soul_id","soul_type_id","fitness"]);
// 20241106	 oObject.list(param1[_loc2_],_loc3_,["hunt_soul_id","soul_type_id","fitness","is_new"]);
//============================================================================
Json::Value sxd_client::Mod_MysticCapture_Base_enter_hunt()
{
	Json::Value data;
	return this->send_and_receive(data, 762, 1);
}

//============================================================================
//  ��׽����
// {module:762, action:2,
// request:[Utils.IntUtil,Utils.IntUtil],
// data.call(Mod_MysticCapture_Base.capture_soul,this.capture_soul_call_back,[this._objMonster.id,this._obj.item_id]);
// Example		
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
// [ 0, 5, 4, 5, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, null, 0 ]
// [ 0, 3, 4, 5, 32, 52, 32, 1, 0, 105, 85, 76, 91, 119, [ [ 1, 1 ], [ 2, 20 ], [ 3, 14 ] ], 4722 ] 
// ���������׽״̬����������������id����������id���ҵĻ���id���������࣬�ȼ������飬�������ԣ��������ԣ��������ԣ���ʥ���ԣ��������ԣ����ܣ����֡�
// ��׽״̬��5δ��׽��3�ɹ���׽��
// Example
// MysticCaptureData.as L239
//             oObject.list(param1,this._objCaptureSoul,["result","state_id","add_fitness","hunt_soul_id","soul_type_id","my_soul_id","type_id","level","experience","personal_basic","personal_attack","personal_defence","personal_chengsheng","personal_special","skills","score"]);
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_MysticCapture_Base_capture_soul(int soul_id, int item_id)
{
	Json::Value data;
	data.append(soul_id);
	data.append(item_id);
	return this->send_and_receive(data, 762, 2);
}

//============================================================================
//  �뿪�����Ծ�
// {module:762, action:3,
// request:[],
// Example		
//     [  ]
// 
// response:[Utils.UByteUtil]
// Example
// FairyLandData.as
// oObject.list(param1,this.info,["level","remain_times","rank","difficulty"]);
//     [  ]
Json::Value sxd_client::Mod_MysticCapture_Base_exit_hunt()
{
	Json::Value data;
	return this->send_and_receive(data, 762, 3);
}