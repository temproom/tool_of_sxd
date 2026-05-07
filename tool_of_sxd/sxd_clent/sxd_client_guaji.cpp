#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class GoldenTouchStoneType	//点金石
{
public:
	static const int TOUCH_STATUS = 2;
	static const int SUCCESS = 3;
	static const int FAIL = 4;
	static const int NOT_ENOUGH_COUNT = 7;
};

class DemonInvasionType		//坐骑试炼
{
public:
	static const int OPEN = 2;
	static const int CLOSE = 3;
	static const int SUCCESS = 4;
	static const int FAILED = 5;
	static const int NO_TIMES = 6;
};

class RefinePoolType		//淬炼池
{
public:
	static const int REFINE_1 = 5;	//淬炼玄铁
	static const int REFINE_2 = 6;	//提炼灵材
	static const int COINS = 7;		//铜钱
	static const int INGOT = 8;
	static const int NORMAL = 9;
	static const int ONE_KEY = 10;
	static const int SUCCESS = 11;
	static const int FAILED = 14;
};

class StarPictureType		//法力星图
{
public:
	static const int SUCCESS = 0;
	static const int NOT_ENOUGH_COUNT = 1;
};

class StarLetterType		//星辰手信
{
public:
	static const int SUCCESS = 0;
	static const int FUNC_NOT_OPEN = 1;
};

class SaUnionWorshipType		//圣盟祭祀
{
public:
	static const int COIN = 0;
	static const int INGOT = 1;
	static const int SUCCESS = 2;
	static const int NO_TIME = 5;
};

class TacticsType		//兵法
{
public:
	static const int XIAO_SHENG_SHUI = 0;
	static const int DA_SHENG_SHUI = 1;
	static const int SUCCEED = 7;
	static const int BUY_LIMIT = 14;
	static const int SUCCESS = 17;
};

class SkyPalaceType		//通天道场
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 1;
	static const int BREAK_THROUGH_FAILD = 2;
	static const int FAILED = 5;
	static const int NO_ENOUGTH_MEDICINE = 10;
	static const int USE_MEDICINE_TIME_LIMIT = 12;
};

//点金石
void sxd_client::GoldenTouchStone()
{
	Json::Value data = this->Mod_GoldenTouchStone_Base_touch_stone();
	if (data[0].asInt() == GoldenTouchStoneType::SUCCESS)
	{
		common::log("【点金石】点金成功！！");
	}
	else if(data[0].asInt() == GoldenTouchStoneType::NOT_ENOUGH_COUNT)
	{
		common::log("【点金石】次数不足！！");
	}
}

//============================================================================
//  点金石 状态信息
// {module:551, action:0, 
// request:[], 
// 
// response:[Utils.UByteUtil]
// 
// GoldenTouchStoneData.as 40:
//				this.info.status = this.status = param1[0];
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_GoldenTouchStone_Base_get_status()
{
	Json::Value data;
	return this->send_and_receive(data, 551, 0);
}
//============================================================================
//  点金石 点金
// {module:551, action:6, 
// request:[], 
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil]
// 
// GoldenTouchStoneData.as 40:
// 
// Example
//     [ 7, 0, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_GoldenTouchStone_Base_touch_stone()
{
	Json::Value data;
	return this->send_and_receive(data, 551, 6);
}

//坐骑试炼
void sxd_client::DemonInvasion()
{
	Json::Value data = this->Mod_DemonInvasion_Base_get_state();
	if (data[0].asInt() == DemonInvasionType::CLOSE)
	{
		common::log("【坐骑试炼】未开启！！");
		return;
	}
	else if (data[0].asInt() == DemonInvasionType::OPEN)
	{
		data = this->Mod_DemonInvasion_Base_open_panel();
		int left_time = data[1].asInt();
		int total_time = data[2].asInt();
		while (left_time > 0)
		{
			data = this->Mod_DemonInvasion_Base_start_challenge();
			if (data[0].asInt() == DemonInvasionType::SUCCESS)
			{
				left_time--;
				common::log("【坐骑试炼】挑战成功！！");
			}
			else if (data[0].asInt() == DemonInvasionType::NO_TIMES)
			{
				common::log("【坐骑试炼】次数不足！！");
				break;
			}
		}
		
	}
}

//============================================================================
//  坐骑试炼 状态信息
// {module:411, action:0, 
// request:[], 
// 
// response:[Utils.UByteUtil]
// 
// DemonInvasionData.as 40:
//				this.activityStatus = param1[0];
// Example
//     [ 3 ]
//============================================================================
Json::Value sxd_client::Mod_DemonInvasion_Base_get_state()
{
	Json::Value data;
	return this->send_and_receive(data, 411, 0);
}

