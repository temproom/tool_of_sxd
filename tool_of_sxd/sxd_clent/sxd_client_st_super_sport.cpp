#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_StSuperSport_Base {
public:
    static const int GROUP_TIAN_BANG = 1;
    static const int GROUP_DI_BANG = 2;
    static const int GROUP_FINALLY = 3;
    static const int ST_SUPER_SPORT_WAR_CLOSE = 4;
    static const int ST_SUPER_SPORT_WAR_PREPARE = 5;
    static const int ST_SUPER_SPORT_WAR_16 = 6;
    static const int ST_SUPER_SPORT_WAR_16_COMPLETE = 7;
    static const int ST_SUPER_SPORT_WAR_8 = 8;
    static const int ST_SUPER_SPORT_WAR_8_COMPLETE = 9;
    static const int ST_SUPER_SPORT_WAR_4 = 10;
    static const int ST_SUPER_SPORT_WAR_4_COMPLETE = 11;
    static const int ST_SUPER_SPORT_WAR_2 = 12;
    static const int ST_SUPER_SPORT_WAR_2_COMPLETE = 13;
    static const int ST_SUPER_SPORT_WAR_1 = 14;
    static const int ST_SUPER_SPORT_WAR_1_COMPLETE = 15;
    static const int ST_SUPER_SPORT_WAR_0 = 16;
    static const int ST_SUPER_SPORT_WAR_0_COMPLETE = 17;
    static const int ST_SUPER_SPORT_WAR_START = 18;
    static const int POINT_RACE_FIRST_DAY = 21;
    static const int POINT_RACE_SECOND_DAY = 22;
    static const int POINT_RACE_THIRD_DAY = 23;
    static const int POINT_RACE_FOUR_DAY = 24;
    static const int WAR_RACE = 25;
    static const int YES = 26;
    static const int NO = 27;
    static const int SUCCESS = 28;
    static const int NO_ENOUGH_CHALLENGE_TIMES = 33;
};

//============================================================================
// R172 ��������
//============================================================================
void sxd_client::get_rank_award(sxd_client* sxd_client_town) {
    //try {
    // get player is_get_award
    Json::Value data = this->Mod_StSuperSport_Base_get_player_st_super_sport();
    if (data[5] == Mod_StSuperSport_Base::YES)
        return;

    // get player level
    data = sxd_client_town->Mod_Player_Base_get_player_info();
    int level = data[1].asInt();

    // get rank award
    data = this->Mod_StSuperSport_Base_get_rank_award(level);
    if (data[0].asInt() != Mod_StSuperSport_Base::SUCCESS) {
        common::log(boost::str(boost::format("����ħ��������ȡ��������ʧ�ܣ�result[%1%]") % data[0]), iEdit);
        return;
    }
    common::log(boost::str(boost::format("����ħ��������ȡ����������[ͭǮ��%1%]��[������%2%]��[Ѫ��������%3%]") % data[1] % data[2] % data[3]), iEdit);
    //} catch (const std::exception& ex) {
    //    common::log(boost::str(boost::format("���ִ���(get rank award)��%1%") % ex.what()));
    //}
}

//============================================================================
// R172 ��ħ����������Ϣ
// {module:115, action:1, request:[], response:[Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.UByteUtil, Utils.ShortUtil]};
// StSuperSportData.as 81:
//     oObject.list(param1, this._getPlayerStSuperSport, ["st_super_sport_rank", "continue_win_times", "remain_challenge_times", "cd_time", "clear_cd_ingot", "is_get_award", "today_buy_times"]);
// Example
//
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_get_player_st_super_sport() {
    Json::Value data;
    return this->send_and_receive(data, 115, 1);
}

//============================================================================
// R172 ��������
// {module:115, action:9,
// request:[Utils.ShortUtil],
// StSuperSportInfoView.as 930:
//     _data.call(Mod_StSuperSport_Base.get_rank_award, this.onGetAwardCallBack, [_ctrl.player.level], true, 1);
// Example
//
// response:[Utils.UByteUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ShortUtil]};
// StSuperSportData.as 727:
//     oObject.list(param1, this._rankAward, ["result", "award_coin", "award_fame", "award_blood"]);
// Example
//
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_get_rank_award(int level) {
    Json::Value data;
    data.append(level);
    return this->send_and_receive(data, 115, 9);
}

