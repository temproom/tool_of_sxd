#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Library_Base {
public:
    static const int YES = 0;
    static const int NO = 1;
    static const int SUCCESS = 2;
};

class Mod_FindImmortal_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// �ؾ���
//============================================================================
void sxd_client::library() {
    // �Ƿ�ͨFindImmortal(70, ϲ���콵)����
    auto data = this->Mod_Player_Base_get_player_function();
    if (std::find_if(data[0].begin(), data[0].end(), [](const Json::Value& x) {return x[0].asInt() == 70;}) != data[0].end()) {
        data = this->Mod_Player_Base_player_info_contrast(player_id);
        int fame = data[0][0][4].asInt();
        int xian_ling = data[0][0][10].asInt();
        // ����С��2000���������һ�
        if (fame >= (1200200) && xian_ling < 2000) {
            int exchange_xian_ling = (fame - 1200000) / 200;
            data = this->Mod_FindImmortal_Base_exchange_xian_ling(exchange_xian_ling);
            if (data[0].asInt() == Mod_FindImmortal_Base::SUCCESS)
                common::log(boost::str(boost::format("���ؾ������� [������%1%] �һ� [�����%2%]") % fame % exchange_xian_ling), iEdit);
        }
    }

    std::string jobs[] = { "", "��ʥ", "����", "����", "", "����", "��ʿ" };
    data = this->Mod_Library_Base_get_library_info();
    auto role_job_info_list = data[0];
    bool is_activate_upgrade = data[4].asInt() == Mod_Library_Base::YES;
    for (int job_id = 1; job_id <= (is_activate_upgrade ? 6 : 3); ++job_id) {
        if (job_id == 4)
            continue;
        if (!is_activate_upgrade && std::find_if(role_job_info_list.begin(), role_job_info_list.end(), [job_id](const Json::Value& x) {return x[0].asInt() == job_id && x[1].asInt() == 180;}) != role_job_info_list.end())
            continue;


        //this->Mod_Library_Base_up_library_level(job_id);
        data = this->Mod_Library_Base_up_library_level(job_id);
        if (data[0].asInt() == Mod_Library_Base::SUCCESS)
            common::log(boost::str(boost::format("���ؾ������� [%1%] ����") % jobs[job_id]), iEdit);
    }
}

//============================================================================
// 2019030101 �����һ�����
// "module":68,"action":6,"request":[Utils.IntUtil],"response":[Utils.UByteUtil]
// Example
//     [ 1 ] -> [ 0(Mod_FindImmortal_Base::SUCCESS) ]
//============================================================================
Json::Value sxd_client::Mod_FindImmortal_Base_exchange_xian_ling(int num) {
    Json::Value data;
    data.append(num);
    return this->send_and_receive(data, 68, 6);
}

//============================================================================
// 2019030101 �ؾ������
// "module":85,"action":1,"request":[],"response":[[Utils.ByteUtil,Utils.ShortUtil],Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.UByteUtil]
// LibraryData.as:
//     for each(_loc2_ in param1[0])
//         this._role_job_info_list.push({
//             "role_job_id":_loc2_[0],
//             "job_library_level":_loc2_[1]
//         });
//     this._cur_role_id = param1[1];
//     this._cur_role_stunt_id = param1[2];
//     this._study_stunt_type_id = param1[3];
//     this._is_activate_upgrade = param1[4] == Mod_Library_Base.YES;
// Example
//     [ [ [ 3(����), 149 ], [ 1(��ʥ), 140 ], [ 2(����), 149 ] ], 5(������), 184(��׷�ʹ�), 117, 1(Mod_Library_Base.NO) ]
//     [ [ [ 1(��ʥ), 242(�������Ƕ���) ], [ 6(��ʿ), 240(��������ʮ��) ], [ 5(����), 240(��������ʮ��) ], [ 2(����), 243(������������) ], [ 3(����), 253(������������) ] ], 6, 209, 322, 0(Mod_Library_Base.YES) ]
//     [ [ [ 3(����), 151(��������һ��) ], [ 1(��ʥ), 143(������������) ], [ 2(����), 151(��������һ��) ] ], 5, 184, 117, 1(Mod_Library_Base.NO) ]
//============================================================================
Json::Value sxd_client::Mod_Library_Base_get_library_info() {
    Json::Value data;
    return this->send_and_receive(data, 85, 1);
}

//============================================================================
// 2019030101 �ؾ��󼤻�
// "module":85,"action":4,"request":[],"response":[Utils.UByteUtil]
// LibraryData.as:
//     this.result = param1[0];
// Example
//     [ 2(Mod_Library_Base::SUCCESS) ]
//============================================================================
Json::Value sxd_client::Mod_Library_Base_activate_library_upgrade() {
    Json::Value data;
    return this->send_and_receive(data, 85, 4);
}

//============================================================================
// 2019030101 �ؾ���תְ����
// "module":85,"action":2,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil]
// Example
//     [ 1(��ʥ) ] -> [ 2(Mod_Library_Base::SUCCESS) ]
//============================================================================
Json::Value sxd_client::Mod_Library_Base_up_library_level(int job_id) {
    Json::Value data;
    data.append(job_id);
    //return this->send_frame(data, 85, 2);
    return this->send_and_receive(data, 85, 2);
}