//============================================================================
//  坐骑试炼 面板信息
// {module:411, action:1, 
// request:[], 
// 
// response:[[Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.LongUtil,Utils.LongUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil],Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
// 
// DemonInvasionData.as 40:
//				for each(_loc4_ in param1[0])	oObject.list(_loc4_,_loc3_,["team_id","members","grids","complete"]);
//				this._info.leftTime = param1[1];
//				this._info.totalTimes = param1[2];
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_DemonInvasion_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 411, 1);
}

//============================================================================
//  坐骑试炼 挑战
// {module:411, action:4, 
// request:[], 
// 
// response:[Utils.UByteUtil]
// 
// DemonInvasionData.as 40:
//				this.result = param1[0];
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_DemonInvasion_Base_start_challenge()
{
	Json::Value data;
	return this->send_and_receive(data, 411, 4);
}

//淬炼池
void sxd_client::RefinePool()
{
	Json::Value data = this->Mod_RefinePool_Base_open_panel();
	data = this->Mod_RefinePool_Base_refine(RefinePoolType::REFINE_2, RefinePoolType::COINS, RefinePoolType::ONE_KEY);
	if (data[0].asInt() == RefinePoolType::SUCCESS)
	{
		common::log("【淬炼池】一键提炼灵材成功！！");
	}
}

//============================================================================
//  淬炼池 打开面板
// {module:282, action:0, 
// request:[], 
// 
// response:[[Utils.ByteUtil,Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.UByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ShortUtil]
// 
// RefinePoolData.as 40:
//				oObject.list(param1,_loc2_,["iron_info_list","cur_seq","level","exp","one_key_lock_flag","coins_need","ingot_need","lian_qi_jing_hua","xuan_tie"]);
// Example
//     [ [ [ 10, 2, 0, 0, 1 ], [ 9, 2, 0, 0, 1 ], [ 8, 2, 0, 0, 1 ], [ 7, 2, 0, 0, 1 ], [ 6, 2, 0, 0, 1 ], [ 5, 2, 0, 0, 1 ], [ 4, 2, 0, 0, 1 ], [ 3, 2, 0, 0, 1 ], [ 2, 2, 0, 0, 1 ], [ 1, 2, 0, 0, 1 ] ], 4, 5, 0, 4, 500000, 50, 132193, 1435 ] 
//============================================================================
Json::Value sxd_client::Mod_RefinePool_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 282, 0);
}

//============================================================================
//  淬炼池 淬炼
// {module:282, action:1, 
// request:[Utils.UByteUtil,Utils.UByteUtil,Utils.UByteUtil], 
// 
// RefinePoolView.as
//			_data.call(Mod_RefinePool_Base.refine,onRefineCallBack,[type,refine_quality,refine_type]);
//				omsumeType = type == Mod_RefinePool_Base.REFINE_1
//				 refine_quality == Mod_RefinePool_Base.INGOT
//				refine_type == Mod_RefinePool_Base.ONE_KEY			
// 
// response:[Utils.UByteUtil,[Utils.ByteUtil,Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.UByteUtil,Utils.ByteUtil,Utils.ShortUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,Utils.IntUtil,Utils.UByteUtil,Utils.IntUtil,Utils.ShortUtil]
// 
// RefinePoolData.as 40:
//				oObject.list(param1,this.refineResult,["result","iron_info_list","cur_seq","award_list","level","exp","one_key_lock_flag","lian_qi_jing_hua","xuan_tie"]);
// Example
//     [ 11, [ [ 4, 1, 9600, 9600, 0 ], [ 10, 1, 67200, 67200, 0 ], [ 8, 1, 48000, 48000, 0 ], [ 9, 1, 57600, 57600, 0 ], [ 2, 1, 86400, 86400, 0 ], [ 1, 1, 76800, 76800, 0 ], [ 5, 1, 19200, 19200, 0 ], [ 6, 1, 28800, 28800, 0 ], [ 7, 1, 38400, 38400, 0 ], [ 3, 1, 96000, 96000, 0 ] ], 4, [ [ 3, 11, 6, 40, [ [ 4759, 20 ] ], [ [ 19, 1 ] ] ], [ 2, 14, 1, 10, [ [ 4759, 5 ] ], null ], [ 1, 11, 6, 40, [ [ 4759, 20 ] ], [ [ 6, 1 ] ] ], [ 10, 14, 2, 20, [ [ 4759, 10 ] ], null ], [ 9, 14, 2, 20, [ [ 4759, 10 ] ], null ], [ 8, 11, 6, 40, [ [ 4759, 20 ] ], [ [ 10, 1 ] ] ], [ 7, 14, 4, 40, [ [ 4759, 20 ] ], null ], [ 6, 14, 2, 20, [ [ 4759, 10 ] ], null ], [ 5, 14, 4, 40, [ [ 4759, 20 ] ], null ], [ 4, 14, 4, 40, [ [ 4759, 20 ] ], null ] ], 5, 0, 4, 132348, 1435 ] 
//============================================================================
Json::Value sxd_client::Mod_RefinePool_Base_refine(int type, int refine_quality, int refine_type)
{
	Json::Value data;
	data.append(type);
	data.append(refine_quality);
	data.append(refine_type);
	return this->send_and_receive(data, 282, 1);
}

