#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_SaTrials_Base
{
public:
	static const int SUCCESS = 0;
	static const int FAILD = 3;
	static const int FAIL = 17;

	static const int SA_TRIALS_OPEN = 18;
	static const int SA_TRIALS_CLOSE = 19;
	static const int SA_TRIALS_FINISH = 20;
};

void sxd_client::sa_trials()
{
	Json::Value data = this->Mod_SaTrials_Base_get_panel_info();
	Sleep(2000);
	data = this->Mod_SaTrials_Base_challage();
	while (data[0].asInt() != Mod_SaTrials_Base::SUCCESS)
	{
		common::log(boost::str(boost::format("【圣域秘境】挑战失败，result[%1%]") % data[0]), iEdit);
		Sleep(2000);
		data = this->Mod_SaTrials_Base_challage();
	}
	if (data[0].asInt() == Mod_SaTrials_Base::SUCCESS)
	{
		common::log("【圣域秘境】挑战成功！", 0);
	}
	Json::Value box = this->Mod_SaTrials_Base_open_box();
	if (data[0].asInt() == Mod_SaTrials_Base::SUCCESS)
	{
		common::log("【圣域秘境】打开盒子成功!\n\t 1:, \n\t2:\n\t请选择奖励:", 0);
		int box_type = data[1].asInt();
		int buff_id = data[2].asInt();
		int special_buff_list;
	}
}

//============================================================================
//  圣域秘境面板
// {module:511, action:0, 
// "request":[],
// "response": [Utils.UByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.StringUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil], [Utils.IntUtil, Utils.IntUtil], [Utils.IntUtil, Utils.ByteUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil]], Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil]
// SaTrialsData.as 70:
//    var _loc2_:int = 0;
//	this.info.result = param1[_loc2_++];
//	if (this.info.result == Mod_SaTrials_Base.SUCCESS)
//	{
//    this.info.missionInfo = this.parseMissionInfo(param1[_loc2_++], param1[_loc2_++]);
//    this.info.defenderInfo = this.parseDeffenderInfo(param1[_loc2_++], param1[_loc2_++], param1[_loc2_++]);
//    this.info.reset_cost_ingot = param1[_loc2_++];
//    this.info.buff_item_list = this.parseBuffItemList(param1[_loc2_++]);
//    this.info.special_buff_list = this.parseSpecialBuffList(param1[_loc2_++]);
//    this.info.finish_mission_list = this.parseFinishMissionList(param1[_loc2_++]);
//    this.info.reset_num = param1[_loc2_++];
//    this.info.free_reset_count = param1[_loc2_++];
//    this.info.finish_num = param1[_loc2_++];
//    this.info.is_set_auto_fight = param1[_loc2_++] == 1;
//	}
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_get_panel_info()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 0);
}

//============================================================================
//  打开盒子
// {module:511, action:3, 
// "request":[],
// "response":[Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil]]
// SaTrialsData.as 267:
//   var _loc3_:int = 0;
//	var _loc2_ : int = 0;
//	this.boxInfo.result = param1[_loc2_++];
//	if (this.boxInfo.result == Mod_SaTrials_Base.SUCCESS)
//	{
//	this.boxInfo.awardBox = {};
//	this.boxInfo.awardBox.box_type = param1[_loc2_++];
//	_loc3_ = param1[_loc2_++];
//	if (_loc3_ > 0)
//	{
//		this.boxInfo.awardBox.buffItem = { "id":_loc3_ };
//		this.addBuffItemInfo(this.boxInfo.awardBox.buffItem);
//	}
//	this.boxInfo.awardBox.special_buff_list = this.parseSpecialBuffList2(param1[_loc2_++]);
//	}
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_open_box()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 3);
}

//============================================================================
//  选择盒子
// {module:511, action:4, 
// "request":[Utils.ByteUtil],
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil]]
// SaTrialsData.as 291:
//   var _loc3_:int = 0;
//	var _loc2_ : int = 0;
//	this.boxInfo.result = param1[_loc2_++];
//	if (this.boxInfo.result == Mod_SaTrials_Base.SUCCESS)
//	{
//	_loc3_ = param1[_loc2_++];
//	if (_loc3_ > 0)
//	{
//		this.boxInfo.awardBox.buffItem = { "id":_loc3_ };
//		this.addBuffItemInfo(this.boxInfo.awardBox.buffItem);
//	}
//	this.boxInfo.awardBox.special_buff_list = this.parseSpecialBuffList2(param1[_loc2_++]);
//	}
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_choose_box_type()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 4);
}

//============================================================================
//  获取阶段奖励
// {module:511, action:6, 
// "request":[],
// "response":[Utils.UByteUtil,[Utils.IntUtil]]
// SaTrialsData.as 319:
//  var _loc2_:Array = null;
//var _loc3_ : int = 0;
//var _loc4_ : Object = null;
//var _loc5_ : Object = null;
//this.boxInfo.result = param1[0];
//if (this.boxInfo.result == Mod_SaTrials_Base.SUCCESS)
//{
//    this.boxInfo.stepAwardList = [];
//    for each (_loc2_ in param1[1])
//    {
//        _loc3_ = _loc2_[0];
//        _loc4_ = {};
//        _loc5_ = SaTrialsType.getStepAward(_loc3_);
//        _loc4_.awardId = _loc3_;
//        _loc4_.itemId = _loc5_.item_id;
//        _loc4_.itemUrl = URI.goodsIconHugeWithoutBgUrl + _loc4_.itemId + ".png";
//        _loc4_.itemNum = _loc5_.item_number;
//        _loc4_.itemName = ItemType.getName(_loc4_.itemId);
//        _loc4_.itemTip = this.getItemTip(_loc4_.itemId);
//        this.boxInfo.stepAwardList.push(_loc4_);
//    }
//}
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_gain_step_award()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 6);
}

//============================================================================
//  选择阶段奖励
// {module:511, action:7, 
// "request":[Utils.IntUtil,Utils.ByteUtil],
// "response":[Utils.UByteUtil]
// SaTrialsData.as 352:
//  this.boxInfo.result = param1[0];
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_choose_step_award()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 7);
}

//============================================================================
//  挑战
// {module:511, action:8, 
// "request":[],
// "response":[Utils.UByteUtil,[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]]],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil],[Utils.IntUtil,Utils.IntUtil]]
// SaTrialsData.as 242:
//  var _loc2_:int = 0;
//this.info.warResult = param1[_loc2_++];
//if (this.info.warResult == Mod_SaTrials_Base.SUCCESS)
//{
//	this.info.war_result = param1[_loc2_++];
//	this.info.award_item_list = param1[_loc2_++];
//}
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_SaTrials_Base_challage()
{
	Json::Value data;
	return this->send_and_receive(data, 511, 8);
}