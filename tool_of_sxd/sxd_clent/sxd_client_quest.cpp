#include "stdafx.h"
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"
#include "vector"

#include <D:\boost_1_78_0\boost_1_78_0\boost/regex.hpp>
#include <D:\boost_1_78_0\boost_1_78_0\boost/algorithm/string.hpp>
#include <D:\boost_1_78_0\boost_1_78_0\boost/algorithm/string/regex.hpp>

class QuestType {
public:
    static const int LevelLimit = -1;
    static const int Acceptable = 0;
    static const int Accepted = 1;
    static const int Completed = 2;

	static const int SUCCEED = 5;
	static const int FAILED = 6;
	static const int PACKFULL = 7;
	static const int NONQUEST = 8;
	static const int NEWQUEST = 9;
};

class MissionType {
public:
	static const int SUCCESS = 10;
	static const int LESS_POWER = 18;

	static const int NORMAL = 0;
	static const int BOSS = 1;
	static const int HERO = 2;
};

//============================================================================
// R177 �Զ�����
//============================================================================
void sxd_client::auto_quest() {
	// û��������
	Json::Value data = this->Mod_Player_Base_get_player_info();
	if (data[6].asInt() < 5)
	{
		common::log("���Զ����񡿣���������!\n", iEdit);
		return;
	}

	//��ȡ��ǰ����
	data = this->Mod_Player_Base_get_player_info();
	int town_id = data[9].asInt();
	mss town_data = db.get_code(version, "Town", town_id);
	common::log(boost::str(boost::format("���Զ����񡿣���ǰ����Ϊ����%1%��\n")% town_data["text"]), iEdit);

	//���������������
	this->accept_quest(town_id);

	//��ǰ����δͨ�ظ���
	std::vector<int> not_mission_id;

	//��ǰ�������и�������Ϣ
	Json::Value data_accept = this->Mod_Mission_Base_get_sections(town_id);
	auto sections = data_accept[0];

	//��ת�б�һ�㶼������ս����ǰ��δ��ս���ں�
	//std::reverse(sections.begin(), sections.end());

	for (auto it : sections)
	{
		if (!it[1].asInt())
		{
			//��¼δͨ�ظ�����id
			not_mission_id.push_back(it[0].asInt());

			//����ҵ���Ӧ�ĸ�������
			//mss names = db.get_mission(version, it[0].asInt());

			//��¼δͨ�ظ���������
			//not_mission_name.push_back(names["mission_name"]);
		}
	}

	//180�������Ժ�ĳ��򣬲������һ����������ֹ����һ������
	//��ʱֻʣ���һ������û�򣬽����ݹ�
	if (not_mission_id.size() == 1)
	{	
		mss mission_data = db.get_mission(version, not_mission_id[0]);
		std::string name = mission_data["mission_name"];

		if (not_mission_id[0] >= 1182)
		{
			boost::smatch match;
			if (regex_search(name, match, boost::regex("[1-9]")))
			{
				std::string num = match[0];
				if (stoi(num) == 4)
				{
					common::log(boost::str(boost::format("���Զ����񡿣���ǰ���򣺡�%1%��ֻʣ���һ��������\n") % town_data["text"]), iEdit);
					return;
				}
			}
		}
	}
	else if (!not_mission_id.size())
	{
		common::log(boost::str(boost::format("���Զ����񡿣���ǰ���򣺡�%1%����ͨ�أ�\n") % town_data["text"]), iEdit);
		return;
	}
	
	//�򸱱�
	for (auto temp_mission_id : not_mission_id)
	{
		//common::log(boost::str(boost::format("temp_mission_id:%1%\n") % temp_mission_id), iEdit);
		this->fight_mission(temp_mission_id);
	}
	
	//�ݹ�ѭ����ֱ��û���������ߴ���������и������������һ����
	this->auto_quest();
}

