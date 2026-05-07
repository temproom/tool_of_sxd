#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_Equipment_Base
{
public:
	static const int TURE = 26;
	static const int FALS = 27;
	static const int ACTION_SUCCESS = 28;
	static const int LEVEL_LIMIT = 33;
	static const int INSUFFICIENT_COINS = 34;
	static const int NO_ENOUGH_MATERIALS = 36;
	static const int LEVEL_2_FUNCTION_NO_OPEN = 37;
};

void sxd_client::sxd_client::upgrade()
{
	//获取所有物品信息
	Json::Value items_data = this->Mod_Item_Base_get_all_player_item_infos();

	//匹配降魔战魂819
	std::vector<Json::Value> equips;
	for (Json::Value item : items_data[0])
	{
		//降魔战魂819，无主=0；
		if (item[1].asInt() == 819 && item[17].asInt() == 0)
		{
			equips.push_back(item);
		}

		/*if (item[1].asInt() == 819)
		{
			equips.push_back(item);
		}*/
	}
	
	/*{
		Json::Value test_data = this->Mod_Item_Base_player_equip_use_reel(equips[0][0].asInt(),8657,0,0,0,0);
		std::cout << "中断测试！";
		int k;
		std::cin >> k;
	}*/

	//降魔战魂的数量
	int num = 0;
	if (equips.empty())
	{
		common::log("【批量强化】：背包内没有降魔战魂，请到npc金犀利处购买！！");
		return;
	}
	else
	{
		num = equips.size();
		common::log(boost::str(boost::format("【批量强化】：背包内有【%1%】个降魔战魂，将全部强化到太初十\n") % num));
		/*common::log(boost::str(boost::format("【批量强化】：背包内有【%1%】个降魔战魂，是否全部强化到太初十？\n") % num));
		common::log("\t继续请输入任意数字；退出请输入0！");
		int k;
		std::cin >> k;
		if (k == 0)return;*/
	}

	for (auto  equip: equips)
	{
		int player_item_id = equip[0].asInt();
		int level = equip[3].asInt();

		while (level < 191)
		{
			// 铜钱不够一亿则不强化
			Json::Value data = this->Mod_Player_Base_get_player_info();
			int64_t coins = data[3].asInt64();
			if (coins < 100000000)
			{
				common::log("【批量强化】：铜钱不足一亿，停止强化！");
				return;
			}

			//批量强化
			data = this->Mod_Equipment_Base_batch_upgrade_equip(player_item_id, -1);

			if (data[0].asInt() == Mod_Equipment_Base::LEVEL_LIMIT || data[0].asInt() == Mod_Equipment_Base::INSUFFICIENT_COINS || data[0].asInt() == Mod_Equipment_Base::NO_ENOUGH_MATERIALS || data[0].asInt() == Mod_Equipment_Base::LEVEL_2_FUNCTION_NO_OPEN)
				break;
			else if (data[0].asInt() != Mod_Equipment_Base::ACTION_SUCCESS)
			{
				common::log(boost::str(boost::format("【强化】[%1%] 失败，result[%2%]") % db.get_code(version, "Item", 819)["text"] % data[0]), iEdit);
				break;
			}
			common::log(boost::str(boost::format("【强化】[%1%] 至 [ %2% ] 级") % db.get_code(version, "Item", 819)["text"] % data[1][0][3]), iEdit);			
		
			//强化后等级
			level = data[1][0][3].asInt();
		}

		//太初只能用普通强化
		while (level >= 191 && level < 200)
		{
			// 铜钱不够一亿则不强化
			Json::Value one_data = this->Mod_Player_Base_get_player_info();
			int64_t coins = one_data[3].asInt64();
			if (coins < 100000000)
				return;

			one_data = this->Mod_Equipment_Base_upgrade_equip(player_item_id, -1, Mod_Equipment_Base::FALS);
			if (one_data[0].asInt() == Mod_Equipment_Base::LEVEL_LIMIT || one_data[0].asInt() == Mod_Equipment_Base::INSUFFICIENT_COINS || one_data[0].asInt() == Mod_Equipment_Base::NO_ENOUGH_MATERIALS || one_data[0].asInt() == Mod_Equipment_Base::LEVEL_2_FUNCTION_NO_OPEN)
				break;
			else if (one_data[0].asInt() != Mod_Equipment_Base::ACTION_SUCCESS)
			{
				common::log(boost::str(boost::format("【强化】[%1%] 失败，result[%2%]") % db.get_code(version, "Item", 819)["text"] % one_data[0]), iEdit);
				break;
			}
			common::log(boost::str(boost::format("【强化】[%1%] 至 [ %2% ] 级") % db.get_code(version, "Item", 819)["text"] % one_data[1][0][3]), iEdit);
			
			level = one_data[1][0][3].asInt();
		}

		//已强化到太初十
		if (level == 200)
		{
			common::log("【批量强化】：已强化到太初十！");
		}
	}
}

