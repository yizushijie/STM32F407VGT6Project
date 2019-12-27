#include "rfask_task.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
//void RFASKTask_Init(RFASK_HandlerType *rfask)
//{
//	RFASKLib_Init(rfask);
//	RFASKLib_SetFreqCurrentPointCmd(&rfask->msgFreqCurrentPointOne, CMD_RFASK_CMD1_FREQ_CURRENT_POINT_ONE);
//	RFASKLib_SetFreqCurrentPointCmd(&rfask->msgFreqCurrentPointTwo, CMD_RFASK_CMD1_FREQ_CURRENT_POINT_TWO);
//}

void RFASKTask_Init(RFASK_HandlerType* rfask, AT24CXX_HandlerType* AT24CXXx)
{
	//AT24CXXTask_I2C_EraseChip(AT24CXXx);
	//---基本参数的初始化
	RFASKLib_Init(rfask);
	//---eeprom中保存的参数
	if (AT24CXXx!=NULL)
	{
		RFASK_EEPROMInit(rfask, AT24CXXx);
	}
	//---传递命令
	RFASKLib_SetFreqCurrentPointCmd(&rfask->msgFreqCurrentPointOne, CMD_RFASK_CMD1_FREQ_CURRENT_POINT_ONE);
	RFASKLib_SetFreqCurrentPointCmd(&rfask->msgFreqCurrentPointTwo, CMD_RFASK_CMD1_FREQ_CURRENT_POINT_TWO);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T RFASKTask_CalcXTAL(RFASK_HandlerType *rfask, UINT32_T rfX100MHz)
{
	return RFASKLib_CalcXTAL(rfask, rfX100MHz);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_SetDeviceType(RFASK_HandlerType *rfask, UINT8_T deviceType)
{
	return RFASKLib_SetDeviceType(rfask, deviceType);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T RFASKTask_GetDeviceType(RFASK_HandlerType *rfask)
{
	return RFASKLib_GetDeviceType(rfask);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T RFASKTask_SetClockFreq(RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, UINT32_T rfX100MHz)
{
	UINT8_T _return = OK_0;
	//---获取设定的晶振值
	UINT32_T freqTemp = RFASKLib_CalcXTAL(rfask, rfX100MHz);
	////---设置时钟芯片WM8510
	//return WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, freqTemp);
	//---20190504,增加设备的时钟输出的自校准功能
	_return= WM8510Task_I2C_SetFreqHzWithAllFreqRegAndCalibrateFreqKHzOutPut(WM8510x, freqTemp);
	if (_return!=OK_0)
	{
		//---复位WM8510
		WM8510Task_I2C_Reset(WM8510x);
		//---再次设置输出时钟并校准，如果时钟输出异常，则进行MCU的软件复位
		_return= WM8510Task_I2C_SetFreqHzWithAllFreqRegAndCalibrateFreqKHzOutPut(WM8510x, freqTemp);
		if (_return!=OK_0)
		{
			//---软件复位，软件复位已经验证过可以成功
			SOFT_RESET();
			////---硬件复位,等待看门狗启动
			//while (1);
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： WM8510的频率
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
//UINT8_T  RFASKTask_WM8510Task(USART_HandlerType* USARTx, WM8510_HandlerType* WM8510x)
UINT8_T  RFASKTask_WM8510Task(USART_HandlerType*USARTx, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;
	UINT32_T freqTemp = 0;

	//---数据发送报头
	USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);

	//---执行任务命令
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
	{
		//---设置WM8510的输出频率
		case CMD_RFASK_CMD2_SET_WM8510:
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//---计算WM8510的输出频率
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---设置输出频率
			_return = WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, freqTemp);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---读取WM8510设置的输出频率
		case CMD_RFASK_CMD2_GET_WM8510:
			//---外部计数模式进行脉冲的计数
			TimerTask_CalcFreq_Task(1);
			//---获取当前输出的频率KHz,增加获取脉冲的时间为10ms，牺牲时间换取精度
			freqTemp = (UINT32_T)TimerTask_GetFreqKHz(); // (UINT32_T)(TimerTask_GetFreqKHz()/10);
			//---获取当前输出的频率Hz
			freqTemp *= 1000;
			USARTTask_RealTime_AddSize(USARTx, 7);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---将数据填充到缓存区
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 0));
			break;
			//---复位WM8510的输出
		case CMD_RFASK_CMD2_RESET_WM8510:
			WM8510Task_I2C_Reset(pWm8510Device0);
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---时钟输出通道1的控制
		case CMD_RFASK_CMD2_CHANNELA_WM8510:
			//---A通道频率输出
			if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] != 0x00)
			{
				CLKA_FREQ_ON;
			}
			else
			{
				CLKA_FREQ_OFF;
			}
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---时钟输出通道2的控制
		case CMD_RFASK_CMD2_CHANNELB_WM8510:
			//---B通道频率输出
			if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] != 0x00)
			{
				CLKB_FREQ_ON;
			}
			else
			{
				CLKB_FREQ_OFF;
			}
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---时钟输出通道3的控制
		case CMD_RFASK_CMD2_CHANNELC_WM8510:
			//---C通道频率输出
			if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] != 0x00)
			{
				CLKC_FREQ_ON;
			}
			else
			{
				CLKC_FREQ_OFF;
			}
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---时钟输出通道4的控制
		case CMD_RFASK_CMD2_CHANNELD_WM8510:
			//---D通道频率输出
			if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] != 0x00)
			{
				CLKD_FREQ_ON;
			}
			else
			{
				CLKD_FREQ_OFF;
			}
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---时钟输出全部通道的控制
		case CMD_RFASK_CMD2_CHANNELS_WM8510:
			//---所有通道频率输出
			if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] != 0x00)
			{
				CLKA_FREQ_ON;
				CLKB_FREQ_ON;
				CLKC_FREQ_ON;
				CLKD_FREQ_ON;
			}
			else
			{
				CLKA_FREQ_OFF;
				CLKB_FREQ_OFF;
				CLKC_FREQ_OFF;
				CLKD_FREQ_OFF;
			}
			USARTTask_RealTime_AddSize(USARTx, 3);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
		default:
			_return = ERROR_1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：预设频率的参数设置
