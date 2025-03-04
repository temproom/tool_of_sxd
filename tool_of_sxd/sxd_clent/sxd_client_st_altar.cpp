#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_StAltar_Base
{
public:
	static const int SUCCESS = 0;
};

//============================================================================
// R179 仙界神坛
//============================================================================
void sxd_client::st_altar()
{
	auto data = this->Mod_StAltar_Base_get_altar_info();
	if (data[0].asInt() != Mod_StAltar_Base::SUCCESS)
		return;
	if (!data[7].asInt())
	{
		data = this->Mod_StAltar_Base_get_end_award();
		if (data[0].asInt() != Mod_StAltar_Base::SUCCESS)
			return;
		common::log("【仙界神坛】领取奖励", iEdit);
	}
}

//============================================================================
// R179 面板
// "module":263,"action":0,"request":[],"response":[Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ShortUtil,Utils.StringUtil,Utils.ShortUtil,Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil]
// StAltarData.as:
//     this._stAltarInfo["result"] = param1[0];
//     this._stAltarInfo["level"] = param1[1];
//     this._stAltarInfo["total_exp"] = param1[2];
//     this._stAltarInfo["rank"] = param1[3];
//     this._stAltarInfo["player_exp"] = param1[4];
//     for each(_loc2_ in param1[5])
//         _loc3_["rank"] = _loc2_[0];
//         _loc3_["nickname"] = _loc2_[1];
//         _loc3_["level"] = _loc2_[2];
//         _loc3_["value"] = _loc2_[3];
//     this._stAltarInfo.bEnd = param1[6] == 1;
//     this._stAltarInfo.bGet = param1[7] == 1;
// Example
// 领取前
//     [ 0, 20, 672177006, 197, 937884, [ [ 1, "\u2570\u2606\u256e\u8ca1\u795e\u723a.s12.yaowan_s0152", 259, 1490402 ], [ 2, "\u5609\u5609.s2.yaowan_s0152", 257, 1471294 ], [ 3, "\u9053\u65e0\u6daf.s4.s04", 259, 1442371 ], [ 4, "\u88ab\u5996\u602a\u6293\u8d70\u4e86.s8.s05", 259, 1441235 ], [ 5, "zc.s3.6711_s0160", 258, 1441037 ], [ 6, "\u4e01\u8377\u854a.s1.s04", 259, 1433006 ], [ 7, "\u5fa1\u98ce.s1.56_s01", 258, 1430273 ], [ 8, "\u6c5f\u4f5f\u7b71.s6.yx567_s0125", 259, 1377807 ], [ 9, "\u67ef\u6ce2\u5ada.s5.6711_s0160", 258, 1371100 ], [ 10, "\u7d2b\u84310226.s1.s04", 255, 1361466 ], [ 11, "\u9648\u4f1f.s1.s04", 259, 1357138 ], [ 12, "mhw.s17.95k_s081", 252, 1350160 ], [ 13, "\u521d\u543b\u611f\u89c9.s5.yx567_s0125", 259, 1343536 ], [ 14, "-\u706b\u9e92\u9e9f-.s4.6711_s0160", 257, 1342841 ], [ 15, "\u728e.s7.s04", 258, 1342230 ], [ 16, "\u5b59\u950b\u9ea6.s3.95k_s081", 255, 1342036 ], [ 17, "MSN\u4e09\u53c9\u621f.s3.56_s024", 258, 1337396 ], [ 18, "\u6714\u98ce.s5.37wan_s0273", 259, 1337191 ], [ 19, "\u6c5f\u575a\u6fef.s5.95k_s081", 255, 1329641 ], [ 20, "\u5149\u5708.s9.yaowan_s0152", 259, 1322605 ], [ 21, "\u597d\u5403\u662f\u79cd\u75c5.s1.yx567_s0125", 259, 1317715 ], [ 22, "\u5929\u6daf.\u6d6a\u5b50.s1.37wan_s0273", 259, 1316842 ], [ 23, "\u591c\u591c\u78e8\u83dc\u5200.s5.37wan_s0273", 254, 1309552 ], [ 24, "\u6b27\u9633\u5a77.s7.37wan_s0273", 259, 1308864 ], [ 25, "\u4e1c\u65b9\u5802\u8fea.s6.95k_s081", 255, 1306004 ], [ 26, "\u5f20\u6668\u59ec.s3.37wan_s0273", 258, 1300475 ], [ 27, "\u5929\u874e.s1.s04", 257, 1296138 ], [ 28, "\u9648\u5efa\u9f99.s5.yx567_s0125", 259, 1278655 ], [ 29, "\u5999\u67d3.s2.56_s01", 257, 1275549 ], [ 30, "\u5411\u524d\u884c\u300a\u300b.s5.6711_s0160", 257, 1275480 ], [ 31, "\u6728\u6728\u5200.s4.yaowan_s0152", 255, 1268475 ], [ 32, "\u53eb\u6211\u519b\u54e5.s8.s05", 258, 1261521 ], [ 33, "\u548c\u7f8e\u5c9a.s3.yx567_s0125", 259, 1259925 ], [ 34, "\u4e91\u6d77\u542c\u98ce.s1.s04", 258, 1259589 ], [ 35, "\u4eba\u7687.s3.37wan_s0273", 259, 1258168 ], [ 36, "\u6b87\u60c5.s5.s03", 257, 1256355 ], [ 37, "\u2605\u6613\u6c34\u5bd2\u2605.s9.s05", 256, 1254096 ], [ 38, "\u4e00\u5c81\u4e00\u67af\u8363.s1.yx567_s0125", 257, 1253626 ], [ 39, "\u4fa7\u9762\u662f\u98ce.s4.56_s024", 257, 1253121 ], [ 40, "\u6734 \u667a \u598d.s1.s04", 257, 1253067 ], [ 41, "\u5085\u96e8\u5bd2.s6.37wan_s0273", 249, 1252976 ], [ 42, "\u6768\u67f3\u5cb8.s7.37wan_s0273", 254, 1252739 ], [ 43, "\u9006\u6c34\u5bd2.s5.37wan_s0273", 248, 1249236 ], [ 44, "\u4e00\u4e86\u767e\u4e86.s3.yaowan_s0152", 255, 1249096 ], [ 45, "\u5929\u5723\u795e\u5e1d.s2.yx567_s0125", 256, 1247805 ], [ 46, "\u4e2d\u56fd\u529f\u592b.s3.s02", 258, 1240226 ], [ 47, "\u5a01\u9f99.s8.yx567_s0125", 259, 1236920 ], [ 48, "\u9f8d\u9b42\u7948\u5929.s6.95k_s081", 252, 1235774 ], [ 49, "\u6e21\u6c38\r\u6e21\u5c0f\u96ea.s6.37wan_s0273", 243, 1229718 ], [ 50, "\u4e50\u9738\u9738.s1.s04", 250, 1225182 ] ],
//       1, 0 ]
// 领取后
//     [ 0, 20, 672177506, 197, 937884, [ [ 1, "\u2570\u2606\u256e\u8ca1\u795e\u723a.s12.yaowan_s0152", 259, 1490402 ], [ 2, "\u5609\u5609.s2.yaowan_s0152", 257, 1471294 ], [ 3, "\u9053\u65e0\u6daf.s4.s04", 259, 1442371 ], [ 4, "\u88ab\u5996\u602a\u6293\u8d70\u4e86.s8.s05", 259, 1441235 ], [ 5, "zc.s3.6711_s0160", 258, 1441037 ], [ 6, "\u4e01\u8377\u854a.s1.s04", 259, 1433006 ], [ 7, "\u5fa1\u98ce.s1.56_s01", 258, 1430273 ], [ 8, "\u6c5f\u4f5f\u7b71.s6.yx567_s0125", 259, 1377807 ], [ 9, "\u67ef\u6ce2\u5ada.s5.6711_s0160", 258, 1371100 ], [ 10, "\u7d2b\u84310226.s1.s04", 255, 1361466 ], [ 11, "\u9648\u4f1f.s1.s04", 259, 1357138 ], [ 12, "mhw.s17.95k_s081", 252, 1350160 ], [ 13, "\u521d\u543b\u611f\u89c9.s5.yx567_s0125", 259, 1343536 ], [ 14, "-\u706b\u9e92\u9e9f-.s4.6711_s0160", 257, 1342841 ], [ 15, "\u728e.s7.s04", 258, 1342230 ], [ 16, "\u5b59\u950b\u9ea6.s3.95k_s081", 255, 1342036 ], [ 17, "MSN\u4e09\u53c9\u621f.s3.56_s024", 258, 1337396 ], [ 18, "\u6714\u98ce.s5.37wan_s0273", 259, 1337191 ], [ 19, "\u6c5f\u575a\u6fef.s5.95k_s081", 255, 1329641 ], [ 20, "\u5149\u5708.s9.yaowan_s0152", 259, 1322605 ], [ 21, "\u597d\u5403\u662f\u79cd\u75c5.s1.yx567_s0125", 259, 1317715 ], [ 22, "\u5929\u6daf.\u6d6a\u5b50.s1.37wan_s0273", 259, 1316842 ], [ 23, "\u591c\u591c\u78e8\u83dc\u5200.s5.37wan_s0273", 254, 1309552 ], [ 24, "\u6b27\u9633\u5a77.s7.37wan_s0273", 259, 1308864 ], [ 25, "\u4e1c\u65b9\u5802\u8fea.s6.95k_s081", 255, 1306004 ], [ 26, "\u5f20\u6668\u59ec.s3.37wan_s0273", 258, 1300475 ], [ 27, "\u5929\u874e.s1.s04", 257, 1296138 ], [ 28, "\u9648\u5efa\u9f99.s5.yx567_s0125", 259, 1278655 ], [ 29, "\u5999\u67d3.s2.56_s01", 257, 1275549 ], [ 30, "\u5411\u524d\u884c\u300a\u300b.s5.6711_s0160", 257, 1275480 ], [ 31, "\u6728\u6728\u5200.s4.yaowan_s0152", 255, 1268475 ], [ 32, "\u53eb\u6211\u519b\u54e5.s8.s05", 258, 1261521 ], [ 33, "\u548c\u7f8e\u5c9a.s3.yx567_s0125", 259, 1259925 ], [ 34, "\u4e91\u6d77\u542c\u98ce.s1.s04", 258, 1259589 ], [ 35, "\u4eba\u7687.s3.37wan_s0273", 259, 1258168 ], [ 36, "\u6b87\u60c5.s5.s03", 257, 1256355 ], [ 37, "\u2605\u6613\u6c34\u5bd2\u2605.s9.s05", 256, 1254096 ], [ 38, "\u4e00\u5c81\u4e00\u67af\u8363.s1.yx567_s0125", 257, 1253626 ], [ 39, "\u4fa7\u9762\u662f\u98ce.s4.56_s024", 257, 1253121 ], [ 40, "\u6734 \u667a \u598d.s1.s04", 257, 1253067 ], [ 41, "\u5085\u96e8\u5bd2.s6.37wan_s0273", 249, 1252976 ], [ 42, "\u6768\u67f3\u5cb8.s7.37wan_s0273", 254, 1252739 ], [ 43, "\u9006\u6c34\u5bd2.s5.37wan_s0273", 248, 1249236 ], [ 44, "\u4e00\u4e86\u767e\u4e86.s3.yaowan_s0152", 255, 1249096 ], [ 45, "\u5929\u5723\u795e\u5e1d.s2.yx567_s0125", 256, 1247805 ], [ 46, "\u4e2d\u56fd\u529f\u592b.s3.s02", 258, 1240226 ], [ 47, "\u5a01\u9f99.s8.yx567_s0125", 259, 1236920 ], [ 48, "\u9f8d\u9b42\u7948\u5929.s6.95k_s081", 252, 1235774 ], [ 49, "\u6e21\u6c38\r\u6e21\u5c0f\u96ea.s6.37wan_s0273", 243, 1229718 ], [ 50, "\u4e50\u9738\u9738.s1.s04", 250, 1225182 ] ],
//       1, 1 ]
//============================================================================
Json::Value sxd_client::Mod_StAltar_Base_get_altar_info()
{
	Json::Value data;
	return this->send_and_receive(data, 263, 0);
}

//============================================================================
// R179 领取
// "module":263,"action":8,"request":[],"response":[Utils.UByteUtil]
// StAltarData.as:
//     this.result = param1[0];
// Example
//     [ 0 ]
//============================================================================
Json::Value sxd_client::Mod_StAltar_Base_get_end_award()
{
	Json::Value data;
	return this->send_and_receive(data, 263, 8);
}
