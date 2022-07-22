#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Memoirs_Base {
public:
    static const int OPEN = 145;  // -111
    static const int CLOSE = 146; // -110
    static const int SUCCESS = 173;
    static const int SUCCEED = 197;
};

class Mod_SpringBigRun_Base {
public:
    static const int SUCCESS = 0;
};

class Mod_NewYearSmallActivity_Base {
public:
    static const int SUCCESS = 0;
};

class Mod_ChargeBack_Base {
public:
    static const int SUCCESS = 5;
};

class Mod_Regression_Base {
public:
    static const int SUCCESS = 0;
};

class Mod_FanFanLe_Base {
public:
    static const int AWARD_1 = 0;
    static const int AWARD_2 = 1;
    static const int NO_AWARD = 2;
    static const int SUCCESS = 3;
};

class Mod_SpringActivity_Base {
public:
    static const int SUCCESS = 14;
};

class Mod_LuckyPoker_Base {
public:
    static const int SUCCESS = 0;
};

class Mod_ActivityGame_Base {
public:
    static const int SUCCESS = 10;
};

class Mod_MidBackLottery_Base {
public:
    static const int SUCCESS = 2;
};

class Mod_ThanksFeedback_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// �´����껪
// 1. �´������; 2. ��������; 3. ����ת��; 4. ��������; 5. ������; 6. ���߽���
//============================================================================
void sxd_client::spring_carnival() {
    auto data = this->Mod_Memoirs_Base_is_activity_open();
    auto boards = data[0];
    for (auto& board : boards) {
        auto& board_info = board[0];
        auto board_type = board[1].asInt();
        /* 160: �´����껪, SPRING_CARNIVAL */
        if (board_type == 160) {
            for (auto& activity : board_info) {
                auto activity_type = activity[0].asInt();
                auto is_open = activity[1].asInt();
                /* 145: �´������, spring_big_run */
                if (activity_type == 145 && is_open == Mod_Memoirs_Base::OPEN)
                    spring_big_run();
                /* 78: ��������, spring_open_year */
                if (activity_type == 78 && is_open == Mod_Memoirs_Base::OPEN)
                    spring_open_year();
                /* 7: ����ת��, assistant_turntable_two */
                if (activity_type == 7 && is_open == Mod_Memoirs_Base::OPEN) {
                    assistant_turntable_two();
                    charge_back();
                }
                /* 21: ��������, regression_dice */
                if (activity_type == 21 && is_open == Mod_Memoirs_Base::OPEN)
                    regression_dice();
                /* 9: ������, fan_fan_le */
                if (activity_type == 9 && is_open == Mod_Memoirs_Base::OPEN)
                    fan_fan_le();
                /* 35: ���߽���, online_award */
                if (activity_type == 35 && is_open == Mod_Memoirs_Base::OPEN)
                    online_award();
                /* 113: �����˿�, lucky_poker */
                if (activity_type == 113 && is_open == Mod_Memoirs_Base::OPEN)
                    lucky_poker();
                /* 43: �ƽ��, golden_worker */
                if (activity_type == 43 && is_open == Mod_Memoirs_Base::OPEN)
                    golden_worker();
                /* 99: �»����齱, mid_back_lottery */
                if (activity_type == 99 && is_open == Mod_Memoirs_Base::OPEN)
                    mid_back_lottery();

                /* 161: R200�ж�����, thanks_feedback */
                if (activity_type == 161 && is_open == Mod_Memoirs_Base::OPEN)
                    thanks_feedback();

            }
            break;
        }
    }
}