//���������������
void sxd_client::accept_quest(int town_id)
{
	Json::Value data_accept;

	//��ǰ�ɽ�����
	Json::Value data = this->Mod_Quest_Base_can_receive_quest(town_id);

	//������
	if (data[0].size())
	{
		for (auto quest_id : data[0])
		{
			mss temp_quest = db.get_quest(version, quest_id[0].asInt());
			Json::Value temp_data = this->Mod_Quest_Base_accept_quest(quest_id[0].asInt());
			if (temp_data[0].asInt() == QuestType::SUCCEED) 
			{
				common::log(boost::str(boost::format("���Զ����񡿣��������񣺡�%1%���ɹ�\n") % temp_quest["quest_name"]), iEdit);
			}
		}
	}

	//��ǰ�����б�
	data_accept = this->Mod_Quest_Base_list_player_quest();

	Json::Value quests = data_accept[0];
	for (auto it : quests)
	{
		//������������
		if (it[1].asInt() == 2)
		{
			//�ύ����
			data_accept = this->Mod_Quest_Base_complete_quest(it[0].asInt());

			//��ȡ��������
			mss temp_quest = db.get_quest(version, it[0].asInt());
			Json::Value temp_quest_name = temp_quest["quest_name"];

			if (data_accept[0].asInt() == QuestType::SUCCEED)
			{
				common::log(boost::str(boost::format("���Զ����񡿣��ύ���񣺡�%1%���ɹ�\n") % temp_quest_name), iEdit);
			}
		}		
	}

	//�ݹ�ѭ��
	data = this->Mod_Quest_Base_can_receive_quest(town_id);
	if (!data[0].size())
	{
		return;
	}
	else
	{
		accept_quest(town_id);
	}	
}

//============================================================================
// R177 ����ս��	
//============================================================================
void sxd_client::fight_mission(int mission_id)
{
	//��ȡ������Ϣ
	mss mission_data = db.get_mission(version, mission_id);

	//180�������Ժ�ĳ��򣬲������һ����������ֹ����һ������
	std::string name = mission_data["mission_name"];
	if (mission_id >= 1182)
	{
		boost::smatch match;
		if (regex_search(name, match, boost::regex("[1-9]")))
		{
			std::string num = match[0];
			if (stoi(num) == 4)
			{
				common::log(boost::str(boost::format("������ս��������%1%��Ϊ��ǰ�������һ������������\n") % name), iEdit);		
				return;
			}
		}
	}
	Json::Value data_accept;

	//��ȡmonster_teams
	std::vector<int> monster_teams;
	monster_teams = this->get_monster_team_by_missionid(mission_id);

	//���븱��
	data_accept = this->Mod_Mission_Base_enter_mission(mission_id);
	if (data_accept[0].asInt() == MissionType::SUCCESS)
	{
		common::log(boost::str(boost::format("������ս���������븱������%1%���ɹ���\n") % mission_data["mission_name"]), iEdit);
	}
	else if(data_accept[0].asInt() == MissionType::LESS_POWER)
	{
		common::log(boost::str(boost::format("������ս���������븱������%1%��ʧ�ܣ�\t��������\n") % mission_data["mission_name"]), iEdit);
	}
	else
	{
		common::log(boost::str(boost::format("������ս���������븱������%1%��ʧ�ܣ�\n״̬��Ϣ����%2%��\n") % mission_data["mission_name"] % data_accept[0].asInt()), iEdit);
	}

	//���
	int i = 1;
	for (auto team_id : monster_teams)
	{
		Json::Value data_receive;

		//����ʧ�ܣ�������ս10��
		int j = 0;
		for (j;j < 10;j++)
		{
			//ս��
			data_receive = this->Mod_Mission_Base_fight_monster(team_id);

			//ս�����
			int status = data_receive[data_receive.size() - 3].asInt();

			if (status == MissionType::SUCCESS)
			{
				common::log(boost::str(boost::format("������ս��������%1%����ս����%2%_%3%���ɹ���\n") % (j + 1) % mission_data["mission_name"] % i), iEdit);
				i++;
				break;
			}
			else
			{
				common::log(boost::str(boost::format("������ս��������%1%����ս����%2%_%3%��ʧ�ܣ�\nstatus=%4%\n") % (j + 1) % mission_data["mission_name"] % i%status), iEdit);
			}
		}
		if (j == 10)
		{
			common::log(boost::str(boost::format("������ս��������������%1%����սʧ�ܣ�\n") % mission_data["mission_name"]), iEdit);
			return;
		}
	}
	this->Mod_Mission_Base_open_box();
	common::log("������ս�������򿪽�������\n", iEdit);
	this->Mod_Mission_Base_pickup_award();
	common::log("������ս������ʰ����\n", iEdit);
}