//法力星图
void sxd_client::StarPicture()
{
	Json::Value data = this->Mod_StarPicture_Base_get_info();
	int has_divination_count = data[0].asInt();
	int free_divination_count = data[1].asInt();
	if (has_divination_count != free_divination_count)
	{
		data = this->Mod_StarPicture_Base_divination();
		if (data[0].asInt() == StarPictureType::SUCCESS)
		{
			common::log("【法力星图】免费占卜成功！！");
			data = this->Mod_StarPicture_Base_gain_award();
		}
		else if (data[0].asInt() == StarPictureType::NOT_ENOUGH_COUNT)
		{
			common::log("【法力星图】免费占卜次数用完！！");
		}
	}
}

//============================================================================
//  法力星图 获取信息
// {module:485, action:0, 
// request:[], 
// 
// response:[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil]
// 
// StarPictureData.as 40:
//				oObject.list(this._aryGetInfo,_loc2_,["has_divination_count","free_divination_count","ingot_divination_count","chip_count","reset_count","cur_delpoy_level","award_list","is_ues_dao_yuan_reset","chip_auto_lock_flag","da_fa_li_shi_auto_lock_flag","is_weekend_discount"]);
// Example
//     [ 0, 1, 2, 189, 0, 2, null, 1, 0, 0, 0 ] 
//	   [ 0, 1, 7, 189, 0, 2, null, 1, 0, 0, 0 ] 
//	   [ 1, 1, 7, 189, 0, 2, null, 1, 0, 0, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_StarPicture_Base_get_info()
{
	Json::Value data;
	return this->send_and_receive(data, 485, 0);
}

//============================================================================
//  法力星图 占卜
// {module:485, action:1, 
// request:[], 
// 
// response:[Utils.UByteUtil]
// 
// StarPictureData.as 40:
//				oObject.list(this._aryDivination,_loc1_,["result"]);
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_StarPicture_Base_divination()
{
	Json::Value data;
	return this->send_and_receive(data, 485, 1);
}

//============================================================================
//  法力星图 收获
// {module:485, action:5, 
// request:[], 
// 
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// StarPictureData.as 40:
//				oObject.list(this._aryGainAward,objReturn,["result","award_list"]);
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_StarPicture_Base_gain_award()
{
	Json::Value data;
	return this->send_and_receive(data, 485, 5);
}

//星辰手信
void sxd_client::StarLetter()
{
	vector<int> star_award = { 0,4,4,4,4,4,5,6,6,7,7,7,6 };

	Json::Value data = this->Mod_StarLetter_Base_star_letter_info();

	//data = this->Mod_StarLetter_Base_active();
	Json::Value active_award_list = data[6];

	
	int star_id = 1;
	int is_choose = 0;
	for (Json::Value star : data[0])
	{
		if (star[1].asInt() == 1)
		{
			common::log("【星辰手信】本周已选择");
			is_choose = 1;
			break;
		}

		int temp = star[0].asInt();
		if (star_award[temp] > star_award[star_id])
		{
			star_id = temp;
			is_choose = star[1].asInt();
		}
	}
	
	if (!is_choose)
	{
		data = this->Mod_StarLetter_Base_choose_star(star_id);
		int result = data[0].asInt();
		if (result == StarLetterType::SUCCESS)
		{
			common::log("【星辰手信】选择星辰成功！！");
			/*int num = star_award[star_id];
			for (int i = 1; i <= num; i++)
			{
				Json::Value data1 = this->Mod_StarLetter_Base_receive_letter();
				int result1= data1[0].asInt();
				if (result1 == StarLetterType::SUCCESS)
				{
					common::log("【星辰手信】领取星辰手信成功！！");
				}
			}*/
		}
		else
		{
			common::log(boost::str(boost::format("【星辰手信】：选择星辰失败，错误代码：【%1%】！！") % result));
			return;
		}
	}
	
	data = this->Mod_StarLetter_Base_star_letter_info();
	int num = 0;
	for (Json::Value award : active_award_list)
	{
		if (!award[3].asInt())
		{
			num++;
		}
	}

	if (!num)
	{
		common::log("【星辰手信】本周已领取");
		return;
	}
	else
	{
		for (int i = 1; i <= num; i++)
		{
			Json::Value data1 = this->Mod_StarLetter_Base_active();
			int result1 = data1[0].asInt();
			if (result1 == StarLetterType::SUCCESS)
			{
				common::log("【星辰手信】领取星辰手信成功！！");
			}
		}
	}
}

