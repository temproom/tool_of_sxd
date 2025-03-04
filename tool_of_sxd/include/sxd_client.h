#pragma once
#ifndef SXD_CLIENT_H_
#define SXD_CLIENT_H_

#include <string>
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <iostreams/filtering_stream.hpp>
#include <iostreams/filter/zlib.hpp>

#include "json/json.h"
//#include "jsoncpp.cpp"
#include <thread>
#include "vector"

#include "database.h"
#include "sql_connection_pool/sql_connection_pool.h"
//#include "threadpool/threadpool.h"

class sxd_client {
private:
	boost::asio::io_service ios;
	boost::asio::ip::tcp::resolver resolver;
	boost::asio::ip::tcp::socket sock;

	std::string version;        // 版本，构造函数初始化
	std::string user_id;        // 对应数据库表config的id，构造函数初始化
	int player_id;              // 在login中赋值
	short pre_module;
	short pre_action;

	int m_state;				//0为send，1为receive
	Json::Value m_data_s;
	short m_module_s;
	short m_action_s;
	Json::Value m_data_r;
	short m_module_r;
	short m_action_r;

	int iEdit;
	bool bLogin;
	int x, y;

	//============================================================================
	// - sxd_client.cpp
	//============================================================================
public:

	sxd_client(const std::string& version, const int hwnd = -1);
	virtual ~sxd_client();
	void connect(const std::string& host, const std::string& port);
	bool is_login() {
		return bLogin;
	}

private:
	void send_frame(const Json::Value& data, short module, short action);
	void receive_frame(Json::Value& data, short& module, short& action);
	Json::Value send_and_receive(const Json::Value& data_s, short module_s, short action_s, std::function<bool(const Json::Value&)> f = NULL);
	Json::Value sends_and_receive(std::vector<Json::Value>& data_s, short module_s, short action_s, std::function<bool(const Json::Value&)> f = NULL);
	Json::Value send_and_receive(const Json::Value& data_s, short module_s, short action_s, short module_r0, short action_r0, std::function<bool(const Json::Value&)> f = NULL);

public:
	//============================================================================
	// - sxd_client_town.cpp			登录
	//============================================================================
	int login_town(const std::string& web_page);
	Json::Value Mod_Player_Base_login(const std::string& player_name, const std::string& hash_code, const std::string& time, const std::string& source, int regdate, const std::string& id_card, int open_time, char is_newst, const std::string& stage, const std::string& client);
	Json::Value Mod_Player_Base_get_player_info();
	Json::Value Mod_Player_Base_player_info_contrast(int player_id);
	Json::Value Mod_Player_Base_get_player_info_by_username(std::string username);
	Json::Value Mod_Player_Base_get_player_function();
	Json::Value Mod_Player_Base_get_game_assistant_info();
	Json::Value Mod_Player_Base_server_time();
	Json::Value Mod_Role_Base_get_role_list(int player_id);
	Json::Value Mod_Town_Base_enter_town(int town_map_id);
	void town_move_to(int x, int y);
	Json::Value Mod_Town_Base_move_to(int x1, int y1, int x2, int y2);
	Json::Value Mod_Player_Base_get_player_war_cd_time(int type);

	//============================================================================
	// - sxd_client_bug.cpp			副本扫荡，未完成，有bug
	//============================================================================
	void bug();
	void mission_practice();
	Json::Value Mod_MissionPractice_Base_start_practice(int mission_id);
	Json::Value Mod_MissionPractice_Base_quickly();

	//============================================================================
	// - sxd_client_chat.cpp		聊天
	//============================================================================
	void Mod_Chat_Base_chat_with_players(int type, const std::string& message);

	//============================================================================
	// - sxd_client_gift.cpp			礼包
	//============================================================================
	void gift3();
	Json::Value Mod_SealSoul_Base_get_day_stone();
	Json::Value Mod_Player_Base_get_player_camp_salary();
	Json::Value Mod_Farm_Base_player_is_player_get_xian_ling_gift();
	Json::Value Mod_Farm_Base_player_get_xian_ling_gift();
	void function_end();
	Json::Value Mod_FunctionEnd_Base_game_function_end_gift();
	Json::Value Mod_FunctionEnd_Base_random_award(int id);
	Json::Value Mod_FunctionEnd_Base_get_game_function_end_gift(int id);
	void gift();
	Json::Value Mod_Item_Base_get_player_gift_all_info();
	Json::Value Mod_Item_Base_player_get_super_gift(int id);
	void Mod_HeroesWar_Base_get_end_gift();
	void Mod_StChallenge_Base_get_end_li_bao();
	void Mod_UnlimitChallenge_Base_get_end_award();

	//============================================================================
	// - sxd_client_lucky_shop.cpp			神秘商人，秘宝商铺-奇珍异宝
	//============================================================================
	void lucky_shop();
	Json::Value Mod_LuckyStore_Base_get_lucky_store_item_list();
	Json::Value Mod_LuckyStore_Base_buy_lucky_store_item(int npc_id, int item_id, int lucky_store_id);
	void black_shop();
	Json::Value Mod_LuckyStore_Base_black_shop_item_list();
	Json::Value Mod_LuckyStore_Base_buy_black_shop_item(int black_shop_id);
	Json::Value Mod_LuckyStore_Base_scrap_item_list();
	Json::Value Mod_LuckyStore_Base_link_soul_item_list();

	//============================================================================
	// - sxd_client_item.cpp			背包，仓库
	//============================================================================
	int get_empty_pack_num();
	void item_reel();
	void item_use();
	void item_sell();
	Json::Value Mod_Item_Base_get_player_pack_item_list();
	Json::Value Mod_Item_Base_get_player_warehouse_grids();
	Json::Value Mod_Item_Base_get_player_home_warehouse_grids();
	Json::Value Mod_Item_Base_get_item_basical_infos(int ids[], int count);
	Json::Value Mod_Item_Base_get_facture_reel_data(int item_id);
	Json::Value Mod_Item_Base_player_use_grid_reel(int player_item_id);
	Json::Value Mod_Item_Base_large_use_grid_item(int player_item_id);
	Json::Value Mod_Item_Base_player_use_grid_item(int box_id);
	Json::Value Mod_Item_Base_player_sell_item(int box_id);
	Json::Value Mod_Item_Base_get_all_player_item_infos();
	Json::Value Mod_GiftItemPack_Base_put_item_to_package(int box_id);
	Json::Value Mod_Item_Base_move_pack_grid_item_to_supergift_warehouse(int box_id);

	//============================================================================
	// - sxd_client_release_welfare.cpp			更新福利
	//============================================================================
	void release_welfare();
	Json::Value Mod_ReleaseWelfare_Base_get_info();
	Json::Value Mod_ReleaseWelfare_Base_get_welfare(int id);