//////////////////////////////////////////////////////////////////////////////
//UINT8_T RFASKTask_YSELTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x)
UINT8_T RFASKTask_YSELTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;
	UINT32_T freqTemp = 0;
	//---数据发送报头
	USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
	//---执行任务命令
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
	{
		//---设置频率点1
		case CMD_RFASK_CMD2_YSEL1_FREQ_SET:
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel1 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL1_ADDR_X1, rfask->msgFreqX100MHzYSel1, 4);
			break;
			//---设置频率点2
		case CMD_RFASK_CMD2_YSEL2_FREQ_SET:
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel2 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL2_ADDR_X1, rfask->msgFreqX100MHzYSel2, 4);
			break;
			//---设置频率点3
		case CMD_RFASK_CMD2_YSEL3_FREQ_SET:
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel3 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL3_ADDR_X1, rfask->msgFreqX100MHzYSel3, 4);
			break;
			//---设置频率点4
		case CMD_RFASK_CMD2_YSEL4_FREQ_SET:
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel4 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL4_ADDR_X1, rfask->msgFreqX100MHzYSel4, 4);
			break;
			//---读取频率点1
		case CMD_RFASK_CMD2_YSEL1_FREQ_GET:
			freqTemp = rfask->msgFreqX100MHzYSel1;
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 4));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回第一个默认频率1
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			break;
			//---读取频率点2
		case CMD_RFASK_CMD2_YSEL2_FREQ_GET:
			freqTemp = rfask->msgFreqX100MHzYSel2;
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 4));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回第一个默认频率1
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			break;
			//---读取频率点3
		case CMD_RFASK_CMD2_YSEL3_FREQ_GET:
			freqTemp = rfask->msgFreqX100MHzYSel3;
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 4));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回第一个默认频率1
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			break;
			//---读取频率点4
		case CMD_RFASK_CMD2_YSEL4_FREQ_GET:
			freqTemp = rfask->msgFreqX100MHzYSel4;
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 4));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回第一个默认频率1
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			break;
			//---设置全部频率点
		case CMD_RFASK_CMD2_YSEL_FREQ_SET:
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			
			//---设置频率点1
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel1 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL1_ADDR_X1, rfask->msgFreqX100MHzYSel1, 4);

			//---设置频率点2
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 5];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 6];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 7];
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel2 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL2_ADDR_X1, rfask->msgFreqX100MHzYSel2, 4);

			//---设置频率点3
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 8];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 9];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 10];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 11];
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel3 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL3_ADDR_X1, rfask->msgFreqX100MHzYSel3, 4);

			//---设置频率点4
			freqTemp = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 12];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 13];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 14];
			freqTemp = (freqTemp << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 15];
			//---保存当前设定的值
			rfask->msgFreqX100MHzYSel4 = freqTemp;
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_FREQ_YSEL4_ADDR_X1, rfask->msgFreqX100MHzYSel4, 4);
			
			break;
			//---读取全部频率点
		case CMD_RFASK_CMD2_YSEL_FREQ_GET:
			freqTemp = rfask->msgFreqX100MHzYSel1;
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 16));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回第一个默认频率1
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			//---返回第一个默认频率2
			freqTemp = rfask->msgFreqX100MHzYSel2;
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			//---返回第一个默认频率3
			freqTemp = rfask->msgFreqX100MHzYSel3;
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			//---返回第一个默认频率2
			freqTemp = rfask->msgFreqX100MHzYSel4;
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 24));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 16));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(freqTemp));
			break;
			//---设置输出第一个预设频率点
		case CMD_RFASK_CMD2_YSEL1_FREQ_OUT:
			//---设置输出频率
			_return = WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, RFASKTask_CalcXTAL(rfask, rfask->msgFreqX100MHzYSel1));
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---设置输出第二个预设频率点
		case CMD_RFASK_CMD2_YSEL2_FREQ_OUT:
			//---设置输出频率
			_return = WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, RFASKTask_CalcXTAL(rfask, rfask->msgFreqX100MHzYSel2));
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---设置输出第三个预设频率点
		case CMD_RFASK_CMD2_YSEL3_FREQ_OUT:
			//---设置输出频率
			_return = WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, RFASKTask_CalcXTAL(rfask, rfask->msgFreqX100MHzYSel3));
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
			//---设置输出第四个预设频率点
		case CMD_RFASK_CMD2_YSEL4_FREQ_OUT:
			//---设置输出频率
			_return = WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, RFASKTask_CalcXTAL(rfask, rfask->msgFreqX100MHzYSel4));
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, _return);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			break;
		default:
			_return = ERROR_1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 获取频率电流扫描激活的SITE数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_ActivateSites(RFASK_HandlerType *rfask, UINT8_T activateSites)
{
	return RFASKLib_ActivateSites(rfask, activateSites);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_SitesCurrent(RFASK_HandlerType *rfask)
{
	UINT8_T i = 0;
	UINT64_T sitePower = 0;
	//---获取ADC的采样结果
	ADCTask_RFASKTask_GetADC(&rfask->msgSiteADC[0], FREQ_CURRENT_MAX_SITE);
	//---装换结果为电流值
	for (i = 0; i < FREQ_CURRENT_MAX_SITE; i++)
	{
		//---采样结果
		sitePower = rfask->msgSiteADC[i];

		//---计算过程
		sitePower *= FREQ_CURRENT_ADC_VREF_MV;
		sitePower *= 10000;
		sitePower /= rfask->msgSampleX100Res;
		sitePower /= rfask->msgAmpTimes;

		//---将结果转换成电流值
		rfask->msgSiteCurrent[i] = (UINT16_T)((sitePower >> 12) + 1);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_SetFreqCurrentPointCmd(FREQ_CURRENT_HandlerType *rfaskFreqCurrent, UINT8_T cmd)
{
	return RFASKLib_SetFreqCurrentPointCmd(rfaskFreqCurrent, cmd);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 频率电流扫描
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_FreqCurrentTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, UINT8_T activateSites)
{
	UINT8_T _return = OK_0;
	//---解析频率电流扫描的激活的SITE
	RFASKTask_ActivateSites(rfask, activateSites);
	//---进行频率电流的流程处理
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 频率电流扫描
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
//UINT8_T  RFASKTask_FreqCurrentPointOneTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x)
UINT8_T  RFASKTask_FreqCurrentPointOneTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;

	//---执行任务命令
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
	{
		//---获取频率电流扫描的频率参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 11);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz >> 24));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz >> 16));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz >> 8));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz));
			//---步进频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz >> 8));
			//---步进频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz));
			//---采集点的个数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgFreqPointNum >> 8));
			//---采集点的个数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgFreqPointNum));
			break;
			//---设置频率电流扫描的频率参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---设置起始频率
			rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---设置起始频率
			rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---设置起始频率
			rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			//---设置起始频率
			rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_START_FREQ_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz, 4);

			//---设置步进频率
			rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 5];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_STEP_FREQ_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz, 2);

			//---设置采集点的个数
			rfask->msgFreqCurrentPointOne.msgFreqPointNum = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 6];
			rfask->msgFreqCurrentPointOne.msgFreqPointNum = (rfask->msgFreqCurrentPointOne.msgFreqPointNum << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 7];
			//---限制采样点的大小，这个值会影响上报上位机的时候数据缓存区的大小
			if (rfask->msgFreqCurrentPointOne.msgFreqPointNum > FREQ_CURRENT_FREQ_POINT_MAX_NUM)
			{
				rfask->msgFreqCurrentPointOne.msgFreqPointNum = FREQ_CURRENT_FREQ_POINT_MAX_NUM;
			}
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_POINT_NUM_ADDR_X1, rfask->msgFreqCurrentPointOne.msgFreqPointNum, 2);
			
			//---计算停止频率
			rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz = rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz;
			rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz *= rfask->msgFreqCurrentPointOne.msgFreqPointNum;
			rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz += rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz;
			break;
			//---获取频率电流扫描的电流参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 2 + 2 + 2 + 2 + 2 + 2 + 2));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---获取起始的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA >> 8));
			//---获取起始的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA));
			//---获取起始的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA >> 8));
			//---获取起始的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA));
			//---ADC间隔的点数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPointNum >> 8));
			//---ADC间隔的点数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPointNum));
			//---ADC合格的最大值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPassMax >> 8));
			//---ADC合格的最大值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPassMax));
			//---ADC合格的最小值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPassMin >> 8));
			//---ADC合格的最小值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgADCPassMin));
			//---获取截止的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA >> 8));
			//---获取截止的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA));
			//---获取截止的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA >> 8));
			//---获取截止的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA));
			break;
			//---设置频率电流扫描的电流参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---设置起始的最大电流
			rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---设置起始的最大电流
			rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA, 2);

			//---设置起始的最小电流
			rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			//---设置起始的最小电流
			rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_START_MIN_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA, 2);

			//---ADC间隔的点数
			rfask->msgFreqCurrentPointOne.msgADCPointNum = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			//---ADC间隔的点数
			rfask->msgFreqCurrentPointOne.msgADCPointNum = (rfask->msgFreqCurrentPointOne.msgADCPointNum << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 5];
			//---判断数据是够超界
			if (rfask->msgFreqCurrentPointOne.msgADCPointNum > FREQ_CURRENT_HISTORY_MAX_SIZE)
			{
				rfask->msgFreqCurrentPointOne.msgADCPointNum = FREQ_CURRENT_HISTORY_MAX_SIZE;
			}
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_ADC_POINT_NUM_ADDR_X1, rfask->msgFreqCurrentPointOne.msgADCPointNum, 2);

			//---ADC合格的最大值
			rfask->msgFreqCurrentPointOne.msgADCPassMax = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 6];
			//---ADC合格的最大值
			rfask->msgFreqCurrentPointOne.msgADCPassMax = (rfask->msgFreqCurrentPointOne.msgADCPassMax << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 7];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_ADC_PASS_MAX_ADDR_X1, rfask->msgFreqCurrentPointOne.msgADCPassMax, 2);

			//---ADC合格的最小值
			rfask->msgFreqCurrentPointOne.msgADCPassMin = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 8];
			//---ADC合格的最小值
			rfask->msgFreqCurrentPointOne.msgADCPassMin = (rfask->msgFreqCurrentPointOne.msgADCPassMin << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 9];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_ADC_PASS_MIN_ADDR_X1, rfask->msgFreqCurrentPointOne.msgADCPassMin, 2);	

			//---设置截止的最大电流
			rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 10];
			//---设置截止的最大电流
			rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 11];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_STOP_MAX_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA, 2);

			//---设置截止的最小电流
			rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 12];
			//---设置截止的最小电流
			rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 13];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTONE_STOP_MIN_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA, 2);
			
			break;
			//---执行频率电流扫描任务
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_DO:
			_return = RFASKTask_FreqCurrentScan(USARTx, rfask, &rfask->msgFreqCurrentPointOne, WM8510x);
			break;
		default:
			_return = ERROR_1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 频率电流扫描
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
//UINT8_T  RFASKTask_FreqCurrentPointTwoTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x)
UINT8_T  RFASKTask_FreqCurrentPointTwoTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;

	//---执行任务命令
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
	{
		//---获取频率电流扫描的参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 11);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz >> 24));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz >> 16));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz >> 8));
			//---获取起始频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz));
			//---步进频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz >> 8));
			//---步进频率
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz));
			//---采集点的个数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgFreqPointNum >> 8));
			//---采集点的个数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgFreqPointNum));
			break;
			//---设置频率电流扫描的参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			
			//---设置起始频率
			rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---设置起始频率
			rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---设置起始频率
			rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			//---设置起始频率
			rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_START_FREQ_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz, 4);

			//---设置步进频率
			rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 5];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_STEP_FREQ_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz, 2);

			//---设置采集点的个数
			rfask->msgFreqCurrentPointTwo.msgFreqPointNum = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 6];
			rfask->msgFreqCurrentPointTwo.msgFreqPointNum = (rfask->msgFreqCurrentPointTwo.msgFreqPointNum << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 7];
			//---限制采样点的大小，这个值会影响上报上位机的时候数据缓存区的大小
			if (rfask->msgFreqCurrentPointTwo.msgFreqPointNum > FREQ_CURRENT_FREQ_POINT_MAX_NUM)
			{
				rfask->msgFreqCurrentPointTwo.msgFreqPointNum = FREQ_CURRENT_FREQ_POINT_MAX_NUM;
			}
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_POINT_NUM_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgFreqPointNum, 2);

			//---计算停止频率
			rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz = rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz;
			rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz *= rfask->msgFreqCurrentPointTwo.msgFreqPointNum;
			rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz += rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz;
			break;
			//---获取频率电流扫描的电流参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, (3 + 2 + 2 + 2 + 2 + 2 + 2 + 2));
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---获取起始的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA >> 8));
			//---获取起始的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA));
			//---获取起始的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA >> 8));
			//---获取起始的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA));
			//---ADC间隔的点数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPointNum >> 8));
			//---ADC间隔的点数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPointNum));
			//---ADC合格的最大值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPassMax >> 8));
			//---ADC合格的最大值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPassMax));
			//---ADC合格的最小值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPassMin >> 8));
			//---ADC合格的最小值
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgADCPassMin));
			//---获取截止的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA >> 8));
			//---获取截止的最大电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA));
			//---获取截止的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA >> 8));
			//---获取截止的最小电流
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA));
			break;
			//---设置频率电流扫描的电流参数
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---设置起始的最大电流
			rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---设置起始的最大电流
			rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA, 2);

			//---设置起始的最小电流
			rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			//---设置起始的最小电流
			rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_START_MIN_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA, 2);

			//---ADC间隔的点数
			rfask->msgFreqCurrentPointTwo.msgADCPointNum = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			//---ADC间隔的点数
			rfask->msgFreqCurrentPointTwo.msgADCPointNum = (rfask->msgFreqCurrentPointTwo.msgADCPointNum << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 5];
			//---判断数据是够超界
			if (rfask->msgFreqCurrentPointTwo.msgADCPointNum > FREQ_CURRENT_HISTORY_MAX_SIZE)
			{
				rfask->msgFreqCurrentPointTwo.msgADCPointNum = FREQ_CURRENT_HISTORY_MAX_SIZE;
			}
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_ADC_POINT_NUM_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgADCPointNum, 2);

			//---ADC合格的最大值
			rfask->msgFreqCurrentPointTwo.msgADCPassMax = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 6];
			//---ADC合格的最大值
			rfask->msgFreqCurrentPointTwo.msgADCPassMax = (rfask->msgFreqCurrentPointTwo.msgADCPassMax << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 7];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_ADC_PASS_MAX_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgADCPassMax, 2);

			//---ADC合格的最小值
			rfask->msgFreqCurrentPointTwo.msgADCPassMin = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 8];
			//---ADC合格的最小值
			rfask->msgFreqCurrentPointTwo.msgADCPassMin = (rfask->msgFreqCurrentPointTwo.msgADCPassMin << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 9];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_ADC_PASS_MIN_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgADCPassMin, 2);

			//---设置截止的最大电流
			rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 10];
			//---设置截止的最大电流
			rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 11];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_STOP_MAX_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA, 2);

			//---设置截止的最小电流
			rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 12];
			//---设置截止的最小电流
			rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 13];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_POINTTWO_STOP_MIN_CURRENT_ADDR_X1, rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA, 2);
			break;
			//---执行频率电流扫描任务
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_DO:
			_return = RFASKTask_FreqCurrentScan(USARTx, rfask, &rfask->msgFreqCurrentPointTwo, WM8510x);
			break;
		default:
			_return = ERROR_1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 上电4毫秒，掉电1ma
