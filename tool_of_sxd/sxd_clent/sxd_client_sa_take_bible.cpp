#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_SaTakeBible_Base {
public:
    static const int SUCCESS = 11;
};

static std::string protectors[] = { "", "������", "ɳ��", "��˽�", "�����", "��ɮ" };

void sxd_client::sa_take_bible() {
    // first get
    Json::Value data = this->Mod_SaTakeBible_Base_get_take_bible_info();
    Json::Value bible_info = data;
    if (bible_info[2].asInt() == bible_info[3].asInt()) {
        common::log("��ʥ��ȡ��������������", 0);
        return;
    }
    if (bible_info[6].asInt() == 0) {
        data = this->Mod_SaTakeBible_Base_refresh();
        if (data[0].asInt() != Mod_SaTakeBible_Base::SUCCESS) {
            common::log(boost::str(boost::format("��ʥ��ȡ����ˢ��ȡ��ʹ��ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("��ʥ��ȡ����ˢ��ȡ��ʹ�ߣ���� [%1%]") % protectors[data[1].asInt()]), iEdit);
    }
    // second get
    bible_info = this->Mod_SaTakeBible_Base_get_take_bible_info();
    if (bible_info[6].asInt() == 0) {
        common::log(boost::str(boost::format("��ʥ��ȡ���������쳣��can_protection[%1%]") % bible_info[6]), iEdit);
        return;
    }
    if (bible_info[5].asInt() == 0) {
        data = this->Mod_SaTakeBible_Base_start_take_bible();
        if (data[0].asInt() != Mod_SaTakeBible_Base::SUCCESS) {
            common::log(boost::str(boost::format("��ʥ��ȡ��������ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("��ʥ��ȡ������ʼ���ͣ�ȡ��ʹ�� [%1%]") % protectors[bible_info[6].asInt()]), iEdit);
    }
}

//============================================================================
// R171 ����ȡ���������
// {module:395, action:2,
// request:[],
// response:[[Utils.UByteUtil, Utils.ByteUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil], [Utils.IntUtil, Utils.StringUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil], Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.UByteUtil, Utils.StringUtil, Utils.ByteUtil, Utils.ByteUtil]}
// SaTakeBibleController.as 225:
//     _loc_2.protectionList = this.renderProtectionList(_loc_1[0]);
//     _loc_2.protectPlayerList = this.renderProtectPlayerList(_loc_1[1]);
//     **_loc_2.takeBibleTimes = _loc_1[2];
//     **_loc_2.totalTakeBibleTimes = _loc_1[3];
//     _loc_2.refreshPrice = _loc_1[4];
//     **_loc_2.takeBibleStatus = _loc_1[5];
//     **_loc_2.canProtection = _loc_1[6];
//     _loc_2.protectPlayer = _ctrl.player.removeNickNameSuffix(_loc_1[7]);
//     _loc_2.bless = _loc_1[8];
//     _loc_2.buff = _loc_1[9];
//     this.getBuff = _loc_1[9];
// Example
//     [ [ [ 1, 20, 1615000, 4, 140, 9, 1988 ], [ 2, 25, 2012500, 8, 205, 6, 2501 ], [ 3, 30, 3275000, 20, 330, 7, 1510 ], [ 4, 35, 4650000, 27, 580, 4, 1917 ], [ 5, 40, 14275500, 48, 1680, 3, 1000 ] ], [ [ 209, "\u728e.s7", 2, 2, 0 ] ],
//       0, 3, 0, 0, 0, "", 0, 0 ]
// refresh
//     [ [ [ 1, 20, 1615000, 4, 140, 9, 1988 ], [ 2, 25, 2012500, 8, 205, 6, 2501 ], [ 3, 30, 3275000, 20, 330, 7, 1510 ], [ 4, 35, 4650000, 27, 580, 4, 1917 ], [ 5, 40, 14275500, 48, 1680, 3, 1000 ] ], null,
//       0, 3, 10, 0, 1, "", 0, 3 ]
//============================================================================
Json::Value sxd_client::Mod_SaTakeBible_Base_get_take_bible_info() {
    Json::Value data;
    return this->send_and_receive(data, 395, 2);
}

//============================================================================
// R171 ˢ��ȡ��ʹ��
// {module:395, action:8,
// request:[], response:[Utils.UByteUtil, Utils.UByteUtil, Utils.ShortUtil, Utils.ByteUtil]}
// SaTakeBibleController.as 348:
//     _loc_2.msg = _loc_1[0];
//     _loc_2.newProtection = _loc_1[1];
//     _loc_2.nextPrice = _loc_1[2];
//     _loc_2.buff = _loc_1[3];
// Example
//     [ 11, 1, 10, 3 ]
//============================================================================
Json::Value sxd_client::Mod_SaTakeBible_Base_refresh() {
    Json::Value data;
    return this->send_and_receive(data, 395, 8);
}

//============================================================================
// R171 ��ʼ����
// {module:395, action:10,
// request:[], response:[Utils.UByteUtil, Utils.ByteUtil, Utils.ByteUtil]}
// SaTakeBibleController.as 428:
//     _loc_2.msg = _loc_1[0];
//     _loc_2.item_id = _loc_1[1];
//     _loc_2.max_sequence_id = _loc_1[2];
// Example
//     [ 11, 0, 1 ]
//============================================================================
Json::Value sxd_client::Mod_SaTakeBible_Base_start_take_bible() {
    Json::Value data;
    return this->send_and_receive(data, 395, 10);
}
