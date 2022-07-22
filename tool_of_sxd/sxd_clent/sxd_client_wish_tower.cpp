#include "stdafx.h"
#include <thread>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Tower_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// R196 �����ֻ�
//============================================================================
void sxd_client::tower() {
    std::string towers[] = { "", "������(һ)", "�����(һ)", "������(һ)", "���޵�(һ)", "�˼��(һ)", "���(һ)", "������(��)", "�����(��)", "������(��)", "���޵�(��)", "�˼��(��)", "���(��)" };
    int auto_sell_l = 1;
    int auto_sell_z = 0;

    auto data = this->Mod_Tower_Base_get_tower_info();
    int sequence_max = data[0].asInt();
    int layer = data[1].asInt();
    int sequence_war = data[2].asInt() + 1;
    int reset_num = data[4].asInt();
    int hide_id = data[6].asInt();
    common::log(boost::str(boost::format("�������ֻء�[%1%]����ͨ�� [%2%] �㣬��ǰ�� [%3%] �㣬���ջ���ˢ�� [%4%] ��") % towers[layer] % sequence_max % sequence_war % reset_num), 0);
    if (layer == 12 && sequence_max == 25 && sequence_war == 26) {
        common::log("�������ֻء���ͨ��", 0);
        return;
    }

    // ˢ��
    if (reset_num) {
        data = this->Mod_Tower_Base_reset_layer();
        if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
            common::log(boost::str(boost::format("�������ֻء�ˢ��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log("�������ֻء�ˢ��", iEdit);
        sequence_war = 1;
    }

    // �Զ�����
    if (sequence_war <= sequence_max) {
        data = this->Mod_Tower_Base_start_auto_challenge(auto_sell_l, auto_sell_z);
        if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
            common::log(boost::str(boost::format("�������ֻء��Զ�����ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log("�������ֻء��Զ�������ʼ", iEdit);

        while (sequence_war <= sequence_max) {
            data = this->Mod_Tower_Base_quickly_auto_challenge(auto_sell_l, auto_sell_z);
            if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
                common::log(boost::str(boost::format("�������ֻء��Զ�����ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            sequence_war = data[1].asInt() + 1;
            common::log(boost::str(boost::format("�������ֻء��Զ������� [%1%] �� [%2%] ��") % towers[layer] % sequence_war), iEdit);
        }
    }

    if (layer == 12 && sequence_max == 25 && sequence_war == 25) {
        common::log("�������ֻء����һ���ݲ���", 0);
        return;
    }

    // ������һ��
    if (sequence_war == 26) {
        data = this->Mod_Tower_Base_enter_next();
        if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
            common::log(boost::str(boost::format("�������ֻء�������һ��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        data = this->Mod_Tower_Base_get_tower_info();
        sequence_war = data[2].asInt() + 1;
        if (sequence_war == 1)
            common::log("�������ֻء�������һ��", iEdit);
        else {
            common::log("�������ֻء�������һ��ʧ��", iEdit);

            // ��ս���ز�
            int failed_count = 0;
            while (hide_id) {
                // CD
                // Tower:int = 19
                data = this->Mod_Player_Base_get_player_war_cd_time(19);
                int war_cd_time = data[0].asInt();
                if (war_cd_time) {
                    if (war_cd_time > 10)
                        break;
                    common::log(boost::str(boost::format("�������ֻء�CD�У��ȴ� [%1%] ��") % war_cd_time), 0);
                    std::this_thread::sleep_for(std::chrono::seconds(war_cd_time + 1));
                }

                // ��ս
                data = this->Mod_Tower_Base_start_challenge(0, auto_sell_l, auto_sell_z);
                if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
                    common::log(boost::str(boost::format("�������ֻء���ս���ز�ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                    return;
                }
                int war_exp = data[3].asInt();
                if (war_exp) {
                    common::log(boost::str(boost::format("�������ֻء���ս���ز㣬սʤ��� [����ֵ��%1%]") % war_exp), iEdit);
                    failed_count = 0;
                    data = this->Mod_Tower_Base_get_tower_info();
                    hide_id = data[6].asInt();
                } else {
                    common::log("�������ֻء���ս���ز㣬ս��", iEdit);
                    if (++failed_count >= 3)
                        break;
                }
            }
        }
    }

    if (sequence_war == 26)
        return;

    // ��ս�����
    int failed_count = 0;
    for (;;) {
        //
        // Tower:int = 19
        data = this->Mod_Player_Base_get_player_war_cd_time(19);
        int war_cd_time = data[0].asInt();
        if (war_cd_time) {
            if (war_cd_time > 10)
                break;
            common::log(boost::str(boost::format("�������ֻء�CD�У��ȴ� [%1%] ��") % war_cd_time), 0);
            std::this_thread::sleep_for(std::chrono::seconds(war_cd_time + 1));
        }

        // ��ս
        data = this->Mod_Tower_Base_start_challenge(sequence_war, auto_sell_l, auto_sell_z);
        if (data[0].asInt() != Mod_Tower_Base::SUCCESS) {
            common::log(boost::str(boost::format("�������ֻء���ս [%1%] �� [%2%] ��ʧ�ܣ�result[%3%]") % towers[layer] % sequence_war % data[0]), iEdit);
            return;
        }
        int war_exp = data[3].asInt();
        if (war_exp) {
            common::log(boost::str(boost::format("�������ֻء���ս [%1%] �� [%2%] �㣬սʤ��� [����ֵ��%3%]") % towers[layer] % sequence_war % war_exp), iEdit);
            failed_count = 0;
            sequence_war++;
            if (sequence_war == 26) {
                this->tower();
                return;
            }
        } else {
            common::log(boost::str(boost::format("�������ֻء���ս [%1%] �� [%2%] �㣬ս��") % towers[layer] % sequence_war), iEdit);
            if (++failed_count >= 3)
                break;
        }
    }
}

//============================================================================
// R196 ���
// "module":32,"action":0,"request":[],"response":[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]
// TowerData.as:
//     this.info.sequenceMax_ = param1[0];
//     this.info.layer = param1[1];
//     this.info.sequenceWar = param1[2] + 1;
//     this.info.bagNum = param1[3];
//     this.info.resetNum = param1[4];
//     this.info.resetIngot = param1[5];
//     this.info.hideId = param1[6];
//     this.info.highStone = param1[7];
// Example
//     [ 1, 2, 1, 10, 1, 0, 0, 101 ]
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_get_tower_info() {
    Json::Value data;
    return this->send_and_receive(data, 32, 0);
}

//============================================================================
// R196 ��ս
// "module":32,"action":2,"request":[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],"response":[...]
// TowerView.as
//     _data.call(Mod_Tower_Base.start_challenge,this.start_challenge_back,[param1,this._target.autoSellL,this._target.autoSellZ]);
// TowerData.as:
//     this.result = param1[0];
//     this.warData.format([0,0,0,[param1[1][0]]]);
//     this.warSoulId = param1[2];
//     this.warExp = param1[3];
//     this.warFame = param1[4];
//     this.warCoin = param1[5];
//     this.isPass = param1[7] == 1;
//     this.warAura = param1[8];
//     this.warHstone = param1[10];
//     this.info.highStone = this.info.highStone + this.warHstone;
//     this.info.bagNum = param1[6];
// Example
//     [ 2, 1, 0 ] -> [ 0, [ [ 3, 355546, [ [ 355546, "\u6717\u50e7\u4e09.s1", [ [ 808183, 70, "ShenYangJian", "\u795e\u6768\u622c", 5, 436400, 436400, 193, 1, 100, 135, 166, 0, 10, 2, 1, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ], [ 803481, 104, "WuShengNanFeiYu", "\u6b66\u5723\u7537\u98de\u7fbd", 3, 559504, 559504, 193, 2, 100, 110, 184, 0, 10, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ] ], [ [ 819527, 82, "FeiWei", "\u98de\u536b", 3, 83234, 83234, 193, 5, 100, 50, 160, 0, 10, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ] ], null, null, null, null, 803481, 1335, 104, 193, 17387, "", 10, 10, [ [ 41, 1, 5 ], [ 3, 3, 4 ], [ 6, 1, 3 ], [ 56, 3, 2 ], [ 55, 6, 1 ] ], 0, null, 0, 0, null, 0, null, 0 ], [ -1666, "\u50f5\u5c38", [ [ -5625, 978, "JiangShi1", "\u50f5\u5c38", 9, 14249, 14249, 56, 2, 100, 100, 27, 0, 0, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ], [ -5629, 978, "JiangShi1", "\u50f5\u5c38", 9, 14249, 14249, 56, 3, 100, 100, 27, 0, 0, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ], [ -5632, 978, "JiangShi1", "\u50f5\u5c38", 9, 14249, 14249, 56, 4, 100, 100, 27, 0, 0, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ], [ -5630, 978, "JiangShi1", "\u50f5\u5c38", 9, 14249, 14249, 56, 6, 100, 100, 27, 0, 0, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ], [ -5631, 978, "JiangShi1", "\u50f5\u5c38", 9, 14249, 14249, 56, 9, 100, 100, 27, 0, 0, 0, 0, 0, 0, null, 0, 0, [ [ 0 ], [ 0 ] ], null, 0, null, 0, null, null, 0, 0, 0, 0, 0, 0 ] ], null, null, null, null, null, 0, 0, 0, 0, 0, "", 0, 0, null, 0, null, 0, 0, null, 0, null, 0 ] ], [ [ [ [ 0, "55|", null, null, [ [ 819527, 83234, 50 ], [ 808183, 436400, 135 ], [ 803481, 559504, 110 ], [ -5625, 14249, 100 ], [ -5629, 14249, 100 ], [ -5632, 14249, 100 ], [ -5630, 14249, 100 ], [ -5631, 14249, 100 ] ], null, null, null, null, null ] ], null, [ [ 355546, 808183, 166, 4, null, "", null, null, null, null, null, null, null, null, "", null, null, "", null, null, "", null, null, 0, 0, 436400, 0, 0, 0, -1666, 0, 0, "", 0, 97, [ [ -5632, 0, null, "", null, null, null, null, null, "", null, "", null, "", null, 100, 493156, 0, 1, 0, 1, -5632, 0 ] ], null, [ [ 808183, 436400, 0 ], [ 803481, 559504, 110 ], [ -5625, 14249, 100 ], [ -5629, 14249, 100 ], [ -5632, 0, 100 ], [ -5630, 14249, 100 ], [ -5631, 14249, 100 ] ], null ], [ -1666, -5625, 27, 0, null, "", null, null, null, null, null, null, null, null, "", null, null, "", null, null, "", null, null, 0, 0, 14249, 0, 0, 0, 355546, 0, 0, "", 0, 97, [ [ 803481, 0, null, "", null, null, null, null, null, "", null, "", null, "", null, 110, 0, 559504, 0, 0, 0, 803481, 10 ] ], null, [ [ 808183, 436400, 0 ], [ 803481, 559504, 110 ], [ -5625, 14249, 0 ], [ -5629, 14249, 100 ], [ -5630, 14249, 100 ], [ -5631, 14249, 100 ] ], null ], [ 355546, 803481, 184, 0, null, "", null, null, null, null, null, null, null, null, "", null, null, "", null, null, "", null, null, 75, 0, 559504, 0, 0, 0, -1666, 0, 0, "", 0, 97, [ [ -5630, 0, [ [ 91, 184 ] ], "", null, null, null, null, null, "", null, "", null, "", null, 100, 329763, 0, 1, 0, 1, -5630, 0 ], [ -5625, 0, [ [ 91, 184 ] ], "", null, null, null, null, null, "", null, "", null, "", null, 0, 329763, 0, 1, 0, 1, -5625, 0 ], [ -5629, 0, [ [ 91, 184 ] ], "", null, null, null, null, null, "", null, "", null, "", null, 100, 329763, 0, 1, 0, 1, -5629, 0 ] ], null, [ [ 808183, 436400, 0 ], [ 803481, 559504, 75 ], [ -5625, 0, 0 ], [ -5629, 0, 100 ], [ -5630, 0, 100 ], [ -5631, 14249, 100 ] ], null ], [ -1666, -5631, 27, 0, null, "", null, null, null, null, null, null, null, null, "", null, null, "", null, null, "", null, null, 0, 0, 14236, 0, 0, 0, 355546, 0, 0, "", 0, 97, [ [ 803481, 0, [ [ 91, 27 ] ], "", null, null, null, null, null, "", null, "", null, "", null, 75, 201, 559504, 1, 0, 0, 803481, 9 ] ], [ [ 10188, "803481|-5631|13" ] ], [ [ 808183, 436400, 0 ], [ 803481, 559504, 75 ], [ -5631, 14236, 0 ] ], null ] ], [ [ -1666, 201 ], [ 355546, 1482458 ] ], [ [ 74, "1", null, null, null, null, null, null, null ], [ 74, "1", null, null, null, null, null, null, null ] ], null, [ [ 819527, 83234, 50 ], [ 808183, 436400, 0 ], [ 803481, 559504, 75 ], [ -5625, 0, 0 ], [ -5629, 0, 100 ], [ -5632, 0, 100 ], [ -5630, 0, 100 ], [ -5631, 14236, 0 ] ] ], [ [ [ 0, "56|", null, null, [ [ 819527, 83234, 50 ], [ 808183, 436400, 0 ], [ 803481, 559504, 75 ], [ -5625, 0, 0 ], [ -5629, 0, 100 ], [ -5632, 0, 100 ], [ -5630, 0, 100 ], [ -5631, 14236, 0 ], [ -5629, 0, 100 ], [ -5625, 0, 0 ], [ -5630, 0, 100 ], [ -5632, 0, 100 ] ], null, null, null, null, null ] ], null, [ [ 355546, 808183, 166, 3, null, "", null, null, null, null, null, null, null, null, "", null, null, "", null, null, "", null, null, 25, 0, 436400, 0, 0, 0, -1666, 0, 0, "", 0, 93, [ [ -5631, 0, null, "", null, null, null, null, null, "", null, "", null, "", null, 25, 493156, 0, 1, 0, 1, -5631, 0 ] ], null, [ [ 808183, 436400, 25 ], [ 803481, 559504, 75 ], [ -5631, 0, 25 ] ], null ] ], [ [ -1666, 201 ], [ 355546, 1975614 ] ], [ [ 74, "1", null, null, null, null, null, null, null ], [ 74, "1", null, null, null, null, null, null, null ] ], null, [ [ 819527, 83234, 50 ], [ 808183, 436400, 0 ], [ 803481, 559504, 75 ], [ -5625, 0, 0 ], [ -5629, 0, 100 ], [ -5632, 0, 100 ], [ -5630, 0, 100 ], [ -5631, 14236, 0 ], [ -5629, 0, 100 ], [ -5625, 0, 0 ], [ -5630, 0, 100 ], [ -5632, 0, 100 ] ] ] ], 0 ] ], 0, 10000, 0, 0, 10, 1, 500, 0, 0 ]
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_start_challenge(int war_idx, int auto_sell_l, int auto_sell_z) {
    Json::Value data;
    data.append(war_idx);
    data.append(auto_sell_l);
    data.append(auto_sell_z);
    return this->send_and_receive(data, 32, 2);
}

//============================================================================
// R196 ˢ��
// "module":32,"action":3,"request":[],"response":[Utils.UByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil]
// TowerData.as:
//     this.result = param1[0];
//     if(this.result != Mod_Tower_Base.SUCCESS)
//        return;
//     this.info.resetNum = param1[1];
//     this.info.resetIngot = param1[2];
//     this.info.hideId = param1[3];
//     this.info.sequenceWar = 1;
//     this.info.panelIdx = 1;
// Example
//     [ 0, 0, 100, 0 ]
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_reset_layer() {
    Json::Value data;
    return this->send_and_receive(data, 32, 3);
}

//============================================================================
// R196 �Զ�������ʼ
// "module":32,"action":4,"request":[Utils.ByteUtil,Utils.ByteUtil],"response":[Utils.UByteUtil]
// TowerData.as:
//     this.result = param1[0];
// Example
//     [ 1, 0 ] -> [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_start_auto_challenge(int auto_sell_l, int auto_sell_z) {
    Json::Value data;
    data.append(auto_sell_l);
    data.append(auto_sell_z);
    return this->send_and_receive(data, 32, 4);
}

//============================================================================
// R196 �Զ�����
// "module":32,"action":7,"request":[Utils.ByteUtil,Utils.ByteUtil],"response":[Utils.UByteUtil]
// TowerData.as:
//     this.result = param1[0];
//     if(this.result == Mod_Tower_Base.SUCCESS)
//        this.info.bagNum = param1[4];
//        this.info.sequenceWar = param1[1] + 1;
// Example
// Mod_Tower_Base.quickly_auto_challenge(32,7)
//     [ 1, 0 ]
// Mod_Tower_Base.notify_auto_challenge_report(32,6)
//     [ 0, 1, 0, 10000, 10, 0 ]
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_quickly_auto_challenge(int auto_sell_l, int auto_sell_z) {
    Json::Value data;
    data.append(auto_sell_l);
    data.append(auto_sell_z);
    return this->send_and_receive(data, 32, 7, 32, 6);
}

//============================================================================
// R196 ������һ��
// "module":32,"action":9,"request":[],"response":[Utils.UByteUtil]
// TowerData.as:
//     this.result = param1[0];
//============================================================================
Json::Value sxd_client::Mod_Tower_Base_enter_next() {
    Json::Value data;
    return this->send_and_receive(data, 32, 9);
}