	//============================================================================
	// - sxd_client_equipment.cpp		装备强化
	//============================================================================
	void equipment();
	void upgrade();
	Json::Value Mod_Equipment_Base_upgrade_equip(int player_item_id, int select_gem_id, int use_coin_upgrade);
	Json::Value Mod_Equipment_Base_batch_upgrade_equip(int player_item_id, int select_gem_id);
	Json::Value Mod_Item_Base_player_equip_use_reel(int PlayerItemId, int ReelId, int ProcessType, int KeepLv, int slsFrame, int allFrame);
	Json::Value Mod_Item_Base_get_role_equip_list(int Player_Role_Id);
	Json::Value Mod_Item_Base_player_buy_equip_item(int Item_Id, int Player_Role_Id, int Position_Id);

	void equip_use_reel();
	Json::Value Mod_Item_Base_player_equip_use_reel(int eqPlayerItemId, int reelItemId);
	Json::Value Mod_Item_Base_equip_player_role_item(int WearGridId, int PlayerRoleId);
	Json::Value Mod_Item_Base_remove_player_role_equipment(int eqPlayerItemId, int reelItemId,int house);

	//============================================================================
	// - sxd_client_library.cpp			藏经阁
	//============================================================================
	void library();
	Json::Value Mod_FindImmortal_Base_exchange_xian_ling(int num);
	Json::Value Mod_Library_Base_get_library_info();
	Json::Value Mod_Library_Base_activate_library_upgrade();
	Json::Value Mod_Library_Base_up_library_level(int job_id);

	//============================================================================
	// - sxd_client_research.cpp		奇术
	//============================================================================
	void research();
	Json::Value Mod_Research_Base_research_list();
	Json::Value Mod_Research_Base_research_upgrade(int id);

	//============================================================================
	// - sxd_client_rune.cpp			招财神符
	//============================================================================
	void rune();
	Json::Value Mod_Rune_Base_rune_list();
	Json::Value Mod_Rune_Base_rune_use();

	//============================================================================
	// - sxd_client_get_peach.cpp		摘仙桃
	//============================================================================
	void get_peach();
	Json::Value Mod_GetPeach_Base_peach_info();
	Json::Value Mod_GetPeach_Base_batch_get_peach();
	Json::Value Mod_GetPeach_Base_get_peach();

	//============================================================================
	// - sxd_client_farm.cpp		药园
	//============================================================================
	void harvest();
	void plant();
	Json::Value Mod_Farm_Base_get_farmlandinfo_list();
	Json::Value Mod_Farm_Base_harvest(int land_id, bool is_double = false);
	Json::Value Mod_Farm_Base_buy_coin_tree_count_info();
	Json::Value Mod_Farm_Base_get_player_roleinfo_list();
	Json::Value Mod_Farm_Base_plant_herbs(int land_id, int play_role_id, int type, int herbs_star_level);

	//============================================================================
	// - sxd_client_three_anniversary_signIn.cpp		新年签到
	//============================================================================
	void sign_in();
	Json::Value Mod_ThreeAnniversarySignIn_Base_get_sign_in_status();
	Json::Value Mod_ThreeAnniversarySignIn_Base_get_player_sign_in_info();
	Json::Value Mod_ThreeAnniversarySignIn_Base_player_sign_in();

	//============================================================================
	// - sxd_client_wager_shop.cpp		游乐城赌坊-大转盘
	//============================================================================
	void wager_shop();
	Json::Value Mod_WagerShop_Base_get_turntable_info();
	Json::Value Mod_WagerShop_Base_turntable();

	//============================================================================
	// - sxd_client_hide_treasure_map.cpp		藏宝图
	//============================================================================
	void hide_treasure_map();
	void hide_treasure_map_search();
	Json::Value Mod_HideTreasureMap_Base_buy_item_info();
	Json::Value Mod_HideTreasureMap_Base_buy_item();
	Json::Value Mod_HideTreasureMap_Base_player_use_grid_item(int box_id);
	Json::Value Mod_HideTreasureMap_Base_start_grub(int box_id, int town_map_id, int x, int y);

	//============================================================================
	// - sxd_client_dice_messenger.cpp			骰子使者
	//============================================================================
	void dice_messenger();
	Json::Value Mod_DiceMessenger_Base_get_info();
	Json::Value Mod_DiceMessenger_Base_play_dice();
	Json::Value Mod_DiceMessenger_Base_get_award();
	Json::Value Mod_DiceMessenger_Base_tol_get();


	//============================================================================
	// - sxd_client_dunhuang_treasure.cpp		敦煌秘宝
	//============================================================================
	void dunhuang_treasure();
	Json::Value Mod_DunhuangTreasure_Base_get_dunhuang_treasure_info();
	Json::Value Mod_DunhuangTreasure_Base_open_treasure(int type);

	//============================================================================
	// - sxd_client_memoirs.cpp			活动列表
	//============================================================================
	Json::Value Mod_Memoirs_Base_is_activity_open();

	//============================================================================
	// - sxd_client_spring_carnival.cpp		// 新春嘉年华
	// 1. 新春大放送; 2. 开年献礼; 3. 福神转盘; 4. 幸运骰子; 5. 翻翻乐; 6. 在线奖励
	//============================================================================
	void spring_carnival();
	void spring_big_run();
	Json::Value Mod_SpringBigRun_Base_panel_info();
	Json::Value Mod_SpringBigRun_Base_gain_award();
	Json::Value Mod_SpringBigRun_Base_gain_total_award(int id);
	void spring_open_year();
	Json::Value Mod_NewYearSmallActivity_Base_get_open_year_award(int id);
	void assistant_turntable_two();
	Json::Value Mod_Memoirs_Base_assistant_turntable_lottery_two();
	void charge_back();
	Json::Value Mod_ChargeBack_Base_activate_fuction_of_mascot_befall(int function_id);
	void regression_dice();
	Json::Value Mod_Regression_Base_dice();
	void fan_fan_le();
	Json::Value Mod_FanFanLe_Base_get_info();
	Json::Value Mod_FanFanLe_Base_flop(int id);
	Json::Value Mod_FanFanLe_Base_get_extra_award();
	void online_award();
	Json::Value Mod_SpringActivity_Base_get_online_award_info();
	Json::Value Mod_SpringActivity_Base_get_online_award(int id);
	void lucky_poker();
	Json::Value Mod_LuckyPoker_Base_get_info();
	Json::Value Mod_LuckyPoker_Base_lottery_draw();
	Json::Value Mod_LuckyPoker_Base_exchange_award();
	Json::Value Mod_Memoirs_Base_get_exchange_item_info(int reflection_id);
	Json::Value Mod_Memoirs_Base_exchange_item_award(int reflection_id, int award_id);
	void golden_worker();
	Json::Value Mod_ActivityGame_Base_start_play_kuang_gong();
	Json::Value Mod_ActivityGame_Base_kuang_gong_game_over(int golden);
	Json::Value Mod_ActivityGame_Base_player_score_exchange_info();
	Json::Value Mod_ActivityGame_Base_exchange_award(int award_id);
	void mid_back_lottery();
	Json::Value Mod_MidBackLottery_Base_get_back_lottery_info();
	Json::Value Mod_MidBackLottery_Base_back_lottery(int id);
	Json::Value Mod_MidBackLottery_Base_memento_coin_exchange(int item_id);
	Json::Value Mod_MidBackLottery_Base_get_memento_coin_exchange_info();
	void thanks_feedback();
	Json::Value Mod_ThanksFeedback_Base_open_panel();
	Json::Value Mod_ThanksFeedback_Base_get_award(int id);