//============================================================================
//  星辰手信 面板信息
// {module:599, action:1, 
// request:[], 
// response:[[Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil],Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// StarLetterView.as
//				
// StarLetterData.as 40:
//			oObject.list(param1,this.starLetterData,["star_list","my_name","chat_box_id","friend_nickname","friend_role_id","friend_chat_box_id","active_award_list","letter_award_list","act_value","act_pre","ingot","ingot_pre","remain_act_times","remain_ingot_times","award_log_list"]);2090, 1000, 140, 500, 119, 506
//					oObject.list(_loc6_[_loc4_],_loc5_,["star_id","is_choose","is_active"]);
// Example
//	   未选择[ [ [ 12, 0, 1 ], [ 2, 0, 1 ], [ 4, 0, 1 ] ], "\u51af\u6d77\u68ee\u5821", 7857, "\u51af\u8bfa\u4f9d\u66fc", 5, 0, null, [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2140, 1000, 0, 500, 132, 466, [ [ 7089, 1 ], [ 1844, 1 ], [ 6499, 1 ], [ 1787, 5 ], [ 4954, 1 ], [ 1844, 1 ], [ 1787, 5 ], [ 6499, 1 ], [ 5823, 1 ], [ 6759, 1 ] ] ] 
//     [ [ [ 5, 0, 1 ], [ 10, 1, 1 ], [ 2, 0, 1 ] ], "\u51af\u8bfa\u4f9d\u66fc", 8123, "\u51af\u6d77\u68ee\u5821", 6, 0, [ [ 1, 7089, 1, 1 ], [ 7, 3054, 2, 1 ], [ 5, 6759, 1, 1 ], [ 6, 4954, 1, 1 ], [ 3, 5104, 2, 1 ], [ 4, 6499, 1, 1 ], [ 2, 1787, 5, 1 ] ], [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2145, 1000, 0, 500, 116, 498, [ [ 6499, 1 ], [ 6759, 1 ], [ 4954, 1 ], [ 6499, 1 ], [ 5104, 2 ], [ 3054, 2 ], [ 7089, 1 ], [ 4954, 1 ], [ 1787, 5 ], [ 6759, 1 ] ] ] 
//	   已领取[ [ [ 5, 0, 1 ], [ 3, 0, 1 ], [ 8, 1, 1 ] ], "\u5929\u624d\u5e05\u5e05.s51", 8120, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51", 507, 0, [ [ 1, 7089, 1, 1 ], [ 2, 4954, 1, 1 ], [ 5, 6759, 1, 1 ], [ 3, 3054, 2, 1 ], [ 4, 1844, 1, 1 ], [ 6, 5104, 2, 1 ] ], [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2120, 1000, 0, 500, 105, 496, [ [ 5117, 10 ], [ 3050, 200 ], [ 2343, 50 ], [ 3050, 200 ], [ 7089, 1 ], [ 4954, 1 ], [ 5104, 2 ], [ 1844, 1 ], [ 3054, 2 ], [ 6759, 1 ] ] ] 
//	   未领取[ [ [ 7, 1, 1 ], [ 5, 0, 1 ], [ 3, 0, 1 ] ], "\u51af\u8bfa\u4f9d\u66fc", 8123, "\u51af\u6d77\u68ee\u5821", 6, 0, [ [ 1, 5823, 1, 0 ], [ 5, 6638, 3, 0 ], [ 6, 6759, 1, 0 ], [ 3, 6499, 1, 0 ], [ 4, 4954, 1, 0 ], [ 2, 1787, 5, 0 ] ], [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2090, 1000, 140, 500, 119, 506, [ [ 7089, 1 ], [ 4954, 1 ], [ 1787, 5 ], [ 6759, 1 ], [ 3706, 200 ], [ 5117, 10 ], [ 347, 200 ], [ 5117, 10 ], [ 5117, 10 ], [ 5183, 10 ] ] ] 
//	   领两次[ [ [ 7, 1, 1 ], [ 5, 0, 1 ], [ 3, 0, 1 ] ], "\u51af\u8bfa\u4f9d\u66fc", 8123, "\u51af\u6d77\u68ee\u5821", 6, 0, [ [ 1, 5823, 1, 1 ], [ 5, 6638, 3, 0 ], [ 6, 6759, 1, 0 ], [ 3, 6499, 1, 1 ], [ 4, 4954, 1, 0 ], [ 2, 1787, 5, 0 ] ], [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2090, 1000, 140, 500, 117, 506, [ [ 1787, 5 ], [ 6759, 1 ], [ 3706, 200 ], [ 5117, 10 ], [ 347, 200 ], [ 5117, 10 ], [ 5117, 10 ], [ 5183, 10 ], [ 5823, 1 ], [ 6499, 1 ] ] ] 
//	   领三次[ [ [ 7, 1, 1 ], [ 5, 0, 1 ], [ 3, 0, 1 ] ], "\u51af\u8bfa\u4f9d\u66fc", 8123, "\u51af\u6d77\u68ee\u5821", 6, 0, [ [ 1, 5823, 1, 1 ], [ 5, 6638, 3, 0 ], [ 6, 6759, 1, 0 ], [ 3, 6499, 1, 1 ], [ 4, 4954, 1, 1 ], [ 2, 1787, 5, 0 ] ], [ [ 5117, 10 ], [ 3050, 200 ], [ 2397, 15 ], [ 5183, 10 ], [ 3706, 200 ], [ 1740, 30 ], [ 4982, 1 ], [ 1411, 5 ], [ 347, 200 ], [ 2343, 50 ] ], 2090, 1000, 140, 500, 116, 506, [ [ 6759, 1 ], [ 3706, 200 ], [ 5117, 10 ], [ 347, 200 ], [ 5117, 10 ], [ 5117, 10 ], [ 5183, 10 ], [ 5823, 1 ], [ 6499, 1 ], [ 4954, 1 ] ] ] 
// 			1-5 四个；6 五个；7-8 六个；9-11 七个；12 六个
//			
//============================================================================
Json::Value sxd_client::Mod_StarLetter_Base_star_letter_info()
{
	Json::Value data;
	return this->send_and_receive(data, 599, 1);
}

