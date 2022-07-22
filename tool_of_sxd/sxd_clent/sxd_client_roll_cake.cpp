#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_RollCake_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// R177 ���Ǹ���
//============================================================================
void sxd_client::roll_cake() {
    std::string cakes[] = { "", "ƽ��", "һ��", "����", "����", "����", "��»��", "�󼪴���", "���ӵǿ�", "��������", "�鸣����", "���Ǹ���", "�󼪴���", "�ٱ���ɽ", "��Դ����", "ƽ������", "��������", "�вƽ���" };
    for (;;) {
        Json::Value data = this->Mod_RollCake_Base_get_state();
        int type = data[1].asInt();
        data = this->Mod_RollCake_Base_get_count();
        int count = data[0].asInt();
        int freeRobeNum = data[1].asInt();
        if (type == 0) {
            // NO_RECORD
            if (count == 0)
                return;
            data = this->Mod_RollCake_Base_roll();
            common::log(boost::str(boost::format("�����Ǹ��ա������ӣ�[%1%]") % cakes[data[1].asInt()]), iEdit);
        } else if (type == 11 || freeRobeNum == 0) {
            // 11:���Ǹ���
            data = this->Mod_RollCake_Base_get_award();
            if (data[0].asInt() != Mod_RollCake_Base::SUCCESS) {
                common::log(boost::str(boost::format("�����Ǹ��ա��ջ�ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
                return;
            }
            common::log("�����Ǹ��ա��ջ�", iEdit);
        } else {
            // HAVE_RECORD
            data = this->Mod_RollCake_Base_reroll();
            common::log(boost::str(boost::format("�����Ǹ��ա�������ˣ�[%1%]") % cakes[data[1].asInt()]), iEdit);
        }
    }
}

//============================================================================
// R177 ״̬
// "module":38, "action":5, "request":[], "response":[Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil]
// RollCakeController.as 147:
//     _loc2_.msg = param1[0];
//     _loc2_.type = param1[1];
//     _loc2_.rerollIngot = param1[2];
// Example
//     [ 4, 0, 0, 0, 0, 0, 0, 0, 0 ]
// ƽ��
//     [ 5, 1, 10, 5, 3, 1, 3, 6, 5 ]
// ����
//     [ 5, 3, 10, 5, 1, 4, 3, 4, 1 ]
//============================================================================
Json::Value sxd_client::Mod_RollCake_Base_get_state() {
    Json::Value data;
    return this->send_and_receive(data, 38, 5);
}

//============================================================================
// R177 ����
// "module":38, "action":2, "request":[], "response":[Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil]
// RollCakeController.as 50:
//     _loc2_.count = _loc1_[0];
//     _loc2_.freeRobeNum = _loc1_[1];
//     _loc2_.robeMaxNum = _loc1_[2];
//     _loc2_.rollCount = _loc1_[3];
// Example
//    [ 10, 9, 99, 3086 ]
//    [ 9, 9, 99, 3091 ]
//============================================================================
Json::Value sxd_client::Mod_RollCake_Base_get_count() {
    Json::Value data;
    return this->send_and_receive(data, 38, 2);
}

//============================================================================
// R177 ������
// "module":38, "action":0, "request":[], "response":[Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil]
// RollCakeController.as 147:
//     _loc2_.msg = param1[0];
//     _loc2_.type = param1[1];
//     _loc2_.rerollIngot = param1[2];
// Example
//     [ 0, 1, 10, 5, 3, 1, 3, 6, 5, 3091 ]
//============================================================================
Json::Value sxd_client::Mod_RollCake_Base_roll() {
    Json::Value data;
    return this->send_and_receive(data, 38, 0);
}

//============================================================================
// R177 �������
// "module":38, "action":3, "request":[], "response":[Utils.UByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil]
// RollCakeController.as 67:
//     _loc2_.msg = param1[0];
//     _loc2_.type = param1[1];
//     _loc2_.canFree = param1[2];
//     _loc2_.rerollIngot = param1[3];
// Example
//     [ 0, 3, 1, 10, 5, 5, 4, 1, 4, 1, 3091 ]
//============================================================================
Json::Value sxd_client::Mod_RollCake_Base_reroll() {
    Json::Value data;
    return this->send_and_receive(data, 38, 3);
}

//============================================================================
// R177 �ջ�
// "module":38, "action":4, "request":[], "response":[Utils.UByteUtil]
// RollCakeController.as 215:
//     _loc2_.msg = _loc1_[0];
// Example
//     [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_RollCake_Base_get_award() {
    Json::Value data;
    return this->send_and_receive(data, 38, 4);
}

