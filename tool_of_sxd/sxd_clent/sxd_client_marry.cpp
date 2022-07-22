#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Marry_Base {
public:
    static const int SUCCESS = 13;
};

//============================================================================
// R179 ���
//============================================================================
void sxd_client::marry() {
    auto data = this->Mod_Marry_Base_get_marry_box();
    if(data[0].asInt()==Mod_Marry_Base::SUCCESS)
        common::log("����顿��ȡ���ޱ���", iEdit);
}

//============================================================================
// R179 ���ޱ���
// "module":206,"action":40,"request":[],"response":[Utils.UByteUtil]
// MarryData.as 265:
//     oObject.list(param1,this._engageIntimacyInfo,["result"]);
//============================================================================
Json::Value sxd_client::Mod_Marry_Base_get_marry_box() {
    Json::Value data;
    return this->send_and_receive(data, 206, 40);
}
