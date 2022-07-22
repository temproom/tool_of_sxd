#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_WagerShop_Base {
public:
    static const int SUCCESS = 7;
};

//============================================================================
// ���ֳǶķ�-��ת��
//============================================================================
void sxd_client::wager_shop() {
    Json::Value data = this->Mod_WagerShop_Base_get_turntable_info();
    int left_times = data[1].asInt();
    while (left_times) {
        data = this->Mod_WagerShop_Base_turntable();
        if (data[0].asInt() != Mod_WagerShop_Base::SUCCESS) {
            common::log(boost::str(boost::format("�����ֳǶķ�����ת�̳齱ʧ�ܣ�result[%1%]") % data[0]), 0);
            return;
        }
        common::log(boost::str(boost::format("�����ֳǶķ�����ת�̳齱�����[��ҡ�%1%]") % data[2]), iEdit);
        left_times = data[3].asInt();
    }
}

//============================================================================
// R177 ��ת�����
// "module":260, "action":10, "request":[], "response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// WagerActivityData.as 154:
//     this._wagerActivityTurnTableInfo.max_times = param1[0];
//     this._wagerActivityTurnTableInfo.left_times = param1[1];
//     this._wagerActivityTurnTableInfo.gold_count = param1[2];
// Example
//     [ 3, 3, 0 ]
//============================================================================
Json::Value sxd_client::Mod_WagerShop_Base_get_turntable_info() {
    Json::Value data;
    return this->send_and_receive(data, 260, 10);
}

//============================================================================
// R177 �齱
// "module":260, "action":11, "request":[], "response":[Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// WagerActivityData.as 174:
//     this._wagerActivityTurnTableInfo.result = param1[0];
//     this._wagerActivityTurnTableInfo.award_id = param1[1];
//     this._wagerActivityTurnTableInfo.award_number = param1[2];
//     this._wagerActivityTurnTableInfo.left_times = param1[3];
//     this._wagerActivityTurnTableInfo.gold_count = param1[4];
// Example
//     [ 7, 8, 50, 2, 50 ]
//============================================================================
Json::Value sxd_client::Mod_WagerShop_Base_turntable() {
    Json::Value data;
    return this->send_and_receive(data, 260, 11);
}

