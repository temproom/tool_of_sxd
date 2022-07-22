#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Fate_Base {
public:
    static const int SUCCESS = 0;
    static const int FULL_TEMP_BAG = 13;
};

static std::string fate_names[] = { "", "����", "��ı���", "ʮ˥�Ű�", "������", "˥����", "Ѫ���ֶ�", "��������", "��������", "ǧ��δ��", "ն��", "���ߺ���", "����", "ǧ����ϲ", "����", "һԪ��ʼ", "�޾�", "�������", "��Ѫ", "߳�����", "����", "����", "̰��", "��������", "�Ҵ�", "ս��", "�ƾ�", "��ԯ����", "����", "�Ի�", "ҹ��", "���ƻ���", "ɢ��", "������", "�����޽�", "̫��", "�쵶����", "�ٲ�����", "�������", "��ת����", "����", "���������", "����ԯ����", "����ƻ���", "������Ƭ", "����ߺ���", "��ǧ����ϲ", "���������", "����ʥ", "������", "������", "������", "�����޷�", "", "", "ǧ���", "������", "ǧ������", "������", "�����˺�", "ʯ���쾪", "�񹤹���", "����ն", "��������Ƭ", "������˫", "��ʯ��Լ", "�������", "������", "���Լ���", "�����", "������Ƭ", "����", "��������", "�����", "����ǧ��", "��и�ɻ�", "���Դ" };
static std::string npc_names[] = { "", "��һ��", "С��Ů", "���", "�ŵ���", "������" };

