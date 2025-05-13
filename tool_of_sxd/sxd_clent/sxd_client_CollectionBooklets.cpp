#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class CollectionBookletstype
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 1;
	static const int NOT_ENOUGH_APPROVE_TIMES = 11;
	static const int ID_ERROR = 14;
};

void sxd_client::CollectionBooklets()
{
	//获取状态
	//Json::Value data = this->Mod_CollectionBooklets_Base_sect_info();
	//data = this->Mod_CollectionBooklets_Base_my_friend();
	
	int approve_player_id;
	if (player_id == 212358)
	{
		approve_player_id = 224623;	//天才帅帅
	}
	else
	{
		approve_player_id = 212358;	//冯-诺依曼
	}
	std::string server_name = "360_s0113";
	for (;;)
	{
		Json::Value data = this->Mod_CollectionBooklets_Base_approve_player(approve_player_id, server_name);
		
		int result = data[0].asInt();
		if (result == CollectionBookletstype::NOT_ENOUGH_APPROVE_TIMES)
		{
			common::log("【万藏录】点赞次数已用完！！");
			return;
		}
		common::log("【万藏录】点赞成功！！");
	}
	
}
//============================================================================
//	万藏录：初始信息
// "module":815,"action":0,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
//		
// CollectionBookletsdata.as		
//			         oObject.list(param1,this.sectInfo,["result","player_info","sect_refine","sect_rank","yuanling_effect"]);
//					oObject.list(param1[_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","refine_val"]);
// Example
//			[ 0, [ [ 199, 22, 7598, "\u6015\u74e6\u843d\u5730.s547", 1416, "s0275", 255528 ], [ 1, 0, 7857, "\u660a\u5929\u5723\u5e1d.s11", 283169, "joy400_s0222", 251355 ], [ 507, 21, 8247, "\u7eb5\u6a2a", 115, "s1494", 240518 ], [ 197, 0, 7150, "\u98de\u9738", 22360, "xunlei_s6", 218377 ], [ 511, 5, 8123, "\u8840\u8272\u5996\u7bad.s34", 18772, "s018", 203716 ], [ 1, 21, 8247, "\u968f\u98ce\u6d41\u901d.s47", 14298, "s025", 200679 ], [ 511, 0, 7150, "123zzx.s1", 30233, "4399_s0116", 199337 ], [ 205, 0, 7845, "\u6de1\u7684\u6bd4\u76d0\u54b8.s50", 16724, "xunlei_s036", 182104 ], [ 511, 0, 8153, "\u6218\u795e\u725b\u8ba9\u8428.s98", 3015990, "51_s0225", 175513 ], [ 203, 16, 7981, "\u53f3\u624b\u5370", 5, "hly_s1288", 172506 ], [ 108, 0, 7597, "\u5b89\u5065\u5e73.s29", 1163283, "49you_s0734", 170861 ], [ 105, 8, 7846, "\u767d\u8272\u98ce\u8f66", 126, "s1898", 165253 ], [ 511, 7, 7564, "\u591c\u309e\u6708\u5f71.s56", 287500, "yx567_s0128", 161371 ], [ 510, 10, 7150, "\u6b27\u9633\u5a77.s7", 338253, "37wan_s0273", 156110 ], [ 110, 10, 7150, "\u6d2a\u65d7\u85e4.s37", 2025040, "51_s0225", 150199 ], [ 511, 15, 8695, "\u5b50\u9f99sy.s121", 139378, "360_s0120", 149285 ], [ 511, 13, 7150, "Spirit", 9, "hly_s1175", 149092 ], [ 197, 13, 8120, "\u8317\u70df\u6155\u96ea.s50", 15577, "xunlei_s036", 141494 ], [ 509, 15, 8120, "\u540d\u4e00\u6d41.s61", 113785, "37wan_s0280", 139189 ], [ 199, 16, 7150, "\u6c6a\u8426\u542c.s6", 701688, "funshion_s03", 138558 ], [ 206, 2, 8120, "BOSS.s35", 18331, "54op_s0213", 135230 ], [ 512, 2, 7857, "\u4e0a\u5b98\u7eaf\u84c9.s13", 96350, "kaixin_s04", 134493 ], [ 512, 16, 7150, "\u7eaa\u5a06\u59b1.s62", 164477, "37wan_s0280", 134184 ], [ 205, 1, 8256, "\u5f26\u65b7\u3001\u5187\u8ab0\u807d.s48", 6437, "xunlei_s036", 133459 ], [ 511, 13, 7564, "\u90d1\u6bc5.s29", 1496806, "51_s0225", 132469 ], [ 508, 16, 8256, "\u674e\u8d38\u7fbd.s243", 15668, "6711_s0267", 132454 ], [ 512, 0, 7150, "\u9053\u4ed9\u795e.s7", 45897, "weibo_s013", 129783 ], [ 512, 2, 7150, "\u559d\u82b1\u9152.s291", 4266, "s0147", 128973 ], [ 511, 9, 7150, "\u795e\u9a6c\u6d6e\u4e91.s110", 3536954, "51_s0225", 128808 ], [ 1, 3, 8120, "\u900d\u9065\u98d8\u6e3a\u4ed9.s226", 6797, "baidu_s0201", 128160 ], [ 107, 9, 8256, "\u5929\u5e1d.s18", 179077, "175ha_s0146", 127730 ], [ 512, 0, 0, "\u7535\u95ea\u96f7\u9e23.s47", 41217, "360_s0113", 126585 ], [ 509, 9, 7728, "\u795e\u5929\u4f7f\u7684\u7fc5\u8180.s40", 15212, "xunlei_s034", 126531 ], [ 509, 9, 8123, "\u767d\u6797\u6625.s14", 621490, "4399_s0116", 125767 ], [ 510, 12, 7150, "\u970d\u82af\u67d4.s2", 72440, "maxthon_s01", 121027 ], [ 196, 0, 7728, "\u98ce\u6e05\u82b8\u6de1", 26136, "53wan_s2", 120644 ], [ 511, 1, 7150, "\u674e\u5c0f\u5e7f@", 130, "kuwan8_s679", 120153 ], [ 507, 0, 8123, "\u5357\u5bab\u4f0a\u4eba.s3", 200775, "4399_s0116", 119448 ], [ 99, 1, 8123, "\u4fa7\u9762\u662f\u98ce.s4", 182958, "56_s024", 118821 ], [ 205, 1, 8123, "\u5b8b\u4ecb\u59cb.s28", 201029, "360_s0145", 117939 ], [ 4, 16, 7150, "\u964c\u7136`\u75f4\u604b.s7", 65121, "kuwan8_s0272", 116308 ], [ 512, 6, 8120, "\u7a7a\u4e2d\u9ca8\u9c7c.s28", 280827, "yx567_s0127", 114230 ], [ 207, 2, 7728, "\u548c\u67d4\u7487", 41, "s1660", 114033 ], [ 508, 2, 7150, "\u8d75\u5802\u950b", 15711, "kaixin_s40", 113531 ], [ 512, 16, 8256, "\u8fd8\u53eb\u3001123.s58", 124714, "bmh_s0122", 113145 ], [ 2, 2, 8120, "\u6c88\u854a\u7b71.s9", 215786, "yaowan_s0152", 111728 ], [ 2, 4, 7150, "DD.s10.s10", 38518, "2133_s038", 111104 ], [ 511, 0, 7150, "\u9f99\u4e4b\u6cea.s58", 334501, "yaowan_s0389", 111030 ], [ 510, 10, 8120, "\u5200\u5200.s3", 187940, "4399_s0116", 110606 ], [ 100, 0, 7342, "\u7ed9\u529b.s48", 20559, "s025", 109094 ] ], 7288512, 30, 30 ] 
//============================================================================
Json::Value sxd_client::Mod_CollectionBooklets_Base_sect_info()
{
	Json::Value data;
	return this->send_and_receive(data, 815, 0);
}