	//============================================================================
	// - sxd_client_regression.cpp		回归活动
	//============================================================================
	void regression();
	void back_assistant();
	Json::Value Mod_Regression_Base_get_back_assistant_award(int id);
	void go_back_shop();
	Json::Value Mod_GoBackShop_Base_open_panel();
	Json::Value Mod_GoBackShop_Base_buy_shop_item(int type, int shop_item_id);
	void go_back_login();
	Json::Value Mod_GoBackLogin_Base_get_award(int id);
	void back_send_gift();
	Json::Value Mod_Regression_Base_get_back_send_gift_award();
	void go_back_task();
	Json::Value Mod_GoBackTask_Base_get_award(int id);

	//============================================================================
	// - sxd_client_find_treasure.cpp		龙宫探宝
	//============================================================================
	void find_treasure();
	Json::Value Mod_FindTreasure_Base_open_find_treasure();
	Json::Value Mod_FindTreasure_Base_find_treasure(int is_lock);
	Json::Value Mod_FindTreasure_Base_exchange_score_award(int id);

	//============================================================================
	// - sxd_client_link_fate.cpp			结缘
	//============================================================================
	void link_fate();
	Json::Value Mod_LinkFate_Base_get_link_fate_box();
	Json::Value Mod_LinkFate_Base_get_player_link_fate_stone_pack();
	Json::Value Mod_LinkFate_Base_one_key_open_box(int id);
	Json::Value Mod_LinkFate_Base_auto_merge_link_fate_stone();

	//============================================================================
	// - sxd_client_training.cpp			培养
	//============================================================================
	void training();
	Json::Value Mod_Training_Base_panel_show(int player_role_id);
	Json::Value Mod_Training_Base_training(int player_role_id, int type);
	Json::Value Mod_Training_Base_modify_role_data(int player_role_id);

	//============================================================================
	// - sxd_client_chaos_equipment.cpp			混沌虚空
	//============================================================================
	void space_find();
	Json::Value Mod_SpaceFind_Base_open_space_find();
	Json::Value Mod_SpaceFind_Base_do_space_find(int type);
	Json::Value Mod_SpaceFind_Base_get_space_find();
	void chaos_equipment();
	Json::Value Mod_ChaosEquipment_Base_get_pack_chaos_monster_list();
	Json::Value Mod_ChaosEquipment_Base_make_chaos_monster(int id);
	Json::Value Mod_ChaosEquipment_Base_resolve_player_chaos_monster(int id);

	//============================================================================
	// - sxd_client_email.cpp		邮件
	//============================================================================
	void email();
	Json::Value Mod_Email_Base_get_email_info();
	Json::Value Mod_Email_Base_get_attachment_award(int id);
	Json::Value Mod_Email_Base_delete_email(int type, int id);

	//============================================================================
	// - sxd_client_super_sport.cpp			竞技场
	//============================================================================
	void super_sport();
	Json::Value Mod_SuperSport_Base_open_super_sport();
	Json::Value Mod_SuperSport_Base_start_challenge(int id);

	//============================================================================
	// - sxd_client_courtyard_pet.cpp		宠物房
	//============================================================================
	void courtyard_pet();
	Json::Value Mod_CourtyardPet_Base_get_player_info();
	Json::Value Mod_CourtyardPet_Base_call(int type);

	void courtyard_pet_quest();
	Json::Value Mod_CourtyardPet_Base_get_courtyard_pet_list();
	Json::Value Mod_CourtyardPet_Base_get_quest_list();
	Json::Value Mod_CourtyardPet_Base_get_courtyard_normal_pet_list();
	Json::Value Mod_CourtyardPet_Base_reload();
	Json::Value Mod_CourtyardPet_Base_accept_quest(int quest_id, int pet_list[], int count);
	Json::Value Mod_CourtyardPet_Base_get_award(int quest_id);

	//============================================================================
	// - sxd_client_pot_world.cpp		壶中界
	//============================================================================
	void pot_world();
	Json::Value Mod_PotWorld_Base_get_info();
	Json::Value Mod_PotWorld_Base_get_store_info(int building_id);
	Json::Value Mod_PotWorld_Base_buy_item(int item_id);
	Json::Value Mod_PotWorld_Base_get_forever_zhu_fu_list();
	Json::Value Mod_PotWorld_Base_get_fu_info();
	Json::Value Mod_PotWorld_Base_merge_item(int item_id);
	Json::Value Mod_PotWorld_Base_get_pack_info();

	//============================================================================
	// - sxd_client_beelzebub_trials.cpp		魔王试炼
	//============================================================================
	void beelzebub_trials();
	Json::Value Mod_BeelzebubTrials_Base_get_moral_award();

	//============================================================================
	// - sxd_client_pet_animal.cpp		叶公好龙
	//============================================================================
	void pet_animal();
	Json::Value Mod_PetAnimal_Base_pet_animal_info();
	Json::Value Mod_PetAnimal_Base_feed_pet_animal();
	Json::Value Mod_PetAnimal_Base_up_pet_animal();

	//============================================================================
	// - sxd_client_travel_event.cpp		仙旅奇缘
	//============================================================================
	void travel_event();
	Json::Value Mod_TravelEvent_Base_get_event_and_answer();
	Json::Value Mod_TravelEvent_Base_lottery_draw();
	Json::Value Mod_TravelEvent_Base_answer_travel_event(int event_id, int answer_id);

	//============================================================================
	// - sxd_client_hunt_demon.cpp		猎妖
	//============================================================================
	void hunt_demon();
	Json::Value Mod_HuntDemon_Base_open_hunt_demon();
	Json::Value Mod_HuntDemon_Base_hunt_demon(int type);

	//============================================================================
	// - sxd_client_awake.cpp		觉醒
	//============================================================================
	void awake();
	Json::Value Mod_Awake_Base_player_forbidden_book_info();
	Json::Value Mod_Awake_Base_explore_forbidden_book(int forbidden_type, int times_type);

	//============================================================================
	// - sxd_client_bai_lian_qian_kun.cpp		百炼乾坤
	//============================================================================
	void bai_lian_qian_kun();
	Json::Value Mod_BaiLianQianKun_Base_one_key_challenge();
	Json::Value Mod_BaiLianQianKun_Base_get_info();
	Json::Value Mod_BaiLianQianKun_Base_challenge();

	//============================================================================
	// - sxd_client_five_elements_laba.cpp		五行天仪
	//============================================================================
	void five_elements_laba();
	Json::Value Mod_Laba_Base_laba_info();
	Json::Value Mod_Laba_Base_draw();

	//============================================================================
	// - sxd_client_roll_cake.cpp		吉星高照
	//============================================================================
	void roll_cake();
	Json::Value Mod_RollCake_Base_get_state();
	Json::Value Mod_RollCake_Base_get_count();
	Json::Value Mod_RollCake_Base_roll();
	Json::Value Mod_RollCake_Base_reroll();
	Json::Value Mod_RollCake_Base_get_award();