//============================================================================
//  星辰手信 选择星辰
// {module:599, action:2, 
// request:[Utils.IntUtil],, 
// response:[Utils.UByteUtil]
// 
// StarLetterView.as
//				_data.call(Mod_StarLetter_Base.choose_star,responseCallback,[starId]);
// StarLetterData.as 40:
//				oObject.list(this._aryGainAward,objReturn,["result","award_list"]);
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_StarLetter_Base_choose_star(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 599, 2);
}

//============================================================================
//  星辰手信 激活星辰
// {module:599, action:3, 
// request:[],, 
// response:[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// StarLetterView.as
// 
// StarLetterData.as 40:
//				oObject.list(this._aryGainAward,objReturn,["result","award_list"]);
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_StarLetter_Base_active()
{
	Json::Value data;
	return this->send_and_receive(data, 599, 3);
}

//============================================================================
//  星辰手信 收取星辰手信
// {module:599, action:4, 
// request:[],, 
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// StarLetterView.as
// 
// StarLetterData.as 40:
//				oObject.list(this._aryGainAward,objReturn,["result","award_list"]);
// Example
//     [ 32, 1, 3, 5, 14565, 500, 31 ]
//============================================================================
Json::Value sxd_client::Mod_StarLetter_Base_receive_letter()
{
	Json::Value data;
	return this->send_and_receive(data, 599, 4);
}

//圣盟祭祀
void sxd_client::SaUnionWorship()
{
	Json::Value data = this->Mod_SaUnionWorship_Base_open_panel();
	data = this->Mod_SaUnionWorship_Base_worship(0);
	if (data[0].asInt() == SaUnionWorshipType::SUCCESS)
	{
		common::log("【圣盟祭祀】祭祀成功！！");
	}
	else if (data[0].asInt() == SaUnionWorshipType::NO_TIME)
	{
		common::log("【圣盟祭祀】次数不足！！", 0);
	}
}

