[toc]
# MeowCut

## Prerequsit
请先下载并安装如下软件:
- **CMake**
  https://github.com/Kitware/CMake/releases/download/v3.23.0-rc2/cmake-3.23.0-rc2-windows-x86_64.msi

- **Qt5**
  https://download.qt.io/archive/qt/5.14/5.14.2/qt-opensource-windows-x86-5.14.2.exe

## Build
### Alternative 1: Use `Visual Studio`
1. 右键'以管理员身份'运行脚本`Setup_Env.bat`配置项目相关的环境变量（必须设置`Qt5_HOME`）
2. `Visual Studio`打开后，`File`->`Open`->`CMake(M)...`，选择项目根目录下`CMakeLists.txt`文件
3. 在`Visual Studio`的项目工程目录`MeowCut`中右键选中`CMakeLists.txt`->`设置为启动项`->点击'开始执行(Ctrl+F5)'

### Alternative 2: Use `cmake + make`
编译方式（在根目录下）为：
1. `mkdir build && cd build`
2. 根据需要使用下面的`cmake`参数
```
cmake
    -D Qt_Path=<qt5 sdk路径，需要包含Qt5Config.cmake文件，如果环境变量设置了QT_DIR或Qt5_DIR，此项可以省略>
    -D CMAKE_BUILD_TYPE=<debug或release, 默认debug，这两个选项添加了不同的编译选项>
    -D SOFTWARE_TYPE=<app、lib、demo, 默认app>
    -D SOFTWARE_VERSION=<版本号，默认1.0>
    -D CXX_STANDARD=<11,14,17，C++标准，默认17，此选项没有做检查>
    -D INSTALL_PREFIX=<目标文件路径，默认build目录下的bin目录>
    -D DEMO_TYPE=<audio_effect、avcodec、avsync、image_filter、meowcut_core、video_render中的一个，demo模式下生效>
    ..
```
3. `make -j4`
4. 编译后生成的文件位于`INSTALL_PREFIX/bin/系统名`文件夹中
