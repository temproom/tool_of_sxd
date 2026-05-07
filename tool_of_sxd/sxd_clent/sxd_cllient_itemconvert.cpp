#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class ItemConvertType	//无量劫海挑战
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int CONVERT_LIMIT = 4;
	static const int RESCHUJI = 3;
	static const int RESZHONGJI = 4;
	static const int RESGAOJI = 5;
};
void sxd_client::itemconvert()
{
	//初级资源包
	Json::Value data = this->Mod_ItemConvert_Base_panel_info(3);
	int cost_free_num = data[3].asInt();
	if (cost_free_num < 15)
	{
		int num = 15 - cost_free_num;
		Json::Value data_convert = this->Mod_ItemConvert_Base_convert(3, 7262, num);
		int result = data_convert[0].asInt();
		if (result == ItemConvertType::SUCCESS)
		{
			common::log(boost::str(boost::format("【物品转换】：成功转换[初级资源包]:【%1%】个！！") % num));
		}
	}
	//中级资源包
	data = this->Mod_ItemConvert_Base_panel_info(4);
	cost_free_num = data[3].asInt();
	if (cost_free_num < 7)
	{
		int num = 7 - cost_free_num;
		Json::Value data_convert = this->Mod_ItemConvert_Base_convert(4, 7263, num);
		int result = data_convert[0].asInt();
		if (result == ItemConvertType::SUCCESS)
		{
			common::log(boost::str(boost::format("【物品转换】：成功转换[中级资源包]:【%1%】个！！") % num));
		}
	}
	//高级资源包
	data = this->Mod_ItemConvert_Base_panel_info(5);
	cost_free_num = data[3].asInt();
	if (cost_free_num < 3)
	{
		int num = 3 - cost_free_num;
		Json::Value data_convert = this->Mod_ItemConvert_Base_convert(5, 7264, num);
		int result = data_convert[0].asInt();
		if (result == ItemConvertType::SUCCESS)
		{
			common::log(boost::str(boost::format("【物品转换】：成功转换[高级资源包]:【%1%】个！！") % num));
		}
	}
}
//============================================================================
// 物品转换--面板信息
// "module":844,"action":0,
// "request":[Utils.IntUtil],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
//
// ItemConvertView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// ItemConvertData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_ItemConvert_Base_panel_info(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 844, 0);
}

//============================================================================
// 物品转换--面板信息
// "module":844,"action":1,
// "request":[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],
// "response":[Utils.UByteUtil]
//
// ItemConvertReschooseView.as:
//          _data.call(Mod_ItemConvert_Base.convert,convertCallback,[type,arr]);
// ItemConvertView.as:
//			_loc2_.push([_loc3_.item_id,_loc3_.item_num]);
//			_data.call(Mod_ItemConvert_Base.convert,this.convert_callbcak,[this._type,_loc2_]);
// 
// type:
//		1:材料。2:制作卷。3:初级 id:7262。4:中级 id:7263。5:高级 id:7264。
// ItemConvertData.as:
//		oObject.list(param1,this._objEraInfinite,["era_level","mission_level","rank_perc","rank","fight_end_ts","era_zone"]);
// 
// Example
//			[ 81, 30, 0, 0, 1772466900, 1 ]
//			[ 93, 42, 0, 0, 1772466900, 2 ]
//============================================================================
Json::Value sxd_client::Mod_ItemConvert_Base_convert(int type, int id, int num)
{
	Json::Value data, data1;
	data.append(type);

	Json::Value data2;
	data2.append(id);
	data2.append(num);
	data1.append(data2);

	data.append(data1);
	return this->send_and_receive(data, 844, 1);
}