//============================================================================
// �´������
//============================================================================
void sxd_client::spring_big_run() {
    auto data = this->Mod_SpringBigRun_Base_panel_info();
    int total_activity = data[0].asInt();
    int left_can_gain_count = data[1].asInt();
    auto total_award_list = data[3];

    // �츣��
    while (left_can_gain_count) {
        data = this->Mod_SpringBigRun_Base_gain_award();
        if (data[0].asInt() != Mod_SpringBigRun_Base::SUCCESS) {
            common::log(boost::str(boost::format("���´�����͡��츣��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            break;
        }
        common::log(boost::str(boost::format("���´�����͡��츣��")), iEdit);
        data = this->Mod_SpringBigRun_Base_panel_info();
        left_can_gain_count = data[1].asInt();
    }

    // ����
    int id = 0;
    if (total_activity >= 500) {
        id = 1;
        if (std::find_if(total_award_list.begin(), total_award_list.end(), [id](const Json::Value& x) {return x[0].asInt() == id;}) == total_award_list.end()) {
            data = this->Mod_SpringBigRun_Base_gain_total_award(id);
            if (data[0].asInt() != Mod_SpringBigRun_Base::SUCCESS)
                common::log(boost::str(boost::format("���´�����͡�����һʧ�ܣ�result[%1%]") % data[0]), iEdit);
            else
                common::log(boost::str(boost::format("���´�����͡�����һ")), iEdit);
        }
    }
    if (total_activity >= 1000) {
        id = 2;
        if (std::find_if(total_award_list.begin(), total_award_list.end(), [id](const Json::Value& x) {return x[0].asInt() == id;}) == total_award_list.end()) {
            data = this->Mod_SpringBigRun_Base_gain_total_award(id);
            if (data[0].asInt() != Mod_SpringBigRun_Base::SUCCESS)
                common::log(boost::str(boost::format("���´�����͡�������ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            else
                common::log(boost::str(boost::format("���´�����͡�������")), iEdit);
        }
    }
    if (total_activity >= 2000) {
        id = 3;
        if (std::find_if(total_award_list.begin(), total_award_list.end(), [id](const Json::Value& x) {return x[0].asInt() == id;}) == total_award_list.end()) {
            data = this->Mod_SpringBigRun_Base_gain_total_award(id);
            if (data[0].asInt() != Mod_SpringBigRun_Base::SUCCESS)
                common::log(boost::str(boost::format("���´�����͡�������ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            else
                common::log(boost::str(boost::format("���´�����͡�������")), iEdit);
        }
    }
}

//============================================================================
// ���
// "module":457,"action":0,"request":[],"response":[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil]]
// SpringBigRunData.as:
//     oObject.list(param1,this.bigRunObject,["total_activity","left_can_gain_count","item_list","total_award_list"]);
//     for each(_loc4_ in this.bigRunObject.item_list)
//         _loc2_ = this.parseItem(_loc4_,["item_id","item_count"]);
// Example
//     �ۼƻ�Ծ��, ʣ����ȡ����, item_list                 �����¼, 1: 500; 2:1000; 3:2000
//     [ 1540, 18, [ [ 1747, 1200000 ], [ 3418, 10 ] ], [ [ 1 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_SpringBigRun_Base_panel_info() {
    Json::Value data;
    return this->send_and_receive(data, 457, 0);
}

//============================================================================
// �츣��
// "module":457,"action":1,"request":[],"response":[Utils.UByteUtil]
// SpringBigRunData.as:
//     this.result = param1[0];
// Example
//     [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_SpringBigRun_Base_gain_award() {
    Json::Value data;
    return this->send_and_receive(data, 457, 1);
}

//============================================================================
// ����
// "module":457,"action":2,"request":[Utils.IntUtil],"response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil]
// SpringBigRunData.as:
//     this.result = param1[0];
//     for each(_loc2_ in param1[1])
//         _loc3_ = this.parseItem(_loc2_,["item_id","item_count"]);
//     this.bagAwardObject.id = param1[2];
// Example
//     [ 2 ] -> [ 0, [ [ 3345, 30 ] ], 2 ]
//============================================================================
Json::Value sxd_client::Mod_SpringBigRun_Base_gain_total_award(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 457, 2);
}

//============================================================================
// ��������
//============================================================================
void sxd_client::spring_open_year() {
    for (int id = 1; id <= 7; ++id) {
        auto data = this->Mod_NewYearSmallActivity_Base_get_open_year_award(id);
        if (data[0].asInt() == Mod_NewYearSmallActivity_Base::SUCCESS)
            common::log(boost::str(boost::format("������������������ [%1%]") % id), iEdit);
    }
}

//============================================================================
// ��������
// "module":340,"action":3,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.ByteUtil,Utils.ByteUtil]]
// NewYearSmallActivityData.as
//     this.result = param1[0];
// Example
//     [ 3 ] -> [ 0, [ [ 3345, 11 ], [ 347, 20 ] ], [ [ 1, 1 ], [ 2, 1 ], [ 3, 1 ], [ 4, 0 ], [ 5, 0 ], [ 6, 0 ], [ 7, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_NewYearSmallActivity_Base_get_open_year_award(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 340, 3);
}

//============================================================================
// ����ת��
//============================================================================
void sxd_client::assistant_turntable_two() {
    for (;;) {
        auto data = this->Mod_Memoirs_Base_assistant_turntable_lottery_two();
        if (data[0].asInt() == Mod_Memoirs_Base::SUCCESS)
            common::log(boost::str(boost::format("������ת�̡��齱")), iEdit);
        else
            break;
    }
}

//============================================================================
// �齱
// "module":225,"action":72,"request":[],"response":[Utils.UByteUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil]
// MemoirsData.as
//     this._fourYearMascotResult.result = param1[0];
// Example
//     [ 173, 1, 5, 1 ]
//============================================================================
Json::Value sxd_client::Mod_Memoirs_Base_assistant_turntable_lottery_two() {
    Json::Value data;
    return this->send_and_receive(data, 225, 72);
}

//============================================================================
// ������
// ժ����(56)��ҩ԰(15)�����ȡ��(90)���ɽ�������(94)���в����(26)���һ�����(35)
//============================================================================
void sxd_client::charge_back() {
    int function_ids[] = { 56, 15, 90, 94, 26, 35 };
    for (int function_id : function_ids) {
        std::string function_name = db.get_code(version, "Function", function_id)["text"];
        auto data = this->Mod_ChargeBack_Base_activate_fuction_of_mascot_befall(function_id);
        if (data[0].asInt() == Mod_ChargeBack_Base::SUCCESS)
            common::log(boost::str(boost::format("�������١����� [%1%]") % function_name), iEdit);
    }
}

//============================================================================
// ����
// "module":155,"action":8,"request":[Utils.ShortUtil],"response":[Utils.UByteUtil]
// Example
//     [ 15 ] -> [ 5 ]
//============================================================================
Json::Value sxd_client::Mod_ChargeBack_Base_activate_fuction_of_mascot_befall(int function_id) {
    Json::Value data;
    data.append(function_id);
    return this->send_and_receive(data, 155, 8);
}

//============================================================================
// ��������
//============================================================================
void sxd_client::regression_dice() {
    for (int i = 0; i < 5; ++i) {
        auto data = this->Mod_Regression_Base_dice();
        if (data[0].asInt() == Mod_Regression_Base::SUCCESS)
            common::log(boost::str(boost::format("���������ӡ���һ��")), iEdit);
        int today_can_use_times = data[3].asInt();
        if (!today_can_use_times)
            break;
    }
}

//============================================================================
// 2019030101 ��һ��
// "module":254,"action":1,"request":[],"response":[Utils.UByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]
// RegressionActivityData.as:
//     this._regressionDiceInfo["result"] = param1[0];
//     this._regressionDiceInfo["dice_number"] = param1[1];
//     this._regressionDiceInfo["cur_grid_pos"] = param1[2];
//     this._regressionDiceInfo["today_can_use_times"] = param1[3];
//     this._regressionDiceInfo["award_item_id"] = param1[4];
//     this._regressionDiceInfo["award_item_number"] = param1[5];
// Example
//     [ 0, 5, 6, 4, 4519, 15 ]
//     [ 0, 3, 9, 3, 2452, 5 ]
//============================================================================
Json::Value sxd_client::Mod_Regression_Base_dice() {
    Json::Value data;
    return this->send_and_receive(data, 254, 1);
}

//============================================================================
// 2019030101 ������
//============================================================================
void sxd_client::fan_fan_le() {
    for (int i = 0; i < 10; ++i) {
        auto data = this->Mod_FanFanLe_Base_get_info();
        int avaliable_times = data[0].asInt();
        int extra_award = data[5].asInt();

        // ��ȡ����
        if (extra_award != Mod_FanFanLe_Base::NO_AWARD) {
            data = this->Mod_FanFanLe_Base_get_extra_award();
            if (data[0].asInt() != Mod_FanFanLe_Base::SUCCESS) {
                common::log(boost::str(boost::format("�������֡���ȡ����ʧ�ܣ�result[%1%]") % data[0]), iEdit);
                return;
            }
            common::log(boost::str(boost::format("�������֡���ȡ�� %1% ������") % (extra_award + 1)), iEdit);
            --i;
            continue;
        }

        // ����
        if (!avaliable_times)
            break;
        data = this->Mod_FanFanLe_Base_flop(i % 5 + 1);
        if (data[0].asInt() == Mod_FanFanLe_Base::SUCCESS)
            common::log(boost::str(boost::format("�������֡����� %1% ����") % (i % 5 + 1)), iEdit);
    }
}

//============================================================================
// 2019030101 ���
// "module":241,"action":0,"request":[],"response":[Utils.ByteUtil,Utils.ByteUtil,[Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.UByteUtil]
// MemoirsData.as:
//     this._partnerCardInfo["avaliable_times"] = param1[0];
//     this._partnerCardInfo["extra_award"] = param1[5];
//     this._partnerCardInfo["haveExtrAward"] = param1[5] != Mod_FanFanLe_Base.NO_AWARD;
// Example
//     [ 6, 10,
//         [ [ 1, 1 ], [ 2, 3 ], [ 3, 3 ], [ 4, 4 ] ],
//         [ [ 1739, 500 ], [ 1740, 10 ], [ 4063, 3 ], [ 3399, 5 ], [ 1747, 1000000 ], [ 347, 50 ], [ 2016, 2 ] ],
//         [ [ 1739, 200 ], [ 1740, 5 ], [ 4063, 1 ], [ 347, 20 ], [ 1747, 300000 ] ],
//     2 ]
//============================================================================
Json::Value sxd_client::Mod_FanFanLe_Base_get_info() {
    Json::Value data;
    return this->send_and_receive(data, 241, 0);
}

//============================================================================
// 2019030101 ����
// "module":241,"action":1,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// MemoirsData.as:
//     this._partnerCardInfo["flop_result"] = param1[0];
//     this._partnerCardInfo["card_id"] = param1[1];
//     for each(_loc2_ in param1[2])
//        _loc3_["item_id"] = _loc2_[0];
//        _loc3_["count"] = _loc2_[1];
// Example
//     [ 5 ] -> [ 3, 1, [ [ 1743, 8 ], [ 1747, 200000 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_FanFanLe_Base_flop(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 241, 1);
}

//============================================================================
// 2019030101 ��ȡ����
// "module":241,"action":2,"request":[],"response":[Utils.UByteUtil]
// MemoirsData.as:
//     this._partnerCardInfo["getAward_result"] = param1[0];
// Example
//     [ 3 ]
//============================================================================
Json::Value sxd_client::Mod_FanFanLe_Base_get_extra_award() {
    Json::Value data;
    return this->send_and_receive(data, 241, 2);
}

//============================================================================
// ���߽���
//============================================================================
void sxd_client::online_award() {
    for (int i = 1; i <= 4; ++i) {
        auto data = this->Mod_SpringActivity_Base_get_online_award(i);
        if (data[0].asInt() == Mod_SpringActivity_Base::SUCCESS)
            common::log(boost::str(boost::format("�����߽������齱 [%1%]") % i), iEdit);
    }
}

//============================================================================
// 2019030101 ���
// "module":218,"action":31,"request":[],"response":[Utils.IntUtil,[Utils.ByteUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// SpringActivityData.as:
//     this._onLineAwardInfo["online_time"] = param1[0];
//     for each(_loc3_ in param1[1])
//         _loc4_["id"] = _loc3_[0];
//         _loc4_["get_status"] = _loc3_[1];
//         for each(_loc6_ in _loc3_[2])
//             _loc7_["award_item_id"] = _loc6_[0];
//             _loc7_["award_num"] = _loc6_[1];
//         _loc4_["item_id"] = _loc3_[3];
//         _loc4_["num"] = _loc3_[4];
//     _loc4_["extra_item_id"] = param1[2];
//     _loc4_["extra_ingot_num"] = param1[3];
//     this._onLineAwardInfo.rate = param1[4] / 10;
// Example
//     [ 3253, [ [ 1, 46, [ [ 1740, 3 ], [ 347, 10 ], [ 1747, 2000000 ], [ 1488(����), 4 ] ], 347, 10 ],
//               [ 2, 47, [ [ 1740, 5 ], [ 347, 20 ], [ 1747, 3000000 ], [ 5822, 1 ] ], 347, 20 ],
//               [ 3, 47, [ [ 1740, 8 ], [ 4649, 2 ], [ 1747, 4000000 ], [ 1488, 8 ], [ 2403, 200 ], [ 2343, 5 ], [ 5822, 1 ] ], 1747, 1600000 ],
//               [ 4, 47, [ [ 1740, 10 ], [ 4649, 3 ], [ 1747, 4000000 ], [ 1488, 10 ], [ 2403, 300 ], [ 4236, 1 ], [ 5822, 2 ] ], 1488, 10 ] ],
//       3345(Ԫ��), 2, 10 ]
//============================================================================
Json::Value sxd_client::Mod_SpringActivity_Base_get_online_award_info() {
    Json::Value data;
    return this->send_and_receive(data, 218, 31);
}

//============================================================================
// 2019030101 �齱
// "module":218,"action":32,"request":[Utils.ByteUtil],"response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// SpringActivityData.as:
//     this._onLineAwardResult["result"] = param1[0];
//     this._onLineAwardResult["item_id"] = param1[1];
//     this._onLineAwardResult["num"] = param1[2];
//     this._onLineAwardResult["extra_item_id"] = param1[3];
//     this._onLineAwardResult["extra_ingot_num"] = param1[4];
//     this._onLineAwardResult["monkey_coin_num"] = param1[5];
// Example
//     [ 1 ] -> [ 14, 1488, 4, 3345, 0, 12 ]
//============================================================================
Json::Value sxd_client::Mod_SpringActivity_Base_get_online_award(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 218, 32);
}

//============================================================================
// �����˿�
//============================================================================
void sxd_client::lucky_poker() {
    for (;;) {
        auto data = this->Mod_LuckyPoker_Base_get_info();
        int exchange_times = data[0].asInt();
        auto poker_list = data[5];
        if (!exchange_times)
            break;
        for (unsigned i = 0; i < poker_list.size(); i++) {
            data = this->Mod_LuckyPoker_Base_lottery_draw();
            if (data[0].asInt() == Mod_LuckyPoker_Base::SUCCESS)
                common::log(common::sprintf("�������˿ˡ���ȡһ���˿��ƣ���û��� [%d]", data[2].asInt()), iEdit);
        }
        data = this->Mod_LuckyPoker_Base_exchange_award();
        if (data[0].asInt() == Mod_LuckyPoker_Base::SUCCESS)
            common::log(common::sprintf("�������˿ˡ���ȡ����"), iEdit);
        else
            break;
    }
    auto data = this->Mod_Memoirs_Base_get_exchange_item_info(9);
    auto award_list = data[0];
    for (auto& award : award_list) {
        int award_id = award[0].asInt();
        int times = award[1].asInt();
        for (int i = 0; i < times; i++) {
            data = this->Mod_Memoirs_Base_exchange_item_award(9, award_id);
            if (data[0].asInt() == Mod_Memoirs_Base::SUCCEED)
                common::log(common::sprintf("�������˿ˡ����ֶһ����� [%d]", award_id), iEdit);
            else
                break;
        }

    }
}

//============================================================================
// �����˿�-���
// "module":405,"action":0,"request":[],
// Example
//     [ 3(exchange_times), 0(jifen), [ [ 1, [ [ 5823, 1 ] ] ], [ 2, [ [ 347, 50 ] ] ], [ 3, [ [ 4982, 1 ] ] ], [ 4, [ [ 347, 20 ] ] ], [ 5, [ [ 1743, 40 ] ] ], [ 6, [ [ 1740, 8 ] ] ] ], 15, 0, [ [ 1 ], [ 2 ], [ 3 ], [ 4 ], [ 5 ], [ 6 ] ](poker_list) ]
//============================================================================
Json::Value sxd_client::Mod_LuckyPoker_Base_get_info() {
    Json::Value data;
    return this->send_and_receive(data, 405, 0);
}

//============================================================================
// �����˿�-��ȡ�˿���
// "module":405,"action":1,"request":[],
// Example
//     [ 0(Mod_LuckyPoker_Base::SUCCESS), 4, 200(get_jifen) ]
//============================================================================
Json::Value sxd_client::Mod_LuckyPoker_Base_lottery_draw() {
    Json::Value data;
    return this->send_and_receive(data, 405, 1);
}

//============================================================================
// �����˿�-��ȡ����
// "module":405,"action":2,"request":[],
// Example
//     [ 0(Mod_LuckyPoker_Base::SUCCESS), [ [ 347, 20 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_LuckyPoker_Base_exchange_award() {
    Json::Value data;
    return this->send_and_receive(data, 405, 2);
}

//============================================================================
// ���ֶһ�����-���
// "module":225,"action":85,"request":[Utils.ShortUtil],
// Example
//     [ 9(�����˿���) ] -> [ [ [ 71(�޻�), 1, 1 ], [ 74, 3, 50 ], [ 72, 3, 10 ], [ 70, 1, 1 ], [ 73, 2, 10 ], [ 75(�ǻ�), 3, 20 ], [ 76, 3, 20 ] ], 4200 ]
//============================================================================
Json::Value sxd_client::Mod_Memoirs_Base_get_exchange_item_info(int reflection_id) {
    Json::Value data;
    data.append(reflection_id);
    return this->send_and_receive(data, 225, 85);
}

//============================================================================
// ���ֶһ�����-�һ�
// "module":225,"action":86,"request":[Utils.ShortUtil,Utils.IntUtil],"response":[Utils.UByteUtil]
// Example
//     [ 9(�����˿���), 71(�޻�) ] -> [ 197(Mod_Memoirs_Base::SUCCEED) ]
//============================================================================
Json::Value sxd_client::Mod_Memoirs_Base_exchange_item_award(int reflection_id, int award_id) {
    Json::Value data;
    data.append(reflection_id);
    data.append(award_id);
    return this->send_and_receive(data, 225, 86);
}

//============================================================================
// �ƽ��
//============================================================================
void sxd_client::golden_worker() {
    for (;;) {
        auto data = this->Mod_ActivityGame_Base_start_play_kuang_gong();
        if (data[0] != Mod_ActivityGame_Base::SUCCESS)
            break;
        data = this->Mod_ActivityGame_Base_kuang_gong_game_over(650);
        if (data[0] != Mod_ActivityGame_Base::SUCCESS)
            break;
        int score = data[1].asInt();
        int total_score = data[2].asInt();
        common::log(common::sprintf("���ƽ�󹤡��ڿ��û��� [%d]�����չ���û��� [%d]", score, total_score), iEdit);
        if (total_score >= 1000)
            break;
    }
    auto data = this->Mod_ActivityGame_Base_player_score_exchange_info();
    auto award_list = data[1];
    for (auto& award : award_list) {
        int award_id = award[0].asInt();
        for (;;) {
            data = this->Mod_ActivityGame_Base_exchange_award(award_id);
            if (data[0] == Mod_ActivityGame_Base::SUCCESS)
                common::log(common::sprintf("���ƽ�󹤡����ֶһ����� [%d]", award_id), iEdit);
            else
                break;
        }
    }
}

//============================================================================
// �ƽ��-��ʼ��Ϸ
// "module":194,"action":25,"request":[],
// Example
//     [ 10(Mod_ActivityGame_Base::SUCCESS), 40(current_score,�ܻ���), 25(plane_score,�����õ��ܻ���), 1000(score_limit) ]
//============================================================================
Json::Value sxd_client::Mod_ActivityGame_Base_start_play_kuang_gong() {
    Json::Value data;
    return this->send_and_receive(data, 194, 25);
}

//============================================================================
// �ƽ��-������Ϸ
// "module":194,"action":26,"request":[Utils.IntUtil],
// Example
//     [ 580 ] -> [ 10(Mod_ActivityGame_Base::SUCCESS), 58(score,�˴λ�õĻ���), 83(total_score,�����õ��ܻ���) ]
//============================================================================
Json::Value sxd_client::Mod_ActivityGame_Base_kuang_gong_game_over(int golden) {
    Json::Value data;
    data.append(golden);
    return this->send_and_receive(data, 194, 26);
}

//============================================================================
// �ƽ��-���ֶһ����
// "module":194,"action":7,"request":[],
// Example
//     [ 1000, [ [ 7(award_id), 2395, 10, 300, 0, 3 ], [ 3, 1740, 10, 200, 0, 3 ], [ 18, 2616, 5, 500, 0, 2 ], [ 17, 2615, 5, 350, 0, 2 ], [ 20, 2682, 5, 300, 0, 3 ], [ 16, 2614, 5, 200, 0, 2 ], [ 1, 1747, 800000, 50, 0, 3 ], [ 21, 2397, 10, 1000, 0, 1 ], [ 22, 4714, 1, 1200, 0, 1 ], [ 9, 2407, 1, 120, 0, 3 ], [ 14, 2625, 1, 1800, 0, 1 ], [ 8, 2263, 10, 300, 0, 3 ], [ 11, 1845, 10, 60, 0, 3 ], [ 5, 2452, 20, 400, 0, 3 ], [ 4, 347, 20, 100, 0, 3 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_ActivityGame_Base_player_score_exchange_info() {
    Json::Value data;
    return this->send_and_receive(data, 194, 7);
}

//============================================================================
// �ƽ��-�һ�
// "module":194,"action":8,"request":[Utils.ShortUtil],
// Example
//     [ 7 ] -> [ 10(Mod_ActivityGame_Base::SUCCESS), 7, 2, 700 ]
//============================================================================
Json::Value sxd_client::Mod_ActivityGame_Base_exchange_award(int award_id) {
    Json::Value data;
    data.append(award_id);
    return this->send_and_receive(data, 194, 8);
}

//============================================================================
// �»����齱
//============================================================================
void sxd_client::mid_back_lottery() {
    for (;;) {
        auto data = this->Mod_MidBackLottery_Base_get_back_lottery_info();
        int remain_free_count = data[2].asInt();
        if (!remain_free_count)
            break;
        std::vector<Json::Value> box_infos;
        std::copy_if(data[4].begin(), data[4].end(), std::back_inserter(box_infos), [](const Json::Value& x) {return x[1].asInt();});
        if (!box_infos.size())
            break;
        int id = box_infos[rand() % box_infos.size()][0].asInt();
        data = this->Mod_MidBackLottery_Base_back_lottery(id);
        if (data[0].asInt() != Mod_MidBackLottery_Base::SUCCESS)
            break;
        common::log(common::sprintf("���»����齱���齱�� [%d] �����", id), iEdit);
    }
    auto data = this->Mod_MidBackLottery_Base_get_memento_coin_exchange_info();
    auto infos = data[0];
    for (const auto info : infos) {
        int item_id = info[0].asInt();
        int is_exchange = info[3].asInt();
        if (!is_exchange)
            continue;
        data = this->Mod_MidBackLottery_Base_memento_coin_exchange(item_id);
        if (data[0].asInt() == Mod_MidBackLottery_Base::SUCCESS)
            common::log(common::sprintf("���»����齱���һ���Ʒ [%d]", item_id), iEdit);
    }
}

//============================================================================
// �»����齱-���
// "module":378,"action":0,
// Example
//     [ 0, 17, 2(remain_free_count), [ [ 1740, 50, 1 ], [ 1747, 8, 1 ], [ 4982, 3, 1 ], [ 347, 120, 1 ], [ 1743, 300, 1 ], [ 5773, 3, 1 ] ],
//         box_info: [ [ 1(id), 1(is_open) ], [ 2, 1 ], [ 3, 0 ], [ 4, 1 ], [ 5, 1 ], [ 6, 1 ], [ 7, 1 ], [ 8, 1 ], [ 9, 1 ], [ 10, 1 ], [ 11, 1 ], [ 12, 1 ], [ 13, 1 ], [ 14, 1 ], [ 15, 1 ], [ 16, 1 ], [ 17, 1 ], [ 18, 1 ] ], 82350951 ]
//     [ 0, 16, 1(remain_free_count), [ [ 1740, 50, 1 ], [ 1747, 8, 1 ], [ 4982, 3, 1 ], [ 347, 120, 1 ], [ 1743, 300, 1 ], [ 5773, 3, 1 ] ],
//         box_info: [ [ 1(id), 1(is_open) ], [ 2, 1 ], [ 3, 0 ], [ 4, 1 ], [ 5, 1 ], [ 6, 1 ], [ 7, 1 ], [ 8, 1 ], [ 9, 0 ], [ 10, 1 ], [ 11, 1 ], [ 12, 1 ], [ 13, 1 ], [ 14, 1 ], [ 15, 1 ], [ 16, 1 ], [ 17, 1 ], [ 18, 1 ] ], 83350951 ]
//============================================================================
Json::Value sxd_client::Mod_MidBackLottery_Base_get_back_lottery_info() {
    Json::Value data;
    return this->send_and_receive(data, 378, 0);
}

//============================================================================
// �»����齱-�齱
// "module":378,"action":1,"request":[Utils.IntUtil],
// Example
//     [ 9 ] -> [ 2(Mod_MidBackLottery_Base::SUCCESS), 1411(item_id), 6(item_num) ]
//============================================================================
Json::Value sxd_client::Mod_MidBackLottery_Base_back_lottery(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 378, 1);
}

