#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class GlazePaviliontype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int not_ENOUGH_SCORE = 4;
	static const int IS_GET = 5;
	static const int TASK_NOT_COMPLETED = 6;
}; 

void sxd_client::GlazePavilion()
{
	//��ȡ״̬
	Json::Value data = this->Mod_GlazePavilion_Base_get_state();
	//����
	int season = data[0].asInt();
	//�Ƿ������0�������У�1���ѽ���
	//if (data[1].asInt() == 1)
		//return;

	//GlazePavilionTypeData.as
	//��һ��(1,2,3,4,5)���ڶ���(6,7,8,9,10)��ÿ��ǰ����Ϊ��Ծ�Ƚ���
	vector<vector<int>> daily_task = { {1,2,3,4,5},{6,7,8,9,10} };
	
	//��ȡ��ϸ��Ϣ
	data = this->Mod_GlazePavilion_Base_main_panel_info();
	int round = data[1].asInt();

	//��ȡÿ��������
	for (int n = 0; n < 3; n++)
	{
		data = this->Mod_GlazePavilion_Base_get_daily_task(daily_task[season-1][n]);
		int result = data[0].asInt();
		if (result == GlazePaviliontype::SUCCESS)
		{
			common::log("������������ȡ��Ծ�Ƚ����ɹ�����");
		}
		else if (result == GlazePaviliontype::FAILED)
		{
			common::log("������������ȡ��Ծ�Ƚ���ʧ�ܣ���");
		}
		else if (result == GlazePaviliontype::IS_GET)
		{
			common::log("���������󡿻�Ծ�Ƚ�������ȡ����");
		}
	}	

	//һ�����֣�ÿ��7��
	//��һ����1-7��,(8-14),(15-21)	
	//�ڶ���(22-28),(29-35),(36-42)
	int m = 2, n = 3, p = 7;
	int via = 1;
	vector<vector<vector<int>>> score_task(m, vector<vector<int>>(n, vector<int>(p, 1)));
	for (int i = 0; i < score_task.size(); i++)
	{
		for (int j = 0; j < score_task[0].size(); j++)
		{
			for (int k = 0; k < score_task[0][0].size(); k++)
			{
				score_task[i][j][k] = via;
				via++;
			}
		}
	}

	for (int i = 0; i < 7; i++)
	{
		//��ȡ���ֽ���
		data = this->Mod_GlazePavilion_Base_get_score_award(score_task[season-1][round-1][i]);
		int result = data[0].asInt();
		if (result == GlazePaviliontype::SUCCESS)
		{
			common::log("������������ȡ���ֽ����ɹ�����");
		}
		else if (result == GlazePaviliontype::FAILED)
		{
			common::log("������������ȡ��Ծ�Ƚ���ʧ�ܣ���");
		}
		else if (result == GlazePaviliontype::IS_GET)
		{
			common::log("���������󡿻��ֽ�������ȡ����");
		}
		else if (result == GlazePaviliontype::not_ENOUGH_SCORE)
		{
			common::log("���������󡿻��ֲ��㣡��");
		}
	}
	
	//��ȡ������
	//��һ����id=1
	//�ڶ�����id=9
	vector<int> limited = { 1,9 };
	data = this->Mod_GlazePavilion_Base_get_limited_buy(limited[season - 1]);
	int result = data[0].asInt();
	if (result == GlazePaviliontype::SUCCESS)
	{
		common::log("������������ȡ�������ɹ�����");
	}
	else if (result == GlazePaviliontype::FAILED)
	{
		common::log("������������ȡ������ʧ�ܣ���");
	}
	else if (result == GlazePaviliontype::IS_GET)
	{
		common::log("��������������������ȡ����");
	}
}

//============================================================================
//	��������״̬
// "module":799,"action":0,
// "request":[]
// 
// "response":[Utils.IntUtil,Utils.ByteUtil]
//		
// glazepaviliondata.as		
//			         oObject.list(param1,this.stateInfo,["season","is_receive"]);
//					this.stateInfo.is_receive = this.stateInfo.is_receive == 1;
// Example
//			[ 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_GlazePavilion_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 799, 0);
}

//============================================================================
//	����������ϸ��Ϣ
// "module":799,"action":1,
// "request":[]
// 
// "response":[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil]
//		
// glazepaviliondata.as	
//			oObject.list(param1,this.mainPanelInfo,["season","round","get_score_awards","tasks","daily_task_receive","limited_buy_receive","now_score"]);
// Example
//			[ 1, 1, [ [ 4 ], [ 2 ], [ 3 ], [ 1 ] ], [ [ 2, 50 ], [ 1, 355 ], [ 5, 175 ] ], 0, 0, 1000 ] 
//			[ 1, 1, [ [ 4 ], [ 2 ], [ 3 ], [ 1 ] ], [ [ 2, 50 ], [ 1, 355 ], [ 5, 675 ] ], 0, 0, 1500 ] 
//============================================================================
Json::Value sxd_client::Mod_GlazePavilion_Base_main_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 799, 1);
}

//============================================================================
//	�������󣺻�ȡ���ֽ���
// "module":799,"action":2,
// "request":[Utils.IntUtil],
// 
// glazepavilionview.as
//					_data.call(Mod_GlazePavilion_Base.get_score_award,this.get_score_award_callback,[param1.id]);
// 
// "response":[Utils.UByteUtil]
//		
// endlessexpeditiondata.as		
//			
// Example
//			[ 0, 1100, 1, 0, 0, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_GlazePavilion_Base_get_score_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 799, 2);
}

//============================================================================
//	�������󣺻�ȡÿ��������
// "module":799,"action":4,
// "request":[Utils.IntUtil],
// 
// glazepaviliontaskview.as
//				_data.call(Mod_GlazePavilion_Base.get_daily_task,this.get_daily_task_callback,[param1.id]);
// "response":[Utils.UByteUtil]
//		
// endlessexpeditiondata.as		
//			oObject.list(param1,this._missionPanel,["result","reg_id","mission_id","pass_flag","get_award_flag","quick_flag","left_award_num"]);
// Example
//			[ 0, 1100, 1, 0, 0, 1, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_GlazePavilion_Base_get_daily_task(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 799, 4);
}

Json::Value sxd_client::Mod_GlazePavilion_Base_get_limited_buy(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 799, 8);
}