//============================================================================
//  圣盟祭祀 面板信息
// {module:419, action:1, 
// request:[], 
// 
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// SaUnionWorshipData.as 40:
/*       this.info.left_coin_times = param1[_loc2_++];
         this.info.left_ingot_times = param1[_loc2_++];
         this.info.total_times = param1[_loc2_++];
         this.info.coin_cost = param1[_loc2_++];
         this.info.ingot_cost = param1[_loc2_++];
         this.info.coin_add_times = param1[_loc2_++];
         this.info.ingot_add_times = param1[_loc2_++];
         this.info.coin_exp = param1[_loc2_++];
         this.info.ingot_exp = param1[_loc2_++];
         this.info.coin_contribute = param1[_loc2_++];
         this.info.ingot_contribute = param1[_loc2_++];
*/
// Example
//     [ 1, 2, 18, 5000000, 20, 1, 2, 200, 300, 100, 200 ] 
//============================================================================
Json::Value sxd_client::Mod_SaUnionWorship_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 419, 1);
}

//============================================================================
//  圣盟祭祀 祭祀
// {module:419, action:2, 
// request:[], 
// 
// SaUnionWorshipView.as
//			 _data.call(Mod_SaUnionWorship_Base.worship,worshipBack,[type],true,2);
// 
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// SaUnionWorshipData.as 40:
//				oObject.list(this._aryGainAward,objReturn,["result","award_list"]);
// Example
//     [ 2, [ [ 3050, 80 ], [ 3706, 80 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_SaUnionWorship_Base_worship(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 419, 2);
}

//兵法
void sxd_client::Tactics()
{
	Json::Value data = this->Mod_Tactics_Base_get_tactics_info();
	Json::Value data_list = data[2];

	for (Json::Value temp : data_list)
	{
		int box_type = temp[0].asInt();
		if (box_type == TacticsType::XIAO_SHENG_SHUI)
		{
			int buy_num = temp[4].asInt();
			for (int i = buy_num; i< 5; i++)
			{
				data = this->Mod_Tactics_Base_buy_item(TacticsType::XIAO_SHENG_SHUI);

				int result = data[0].asInt();
				if (result == TacticsType::SUCCEED)
				{
					common::log("【兵法】购买小圣水成功！！");
				}
				else
				{
					common::log(boost::str(boost::format("【兵法】购买小圣水失败，代码【%1%】！！") % result));
					return;
				}
			}
		}
		else
		{
			continue;
		}
	}
}

//============================================================================
//  兵法 面板信息
// {module:299, action:0, 
// request:[], 
// 
// response:[Utils.IntUtil,Utils.IntUtil,[Utils.UByteUtil,Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil]],[Utils.ByteUtil,Utils.ByteUtil,Utils.UByteUtil],[Utils.UByteUtil],Utils.IntUtil]
// 
// TacticsData.as 40:
/*       this._tacticsInfo.level = param1[0];
         this._tacticsInfo.exp = param1[1];
		 for each(_loc5_ in param1[2])
		 {
			_loc2_ = {};
			_loc2_.box_type = _loc5_[0];
			_loc2_.cost_type = _loc5_[1];
			_loc2_.cost_num = _loc5_[2];
			_loc2_.sheng_shui_num = _loc5_[3];
			_loc2_.buy_num = _loc5_[4];
			_loc2_.buy_limit = _loc5_[5];
			_loc2_.list_exp = new Array();
			_loc2_.list_exp = _loc5_[6];
		for each(_loc6_ in param1[3])
		 {
			_loc2_ = {};
			_loc2_.line_num = _loc6_[0];
			_loc2_.column_num = _loc6_[1];
			_loc2_.buff_id = _loc6_[2];
			this._tacticsInfo.buff_place_list.push(_loc2_);
		 }
		 for each(_loc7_ in param1[4])
		 {
			_loc2_ = {};
			_loc2_.activate_buff_id = _loc7_[0];
			this._tacticsInfo.activate_buff_list.push(_loc2_);
		 }
*/
// Example
//     [ 120, 1200, [ [ 0, 2, 2000000, 15879, 1, 5, [ [ 50 ], [ 200 ], [ 1000 ], [ 500 ], [ 100 ] ] ], [ 1, 3, 12, 7115, 0, 20, [ [ 300 ], [ 800 ], [ 3000 ], [ 1000 ], [ 500 ] ] ] ], [ [ 6, 9, 6 ] ], null, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_Tactics_Base_get_tactics_info()
{
	Json::Value data;
	return this->send_and_receive(data, 299, 0);
}