//////////////////////////////////////////////////////////////////////////////
UINT8_T RFASKTask_FreqCurrentScan(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, FREQ_CURRENT_HandlerType *rfaskFreqCurrent, WM8510_HandlerType *WM8510x)
{
	UINT8_T  siteNum = 0;
	UINT8_T  adcPointNum = 0;
	UINT16_T detaCurrentX10uA = 0;
	UINT16_T freqPointNum = 0;
	//---电流点计数
	UINT8_T   currentPointNum[FREQ_CURRENT_MAX_SITE] = {0};
	//---电流点的值
	UINT16_T  currentPointCurrent[FREQ_CURRENT_MAX_SITE] = {0};
	//---连续相等的点
	UINT8_T   currentEqualMinPointNum[FREQ_CURRENT_MAX_SITE] = {0};
	UINT8_T   currentEqualMaxPointNum[FREQ_CURRENT_MAX_SITE] = {0};
	//---激活的SITE的上次的电流值
	UINT16_T	siteOldCurrent[FREQ_CURRENT_MAX_SITE] = {0};
	//---SITE连续相等点的个数
	UINT8_T		siteCurrentEquaPointNum[FREQ_CURRENT_MAX_SITE] = { 0 };
	//---历史数据的搬移
	UINT16_T *pMsgActivateSiteCurrentHistory;
	UINT32_T freqX100MHz = rfaskFreqCurrent->msgStartFreqX100MHz;
	UINT32_T xtalHz = 0;
	//---停止解码，解码使用的中断优先级比较的高，避免因为中断优先级的问题，导致频率电流扫描异常的问题
	DecodeTask_STOP();
	//---复位看门狗
	WDT_RESET();
	//---初始化合格/失效判断结果数组---初始化为合格模式
	//memset(rfask->msgSitePass, 0, FREQ_CURRENT_MAX_SITE);
	for (freqPointNum = 0; freqPointNum < FREQ_CURRENT_MAX_SITE; freqPointNum++)
	{
		rfask->msgSitePass[freqPointNum] = 0;
		currentPointNum[freqPointNum] = 0;
		currentPointCurrent[freqPointNum] = 0;
		currentEqualMinPointNum[freqPointNum] = 0;
		currentEqualMaxPointNum[freqPointNum] = 0;
	}
	//---发送数据的报头
	USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
	//---设置数据大小
	USARTTask_RealTime_AddSize(USARTx, 4 + ((rfaskFreqCurrent->msgFreqPointNum + 1) * 2)*FREQ_CURRENT_MAX_SITE);
	//---返回的一级命令
	USARTTask_RealTime_AddByte(USARTx, rfaskFreqCurrent->msgCMD);
	//--得到返回结果
	USARTTask_RealTime_AddByte(USARTx, 0);
	//---返回的二级命令
	USARTTask_RealTime_AddByte(USARTx, CMD_RFASK_CMD1_FREQ_CURRENT_POINT_DO);
	freqPointNum = rfaskFreqCurrent->msgStartFreqX100MHz;
	freqPointNum /= 2000;
	freqPointNum = (freqPointNum << 3) + FREQ_CURRENT_MAX_SITE;
	//---返回SITE数
	USARTTask_RealTime_AddByte(USARTx, (UINT8_T)freqPointNum);
	freqPointNum = 0;
	//---打开时钟输出
	CLK_FREQ_ON;
	//---关闭电源
	DPS_POWER_HZ;
	//---进行频率电流的扫描
	for (freqPointNum = 0; freqPointNum < rfaskFreqCurrent->msgFreqPointNum; freqPointNum++)
	{
		//---关闭时钟输出
		// CLK_FREQ_OFF;
		//---关闭电源
		DPS_POWER_OFF;
		//---计算时钟频率，给定的频率是RF频率，需要计算成晶振频率
		xtalHz = RFASK_CalcXTAL(rfask, freqX100MHz);
		//---输出目标频率
		WM8510Task_I2C_SetFreqHzWithAllFreqReg(WM8510x, xtalHz);
		DelayTask_us(100);
		//---下一次的频率输出
		freqX100MHz += rfaskFreqCurrent->msgStepFreqX100MHz;
		//---打开电源
		DPS_POWER_ON;
		//---打开时钟输出
		// CLK_FREQ_ON;
		//if	(rfaskFreqCurrent->msgStepFreqX100MHz<300)
		//{
		//
		//	//---等待上电稳定
		//	DelayTask_ms(8);
		//}
		//else
		//{
		//
		//	//---等待上电稳定
		//	DelayTask_ms(6);
		//}
		//-- - 等待上电稳定
		DelayTask_ms(5);		
		//---开始n个点，多等待一下
		if (freqPointNum <3)
		{
			DelayTask_ms(1);
		}
		//---前10个点，多等几次上电稳定
		//if (freqPointNum <10)
		//{
		//	//---获取每个SITE的电流和ADC的采样结果
		//	RFASKTask_SitesCurrent(rfask);
		//	DelayTask_ms(10);
		//}
		//---获取每个SITE的电流和ADC的采样结果
		RFASKTask_SitesCurrent(rfask);
		//---轮训判断各个SITE的结果
		for (siteNum = 0; siteNum < FREQ_CURRENT_MAX_SITE; siteNum++)
		{
			//---起始点
			if (freqPointNum == 0)
			{
				if ((rfask->msgSiteCurrent[siteNum] < rfaskFreqCurrent->msgStartMinCurrentX100mA) || (rfask->msgSiteCurrent[siteNum] > (rfaskFreqCurrent->msgStartMaxCurrentX100mA + 200)))
				{
					//---判断不合格；原因是第一个点的电流比最大的大，比最小的小
					rfask->msgSitePass[siteNum] = 1;
				}
			}
			//---终止点
			if ((rfask->msgSitePass[siteNum] == 0) && (freqPointNum == (rfaskFreqCurrent->msgFreqPointNum - 1)))
			{
				if ((rfask->msgSiteCurrent[siteNum] < rfaskFreqCurrent->msgStopMinCurrentX100mA) || (rfask->msgSiteCurrent[siteNum] > rfaskFreqCurrent->msgStopMaxCurrentX100mA))
				{
					//---判断不合格；原因是最后一个点的电流比最大的大，比最小的小
					rfask->msgSitePass[siteNum] = 1 + freqPointNum;
				}
			}
			//---判断合格条件
			if (rfask->msgSitePass[siteNum] == 0)
			{
				if (currentPointNum[siteNum] == 0)
				{
					//---获取当前电流值
					currentPointCurrent[siteNum] = rfask->msgSiteCurrent[siteNum];
				}
				currentPointNum[siteNum]++;				
				//---电流出现连续相等的状态逻辑判断开始
				//---判断电流是否发生变化
				if (siteOldCurrent[siteNum]!= rfask->msgSiteCurrent[siteNum])
				{
					siteCurrentEquaPointNum[siteNum] = 0;
				}
				else
				{
					siteCurrentEquaPointNum[siteNum] ++;
					//---连续多个点的电流值不发生变化
					//if (siteCurrentEquaPointNum[siteNum] > 3)					
					if (siteCurrentEquaPointNum[siteNum] > RFASK_FREQ_CURRENT_CONT_EQUAL_POINT_MAX_NUM)
					{
						//---判断不合格；原因是该点的电流之前的几个点小或者差值比设定合格条件最小值小，或则比设定的合格条件的最大值大
						rfask->msgSitePass[siteNum] = 1 + freqPointNum;
					}
				}
				//---更新SITE的电流值
				siteOldCurrent[siteNum] = rfask->msgSiteCurrent[siteNum];
				//---电流出现连续相等的状态逻辑判断结束
				//---合格条件的频率点差值的计算
				if (freqPointNum < rfaskFreqCurrent->msgADCPointNum)
				{
					//---填充历史数据
					rfask->msgSiteCurrentHistory[siteNum][freqPointNum] = rfask->msgSiteCurrent[siteNum];
				}
				else
				{
					//---历史数据前移
					pMsgActivateSiteCurrentHistory = rfask->msgSiteCurrentHistory[siteNum];
					for (adcPointNum = 0; adcPointNum < (rfaskFreqCurrent->msgADCPointNum - 1); adcPointNum++)
					{
						*pMsgActivateSiteCurrentHistory = *(pMsgActivateSiteCurrentHistory + 1);
						pMsgActivateSiteCurrentHistory++;
					}
					//---最后一个数据填充为当前采集值
					*pMsgActivateSiteCurrentHistory = rfask->msgSiteCurrent[siteNum];
					//---这部分差值的计算，进行重新处理，目前发现电流的偏差很小
					detaCurrentX10uA = 1;//---避免下限值相等的问题
					if (rfask->msgSiteCurrent[siteNum] > rfask->msgSiteCurrentHistory[siteNum][0])
					{
						detaCurrentX10uA = rfask->msgSiteCurrent[siteNum] - rfask->msgSiteCurrentHistory[siteNum][0];
					}
					else if (rfask->msgSiteCurrent[siteNum] < rfask->msgSiteCurrentHistory[siteNum][0])
					{
						detaCurrentX10uA = rfask->msgSiteCurrentHistory[siteNum][0] - rfask->msgSiteCurrent[siteNum];
					}
					////---计算电流差值
					//detaVal = (UINT16_T)(rfask->msgActivateSiteCurrent[siteNum] - rfask->msgActivateSiteCurrentHistory[siteNum][0]);
					//---判断采集点合格条件
					//if ((rfask->msgSiteCurrent[siteNum] < rfask->msgSiteCurrentHistory[siteNum][0]) || (detaCurrentX10uA < rfaskFreqCurrent->msgADCPassMin) || (adcPointNum > rfaskFreqCurrent->msgADCPassMax))
					//if((detaCurrentX10uA < rfaskFreqCurrent->msgADCPassMin) || (detaCurrentX10uA > rfaskFreqCurrent->msgADCPassMax))					
					//---小于最小值
					if (detaCurrentX10uA < rfaskFreqCurrent->msgADCPassMin)
					{
						currentEqualMinPointNum[siteNum]++;
						//---下限连续相等的点数
						//if (currentEqualMinPointNum[siteNum] > 3)
						if (currentEqualMinPointNum[siteNum] > RFASK_FREQ_CURRENT_CONT_EQUAL_POINT_MAX_NUM)
						{
							//---判断不合格；原因是该点的电流之前的几个点小或者差值比设定合格条件最小值小，或则比设定的合格条件的最大值大
							rfask->msgSitePass[siteNum] = 1 + freqPointNum;
						}
					}
					//---大于最大值
					else if (detaCurrentX10uA > rfaskFreqCurrent->msgADCPassMax)
					{
						currentEqualMaxPointNum[siteNum]++;
						//---上限连续相等的点数
						//if (currentEqualMaxPointNum[siteNum] > 3)
						if (currentEqualMaxPointNum[siteNum] > RFASK_FREQ_CURRENT_CONT_EQUAL_POINT_MAX_NUM)
						{
							//---判断不合格；原因是该点的电流之前的几个点小或者差值比设定合格条件最小值小，或则比设定的合格条件的最大值大
							rfask->msgSitePass[siteNum] = 1 + freqPointNum;
						}
					}

					//---在两者之间
					else
					{
						currentEqualMinPointNum[siteNum] = 0;
						currentEqualMaxPointNum[siteNum] = 0;
					}
					//---第21个点一定要比第一个点大30uA
					if (currentPointNum[siteNum] >= 20)
					{
						currentPointNum[siteNum] = 0;
						//---计算电流差值
						//detaCurrentX10uA = (UINT16_T)(rfask->msgSiteCurrent[siteNum] - currentPointCurrent[siteNum]);
						detaCurrentX10uA = ABS_SUB(rfask->msgSiteCurrent[siteNum],currentPointCurrent[siteNum]);
						//---判断采集点合格条件
						if ((detaCurrentX10uA <3)||(rfask->msgSiteCurrent[siteNum]<currentPointCurrent[siteNum]))
						{
							//---判断不合格；原因是该点的电流之前的几个点小或者差值比设定合格条件最小值小，或则比设定的合格条件的最大值大
							rfask->msgSitePass[siteNum] = 1 + freqPointNum;
						}
					}
				}
			}
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSiteCurrent[siteNum] >> 8));
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSiteCurrent[siteNum]));
		}
		//---喂狗，避免长时间的，从而导致看门狗复位
		WDT_RESET();
	}
	//---发送测试完成的结果
	for (siteNum = 0; siteNum < FREQ_CURRENT_MAX_SITE; siteNum++)
	{
		USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSitePass[siteNum] >> 8));
		USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSitePass[siteNum]));
	}
	//---关闭时钟输出
	CLK_FREQ_OFF;
	//---关闭电源
	DPS_POWER_OFF;
	//---设置频率工作在不工作的点,默认是20KHz
	//RFASKTask_SetClockFreq(rfask, WM8510x, FREQ_YSEL_X100MHz);
	//---复位时钟芯片WM8510，需要评估一下那个比较的适合当前模式
	WM8510Task_I2C_Reset(WM8510x);
	//---频率电流扫描主板的第二个版本，可以支持实时解码
	#if (LNW_FT_ASK_MBOARD_VERSION==2)
		//---启动解码
		DecodeTask_START();
	#endif  	
	//---返回值
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：获取设备的基本类型
//////////////////////////////////////////////////////////////////////////////
//UINT8_T RFASKTask_FreqCurrentHandlerTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask)
UINT8_T RFASKTask_FreqCurrentHandlerTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;
	//---执行任务命令
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
	{
		//---获取设备类型
		case CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_TYPE_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 4);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回设备类型
			USARTTask_RealTime_AddByte(USARTx, rfask->msgDeviceType);
			break;
			//---获取采样电阻
		case CMD_RFASK_CMD1_FREQ_CURRENT_SAMPLE_RES_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 5);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回采样电阻的大小
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSampleX100Res >> 8));
			//---返回采样电阻的大小
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSampleX100Res));
			break;
			//---获取放大倍数
		case CMD_RFASK_CMD1_FREQ_CURRENT_AMP_TIMES_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 5);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回放大倍数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgAmpTimes >> 8));
			//---返回放大倍数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgAmpTimes));
			break;
			//---设置设备类型
		case CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_TYPE_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---重新设置设备的类型
			rfask->msgDeviceType = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_TYPE_ADDR_X1, rfask->msgDeviceType, 1);
			break;
			//---设置采样电阻
		case CMD_RFASK_CMD1_FREQ_CURRENT_SAMPLE_RES_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---重新设置采样电阻的大小
			rfask->msgSampleX100Res = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			rfask->msgSampleX100Res = (rfask->msgSampleX100Res << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_SAMPLE_RES_ADDR_X1, rfask->msgSampleX100Res, 2);
			break;
			//---设置放大倍数
		case CMD_RFASK_CMD1_FREQ_CURRENT_AMP_TIMES_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			
			//---重新设置放大倍数
			rfask->msgAmpTimes = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			rfask->msgAmpTimes = (rfask->msgSampleX100Res << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_AMP_TIMES_ADDR_X1, rfask->msgAmpTimes, 2);
			break;
			//---获取设备信息
		case CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_GET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 8);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
			//---返回设备类型
			USARTTask_RealTime_AddByte(USARTx, rfask->msgDeviceType);
			//---返回采样电阻的大小
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSampleX100Res >> 8));
			//---返回采样电阻的大小
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgSampleX100Res));
			//---返回放大倍数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgAmpTimes >> 8));
			//---返回放大倍数
			USARTTask_RealTime_AddByte(USARTx, (UINT8_T)(rfask->msgAmpTimes));
			break;
			//---设置设备信息
		case CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_SET:
			//---数据发送报头
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
			//---设置数据大小
			USARTTask_RealTime_AddSize(USARTx, 3);
			//---返回的一级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
			//--得到返回结果
			USARTTask_RealTime_AddByte(USARTx, 0);
			//---返回的二级命令
			USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);

			//---重新设置设备的类型
			rfask->msgDeviceType = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_TYPE_ADDR_X1, rfask->msgDeviceType, 1);

			//---重新设置采样电阻的大小
			rfask->msgSampleX100Res = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
			rfask->msgSampleX100Res = (rfask->msgSampleX100Res << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 2];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_SAMPLE_RES_ADDR_X1, rfask->msgSampleX100Res, 2);

			//---重新设置放大倍数
			rfask->msgAmpTimes = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 3];
			rfask->msgAmpTimes = (rfask->msgAmpTimes << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 4];
			//---保存配置参数
			RFASK_EEPROMWrite(AT24CXXx, RFASK_AMP_TIMES_ADDR_X1, rfask->msgAmpTimes, 2);
			break;
		default:
			_return = 1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：获取当前程序编译的日期和时间
