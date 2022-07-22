#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Assistant_Base {
public:
    static const int SUCCESS = 0;
};

//============================================================================
// R179 ��Ծ��
//============================================================================
void sxd_client::assistant() {
    auto data = this->Mod_Assistant_Base_info();
    auto info = data;
    for (unsigned sn = 1; sn < info.size(); sn++) {
        if (info[sn].asInt())
            continue;
        data = this->Mod_Assistant_Base_get_award(sn);
        if (data[0].asInt() == Mod_Assistant_Base::SUCCESS)
            common::log(boost::str(boost::format("����Ծ�ȡ���ȡ�� [%1%] ����Ծ�Ƚ���") % sn), iEdit);
        else
            break;
    }
}

//============================================================================
// R179 С����
// "module":52,"action":0,"request":[],"response":[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil]
// AssistantData.as 215:
//     this._aryBoxInfo = this.getBoxData(param1);
//     this._liveNum = param1[0] * 5;
//     this._boxInfo = this.renderBoxInfo(param1);
// Example
//     [ 172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ] // 172*5=860(��Ծ��)
// Mod_Assistant_Base.get_award(1)��
//     [ 172, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
//============================================================================
Json::Value sxd_client::Mod_Assistant_Base_info() {
    Json::Value data;
    return this->send_and_receive(data, 52, 0);
}

//============================================================================
// R179 ��ȡ
// "module":52,"action":1,"request":[Utils.IntUtil],"response":[Utils.UByteUtil]
// AssistantData.as 223
//     oObject.list(param1,this._objGetAward,["result"]);
// Example
//     [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_Assistant_Base_get_award(int sn) {
    Json::Value data;
    data.append(sn);
    return this->send_and_receive(data, 52, 1);
}