//============================================================================
//  兵法 购买物品
// {module:299, action:2, 
// request:[Utils.UByteUtil], 
// response:[Utils.UByteUtil]
// 
// TacticsView.as
//		_data.call(Mod_Tactics_Base.buy_item,this.onBuyWaterBack,[waterType]);
// 
// TacticsData.as 40:
/*       this.info.left_coin_times = param1[_loc2_++];
*/
// Example
//     [ 1, 2, 18, 5000000, 20, 1, 2, 200, 300, 100, 200 ] 
//============================================================================
Json::Value sxd_client::Mod_Tactics_Base_buy_item(int type)
{
	Json::Value data;
	data.append(type);
	return this->send_and_receive(data, 299, 2);
}

void sxd_client::SkyPalace()
{
	Json::Value data = this->Mod_SkyPalace_Base_main_panel_info();
	int break_time = data[7].asInt();
	Json::Value medicine_list = data[9];
	int times = data[10].asInt();
	int max_times = data[11].asInt();
	if (times == max_times)
	{
		common::log("【通天道场】今日服用次数已用完！！！");
	}
	else
	{
		int num = max_times - times;
		int one = 0, two = 0, three = 0, four = 0;
		for (Json::Value medicine : medicine_list)
		{
			if (medicine[0].asInt() == 8308)
			{
				one = medicine[1].asInt();
			}
			else if (medicine[0].asInt() == 8309)
			{
				two = medicine[1].asInt();
			}
			else if (medicine[0].asInt() == 8310)
			{
				three = medicine[1].asInt();
			}
			else if (medicine[0].asInt() == 8311)
			{
				four = medicine[1].asInt();
			}
		}
		while (num > 0 && four > 0)
		{
			data = this->Mod_SkyPalace_Base_use_medicine(8311);
			int result = data[0].asInt();
			if (result == SkyPalaceType::SUCCESS)
			{
				common::log("【通天道场】服用四阶修为丹成功！！！");
				num--;
				four--;
			}
			else if (result == SkyPalaceType::NO_ENOUGTH_MEDICINE)
			{
				break;
			}
		}
		while (num > 0 && three > 0)
		{
			data = this->Mod_SkyPalace_Base_use_medicine(8310);
			int result = data[0].asInt();
			if (result == SkyPalaceType::SUCCESS)
			{
				common::log("【通天道场】服用三阶修为丹成功！！！");
				num--;
				three--;
			}
			else if (result == SkyPalaceType::NO_ENOUGTH_MEDICINE)
			{
				break;
			}
		}
		while (num > 0 && two > 0)
		{
			data = this->Mod_SkyPalace_Base_use_medicine(8309);
			int result = data[0].asInt();
			if (result == SkyPalaceType::SUCCESS)
			{
				common::log("【通天道场】服用二阶修为丹成功！！！");
				num--;
				two--;
			}
			else if (result == SkyPalaceType::NO_ENOUGTH_MEDICINE)
			{
				break;
			}
		}
		while (num > 0 && one > 0)
		{
			data = this->Mod_SkyPalace_Base_use_medicine(8308);
			int result = data[0].asInt();
			if (result == SkyPalaceType::SUCCESS)
			{
				common::log("【通天道场】服用一阶修为丹成功！！！");
				num--;
				one--;
			}
			else if (result == SkyPalaceType::NO_ENOUGTH_MEDICINE)
			{
				break;
			}
		}
	}

	data = this->Mod_SkyPalace_Base_main_panel_info();
	break_time = data[7].asInt();
	int recover_time = data[20].asInt();//突破冷却

	if (break_time == 0 && recover_time == 0)
	{
		data = this->Mod_SkyPalace_Base_break_through();
		int result = data[0].asInt();
		if (result == SkyPalaceType::SUCCESS)
		{
			common::log("【通天道场】突破成功！！！");
		}
		else if (result == SkyPalaceType::BREAK_THROUGH_FAILD)
		{
			common::log("【通天道场】突破失败！！！");
		}
	}
}