void sxd_client::equipment()
{
	// 200:"太初10级",201:"混沌1级",
	auto data = this->Mod_Role_Base_get_role_list(player_id);
	auto roles = data[15];
	data = this->Mod_Item_Base_get_all_player_item_infos();

	// 有主的才可能是装备
	std::vector<Json::Value> equips;
	std::copy_if(data[0].begin(), data[0].end(), std::back_inserter(equips), [](const Json::Value& x) { return x[17].asInt(); });
	for (const auto& equip : equips)
	{
		int player_item_id = equip[0].asInt();
		int player_role_id = equip[17].asInt();
		// 排除掉客栈里伙伴的装备
		if (std::find_if(roles.begin(), roles.end(), [player_role_id](const Json::Value& role) { return role[2].asInt() == player_role_id; }) == roles.end())
			continue;

		for (;;)
		{
			// 铜钱不够一亿则不强化
			data = this->Mod_Player_Base_get_player_info();
			int64_t coins = data[3].asInt64();
			if (coins < 100000000)
				return;

			data = this->Mod_Equipment_Base_upgrade_equip(player_item_id, -1, Mod_Equipment_Base::FALS);
			if (data[0].asInt() == Mod_Equipment_Base::LEVEL_LIMIT || data[0].asInt() == Mod_Equipment_Base::INSUFFICIENT_COINS || data[0].asInt() == Mod_Equipment_Base::NO_ENOUGH_MATERIALS || data[0].asInt() == Mod_Equipment_Base::LEVEL_2_FUNCTION_NO_OPEN)
				break;
			else if (data[0].asInt() != Mod_Equipment_Base::ACTION_SUCCESS)
			{
				common::log(boost::str(boost::format("【强化】[%1%] 失败，result[%2%]") % db.get_code(version, "Item", equip[1].asInt())["text"] % data[0]), iEdit);
				break;
			}
			common::log(boost::str(boost::format("【强化】[%1%] 至 [%2%] 级") % db.get_code(version, "Item", equip[1].asInt())["text"] % data[1][0][3]), iEdit);
		}
	}
}