	//============================================================================
	// - sxd_client_send_flower.cpp		送花
	//============================================================================
	void send_flower();
	Json::Value Mod_Friend_Base_get_friend_list();
	Json::Value Mod_SendFlower_Base_player_send_flower_info(int player_id);
	Json::Value Mod_SendFlower_Base_send_player_flower(int player_id);

	//============================================================================
	// - sxd_client_fate.cpp		命格
	//============================================================================
	void batch_fate(int count);
	void fate();
	void Mod_Fate_Base_merge_all_in_bag();
	Json::Value Mod_Fate_Base_get_fate_npc();
	Json::Value Mod_Fate_Base_appoint_fate_npc(int npc_id);
	Json::Value Mod_Fate_Base_get_temp_fate();
	Json::Value Mod_Fate_Base_pickup_fate(const std::vector<long long> &temp_fate_ids);
	Json::Value Mod_Fate_Base_sale_temp_fate(const std::vector<long long> &temp_fate_ids);

	//============================================================================
	// - sxd_client_find_immortal.cpp			喜从天降 画龙鱼
	//============================================================================
	void find_immortal();
	Json::Value Mod_FindImmortal_Base_open_find_immortal();
	Json::Value Mod_FindImmortal_Base_start_find_immortal();
	Json::Value Mod_FindImmortal_Base_pickup_award();

	void find_immortal2();
	Json::Value Mod_FindImmortal_Base_is_five_blessings_open();
	Json::Value Mod_FindImmortal_Base_open_five_blessings();
	Json::Value Mod_FindImmortal_Base_start_bless();
	Json::Value Mod_FindImmortal_Base_end_bless();

	//============================================================================
	// - sxd_client_rob_money.cpp		劫镖
	//============================================================================
	void rob_money();
	Json::Value Mod_RobMoney_Base_get_rob_money_info();
	Json::Value Mod_RobMoney_Base_search();
	Json::Value Mod_RobMoney_Base_rob();

	//============================================================================
	// - sxd_client_nine_regions.cpp		九空无界
	//============================================================================
	void nine_regions();
	Json::Value Mod_NineRegions_Base_open_panel();
	Json::Value Mod_NineRegions_Base_get_calabash_count();
	Json::Value Mod_NineRegions_Base_get_calabash_info(int cur_jie);
	Json::Value Mod_NineRegions_Base_call(int cur_jie);
	Json::Value Mod_NineRegions_Base_gathering(int cur_jie);
	Json::Value Mod_NineRegions_Base_collect(int cur_jie);

	//============================================================================
	// - sxd_client_dance.cpp		群仙乱舞
	//============================================================================
	void dance();
	Json::Value Mod_Dance_Base_get_dance_info();
	Json::Value Mod_Dance_Base_start_dance();
	Json::Value Mod_Dance_Base_receive_award();

	//============================================================================
	// - sxd_client_marry.cpp		结婚
	//============================================================================
	void marry();
	void marry_gift();
	Json::Value Mod_Marry_Base_get_marry_box(); 
	Json::Value Mod_Marry_Base_get_player_engagement_info();
	Json::Value Mod_Marry_Base_give_gift(int gift_id);

	//============================================================================
	// - sxd_client_coin_mission.cpp		铜钱副本
	//============================================================================
	void coin_mission();
	Json::Value Mod_CoinMission_Base_get_coin_mission_info();
	Json::Value Mod_CoinMission_Base_get_deploys(int mission_id);
	Json::Value Mod_CoinMission_Base_fight(int mission_id);

	//============================================================================
	// - sxd_client_hero_mission.cpp		英雄副本
	//============================================================================
	void hero_mission();
	Json::Value Mod_HeroMission_Base_get_hero_mission_list(int town_id);
	Json::Value Mod_HeroMission_Base_start_practice(int town_id, int auto_num, int hero_type);
	Json::Value Mod_HeroMission_Base_quickly();

	//============================================================================
	// - sxd_client_lucky_super_number.cpp		幸运大比拼
	//============================================================================
	void lucky_super_number();
	Json::Value Mod_StLuckySuperNumber_Base_get_lucky_super_number_info();
	Json::Value Mod_StLuckySuperNumber_Base_draw(int position);

	//============================================================================
	// - sxd_client_sunday_fruit.cpp		周末水果机
	//============================================================================
	void sunday_fruit();
	Json::Value Mod_SundayFruit_Base_active_status();
	Json::Value Mod_SundayFruit_Base_sunday_fruit_info();
	Json::Value Mod_SundayFruit_Base_draw();

	//============================================================================
	// - sxd_client_partner_link.cpp		天缘
	//============================================================================
	void partner_link();
	Json::Value Mod_PartnerLink_Base_get_shopping_info();
	Json::Value Mod_PartnerLink_Base_buy_item(int type);

	//============================================================================
	// - sxd_client_dragonball.cpp		龙珠
	//============================================================================
	void dragonball();
	Json::Value Mod_Dragonball_Base_get_dragonball_skill_data();
	Json::Value Mod_Dragonball_Base_get_tmp_dragonball_basic_info();
	Json::Value Mod_Dragonball_Base_sacrifice(int mode, int auuto);
	Json::Value Mod_Dragonball_Base_collect_dragonball(int ids[], int count);
	Json::Value Mod_Dragonball_Base_sell_dragonball(int ids[], int count);

	//============================================================================
	// - npc_friendship.cpp
	//============================================================================
	void npc_friendship();
	Json::Value Mod_NpcFriendship_Base_get_info();
	Json::Value Mod_NpcFriendship_Base_get_give_present_info();
	Json::Value Mod_NpcFriendship_Base_give_present(int id, int npc_id);

	//============================================================================
	// - sxd_client_tower.cpp			六道轮回
	//============================================================================
	void tower();
	Json::Value Mod_Tower_Base_get_tower_info();
	Json::Value Mod_Tower_Base_start_challenge(int war_idx, int auto_sell_l, int auto_sell_z);
	Json::Value Mod_Tower_Base_reset_layer();
	Json::Value Mod_Tower_Base_start_auto_challenge(int auto_sell_l, int auto_sell_z);
	Json::Value Mod_Tower_Base_quickly_auto_challenge(int auto_sell_l, int auto_sell_z);
	Json::Value Mod_Tower_Base_enter_next();

	//============================================================================
	// - sxd_client_faction.cpp			帮派
	//============================================================================
	std::string get_faction_name();

	void faction_god();
	Json::Value Mod_Faction_Base_faction_god_info();
	Json::Value Mod_Faction_Base_incense();

	void faction_roll_cake();
	Json::Value Mod_Faction_Base_faction_roll_cake_info();
	Json::Value Mod_Faction_Base_roll_cake();

	void seal_satan();
	Json::Value Mod_Faction_Base_seal_satan_member_list();
	void Mod_Faction_Base_join_seal_satan();
	//Json::Value Mod_Faction_Base_join_seal_satan();

	void faction_lucky_wheel();
	Json::Value Mod_LuckyWheel_Base_open_lucky_wheel();
	Json::Value Mod_LuckyWheel_Base_start_lucky_wheel();

