#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class FiveFuType	//新春集五福
{
public:
	static const int SUCCESS = 0;
	static const int NOACTIVITY = 2;
	static const int FAILED = 14;
};

void sxd_client::FiveFu()
{
	Json::Value data = this->Mod_FiveFu_Base_get_info();
    for (;;)
    {
        data = this->Mod_FiveFu_Base_open_pack();
        int result = data[0].asInt();
        if (result == FiveFuType::SUCCESS)
        {
            common::log("【新春集五福】开启福袋成功！！");
            continue;
        }
        else
        {
            common::log("【新春集五福】次数不足！！");
            break;
        }
    }

}

//============================================================================
// 新春集五福--面板信息
// "module":400,"action":0,
// "request":[],
// "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//
// FiveFuView.as:
//
// FiveFuData.as:
/*var _loc2_:* = 0;
         this.info.activity = param1[_loc2_++];
         this.info.need_activity = param1[_loc2_++];
         this.info.lottery_time = param1[_loc2_++];
         this.info.lottery_time_zero = param1[_loc2_++];
         this.info.full_fu = param1[_loc2_++];
         this.info.isget_big_pack = param1[_loc2_++];
         this.info.get_big_pack_ingot = param1[_loc2_++];
         this.info.fu_list = this.parseFuList(param1[_loc2_++]);
         this.info.get_quan_num = param1[_loc2_++];
         this.info.can_get_quan_num = param1[_loc2_++];
         this.info.get_quan_times = param1[_loc2_++];
         this.info.five_bless_total_times = CommonEnumType.getIntValue(CommonEnumType.FiveBlessTotalTimes);
         this.info.item_id = 10065;
         this.info.now_open_times_cycle = param1[_loc2_++];
*/
// Example
//			[ 0, [830, 200, 1772942399, 1772899199, 0, 0, 0, [[ 1, 1 ], [2, 0], [3, 0], [4, 4], [5, 0]], 7, 7, 10, 2]
//============================================================================
Json::Value sxd_client::Mod_FiveFu_Base_get_info()
{
	Json::Value data;
	return this->send_and_receive(data, 400, 0);
}

//============================================================================
// 新春集五福--打开福袋
// "module":400,"action":1,
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil]
//
// FiveFuView.as:
//          _data.call(Mod_ItemConvert_Base.panel_info,param1,[this._type]);
// 
// FiveFuData.as:
//		oObject.list(param1,_loc2_,["result","items","today_convert_num","cost_free_num"]);
//			oObject.list(_loc3_[_loc5_],_loc4_,["item_id","item_num"]);
// Example
//			[ 0, [ [ 7262, 198 ] ], 0, 15 ]
//			[ 0, [ [ 7263, 1129 ] ], 0, 7 ] 
//============================================================================
Json::Value sxd_client::Mod_FiveFu_Base_open_pack()
{
	Json::Value data;
	return this->send_and_receive(data, 400, 1);
}