//============================================================================
// R180 ��������
//============================================================================
void sxd_client::batch_fate(int count) {
    for (int i = 0; i < count; i++) {
        auto data = this->Mod_Fate_Base_get_fate_npc();
        auto npcs = data[1];
        std::vector<Json::Value> npcs_valid;
        std::copy_if(npcs.begin(), npcs.end(), std::back_inserter(npcs_valid), [](const Json::Value& x) {return x[1].asInt()==1;});
        auto npc_opt = *std::max_element(npcs_valid.begin(), npcs_valid.end(), [](const Json::Value& x1, const Json::Value& x2) {return x1[0].asInt()<x2[0].asInt();});

        data = this->Mod_Fate_Base_appoint_fate_npc(npc_opt[0].asInt());
        auto msg = data[0].asInt();
        if (msg == Mod_Fate_Base::FULL_TEMP_BAG) {
            common::log("�������������ռ�����", iEdit);

            // ����ռ�
            data = this->Mod_Fate_Base_get_temp_fate();
            auto temp_fates = data[0];

            // һ������
            std::vector<Json::Value> temp_fates_bad;
            std::copy_if(temp_fates.begin(), temp_fates.end(), std::back_inserter(temp_fates_bad), [](const Json::Value& x) {return x[1].asInt()<=9;});
            std::vector<long long> temp_fate_ids_bad;
            std::transform(temp_fates_bad.begin(), temp_fates_bad.end(), std::back_inserter(temp_fate_ids_bad), [](const Json::Value& x) {return x[0].asInt64();});
            if (temp_fate_ids_bad.size()) {
                data = this->Mod_Fate_Base_sale_temp_fate(temp_fate_ids_bad);
                common::log("��������һ������", iEdit);
            }

            // һ��ʰȡ
            this->Mod_Fate_Base_merge_all_in_bag();
            std::vector<Json::Value> temp_fates_good;
            std::copy_if(temp_fates.begin(), temp_fates.end(), std::back_inserter(temp_fates_good), [](const Json::Value& x) {return x[1].asInt()>9;});
            std::vector<long long> temp_fate_ids_good;
            std::transform(temp_fates_good.begin(), temp_fates_good.end(), std::back_inserter(temp_fate_ids_good), [](const Json::Value& x) {return x[0].asInt64();});
            if (temp_fate_ids_good.size()) {
                data = this->Mod_Fate_Base_pickup_fate(temp_fate_ids_good);
                common::log("��������һ��ʰȡ", iEdit);
            }
            this->Mod_Fate_Base_merge_all_in_bag();
            continue;
        } else if (msg != Mod_Fate_Base::SUCCESS) {
            common::log(boost::str(boost::format("������������ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("������������ [%1%]����� [%2%]������ [%3%]") % npc_names[npc_opt[0].asInt()] % fate_names[data[1].asInt()] % npc_names[data[2].asInt()]), iEdit);
    }
}

//============================================================================
// R179 ����
//============================================================================
void sxd_client::fate() {
    // read config
    auto config = atoi(db.get_config(user_id.c_str(), "Fate").c_str());
    common::log(boost::str(boost::format("��Fate��[%1%] (��0Ϊ����������1Ϊÿ�������һ��������2Ϊ�Ե���������ͣ)") % config), 0);
    for (;;) {
        this->Mod_Fate_Base_merge_all_in_bag();

        // ����ռ�
        auto data = this->Mod_Fate_Base_get_temp_fate();
        auto temp_fates = data[0];

        // һ������
        std::vector<Json::Value> temp_fates_bad;
        std::copy_if(temp_fates.begin(), temp_fates.end(), std::back_inserter(temp_fates_bad), [](const Json::Value& x) {return x[1].asInt()<=9;});

        std::vector<long long> temp_fate_ids_bad;
        std::transform(temp_fates_bad.begin(), temp_fates_bad.end(), std::back_inserter(temp_fate_ids_bad), [](const Json::Value& x) {return x[0].asInt64();});

        if (temp_fate_ids_bad.size()) {
            data = this->Mod_Fate_Base_sale_temp_fate(temp_fate_ids_bad);
            if (data[0].asInt() != Mod_Fate_Base::SUCCESS) {
                common::log(boost::str(boost::format("��������һ������ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
                return;
            }
            common::log("��������һ������", iEdit);
        }

        // һ��ʰȡ
        std::vector<Json::Value> temp_fates_good;
        std::copy_if(temp_fates.begin(), temp_fates.end(), std::back_inserter(temp_fates_good), [](const Json::Value& x) {return x[1].asInt()>9;});
        std::vector<long long> temp_fate_ids_good;
        std::transform(temp_fates_good.begin(), temp_fates_good.end(), std::back_inserter(temp_fate_ids_good), [](const Json::Value& x) {return x[0].asInt64();});
        if (temp_fate_ids_good.size()) {
            data = this->Mod_Fate_Base_pickup_fate(temp_fate_ids_good);
            if (data[0].asInt() != Mod_Fate_Base::SUCCESS) {
                common::log(boost::str(boost::format("��������һ��ʰȡʧ�ܣ�msg[%1%]") % data[0]), iEdit);
                return;
            }
            common::log("��������һ��ʰȡ", iEdit);
        }

        this->Mod_Fate_Base_merge_all_in_bag();

        data = this->Mod_Fate_Base_get_fate_npc();
        int free_appoint_times = data[0].asInt();
        auto npcs = data[1];
        std::vector<Json::Value> npcs_valid;
        std::copy_if(npcs.begin(), npcs.end(), std::back_inserter(npcs_valid), [](const Json::Value& x) {return x[1].asInt()==1;});
        auto npc_opt = *std::max_element(npcs_valid.begin(), npcs_valid.end(), [](const Json::Value& x1, const Json::Value& x2) {return x1[0].asInt()<x2[0].asInt();});
        if (config <= 0 || config > 2)
            return;
        if (config == 1 && free_appoint_times == 0)
            return;
        if (config == 2 && free_appoint_times == 0 && npc_opt[0].asInt() == 5)
            return;
        data = this->Mod_Fate_Base_appoint_fate_npc(npc_opt[0].asInt());
        if (data[0].asInt() != Mod_Fate_Base::SUCCESS) {
            common::log(boost::str(boost::format("������������ʧ�ܣ�msg[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("������������ [%1%]����� [%2%]������ [%3%]") % npc_names[npc_opt[0].asInt()] % fate_names[data[1].asInt()] % npc_names[data[2].asInt()]), iEdit);
    }
}

//============================================================================
// R179 һ���ϳ�
// "module":22,"action":17,"request":[],"response":[]
//============================================================================
void sxd_client::Mod_Fate_Base_merge_all_in_bag() {
    Json::Value data;
    this->send_frame(data, 22, 17);
}

//============================================================================
// R179 NPC
// "module":22,"action":0,"request":[],"response":[Utils.ByteUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.IntUtil]
// FateController.as 79:
//     _loc2_.freeAppointTimes = _loc1_[0];
//     var _loc3_:Array = this.renderFateNpc(_loc1_[1],_loc1_[2]);
//         _loc8_.fateNpcId = param1[_loc7_][0];
//         _loc8_.status = param1[_loc7_][1];
//         _loc8_.canBuyTimer = param1[_loc7_][2];
//         _loc8_.maxBuyTimes = param1[_loc7_][3];
//         _loc8_.buyPrice = param1[_loc7_][4];
// Example
//
//============================================================================
Json::Value sxd_client::Mod_Fate_Base_get_fate_npc() {
    Json::Value data;
    return this->send_and_receive(data, 22, 0);
}

//============================================================================
// R179 ����
// "module":22,"action":9,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil]
// FateController.as 506:
//     _loc2_.msg = _loc1_[0];
//     _loc2_.fateId = _loc1_[1];
//     _loc2_.fateNpcId = _loc1_[2];
//     _loc2_.tempFateId = _loc1_[3];
// Example
//
//============================================================================
Json::Value sxd_client::Mod_Fate_Base_appoint_fate_npc(int npc_id) {
    Json::Value data;
    data.append(npc_id);
    return this->send_and_receive(data, 22, 9);
}

//============================================================================
// R179 ����ռ�
// "module":22,"action":1,"request":[],"response":[[Utils.LongUtil,Utils.ShortUtil]]
// FateController.as 136:
//     return this.renderTempFate(_loc1_[0]);
//         _loc5_.tempFateId = param1[_loc4_][0];
//         _loc5_.fateId = param1[_loc4_][1];
// Example
//
//============================================================================
Json::Value sxd_client::Mod_Fate_Base_get_temp_fate() {
    Json::Value data;
    return this->send_and_receive(data, 22, 1);
}

//============================================================================
// R179 һ��ʰȡ
// "module":22,"action":2,"request":[[Utils.LongUtil]],"response":[Utils.UByteUtil,[Utils.LongUtil]]
// FateController.as 171:
//     _loc2_.msg = _loc1_[0];
//     _loc2_.gridList = _loc1_[1];
// Example
//
//============================================================================
Json::Value sxd_client::Mod_Fate_Base_pickup_fate(const std::vector<long long> &temp_fate_ids) {
    Json::Value data, data1;
    for (auto id : temp_fate_ids) {
        Json::Value data2;
        data2.append(id);
        data1.append(data2);
    }
    data.append(data1);
    return this->send_and_receive(data, 22, 2);
}

//============================================================================
// R179 һ������
// "module":22,"action":3,"request":[[Utils.LongUtil]],"response":[Utils.UByteUtil]
// FateController.as 180:
//     _loc2_.msg = _loc1_[0];
// Example
//
//============================================================================
Json::Value sxd_client::Mod_Fate_Base_sale_temp_fate(const std::vector<long long> &temp_fate_ids) {
    Json::Value data;
    Json::Value data1;
    for (auto id : temp_fate_ids) {
        Json::Value data2;
        data2.append(id);
        data1.append(data2);
    }
    data.append(data1);
    return this->send_and_receive(data, 22, 3);
}
