#include "stdafx.h"
#include <vector>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

//谁与争锋
class NewShuiYuZhengFengtype
{
public:
	static const int SUCCESS = 0;	
	static const int YES = 4;
	static const int NO = 5;
	static const int CAN = 6;
	static const int CANNT = 7;
	static const int EASY = 8;
	static const int NORMAL = 9;
	static const int HARD = 10;
	static const int WIN = 19;
	static const int LOSE = 20;
	static const int ALREADY_CHALLENGE = 22;
	static const int NOT_ENOUGH_CHALLENGE_NUM = 25;
	static const int LACK_OF_TICKET = 38;
	static const int ALREADY_REFRESH = 39;
};

void sxd_client::NewShuiYuZhengFeng()
{
	//难易度
	std::unordered_map<int, string> type_list({ {4,"是"},{5,"否"}, {6,"可以"},{7,"不可以"}, {8,"简单"},{9,"普通"},{10,"困难"} });

	////获取基本信息
	//json::value data = this->mod_newshuiyuzhengfeng_base_get_player_shui_lu();

	////挑战券数量
	//int ticket_count = data[13].asint();
	////是否可刷新对手，can/cannt
	//int refresh_enemy_flag = data[17].asint();
	////对手列表
	//json::value enemylist = data[18];
	////剩余失败次数
	//int remain_failed_times = data[19].asint();
	//
	//common::log(boost::str(boost::format("【谁与争锋】剩余挑战券：【%1%】！！") % ticket_count));

	for (;;)
	{
		//获取基本信息
		Json::Value data = this->Mod_NewShuiYuZhengFeng_Base_get_player_shui_lu();

		//挑战券数量
		int ticket_count = data[14].asInt();
		//是否可刷新对手，CAN/CANNT
		int refresh_enemy_flag = data[18].asInt();
		//对手列表
		Json::Value EnemyList = data[19];
		//剩余失败次数
		int remain_failed_times = data[20].asInt();

		common::log(boost::str(boost::format("【谁与争锋】剩余挑战券：【%1%】！！") % ticket_count));
		common::log("【谁与争锋】\n\n\t 1.挑战\n\t 2.领取奖励\n\t请选择相应的功能：");
		int via;
		std::cin >> via;
		if (via == 1)
		{
			while (ticket_count)
			{				
				if (refresh_enemy_flag == NewShuiYuZhengFengtype::CAN)
				{
					//可刷新
					//刷新对手
					data = this->Mod_NewShuiYuZhengFeng_Base_refresh_enemy();
					int result = data[0].asInt();
					if (result == NewShuiYuZhengFengtype::SUCCESS)
					{
						//刷新成功
						//common::log("【谁与争锋】：刷新成功！！");
						ticket_count--;
						common::log(boost::str(boost::format("【谁与争锋】刷新成功，剩余挑战券：【%1%】！！") % ticket_count));
						//对手列表
						EnemyList = data[2];
						//剩余失败次数
						remain_failed_times = data[3].asInt();
						//是否可刷新
						refresh_enemy_flag = NewShuiYuZhengFengtype::CANNT;
					}
					else if (result == NewShuiYuZhengFengtype::ALREADY_REFRESH)
					{
						//已刷新
						common::log("【谁与争锋】：对手已刷新！！");
					}
					else if (result == NewShuiYuZhengFengtype::LACK_OF_TICKET)
					{
						//挑战券不足
						common::log("【谁与争锋】：挑战券不足！！");
						return;
					}
					else
					{
						common::log(boost::str(boost::format("【谁与争锋】刷新失败，代码【%1%】！！") % result));
						return;
					}
				}

				int num = 0;	//可挑战对手数量。
				for (Json::Value enemy : EnemyList)
				{
					int seq = enemy[0].asInt();		//序号
					int type = enemy[1].asInt();	//难易度，EASY/NORMAL/HARD
					int attack_power = enemy[10].asInt();		//战力
					int challenge_flag = enemy[16].asInt();		//能否挑战，CAN/CANNT

					if (challenge_flag == NewShuiYuZhengFengtype::CAN)
					{
						num++;
					}
					//输出对手信息，1：简单,80004000，已挑战
					common::log(boost::str(boost::format("【谁与争锋】对手信息：\t%1%:%2%，%3%，%4%") % seq % type_list[type] % attack_power % type_list[challenge_flag]));
				}

				common::log(boost::str(boost::format("【谁与争锋】剩余失败次数：【%1%】，请选择挑战的对手：") % remain_failed_times));

				while (remain_failed_times && num)
				{
					int fight_id;
					std::cin >> fight_id;

					Json::Value data_fight = this->Mod_NewShuiYuZhengFeng_Base_fight(fight_id);
					int result = data_fight[0].asInt();
					if (result == NewShuiYuZhengFengtype::WIN)
					{
						//挑战成功
						num--;
						if (num == 0)
						{
							refresh_enemy_flag = NewShuiYuZhengFengtype::CAN;
							common::log("【谁与争锋】所有对手都已挑战，请刷新对手！！");							
						}
						else
						{
							common::log("【谁与争锋】挑战成功，选择下一个对手：");
						}						
					}
					else if (result == NewShuiYuZhengFengtype::LOSE)
					{
						//挑战失败
						remain_failed_times--;
						if (!remain_failed_times)
						{
							//剩余失败次数归零
							common::log("【谁与争锋】已失败两次，请刷新对手！！");
							refresh_enemy_flag = NewShuiYuZhengFengtype::CAN;
						}
						else
						{
							common::log("【谁与争锋】挑战失败，重新选择对手：");
						}						
					}
					else if (result == NewShuiYuZhengFengtype::ALREADY_CHALLENGE)
					{
						common::log("【谁与争锋】该对手已挑战，重新选择对手：");
					}
					else
					{
						common::log(boost::str(boost::format("【谁与争锋】挑战错误，代码【%1%】！！") % result));
						return;
					}
				}
			}
		}
		else if (via == 2)
		{
			data = this->Mod_NewShuiYuZhengFeng_Base_get_score_award_info();
			int box_count = data[3].asInt();
			Json::Value score_list = data[0];

			if (box_count)
			{
				data = this->Mod_NewShuiYuZhengFeng_Base_get_box();
				if (data[0].asInt() == NewShuiYuZhengFengtype::SUCCESS)
				{
					common::log("【谁与争锋】领取宝箱成功！！");
				}
			}

			for (Json::Value score : score_list)
			{
				if (score[3].asInt() == NewShuiYuZhengFengtype::CAN)
				{
					data = this->Mod_NewShuiYuZhengFeng_Base_get_score_award(score[0].asInt());
					if (data[0].asInt() == NewShuiYuZhengFengtype::SUCCESS)
					{
						common::log("【谁与争锋】领取积分奖励成功！！");
					}
				}
			}
		}
		else
		{
			break;
		}
	}
}

