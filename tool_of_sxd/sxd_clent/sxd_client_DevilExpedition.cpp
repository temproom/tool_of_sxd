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
	//std::cout << boost::str(boost::format("��ħ��Զ������ѡ�����ڼ���\n") );
	//common::log(boost::str(boost::format("\n\t��ħ��Զ������ѡ�����ڼ���:") ));
	common::log("��ħ��Զ������ѡ�����ڼ���\n\t\t1:��һ�أ������ã�\n\t\t2:�ڶ��أ������ã�\n\t\t3:�����أ������ã�\n\t\t4:��սħ����\n\t\t5:��ս�������\n\t\t6:��ս���ɻ��\t\t��ѡ�����ڼ��أ�");
	//common::log("\n\t��ѡ�����ڼ��أ�");
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
			//�ɹ�
			common::log("��ħ��Զ���������븱���ɹ���");
			//��ȡ������Ϣ
			data = this->Mod_DevilExpedition_Base_get_mission_data_m1();
			common::log(boost::str(boost::format("��ħ��Զ������������Ϣ��[%1%]\n") % data));
		}
		else
		{
			common::log(boost::str(boost::format("��ħ��Զ�����������һ�ظ���ʧ�ܣ�������룺[%1%]\n") % result));
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
			//�ɹ�
			common::log("��ħ��Զ���������븱���ɹ���");
			//��ȡ������Ϣ
			//data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
			//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
			bool term = true;
			int type = 1;
			common::log("��ħ��Զ������ \n\t 1.�ۿ�\n\t 2.�˺�\n\t 3.����\n\t ��ѡ����Ӧ�Ĺ��ܣ�");
			std::cin >> type;
			while (term)
			{
				data = this->Mod_DevilExpedition_Base_fight_monster_m5(type);
				//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("��ħ��Զ���������������ս��");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("��ħ��Զ���������븱��ʧ�ܣ�������룺[%1%]\n") % result));
		}
		break;
	case 5:
		//������������
		data = this->Mod_DevilExpedition_Base_enter_town(144);
		result = data[0].asInt();
		if (result == DevilExpeditionType::SUCCESS)
		{
			//�ɹ�
			common::log("��ħ��Զ���������븱���ɹ���");
			//��ȡ������Ϣ
			data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
			//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
			bool term = true;
			while (term)
			{
				data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
				//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("��ħ��Զ���������������ս��");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("��ħ��Զ������������������ʧ�ܣ�������룺[%1%]\n") % result));
		}
		break;
	case 6:
		//�������ɸ���
		data = this->Mod_DevilExpedition_Base_enter_town(154);
		result = data[0].asInt();
		if(result == DevilExpeditionType::SUCCESS)
		{
			//�ɹ�
			common::log("��ħ��Զ���������븱���ɹ���");
			//��ȡ������Ϣ
			data = this->Mod_DevilExpedition_Base_get_mission_data_m7();
			//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
			bool term = true;
			while (term)
			{								
				data = this->Mod_DevilExpedition_Base_fight_monster_m7(1);
				//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
				int via = data[0].asInt();
				if (via != DevilExpeditionType::SUCCESS)
				{
					term = false;
					data = this->Mod_DevilExpedition_Base_quit();
					common::log("��ħ��Զ���������������ս��");
				}
			}
		}
		else
		{
			common::log(boost::str(boost::format("��ħ��Զ�������������ɸ���ʧ�ܣ�������룺[%1%]\n") % result));
		}
		break;
	}		
}

void sxd_client::Single_DevilExpedition()
{
	common::log("��ħ��Զ������ѡ������ս�ڼ���\n\t1:����\n\t2:����\n\t3:ħ��\n\t��ѡ�����ڼ��أ�");
	int floor;
	int via;
	std::cin >> via;
	Json::Value data;
	int type = 1;		//ħ����ս��ѡ��ħ�����ͣ�Ĭ�ϵۿ�
	floor = DevilExpeditionType::MISSION_HERMIT_HOME;			//Ĭ����ս����
	int mission_id = 154;

	if (via == 2)
	{
		//����
		floor = DevilExpeditionType::MISSION_YINYANG_HALL;
		mission_id = 144;
	}	
	else if (via == 3)
	{
		//ħ��
		floor = DevilExpeditionType::MISSION_MONSTER_GOD;
		mission_id = 91;;
		common::log(" ��ħ��Զ������ѡ����սħ��\n\t 1.�ۿ�\n\t 2.�˺�\n\t 3.����\n\t ��ѡ����Ӧ�Ĺ��ܣ�");
		std::cin >> type;
	}

	data = this->Mod_DevilExpedition_Base_start_expedition_single(floor);

	//data = this->Mod_DevilExpedition_Base_enter_town(mission_id);
	int result = data[0].asInt();
	if (result == DevilExpeditionType::SUCCESS)
	{
		//�ɹ�
		common::log("��ħ��Զ���������븱���ɹ���");

		data = this->Mod_DevilExpedition_Base_reject_buy_box();

		//��ȡ������Ϣ
		if (via == 1)
		{
			//����
			data = this->Mod_DevilExpedition_Base_get_mission_data_m7();
		}
		else if (via == 2)
		{
			//����
			data = this->Mod_DevilExpedition_Base_get_mission_data_m6();
		}	
		//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
		bool term = true;
		while (term)
		{
			if (via == 1)
			{
				//����
				data = this->Mod_DevilExpedition_Base_fight_monster_m7(1);
			}
			else if (via == 2)
			{
				//����
				data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
			}
			else if (via == 3)
			{
				//ħ��
				data = this->Mod_DevilExpedition_Base_fight_monster_m5(type);
			}
			//data = this->Mod_DevilExpedition_Base_fight_monster_m6(1);
			//common::log(boost::str(boost::format("\n\t��ħ��Զ������������Ϣ��[%1%]\n") % data));
			int end = data[0].asInt();
			if (end != DevilExpeditionType::SUCCESS)
			{
				term = false;
				data = this->Mod_DevilExpedition_Base_quit();
				common::log("��ħ��Զ���������������ս��");
			}
		}
	}
}

//86:["MoJieYuanZheng1","ħ��Զ��1",{},5010,"ħ��Զ����һ�س���",0,0],
//87: ["MoJieYuanZheng2", "ħ��Զ��2", {}, 5020, "ħ��Զ���ڶ��س���", 0, 0] ,
//88 : ["MoJieYuanZheng3", "ħ��Զ��3", {}, 5030, "ħ��Զ�������س���", 0, 0] ,
//89 : ["MoWangDian", "ħ����", {}, 5040, "ħ��Զ�����Ĺس���", 0, 0] ,
//91:["ShenYuanFuBen","��Ԩ����",{},5050,"ħ��Զ������س���",0,0] ħ��
//144:["ShenYuanFuBen2","��Ԩ����2",{},5060,"ħ��Զ������س���",0,0], ����
//154: ["ShenYuanFuBen3", "��Ԩ����3", {}, 5070, "ħ��Զ������س���", 0, 0] ����
//============================================================================
//  ����ħ��Զ����Ϣ
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
//  �뿪ħ��Զ������
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
//  �˳�ħ��Զ��
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
//  ��ȡ���ɸ�����Ϣ ��һ��
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
//  ��ȡ���ɸ�����Ϣ ����
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
//  ս�� ����
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
//  ��ȡ���ɸ�����Ϣ ����
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
//  ��ȡ���ɸ�������
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
//  ��ȡ���ɸ�����Ϣ ����
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
//  ս�� ����
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
//  ս�� ħ��
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
//  ���˿�����ս��
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
//  �ܾ�Ԫ�����һر���
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
