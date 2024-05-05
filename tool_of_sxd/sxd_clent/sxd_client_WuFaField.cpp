#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

class WuFaFieldType
{
public:
	static const int SUCCESS = 2;
	static const int FAILED = 3;
	static const int NO_ENOUGTH_INGOT = 4;		//Ԫ������
	static const int NOT_ENOUGH_RESOURSE = 6;	//ڤ����Դ����
	static const int NOT_ENOUGH_COUNT = 8;		//��������
};
void sxd_client::WuFaField()
{
	Json::Value data = this->Mod_WuFaField_Base_panel_info();
	for (auto list : data[0])
	{
		if (list[0].asInt() == 1)
		{
			//ͨ���ܹ� �߼�
			if (list[4].asInt() == 1)
			{
				//��û��ڤ�룬������ʣ�����ڤ�����Ϊһ�������ڤ��һ��
				data = this->Mod_WuFaField_Base_draw(1, 0);
				int result = data[0].asInt();
				if (result == WuFaFieldType::SUCCESS)
				{
					common::log("�������򡿣����ڤ��һ�Σ��ɹ�����");
					return;
				}
				else
				{
					common::log(boost::str(boost::format("�������򡿣�result��[%1%]\n") % data[0]));
					return;
				}
			}
			else
			{
				common::log("�������򡿣�����������ڤ�룡��");
				return;
			}			
		}
	}
	//common::log(boost::str(boost::format("\n\t�������򡿣��������飺[%1%]\n") % data));
}

//============================================================================
//  ��ȡ��������Ϣ
// {module:658, action:0,
// request:[]
// Example		
//     [  ]
// response:[]
// 
// Example
/*
(_loc9_ = {}).field_type = _loc3_[_loc4_][0];
_loc9_.today_count = _loc3_[_loc4_][1];
_loc9_.total_count = _loc3_[_loc4_][2];
_loc9_.curr_count_id = _loc3_[_loc4_][3];
_loc9_.left_free_one_count = _loc3_[_loc4_][4];
_loc9_.left_free_ten_count = _loc3_[_loc4_][5];
_loc9_.wish_gongfa = _loc3_[_loc4_][6];
_loc9_.wish_gongfa_prob = _loc3_[_loc4_][7];
_loc9_.left_total_ten_count = _loc3_[_loc4_][8];
_loc2_.push(_loc9_);
_loc4_++;
}*/
// 1��ͨ���ܹ� �߼��� 2��ͨ���ܹ� ���ţ� 3��ͨ���ܹ� ���ţ� 4����ڤ�ܹ�
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_WuFaField_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 658, 0);
}

//============================================================================
//  ��ȡ���ɸ�����Ϣ
// {module:658, action:1,
// request:[Utils.ByteUtil,Utils.ByteUtil]
// Example		
// WuFaFieldView.as
//       _data.call(Mod_WuFaField_Base.draw,this.draw_cal,[_loc1_,this._obj.num > 1 ? Mod_WuFaField_Base.DRAW_TEN : Mod_WuFaField_Base.DRAW_ONE]);
//     DRAW_ONE:int = 0��DRAW_TEN:int = 1
// [ type,num ]
// response:[]
// 
// Example
// 
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_WuFaField_Base_draw(int type, int num)
{
	Json::Value data;
	data.append(type);
	data.append(num);
	return this->send_and_receive(data, 658, 1);
}