//============================================================================
// 谁与争锋：获取玩家信息
// {module:317, action:1, 
// request:[],
// 
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.UByteUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.UByteUtil,[Utils.ByteUtil,Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.UByteUtil, [Utils.ShortUtil], Utils.UByteUtil], Utils.ByteUtil, Utils.ByteUtil]
// 
// NewShuiYuZhengFengData.as
//     oObject.list(param1,_loc2_,["0.flag","1.apply_id","2.nick_name","3.level","4.score","5.equip_item_id","6.stage_name","7.server_name","8.attack_power","9.rank","10.role_id","11.is_lei_zhu","l2.evel_bracket","13.cur_ticket_count","14.today_buy_count","15.day_buy_limit","16.buy_ingot_cost","17.refresh_enemy_flag","18.enemy_info_list","19.remain_failed_times","20.is_skip_fight_flag"]);
//				_loc2_.hasSignUp = _loc2_.flag == Mod_NewShuiYuZhengFeng_Base.SIGNED_UP
//		parseEnemyList	
// 				oObject.list(_loc3_,_loc4_,["0.seq","1.type","2.winner_score","3.apply_id","4.nick_name","5.level","6.score","7.equip_item_id","8.stage_name","9.server_name","10.attack_power","11.rank","12.role_id","13.is_lei_zhu","14.deploy_role_list","15.challenge_flag"]);
// 20260319	 12新增，后续后移一位
//	   oObject.list(param1,_loc2_,["0.flag","1.apply_id","2.nick_name","3.level","4.score","5.equip_item_id","6.stage_name","7.server_name","8.attack_power","9.rank","10.role_id","11.is_lei_zhu","12.era_level","13.level_bracket","14.cur_ticket_count","15.today_buy_count","16.day_buy_limit","17.buy_ingot_cost","18.refresh_enemy_flag","19.enemy_info_list","20.remain_failed_times","21.is_skip_fight_flag"]);
//              oObject.list(_loc3_,_loc4_,["0.seq","1.type","2.winner_score","3.apply_id","4.nick_name","5.level","6.score","7.equip_item_id","8.stage_name","9.server_name","10.attack_power","11.rank","12.role_id","13.is_lei_zhu","14.era_level","15.deploy_role_list",16."challenge_flag"]);
// 
// Example
//     [ 1, 216620, "\u5929\u624d\u5e05\u5e05.s51", 300, 1000, 8091, "360", "360_s0113", 87853841, 2040, 207, 5, 310, 6, 0, 5, 50, 6, null, 2, 0 ] 
//	   [ 1, 216620, "\u5929\u624d\u5e05\u5e05.s51", 300, 1000, 8091, "360", "360_s0113", 87853841, 2055, 207, 5, 310, 5, 0, 5, 50, 7, 
//     [ [ 1, 8, 30, 220228, "\u4f55\u4ed9\u59d1.s45", 300, 1000, 2406, "\u5feb\u73a9", "kuaiwan_s0108", 87028331, 2051, 198, 5, [ [ 839 ], [ 664 ], [ 931 ], [ 552 ], [ 622 ] ], 6 ], [ 2, 8, 30, 216438, "\u6708\u5f71.s5", 300, 1000, 8091, "\u98ce\u884c", "funshion_s03", 87265384, 2047, 508, 5, [ [ 796 ], [ 915 ], [ 923 ], [ 583 ], [ 458 ] ], 6 ], [ 3, 9, 50, 222611, "\u4e07\u4fdf\u799a\u96c9.s17", 300, 1097, 8091, "56uu", "56uu_s0210", 94144644, 992, 509, 5, [ [ 668 ], [ 931 ], [ 795 ], [ 459 ], [ 621 ] ], 6 ], [ 4, 9, 50, 220575, "199", 300, 1099, 7412, "\u5f00\u5fc3\u7f51", "kaixin_s44", 89231746, 988, 6, 5, [ [ 922 ], [ 796 ], [ 620 ], [ 522 ], [ 535 ] ], 6 ], [ 5, 10, 65, 222185, "\u96f7\u9706\u4e00\u5251.s444", 300, 1189, 8091, "\u5fc3\u52a8", "s0223", 99185300, 795, 511, 5, [ [ 797 ], [ 818 ], [ 922 ], [ 621 ], [ 927 ] ], 6 ] ], 2, 0 ] 
//
// 20260319    
//     [ 1, 231459, "\u51af\u6d77\u68ee\u5821", 300, 1000, 8091, "360", "360_s313", 124349347, 1552, 207, 5, 110, 310, 5, 0, 5, 50, 7, 
//	   [ [ 1, 8, 30, 230905, "\u541b\u541b", 300, 1000, 8091, "\u9177\u73a9\u5427", "kuwan8_s2130", 123599021, 1533, 101, 5, 90, [ [ 818 ], [ 927 ], [ 795 ], [ 923 ], [ 915 ] ], 6 ], [ 2, 8, 30, 230221, "\u5f00\u5fc3\u5c31\u597d.s48", 300, 1000, 8091, "\u8981\u73a9", "yaowan_s0398", 123379151, 1534, 511, 5, 90, [ [ 795 ], [ 666 ], [ 448 ], [ 857 ], [ 937 ] ], 6 ], 
//	   [ 3, 9, 50, 226957, "\u597d\u5403\u662f\u79cd\u75c5.s1", 300, 1097, 8091, "7k7k", "yx567_s0125", 121620057, 759, 512, 5, 90, [ [ 858 ], [ 943 ], [ 854 ], [ 884 ], [ 806 ] ], 6 ], [ 4, 9, 50, 227122, "\u5927\u8865\u4e38.s6", 300, 1083, 1308, "\u4e50\u90fd\u7f51", "ledu_s020", 126180348, 769, 512, 5, 90, [ [ 798 ], [ 535 ], [ 857 ], [ 522 ], [ 953 ] ], 6 ], 
//     [ 5, 10, 65, 230111, "\u6625\u68a6\u4e86\u65e0\u75d5.s45", 300, 1190, 8091, "\u591a\u73a9", "duowan_s0190", 276019142, 605, 99, 5, 100, [ [ 927 ], [ 947 ], [ 884 ], [ 795 ], [ 908 ] ], 6 ] ], 2, 0 ]
// ============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_get_player_shui_lu()
{
	Json::Value data;
	return this->send_and_receive(data, 317, 1);
}

