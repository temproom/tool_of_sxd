#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

//������

class RepressDemonTowerType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int RACE_STEP_ERROR = 6;	//���½׶δ���
	//static const int TIMES_LIMIT = 3;
};

void sxd_client::RepressDemonTower()
{
	for (;;)
	{		
		common::log(" \n\t 1.�Զ���ս\n\t 2.�ֶ���ս\n\t  0.�˳�\n\t ��ѡ����Ӧ�Ĺ��ܣ�");
		int via;
		std::cin >> via;

		//��ȡ��������Ϣ
		Json::Value data = this->Mod_RepressDemonTower_Base_panel_info();

		//��ȡ����
		int step = data[4].asInt();
		//��ȡ����ɲ���
		int layer = data[5].asInt();
		//��ȡʣ����սȯ����
		int ticket_num = data[12].asInt();
		//�ܻغ���
		int round = data[6].asInt();
		common::log(boost::str(boost::format("��������������ǰ�ܻغ�����%1% \n") % round));

		if (layer == 40)
		{
			//std::cout << "��������������ǰ40����ȫ����ս��ϣ���\n\n";
			common::log("��������������ǰ40����ȫ����ս��ϣ���\n\n");
			common::log("�������������Ƿ������ս��\n\t 1.����\n\t 2.�˳�");
			int via;
			std::cin >> via;
			if (via != 1)
			{
				return;
			}
			else
			{
				common::log("��������������Ҫ��ս�����������");
				int id;
				std::cin >> id;
				if (id >= 1 && id <= 40)
				{
					int true_id = (step - 1) * 40 + id;

					int pass = 0;
					int times = 1;

					while (!pass)
					{
						data = this->Mod_RepressDemonTower_Base_challenge(true_id);
						if (data[0].asInt() == RepressDemonTowerType::SUCCESS)
						{
							Json::Value data_fight = this->Mod_RepressDemonTower_Base_panel_info();

							if (data_fight[12].asInt() == ticket_num)
							{
								common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,ʧ�ܣ���") % times % id));

								if (times == 10)
								{
									std::cout << "\n���ۼ�ʧ��10�Σ�ֹͣ��ս\n\n";
									pass = 1;
									return;
								}
								else
								{
									times++;
								}
							}
							else
							{
								common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,�ɹ�����") % times % id));

								Json::Value data_now = this->Mod_RepressDemonTower_Base_panel_info();
								int now_round = data_now[6].asInt();
								common::log(boost::str(boost::format("��������������ǰ�ܻغ�����%1% \n") % now_round));

								pass = 1;
								times = 1;
								return;
							}
						}
						else
						{
							common::log("��������������ս�����������ã���\n\n");
							return;
						}
					}

				}
			}
		}

		if (via == 2)
		{
			int finish = 0;
			while (!finish)
			{
				common::log(" \n\t ��������ս�Ĳ�����0�˳�����");
				int layer_fight;
				std::cin >> layer_fight;

				if (layer_fight == 0)
				{
					finish = 1;
					break;
				}
				int fight_id = (step - 1) * 40 + layer_fight;

				//data = this->Mod_RepressDemonTower_Base_challenge(fight_id);

				int times = 1;
				int pass = 0;

				while (!pass)
				{
					data = this->Mod_RepressDemonTower_Base_challenge(fight_id);

					if (data[0].asInt() == RepressDemonTowerType::SUCCESS)
					{
						Json::Value data_fight = this->Mod_RepressDemonTower_Base_panel_info();

						if (data_fight[12].asInt() == ticket_num)
						{
							common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,ʧ�ܣ���") % times % layer_fight));

							if (times == 100)
							{
								std::cout << "\n���ۼ�ʧ��100�Σ�ֹͣ��ս\n\n";
								pass = 1;
								return;
							}
							else
							{
								times++;
							}
						}
						else
						{
							common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,�ɹ�����") % times % layer_fight));

							Json::Value data_now = this->Mod_RepressDemonTower_Base_panel_info();

							int now_round = data_now[6].asInt();
							common::log(boost::str(boost::format("��������������ǰ�ܻغ�����%1% \n") % now_round));

							step = data_now[4].asInt();

							pass = 1;
							times = 1;
							return;
						}
					}
					else
					{
						common::log("��������������ս�����������ã���\n\n");
						return;
					}
				}
			}
			
		}
		else if (via == 1)
		{
			int finish = 0;
			while (!finish)
			{
				//��Ҫ��ս�Ĳ���
				int battle_id = (step - 1) * 40 + layer + 1;

				int times = 1;
				int pass = 0;
				//std::cout << "step= " << step << " layer= " << layer << " battle_id= " << battle_id;
				while (!pass)
				{
					data = this->Mod_RepressDemonTower_Base_challenge(battle_id);

					if (data[0].asInt() == RepressDemonTowerType::SUCCESS)
					{
						Json::Value data_fight = this->Mod_RepressDemonTower_Base_panel_info();

						//��ȡ����
						int step_fight = data_fight[4].asInt();
						//��ȡ����ɲ���
						int layer_fight = data_fight[5].asInt();

						if (layer_fight == layer)
						{
							common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,ʧ�ܣ���") % times % (layer + 1)));

							if (times == 100)
							{
								std::cout << "\n���ۼ�ʧ��100�Σ�ֹͣ��ս\n\n";
								pass = 1;
								return;
							}
							else
							{
								times++;
							}
						}
						else
						{
							common::log(boost::str(boost::format("�������������� %1% ����ս�� %2% ��,�ɹ�����") % times % (layer + 1)));

							//Json::Value data_now = this->Mod_RepressDemonTower_Base_panel_info();
							int now_round = data_fight[6].asInt();
							common::log(boost::str(boost::format("��������������ǰ�ܻغ�����%1% \n") % now_round));

							//������ս״̬�����
							pass = 1;
							times = 1;

							//���½���
							step = step_fight;
							//��������ɲ���
							layer = layer_fight;
							//���²���
							battle_id = (step_fight - 1) * 40 + layer_fight + 1;
							//RepressDemonTower();
						}
					}
					else if (data[0].asInt() == RepressDemonTowerType::RACE_STEP_ERROR)
					{
						finish = 1;
						common::log("�������������ȴ���һ�׶ο��ţ���\n\n");
						return;
					}
					else
					{
						common::log("��������������ս�����������ã���\n\n");
						return;
					}
				}
			}
			
		}
		else if (via == 0)
		{
			break;
		}
	}
	


	
	//std::cout << "������������data[0]:" << data[0] << "\ndata[1]:" << data[1];
}

