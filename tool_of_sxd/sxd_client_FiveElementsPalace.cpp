#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/format.hpp>

//五行天宫

class FiveElementsPalaceType
{
public:
	static const int SUCCESS = 0;
	static const int FAILED = 2;	
	static const int PASS = 3;			//已通关
	static const int NOT_PASS = 4;		//未通关
	static const int ISTRUE = 7;		
	static const int ISFALSE = 8;
	static const int USED = 9;
	static const int UNDEPLOYED = 10;	//未布阵
	static const int DEPLOYED = 11;		//已布阵
};

void sxd_client::FiveElementsPalace()
{
	//获取五行天宫信息
	Json::Value data = this->Mod_FiveElementsPalace_Base_five_elements_info();

	int result = data[0].asInt();
	if (result != FiveElementsPalaceType::SUCCESS)
	{
		std::cout << "【五行天宫】：获取五行天宫信息失败！！\n";
		return;
	}

	//当前层数
	int floor = data[1].asInt();

	//记录未通关副本
	vector<vector<int>> mission_list;

	//0不是boss，1是boss
	int boss = 0;
	for (Json::Value seq_pass_list : data[2])
	{
		if (seq_pass_list[0].asInt() > 0)
		{
			//大于0，不是boss
			if (seq_pass_list[1].asInt() == FiveElementsPalaceType::NOT_PASS)
			{
				vector<int> term;
				//关卡id，从一到五
				term.push_back(seq_pass_list[0].asInt());
				//副本id
				term.push_back(seq_pass_list[2].asInt());

				mission_list.push_back(term);
			}
		}
		else
		{
			//等于0，boss
			if (seq_pass_list[1].asInt() == FiveElementsPalaceType::NOT_PASS)
			{
				//未通关，记录副本id
				vector<int> term;
				term.push_back(seq_pass_list[0].asInt());
				term.push_back(seq_pass_list[2].asInt());

				mission_list.push_back(term);
				boss = 1;
			}
			else
			{
				//已通关
				if (floor == 100)
				{
					std::cout << "【五行天宫】：五行天宫已全部通关！！\n";
					return;
				}
				common::log(boost::str(boost::format("【五行天宫】：第 %1% 层boss已通关！等待开放") % floor));
				return;
			}
		}
	}

	//获取布阵信息
	data = this->Mod_FiveElementsPalace_Base_get_deploy();


	//记录未布阵伙伴id
	//三种状态，9已使用，11已布阵，10未布阵
	int already = 0;
	vector<int> role_list;
	for (Json::Value seq_role_list : data[0])
	{
		if (seq_role_list[12].asInt() == FiveElementsPalaceType::UNDEPLOYED)
		{
			//伙伴未布阵，记录id
			role_list.push_back(seq_role_list[0].asInt());
		}
		else if (seq_role_list[12].asInt() == FiveElementsPalaceType::DEPLOYED)
		{
			//是否为主角
			if (seq_role_list[1].asInt() != FiveElementsPalaceType::ISTRUE)
			{
				//不是主角，置状态为已布阵
				already = 1;
			}			
		}
	}

	if (boss == 1)
	{
		//boss关，直接打boss
		int pass = 1;

		while (pass != 0)
		{
			data = this->Mod_FiveElementsPalace_Base_fight_boss(floor);
			if (data[0].asInt() == FiveElementsPalaceType::SUCCESS)
			{
				common::log(boost::str(boost::format("【五行天宫】：第 %1% 次挑战 %2% 层boss,成功！！") % pass % floor));
				
				//重置挑战次数
				pass = 0;

				//循环挑战
				//FiveElementsPalace();
				return;
			}
			else
			{
				common::log(boost::str(boost::format("【五行天宫】：第 %1% 次挑战 %2% 层boss,失败！！") % pass % floor));
				
				if (pass >= 10)
				{
					std::cout << "\n已累计失败10次，停止挑战\n\n";
					pass = 0;
					return;
				}
				pass++;
			}

		}
	}

	for (vector<int> mission_id : mission_list)
	{
		if (already != 1)
		{
			//未布阵，先布阵

			//取出最后一个伙伴id
			int role_id = role_list.back();
			//删除最后一个伙伴id
			role_list.pop_back();

			//阵位id，51为替补
			//int grid_id = 51;
			int grid_id = 7;

			//std::cout << "【五行天宫】：role_id=" << role_id << " grid_id=" << grid_id << "\n";

			Json::Value data_deploy = this->Mod_FiveElementsPalace_Base_deploy(role_id,grid_id);
			
			if (data_deploy[0].asInt() == FiveElementsPalaceType::SUCCESS)
			{
				already = 1;
				//std::cout << "【五行天宫】：布阵成功，放置于替补\n";
				common::log("【五行天宫】：布阵成功，放置于替补\n");
			}
			else
			{
				std::cout << "【五行天宫】：布阵失败\n";
			}
		}

		//已布阵，直接打
		int pass = 1;
		while (pass != 0)
		{
			//std::cout << "【五行天宫】：已布阵，直接打\n";
			Json::Value data_fight = this->Mod_FiveElementsPalace_Base_fight(mission_id[0]);

			if (data_fight[0].asInt() == FiveElementsPalaceType::SUCCESS)
			{
				//挑战成功
				common::log(boost::str(boost::format("【五行天宫】：第 %1% 次挑战 %2% 层第 %3% 关，成功！！") % pass % floor % mission_id[0]));
				
				//重置挑战次数
				pass = 0;
				//挑战成功，重置布阵伙伴状态
				already = 0;
			}
			else
			{
				//挑战失败
				common::log(boost::str(boost::format("【五行天宫】：第 %1% 次挑战 %2% 层第 %3% 关，失败！！") % pass % floor % mission_id[0]));

				if (pass >= 10)
				{
					std::cout << "【五行天宫】：已累计失败10次，停止挑战\n\n";

					//重置挑战次数
					pass = 0;
				}
				else 
				{
					pass++;
				}				
			}
		}
	}

	
}
//============================================================================
//  五行天宫信息
// {module:453, action:0,
// request:[],
// Example		
//     [  ]
// 
// response:[Utils.UByteUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],Utils.ShortUtil,[Utils.UByteUtil,Utils.StringUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil]]
// Example
// FiveElemetsPalaceData.as 
// 
// oObject.list(param1,this.info,["result","now_seq","seq_pass_list","difficulty","record_list"]);
// for each(_loc4_ in this.info.seq_pass_list)
//  if(_loc4_[0] > 0)
/*  {
		(_loc5_ = FiveElementsPalaceType.getMonsterTeamObject(this.info.now_seq, _loc4_[0])).sign = FiveDeployType.GetPassSign(_loc4_[0]);
	}
	else
	{
		_loc5_ = FiveElementsPalaceType.getMonsterTeamObject(this.info.now_seq, 0);
		this.info.is_pass = _loc4_[1] == Mod_FiveElementsPalace_Base.PASS ? 1 : 0;
	}
*/
// oObject.list(_loc4_,_loc5_,["pass_id","is_pass","mission_id","special_pattern_id","item_id","award_number","use_role_id"]);
//     
// [ 0, 22, [ [ 5, 4, 9377, 58, 5822, 1, 0 ], [ 3, 4, 9375, 25, 5822, 1, 0 ], [ 4, 4, 9374, 17, 5822, 1, 0 ], [ 1, 4, 9378, 26, 5822, 1, 0 ], [ 2, 4, 9376, 31, 5822, 1, 0 ] ], 30, null ] 
// ["result","now_seq","seq_pass_list","difficulty","record_list"]
// 
// seq_pass_list=["pass_id","is_pass","mission_id","special_pattern_id","item_id","award_number","use_role_id"]
//			[ 5, 4, 9377, 58, 5822, 1, 0 ]
//============================================================================
Json::Value sxd_client::Mod_FiveElementsPalace_Base_five_elements_info()
{
	Json::Value data;
	return this->send_and_receive(data, 453, 0);
}