//============================================================================
// R172 ��ħ����
//============================================================================
void sxd_client::get_score_award() {
    //try {
    // can get score award
    Json::Value data = this->Mod_StSuperSport_Base_can_get_score_award();
    if (data[0].asInt() != Mod_StSuperSport_Base::YES)
        return;

    // player score award information
    data = this->Mod_StSuperSport_Base_player_score_award_info();
    int self_score = data[0].asInt();
    Json::Value award_info = data[1];

    // player get score award
    Json::Value awards;
    std::istringstream("[[1, \"��ͨ����\", 1100, 5, 20, 300000, 500, 2], [2, \"��ͨ����\", 1300, 8, 50, 500000, 600, 2], [3, \"��ͨ����\", 1500, 10, 100, 1000000, 800, 2], [4, \"��ͨ����\", 1800, 20, 200, 1200000, 1000, 2], [5, \"��ͨ����\", 2000, 30, 300, 1500000, 1200, 2], [6, \"��ͭ����\", 2200, 50, 500, 2000000, 1500, 3], [7, \"��������\", 2400, 80, 700, 3000000, 1600, 4], [8, \"�ƽ���\", 2600, 100, 1000, 5000000, 1800, 5]]") >> awards;
    for (const auto& award : awards) {
        if (award[2].asInt() > self_score)
            break;
        int index = award[0].asInt();
        auto award_info_select = std::find_if(award_info.begin(), award_info.end(), [index](const Json::Value& x) {return x[0].asInt()==index;});
        if (award_info_select == award_info.end()) {
            common::log(boost::str(boost::format("����ħ��������ȡ [��ħ����] �����쳣��index[%1%]") % index), iEdit);
            return;
        }
        if ((*award_info_select)[1] == Mod_StSuperSport_Base::YES)
            continue;
        data = this->Mod_StSuperSport_Base_player_get_score_award(index);
        if (data[0].asInt() != Mod_StSuperSport_Base::SUCCESS) {
            common::log(boost::str(boost::format("����ħ��������ȡ [��ħ����] ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("����ħ��������ȡ [��ħ����]��[ͭǮ��%1%]��[������%2%]��[Ѫ��������%3%]") % award[5] % award[6] % award[3]), iEdit);
    }
    //} catch (const std::exception& ex) {
    //    common::log(boost::str(boost::format("���ִ���(get score award)��%1%") % ex.what()));
    //}
}

//============================================================================
// R172 ��ħ����
// {module:115, action:11, request:[], response:[Utils.UByteUtil]};
// StSuperSportData.as 757:
//     this._canGetScoreAward = param1[0] == Mod_StSuperSport_Base.YES;
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_can_get_score_award() {
    Json::Value data;
    return this->send_and_receive(data, 115, 11);
}

//============================================================================
// R172 ��ħ�������
// {module:115, action:12, request:[], response:[Utils.ShortUtil, [Utils.ByteUtil, Utils.UByteUtil]]};
// StSuperSportData.as 771:
//     oObject.list(param1, this._playerScoreAwardInfo, ["self_score", "award_info"]);
//     for each (_loc_3 in this._playerScoreAwardInfo.award_info)
//         oObject.list(_loc_3, _loc_4, ["award_id", "is_get"]);
// Example
//     [ 1302, [ [ 1, 26 ], [ 5, 27 ], [ 7, 27 ], [ 8, 27 ], [ 4, 27 ], [ 2, 27 ], [ 3, 27 ], [ 6, 27 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_player_score_award_info() {
    Json::Value data;
    return this->send_and_receive(data, 115, 12);
}

//============================================================================
// R172 ��ȡ
// {module:115, action:13, request:[Utils.ByteUtil], response:[Utils.UByteUtil]};
// StSuperSportData.as 791:
//     this._playerGetScoreAwardResult = param1[0];
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_player_get_score_award(int index) {
    Json::Value data;
    data.append(index);
    return this->send_and_receive(data, 115, 13);
}

//============================================================================
// R172 ������
//============================================================================
void sxd_client::point_race(sxd_client* sxd_client_town) {
    //try {
    // get status
    Json::Value data = this->Mod_StSuperSport_Base_get_st_super_sport_status();
    if (data[0].asInt() < Mod_StSuperSport_Base::POINT_RACE_FIRST_DAY || data[0].asInt() > Mod_StSuperSport_Base::POINT_RACE_FOUR_DAY)
        return;
    common::log("����ħ���������� [������]", 0);

    // get player information
    data = this->Mod_StSuperSport_Base_get_player_st_super_sport();
    common::log(boost::str(boost::format("����ħ���������� [%1%]�����ջ�����ս [%2%] ��") % data[0] % data[2]), 0);
    if (data[2].asInt() == 0)
        return;
    // cd
    if (data[3].asInt()) {
        common::log("����ħ������CD��...", 0);
        return;
    }

    // challenge list
    data = this->Mod_StSuperSport_Base_challenge_list();
    // 1. sort
    std::vector<Json::Value> challenge_players;
    std::copy(data[0].begin(), data[0].end(), std::back_inserter(challenge_players));
    std::sort(challenge_players.begin(), challenge_players.end(), [](const Json::Value& x, const Json::Value& y) {return x[5].asInt()<y[5].asInt();});
    // 2. filter(AM)
    data = sxd_client_town->Mod_Player_Base_server_time();
    //auto server_time = boost::posix_time::from_time_t(data[0].asInt());
    std::time_t server_time = data[0].asInt();
    int hour = (std::stoi(common::to_string(server_time, "%H")) + 8) % 24;
    if (hour < 12) {
        //challenge_players.assign(challenge_players.begin(), challenge_players.begin() + 2);
        challenge_players.pop_back();
        challenge_players.pop_back();
        challenge_players.pop_back();
    }
    // 3. filter(is_challenge==Mod_StSuperSport_Base::NO)
    std::vector<Json::Value> challenge_players_valid;
    std::copy_if(challenge_players.begin(), challenge_players.end(), std::back_inserter(challenge_players_valid), [](const Json::Value& x) {return x[6].asInt()==Mod_StSuperSport_Base::NO;});
    if (challenge_players_valid.size() == 0)
        return;

    // challenge
    for (const auto& player : challenge_players_valid) {
        data = this->Mod_StSuperSport_Base_challenge(player[0].asInt());
        if (data[0].asInt() == Mod_StSuperSport_Base::NO_ENOUGH_CHALLENGE_TIMES)
            return;
        if (data[0].asInt() != Mod_StSuperSport_Base::SUCCESS) {
            common::log(boost::str(boost::format("����ħ��������ս [%1%(%2%)] ʧ�ܣ�result[%3%]") % common::utf2gbk(player[2].asString()) % player[1] % data[0]), iEdit);
            return;
        }
        if (data[8].asInt()) {
            common::log(boost::str(boost::format("����ħ��������ս [%1%(%2%)]��ս��") % common::utf2gbk(player[2].asString()) % player[1]), iEdit);
            return;
        }
        common::log(boost::str(boost::format("����ħ��������ս [%1%(%2%)]��սʤ") % common::utf2gbk(player[2].asString()) % player[1]), iEdit);
    }
    //} catch (const std::exception& ex) {
    //    common::log(boost::str(boost::format("���ִ���(point race)��%1%") % ex.what()));
    //}
}

//============================================================================
// R172 ������|��ħ��ս
// {module:115, action:0, request:[], response:[Utils.UByteUtil]};
// Example
//     ��������   [ 21-24 ]
//     ��ħ��ս��[ 25 ]
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_get_st_super_sport_status() {
    Json::Value data;
    return this->send_and_receive(data, 115, 0);
}

//============================================================================
// R172 ��ս������Ϣ
// {module:115, action:2, request:[], response:[[Utils.ByteUtil, Utils.IntUtil, Utils.StringUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.IntUtil, Utils.UByteUtil, Utils.StringUtil, Utils.StringUtil, Utils.ShortUtil], Utils.IntUtil]};
// StSuperSportData.as 110:
//     oObject.list(param1, this._challengeList, ["challenge_players", "next_refresh_time"]);
//     for each (_loc_3 in this._challengeList.challenge_players)
//         oObject.list(_loc_3, _loc_4, ["sequence_id", "st_player_id", "nickname", "role_id", "player_level", "attack_power", "is_challenge", "stage_name", "server_name", "equip_item_id"]);
// Example
//
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_challenge_list() {
    Json::Value data;
    return this->send_and_receive(data, 115, 2);
}

//============================================================================
// R172 ��ս
// {module:115, action:3,
// request:[Utils.ByteUtil],
// Example
//
// response:[Utils.UByteUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ShortUtil, [Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.StringUtil, [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil, Utils.StringUtil, Utils.IntUtil, Utils.LongUtil, Utils.LongUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ShortUtil], Utils.ByteUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, [Utils.ShortUtil], [Utils.ShortUtil], Utils.ShortUtil, Utils.ByteUtil, Utils.LongUtil, Utils.ByteUtil, Utils.IntUtil], [Utils.IntUtil], [Utils.IntUtil, Utils.IntUtil], Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.IntUtil, Utils.StringUtil, Utils.IntUtil, Utils.IntUtil, [Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil], Utils.IntUtil, [Utils.IntUtil, Utils.ByteUtil], Utils.LongUtil, Utils.LongUtil, [Utils.IntUtil], Utils.ByteUtil, [Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.IntUtil, Utils.IntUtil]]], [[Utils.ByteUtil, Utils.StringUtil, [Utils.UByteUtil, Utils.StringUtil], [Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil]], [Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, [Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil]], [Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil, [Utils.UByteUtil, Utils.ShortUtil, Utils.ByteUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], Utils.StringUtil, [Utils.IntUtil], [Utils.StringUtil], Utils.IntUtil, Utils.ShortUtil, Utils.LongUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.StringUtil, Utils.IntUtil, Utils.UByteUtil, [Utils.IntUtil, Utils.ShortUtil, [Utils.UByteUtil, Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], [Utils.StringUtil], [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.ShortUtil], Utils.StringUtil, [Utils.IntUtil], Utils.IntUtil, Utils.IntUtil, Utils.LongUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.IntUtil, Utils.ByteUtil], [Utils.IntUtil, Utils.StringUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil], [Utils.UByteUtil, Utils.StringUtil]], [Utils.IntUtil, Utils.IntUtil], [Utils.UByteUtil, Utils.StringUtil, [Utils.UByteUtil, Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil], [Utils.StringUtil]], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil], [Utils.IntUtil, Utils.LongUtil, Utils.IntUtil]], Utils.ShortUtil], Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.ByteUtil, Utils.StringUtil, Utils.StringUtil, Utils.ShortUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, Utils.IntUtil, [Utils.IntUtil, Utils.IntUtil]]};
// StSuperSportData.as 147:
//     oObject.list(param1, this._challengeResult, ["result", "award_coin", "award_fame", "award_score", "war_result", "st_super_sport_rank", "cur_score", "remain_challenge_times", "cd_time", "clear_cd_ingot", "award_blood_essence", "stage_name", "server_name", "continue_win_times", "award_dao_yuan", "award_gun", "scrapId", "scrapNum", "awardItemId", "awardCount", "war_report_id", "award_item_list"]);
// Example
//
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_challenge(int index) {
    Json::Value data;
    data.append(index);
    return this->send_and_receive(data, 115, 3);
}

//============================================================================
// R172 ��ħ��ս
//============================================================================
void sxd_client::war_race(sxd_client* sxd_client_town) {
    //try {
    // get status
    Json::Value data = this->Mod_StSuperSport_Base_get_st_super_sport_status();
    if (data[0].asInt() != Mod_StSuperSport_Base::WAR_RACE)
        return;
    common::log("����ħ���������� [��ħ��ս]", 0);

    // get race step
    data = this->Mod_StSuperSport_Base_get_race_step();
    if (data[10].asInt()) {
        common::log("����ħ����������ע", 0);
        return;
    }
    int race_step = data[0].asInt();
    // 5, 7, 9 ,11, 13, 15
    if ((race_step - Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_PREPARE) % 2)
        return;
    if (race_step < Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_PREPARE)
        return;
    if (race_step > Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_1_COMPLETE)
        return;

    // get race list
    std::vector<Json::Value> players_all, players_top16, players_top8, players_top4, players_top2, players_top1;
    // ����
    data = this->Mod_StSuperSport_Base_get_race_list(Mod_StSuperSport_Base::GROUP_TIAN_BANG);
    for (const auto& player : data[1]) {
        Json::Value temp1, temp2;
        for (int i = 0; i < 14; i++)
            temp1.append(player[i]);
        players_all.push_back(temp1);
        for (int i = 14; i < 28; i++)
            temp2.append(player[i]);
        players_all.push_back(temp2);
    }
    for (const auto& player : data[3]) {
        int player_id = player[0].asInt();
        players_top16.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[4]) {
        int player_id = player[0].asInt();
        players_top8.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[5]) {
        int player_id = player[0].asInt();
        players_top4.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[6]) {
        int player_id = player[0].asInt();
        players_top2.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    if (data[7].asInt()) {
        int player_id = data[7].asInt();
        players_top1.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }

    // ħ��
    data = this->Mod_StSuperSport_Base_get_race_list(Mod_StSuperSport_Base::GROUP_DI_BANG);
    for (const auto& player : data[1]) {
        Json::Value temp1, temp2;
        for (int i = 0; i < 14; i++)
            temp1.append(player[i]);
        players_all.push_back(temp1);
        for (int i = 14; i < 28; i++)
            temp2.append(player[i]);
        players_all.push_back(temp2);
    }
    for (const auto& player : data[3]) {
        int player_id = player[0].asInt();
        players_top16.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[4]) {
        int player_id = player[0].asInt();
        players_top8.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[5]) {
        int player_id = player[0].asInt();
        players_top4.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    for (const auto& player : data[6]) {
        int player_id = player[0].asInt();
        players_top2.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }
    if (data[7].asInt()) {
        int player_id = data[7].asInt();
        players_top1.push_back(*std::find_if(players_all.begin(), players_all.end(), [player_id](const Json::Value& x) {return x[0].asInt()==player_id;}));
    }

    /*for (const auto& player : players_all)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);
     common::log("��16ǿ��", iEdit);
     for (const auto& player : players_top16)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);
     common::log("��8ǿ��", iEdit);
     for (const auto& player : players_top8)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);
     common::log("��4ǿ��", iEdit);
     for (const auto& player : players_top4)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);
     common::log("��2ǿ��", iEdit);
     for (const auto& player : players_top2)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);
     common::log("��1ǿ��", iEdit);
     for (const auto& player : players_top1)
     common::log(boost::str(boost::format("����ħ������[%1%(%2%)]��[ս��%3%]") % common::utf2gbk(player[1].asString()) % player[0] % player[3]), iEdit);*/

    // select
    std::vector<Json::Value>::iterator player_select;
    switch (race_step) {
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_PREPARE:
        std::sort(players_all.begin(), players_all.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_all.begin();
        break;
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_16_COMPLETE:
        std::sort(players_top16.begin(), players_top16.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_top16.begin();
        break;
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_8_COMPLETE:
        std::sort(players_top8.begin(), players_top8.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_top8.begin();
        break;
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_4_COMPLETE:
        std::sort(players_top4.begin(), players_top4.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_top4.begin();
        break;
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_2_COMPLETE:
        std::sort(players_top2.begin(), players_top2.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_top2.begin();
        break;
    case Mod_StSuperSport_Base::ST_SUPER_SPORT_WAR_1_COMPLETE:
        std::sort(players_top1.begin(), players_top1.end(), [](const Json::Value& x, const Json::Value& y) {return x[3].asInt()>y[3].asInt();});
        player_select = players_top1.begin();
        break;
    default:
        // impossible
        break;
    }

    // bet
    int player_id_select = (*player_select)[0].asInt();
    std::string player_name_select = common::utf2gbk((*player_select)[1].asString());
    data = this->Mod_StSuperSport_Base_bet(player_id_select);
    if (data[0].asInt() != Mod_StSuperSport_Base::SUCCESS) {
        common::log(boost::str(boost::format("����ħ��������ע [%1%(%2%)] ʧ�ܣ�result[%3%]") % player_name_select % player_id_select % data[0]), iEdit);
        return;
    }
    common::log(boost::str(boost::format("����ħ��������ע [%1%(%2%)]") % player_name_select % player_id_select), iEdit);
    //} catch (const std::exception& ex) {
    //    common::log(boost::str(boost::format("���ִ���(war race)��%1%") % ex.what()));
    //}
}

//============================================================================
// R172 ��ħ��ս�����׶�
// {module:115, action:30, request:[],
// response:[Utils.UByteUtil, Utils.UByteUtil, Utils.IntUtil, Utils.IntUtil, [Utils.UByteUtil, Utils.IntUtil], Utils.IntUtil, Utils.IntUtil, Utils.UByteUtil, Utils.StringUtil, [Utils.StringUtil, Utils.StringUtil], Utils.IntUtil]};
// StSuperSportData.as 273:
//     oObject.list(param1, _loc_2, ["race_step", "player_step", "war_round", "player_number", "time_list", "cup_race_time_step", "apply_id", "group", "war_zone", "server_list", "bet_player_id"]);
// Example
//     ׼����[5,6,0,0,[[6,1511773200],[8,1511776800],[10,1511780400],[12,1511784000],[14,1511787600],[16,1511791200]],600,203957,1,"",[],0]
//     16ǿ��[6,6,0,64,[[6,1511773200],[8,1511776800],[10,1511780400],[12,1511784000],[14,1511787600],[16,1511791200]],600,203957,1,"",[],5417]
//     16ǿ�꣺[7,6,0,0,[[6,1511773200],[8,1511776800],[10,1511780400],[12,1511784000],[14,1511787600],[16,1511791200]],600,203957,1,"",[],5417]
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_get_race_step() {
    Json::Value data;
    return this->send_and_receive(data, 115, 30);
}

//============================================================================
// R172 ��ħ��սѡ��
// {module:115, action:32, request:[Utils.UByteUtil], response:[Utils.UByteUtil, [Utils.IntUtil, Utils.StringUtil, Utils.ShortUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.StringUtil, Utils.StringUtil, Utils.LongUtil, [Utils.ShortUtil, Utils.ShortUtil], Utils.StringUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.UByteUtil, Utils.IntUtil, Utils.StringUtil, Utils.ShortUtil, Utils.IntUtil, Utils.IntUtil, Utils.ShortUtil, Utils.StringUtil, Utils.StringUtil, Utils.LongUtil, [Utils.ShortUtil, Utils.ShortUtil], Utils.StringUtil, Utils.ShortUtil, Utils.ByteUtil, Utils.UByteUtil], [Utils.IntUtil, Utils.IntUtil, Utils.StringUtil], [Utils.IntUtil], [Utils.IntUtil], [Utils.IntUtil], [Utils.IntUtil], Utils.IntUtil, [Utils.IntUtil]]};
// StSuperSportData.as 273:
//     oObject.list(param1, _loc_2, ["race_step", "player_step", "war_round", "player_number", "time_list", "cup_race_time_step", "apply_id", "group", "war_zone", "server_list", "bet_player_id"]);
// Example
//     [1,[[24373,"��������.s1",104,7186843,0,250,"�Ķ�","s04",2000000,[],"",0,0,20,6395,"������.s8",100,3890726,0,217,"37wan","37wan_s0273",0,[],"",0,0,20],[4074,"�ʵ�.s1",104,3526116,0,250,"�Ķ�","s04",0,[],"",0,0,20,204132,"��ԯ��.s1",100,882214,0,149,"�Ķ�","s04",0,[],"",0,0,20],[197708,"��������.s1",5,7010203,0,250,"�Ķ�","s04",0,[],"",0,0,20,4554,"���޳�������.s1",104,782668,0,125,"�Ķ�","s04",0,[],"",0,0,20],[205960,"�����.s1",6,767188,0,126,"�Ķ�","s04",0,[],"",0,0,20,203964,"��ǧ��.s1",104,478252,0,152,"�Ķ�","s04",0,[],"",0,0,20],[5417,"����.s6",104,8865013,0,210,"37wan","37wan_s0273",131500000,[],"",0,0,20,205955,"������.s3",2,1301225,0,146,"�Ķ�","s02",0,[],"",0,0,20],[2917,"�貽����.s10",100,3044919,0,250,"Ҫ��","yaowan_s0152",0,[],"",0,0,20,14792,"��.s4",1,1070376,0,142,"�Ķ�","s04",0,[],"",0,0,20],[201283,"���˷���.s1",2,5300471,0,246,"37wan","37wan_s0273",0,[],"",0,0,20,6508,"������.s7",5,3223408,0,217,"37wan","37wan_s0273",0,[],"",0,0,20],[203725,"��ɫ���.s1",102,3108479,0,204,"�Ķ�","s04",0,[],"",0,0,20,2874,"ʯ��.s10",104,3125595,0,197,"Ҫ��","yaowan_s0152",0,[],"",0,0,20],[8536,"��߱���ߡ�.s1",102,5394715,0,250,"7k7k","yx567_s0125",0,[],"",0,0,20,26581,"�޿��κ�.s1",100,5061826,0,223,"�Ķ�","s04",0,[],"",0,0,20],[203966,"����.s1",104,397770,0,154,"�Ķ�","s04",0,[],"",0,0,20,6511,"������.s6",1,3319714,0,217,"37wan","37wan_s0273",0,[],"",0,0,20],[7804,"������һ.s5",6,5821585,0,205,"7k7k","yx567_s0125",0,[],"",0,0,20,17756,"������.s1",100,1188761,0,162,"7k7k","yx567_s0125",0,[],"",0,0,20],[203962,"�����.s1",110,516879,0,156,"�Ķ�","s04",0,[],"",0,0,20,11083,"ǿ�����ţ.s1",104,3392974,0,211,"�Ķ�","s04",0,[],"",0,0,20],[10243,"�λ�С�Ͼ�.s1",102,5175558,0,236,"7k7k","yx567_s0125",0,[],"",0,0,20,203957,"ׯ����.s1",5,753066,0,167,"�Ķ�","s04",0,[],"",0,0,20],[2112,"ţţ.s3",106,4209741,0,250,"6711","6711_s0160",0,[],"",0,0,20,14969,"lx1026.s1",5,907332,0,134,"�Ķ�","s04",0,[],"",0,0,20],[200547,"����ؼ�į.s1",104,7018813,0,250,"�Ķ�","s04",0,[],"",0,0,20,206601,"������.s1",3,1033570,0,146,"�Ķ�","s04",0,[],"",0,0,20],[2833,"����1.s4",100,4518472,0,232,"95k","95k_s081",0,[],"",0,0,20,203960,"������.s1",3,356303,0,151,"�Ķ�","s04",0,[],"",0,0,20]],[],[],[],[],[],0,[[2000000]]]
//     [2,[[684,"����а����.s4",100,5567983,0,237,"95k","95k_s081",0,[],"",0,0,20,206932,"Ԭ����.s1",1,582194,0,110,"37wan","37wan_s0273",0,[],"",0,0,20],[9167,"�ǻ����ؼ.s1",2,4571859,0,226,"�Ķ�","s04",0,[],"",0,0,20,3270,"������������.s4",1,1586823,0,149,"�Ķ�","s04",0,[],"",0,0,20],[8147,"��[��.s5",106,7380024,0,250,"7k7k","yx567_s0125",0,[],"",0,0,20,2364,"������.s10",104,5408628,0,220,"Ҫ��","yaowan_s0152",0,[],"",0,0,20],[207059,"������.s1",6,397035,0,89,"�Ķ�","s04",0,[],"",0,0,20,205754,"�����ʥ.s3",102,1169741,0,154,"�Ķ�","s02",0,[],"",0,0,20],[201992,"ҹ����.s1",100,5817810,0,220,"�Ķ�","s04",0,[],"",0,0,20,2934,"˾�����.s10",106,2390827,0,192,"Ҫ��","yaowan_s0152",0,[],"",0,0,20],[203965,"������.s1",107,391110,0,153,"�Ķ�","s04",0,[],"",0,0,20,2224,"����֭.s8",102,4312194,0,246,"�Ķ�","s05",0,[],"",0,0,20],[200966,"����ļ�į.s1",102,6079973,0,250,"�Ķ�","s04",0,[],"",0,0,20,206708,"������.s1",100,857972,0,130,"37wan","37wan_s0273",0,[],"",0,0,20],[1000,"����ï.s1",100,3854562,0,215,"�Ķ�","s04",0,[],"",0,0,20,206808,"������.s2",109,549625,0,106,"37wan","37wan_s0273",0,[],"",0,0,20],[1237,"���Ź�����.s1",100,7209889,0,217,"�Ķ�","s04",5000000,[],"",0,0,20,206177,"ŷ�����.s4",6,1393017,0,159,"37wan","37wan_s0273",0,[],"",0,0,20],[22112,"������.s4",108,1221336,0,161,"7k7k","yx567_s0125",0,[],"",0,0,20,7975,"ȭ��1��.s5",104,4116792,0,250,"7k7k","yx567_s0125",0,[],"",0,0,20],[2187,"ʥ�Ѻ��ɹ�.s3",206,7817505,0,231,"56","56_s024",0,[],"",0,0,20,12618,"������.s9",1,880587,0,142,"�Ķ�","s05",0,[],"",0,0,20],[204599,"����ï.s4",104,2477404,0,195,"37wan","37wan_s0273",0,[],"",0,0,20,2821,"�ƾ���.s4",102,3964554,0,222,"95k","95k_s081",0,[],"",0,0,20],[3212,"����޹.s1",106,6497897,0,249,"�Ķ�","s04",0,[],"",0,0,20,205700,"ŷ��ǧΰ.s1",5,1118576,0,153,"�Ķ�","s04",0,[],"",0,0,20],[10585,"����.s3",6,4199984,0,209,"�Ķ�","s02",0,[],"",0,0,19,190989,"������.s10",6,5313525,0,243,"95k","95k_s081",0,[],"",0,0,20],[8474,"�޵���ɰ��.s1",102,6885750,0,250,"7k7k","yx567_s0125",0,[],"",0,0,20,3426,"����.s9",6,5119910,0,250,"�Ķ�","s05",0,[],"",0,0,20],[9890,"��������.s2",5,1205008,0,158,"7k7k","yx567_s0125",0,[],"",0,0,20,203963,"��ɭ��.s1",104,475050,0,153,"�Ķ�","s04",0,[],"",0,0,20]],[],[],[],[],[],0,[[2000000]]]
//     [3,[],[[2,243822,"2017112401"]],[],[],[],[],0,[]]
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_get_race_list(int group) {
    Json::Value data;
    data.append(group);
    return this->send_and_receive(data, 115, 32);
}

//============================================================================
// R172 ��ħ��ս��ע
// {module:115, action:37,
// request:[Utils.IntUtil],
// Example
//     [5417] "����.s6"
// response:[Utils.UByteUtil, Utils.IntUtil]};
// StSuperSportData.as 593:
//     this.result = param1[0];
//     var _loc_2:* = param1[1];
//     this.info.isBet = _loc_2 > 0;
//     this.info.betPlayerId = _loc_2;
// Example
//     [28,5417]
//============================================================================
Json::Value sxd_client::Mod_StSuperSport_Base_bet(int id) {
    Json::Value data;
    data.append(id);
    return this->send_and_receive(data, 115, 37);
}

