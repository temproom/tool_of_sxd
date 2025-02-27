#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

//ʥ���ؾ�

class SaTrialsType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 3;
	static const int SA_TRIALS_OPEN = 18;	//ʥ���ؾ�����
	static const int SA_TRIALS_CLOSE = 19;	//ʥ���ؾ��ر�
	static const int SA_TRIALS_FINISH = 20;	//ʥ���ؾ����
};

void sxd_client::satrials()
{
	//��ȡʥ���ؾ���Ϣ
	Json::Value data = this->Mod_satrials_Base_get_panel_info();

	//��ս
	data = this->Mod_satrials_Base_challenge();
	data = this->Mod_satrials_Base_open_box();
	data = this->Mod_satrials_Base_choose_box_type(2);
}

//============================================================================
//  ʥ���ؾ� ��ȡ��Ϣ
// {module:511, action:0,
// request:[],
// Example	
// 
// response:
// Example
// satrialsData.as
// this.result = param1[0];
//     [ 0, 1, 0, "DJ\u51b7\u5c11", 300, 511, 50, [ [ 5, 0, 0 ], [ 3, 0, 0 ], [ 4, 0, 0 ], [ 1, 0, 0 ], [ 2, 1, 0 ], [ 6, 0, 0 ] ], null, [ [ 3, 0, [ [ 12, 0, 0 ], [ 11, 0, 0 ], [ 13, 0, 0 ], [ 14, 0, 0 ], [ 15, 0, 0 ] ] ], [ 4, 0, [ [ 19, 0, 0 ], [ 20, 0, 0 ], [ 17, 0, 0 ], [ 16, 0, 0 ], [ 18, 0, 0 ] ] ], [ 1, 0, [ [ 5, 0, 0 ], [ 3, 0, 0 ], [ 4, 0, 0 ], [ 1, 0, 0 ], [ 2, 0, 0 ] ] ], [ 2, 0, [ [ 8, 0, 0 ], [ 7, 0, 0 ], [ 10, 0, 0 ], [ 9, 0, 0 ], [ 6, 0, 0 ] ] ] ], 0, 1, 14, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 0);
}

//============================================================================
//  ʥ���ؾ� ����
// {module:511, action:1,
// request:[],
// Example	
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_reset_trials()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 1);
}

//============================================================================
//  ʥ���ؾ� �򿪺���
// {module:511, action:3,
// request:[],
// Example	
// 
// response:
// Example
// satrials.as
// [result,box_type,bufftem,pecial_buff_list]
// bufftem>0,
//     [ 0, 3, 0, null ] 
//	   [ 0, 1, 0, [ [ 5 ], [ 6 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_open_box()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 3);
}

//============================================================================
//  ʥ���ؾ� ѡ���������
// {module:511, action:4,
// request:[],
// Example	
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_choose_box_type(int floor)
{
	Json::Value data;
	data.append(floor);
	return this->send_and_receive(data, 511, 4);
}

//============================================================================
//  ʥ���ؾ� ѡ������buff
// {module:511, action:5,
// request:[],
// Example	
// _data.call(Mod_SaTrials_Base.choose_special_buff,this.chooseSpecialBuffBack,[param1]);
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_choose_special_buff(int floor)
{
	Json::Value data;
	data.append(floor);
	return this->send_and_receive(data, 511, 5);
}

//============================================================================
//  ʥ���ؾ� ��ȡ�׶ν���
// {module:511, action:6,
// request:[],
// Example	
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_gain_step_award()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 6);
}

//============================================================================
//  ʥ���ؾ� ѡ��׶ν���
// {module:511, action:7,
// request:[],
// Example	
// _data.call(Mod_SaTrials_Base.choose_step_award,chooseStepAwardBack,[awardId,!!isGetAll ? 1 : 0]);
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_choose_step_award(int floor, int k)
{
	Json::Value data;
	data.append(floor);
	data.append(k);
	return this->send_and_receive(data, 511, 7);
}

//============================================================================
//  ʥ���ؾ� ��ս
// {module:511, action:8,
// request:[],
// Example	
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_challenge()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 8);
}

//============================================================================
//  ʥ���ؾ� 
// {module:511, action:9,
// request:[],
// Example	
// 
// response:
// Example
// RepressDemonTowerData.as
// this.result = param1[0];
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_satrials_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 9);
}