void sxd_client::equip_use_reel()
{

	//获取主角 player_role_id
	Json::Value role_data = this->Mod_Role_Base_get_role_list(player_id);
	int main_player_role_id = role_data[0].asInt();

	//获取所有物品信息
	Json::Value items_data = this->Mod_Item_Base_get_all_player_item_infos();

	//匹配降魔战魂819
	std::vector<Json::Value> equips;
	for (Json::Value item : items_data[0])
	{
		//降魔战魂819，无主=0；
		if (item[1].asInt() == 819 && item[17].asInt() == 0)
		{
			equips.push_back(item);
		}

		/*if (item[1].asInt() == 819)
		{
			equips.push_back(item);
		}*/
	}
	for (Json::Value equip : equips)
	{
		int player_item_id = equip[0].asInt();
		int grid_id = equip[2].asInt();
		int level = equip[3].asInt();

		//太初十才制作逍遥
		if (level <  200)
		{
			break;
		}

		Json::Value data = this->Mod_Item_Base_equip_player_role_item(grid_id, main_player_role_id);
		if (data[0] = Mod_Equipment_Base::ACTION_SUCCESS)
		{
			common::log("【制作逍遥战魂】：装备降魔战魂成功！");
		}

		data = this->Mod_Item_Base_player_equip_use_reel(player_item_id, 942);
		if (data[0] = Mod_Equipment_Base::ACTION_SUCCESS)
		{
			common::log("【制作逍遥战魂】：制作逍遥战魂成功！");
		}
	}
	//auto data = this->Mod_Item_Base_player_equip_use_reel(eqPlayerItemId, reelItemId);
}

//============================================================================
// R196 强化
// "module":77,"action":7,"request":[Utils.IntUtil,Utils.IntUtil,Utils.UByteUtil],"response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// SuperUpgradeView.as:
//     this._data.call(Mod_Equipment_Base.upgrade_equip,callBack,[intPlayerItemId,intSelectGemId,!!intUseCoinUpgrade?Mod_Equipment_Base.TRUE:Mod_Equipment_Base.FALSE]);
// UpgradeData.as:
//     oObject.list(this._aryUpgradeEquip,_loc1_,["result"]);
// 旧版oObject.list(this._aryUpgradeEquip[1][0],_loc1_,["player_item_id","item_id","player_role_id","upgrade_level","upgrade_price","gold_oil_id"]);
// 新版oObject.list(this._aryUpgradeEquip[1][0],_loc1_,["player_item_id","item_id","player_role_id","upgrade_level","upgrade_price","gold_oil_id","batch_upgrade_price","batch_level"]);
// Example
//     旧版[ 46783924, -1, 27 ] -> [ **28**, [ [ 46783924, 1038, 808172, **59**, 648000, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_Equipment_Base_upgrade_equip(int player_item_id, int select_gem_id, int use_coin_upgrade)
{
	Json::Value data;
	data.append(player_item_id);
	data.append(select_gem_id);
	data.append(use_coin_upgrade);
	return this->send_and_receive(data, 77, 7);
}

//============================================================================
// 批量强化
// "module":77,"action":26,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.UByteUtil],
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// SuperUpgradeView.as:
//          _data.call(Mod_Equipment_Base.batch_upgrade_equip,ddcallBack,[obj["player_item_id"],intSelectGemId]);
// UpgradeData.as:
//			oObject.list(param1,this.batchUpgradeEquipInfo,["result","upgrade_equit_list","upgrade_level"]);
//    【upgrade_equit_list】：["player_item_id","item_id","player_role_id","upgrade_level","upgrade_price","gold_oil_id","batch_upgrade_price","batch_level"]);
// Example
// 
//============================================================================
Json::Value sxd_client::Mod_Equipment_Base_batch_upgrade_equip(int player_item_id, int select_gem_id)
{
	Json::Value data;
	data.append(player_item_id);
	data.append(select_gem_id);
	return this->send_and_receive(data, 77, 26);
}

//============================================================================
// 制作逍遥战魂
// "module":2,"action":98,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],
// superupgradeview.as.
//			this._data.call(Mod_Item_Base.player_equip_use_reel, callBack, [intPlayerItemId, intReelId, intProcessType, intKeepLv, slsFrame, allFrame]);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]

// ItemData.as:
//			this.playerEquipUseReel["result"] = param1[0];
//			this.playerEquipUseReel["itemId"] = param1[1];
// 
//============================================================================
Json::Value sxd_client::Mod_Item_Base_player_equip_use_reel(int PlayerItemId, int ReelId, int ProcessType, int KeepLv, int slsFrame, int allFrame)
{
	Json::Value data;
	data.append(PlayerItemId);
	data.append(ReelId);
	data.append(ProcessType);
	data.append(KeepLv);
	data.append(slsFrame);
	data.append(allFrame);
	return this->send_and_receive(data, 2, 98);
}