	void faction_join_feast();
	Json::Value Mod_Faction_Base_join_feast();

	void faction_approve();
	Json::Value Mod_Faction_Base_my_faction_info();
	Json::Value Mod_Faction_Base_request_list(int faction_id);
	Json::Value Mod_Faction_Base_accept_request(int id);

	void faction_war();
	Json::Value Mod_FactionWar_Base_get_faction_war_list();
	Json::Value Mod_FactionWar_Base_sign_up();

	//============================================================================
	// - sxd_client_assistant.cpp			活跃度，小助手
	//============================================================================
	void assistant();
	Json::Value Mod_Assistant_Base_info();
	Json::Value Mod_Assistant_Base_get_award(int sn);

	//============================================================================
	// - sxd_client_fish.cpp				钓鱼
	//============================================================================
	void fish();
	Json::Value Mod_Fish_Base_get_player_fishhook();
	Json::Value Mod_Fish_Base_do_fishing();

	//============================================================================
	// - sxd_client_super_town.cpp		仙界
	//============================================================================
	int login_super_town(sxd_client* sxd_client_town);
	Json::Value Mod_StcLogin_Base_get_status();
	Json::Value Mod_StcLogin_Base_get_login_info();
	Json::Value Mod_StLogin_Base_login(const std::string& server_name, int player_id_town, const std::string& nickname, int time, const std::string& pass_code);
	Json::Value Mod_StTown_Base_enter_town();
	Json::Value Mod_StTown_Base_get_players();
	void st_town_move_to(int x, int y);
	Json::Value Mod_StTown_Base_move_to(int x1, int y1, int x2, int y2);

	//仙界虚空
	void StVoid();
	Json::Value Mod_StVoid_Base_open_panel();
	Json::Value Mod_StVoid_Base_like(int id);

	//============================================================================
	// - sxd_client_st_union.cpp	仙盟
	//============================================================================
	std::string get_st_union_name();
	Json::Value Mod_StUnion_Base_get_player_st_union_info();
	void st_union_god_incense();
	Json::Value Mod_StUnionActivity_Base_st_union_god_incense(int id);
	void st_union_activity();
	Json::Value Mod_StUnionActivity_Base_get_st_union_tree_info();
	Json::Value Mod_StUnionActivity_Base_need_bless_player();
	Json::Value Mod_StUnionActivity_Base_choose_wish_item(int id);
	Json::Value Mod_StUnionActivity_Base_start_bless();
	Json::Value Mod_StUnionActivity_Base_bless_st_union_player(int id);
	Json::Value Mod_StUnionActivity_Base_player_get_tree_gift();
	void st_union_task();
	Json::Value Mod_StUnionTask_Base_get_challenge_info();
	Json::Value Mod_StUnionTask_Base_fight(); 
	Json::Value Mod_StUnionTask_Base_buy_ticket();
	std::vector<Json::Value> get_all_st_union_members();
	Json::Value Mod_StUnionTask_Base_get_st_union_list(int page);
	Json::Value Mod_StUnionTask_Base_get_st_union_info_simp(int st_union_i);
	void st_union_approve();
	Json::Value Mod_StUnion_Base_get_request_join_info();
	Json::Value Mod_StUnion_Base_deal_join_request(int id);
	void st_union_nimal();
	Json::Value Mod_StUnionAnimal_Base_get_animal_info();
	Json::Value Mod_StUnionAnimal_Base_fight(int type);

	//============================================================================
	// - sxd_client_wish_pool.cpp		仙界许愿池
	//============================================================================
	void wish_pool();
	Json::Value Mod_WishPool_Base_get_wish_pool_info();
	Json::Value Mod_WishPool_Base_choose_awards(int ids[], int count);
	Json::Value Mod_WishPool_Base_wish_self();
	Json::Value Mod_WishPool_Base_get_wish_list();
	Json::Value Mod_WishPool_Base_wish_other(int id);
	Json::Value Mod_WishPool_Base_get_award(int flag);

	//============================================================================
	// - sxd_client_marry_home.cpp			家园
	//============================================================================
	void furniture_effect();
	Json::Value Mod_MarryHome_Base_batch_get_furniture_effect();

	//============================================================================
	// - sxd_client_st_super_sport.cpp		神魔竞技，仙界商店（道源商店），游乐城大转盘
	//============================================================================
	void get_rank_award(sxd_client* sxd_client_town);
	Json::Value Mod_StSuperSport_Base_get_player_st_super_sport();
	Json::Value Mod_StSuperSport_Base_get_rank_award(int level);
	void get_score_award();
	Json::Value Mod_StSuperSport_Base_can_get_score_award();
	Json::Value Mod_StSuperSport_Base_player_score_award_info();
	Json::Value Mod_StSuperSport_Base_player_get_score_award(int index);
	void point_race(sxd_client* sxd_client_town);
	Json::Value Mod_StSuperSport_Base_get_st_super_sport_status();
	Json::Value Mod_StSuperSport_Base_challenge_list();
	Json::Value Mod_StSuperSport_Base_get_rank_award();
	Json::Value Mod_StSuperSport_Base_challenge(int index);
	void war_race(sxd_client* sxd_client_town);
	Json::Value Mod_StSuperSport_Base_get_race_step();
	Json::Value Mod_StSuperSport_Base_get_race_list(int group);
	Json::Value Mod_StSuperSport_Base_bet(int id);

	// - sxd_client_st_daoyuan_shop.cpp
	void st_daoyuan_shop();
	Json::Value Mod_StDaoyuanShop_Base_daoyuan_shop_item_list();
	Json::Value Mod_StDaoyuanShop_Base_buy_daoyuan_shop_item(int id, int count);

	// - sxd_client_st_big_turntable.cpp
	void st_big_turntable();
	Json::Value Mod_StBigTurntable_Base_get_big_turntable_info();
	Json::Value Mod_StBigTurntable_Base_turntable();

	//============================================================================
	// - sxd_client_st_altar.cpp			仙界神坛
	//============================================================================
	void st_altar();
	Json::Value Mod_StAltar_Base_get_altar_info();
	Json::Value Mod_StAltar_Base_get_end_award();

	//============================================================================
	// - sxd_client_st_mine.cpp			矿山系统
	//============================================================================
	void st_mine();
	Json::Value Mod_StMine_Base_get_mine_mountain_info();
	Json::Value Mod_StMine_Base_enter_mine_mountain(int id);
	Json::Value Mod_StMine_Base_leave_mine_hole();
	Json::Value Mod_StMine_Base_receive_award();
	Json::Value Mod_StMine_Base_rob_mine_hole(int hole_id, int robbed_player_id);

	//============================================================================
	// - sxd_client_st_practice_room.cpp 仙界练功房
	//============================================================================
	void st_practice_room();

	Json::Value Mod_StPracticeRoom_Base_get_rooms();
	Json::Value Mod_StPracticeRoom_Base_enter_room();
	Json::Value Mod_StPracticeRoom_Base_get_free_room_info();
	Json::Value Mod_StPracticeRoom_Base_get_free_room_info_by_page(int page);
	Json::Value Mod_StPracticeRoom_Base_get_room_info(int id);

