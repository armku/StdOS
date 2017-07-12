echo off

del ../User/buidDate.h

echo #pragma once >> ../User/buidDate.h
echo BUILDYEAR %date:~0,4% >> ../User/buidDate.h
echo BUILDMON %date:~5,2% >> ../User/buidDate.h
echo BUILDDAY %date:~8,2% >> ../User/buidDate.h
