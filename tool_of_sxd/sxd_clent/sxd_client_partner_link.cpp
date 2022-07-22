#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_PartnerLink_Base {
public:
    static const int SUCCESS = 2;
    static const int GENERAL = 5;
    static const int ADVANCED = 6;
};

//============================================================================
// R179 ��Եϵͳ
//============================================================================
void sxd_client::partner_link() {
    for (;;) {
        auto data = this->Mod_PartnerLink_Base_get_shopping_info();
        // ��ͨԵ�걦��
        auto item = *std::find_if(data[0].begin(), data[0].end(), [](const Json::Value& x) {return x[0].asInt()==Mod_PartnerLink_Base::GENERAL;});
        int times = item[2].asInt();
        if (times == 0)
            break;
        data = this->Mod_PartnerLink_Base_buy_item(Mod_PartnerLink_Base::GENERAL);
        if (data[0].asInt() != Mod_PartnerLink_Base::SUCCESS) {
            common::log(boost::str(boost::format("����Եϵͳ��������ͨԵ�걦��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            break;
        }
        common::log("����Եϵͳ��������ͨԵ�걦��", iEdit);
    }
    for (;;) {
        auto data = this->Mod_PartnerLink_Base_get_shopping_info();
        // �߼�Ե�걦��
        auto item = *std::find_if(data[0].begin(), data[0].end(), [](const Json::Value& x) {return x[0].asInt()==Mod_PartnerLink_Base::ADVANCED;});
        auto times = item[2].asInt();
        if (times == 0)
            break;
        data = this->Mod_PartnerLink_Base_buy_item(Mod_PartnerLink_Base::ADVANCED);
        if (data[0].asInt() != Mod_PartnerLink_Base::SUCCESS) {
            common::log(boost::str(boost::format("����Եϵͳ�������߼�Ե�걦��ʧ�ܣ�result[%1%]") % data[0]), iEdit);
            break;
        }
        common::log("����Եϵͳ�������߼�Ե�걦��", iEdit);
    }
}

//============================================================================
// R179 Ե�걦�����
// "module":290,"action":2,"request":[],"response":[[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.UByteUtil,Utils.IntUtil]]
// PartnerLinkData.as:
//     for each(_loc4_ in param1[0])
//         oObject.list(_loc4_,_loc2_,["box_type","once_num","times","total_times","money_type","money_num"]);
// Example
//     [ [ [ 7, 12, 3, 3, 10, 100 ], [ 6, 6, 3, 3, 8, 200 ], [ 5, 2, 3, 3, 9, 500000 ] ] ]
//     [ [ [ 7, 12, 3, 3, 10, 100 ], [ 6, 6, 3, 3, 8, 200 ], [ 5, 2, 2, 3, 9, 500000 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_PartnerLink_Base_get_shopping_info() {
    Json::Value data;
    return this->send_and_receive(data, 290, 2);
}

//============================================================================
// R179 ��Եϵͳ
// "module":290,"action":3,"request":[Utils.UByteUtil],"response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// PartnerLinkData.as:
//     oObject.list(param1,this._buyItem,["result","yuan_hun_list"]);
//     for each(_loc4_ in this._buyItem.yuan_hun_list)
//         oObject.list(_loc4_,_loc2_,["id","num"]);
// Example
//     [ 5 ] --> [ 2, [ [ 18, 2 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_PartnerLink_Base_buy_item(int type) {
    Json::Value data;
    data.append(type);
    return this->send_and_receive(data, 290, 3);
}
