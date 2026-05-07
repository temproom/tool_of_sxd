#include "stdafx.h"
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include "common.h"
#include "sxd_client.h"

class Mod_ExpeditionTask_Base
{
public:
	static const int SUCCESS = 0;
	static const int ALREADY_START = 1;
	static const int OVER_GET = 2;
	static const int FAILED = 3;
};

void sxd_client::ExpeditionTask()
{
	Json::Value data = this->Mod_ExpeditionTask_Base_get_info();
	vector<int> list = { 1,2,3,4,6,9,10,8 };
	for (int id : list)
	{
		Json::Value data_task = this->Mod_ExpeditionTask_Base_start_task(id);
		int result = data_task[0].asInt();
		if (result == Mod_ExpeditionTask_Base::SUCCESS)
		{
			common::log("【远征任务】领取任务成功！");
		}
		else if (result == Mod_ExpeditionTask_Base::ALREADY_START)
		{
			common::log("【远征任务】任务已开始！",0);
		}
		else if (result == Mod_ExpeditionTask_Base::OVER_GET)
		{
			common::log("【远征任务】奖励已领取！", 0);
		}
	}
}

//============================================================================
//  远征任务 面板信息
// {module:430, action:0, 
// request:[], 
// response:[Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]],Utils.IntUtil,[Utils.IntUtil,[Utils.IntUtil,Utils.IntUtil]]]
// 
// ExpeditionTaskData.as 40:
//     oObject.list(param1,this.taskObject,["fighting_add","tu_mo_ling","task_list","player_complete","tol_task_award"]);
//				task_list：		oObject.list(_loc3_,_loc2_,["task_id","is_get","player_count","need_count","award_list"]);
//						award_list：	oObject.list(_loc5_,_loc4_,["item_id","item_count"]);
// Example
//     [ 0, 0, [ [ 10, 1, 0, 10, [ [ 4099, 30 ], [ 6192, 10 ], [ 2263, 20 ] ] ], [ 3, 1, 0, 15, [ [ 2407, 4 ], [ 3964, 2 ], [ 6192, 10 ] ] ], [ 8, 1, 0, 50, [ [ 1740, 30 ], [ 6192, 10 ], [ 347, 100 ] ] ], [ 7, 0, 0, 10, [ [ 3783, 20 ], [ 6192, 10 ], [ 2343, 20 ] ] ], [ 5, 0, 0, 7, [ [ 4641, 1 ], [ 4635, 3 ], [ 6192, 10 ] ] ] ], 0, [ [ 5, [ [ 4431, 1 ] ] ], [ 3, [ [ 2397, 30 ] ] ], [ 4, [ [ 4611, 1 ] ] ], [ 1, [ [ 2343, 50 ] ] ], [ 2, [ [ 1740, 50 ] ] ] ] ] 
//	   [ 25, 1760, [ [ 1, 0, 0, 3000, [ [ 3706, 80 ], [ 6192, 10 ], [ 2016, 5 ] ] ], [ 7, 0, 0, 10, [ [ 3783, 20 ], [ 6192, 10 ], [ 2343, 20 ] ] ], [ 2, 0, 0, 100, [ [ 4519, 80 ], [ 6192, 10 ], [ 5323, 5 ] ] ], [ 4, 0, 0, 3, [ [ 3784, 15 ], [ 4237, 3 ], [ 6192, 10 ] ] ], [ 9, 0, 0, 500000000, [ [ 2452, 50 ], [ 6192, 10 ], [ 5822, 5 ] ] ] ], 733, [ [ 5, [ [ 4431, 1 ] ] ], [ 3, [ [ 2397, 30 ] ] ], [ 4, [ [ 4611, 1 ] ] ], [ 1, [ [ 2343, 50 ] ] ], [ 2, [ [ 1740, 50 ] ] ] ] ] 
//============================================================================
Json::Value sxd_client::Mod_ExpeditionTask_Base_get_info()
{
	Json::Value data;
	return this->send_and_receive(data, 430, 0);
}

//============================================================================
//  远征任务	领取任务
// {module:430, action:1, 
// request:[Utils.IntUtil],
// 
// ExpeditionTaskView.as
//			_data.call(Mod_ExpeditionTask_Base.start_task,start_task_callback,[taskId]);
//		1,2,3,4,6,9,10,8
/*       1:"获得3000点活跃度",
         2:"通过坐骑试炼累计获得100个驯养令",
         3:"成功完成15次宠物任务",
         4:"在全网聊天频道连续发言3天",
         5:"通过经验丹果树收获7颗经验丹",
         6:"累计摘仙桃35颗",
         7:"累计使用10个全网伏魔塔宝箱",
         8:"在仙界矿山累计挖矿50小时",
         9:"累计消费5亿铜钱",
         10:"在仙界竞技场中累计获胜10场（功能终结后将于次日0点增加任务进度）"
*/
// response:[Utils.UByteUtil]
// 
// ExpeditionTaskData.as 40:
//     this.result = param1[0];
// Example
//     []
//============================================================================
Json::Value sxd_client::Mod_ExpeditionTask_Base_start_task(int id)
{
	Json::Value data;
	data.append(id);
	return this->send_and_receive(data, 430, 1);
}