//============================================================================
// 谁与争锋：获取比赛信息
// {module:317, action:4, 
// request:[],
// 
//	NewShuiYuZhengFengview.as
//			_data.call(Mod_AnniversaryTower_Base.get_award,get_award_callback,[data.floor_info.id,type]);
// 
// response:[Utils.UByteUtil,[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil],[Utils.ShortUtil],Utils.ShortUtil]
// 
// NewShuiYuZhengFengData.as
//     this._info.raceStep = param1[0];
//	   for each(_loc2_ in param1[1])
//				oObject.list(_loc2_,_loc5_,["race_step","start_time","end_time"]);
//		this._info.level_brackets = _loc3_;
//		this._info.cur_period = param1[3];
// 
// Example
//     [ 14, [ [ 13, 1750730400, 1750827600 ], [ 14, 1750827600, 1751518800 ], [ 15, 1751518800, 1751545800 ], [ 16, 1751545800, 1751547600 ] ], [ [ 100 ], [ 110 ], [ 120 ], [ 130 ], [ 140 ], [ 150 ], [ 160 ], [ 170 ], [ 180 ], [ 190 ], [ 200 ], [ 210 ], [ 220 ], [ 230 ], [ 240 ], [ 250 ], [ 260 ], [ 270 ], [ 280 ], [ 290 ], [ 300 ], [ 310 ] ], 27 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_get_race_info()
{
	Json::Value data;
	return this->send_and_receive(data, 317, 4);
}