//============================================================================
//  五行天宫布阵信息
// {module:453, action:1,
// request:[],
// Example		
//     [  ]
// response:[[Utils.IntUtil,Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.UByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil],[Utils.IntUtil,Utils.ByteUtil]]
// Example
// FiveElemetsPalaceData.as 
// 
// for each(_loc2_ in param1[0])
/*
	{
		_loc4_ = {};
		oObject.list(_loc2_, _loc4_, ["player_role_id", "is_main_role", "health", "max_health", "role_id", "role_name", "role_level", "role_job_name", "role_stunt", "strength", "agile", "intellect", "is_deploy", "stId", "suit_equip_id", "role_rebirth_skill_id"]);
		_loc4_["is_main_role"] = _loc4_["is_main_role"] == Mod_FiveElementsPalace_Base.TRUE ? 1 : 0;
		_loc4_["role_sign"] = RoleType.getRoleSign(_loc4_["role_id"], _loc4_["suit_equip_id"]);
		_loc4_["role_type"] = RoleType.getRoleType(_loc4_["role_id"]);
		_loc4_["role_color"] = RoleType.getRoleColorById(_loc4_["role_id"]);
		_loc4_["is_special_partner"] = RoleType.checkSuperRole(_loc4_["role_id"]);
		if (_loc5_ = _loc4_["is_main_role"] == 1)
		{
			_loc4_["role_name"] = this._data.player.nickname;
		}
		_loc4_["url"] = PlayerInfo.getPortraitUrl_size75(_loc4_["role_sign"], _data.player.headPortraitId, _loc5_);
		_loc4_["stName"] = RoleStunt.getSTName(_loc4_["stId"]);
		if (_loc4_["role_rebirth_skill_id"] > 0)
		{
			_loc4_["rebirthSkillName"] = RoleStunt.getRebirthSkillName(_loc4_["role_rebirth_skill_id"]);
		}
		_loc4_["on_deploy"] = _loc4_["is_deploy"] == Mod_FiveElementsPalace_Base.DEPLOYED || _loc4_["is_deploy"] == Mod_FiveElementsPalace_Base.TIBU ? 1 : 0;
		_loc4_["is_used"] = _loc4_["is_deploy"] == Mod_FiveElementsPalace_Base.USED;
		this._aryRoleList.push(_loc4_);
	}
*/
//     [ [ [ 1967, 8, 4787358, 5170339, 137, "\u65b9\u5b50\u6674", 300, "\u672f\u58eb", "\u6653\u68a6\u62c2\u6674", 5322, 5247, 4816, 10, 0, 0, 0 ], [ 2334, 8, 6420920, 6923394, 623, "\u5723\u4e09\u6e05\u5251\u4ed9", 300, "\u5251\u7075", "\u5251\u5f71\u56db\u9636", 6575, 6392, 4990, 10, 0, 0, 0 ], [ 2331, 8, 13813265, 13815422, 552, "\u5723\u59dc\u5b50\u7259", 300, "\u5929\u5e08", "\u592a\u516c\u5f15\u9c7c", 11909, 12400, 11769, 10, 0, 0, 0 ], [ 2329, 8, 26233203, 22390923, 807, "\u9b54\u5b87\u6587\u62d3", 300, "\u5251\u7075", "\u9b54\u00b7\u4e07\u7075\u5251\u9635\u4e8c\u9636", 26470, 27358, 23304, 10, 0, 0, 139 ], [ 2350, 8, 5212882, 5212882, 615, "\u5723\u5e72\u5c06", 198, "\u5251\u7075", "\u4e07\u5251\u5f52\u5b97", 6019, 6268, 4649, 10, 0, 0, 0 ], [ 2085, 8, 17732256, 18521214, 633, "\u4f5b\u8d75\u7075\u513f", 300, "\u672f\u58eb", "\u4f5b\u00b7\u4e94\u6c14\u671d\u5143\u4e8c\u9636", 19286, 19829, 21194, 10, 0, 0, 111 ], [ 2346, 8, 47805710, 47809418, 884, "\u9b54\u62d3\u8dcb\u7389\u513f", 300, "\u5c06\u661f", "\u9b54\u00b7\u8ffd\u661f\u7834\u6708\u4e8c\u9636", 45513, 43434, 38458, 10, 0, 0, 164 ], [ 2344, 8, 7873602, 8501694, 860, "\u5723\u7b2c\u4e8c\u68a6", 300, "\u672f\u58eb", "\u5929\u5730\u65e0\u60c5", 6078, 6220, 5897, 10, 0, 0, 0 ], [ 1069, 7, 25068988, 29740986, 512, "\u98de\u7fbd\u5973\u5929\u5e08", 300, "\u5929\u5e08", "\u6444\u9b42\u593a\u9b44\u56db\u9636", 34410, 32646, 30723, 11, 0, 0, 0 ], [ 2063, 8, 32648534, 33791457, 603, "\u4f5b\u6797\u6708\u5982", 300, "\u6b66\u5723", "\u4f5b\u00b7\u753b\u5730\u4e3a\u7262\u4e8c\u9636", 32100, 31649, 28837, 10, 0, 0, 91 ] ], [ [ 0, 5 ], [ 0, 8 ], [ 0, 3 ], [ 0, 4 ], [ 0, 7 ], [ 1069, 1 ], [ 0, 9 ], [ 0, 51 ], [ 0, 2 ], [ 0, 6 ] ] ] 
//["player_role_id","is_main_role","health","max_health","role_id","role_name","role_level","role_job_name","role_stunt","strength","agile","intellect","is_deploy","stId","suit_equip_id","role_rebirth_skill_id"]);
// [0]=角色id，[12]=is_deploy
// [12]=10，没有布阵，[12]=11，已布阵，[12]=9,已使用
// 
// [ [ 0, 5 ], [ 0, 8 ], [ 0, 3 ], [ 0, 4 ], [ 0, 7 ], [ 1069, 1 ], [ 0, 9 ], [ 0, 51 ], [ 0, 2 ], [ 0, 6 ] ]
//	["player_role_id","grid_type_id"]
//  [角色id，阵位id]，阵位51是替补，1069是主角
//============================================================================
Json::Value sxd_client::Mod_FiveElementsPalace_Base_get_deploy()
{
	Json::Value data;
	return this->send_and_receive(data, 453, 1);
}

