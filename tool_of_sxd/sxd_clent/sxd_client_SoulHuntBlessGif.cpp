#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class SoulHuntBlessGiftype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
};

void sxd_client::SoulHuntBlessGif()
{
	//»ñÈ¡×´Ì¬
	Json::Value data = this->Mod_SoulHuntBlessGif_Base_main_panel_info();

	int now_level = data[0].asInt();

	vector<int> awards;
	for (Json::Value award_list : data[5])
	{
		for (int i = 0; i < 2; i++)
		{
			//Ãâ·Ñ½±Àø£¬level£º1-18£»id£º1-18
			if (award_list[1][i][0].asInt() < 19 && award_list[1][i][4].asInt() != 1)
			{
				awards.push_back(award_list[1][i][0].asInt());
			}
		}	
	}

	//»ñÈ¡½±Àø
	if (!awards.size())
	{
		common::log("¡¾ÁéÓòÁÔÑý¡¿ÁéÓò¸£Ôù½±ÀøÒÑÈ«²¿ÁìÈ¡£¡£¡");
		return;
	}
	else
	{
		for (int id : awards)
		{
			int level = id;

			//µÈ¼¶²»¹»£¬Ö±½ÓÍË³ö
			if (now_level < level)
				return;

			data = this->Mod_SoulHuntBlessGif_Base_get_award(level, id);
			int result = data[0].asInt();
			if (result == SoulHuntBlessGiftype::SUCCESS)
			{
				common::log(boost::str(boost::format("¡¾ÁéÓòÁÔÑý¡¿ÁìÈ¡½±Àø[ %1% ]³É¹¦£¡£¡") % id), iEdit);
			}
			else if (result == SoulHuntBlessGiftype::FAILED)
			{
				common::log(boost::str(boost::format("¡¾ÁéÓòÁÔÑý¡¿ÁìÈ¡½±Àø[ %1% ]Ê§°Ü£¡£¡´úÂë[ %2% ]") % id % result), iEdit);
			}
		}
	}
}

//============================================================================
//	ÁéÓòÁÔÑý-ÁéÓò¸£Ôù£ºÏêÏ¸ÐÅÏ¢
// "module":672,"action":0,
// "request":[],
//
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil]]]
//		
// soulhuntblessdata.as
//		 oObject.list(list,this._panelData,["now_level","today_charge_ingot","need_charge_ingot","is_active","up_level","award_list"]);
//			award_list,["level","item_list"],function(param1:Object):void
//				item_list, ["award_id", "item_id", "item_num", "is_free", "is_get"], parseItem);
// Example
//			 [ 19, 0, 980, 0, 23, [ [ 1, [ [ 1, 2016, 3, 1, 1 ], [ 19, 8643, 5, 0, 0 ] ] ], [ 2, [ [ 20, 1411, 10, 0, 0 ], [ 2, 7924, 15, 1, 1 ] ] ], [ 3, [ [ 3, 347, 300, 1, 1 ], [ 21, 8643, 5, 0, 0 ] ] ], [ 4, [ [ 22, 1411, 10, 0, 0 ], [ 4, 2016, 3, 1, 1 ] ] ], [ 5, [ [ 5, 7924, 15, 1, 1 ], [ 23, 8643, 5, 0, 0 ] ] ], [ 6, [ [ 37, 2011, 1, 0, 0 ], [ 6, 7892, 5, 1, 1 ], [ 24, 8359, 5, 0, 0 ] ] ], [ 7, [ [ 7, 347, 300, 1, 1 ], [ 25, 8643, 5, 0, 0 ] ] ], [ 8, [ [ 26, 1411, 10, 0, 0 ], [ 8, 2016, 3, 1, 1 ] ] ], [ 9, [ [ 27, 8643, 5, 0, 0 ], [ 9, 7924, 15, 1, 1 ] ] ], [ 10, [ [ 28, 1411, 10, 0, 0 ], [ 10, 347, 300, 1, 1 ] ] ], [ 11, [ [ 11, 2016, 3, 1, 1 ], [ 29, 8643, 5, 0, 0 ] ] ], [ 12, [ [ 12, 8643, 5, 1, 1 ], [ 38, 2013, 1, 0, 0 ], [ 30, 8359, 5, 0, 0 ] ] ], [ 13, [ [ 13, 7924, 15, 1, 1 ], [ 31, 8643, 5, 0, 0 ] ] ], [ 14, [ [ 32, 1411, 10, 0, 0 ], [ 14, 347, 300, 1, 1 ] ] ], [ 15, [ [ 33, 8643, 5, 0, 0 ], [ 15, 7892, 5, 1, 1 ] ] ], [ 16, [ [ 16, 2016, 3, 1, 1 ], [ 34, 1411, 10, 0, 0 ] ] ], [ 17, [ [ 17, 7924, 15, 1, 1 ], [ 35, 8643, 5, 0, 0 ] ] ], [ 18, [ [ 18, 4431, 1, 1, 1 ], [ 39, 2012, 1, 0, 0 ], [ 36, 8365, 1, 0, 0 ] ] ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_SoulHuntBlessGif_Base_main_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 672, 0);
}
//============================================================================
//	ÁéÓòÁÔÑý-ÁéÓò¸£Ôù£º»ñÈ¡½±Àø
// "module":672,"action":2,
// "request":[Utils.IntUtil,Utils.IntUtil],
// 
//	SoulHuntBlessview.as
//          this._level = param1;
//			this._id = param2;
//		_data.call(Mod_SoulHuntBlessGift_Base.get_award,this.get_award_callback,[param1,param2]);
// "response":[Utils.UByteUtil]
//		
// Example
//			
//============================================================================
Json::Value sxd_client::Mod_SoulHuntBlessGif_Base_get_award(int level, int id)
{
	Json::Value data;
	data.append(level);
	data.append(id);
	return this->send_and_receive(data, 672, 2);
}