//============================================================================
// 谁与争锋：挑战
// {module:317, action:5, 
// request:[Utils.ByteUtil],
// 
//	NewShuiYuZhengFengIntegralRaceView.as
//			this._data.call(Mod_NewShuiYuZhengFeng_Base.fight,this.onFightCallBack,[this._fightKey]);
// 
// response:[Utils.UByteUtil,[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil],[Utils.ShortUtil],Utils.ShortUtil]
// 
// NewShuiYuZhengFengData.as
//     this._info.scoreRaceInfo.fightResult = param1[0];
/*
		if(param1[0] == Mod_NewShuiYuZhengFeng_Base.WIN || param1[0] == Mod_NewShuiYuZhengFeng_Base.LOSE)
		 {
			this._info.scoreRaceInfo.war_result_list = param1[1];
			this._info.scoreRaceInfo.winner_score = param1[2];
			this._info.scoreRaceInfo.refresh_enemy_flag = param1[3];
			this._info.scoreRaceInfo.remain_failed_times = param1[4];
			if(param1[0] == Mod_NewShuiYuZhengFeng_Base.WIN)
			{
			   this._info.myPlayerInfo.score += this._info.scoreRaceInfo.winner_score;
			}
			if(this._info.scoreRaceInfo.refresh_enemy_flag == Mod_NewShuiYuZhengFeng_Base.CAN)
			{
			   this._info.scoreRaceInfo.enemy_info_list = [];
			}
		 }
*/
// 
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_fight(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 317, 5);
}

//============================================================================
// 谁与争锋：获取积分奖励信息
// {module:317, action:17, 
// request:[],
// 
//	NewShuiYuZhengFengview.as
//			
// 
// response:[[Utils.ByteUtil,Utils.ShortUtil,[Utils.IntUtil,Utils.IntUtil],Utils.UByteUtil],Utils.ShortUtil,Utils.ByteUtil,Utils.ByteUtil]
// 
// NewShuiYuZhengFengData.as
//      oObject.list(param1,this.scoreAwardInfo,["score_list","killed_player_count","need_killed","box_count"]);
//		for each(_loc3_ in this.scoreAwardInfo.score_list)
//				oObject.list(_loc3_,_loc4_,["award_id","need_score","award_list","get_flag"]);
//				for each(_loc6_ in _loc4_.award_list)
//						oObject.list(_loc6_,_loc7_,["item_id","count"]);
// 
// Example
//     [ [ [ 10, 2500, [ [ 1740, 50 ], [ 5169, 1 ] ], 7 ], [ 9, 3000, [ [ 5168, 1 ], [ 5169, 2 ] ], 7 ], [ 8, 3500, [ [ 1740, 100 ], [ 5169, 2 ] ], 7 ], [ 7, 4000, [ [ 1740, 150 ], [ 5169, 2 ] ], 7 ], [ 6, 4500, [ [ 5167, 1 ], [ 5169, 3 ] ], 7 ], [ 5, 5000, [ [ 1740, 200 ], [ 5169, 3 ] ], 7 ], [ 4, 5500, [ [ 1740, 300 ], [ 5169, 3 ] ], 7 ], [ 3, 6000, [ [ 5166, 1 ], [ 5169, 5 ] ], 7 ], [ 2, 7000, [ [ 1740, 400 ], [ 5169, 5 ] ], 7 ], [ 1, 8000, [ [ 5165, 1 ], [ 5169, 5 ] ], 7 ] ], 0, 10, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_get_score_award_info()
{
	Json::Value data;
	return this->send_and_receive(data, 317, 17);
}