//============================================================================
// R177 ��ȡmission_monster_team_id		
//180������֮��ĸ�������boss��ֻ�־�Ӣ��type=31	mission_id>=1182Ϊ����֮��ĸ���
//============================================================================
std::vector<int> sxd_client::get_monster_team_by_missionid(int mission_id)
{
	//���ص�monster_team�б�
	std::vector<int> monster_team;

	//���mission���õ�������Ϣ
	mss mission = db.get_mission(version, mission_id);

	//�������ͣ���Ӣ��boss����ͨ����
	int mission_type;

	//�ж��Ƿ�Ϊ180����֮��ĸ���
	if (mission_id >= 1182) 
	{
		//��Щ��������ֱ��ͨ��monster_id�鵽team_id
		int monster_id = stoi(mission["boss_id"]);

		//��monster_id����õ���Ӧ������team_id
		auto teams = db.get_mission_teamid_by_monsterid(version, monster_id);
		for (auto& team : teams)
		{
			int team_id = stoi(team["team_id"]);
			monster_team.push_back(team_id);
		}
		return monster_team;
	}
	else
	{
		if (stoi(mission["hero_mission"]) == 1)
		{
			//��Ӣ����
			mission_type = MissionType::HERO;
		}
		else
		{
			//���Ǿ�Ӣ����
			if (stoi(mission["boss_mission"]) == 0)
			{
				//Ҳ����boss����,��Ϊ��ͨ����
				mission_type = MissionType::NORMAL;
			}
			else if (stoi(mission["boss_mission"]) == 1)
			{
				//boss����
				mission_type = MissionType::BOSS;
			}
		}
	}

	//if (mission_type == MissionType::HERO|| mission_type == MissionType::BOSS)
	//{
	//	//��Щ��������ֱ��ͨ��monster_id�鵽team_id
	//	int monster_id = stoi(mission["boss_id"]);

	//	//��monster_id����õ���Ӧ������team_id
	//	auto teams = db.get_mission_teamid_by_monsterid(version, monster_id);
	//	for (auto& team : teams)
	//	{
	//		int team_id = stoi(team["team_id"]);
	//		monster_team.push_back(team_id);
	//	}
	//	return monster_team;
	//}
	//else if (mission_type == MissionType::NORMAL)
	//{
	//	return monster_team;
	//}
	return monster_team;
	/*Json::Value data_accept;
	data_accept = this->Mod_MissionPractic_Base_get_practice_info(mission_id);

	//�ж��Ƿ���boss����
	int is_boss = data_accept[2].asInt() == 0 ? false : true;

	//��ȡ�����б�
	Json::Value data = data_accept[1];

	//��һλ�ǹ���id,�ڶ�λ��������������
	std::vector<std::vector<int>> monsterid_count;

	//i��¼�м��ֹ���
	int monster_count=0;

	for (Json::Value iter : data)
	{	
		monsterid_count[monster_count].push_back(iter[0].asInt());
		monsterid_count[monster_count].push_back(iter[2].asInt());

		monster_count++;
	}*/
}


//============================================================================
// R171 ��ǰ����
// {module:3, action:6, request:[],
// response:[[Utils.IntUtil, Utils.ByteUtil]]};
// Example
// quest_id, state
//     [ [ [ 897, 1 ], [ 894, 1 ], [ 874, 1 ], [ 900, 1 ], [ 876, 1 ], [ 893, 1 ], [ 875, 1 ], [ 872, 2 ], [ 889, 1 ], [ 899, 1 ], [ 890, 1 ], [ 895, 1 ], [ 898, 1 ], [ 896, 1 ], [ 892, 1 ], [ 887, 1 ] ] ]
//		1��ʾ�ѽӣ�2��ʾ�����
//		QuestTypeData.as
//     887:[2,24200,0,"�޸�������","...","...","ȥ[�����ɳ�7)]����",193,193,5100000,1700000,0,0,"...","...","...",872,1],
//		1�����ߣ�2��֧������3����������4����Ӣ����
//		���������֣������б�ʾ�ȼ���֧���б�ʾδͨ�أ�
//		193��������npcid��193�������npcid��5100000���飬1700000ͭǮ��872����id
//		MissionTypeData.as
//     872:[64,53070,5,25200,190,126000,0,"�����ɳ�(7)",0,0,0],
//============================================================================
Json::Value sxd_client::Mod_Quest_Base_list_player_quest() {
    Json::Value data;
    return this->send_and_receive(data, 3, 6);
}

