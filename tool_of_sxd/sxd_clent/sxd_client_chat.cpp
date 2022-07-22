#include "stdafx.h"
#include "sxd_client.h"

//============================================================================
// R171 ����
// {module:6, action:0,
// request:[Utils.UByteUtil, Utils.StringUtil, Utils.StringUtil, Utils.StringUtil],
// ChatView.as 126:
//     _data.call(Mod_Chat_Base.chat_with_players, callBack, [data.messageType, data.message, data.eipNum, data.eipIndex]);
// Example
//     �������죺[ 1, "\u65b0\u5e74\u5feb\u4e50", "", "" ]
//     �ɽ����죺[ 4, "\u5176\u4e50\u878d\u878d", "", "" ]
// response:[Utils.IntUtil, Utils.UByteUtil]};
// Example
//     no response
//============================================================================
void sxd_client::Mod_Chat_Base_chat_with_players(int type, const std::string& message) {
	Json::Value data;
	data.append(type);
	data.append(message);
	data.append("");
	data.append("");
	this->send_frame(data, 6, 0);
}