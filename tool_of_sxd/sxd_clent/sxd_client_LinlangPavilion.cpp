#include "stdafx.h"
#include <vector>
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class LinlangPavilionType
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 1;
	static const int IS_BUY = 5;
	static const int ITEM_EMPTY = 10;
};

void sxd_client::LinlangPavilion()
{
	Json::Value test = this->Mod_LinlangPavilion_Base_one_key_buy_panel(2);

	Json::Value data = this->Mod_LinlangPavilion_Base_one_key_buy();
	if (data[0].asInt() == LinlangPavilionType::SUCCESS)
	{
		common::log("¡¾ÁÕÀÅ¸ó¡¿£ºÒ»¼ü¹ºÂò³É¹¦£¡");
	}
	else if (data[0].asInt() == LinlangPavilionType::ITEM_EMPTY)
	{
		common::log("¡¾ÁÕÀÅ¸ó¡¿£ºÒÑ¹ºÂò£¡");
	}

	data = this->Mod_LinlangPavilion_Base_open_panel();

	int free_refresh_count = data[7].asInt();

	while (free_refresh_count)
	{
		common::log(boost::str(boost::format("¡¾ÁÕÀÅ¸ó¡¿µ±Ç°Ê£ÓàÃâ·ÑË¢ÐÂ´ÎÊý£º[%1%] ") % free_refresh_count), 0);
		data = this->Mod_LinlangPavilion_Base_refresh_item();
		if (data[0].asInt() == LinlangPavilionType::SUCCESS)
		{
			common::log("¡¾ÁÕÀÅ¸ó¡¿£ºË¢ÐÂ³É¹¦£¡");
		}
		test = this->Mod_LinlangPavilion_Base_one_key_buy_panel(2);
		data = this->Mod_LinlangPavilion_Base_one_key_buy();
		if (data[0].asInt() == LinlangPavilionType::SUCCESS)
		{
			common::log("¡¾ÁÕÀÅ¸ó¡¿£ºÒ»¼ü¹ºÂò³É¹¦£¡");
		}
		data = this->Mod_LinlangPavilion_Base_open_panel();

		free_refresh_count = data[7].asInt();
	}
}

//============================================================================
//  ÁÕÀÅ¸ó-Ãæ°å
// {module:651, action:0,
// request:[],
// Example		
//     [  ]
// response:[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// 
// Example
// LinlangPavilionData.as
//    oObject.list(param1,this.info,["tong_tian_tower_heaven","item_list","lin_lang_ling","refresh_time","is_lucky_day","lucky_day_times","npc_friendship","free_refresh_count","ingot_refresh_count","free_expire_time"]);
//			[ 9, [ [ 92, 365, 0 ], [ 719, 2873, 1 ], [ 670, 2677, 1 ], [ 84, 334, 0 ], [ 83, 329, 0 ], [ 85, 338, 0 ], [ 646, 2581, 1 ], [ 89, 355, 0 ], [ 650, 2597, 0 ], [ 647, 2585, 0 ], [ 676, 2701, 1 ], [ 708, 2829, 0 ], [ 622, 2485, 0 ], [ 93, 370, 0 ], [ 737, 2946, 1 ], [ 720, 2878, 0 ], [ 636, 2542, 0 ], [ 639, 2554, 1 ] ], 829, 50393, 0, 0, 1, 0, 5, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_LinlangPavilion_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 651, 0);
}

//============================================================================
//  ÁÕÀÅ¸ó-Ë¢ÐÂ
// {module:651, action:2,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil]
// 
// Example
// LinlangPavilionData.as
//     this.result = param1[0];
//			[ 2, 1736870400, 1737734399, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_LinlangPavilion_Base_refresh_item()
{
	Json::Value data;
	return this->send_and_receive(data, 651, 2);
}

//============================================================================
//  ÁÕÀÅ¸ó-Ò»¼ü¹ºÂò
// {module:651, action:3,
// request:[],
// Example		
//     [  ]
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// Example
// LinlangPavilionData.as
//     this.result = param1[0];
//			[ 2, 1736870400, 1737734399, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_LinlangPavilion_Base_one_key_buy()
{
	Json::Value data;
	return this->send_and_receive(data, 651, 3);
}

//============================================================================
//  ÁÕÀÅ¸ó-Ò»¼ü¹ºÂòÃæ°å
// {module:651, action:4,
// request:[Utils.ByteUtil],,
// Example		
// _data.call(Mod_LinlangPavilion_Base.one_key_buy_panel,param1,[this._indexPage]);
//     [  ]
// response:[[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]]
// Example
// LinlangPavilionData.as
//     oObject.list(param1,this._oneKeyBuyPanel,["discount_list","setting_list"]);
//			[ [ [ 1 ], [ 40 ], [ 60 ], [ 90 ], [ 100 ] ], [ [ 8319, 0 ], [ 8320, 0 ], [ 8318, 0 ], [ 8325, 1 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_LinlangPavilion_Base_one_key_buy_panel(int page)
{
	Json::Value data;
	data.append(page);
	return this->send_and_receive(data, 651, 4);
}