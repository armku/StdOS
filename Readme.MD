暂定名：标准系统(StdOS)

基于STM32官方固件库的中间层软件，提供常用器件的驱动程序，提供前后台系统调用

部分兼容Arduino

Drivers中每个驱动下面有使用说明

固件固件库版本
F070:1.5
F103:3.5
F407:1.5

####C++实现

##### 兼容Arduino接口

###### void delayMicroseconds(uint32_t us);//us延时，100us以下精确

###### void delay(uint64_t ms);//延时毫秒

###### uint64_t millis();//系统启动后的毫秒数

###### uint64_t micros();//开机到现在的微妙数

## 

|                          项目                           | 年份 |  状态  | 语言环境 |                           说明                           |
| :-----------------------------------------------------: | :--: | :----: | :------: | :------------------------------------------------------: |
|        [MDK2VC](https://github.com/armku/MDK2VC)        | 2018 | 维护中 |    C#    |                   MDK工程转VS2017工程                    |
|         [StdOS](https://github.com/armku/StdOS)         | 2016 | 维护中 |   c++    |                    stm32快速开发系统                     |
| [MDKTimeHelper](https://github.com/armku/MDKTimeHelper) | 2018 | 维护中 |    c#    | mdk工程助手，时间字符串(yyyy-MM-dd HH:mm:ss)转为当前时间 |
|    [HelperComm](https://github.com/armku/HelperComm)    | 2017 | 维护中 |    c#    |                       串口调试助手                       |