//============================================================================
// R171 �ɽ�����
// {module:3, action:8,
// request:[Utils.IntUtil],
// Example		_data.call(Mod_Quest_Base.can_receive_quest,this.canReceiveQuestCallback,[_ctrl.player.mapId]);
//     [ 33 ]
// response:[[Utils.IntUtil]]};
// Example
//     [ [ [ 877 ], [ 869 ], [ 873 ], [ 891 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_Quest_Base_can_receive_quest(int town_map_id) {
    Json::Value data;
    data.append(town_map_id);
    return this->send_and_receive(data, 3, 8);
}



//============================================================================
// R171 ������
// {module:3, action:,
// request:[Utils.IntUtil],
// Example		 _data.call(Mod_Quest_Base.accept_quest,func,[questId]);
//     [ 33 ]
// response:[[Utils.IntUtil]]};
// Example
//     [ [ [ -1 ], [ 0 ], [ 1 ], [ 2 ] ] ]	LevelLimit:int = -1;Acceptable:int = 0;Accepted:int = 1;Completed:int = 2;
//============================================================================
Json::Value sxd_client::Mod_Quest_Base_accept_quest(int quest_id) {
	Json::Value data;
	data.append(quest_id);
	return this->send_and_receive(data, 3, 3);
}

//============================================================================
// R171 �ύ����
// {module:3, action:5,
// request:[Utils.IntUtil],		questId
// Example
//     [ 33 ]
// response:[Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.ByteUtil,Utils.UByteUtil];
//			["state","experience","coins","awardItemId","townKey","questKey","newQuest"])
// Example
//     
//============================================================================
Json::Value sxd_client::Mod_Quest_Base_complete_quest(int quest_id) {
	Json::Value data;
	data.append(quest_id);
	return this->send_and_receive(data, 3, 5);
}

//============================================================================
// R171 ������ ����ò���
// {module:3, action:4,
// request:[Utils.IntUtil],
// Example
//     [ 33 ]
// response:[Utils.UByteUtil];
// Example
//     [ [ [ 877 ], [ 869 ], [ 873 ], [ 891 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_Quest_Base_finish_quest(int quest_id) {
	Json::Value data;
	data.append(quest_id);
	return this->send_and_receive(data, 3, 4);
}

//============================================================================
// R171 ����
// {module:4, action:0,
// request:[Utils.IntUtil],
// Example
//     [ 33 ]
// response:[[Utils.IntUtil, Utils.ByteUtil, Utils.UByteUtil, Utils.ByteUtil, Utils.ShortUtil, Utils.ByteUtil]]};
// ChooseMission2View.as 59:
//     oObject.list(tempArr, tempObj, ["mission_id", "status", "rank", "is_boss", "complete_count", "difficulty"]);
// Example
// status=1, ��ͨ��, ��ɨ��; status=0, δͨ��
//if (_loc2_.isBoss == 0){_loc2_.monsterLen = 3;}else{_loc2_.monsterLen = 1;}
//     [ [ [ 866, 1, 74, 0, 0, 0 ], [ 867, 1, 75, 0, 0, 0 ], [ 868, 1, 74, 1, 0, 0 ], [ 869, 1, 74, 0, 0, 0 ], [ 870, 1, 74, 0, 0, 0 ], [ 871, 1, 74, 1, 0, 0 ], [ 872, 1, 74, 0, 0, 0 ], [ 873, 1, 74, 0, 0, 0 ], [ 874, 1, 74, 1, 0, 0 ], [ 875, 1, 74, 0, 0, 0 ], [ 876, 1, 76, 0, 0, 0 ], [ 877, 1, 76, 1, 0, 0 ], [ 878, 1, 76, 0, 0, 0 ], [ 879, 1, 76, 0, 0, 0 ], [ 880, 1, 76, 1, 0, 0 ], [ 881, 1, 76, 0, 0, 0 ], [ 882, 1, 76, 0, 0, 0 ], [ 883, 1, 76, 1, 0, 0 ], [ 884, 1, 76, 0, 0, 0 ], [ 885, 1, 76, 0, 0, 0 ], [ 886, 1, 76, 1, 0, 0 ], [ 887, 1, 76, 0, 0, 0 ], [ 888, 1, 76, 0, 0, 0 ], [ 889, 1, 76, 1, 0, 0 ], [ 890, 0, 76, 0, 0, 0 ] ] ]
//============================================================================
Json::Value sxd_client::Mod_Mission_Base_get_sections(int town_map_id){
    Json::Value data;
    data.append(town_map_id);
    return this->send_and_receive(data, 4, 0);
}