//============================================================================
// 布阵
// {module:453, action:2,
// request:[Utils.IntUtil,Utils.ByteUtil],
// Example		
//     [  ]
// [角色id，阵位]
// 
// response:[Utils.UByteUtil]
// Example
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_FiveElementsPalace_Base_deploy(int role_id, int grid_id)
{
	Json::Value data;
	data.append(role_id);
	data.append(grid_id);
	return this->send_and_receive(data, 453, 2);
}

//============================================================================
//  打怪
// {module:453, action:3,
// request:[Utils.ByteUtil],
// Example		
//     [  ]
//	[副本id]
// 
// response:[Utils.UByteUtil,[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]]
// Example
//     [  ]
//============================================================================
Json::Value sxd_client::Mod_FiveElementsPalace_Base_fight(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 453, 3);
}

//============================================================================
//  打boss
// {module:453, action:4,
// request:[Utils.IntUtil],
// Example		
//     [  ]
//		[mission_id]
// 
// response:[Utils.UByteUtil,[Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.LongUtil,Utils.LongUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil],Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ShortUtil],Utils.ByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],Utils.ShortUtil,[Utils.ShortUtil],[Utils.ShortUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.LongUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ByteUtil,[Utils.IntUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.IntUtil],[Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,[Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,[Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.ByteUtil],Utils.LongUtil,Utils.LongUtil,[Utils.IntUtil],Utils.ByteUtil,[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil]],Utils.ByteUtil,[Utils.IntUtil],Utils.IntUtil,[Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.ShortUtil,Utils.IntUtil],[Utils.ShortUtil,Utils.ByteUtil]],Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil],Utils.IntUtil,Utils.IntUtil],[[Utils.ByteUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],Utils.StringUtil],[Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,[Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],[Utils.ByteUtil,Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil]],[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil,[Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.ShortUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.StringUtil,Utils.IntUtil,Utils.UByteUtil,[Utils.IntUtil,Utils.ShortUtil,[Utils.UByteUtil,Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],[Utils.StringUtil],[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.ShortUtil],Utils.StringUtil,[Utils.IntUtil],[Utils.ShortUtil],[Utils.ShortUtil],Utils.IntUtil,Utils.IntUtil,Utils.LongUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil],[Utils.IntUtil,Utils.StringUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.StringUtil],Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.StringUtil,[Utils.IntUtil,Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil],[Utils.StringUtil]],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil],[Utils.IntUtil,Utils.LongUtil,Utils.IntUtil,Utils.LongUtil]],Utils.ShortUtil,Utils.IntUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil]]
// Example
//     [ ]
//============================================================================
Json::Value sxd_client::Mod_FiveElementsPalace_Base_fight_boss(int mission_id)
{
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 453, 4);
}