//============================================================================
// 购买装备
// "module":2,"action":80,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil],
// example：
// SuperUpgradeView.as
//			this._data.call(Mod_Item_Base.player_buy_equip_item,callBack,[intItemId,intPlayerRoleId,intPositionId]);
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.LongUtil,Utils.IntUtil]
// example：
// itemdata.as
//          this.playerBuyEquipItem["result"] = param1[0];
//			this.playerBuyEquipItem["player_item_id"] = param1[1];
//			this.playerBuyEquipItem["batch_upgrade_price"] = param1[2];
//			this.playerBuyEquipItem["batch_level"] = param1[3];
//     
//============================================================================
Json::Value sxd_client::Mod_Item_Base_player_buy_equip_item(int Item_Id, int Player_Role_Id, int Position_Id)
{
	Json::Value data;
	data.append(Item_Id);
	data.append(Player_Role_Id);
	data.append(Position_Id);
	return this->send_and_receive(data, 2, 80);
}

//============================================================================
// 获取角色身上装备
// "module":2,"action":43,
// "request":[Utils.IntUtil]
// example：
//rolemsgview.as         
//			_data.call(Mod_Item_Base.get_role_equip_list, this.get_role_equip_list_back, [this.playerRoleId]);
// "response":[[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]
// example：
// itemdata.as
//          this.playerBuyEquipItem["result"] = param1[0];
//			this.playerBuyEquipItem["player_item_id"] = param1[1];
//			this.playerBuyEquipItem["batch_upgrade_price"] = param1[2];
//			this.playerBuyEquipItem["batch_level"] = param1[3];
//     
//============================================================================
Json::Value sxd_client::Mod_Item_Base_get_role_equip_list(int Player_Role_Id)
{
	Json::Value data;
	data.append(Player_Role_Id);
	return this->send_and_receive(data, 2, 43);
}

//============================================================================
// 制作逍遥战魂
// "module":2,"action":98,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil],
// //superupgrade.as
//if(this._slsFrame == 2 && curSlsNum > 0 && objReelItem["item_type"] != ItemType.LianQiJuanZhou && objReelItem["item_id"] != this._shenLianShiItemId)

// superupgradeview.as.
//			this._data.call(Mod_Item_Base.player_equip_use_reel, callBack, [intPlayerItemId, intReelId, intProcessType, intKeepLv, slsFrame, allFrame]);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]

// ItemData.as:
//			this.playerEquipUseReel["result"] = param1[0];
//			this.playerEquipUseReel["itemId"] = param1[1];
// 
//============================================================================
Json::Value sxd_client::Mod_Item_Base_player_equip_use_reel(int eqPlayerItemId, int reelItemId)
{
	Json::Value data;
	data.append(eqPlayerItemId);	//装备id
	data.append(reelItemId);		//卷轴id，逍遥战魂制作卷 942，
	data.append(0);					//元宝制作，需要vip等级
	data.append(1);					//保留等级
	data.append(1);					//神炼石
	data.append(1);
	return this->send_and_receive(data, 2, 98);
}

//============================================================================
// 穿戴装备
// "module":2,"action":50,
// "request":[Utils.ShortUtil,Utils.IntUtil,Utils.ShortUtil],
// example：
// SuperUpgradeView.as
//			this._data.call(Mod_Item_Base.equip_player_role_item,callBack,[intWearGridId,intPlayerRoleId,0]);

// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]
// example：
// itemdata.as
//          this.playerBuyEquipItem["result"] = param1[0];
//			this.playerBuyEquipItem["player_item_id"] = param1[1];
//			this.playerBuyEquipItem["batch_upgrade_price"] = param1[2];
//			this.playerBuyEquipItem["batch_level"] = param1[3];
//     
//============================================================================
Json::Value sxd_client::Mod_Item_Base_equip_player_role_item(int WearGridId, int PlayerRoleId)
{
	Json::Value data;
	data.append(WearGridId);
	data.append(PlayerRoleId);
	data.append(0);
	return this->send_and_receive(data, 2, 50);
}

//============================================================================
// 移除装备
// "module":2,"action":51,
// "request":[Utils.IntUtil,Utils.ShortUtil,Utils.ShortUtil],
// example：
// SuperUpgradeView.as
//			this._data.call(Mod_Item_Base.player_buy_equip_item,callBack,[intItemId,intPlayerRoleId,intPositionId]);
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]
// example：
// itemdata.as
//          this.playerBuyEquipItem["result"] = param1[0];
//			this.playerBuyEquipItem["player_item_id"] = param1[1];
//			this.playerBuyEquipItem["batch_upgrade_price"] = param1[2];
//			this.playerBuyEquipItem["batch_level"] = param1[3];
//     
//============================================================================
Json::Value sxd_client::Mod_Item_Base_remove_player_role_equipment(int eqPlayerItemId, int reelItemId, int house)
{
	Json::Value data;
	data.append(eqPlayerItemId);
	data.append(reelItemId);
	data.append(house);
	return this->send_and_receive(data, 2, 51);
}

