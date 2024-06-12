# cmake_template
这是一个我总结的C++工程模板，适合各种不含UI的C++项目，根目录下包含以下几个目录：

- 3rdparty:放没有提供`find_xxx.cmake`的第三方库的`.lib`和`.dll`文件
- cmake:放`find_xxx.cmake`文件，将本地的库文件引入项目
- scripts:放一些脚本
- src:项目的源代码，该目录不是直接放置所有源代码，源代码需要分模块进行放置
- test:单元测试的源代码，也是按模块进行放置