//============================================================================
//  通天道场 面板信息
// {module:646, action:1, 
// request:[l], 
// response:[Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.LongUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil]
// 
// SkyPalaceView.as
//		_data.call(Mod_Tactics_Base.buy_item,this.onBuyWaterBack,[waterType]);
// 
// SkyPalaceData.as 40:
/*       	oObject.list(param1,this.mainPanelInfo,["stage","level","max_level","practic_exp","max_practic_exp","speed","next_speed","break_time","break_prob","medicine_list","times","max_times","can_call_back_times","call_back_need_ingot","grand_total_exp","tran_times","max_tran_times","supplement_tran_times","max_supplement_tran_times","supplement_need_ingot","recover_time","recover_consume","now_consume","medicine_rate","tran_rate","is_weekend_discount","benchmark_value","mindset_bonus","mindset_bonus_exp","mindset_value","mindset_award_times","use_medicine_time_limit"]);
while(_loc4_ < this.mainPanelInfo.medicine_list.length)
		 {
			_loc5_ = {};
			_loc5_.item_id = this.mainPanelInfo.medicine_list[_loc4_][0];
			_loc5_.num = this.mainPanelInfo.medicine_list[_loc4_][1];
			_loc5_.name = ItemType.getName(_loc5_.item_id);
			_loc5_.item_url = URI.goodsNewUrl + ItemType.getItemResourceId(_loc5_.item_id) + ".png";
			_loc2_.push(_loc5_);
			_loc4_++;
		 }
*/		
//		8308:一阶 8309:二阶 8310:三阶 8311:四阶
// Example
//		["stage","level","max_level","practic_exp","max_practic_exp","speed","next_speed","break_time","break_prob","medicine_list","times","max_times","can_call_back_times","call_back_need_ingot","grand_total_exp","tran_times","max_tran_times","supplement_tran_times","max_supplement_tran_times","supplement_need_ingot","recover_time","recover_consume","now_consume","medicine_rate","tran_rate","is_weekend_discount","benchmark_value","mindset_bonus","mindset_bonus_exp","mindset_value","mindset_award_times","use_medicine_time_limit"]);
//     [ 34, 6, 16, 65392699, 54864000, 2551, 2571, 0, 35, [ [ 8308, 59 ], [ 8309, 0 ], [ 8310, 0 ], [ 8311, 0 ] ], 10, 10, 0, 20, 5605610200121, 80, 80, 0, 20, 20, 0, 0, 20465, 0, 0, 0, 70, 181, 1142, 0, 0, 0 ] 
//     [ 34, 7, 16, 10862880, 58521600, 2551, 2571, 280245, 5, [ [ 8308, 59 ], [ 8309, 0 ], [ 8310, 0 ], [ 8311, 0 ] ], 10, 10, 0, 20, 5605610200121, 80, 80, 0, 20, 20, 0, 0, 20440, 0, 0, 0, 70, 181, 1142, 0, 0, 0 ] 
// 冷却[ 34, 3, 16, 48044529, 43891200, 2628, 2649, 0, 5, [ [ 8308, 51 ], [ 8309, 1 ], [ 8310, 0 ], [ 8311, 0 ] ], 10, 10, 0, 20, 3223523429078, 80, 80, 0, 20, 20, 7170, 48, 17524, 60, 0, 0, 69, 187, 1219, 0, 0, 0 ] 
//     [ 33, 19, 20, 120053082, 104025600, 2687, 2708, 0, 35, [ [ 8308, 60 ], [ 8309, 5 ], [ 8310, 0 ], [ 8311, 1 ] ], 0, 10, 0, 20, 3025224693423, 0, 80, 0, 20, 20, 0, 0, 24096, 60, 0, 0, 68, 193, 1289, 0, 0, 0 ] 
// ============================================================================
Json::Value sxd_client::Mod_SkyPalace_Base_main_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 646, 1);
}

//============================================================================
//  通天道场 突破
// {module:646, action:2, 
// request:[], 
// response:[Utils.UByteUtil]
// 
// SkyPalaceView.as
//		_data.call(Mod_Tactics_Base.buy_item,this.onBuyWaterBack,[waterType]);
// 
// SkyPalaceData.as 40:
/*       this.info.left_coin_times = param1[_loc2_++];
*/
// Example
//     [ 1, 2, 18, 5000000, 20, 1, 2, 200, 300, 100, 200 ] 
//============================================================================
Json::Value sxd_client::Mod_SkyPalace_Base_break_through()
{
	Json::Value data;
	return this->send_and_receive(data, 646, 2);
}

//============================================================================
//  通天道场 服用修为丹
// {module:646, action:16, 
// request:[Utils.IntUtil], 
// response:[Utils.UByteUtil,Utils.LongUtil,Utils.LongUtil]
// 
// SkyPalaceView.as
//		_data.call(Mod_Tactics_Base.buy_item,this.onBuyWaterBack,[waterType]);
// 
// SkyPalaceData.as 40:
/*       this.info.left_coin_times = param1[_loc2_++];
*/
// Example
//     [ 1, 2, 18, 5000000, 20, 1, 2, 200, 300, 100, 200 ] 
//============================================================================
Json::Value sxd_client::Mod_SkyPalace_Base_use_medicine(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 646, 16);
}