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
	static const int NO_ENOUGTH_INGOT = 4;		//元宝不足
	static const int NOT_ENOUGH_RESOURSE = 6;	//冥想资源不足
	static const int NOT_ENOUGH_COUNT = 8;		//次数不足
};
void sxd_client::WuFaField()
{
	Json::Value data = this->Mod_WuFaField_Base_panel_info();
	for (auto list : data[0])
	{
		if (list[0].asInt() == 1)
		{
			//通玄密馆 高级
			if (list[4].asInt() == 1)
			{
				//若没有冥想，即今日剩余免费冥想次数为一，则免费冥想一次
				data = this->Mod_WuFaField_Base_draw(1, 0);
				int result = data[0].asInt();
				if (result == WuFaFieldType::SUCCESS)
				{
					common::log("【悟法领域】：免费冥想一次，成功！！");
					return;
				}
				else
				{
					common::log(boost::str(boost::format("【悟法领域】：result：[%1%]\n") % data[0]));
					return;
				}
			}
			else
			{
				common::log("【悟法领域】：今日完成免费冥想！！");
				return;
			}			
		}
	}
	//common::log(boost::str(boost::format("\n\t【悟法领域】：数据详情：[%1%]\n") % data));
}

//============================================================================
//  获取悟法领域信息
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
// 1：通玄密馆 高级， 2：通玄密馆 独门， 3：通玄密馆 入门， 4：北冥密馆
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_WuFaField_Base_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 658, 0);
}

//============================================================================
//  获取隐仙副本信息
// {module:658, action:1,
// request:[Utils.ByteUtil,Utils.ByteUtil]
// Example		
// WuFaFieldView.as
//       _data.call(Mod_WuFaField_Base.draw,this.draw_cal,[_loc1_,this._obj.num > 1 ? Mod_WuFaField_Base.DRAW_TEN : Mod_WuFaField_Base.DRAW_ONE]);
//     DRAW_ONE:int = 0；DRAW_TEN:int = 1
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