//============================================================================
// R171 ���븱��
// {module:4, action:1,
// request:[Utils.IntUtil],
// Example
//     [ 33 ]
// response:[Utils.UByteUtil,Utils.IntUtil,Utils.IntUtil,Utils.ByteUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil],[Utils.IntUtil]];
// ChooseMission2View.as 59:
//     oObject.list(result, missionId,"",��isFirstChallenge, missionLock, monsterLock, [clickableItemList]);
// Example
// result=10, ���븱���ɹ�; result=29, 180�����Ӣ����ֻ����սһ��
//     
//============================================================================
Json::Value sxd_client::Mod_Mission_Base_enter_mission(int mission_id) {
	Json::Value data;
	data.append(mission_id);
	return this->send_and_receive(data, 4, 1);
}

//============================================================================
// R171 ���븱���󣬴��
// {module:4, action:2,
// request:[Utils.IntUtil,Utils.ByteUtil],
// Example
//     [ 33 ]
// response:�ܳ�
// MissionData.as 199:
//public function fight_monster(param1:Array) : void
//{
//	this._warStatus = param1[param1.length - 3];
//	this._warData = new WarData();
//	if (this.warStatus == Mod_Mission_Base.SUCCESS)
//	{
//		this._warData.format(param1);
//	}
//	this._worship = param1[param1.length - 2];
//	this.warAura = param1[param1.length - 1];
//============================================================================
Json::Value sxd_client::Mod_Mission_Base_fight_monster(int monsterTeamId) {
	Json::Value data;
	data.append(monsterTeamId);
	data.append(0);
	return this->send_and_receive(data, 4, 2);
}

//============================================================================
// R171 ��ȡɨ����Ϣ��Ŀ���ǻ�ȡ����������Ϣ
// {module:25, action:0,
// request:[Utils.IntUtil],
// Example		[missionId]
//     [ 630 ]
// response:[Utils.ShortUtil,[Utils.IntUtil,Utils.ShortUtil,Utils.ByteUtil],Utils.ByteUtil,Utils.IntUtil];
//	[Utils.ShortUtil, [Utils.IntUtil, Utils.ShortUtil, Utils.ByteUtil],Utils.ByteUtil, Utils.IntUtil]
// Example
//     	[hourPower,[monsterList],isBoss,attackPower]	isboss==0 elae 
//		[monsterList]=[monsterId, level, number]
//============================================================================
Json::Value sxd_client::Mod_MissionPractic_Base_get_practice_info(int mission_Id)
{
	Json::Value data;
	data.append(mission_Id);
	return this->send_and_receive(data, 25, 0);
}

//============================================================================
// �򿪽�������
// {module:4, action:4,
// request:[],
// response:[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil],Utils.ShortUtil];
//============================================================================
Json::Value sxd_client::Mod_Mission_Base_open_box()
{
	Json::Value data;
	return this->send_and_receive(data, 4, 4);
}


//============================================================================
// ����������
// {module:4, action:4,
// request:[],
// response:[Utils.UByteUtil];
//============================================================================
Json::Value sxd_client::Mod_Mission_Base_pickup_award()
{
	Json::Value data;
	return this->send_and_receive(data, 4, 5);
}