//============================================================================
//  ��������Ϣ
// {module:705, action:0,
// request:[],
// Example		
//     [  ]
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// Example
// RepressDemonTowerData.as
// oObject.list(param1,this._panelInfo,["result","is_first","season","left_time","now_step","now_finish_layer","now_total_round","next_layer_left_time","rank","step_list","layer_list","buy_ticket_num","now_ticket_num","race_step","ranks"]);
//     [ 0, 0, 8, 810041, 4, 27, 44, 205241, 67, [ [ 9, 30, 74 ], [ 10, 27, 188 ], [ 17, 27, 198 ] ], [ [ 133, 2 ], [ 146, 2 ], [ 135, 1 ], [ 131, 1 ], [ 128, 2 ], [ 124, 1 ], [ 126, 1 ], [ 145, 3 ], [ 143, 4 ], [ 125, 1 ], [ 142, 1 ], [ 127, 1 ], [ 139, 2 ], [ 122, 1 ], [ 123, 1 ], [ 130, 1 ], [ 144, 1 ], [ 129, 2 ], [ 147, 3 ], [ 134, 1 ], [ 136, 5 ], [ 141, 1 ], [ 132, 1 ], [ 140, 1 ], [ 138, 1 ], [ 121, 1 ], [ 137, 2 ] ], 0, 256, 183, 0 ] 
// (now_step-1)*40+layer=��սid
//============================================================================
Json::Value sxd_client::Mod_RepressDemonTower_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 705, 0);
}

//============================================================================
//  ��������ս
// {module:705, action:1,
// request:[Utils.IntUtil],
// Example	
// RepressDemonTowerView.as		
//     _data.call(Mod_RepressDemonTower_Base.challenge,this.challenge_callBack,[this._objBattle.id]);
//	[����id]
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_RepressDemonTower_Base_challenge(int floor)
{
	Json::Value data;
	data.append(floor);
	return this->send_and_receive(data, 705, 1);
}

//============================================================================
//  ������ ��ȡ��������
// {module:705, action:6,
// request:[Utils.IntUtil],
// Example	
// RepressDemonTowerView.as	
//     _data.call(Mod_RepressDemonTower_Base.get_monster_team_info,this.get_monster_team_info_callback,[this._objBattle.id]);
//	[����id]
// 
// response:[Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]
// Example
// RepressDemonTowerData.as
// oObject.list(_loc2_,_loc3_,["monster_id","grid_id","stunt_id","supernatural_id","max_health","left_health","ratio"]);
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_RepressDemonTower_Base_get_monster_team_info(int floor)
{
	Json::Value data;
	data.append(floor);
	return this->send_and_receive(data, 705, 6);
}