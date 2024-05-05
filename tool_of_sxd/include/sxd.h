#ifndef SXD_H_
#define SXD_H_

#include "sxd_client.h"

extern std::string player_name;

class sxd
{
public:
	sxd();
	virtual ~sxd();
	
	static void log_write(const std::string& player_name, int close_log=0, int max_queue_size = 100);
	static void sql_pool();
	//static void thread_pool();

	static void run(std::string arg, bool auto_exit);
	static void login();
	static void analyze();
	static void collect();	

	//日志
	/*int m_close_log;
	int m_log_write;
	std::string player_name;*/
	//connection_pool *m_sql_pool;

	//数据库
	//int m_sql_num;

	//线程池
	//static threadpool<sxd_client> *m_thread_pool;

private:
	static void auto_play(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie);

	//猎命
	static void batch_fate(const std::string& version, const std::string& user_id, const std::string& url, const std::string& cookie);

	static void collect_protocol(const std::string& version, const std::string& path);
	static void collect_end_function_gift(const std::string& version, const std::string& path);
	static void collect_function(const std::string& version, const std::string& path);
	static void collect_gift(const std::string& version, const std::string& path);
	static void collect_item(const std::string& version, const std::string& path);
	static void collect_lucky_shop_item(const std::string& version, const std::string& path);
	static void collect_role(const std::string& version, const std::string& path);
	static void collect_town(const std::string& version, const std::string& path);
	static void collect_facture_reel(const std::string& version, const std::string& path);
	static void collect_mission_monster(const std::string& version, const std::string& path);
	static void collect_mission_team(const std::string& version, const std::string& path);
	static void collect_mission(const std::string& version, const std::string& path);
	static void collect_quest(const std::string& version, const std::string& path);
	static void collect_tongtiantower(const std::string& version, const std::string& path);

	static void collect_mission_to_monster(const std::string& version, const std::string& path);	
};
#endif /* SXD_H_ */