	Json::Value Mod_StPracticeRoom_Base_get_seat_info(int room_id, int seat_id);
	Json::Value Mod_StPracticeRoom_Base_get_exp_flag();
	Json::Value Mod_StPracticeRoom_Base_player_leave_seat();
	Json::Value Mod_StPracticeRoom_Base_get_room_practice_exp();
	Json::Value Mod_StPracticeRoom_Base_sit_down(int room_id, int seat_id);

	//============================================================================
	// - sxd_client_st_take_bible.cpp 仙界取经
	//============================================================================
	void st_take_bible();
	Json::Value Mod_StTakeBible_Base_get_take_bible_info();
	Json::Value Mod_StTakeBible_Base_refresh();
	Json::Value Mod_StTakeBible_Base_start_take_bible();

	void st_rob_bible(sxd_client* sxd_client_town);
	Json::Value Mod_StTakeBible_Base_open_take_bible();
	Json::Value Mod_StTakeBible_Base_get_rob_take_bible_cd_time();
	Json::Value Mod_StTakeBible_Base_get_player_take_bible(int player_id, int my_level, int loc3, int sequence_id);
	Json::Value Mod_StTakeBible_Base_rob(int rob_id, int sequence_id);

	//============================================================================
	// - sxd_client_st_arena.cpp		仙界			仙界竞技场，荣誉商店
	//============================================================================
	void st_arena();
	Json::Value Mod_StArena_Base_get_race_step();
	Json::Value Mod_StArena_Base_open_st_arena();
	Json::Value Mod_StArena_Base_challenge(int player_id);
	Json::Value Mod_StArena_Base_refresh_player_list();
	void exploit_shop();
	Json::Value Mod_StArena_Base_exploit_shop_item_list();
	Json::Value Mod_StArena_Base_buy_exploit_shop_item(int id, int count);

	//============================================================================
	// - sxd_client_saint_area.cpp		圣域
	//============================================================================
	int login_saint_area(sxd_client* sxd_client_town);
	Json::Value Mod_SaintAreaLogin_Base_get_status();
	Json::Value Mod_SaintAreaLogin_Base_get_login_info();
	Json::Value Mod_SaintAreaLogin_Base_login(const std::string& server_name, int player_id, const std::string& nickname, int time, const std::string& pass_code);
	Json::Value Mod_SaintAreaTown_Base_enter_town();

	//============================================================================
	// - sxd_client_sa_take_bible.cpp			圣域取经
	//============================================================================
	void sa_take_bible();
	Json::Value Mod_SaTakeBible_Base_get_take_bible_info();
	Json::Value Mod_SaTakeBible_Base_refresh();
	Json::Value Mod_SaTakeBible_Base_start_take_bible();

	//============================================================================
	// - sxd_client_sa_super_sport.cpp				圣域竞技场
	//============================================================================
	void sa_super_sport();
	Json::Value Mod_SaSuperSport_Base_race_info();
	Json::Value Mod_SaSuperSport_Base_challage(int seq, int rank);

	//============================================================================
	// - sxd_client_sa_trials.cpp				圣域秘境
	//============================================================================
	void sa_trials();
	Json::Value Mod_SaTrials_Base_get_panel_info();
	Json::Value Mod_SaTrials_Base_open_box();
	Json::Value Mod_SaTrials_Base_choose_box_type();
	Json::Value Mod_SaTrials_Base_gain_step_award();
	Json::Value Mod_SaTrials_Base_choose_step_award();
	Json::Value Mod_SaTrials_Base_challage();

	//============================================================================
	// - sxd_client_satrials.cpp   圣域秘境
	//============================================================================
	void satrials();
	Json::Value Mod_satrials_Base_get_panel_info();
	Json::Value Mod_satrials_Base_reset_trials();
	Json::Value Mod_satrials_Base_open_box();
	Json::Value Mod_satrials_Base_choose_box_type(int floor);
	Json::Value Mod_satrials_Base_choose_special_buff(int floor);
	Json::Value Mod_satrials_Base_gain_step_award();
	Json::Value Mod_satrials_Base_choose_step_award(int floor, int k);
	Json::Value Mod_satrials_Base_challenge();
	Json::Value Mod_satrials_Base_get_state();

	//============================================================================
	// - sxd_client_server_chat_room.cpp			聊天室 宠物派遣
	//============================================================================
	int login_server_chat(sxd_client* sxd_client_town);
	Json::Value Mod_ServerChatRoom_Base_get_recent_room_list();
	Json::Value Mod_ServerChatRoom_Base_get_chat_room_status();
	Json::Value Mod_ServerChatRoom_Base_get_chat_room_logincode(int id);
	Json::Value Mod_ServerChatRoom_Base_login_chat_room(const std::string& node, int player_id, const std::string& servername, const std::string& stagename, int timestamp, const std::string& login_code);
	void pet_escort(sxd_client* sxd_client_town);
	Json::Value Mod_ServerChatRoom_Base_get_player_pet_escort_info();
	Json::Value Mod_ServerChatRoom_Base_feed_pet(int type);
	Json::Value Mod_ServerChatRoom_Base_escort_pet(int type);
	Json::Value Mod_ServerChatRoom_Base_chat_with_players(const std::string& message, const std::string& eip_num = "", const std::string& eip_index = "");
	Json::Value Mod_ServerChatRoom_Base_get_pet_escort_award();
	Json::Value Mod_ServerChatRoom_Base_escort_pet_panel();

	//============================================================================
	// - sxd_client_quest.cpp			任务系统
	//============================================================================
	void auto_quest();
	void fight_mission(int mission_id);
	void accept_quest(int town_id);
	std::vector<int> get_monster_team_by_missionid(int mission_id);
	std::vector<int> get_monster_teamid_by_missionid(int mission_id);
	Json::Value Mod_Quest_Base_list_player_quest();
	Json::Value Mod_Quest_Base_can_receive_quest(int town_map_id);
	Json::Value Mod_Quest_Base_accept_quest(int quest_id);
	Json::Value Mod_Quest_Base_complete_quest(int quest_id);
	Json::Value Mod_Quest_Base_finish_quest(int quest_id);
	Json::Value Mod_Mission_Base_get_sections(int town_map_id);
	Json::Value Mod_Mission_Base_enter_mission(int mission_id);
	Json::Value Mod_Mission_Base_fight_monster(int mission_monster_team_id);
	Json::Value Mod_MissionPractic_Base_get_practice_info(int mission_Id);
	Json::Value Mod_Mission_Base_open_box();
	Json::Value Mod_Mission_Base_pickup_award();

	//============================================================================
	// - sxd_client_shanhaiworld.cpp			山海界 通天塔
	//============================================================================
	int shanhaiworld_login(sxd_client* sxd_client_town);
	Json::Value Mod_ShanhaiWorld_Base_get_login_info(int node_id);
	Json::Value Mod_ShanhaiWorld_Base_login(const std::string& server_name, int player_id, int time, const std::string& pass_code);
	Json::Value Mod_ShanhaiWorld_Base_get_room_list();
	Json::Value Mod_ShanhaiWorld_Base_enter_town(int node_id);