//============================================================================
// �»����齱-�һ�
// "module":378,"action":2,"request":[Utils.ShortUtil],
// Example
//
//============================================================================
Json::Value sxd_client::Mod_MidBackLottery_Base_memento_coin_exchange(int item_id) {
    Json::Value data;
    data.append(item_id);
    return this->send_and_receive(data, 378, 2);
}

//============================================================================
// �»����齱-�һ����
// "module":378,"action":3,
// Example
//     [ [ [ 5104(item_id), 10, 10, 1(is_exchange) ], [ 1787, 10, 10, 1 ], [ 4431, 1, 10, 1 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_MidBackLottery_Base_get_memento_coin_exchange_info() {
    Json::Value data;
    return this->send_and_receive(data, 378, 3);
}

//============================================================================
// R200�ж�����
//============================================================================
void sxd_client::thanks_feedback() {
    for (int id = 1; id <= 7; id++) {
        auto data = this->Mod_ThanksFeedback_Base_get_award(id);
        if (data[0].asInt() == Mod_ThanksFeedback_Base::SUCCESS)
            common::log(common::sprintf("��R200�ж���������ȡ�� [%d] �콱��", id), iEdit);
    }
}

//============================================================================
// R200�ж�����
// "module":464,"action":1,
// Example
//     [ 4(login_day), [ [ 1, 20, 20, 1(����ȡ) ], [ 2, 20, 20, 0(δ��ȡ) ], [ 3, 20, 20, 0 ], [ 4, 30, 30, 0 ], [ 5, 30, 30, 0 ], [ 6, 30, 30, 0 ], [ 7, 50, 50, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_ThanksFeedback_Base_open_panel() {
    Json::Value data;
    return this->send_and_receive(data, 464, 1);
}

//============================================================================
// R200�ж�����
// "module":464,"action":2,"request":[Utils.IntUtil],
// Example
//     [ 1(1-7) ] -> [ 0(Mod_ThanksFeedback_Base::SUCCESS) ]
//============================================================================
Json::Value sxd_client::Mod_ThanksFeedback_Base_get_award(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 464, 2);
}