//============================================================================
// 谁与争锋：刷新对手
// {module:317, action:25, 
// request:[],
// 
//	NewShuiYuZhengFengview.as
//			_data.call(Mod_AnniversaryTower_Base.get_award,get_award_callback,[data.floor_info.id,type]);
// 
// response:[Utils.UByteUtil,Utils.ByteUtil,[Utils.ByteUtil,Utils.UByteUtil,Utils.ByteUtil,Utils.IntUtil,Utils.StringUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.StringUtil,Utils.StringUtil,Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil,Utils.UByteUtil,[Utils.ShortUtil],Utils.UByteUtil],Utils.ByteUtil]
// 
// NewShuiYuZhengFengData.as
		/*this._info.scoreRaceInfo.refreshEnemyResult = param1[0];
         if(param1[0] == Mod_NewShuiYuZhengFeng_Base.SUCCESS)
         {
            this._info.scoreRaceInfo.cur_ticket_count = param1[1];
            this._info.scoreRaceInfo.enemy_info_list = this.parseEnemyList(param1[2]);
            this._info.scoreRaceInfo.remain_failed_times = param1[3];
            this._info.scoreRaceInfo.refresh_enemy_flag = Mod_NewShuiYuZhengFeng_Base.CANNT;
         }*/
//
// Example
//     [ 0, 5, [ [ 1, 8, 30, 220228, "\u4f55\u4ed9\u59d1.s45", 300, 1000, 2406, "\u5feb\u73a9", "kuaiwan_s0108", 87028331, 2051, 198, 5, [ [ 839 ], [ 664 ], [ 931 ], [ 552 ], [ 622 ] ], 6 ], [ 2, 8, 30, 216438, "\u6708\u5f71.s5", 300, 1000, 8091, "\u98ce\u884c", "funshion_s03", 87265384, 2047, 508, 5, [ [ 796 ], [ 915 ], [ 923 ], [ 583 ], [ 458 ] ], 6 ], [ 3, 9, 50, 222611, "\u4e07\u4fdf\u799a\u96c9.s17", 300, 1097, 8091, "56uu", "56uu_s0210", 94144644, 992, 509, 5, [ [ 668 ], [ 931 ], [ 795 ], [ 459 ], [ 621 ] ], 6 ], [ 4, 9, 50, 220575, "199", 300, 1099, 7412, "\u5f00\u5fc3\u7f51", "kaixin_s44", 89231746, 988, 6, 5, [ [ 922 ], [ 796 ], [ 620 ], [ 522 ], [ 535 ] ], 6 ], [ 5, 10, 65, 222185, "\u96f7\u9706\u4e00\u5251.s444", 300, 1189, 8091, "\u5fc3\u52a8", "s0223", 99185300, 795, 511, 5, [ [ 797 ], [ 818 ], [ 922 ], [ 621 ], [ 927 ] ], 6 ] ], 2 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_refresh_enemy()
{
	Json::Value data;
	return this->send_and_receive(data, 317, 25);
}

//============================================================================
// 谁与争锋：获取积分奖励
// {module:317, action:26, 
// request:[Utils.ByteUtil]
// 
//	NewShuiYuZhengFengScoreAwardView.as
//			_data.call(Mod_NewShuiYuZhengFeng_Base.get_score_award,this.getAwardBack,[param1]);
// 
// response:[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil]]
// 
// NewShuiYuZhengFengData.as
//     this.scoreAwardInfo.get_score_award_result = param1[0]
// 
// Example
//     [ 264, 4, 1, 1 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_get_score_award(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 317, 26);
}

//============================================================================
// 谁与争锋：获取宝箱奖励
// {module:317, action:27, 
// request:[],
// 
//	NewShuiYuZhengFengview.as
//			_data.call(Mod_AnniversaryTower_Base.get_award,get_award_callback,[data.floor_info.id,type]);
// 
// response:[Utils.UByteUtil,Utils.ByteUtil]
// 
// NewShuiYuZhengFengData.as
//     this.scoreAwardInfo.get_box_result = param1[0];
//		this.scoreAwardInfo.box_count = param1[1];
// 
// Example
//     [ 43, 0 ] 
//============================================================================
Json::Value sxd_client::Mod_NewShuiYuZhengFeng_Base_get_box()
{
	Json::Value data;
	return this->send_and_receive(data, 317, 27);
}