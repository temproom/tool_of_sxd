========================================================================
    控制台应用程序：tool_of_sxd 项目概述
========================================================================

应用程序向导已为您创建了此 tool_of_sxd 应用程序。

本文件概要介绍组成 tool_of_sxd 应用程序的每个文件的内容。


tool_of_sxd.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

tool_of_sxd.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

tool_of_sxd.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
其他标准文件:

StdAfx.h, StdAfx.cpp
    这些文件用于生成名为 tool_of_sxd.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释:

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////

jsoncpp
运行源码根目录下有个Python文件叫amalgamate.py，生成dist文件夹，复制到项目目录
复制源码的include下json到项目include目录
VC++目录 包含目录 添加项目include文件和项目自身文件

Boost库
1、双击bootstrap.bat文件，生成b2.exe；
2、在cmd中输入以下命令（未尝试）或者双击b2.exe运行。bjam --toolset=msvc --build-type=complete stage ，回车
这里面--build-type=complete时link，runtime-link3种组合下debug， release的多线程版本都生成出来了除此之外还生成了link=static，runtime-link=static的debug， release的单线程版本。
得到stage下的dll和lib文件

1、配置属性->VC++目录：
（1）、"包含目录": boost的根目录，例:D:\my_workspace\C_program\C_boost\boost_1_79_0
（2）、"库目录": stage下的链接库目录，例:D:\my_workspace\C_program\C_boost\boost_1_79_0\stage\lib
2、配置属性->链接器->常规:"附加库目录":同上面的"库目录"，例:D:\my_workspace\C_program\C_boost\boost_1_79_0\stage\lib