	void tong_tian_tower_challenge();
	void tong_tian_tower_challenge(int heaven, int floor, Json::Value data);
	void tong_tian_tower_challenge(int heaven, int floor, Json::Value data_main, Json::Value data_soul);
	void tong_tian_tower_practice();
	Json::Value Mod_TongTianTower_Base_panel_info();
	Json::Value Mod_TongTianTower_Base_challenge(Json::Value data_k);
	Json::Value Mod_TongTianTower_Base_practice(int cur_heaven, int cur_floor, int times);

	//============================================================================
	// - sxd_client_StEightImmortals.cpp			八仙过海
	//============================================================================
	void StEightImmortals();
	Json::Value Mod_StEightImmortals_Base_get_finished_missions(int mission_id);
	Json::Value Mod_StEightImmortals_Base_enter_mission(int mission_id);
	Json::Value Mod_StEightImmortals_Base_fight();

	//============================================================================
	// - sxd_client_master_practice.cpp				宗师修行
	//============================================================================
	void MasterPractice();
	void challenge(const std::string& job, int job_id, int battleStars);
	Json::Value Mod_MasterPractice_Base_main_panel();
	Json::Value Mod_MasterPractice_Base_start_challenge(int job_id, int battleStars);
	//Json::Value Mod_MasterPractice_Base_fight();
	

	//============================================================================
	// - sxd_client_DevilExpedition.cpp				魔界远征
	//============================================================================
	void DevilExpedition();
	void Single_DevilExpedition();
	Json::Value Mod_DevilExpedition_Base_enter_town(int town_id);
	Json::Value Mod_DevilExpedition_Base_leave_town();
	Json::Value Mod_DevilExpedition_Base_quit();
	Json::Value Mod_DevilExpedition_Base_get_mission_data_m1();

	Json::Value Mod_DevilExpedition_Base_get_mission_data_m7();
	Json::Value Mod_DevilExpedition_Base_fight_monster_m7(int mission_id);
	Json::Value Mod_DevilExpedition_Base_notify_mission_data_change_m7();

	Json::Value Mod_DevilExpedition_Base_get_fight_hermit_record();

	Json::Value Mod_DevilExpedition_Base_get_mission_data_m6();
	Json::Value Mod_DevilExpedition_Base_fight_monster_m6(int mission_id);

	Json::Value Mod_DevilExpedition_Base_start_expedition_single(int mission_id);
	Json::Value Mod_DevilExpedition_Base_reject_buy_box();

	//Json::Value Mod_DevilExpedition_Base_get_mission_data_m6();
	Json::Value Mod_DevilExpedition_Base_fight_monster_m5(int mission_id);

	//============================================================================
	// - sxd_client_PhantomGemLottery.cpp				多宝山
	//============================================================================
	void PhantomGemLottery();
	Json::Value Mod_PhantomGemLottery_Base_panel_info();
	Json::Value Mod_PhantomGemLottery_Base_find(int id, int type);
	Json::Value Mod_PhantomGemLottery_Base_get_requite_award();


	//============================================================================
	// - sxd_client_MemoryZone.cpp						回忆之境
	//============================================================================
	void MemoryZone();
	Json::Value Mod_MemoryZone_Base_back_panel(int curtype);
	Json::Value Mod_MemoryZone_Base_backtrack(int role_id);


	//============================================================================
	// - sxd_client_ImmortalLeague.cpp   神仙联赛
	//============================================================================

	//============================================================================
	// - sxd_client_ClearClouds.cpp			拨云见日
	//============================================================================
	void ClearClouds();
	Json::Value Mod_ClearClouds_Base_open_buy_panel();
	Json::Value Mod_ClearClouds_Base_buy_gift(int id);


	//============================================================================
	// - sxd_client_MainRoleFaXiang.cpp   法相，守护魔兽 faxiang，fa_xiang，MRFXExtraMo, zhu_jue_fo:int = 1;zhu_jue_mo:int = 2
	//============================================================================
	void MainRoleFaXiang();
	void MRFXExtraMo();
	void MRFXExtraFo();
	Json::Value Mod_MainRoleFaXiang_Base_extra_mo_info();
	Json::Value Mod_MainRoleFaXiang_Base_extra_mo_challenge();

	Json::Value Mod_MainRoleFaXiang_Base_extra_fo_info();		//祈愿信息
	Json::Value Mod_MainRoleFaXiang_Base_extra_pray();			//祈愿
	Json::Value Mod_MainRoleFaXiang_Base_extra_refresh_pray();	//刷新祈愿道具
	Json::Value Mod_MainRoleFaXiang_Base_extra_choose_pray_award(int item_id);	//选择祈愿道具
	Json::Value Mod_MainRoleFaXiang_Base_gain_pray_award();			//收获祈愿道具


	//============================================================================
	// - sxd_client_WuFaField.cpp   悟法领域 WuFaField，gongfa
	//============================================================================
	void WuFaField();
	Json::Value Mod_WuFaField_Base_panel_info();
	Json::Value Mod_WuFaField_Base_draw(int type, int num);

	//============================================================================
	// - sxd_client_FiveElementsPalace.cpp   五行天宫 FiveElementsPalace，FiveElemetsPalace
	//============================================================================
	void FiveElementsPalace();
	Json::Value Mod_FiveElementsPalace_Base_five_elements_info();
	Json::Value Mod_FiveElementsPalace_Base_get_deploy();
	Json::Value Mod_FiveElementsPalace_Base_deploy(int role_id, int grid_id);
	Json::Value Mod_FiveElementsPalace_Base_fight(int mission_id);
	Json::Value Mod_FiveElementsPalace_Base_fight_boss(int mission_id);

	//============================================================================
	// - sxd_client_Fairyland.cpp   九霄云巅
	//============================================================================
	void Fairyland();
	Json::Value Mod_Fairyland_Base_fairyland_info();
	Json::Value Mod_Fairyland_Base_fight(int floor);

	//============================================================================
	// - sxd_client_RepressDemonTower.cpp   锁妖塔
	//============================================================================
	void RepressDemonTower();
	Json::Value Mod_RepressDemonTower_Base_panel_info();
	Json::Value Mod_RepressDemonTower_Base_challenge(int floor);
	Json::Value Mod_RepressDemonTower_Base_get_monster_team_info(int floor);

	//============================================================================
	// - sxd_client_MysticCapture.cpp   仙幽猎境
	//============================================================================
	void MysticCapture();
	Json::Value Mod_MysticCapture_Base_click_enter_hunt();
	Json::Value Mod_MysticCapture_Base_enter_hunt();
	Json::Value Mod_MysticCapture_Base_capture_soul(int soul_id, int item_id);
	Json::Value Mod_MysticCapture_Base_exit_hunt();