void sxd_client::buy_item()
{
	//获取背包物品信息
	Json::Value data = this->Mod_Item_Base_get_player_pack_item_list();
	//[ 48, 0, [ [ 70146158, 1411, 6, 1, 74, 640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71945829, 7879, 3, 1, 320, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 72005190, 8108, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71971292, 1007, 5, 1, 3, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71864586, 4238, 14, 1, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71998936, 3067, 12, 1, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71944427, 7262, 19, 1, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71998529, 1962, 11, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71665516, 1488, 8, 1, 1, 7447, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71287460, 1444, 7, 1, 1, 2122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71989722, 4237, 13, 1, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71933283, 7263, 21, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71999025, 8133, 25, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71871645, 4239, 15, 1, 0, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71814628, 8643, 26, 1, 0, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71893525, 6365, 16, 1, 0, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71987282, 7879, 2, 1, 320, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71838370, 7879, 4, 1, 320, 5802, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71821526, 7089, 18, 1, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71859524, 7264, 22, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71881091, 1787, 10, 1, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71950540, 9213, 24, 1, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71941258, 6759, 17, 1, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71719829, 2016, 9, 1, 1, 1493, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71944299, 7697, 23, 1, 0, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 72004173, 7262, 20, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ] ], 0, null ] 
	
	//背包物品数量
	int now_box = data[2].size();
	//背包剩余空格数
	int rest_box = data[0].asInt() - data[2].size();

	//天庭，金犀利(202)
	Json::Value data_buy = this->Mod_Item_Base_get_npc_item_list(202);
	for (int grid_id = 48; grid_id > now_box + 1; grid_id--)
	{
		//分别为【城镇NPCid，物品id，背包格子id】
		data_buy = this->Mod_Item_Base_player_buy_npc_item(202, 819, grid_id);
		if (data_buy[0].asInt() == 20)
		{
			common::log("【购买】购买降魔战魂成功！");
		}
	}	
}
void sxd_client::sell_item()
{
	//获取背包物品信息
	Json::Value data = this->Mod_Item_Base_get_player_pack_item_list();
	//[ 48, 0, [ [ 70146158, 1411, 6, 1, 74, 640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71945829, 7879, 3, 1, 320, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 72005190, 8108, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71971292, 1007, 5, 1, 3, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71864586, 4238, 14, 1, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71998936, 3067, 12, 1, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71944427, 7262, 19, 1, 0, 99, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71998529, 1962, 11, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71665516, 1488, 8, 1, 1, 7447, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71287460, 1444, 7, 1, 1, 2122, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71989722, 4237, 13, 1, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71933283, 7263, 21, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71999025, 8133, 25, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71871645, 4239, 15, 1, 0, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71814628, 8643, 26, 1, 0, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71893525, 6365, 16, 1, 0, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71987282, 7879, 2, 1, 320, 9999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71838370, 7879, 4, 1, 320, 5802, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71821526, 7089, 18, 1, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71859524, 7264, 22, 1, 0, 98, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71881091, 1787, 10, 1, 0, 174, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71950540, 9213, 24, 1, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71941258, 6759, 17, 1, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71719829, 2016, 9, 1, 1, 1493, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 71944299, 7697, 23, 1, 0, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ], [ 72004173, 7262, 20, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11 ] ], 0, null ] 

	//背包剩余空格数
	int rest_box = data[0].asInt() - data[2].size();
	//背包物品数量
	int now_box = data[2].size();

	Json::Value items = data[2];

	//逍遥战魂826
	for (const auto& item : items)
	{
		int item_id = item[1].asInt();
		int box_id = item[2].asInt();
		std::string item_name = db.get_code(version, "Item", item_id)["text"];

		if (item_id == 826)
		{
			Json::Value data_sell = this->Mod_Item_Base_player_sell_item(box_id);
			if (data_sell[0].asInt() != 20)
				common::log(boost::str(boost::format("【出售】自动出售 [%1%]失败，result[%2%]") % item_name % data_sell[0]), iEdit);
			else
				common::log(boost::str(boost::format("【背包】自动出售 [%1%] 成功！") % item_name), iEdit);
		}
	}

	/*for (const auto& item : items)
	{
		int item_id = item[1].asInt();
		int box_id = item[2].asInt();
		std::string item_name = db.get_code(version, "Item", item_id)["text"];
		// 1. 必须facture_reel中存在
		std::ostringstream where_clause;
		where_clause << "version='" << version << "' and item_id=" << item_id;
		auto items = db.get_records("facture_reel", where_clause.str());
		if (!items.size())
			continue;
		// 2. 必须是装备制作材料
		where_clause.str("");
		where_clause << "version='" << version << "' and type='Item' and value='" << item_id << "' and comment like '%" << common::gbk2utf("装备") << "%'";
		items = db.get_records("code", where_clause.str());
		if (!items.size())
			continue;
		// 3. 不能在LuckyStore2中存在
		if (items_config2.find(item_id) != items_config2.end())
			continue;
		// 4. 不能是有用的装备制作材料
		bool can_sell = true;
		for (const auto& equip : equips)
		{
			int player_role_id = equip[16].asInt();
			if (std::find_if(roles.begin(), roles.end(), [player_role_id](const Json::Value& role) { return role[2].asInt() == player_role_id; }) == roles.end())
				continue;
			auto materials = db.get_facture_reel(version, equip[1].asInt());
			if (std::find_if(materials.begin(), materials.end(), [item_id](mss& material) { return atoi(material["item_id"].c_str()) == item_id; }) != materials.end())
				can_sell = false;
		}
		// 5. 卖吧
		if (can_sell)
		{
			data = this->Mod_Item_Base_player_sell_item(box_id);
			if (data[0].asInt() != Mod_Item_Base::ACTION_SUCCESS)
				common::log(boost::str(boost::format("【背包】自动出售无用材料 [%1%]失败，result[%2%]") % item_name % data[0]), iEdit);
			else
				common::log(boost::str(boost::format("【背包】自动出售无用材料 [%1%]") % item_name), iEdit);
		}
	}*/
}
//============================================================================
// 出售物品
// "module":2,"action":73,
// "request":[Utils.ShortUtil],
// example：
// SuperUpgradeView.as
//			this._data.call(Mod_Item_Base.player_buy_equip_item,callBack,[intItemId,intPlayerRoleId,intPositionId]);
// "response":Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil],[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]]
// example：
// itemdata.as
//          this.playerBuyEquipItem["result"] = param1[0];
//			this.playerBuyEquipItem["player_item_id"] = param1[1];
//			this.playerBuyEquipItem["batch_upgrade_price"] = param1[2];
//			this.playerBuyEquipItem["batch_level"] = param1[3];
// 
//============================================================================
/*Json::Value sxd_client::Mod_Item_Base_player_sell_item(int ItemId)
{
	Json::Value data;
	data.append(ItemId);
	return this->send_and_receive(data, 2, 73);
}*/

