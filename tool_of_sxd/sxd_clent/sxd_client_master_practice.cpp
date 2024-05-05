#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class MasterPractice
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int NO_CHALLENGE_TIMES = 5;		//��ս��������
	static const int IS_CHALLENGED = 6;				//����ս
	static const int JOB_ERROR = 8;					//����ְҵ����
	/*
	��ʥ��1
	���飺2 
	����3 
	���ǣ�5 
	��ʿ��6 
	��ʦ��15
	*/
};

int m_times = 1;

void sxd_client::MasterPractice()
{
	Json::Value data = this->Mod_MasterPractice_Base_main_panel();
	Json::Value job_practice_info_list = data[4];

	int job_id, battleStars;
	std::string job;
	common::log("\n\t����ʦ���С���\n\t 1.��ʥ\n\t 2.����\n\t 3.����\n\t 4.����\n\t 5.��ʿ\n\t 6.��ʦ\n\t ��ѡ����ս��ְҵ��");
	int k;
	std::cin >> k;
	switch (k) 
	{
	case 1:
		job_id = 1;
		job = "��ʥ";
		break;
	case 2:
		job_id = 2;
		job = "����";
		break;
	case 3:
		job_id = 3;
		job = "����";
		break;
	case 4:
		job_id = 5;
		job = "����";
		break;
	case 5:
		job_id = 6;
		job = "��ʿ";
		break;
	case 6:
		job_id = 15;
		job = "��ʦ";
		break;
	default:
		common::log("\n\t����ʦ���С���������󣡣����˳���");
		return;
	}
	
	common::log("\n\t����ʦ���С���\n\t 1.һ��\n\t 2.����\n\t 3.����\n\t ��ѡ����ս���Ǽ���");
	std::cin >> battleStars;
	if (battleStars != 1 && battleStars != 2 && battleStars != 3)
	{
		common::log("\n\t����ʦ���С���������󣡣����˳���");
		return;
	}

	this->challenge(job, job_id, battleStars);

	//Json::Value data = this->Mod_MasterPractice_Base_start_challenge(job_id, battleStars);
	//std::cout <<"data: "<< data << "\n";
}

void sxd_client::challenge(const std::string& job, int job_id, int battleStars)
{
	std::cout << boost::str(boost::format("����ʦ���С�����[ %3% ]����ս[%1%] [%2%]�Ǽ� \n") % job % battleStars % m_times);
	Json::Value data = this->Mod_MasterPractice_Base_start_challenge(job_id, battleStars);
	if (data[0].asInt() == MasterPractice::SUCCESS)
	{
		m_times++;
		this->challenge(job, job_id, battleStars);
		//std::cout << boost::str(boost::format("\n\t����ʦ���С���[%1%] [%2%]�Ǽ� ��ս�ɹ�����\n\n") % job % battleStars);
	}
	else if (data[0].asInt() == MasterPractice::IS_CHALLENGED)
	{
		std::cout << boost::str(boost::format("\n\t����ʦ���С���[%1%] [%2%]�Ǽ� �Ѿ���ս���ˣ���\n\n") % job % battleStars);
	}
	else if (data[0].asInt() == MasterPractice::FAILED)
	{
		std::cout << boost::str(boost::format("\n\t����ʦ���С���[%1%] [%2%]�Ǽ� ��սʧ�ܣ���\n\n") % job % battleStars);
		//m_times++;
		//this->challenge(job, job_id, battleStars);
	}
	else if (data[0].asInt() == MasterPractice::NO_CHALLENGE_TIMES)
	{
		common::log("\n\t����ʦ���С���������ս���������꣡��\n\n");
	}
}

//============================================================================
//  ��ʦ������Ϣ
// {module:665, action:1,
// request:[]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]]]
// Example:
// MasterPracticeData.as 82
// ["result","today_challenge_times","artifact_skill_level","have_item_list[]","job_practice_info_list[]"]
// job_practice_info_list:
//						[job_id, step_id, total_stars, step_up_cd_ts, today_challenge_list[], actived_soul_list[], use_features_list[]]
//============================================================================
Json::Value sxd_client::Mod_MasterPractice_Base_main_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 665, 1);
}

//============================================================================
//  ��ʦ������ս
// {module:665, action:5,
// request:[Utils.IntUtil,Utils.IntUtil]
// Example��
//		[job_id,battleStars]
// MasterPracticeView.as 128
//		 _data.call(Mod_MasterPractice_Base.start_challenge,this.start_challenge_callback,[this._info.job_practice_info.job_id,this._battleStars]);
// 
// response:[]
// Example
//	MasterPracticeData.as 681 
//     [  ]
//		this.result = param1[0];
//		if (this.result == Mod_MasterPractice_Base.SUCCESS)
//		{
//			this.warResult = param1[1];
//		}
//============================================================================
Json::Value sxd_client::Mod_MasterPractice_Base_start_challenge(int job_id, int battleStars)
{
	Json::Value data;
	data.append(job_id);
	data.append(battleStars);
	return this->send_and_receive(data, 665, 5);
}