	//============================================================================
	// - sxd_client_Sect.cpp   宗门
	//============================================================================
	void Sect();
	//登录宗门仙境
	int login_Sect_area(sxd_client* sxd_client_town);
	Json::Value Mod_SectLogin_Base_get_login_info(int group);
	Json::Value Mod_SectLogin_Base_login(int appId, int playerId, const std::string& serverName, const std::string& node, int time, const std::string& passCode);
	Json::Value Mod_SectLogin_Base_enter_town(int id,int sect_id);
	Json::Value Mod_SectLogin_Base_get_player_sect_info();
	//赏金堂
	void SectBonus(int sect_id);
	Json::Value Mod_Sect_Base_panel_info();
	Json::Value Mod_Sect_Base_sect_list();
	Json::Value Mod_SectBonus_Base_open_panel(int sect_id);
	Json::Value Mod_SectBonus_Base_get_gift(int sect_id);
	//商店
	void NewSectShop();
	Json::Value Mod_NewSectShop_Base_gift_shop_panel();
	Json::Value Mod_NewSectShop_Base_buy_gift_shop_item(int item_id);
	//灵兽
	void SectMonster(int sect_id);
	Json::Value Mod_SectMonster_Base_egg_panel_info(int sect_id);
	Json::Value Mod_SectMonster_Base_get_monster_list(int sect_id);
	Json::Value Mod_SectMonster_Base_push_soul();
	Json::Value Mod_SectMonster_Base_feed(int monster_id, int type);

	//============================================================================
	// - sxd_client_RaceOrder.cpp   赛事战令
	//============================================================================
	void RaceOrder();
	int get_award(int race_id);
	Json::Value Mod_RaceOrder_Base_main_panel();
	Json::Value Mod_RaceOrder_Base_get_award(int race_id);

	//============================================================================
	// - sxd_client_SeekImmortalRoad.cpp   寻仙之路
	//============================================================================
	void SeekImmortalRoad();
	Json::Value Mod_SeekImmortalRoad_Base_open_panel();
	Json::Value Mod_SeekImmortalRoad_Base_get_award(int award_id);
	Json::Value Mod_SeekImmortalRoad_Base_open_box();

	//============================================================================
	// - sxd_client_Monopoly.cpp   monopoly/monoploy	山河游历（骰子）
	//============================================================================
	void Monopoly();
	void upgrade_building(Json::Value building1, Json::Value building2);
	Json::Value Mod_Monoploy_Base_main_panel();
	Json::Value Mod_Monoploy_Base_roll_dice();
	Json::Value Mod_Monoploy_Base_build(int gridId, int buildingId);
	Json::Value Mod_Monoploy_Base_upgrade(int gridId);
	Json::Value Mod_Monoploy_Base_get_product();
	Json::Value Mod_Monoploy_Base_task_panel();
	Json::Value Mod_Monoploy_Base_get_task_award(int missionId);
	Json::Value Mod_Monoploy_Base_get_total_product();

	//============================================================================
	// - sxd_client_wanfa.cpp   万法界
	//============================================================================

	//登录万法界
	Json::Value Mod_WanfaWorld_Base_get_login_info();
	Json::Value Mod_WanfaWorld_Base_login(const std::string& server_name, int playerId, int time, const std::string& pass_code);
	Json::Value Mod_WanfaWorld_Base_enter_town(int town_id);

	//tombartifacts		诸法器冢
	void TombArtifacts();
	Json::Value Mod_TombArtifacts_Base_get_state();
	Json::Value Mod_TombArtifacts_Base_main_panel_info(int curid);
	Json::Value Mod_TombArtifacts_Base_draw(int curid,int typeb);

	//EndlessExpedition		诸法洞天
	void EndlessExpedition();
	Json::Value Mod_EndlessExpedition_Base_mission_panel();
	Json::Value Mod_EndlessExpedition_Base_mission_info(int mission_id);
	Json::Value Mod_EndlessExpedition_Base_challenge(int mission_id);
	Json::Value Mod_EndlessExpedition_Base_enter_next_mission();

	//无尽模式
	void Endlesschallenge();
	Json::Value Mod_EndlessExpedition_Base_endless_panel();
	Json::Value Mod_EndlessExpedition_Base_get_monster_team_info(int layer_id);
	Json::Value Mod_EndlessExpedition_Base_endless_challenge(int layer_id);
	Json::Value Mod_EndlessExpedition_Base_endless_enter_next_layer();

	//============================================================================
	// - sxd_client_GlazePavilion.cpp   琉璃宝阁
	//============================================================================
	void GlazePavilion();
	Json::Value Mod_GlazePavilion_Base_get_state();
	Json::Value Mod_GlazePavilion_Base_main_panel_info();
	Json::Value Mod_GlazePavilion_Base_get_score_award(int id);
	Json::Value Mod_GlazePavilion_Base_get_daily_task(int id);
	Json::Value Mod_GlazePavilion_Base_get_limited_buy(int id);

	//============================================================================
	// - sxd_client_SoulHuntBlessGif.cpp   灵域猎妖-灵域福赠
	//============================================================================
	void SoulHuntBlessGif();
	Json::Value Mod_SoulHuntBlessGif_Base_main_panel_info();
	Json::Value Mod_SoulHuntBlessGif_Base_get_award(int level, int id);

	//============================================================================
	// - sxd_client_CosmosFight.cpp   寰宇乱斗
	//============================================================================
	void CosmosFight();
	void choosebless();
	Json::Value Mod_CosmosFight_Base_get_state();
	Json::Value Mod_CosmosFight_Base_get_panel_info();
	Json::Value Mod_CosmosFight_Base_find_opponent();
	Json::Value Mod_CosmosFight_Base_fight();
	Json::Value Mod_CosmosFight_Base_exit_fight();
	Json::Value Mod_CosmosFight_Base_bless_or_treasure_panel_info(int type);
	Json::Value Mod_CosmosFight_Base_choose_bless_or_treasure(int type, int id);
	Json::Value Mod_CosmosFight_Base_gift_box_panel();
	Json::Value Mod_CosmosFight_Base_get_or_buy_gift_box(int gift_type, int id);

	//============================================================================
	// - sxd_client_LinlangPavilion.cpp   琳琅阁
	//============================================================================
	void LinlangPavilion();
	Json::Value Mod_LinlangPavilion_Base_open_panel();
	Json::Value Mod_LinlangPavilion_Base_refresh_item();
	Json::Value Mod_LinlangPavilion_Base_one_key_buy();
	Json::Value Mod_LinlangPavilion_Base_one_key_buy_panel(int page);

	//============================================================================
	// - sxd_client_XiuZhenRealm.cpp   修真境界
	//============================================================================
	void XiuZhenRealm();
	Json::Value Mod_XiuZhenRealm_Base_get_extra_award();

	//============================================================================
	// - sxd_client_HeroesBattle.cpp   群英战
	//============================================================================
	void HeroesBattleScoreRace();
	Json::Value Mod_HeroesBattleScoreRace_Base_score_race_panel();
	Json::Value Mod_HeroesBattleScoreRace_Base_award_panel();
	Json::Value Mod_HeroesBattleScoreRace_Base_get_award(int type, int id);
	Json::Value Mod_HeroesBattleScoreRace_Base_find_opponent();
	Json::Value Mod_HeroesBattleScoreRace_Base_fight();

};

#endif /* SXD_CLIENT_H_ */