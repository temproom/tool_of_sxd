========================================================================
    数据库
========================================================================

/////////////////////////////////////////////////////////////////////////////

查找数据库，返回得到的信息
typedef std::unordered_map<std::string, std::string> mss
单个项目用	mss
多个项目	std::list<mss>

int sqlite3_exec(
        sqlite3*,                                  /* An open database */
        const char *sql,                           /* SQL to be evaluated */
        int (*callback)(void*,int,char**,char**),  /* Callback function */
        void *,                                    /* 1st argument to callback */
        char **errmsg                              /* Error msg written here */
);
 sqlite3*：打开的数据库			   
 const char *sql：要执行的sql语句
 int (*callback)(void*,int,char**,char**)：执行sql语句时对应的回调函数
 void *：回调函数的参数				
 char **errmsg：存放错误信息
 int callback(void *arg, int column_size, char *column_value[], char *column_name[])	//回调函数是用户自定义的的
 void *arg：是sqlite3_exec函数的第四个参数		column_size：数据库的字段数
 column_value[]：列的值 						column_name：字段名字
 执行成功返回SQLITE_OK （0），否则返回其他值。


1）Vector
连续存储的容器，动态数组，在堆上分配空间
底层实现：数组
两倍容量增长：
vector 增加（插入）新元素时，如果未超过当时的容量，则还有剩余空间，那么直接添加到最后（插入指定位置），然后调整迭代器。
如果没有剩余空间了，则会重新配置原有元素个数的两倍空间，然后将原空间元素通过复制的方式初始化新空间，再向新空间增加元素，最后析构并释放原空间，之前的迭代器会失效。
性能：
访问：O(1)
插入：在最后插入（空间够）：很快
在最后插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
在中间插入（空间够）：内存拷贝
在中间插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
删除：在最后删除：很快
在中间删除：内存拷贝
适用场景：经常随机访问，且不经常对非尾节点进行插入删除。
2) List
动态链表，在堆上分配空间，每插入一个元数都会分配空间，每删除一个元素都会释放空间。
底层：双向链表
性能：
访问：随机访问性能很差，只能快速访问头尾节点。
插入：很快，一般是常数开销
删除：很快，一般是常数开销
适用场景：经常插入删除大量数据
2、区别：
1）vector底层实现是数组；list是双向 链表。
2）vector支持随机访问，list不支持。
3）vector是顺序内存，list不是。
4）vector在中间节点进行插入删除会导致内存拷贝，list不会。
5）vector一次性分配好内存，不够时才进行2倍扩容；list每次插入新节点都会进行内存申请。
6）vector随机访问性能好，插入删除性能差；list随机访问性能差，插入删除性能好。
3、应用
vector拥有一段连续的内存空间，因此支持随机访问，如果需要高效的随即访问，而不在乎插入和删除的效率，使用vector。
list拥有一段不连续的内存空间，如果需要高效的插入和删除，而不关心随机访问，则应使用list。