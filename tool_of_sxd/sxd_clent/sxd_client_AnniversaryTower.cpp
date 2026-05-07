#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//µÇÏÉÂ¥
class AnniversaryTowertype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int ACT_NOT_OPEN = 2;	
	static const int TODAY_TIMES_LIMIT = 11;
};

void sxd_client::AnniversaryTower()
{
	Json::Value data = this->Mod_AnniversaryTower_Base_open_panel();

	//Json::Value buy_gift = data[11];

	data = this->Mod_AnniversaryTower_Base_buy_gift(1);
	if(data[0].asInt() == AnniversaryTowertype::SUCCESS)
	{
		common::log("¡¾µÇÏÉÂ¥¡¿£ºÁìÈ¡Ãâ·ÑÀñ°ü³É¹¦£¡£¡");
	}
	else if (data[0].asInt() == AnniversaryTowertype::TODAY_TIMES_LIMIT)
	{
		common::log("¡¾µÇÏÉÂ¥¡¿£º½ñÈÕÃâ·ÑÀñ°üÒÑÁìÈ¡£¡£¡");
	}
	/*data = this->Mod_AnniversaryTower_Base_buy_gift(2);
	if (data[0].asInt() == AnniversaryTowertype::SUCCESS)
	{
		common::log("¡¾µÇÏÉÂ¥¡¿£º¹ºÂòÀñ°ü1³É¹¦£¡£¡");
	}
	else if (data[0].asInt() == AnniversaryTowertype::TODAY_TIMES_LIMIT)
	{
		common::log("¡¾µÇÏÉÂ¥¡¿£ºÀñ°ü1ÒÑ¹ºÂò£¡£¡");
	}*/
}

//============================================================================
// µÇÏÉÂ¥£º´ò¿ªÃæ°å
// {module:685, action:1, 
// request:[],
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
// 
// AnniversaryTowerData.as
//     oObject.list(param1,this.info,["result","self_value","exceed_value","coin_num","role_id","head_id","rim_id","normal_get_list","ingot_get_list","extra_get_list","exchange_awards","buy_gift","fireworks_list","exchange_res_nun","today_charge_ingot"]);
// 
// Example
//     [ 0, 420, 0, 52, 207, 12, 8120, [ [ 1, 1 ], [ 2, 1 ], [ 3, 1 ], [ 4, 1 ] ], null, null, null, [ [ 1, 1, 1 ] ], [ [ 8788, 1, 20 ], [ 8787, 0, 0 ] ], 420, 0 ] 
// buy_gift:
//		oObject.list(this.info.buy_gift[_loc5_],_loc18_,["id","today_buy_times","total_buy_times"]);
//============================================================================
Json::Value sxd_client::Mod_AnniversaryTower_Base_open_panel()
{
	Json::Value data;
	return this->send_and_receive(data, 685, 1);
}

//============================================================================
// µÇÏÉÂ¥£º»ñÈ¡½±Àø
// {module:685, action:2, 
// request:[Utils.IntUtil,Utils.IntUtil],
// 
//	AnniversaryTowerview.as
//			_data.call(Mod_AnniversaryTower_Base.get_award,get_award_callback,[data.floor_info.id,type]);
// 
// response:[Utils.UByteUtil]
// 
// AnniversaryTowerData.as
//     this.result = param1[0];
// 
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_AnniversaryTower_Base_get_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 685, 2);
}

//============================================================================
// µÇÏÉÂ¥£º¹ºÂòÀñ°ü
// {module:685, action:5, 
// request:[Utils.IntUtil],
// 
// AnniversaryTowerview.as
//		_data.call(Mod_AnniversaryTower_Base.buy_gift,this.buy_gift_call_back,[param1.id]);
//		
// response:[Utils.UByteUtil]
// 
// AnniversaryTowerData.as
//     this.result = param1[0];
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_AnniversaryTower_Base_buy_gift(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 685, 5);
}