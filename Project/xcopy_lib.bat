echo off
echo start copy .... %time%

xcopy ..\Project\Objects\*.lib ..\lib\ /y

echo xcopy backup success! (by FBI/sina.com.cn) ...%time%
