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
		common::log(boost::str(boost::format("【批量强化】：背包内有【%1%】个降魔战魂，是否全部强化到太初十？\n") % num));
		common::log("\t继续请输入任意数字；退出请输入0！");
		int k;
		std::cin >> k;
		if (k == 0)return;
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
// 购买装备
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