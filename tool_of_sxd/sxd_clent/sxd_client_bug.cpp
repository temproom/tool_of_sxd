#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_MissionPractice_Base {
public:
    static const int SUCCESS = 0;
    static const int NOT_ENOUGH_POWER = 1;
    static const int RANK_REPORT = 6;
};

void sxd_client::bug() {
    auto data = this->Mod_Player_Base_get_player_function();
    auto functions = data[0];

    for (;;) {
        data = this->Mod_Player_Base_get_player_info();
        std::string nickname = common::utf2gbk(data[0].asString());
        int level = data[1].asInt();
        common::log(boost::str(boost::format("��%1%��, %2%��") % nickname % level), iEdit);

        //this->equipment();

        //if (std::find_if(functions.begin(), functions.end(), [](const Json::Value& x) {return x[0].asInt() == 78;}) != functions.end())
        //    this->library();

        //this->mission_practice();
        this->regression();
    }
}

void sxd_client::mission_practice() {
    // ���һ���ѹ����ҷ�BOSS��mission
    int mission_id_opt = 0;
    // 90:���, ..., 56:����
    // 55:�����, ..., 14:���ݳ�, 13:����ɽ, 9:���ſ�, 8:������, 7:�ŵ���, 4:�콣��
    int town_ids[] = {/*149,147,145,106,105,100,93,90, 84, 83, 82, 81, 80, 79, 76, 75, 73, 70, 68, 65, 62, 58, 56,*/ 55, 54, 43, 40, 39, 35, 33, 29, 27, 26, 24, 23, 22, 21, 15, 14, 13, 9, 8, 7, 4 };
    for (int town_id : town_ids) {
        auto data = this->Mod_Mission_Base_get_sections(town_id);
        auto sections = data[0];
        if (!sections.size())
            continue;
        std::reverse(sections.begin(), sections.end());
        auto it = std::find_if(sections.begin(), sections.end(), [](const Json::Value& x) {return x[1].asInt()/*�ѹ���*/&& !x[3].asInt()/*��BOSS*/;});
        if (it != sections.end()) {
            mission_id_opt = (*it)[0].asInt();
            break;
        }
    }

    for (int i = 0; i < 10; ++i) {
        auto data = this->Mod_MissionPractice_Base_start_practice(mission_id_opt);
        if (data[0].asInt() == Mod_MissionPractice_Base::NOT_ENOUGH_POWER) {
            common::log("������ɨ��������������");
            break;
        }
        if (data[0].asInt() != Mod_MissionPractice_Base::SUCCESS) {
            common::log("������ɨ����������");
            break;
        }
        this->Mod_MissionPractice_Base_quickly();
    }
    common::log(boost::str(boost::format("������ɨ����mission_id: %1%") % mission_id_opt), iEdit);
}

Json::Value sxd_client::Mod_MissionPractice_Base_start_practice(int mission_id) {
    Json::Value data;
    data.append(mission_id);
    data.append(5);
    data.append(1);
    return this->send_and_receive(data, 25, 1);
}

Json::Value sxd_client::Mod_MissionPractice_Base_quickly() {
    Json::Value data;
    data.append(0);
    return this->send_and_receive(data, 25, 3, 25, 4, [](const Json::Value& x) {return x[0].asInt() == Mod_MissionPractice_Base::RANK_REPORT;});
}
