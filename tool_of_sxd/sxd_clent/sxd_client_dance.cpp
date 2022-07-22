#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Dance_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// R179 Ⱥ������
//============================================================================
void sxd_client::dance() {

    for (;;) {
        auto data = this->Mod_Dance_Base_get_dance_info();
        if (data[0].asInt() == 0)
            return;
        int dan_state = 0;
        if (data[1].asInt() == 1)
            dan_state = 1;
        if (data[2].asInt() == 1)
            dan_state = 2;

        if (dan_state == 0)
            return;
        if (dan_state == 1) {
            this->Mod_Town_Base_enter_town(1);
            data = this->Mod_Dance_Base_start_dance();
            if (data[0].asInt() != Mod_Dance_Base::SUCCESS) {
                common::log(boost::str(boost::format("��Ⱥ�����衿����ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            common::log("��Ⱥ�����衿����", iEdit);
        } else if (dan_state == 2) {
            this->Mod_Town_Base_enter_town(1);
            data = this->Mod_Dance_Base_receive_award();
            if (data[0].asInt() != Mod_Dance_Base::SUCCESS) {
                common::log(boost::str(boost::format("��Ⱥ�����衿��ȡ����ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            common::log("��Ⱥ�����衿��ȡ����", iEdit);
        }
    }
}

//============================================================================
// R179 ���
// "module":76,"action":0,"request":[],"response":[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil]
// DanceData.as 25:
//     this.bDanceTime = param1[0] == 1;
//     this.danState = 0;
//     if(param1[1] == 1)
//        this.danState = 1;
//     if(param1[2] == 1)
//        this.danState = 2;
// Example
// δ������ʱ��
//     [ 0, 0, 0 ]
// ʱ�䵽��
//     [ 1, 1, 0 ]
// �����
//     [ 1, 1, 1 ]
// ��ȡ��
//     [ 1, 1, 0 ]
//============================================================================
Json::Value sxd_client::Mod_Dance_Base_get_dance_info() {
    Json::Value data;
    return this->send_and_receive(data, 76, 0);
}

//============================================================================
// R179 ��ʼ����
// "module":76,"action":1,"request":[],"response":[Utils.UByteUtil]
// Example
//     [ 1 ]
//     [ 0 ] �ɹ�
//============================================================================
Json::Value sxd_client::Mod_Dance_Base_start_dance() {
    Json::Value data;
    return this->send_and_receive(data, 76, 1);
}

//============================================================================
// R179 ��ȡ����
// "module":76,"action":2,"request":[],"response":[Utils.UByteUtil]
// Example
//     [ 1 ]
//     [ 0 ] �ɹ�
//============================================================================
Json::Value sxd_client::Mod_Dance_Base_receive_award() {
    Json::Value data;
    return this->send_and_receive(data, 76, 2);
}
