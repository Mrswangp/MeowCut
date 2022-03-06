::设置环境变量

::关闭终端回显
@echo off

::设置变量
set Qt5_HOME=C:\Qt\Qt5.14.2\5.14.2
set Qt_MSVC_Path=%Qt5_HOME%\msvc2017_64

::设置环境变量
SETX Qt_Path "%Qt_MSVC_Path%\lib\cmake\Qt5" /m
SETX PATH "%PATH%;%Qt_MSVC_Path%\bin;%Qt_MSVC_Path%\lib;" /m

echo "Enviroment Setup successful !"
pause
exit