//============================================================================
//	万藏录：朋友列表
// "module":815,"action":5,
// "request":[]
// 
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.ByteUtil]]
//		
// CollectionBookletsdata.as		
//			         this.result = param1[0];
//					oObject.list(param1[1][_loc2_],_loc3_,["role_id","head_id","rim_id","nickname","player_id","server_name","popularity","is_open_visit"]);
// Example
//			[ 0, [ [ 99, 21, 8256, "\u6a0a\u6cc9\u68ee.s20", 449554, "xdwan_s0259", 10, 1 ], [ 201, 11, 8120, "\u51af\u2014\u8bfa\u4f9d\u66fc.s51", 212358, "360_s0113", 41, 1 ], [ 207, 0, 7857, "\u51af\u6d77\u68ee\u5821", 211, "360_s313", 4, 1 ] ] ]  
//============================================================================
Json::Value sxd_client::Mod_CollectionBooklets_Base_my_friend()
{
	Json::Value data;
	return this->send_and_receive(data, 815, 5);
}

//============================================================================
//	万藏录：点赞
// "module":815,"action":8,
// "request":[Utils.IntUtil,Utils.StringUtil],
// 
// CollectionBookletsview.as
//					_data.call(Mod_CollectionBooklets_Base.approve_player,this.approve_player_call_back,[this._playerId,this._serverName]);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]
//		
// CollectionBookletsdata.as		
//			         this.result = param1[0];
//					oObject.list(param1[2][_loc2_],_loc3_,["item_id","item_num"]);
// Example
//			
//		冯-诺依曼：212358，360_s0113
//		天才帅帅：224623，360_s0113
//		冯诺依曼：60，360_s0113
//		冯海森堡：211，360_s113
//============================================================================
Json::Value sxd_client::Mod_CollectionBooklets_Base_approve_player(int player_id, const std::string& server_name)
{
	Json::Value data;
	data.append(player_id);
	data.append(server_name);
	return this->send_and_receive(data, 815, 8);
}