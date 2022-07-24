========================================================================
    控制台应用程序：tool_of_sxd 项目概述
========================================================================

/////////////////////////////////////////////////////////////////////////////

jsoncpp
https://github.com/open-source-parsers/jsoncpp/releases
运行源码根目录下有个Python文件叫amalgamate.py，生成dist文件夹，复制到项目目录
复制源码的include下json到项目include目录
VC++目录 包含目录 添加项目include文件	D:\json\jsoncpp-1.9.5\jsoncpp-1.9.5\include
和项目自身文件	D:\**\tool_of_sxd\tool_of_sxd\include  D:\**\tool_of_sxd\tool_of_sxd

Boost库
https://blog.csdn.net/nanke_yh/article/details/124346308
1、双击bootstrap.bat文件，生成b2.exe；
2、在cmd中输入以下命令（未尝试）或者双击b2.exe运行。bjam --toolset=msvc --build-type=complete stage ，回车
这里面--build-type=complete时link，runtime-link3种组合下debug， release的多线程版本都生成出来了除此之外还生成了link=static，runtime-link=static的debug， release的单线程版本。
得到stage下的dll和lib文件
1、配置属性->VC++目录：
（1）、"包含目录": boost的根目录，例:D:\boost_1_78_0\boost_1_78_0
（2）、"包含目录":D:\boost_1_78_0\boost_1_78_0\boost
（3）、"库目录": stage下的链接库目录，例:D:\my_workspace\C_program\C_boost\boost_1_79_0\stage\lib
2、配置属性->链接器->常规:"附加库目录":同上面的"库目录"，例:D:\boost_1_78_0\boost_1_78_0\stage\lib

libiconv库
https://blog.csdn.net/ZX_Infinite/article/details/119283069
iconv.h
extern size_t iconv (iconv_t cd, /* ICONV_CONST */const char* * inbuf, size_t *inbytesleft, char* * outbuf, size_t *outbytesleft);
iconv.c
size_t iconv (iconv_t icd,const char* * inbuf, size_t *inbytesleft,char* * outbuf, size_t *outbytesleft)

localtime_s	线程安全
localtime	线程不安全

C++字符串转换（stoi；stol；stoul；stoll；stoull；stof；stod；stold）
函数参数默认值 编译时确定
默认值要在右边，没有默认值的在左边
可以在声明中设定默认值，也可以在定义时设定默认值(没有声明)，但不可以同时存在，二者选其一
函数调用时，参数数量必须大于等于没有默认值的参数的数量，从左到右赋值
//声明
int out(int a,int b=2,int c=3,int d=4)	
//定义
int out(int a,int b,int c,int d)		
{
	cout<<a<<b<<c<<d;
}
//调用
out(1);			1234
out(1,1);		1134
out(1,1,1);		1114