//////////////////////////////////////////////////////////////////////////////
UINT8_T RFASKTask_VersionTask(USART_HandlerType*USARTx)
{
	UINT8_T i = 0;
	//---数据发送报头
	USARTTask_RealTime_AddByte(USARTx, USARTx->msgTxID);
	//---设置数据大小
	USARTTask_RealTime_AddSize(USARTx, (3 + VERSION_DATE_SIZE + VERSION_TIME_SIZE));
	//---返回的一级命令
	USARTTask_RealTime_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
	//--得到返回结果
	USARTTask_RealTime_AddByte(USARTx, 0);
	//---返回的二级命令
	USARTTask_RealTime_AddByte(USARTx, 0xFF);
	//---版本日期
	for (i = 0; i < VERSION_DATE_SIZE; i++)
	{
		USARTTask_RealTime_AddByte(USARTx, g_VersionDate[i]);
	}
	//---版本时间
	for (i = 0; i < VERSION_TIME_SIZE; i++)
	{
		USARTTask_RealTime_AddByte(USARTx, g_VersionTime[i]);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
//UINT8_T RFASKTask_HandlerTask(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x)
UINT8_T RFASKTask_HandlerTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x,AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;
	//---命令处理函数
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex])
	{
		case CMD_RFASK_CMD1_VERSIOM:
			break;
			//---WM8510时钟的控制
		case CMD_RFASK_CMD1_FREQ_WM8510:
			_return = RFASKTask_WM8510Task(USARTx, WM8510x,AT24CXXx);
			break;
			//---默认预设频率点的控制
		case CMD_RFASK_CMD1_YSEL_FREQ:
			_return = RFASKTask_YSELTask(USARTx, rfask, WM8510x, AT24CXXx);
			break;
			//---频率电流扫描的参数
		case CMD_RFASK_CMD1_FREQ_CURRENT:
			RFASKTask_FreqCurrentHandlerTask(USARTx, rfask, AT24CXXx);
			break;
			//---第一个电压点的频率电流扫描
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_ONE:
			//---频率电流扫描
			_return = RFASKTask_FreqCurrentPointOneTask(USARTx, rfask, WM8510x, AT24CXXx);
			break;
			//---第二个电压点的频率电流扫描
		case CMD_RFASK_CMD1_FREQ_CURRENT_POINT_TWO:
			//---频率电流扫描
			_return = RFASKTask_FreqCurrentPointTwoTask(USARTx, rfask, WM8510x, AT24CXXx);
			break;
		default:
			_return = ERROR_1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：在线任务处理通过串口下发命令
//////////////////////////////////////////////////////////////////////////////
//UINT8_T RFASKTask_Task(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x)
UINT8_T RFASKTask_Task(USART_HandlerType* USARTx, RFASK_HandlerType* rfask, WM8510_HandlerType* WM8510x,AT24CXX_HandlerType* AT24CXXx)
{
	//---判断接收是否完成
	if (USARTTask_GetReadState(USARTx) == 1)
	{
		//---CRC的校验和设备ID校验
		if ((USARTTask_CRCTask_Read(USARTx) == OK_0) && (USARTTask_DeviceID(USARTx) == OK_0))
		{
			//---任务命令处理函数
			if (RFASKTask_HandlerTask(USARTx, rfask, WM8510x,AT24CXXx) == OK_0)
			{
				//---判断是否发送CRC
				USARTTask_RealTime_AddCRC(USARTx);
			}
		}
		else
		{
			//---发生CRC校验错误
			USART_Printf(USARTx, "=>>串口%d:发生CRC校验错误<<=\r\n", (USARTx->msgIndex - 1));
		}
		return USARTTask_Read_Init(USARTx);
	}
	return USARTTask_TimeOVFTask(USARTx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_ScanYSEL(void)
{
	return RFASKLib_ScanYSEL();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_EOTSTART(RFASK_HandlerType *rfask)
{
	KeyTask_EOTSTART();
	//---初始化合格/失效判断结果数组---初始化为合格模式
	memset(rfask->msgSitePass, 0, FREQ_CURRENT_MAX_SITE);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_EOTSTOP(void)
{
	KeyTask_EOTSTOP();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  RFASKTask_BINPass(RFASK_HandlerType *rfask)
{
	UINT8_T i = 0;
	for (i = 0; i < FREQ_CURRENT_MAX_SITE; i++)
	{
		switch (i)
		{
			case 0:
				KeyTask_BINAPass(rfask->msgSitePass[i]);
				break;
			case 1:
				KeyTask_BINBPass(rfask->msgSitePass[i]);
				break;
			case 2:
				KeyTask_BINCPass(rfask->msgSitePass[i]);
				break;
			case 3:
				KeyTask_BINDPass(rfask->msgSitePass[i]);
				break;
			default:
				//---SITE都不合格
				KeyTask_BINAPass(1);
				KeyTask_BINBPass(1);
				KeyTask_BINCPass(1);
				KeyTask_BINDPass(1);
				break;
		}
	}
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：按键扫描任务
//////////////////////////////////////////////////////////////////////////////
UINT8_T RFASKTask_KeyTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, UINT8_T activateSites)
{
	//---获取YSEL的值
	UINT8_T _return = RFASKTask_ScanYSEL();
	//---判断YSEL是否有效
	if (_return == 0xFF)
	{
		//---没有解读到YSEL信息，返回2
		return ERROR_2;
	}

	//---清除所有的状态标志位
	else if (_return == RFASK_TASK_CLEAR)
	{
		//---设置时钟不输出
		CLK_FREQ_OFF;
		//---复位时钟芯片WM8510
		WM8510Task_I2C_Reset(WM8510x);
		//---SOT输出高
		EOT_CTR_OUT_1;
		BIN_CTR_OUT_1;
		//---启动解码
		DecodeTask_START();
	}
	else
	{
		//---判断是否有SITE激活，避免误操作
		if (activateSites == 0)
		{
			//---没有SITE激活，
			return ERROR_3;
		}
		
		//---关闭解码
		DecodeTask_STOP();
		
		//---SOT输出高,清楚状态标志位，避免下次进入的时候状态保留的事上一状态
		EOT_CTR_OUT_1;
		BIN_CTR_OUT_1;
		//---获取激活的SITE信息
		RFASKTask_ActivateSites(rfask, activateSites);
		//---初始化EOT
		RFASKTask_EOTSTART(rfask);
		//---任务序号
		switch (_return)
		{
			case RFASK_TASK_YSEL1:
				//---设置默认频率1
				RFASKTask_SetClockFreq(rfask, WM8510x, rfask->msgFreqX100MHzYSel1);
				//---使能频率输出
				CLK_FREQ_ON;
				break;
			case RFASK_TASK_YSEL2:
				//---设置默认频率2
				RFASKTask_SetClockFreq(rfask, WM8510x, rfask->msgFreqX100MHzYSel2);
				//---使能频率输出
				CLK_FREQ_ON;
				break;
			case RFASK_TASK_YSEL3:
				//---设置默认频率3
				RFASKTask_SetClockFreq(rfask, WM8510x, rfask->msgFreqX100MHzYSel3);
				//---使能频率输出
				CLK_FREQ_ON;
				break;
			case RFASK_TASK_YSEL4:
				//---设置默认频率4
				RFASKTask_SetClockFreq(rfask, WM8510x, rfask->msgFreqX100MHzYSel4);
				//---使能频率输出
				CLK_FREQ_ON;
				break;
			case RFASK_TASK_POINT_ONE:
				//---执行频率电流扫描第一个点
				RFASKTask_FreqCurrentScan(USARTx, rfask, &rfask->msgFreqCurrentPointOne, WM8510x);
				//---不使能频率输出
				CLK_FREQ_OFF;
				//---电源高阻
				DPS_POWER_HZ;
				break;
			case RFASK_TASK_POINT_TWO:
				//---执行频率电流扫描第二个点
				RFASKTask_FreqCurrentScan(USARTx, rfask, &rfask->msgFreqCurrentPointTwo, WM8510x);
				//---不使能频率输出
				CLK_FREQ_OFF;
				//---电源高阻
				DPS_POWER_HZ;
				break;
			default:
				CLK_FREQ_OFF;
				_return = 0xFF;
				break;
		}

		//---执行结果
		RFASKTask_BINPass(rfask);
		//---完成EOT信号
		RFASKTask_EOTSTOP();

		//if((_return!=RFASK_TASK_POINT_ONE)&&(_return!=RFASK_TASK_POINT_TWO))
		//{
		//		DelayTask_ms(20);
		//		//---启动解码
		//		DecodeTask_START();
		//}
	}
	if (_return != 0xFF)
	{
		_return = 0;
	}
	return _return;
}
