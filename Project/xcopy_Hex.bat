echo off
echo start copy .... %time%

xcopy ..\Project\Objects\*.hex ..\hex\ /y

echo xcopy backup success! (by FBI/sina.com.cn) ...%time%
