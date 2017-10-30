/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI所需的系统函数
*	文件名称 : App_SysFunction.c
*	版    本 : V1.1
*	说    明 : 图片显示和保存的API
*	修改记录 :
*		版本号    日期          作者         说明
*		V1.0    2013-03-26    Eric2013       首发
*       V1.1    2014-05-23    Eric2013       删除此文件中多余的函数，只保留截图和BMP图片加载
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"

/*
*********************************************************************************************************
*	                                         用于SD卡
*********************************************************************************************************
*/
FRESULT result;
FIL file;
FIL FileSave;   //专门用于图片的保存
FILINFO finfo;
DIR DirInf;
UINT bw;
FATFS fs;
FATFS fs_nand;
FATFS fs_usb;

char *_acBuffer2;
GUI_HMEM hMem;

GUI_GIF_INFO InfoGif1;
GUI_GIF_IMAGE_INFO InfoGif2;
GUI_JPEG_INFO JpegInfo;

/*
*********************************************************************************************************
*	函 数 名: _ShowBMP()
*	功能说明: 加载SD中的图片数据到外部SRAM中
*	形    参：sFilename  文件名        	
*	返 回 值: 1：表示成功 0：表示失败
*********************************************************************************************************
*/
uint8_t  _ShowBMP(const char * sFilename) 
{
	OS_ERR      	err;

	/* 读取BMP图片数据到外部SRAM中 */	
	OSSchedLock(&err);
	
	result = f_open(&file, sFilename, FA_READ);
	if (result != FR_OK)
	{
		OSSchedUnlock(&err);
		return 0;
	}
	result = f_read(&file, _acBuffer2, file.fsize, &bw);
	if (result != FR_OK)
  	{
		OSSchedUnlock(&err);
		return 0;
  	}
    f_close(&file);
	
	OSSchedUnlock(&err);

	return 1;
	
}

/*
*********************************************************************************************************
*	函 数 名: _WriteByte2File()
*	功能说明: 写文件到SD卡或者其他存储介质
*	形    参：Data 要写的数据， p 指向FIL类型变量      	
*	返 回 值: 无
*********************************************************************************************************
*/
void _WriteByte2File(U8 Data, void * p) 
{
	result = f_write (p, &Data, 1, &bw);
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
