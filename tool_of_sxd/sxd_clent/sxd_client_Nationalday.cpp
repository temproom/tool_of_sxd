#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//礼赠诸君
class NationaldayCheckin2024Type
{
public:
	static const int ALREADY_SIGN = 0;
	static const int NO_SIGN = 1;
	static const int MAKEUP = 2;
	static const int SIGN = 3;
	static const int SUCCESS = 4;
	static const int FAILED = 5; 
	static const int ACT_NOT_OPEN = 6;
	static const int DAY_ERROR = 8;
	static const int NOT_ENOUGH_DAY = 14;
	static const int IS_GET = 15;
};
//乾坤一掷
class DiceRewardsType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int ACT_NOT_OPEN = 2; 
	static const int NOT_ENOUGH_RES = 3;
};

void sxd_client::Nationalday()
{
	//礼赠诸君
	NationaldayCheckin();

	//乾坤一掷
	DiceRewards();
	
}
void sxd_client::NationaldayCheckin()	//礼赠诸君
{
	
	Json::Value data = this->Mod_NationaldayCheckin2024_Base_main_panel();

	Json::Value list = data[0];
	vector<int> day_list(22);	//初始化22天，值为0；

	for (Json::Value day : list)
	{
		day_list[day[0].asInt()] = day[1].asInt();
	}

	for (int i = 1; i < day_list.size(); i++)
	{
		if (day_list[i])
		{
			Json::Value data_sign = this->Mod_NationaldayCheckin2024_Base_sign_in(i);
			if (data_sign[0].asInt() == NationaldayCheckin2024Type::SUCCESS)
			{
				common::log("【礼赠诸君】签到成功！");
			}
			else if (data_sign[0].asInt() == NationaldayCheckin2024Type::ALREADY_SIGN)
			{
				common::log("【礼赠诸君】已签到！");
				break;
			}
			else if (data_sign[0].asInt() == NationaldayCheckin2024Type::DAY_ERROR)
			{
				break;
			}
		}
	}

	//累签奖励
	for (int i = 1; i <= 4; i++)
	{
		data = this->Mod_NationaldayCheckin2024_Base_get_extra(i);
		if (data[0].asInt() == NationaldayCheckin2024Type::SUCCESS)
		{
			common::log("【礼赠诸君】领取累签奖励成功！");
		}
		else if (data[0].asInt() == NationaldayCheckin2024Type::NOT_ENOUGH_DAY)
		{
			common::log("【礼赠诸君】累签次数不足！");
			break;
		}
		else if(data[0].asInt() == NationaldayCheckin2024Type::IS_GET)
		{
			common::log("【礼赠诸君】累签已领取！");
		}
	}
	
}

void sxd_client::DiceRewards()		//乾坤一掷
{
	Json::Value data = this->Mod_DiceRewards_Base_main_panel();
	int res_num = data[1].asInt();//剩余骰子数量
	while (res_num > 0)
	{
		Json::Value data_roll = this->Mod_DiceRewards_Base_roll_dice(0);
		if (data_roll[0].asInt() == DiceRewardsType::SUCCESS)
		{
			common::log("【乾坤一掷】投掷成功！");
			res_num--;
		}
		else if (data_roll[0].asInt() == DiceRewardsType::NOT_ENOUGH_RES)
		{
			common::log("【乾坤一掷】次数不足！");
			break;
		}
		
	}
}

//============================================================================
// 礼赠诸君——面板信息
// "module":784,"action":0,
// "request":[]
// 
// "response":[[Utils.IntUtil,Utils.UByteUtil],[Utils.IntUtil]]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
//     [ [ [ 19, 1 ], [ 5, 1 ], [ 20, 1 ], [ 8, 1 ], [ 3, 1 ], [ 4, 1 ], [ 7, 1 ], [ 12, 1 ], [ 21, 1 ], [ 17, 1 ], [ 1, 0 ], [ 11, 1 ], [ 13, 1 ], [ 16, 1 ], [ 10, 1 ], [ 9, 1 ], [ 14, 1 ], [ 18, 1 ], [ 2, 1 ], [ 15, 1 ], [ 6, 1 ] ], null ] 

//============================================================================
Json::Value sxd_client::Mod_NationaldayCheckin2024_Base_main_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 784, 0);
}

//============================================================================
// 礼赠诸君——签到
// "module":784,"action":1,
// "request":[Utils.IntUtil]
//		_data.call(Mod_NationaldayCheckin2024_Base.sign_in,this.sign_in_call_back,[param1.day]);
// 
// "response":[Utils.UByteUtil]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_NationaldayCheckin2024_Base_sign_in(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 784, 1);
}

//============================================================================
// 礼赠诸君——累签奖励
// "module":784,"action":2,
// "request":[Utils.IntUtil]
//		_data.call(Mod_NationaldayCheckin2024_Base.get_extra,this.get_extra_call_back,[param1.id]);
// 
// "response":[Utils.UByteUtil]
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
// 
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_NationaldayCheckin2024_Base_get_extra(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 784, 2);
}

//============================================================================
// 乾坤一掷——面板信息
// "module":843,"action":0,
// "request":[]
// 
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,[Utils.IntUtil],[Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
//
//		oObject.list(param1,this.objPanelInfo,["pos_id","res_num","is_buy_privilege","lucky_number","map_grid","tasks","chrage_ingot"]);
//
// Example
//     [ 13, 0, 0, [ [ 328 ] ], [ [ 19, 0, 5822, 8 ], [ 5, 1, 8362, 20 ], [ 20, 0, 8358, 1 ], [ 26, 0, 9085, 2 ], [ 8, 0, 6759, 1 ], [ 3, 1, 10085, 1 ], [ 4, 0, 9886, 20 ], [ 7, 0, 8358, 1 ], [ 30, 0, 6504, 1 ], [ 12, 0, 6499, 1 ], [ 21, 0, 9886, 20 ], [ 17, 0, 9886, 20 ], [ 1, 0, 8362, 60 ], [ 29, 0, 9886, 20 ], [ 24, 0, 5853, 1 ], [ 11, 0, 7892, 8 ], [ 25, 0, 6499, 1 ], [ 13, 1, 9085, 2 ], [ 28, 0, 8362, 60 ], [ 23, 0, 7892, 8 ], [ 16, 0, 5671, 1 ], [ 10, 0, 9131, 1 ], [ 9, 0, 1411, 15 ], [ 14, 0, 8362, 60 ], [ 18, 0, 6504, 1 ], [ 2, 0, 2397, 40 ], [ 22, 0, 1411, 15 ], [ 15, 0, 2397, 40 ], [ 27, 0, 7264, 1 ], [ 6, 0, 5822, 8 ] ], [ [ 3, 0 ], [ 1, 600 ], [ 2, 0 ] ], 0 ] 
//============================================================================
Json::Value sxd_client::Mod_DiceRewards_Base_main_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 843, 0);
}

//============================================================================
// 乾坤一掷——投掷骰子
// "module":843,"action":1,
// "request":[Utils.IntUtil]
//		_data.call(Mod_DiceRewards_Base.roll_dice,this.roll_dice_call_back,[param1]);
// 
//
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
//
//		oObject.list(param1, this._objRollDice, ["result", "roll_points", "events", "award_list"]);
//
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_DiceRewards_Base_roll_dice(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 843, 1);
}