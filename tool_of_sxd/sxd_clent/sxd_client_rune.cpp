#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Rune_Base {
public:
    static const int SUCCEED = 0;
};

//============================================================================
// �в����
//============================================================================
void sxd_client::rune() {
    auto data = this->Mod_Player_Base_get_player_info();
    int my_ingot = data[2].asInt();
    if (my_ingot < 2) {
        common::log("���в������ʹ��ʧ�ܣ�Ԫ������ [2]", iEdit);
        return;
    }
    data = this->Mod_Rune_Base_rune_list();
    int count = data[0].asInt();
    int ingot = data[1].asInt();
    if (count && ingot == 2) {
        data = this->Mod_Rune_Base_rune_use();
        if (data[0].asInt() != Mod_Rune_Base::SUCCEED) {
            common::log(boost::str(boost::format("���в������ʹ��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("���в���������� [Ԫ����%1%]����� [ͭǮ��%2%]") % ingot % data[1]), iEdit);
    }
}

//============================================================================
// R180 ���
// "module":16,"action":1,"request":[],"response":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// RuneController.as:
//     _loc2_.count = _loc1_[0];
//     _loc2_.ingot = _loc1_[1];
//     _loc2_.coins = _loc1_[2];
// Example
//     [ 1, 2, 800000 ]
//============================================================================
Json::Value sxd_client::Mod_Rune_Base_rune_list() {
    Json::Value data;
    return this->send_and_receive(data, 16, 1);
}

//============================================================================
// R180 ʹ��
// "module":16,"action":2,"request":[],"response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// RuneController.as:
//     _loc2_.msg = _loc1_[0];
//     _loc2_.getCoins = _loc1_[1];
//     _loc2_.count = _loc1_[2];
//     _loc2_.ingot = _loc1_[3];
//     _loc2_.coins = _loc1_[4];
//     _loc2_.crit = _loc1_[5];
// Example
//     [ 0, 800000, 0, 4, 800000, 10 ]
//============================================================================
Json::Value sxd_client::Mod_Rune_Base_rune_use() {
    Json::Value data;
    return this->send_and_receive(data, 16, 2);
}