//============================================================================
// 购买NPC物品
// "module":2,"action":75,
// "request":[Utils.IntUtil,Utils.IntUtil,Utils.ShortUtil],
//			
// example：
// ShopView.as
//			this.player_buy_npc_item(this.shopNpcId,param1.item,param2.id);
//				_data.call(Mod_Item_Base.player_buy_npc_item,this.player_buy_npc_item_back,[param1,param2.itemId,param3]);
//		【城镇NPCid，物品id，背包格子id】
// "response":[Utils.UByteUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ShortUtil,Utils.ShortUtil,Utils.ByteUtil,Utils.IntUtil]]
// example：
// 
//     _data.call(Mod_Item_Base.get_npc_item_list,param1,[this.shopNpcId],false);
//============================================================================
Json::Value sxd_client::Mod_Item_Base_player_buy_npc_item(int npcid, int eqPlayerItemId, int reelItemId)
{
	Json::Value data;
	data.append(npcid);
	data.append(eqPlayerItemId);
	data.append(reelItemId);
	return this->send_and_receive(data, 2, 75);
}
//============================================================================
// NPC物品信息
// "module":2,"action":71,
// "request":[Utils.IntUtil],
//			
// example：
// ShopView.as
//			 _data.call(Mod_Item_Base.get_npc_item_list,param1,[this.shopNpcId],false);
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,[Utils.IntUtil]]
// example：
// 
//    [ 20, 202, [ [ 906 ], [ 821 ], [ 835 ], [ 819 ], [ 871 ], [ 852 ], [ 822 ], [ 809 ], [ 846 ], [ 825 ], [ 820 ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_Item_Base_get_npc_item_list(int shopNpcId)
{
	Json::Value data;
	data.append(shopNpcId);
	return this->send_and_receive(data, 2, 71);
}

//新版一键强化
void sxd_client::one_key_upgrade()
{
	//获取主角 player_role_id
	Json::Value role_data = this->Mod_Role_Base_get_role_list(player_id);
	int main_player_role_id = role_data[0].asInt();

	Json::Value upgrade_data = this->Mod_Equipment_Base_all_equip_upgrade_type_2(main_player_role_id);
}


//============================================================================
// 一键强化
// "module":77,"action":32,
// "request":[Utils.IntUtil,Utils.ByteUtil],
//			
// example：
// AllEquipUpgradeView.as
			 /*
			 【equipType == 1】极限强化 灵尘
			 【equipType == 0】普通强化 铜钱
			 if(_ctrl.equipment.equipType == 1)
{
	_data.call(Mod_Equipment_Base.all_equip_upgrade_type_1, this.all_equip_upgrade_call_back, [this._playerRoleId, param1 >= 1 ? Mod_Equipment_Base.TRUE : Mod_Equipment_Base.FALSE, param2 * 10000]);
}
		 else
		 {
			 _data.call(Mod_Equipment_Base.all_equip_upgrade_type_2, this.all_equip_upgrade_call_back, [this._playerRoleId, param1]);
		 }*/
// 
// "response":[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil]
// example：
// 
//     
//============================================================================
Json::Value sxd_client::Mod_Equipment_Base_all_equip_upgrade_type_2(int playerRoleId)
{
	Json::Value data;
	data.append(playerRoleId);
	data.append(0);
	return this->send_and_receive(data, 77, 32);
}
