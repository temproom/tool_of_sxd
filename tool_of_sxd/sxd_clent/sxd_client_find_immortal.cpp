#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_FindImmortal_Base {
public:
    static const int SUCCESS = 0;
    static const int YES = 12;
};

//============================================================================
// R179 ϲ���콵
//============================================================================
void sxd_client::find_immortal() {
    for (;;) {
        auto data = this->Mod_FindImmortal_Base_open_find_immortal();
        int remain_number = data[0].asInt();
        int state = data[6].asInt();
        if (state) {
            data = this->Mod_FindImmortal_Base_pickup_award();
            if (data[0].asInt() != Mod_FindImmortal_Base::SUCCESS) {
                common::log(boost::str(boost::format("��ϲ���콵�����þ���ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            common::log(boost::str(boost::format("��ϲ���콵�����þ��գ���� [������%1%]��[������%2%]��[�����%3%]") % data[1] % data[2] % data[3]), iEdit);
        }
        if (!remain_number)
            return;
        data = this->Mod_FindImmortal_Base_start_find_immortal();
        if (data[0].asInt() != Mod_FindImmortal_Base::SUCCESS) {
            common::log(boost::str(boost::format("��ϲ���콵��������ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log("��ϲ���콵��������", iEdit);
    }
}

//============================================================================
// R179 ���
// "module":68,"action":1,"request":[],"response":[Utils.ShortUtil,Utils.ShortUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]
// FindImmortalData.as:
//     _loc1_.can_buy_number = this._open[0];
//       _loc2_.fame = param1[1];
//       _loc2_.skill = param1[2];
//       _loc2_.cultivation = param1[3];
//       _loc2_.statePoint = param1[4];
//       _loc2_.yuPai = param1[5];
//     _loc1_.state = this._open[6];
//     _loc1_.imgId = this._open[7];
//     _loc1_.cultivation = this._open[8];
//     _loc1_.total_count = this._open[9];
// Example
//     [ 20, 0, 0, 0, 0, 0, 0, 0, 556, 1075 ]
// �������
//     [ 19, 0, 1500, 0, 0, 0, 1, 1, 556, 1076 ]
// ���þ��պ�
//     [ 19, 0, 0, 0, 0, 0, 0, 0, 556, 1076 ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_open_find_immortal() {
    Json::Value data;
    return this->send_and_receive(data, 68, 1);
}

//============================================================================
// R179 ������
// "module":68,"action":2,"request":[],"response":[Utils.UByteUtil,Utils.ShortUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.IntUtil]
// FindImmortalData.as:
//     _loc1_.type = this._start[0];
//       _loc2_.fame = param1[1];
//       _loc2_.skill = param1[2];
//       _loc2_.cultivation = param1[3];
//       _loc2_.statePoint = param1[4];
//       _loc2_.yuPai = param1[5];
//     _loc1_.state = this._start[6];
//     _loc1_.imgId = this._start[7];
//     _loc1_.remain_number = this._start[8];
//     _loc1_.total_count = this._start[9];
// Example
//     [ 0, 20, 1500, 2, 0, 0, 1, 3, 18, 1081 ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_start_find_immortal() {
    Json::Value data;
    return this->send_and_receive(data, 68, 2);
}

//============================================================================
// R179 ���þ���
// "module":68,"action":4,"request":[],"response":[Utils.UByteUtil,Utils.ShortUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.IntUtil]
// FindImmortalData.as:
//     _loc1_.type = this._award[0];
//       _loc2_.fame = param1[1];           // ����
//       _loc2_.skill = param1[2];          // ����
//       _loc2_.cultivation = param1[3];    // ����
//       _loc2_.statePoint = param1[4];
//       _loc2_.yuPai = param1[5];
//     _loc1_.cultivation = this._award[6];
// Example
//     [ 0, 20, 1500, 2, 0, 0, 558 ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_pickup_award() {
    Json::Value data;
    return this->send_and_receive(data, 68, 4);
}

//============================================================================
// R179 �帣����
//============================================================================
void sxd_client::find_immortal2() {
    auto data = this->Mod_FindImmortal_Base_is_five_blessings_open();
    if (data[0].asInt() != Mod_FindImmortal_Base::YES)
        return;

    for (;;) {
        data = this->Mod_FindImmortal_Base_open_five_blessings();
        int bless_number = data[0].asInt();
        int stage = data[1].asInt();
        if (stage) {
            data = this->Mod_FindImmortal_Base_end_bless();
            if (data[0].asInt() != Mod_FindImmortal_Base::SUCCESS) {
                common::log(boost::str(boost::format("���帣���š����þ���ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            std::vector<int> awards = { 0, 0, 0, 0 };
            for (const Json::Value& item : data[1]) {
                awards[0] += item[1].asInt();
                awards[1] += item[2].asInt();
                awards[2] += item[3].asInt();
                awards[3] += item[4].asInt();
            }
            common::log(boost::str(boost::format("���帣���š����þ��գ���� [������%1%]��[�����%2%]��[�ǻ��%3%]��[ħʯ��Ƭ��%4%]") % awards[0] % awards[1] % awards[2] % awards[3]), iEdit);
        }
        if (bless_number == 0)
            break;
        data = this->Mod_FindImmortal_Base_start_bless();
        if (data[0].asInt() != Mod_FindImmortal_Base::SUCCESS) {
            common::log(boost::str(boost::format("���帣���š�������ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log("���帣���š�������", iEdit);
    }
}

//============================================================================
// R179 ��ͨ��
// "module":68,"action":14,"request":[],"response":[Utils.UByteUtil]
// FindImmortalData.as 148:
//     this.fiveBlessingInfo.isOpen = param1[0] == Mod_FindImmortal_Base.YES;
// Example
//     [ 11 ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_is_five_blessings_open() {
    Json::Value data;
    return this->send_and_receive(data, 68, 14);
}

//============================================================================
// R179 ���
// "module":68,"action":10,"request":[],"response":[Utils.ShortUtil,Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil],Utils.ByteUtil]
// FindImmortalData.as 153:
//     this.fiveBlessingInfo.bless_number = param1[0];
//     this.fiveBlessingInfo.stage = param1[1];
//     this.fiveBlessingInfo.award = this.parseAward(param1[2]);
//     this.fiveBlessingInfo.total_count = param1[3];
// Example
//     [ 10, 0, null, 20 ]
//     [ 9, 1, [ [ 4, 1200, 0, 2, 1 ], [ 3, 600, 3, 0, 0 ], [ 3, 600, 3, 0, 0 ], [ 5, 1200, 0, 3, 2 ], [ 4, 1200, 0, 2, 1 ] ], 20 ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_open_five_blessings() {
    Json::Value data;
    return this->send_and_receive(data, 68, 10);
}

//============================================================================
// R179 ������
// "module":68,"action":11,"request":[],"response":[Utils.UByteUtil,Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil]]
// FindImmortalData.as 161:
//     this.fiveBlessingInfo.result = param1[0];
//     this.fiveBlessingInfo.stage = param1[1];
//     this.fiveBlessingInfo.award = this.parseAward(param1[2]);
// Example
//     [ 0, 1, [ [ 4, 1200, 0, 2, 1 ], [ 3, 600, 3, 0, 0 ], [ 4, 1200, 0, 2, 1 ], [ 3, 600, 3, 0, 0 ], [ 5, 1200, 0, 3, 2 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_start_bless() {
    Json::Value data;
    return this->send_and_receive(data, 68, 11);
}

//============================================================================
// R179 ���þ���
// "module":68,"action":13,"request":[],"response":[Utils.UByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil]]
// FindImmortalData.as 175:
//     this.fiveBlessingInfo.result = param1[0];
//     this.fiveBlessingInfo.award = this.parseAward(param1[1]);
//         _loc3_.image_id = _loc4_[0];
//         _loc3_.skill = _loc4_[1];        // ����
//         _loc3_.xian_ling = _loc4_[2];    // ����
//         _loc3_.star_soul = _loc4_[3];    // �ǻ�
//         _loc3_.jewel_scrap = _loc4_[4];  // ħʯ��Ƭ
// Example
//     [ 0, [ [ 4, 1200, 0, 2, 1 ], [ 3, 600, 3, 0, 0 ], [ 3, 600, 3, 0, 0 ], [ 5, 1200, 0, 3, 2 ], [ 4, 1200, 0, 2, 1 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_end_bless() {
    Json::Value data;
    return this->send_and_receive(data, 68, 13);
}
