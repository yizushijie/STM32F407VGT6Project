#include "jtag_task.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	return JTAGLib_Init(JTAGx, pFuncDelayus, pFuncDelayms, pFuncTimerTick);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置固定编程时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_SetProgClock(JTAG_HandlerType* JTAGx, UINT8_T clok)
{
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ExitProg(JTAG_HandlerType* JTAGx)
{
	return JTAGLib_ExitProg(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_EnterProg(JTAG_HandlerType* JTAGx)
{
	return JTAGLib_EnterProg(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_EnterProgAndConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T isPollReady, UINT8_T* pVal)
{
	return JTAGLib_EnterProgAndConfigInfo(JTAGx, isPollReady, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_EraseChip(JTAG_HandlerType* JTAGx)
{
	return JTAGLib_EraseChip(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAGLib_ReadIDChip(JTAGx, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAGLib_ReadChipID(JTAGx, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAGLib_ReadChip(JTAGx, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length)
{
	return JTAGLib_ReadChipCalibration(JTAGx, pVal, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	return JTAGLib_ReadChipFuse(JTAGx, pVal, isNeedExternFuse);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAGLib_ReadChipLock(JTAGx, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length)
{
	return JTAGLib_ReadChipRom(JTAGx, pVal, addr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	return JTAGLib_WriteChipFuse(JTAGx, pVal, isNeedExternFuse);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal)
{
	return JTAGLib_WriteChipLock(JTAGx, lockVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	return JTAGLib_ReadChipEepromAddr(JTAGx, pVal, highAddr, lowAddr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length)
{
	return JTAGLib_ReadChipEepromLongAddr(JTAGx, pVal, addr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	return JTAGLib_WriteChipEeprom(JTAGx, pVal, highAddr, lowAddr, pageNum);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	return JTAGLib_ReadChipFlashAddr(JTAGx, pVal, externAddr, highAddr, lowAddr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length)
{
	return JTAGLib_ReadChipFlashLongAddr(JTAGx, pVal, addr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	return JTAGLib_WriteChipFlashPage(JTAGx, pVal, externAddr, highAddr, lowAddr, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL)
{
	return JTAGLib_CheckChipFlashEmpty(JTAGx, pageByteSizeH, pageByteSizeL, pageNumH, pageNumL);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum)
{
	return JTAGLib_CheckChipFlashEmptyLong(JTAGx, pageByteSize, pageNum);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num)
{
	return JTAGLib_CheckChipEepromEmpty(JTAGx, byteSize, num);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAGLib_SetConfigInfo(JTAGx, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_ReadChipPower(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	//---读取芯片的供电电压
	UINT16_T tempPower = ADCTask_GetChipPower();
	//---读取电压值
	//---填充数据
	*(pVal++) = (UINT8_T)(tempPower >> 8);
	*pVal = (UINT8_T)(tempPower);
	return  OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_WriteChipPower(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT16_T tempPower = *(pVal++);
	tempPower = (tempPower << 8) + *(pVal++);
	//---通过DAC设置可调电源LM317的输出值
	LM317Task_Init(0, tempPower);
	//---校验电源是否开启
	if (*pVal > 0)
	{
		LM317_POWER_ON;
	}
	else
	{
		LM317_POWER_HZ;
	}
	return  OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出或者进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_OpenAndClose(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	//---命令位置
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 1)
	{
		//---进入编程模式，并配置基本参数
		_return = JTAGTask_EnterProgAndConfigInfo(JTAGx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset], USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1);
	}
	else if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 2)
	{
		//---配置基本参数,主要是使不使能EEPROM的页编程模式
		_return = JTAGTask_SetConfigInfo(JTAGx, USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1);
	}
	else
	{
		_return = JTAGTask_ExitProg(JTAGx);
	}
	//---执行结果
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：擦除Chip
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_EraseChip(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T _return = OK_0;
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 0)
	{
		//---设备擦除
		_return = JTAGTask_EraseChip(JTAGx);
	}
	else if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 1)
	{
		//---检查Flash为空
		_return = JTAGTask_CheckChipFlashEmpty(JTAGx,
			USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 1], USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 2],
			USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 3], USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 4]
		);
	}
	else if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 2)
	{
		//---检查Eeprom为空
		_return = JTAGTask_CheckChipEepromEmpty(JTAGx,
			USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 1], USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 2]
		);
	}
	else
	{
		_return = 0xFF;
	}
	//---检验是不是查空操作
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] != 0)
	{
		//---填充查空操作的值
		USARTTask_FillMode_AddByte(USARTx, _return);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取芯片ID
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipID(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	//---读取设备ID信息
	_return = JTAGTask_ReadChipID(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW);
	//---数据长度偏移
	USARTx->msgTxdHandler.msgIndexW += 3;
	//---执行结果
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取芯片的校准字
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipCalibration(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	//---读取设备的校准字
	_return = JTAGTask_ReadChipCalibration(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset]);
	//---数据地址偏移
	USARTx->msgTxdHandler.msgIndexW += USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset];
	//---执行结果
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取芯片的熔丝位和校准字
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipFuseAndLock(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	//---判断是读取熔丝位还是加密位
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 0)
	{
		//---读取熔丝位
		_return = JTAGTask_ReadChipFuse(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset]);
		//---校验是否读取拓展熔丝位
		if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset] != 0)
		{
			USARTx->msgTxdHandler.msgIndexW += 1;
		}
		USARTx->msgTxdHandler.msgIndexW += 2;
	}
	else
	{
		//---读取加密位
		_return = JTAGTask_ReadChipLock(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW);
		USARTx->msgTxdHandler.msgIndexW += 1;
	}
	//---执行结果
	return _return;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程熔丝位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_WriteChipFuse(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	return JTAGTask_WriteChipFuse(JTAGx, USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataTwoIndex + USARTx->msgIndexOffset, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset]);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程加密位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_WriteChipLock(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	return JTAGTask_WriteChipLock(JTAGx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset]);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取芯片的ROM信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipRom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	UINT16_T length = 0;
	//---计算读取数据的大小
	if (USARTx->msgRxdHandler.msgSize < 0xFF)
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset];
	}
	else
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset];
		length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1];
	}
	//---读取ROM页信息
	_return = JTAGTask_ReadChipRom(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset], length);
	//---数据的偏移
	USARTx->msgTxdHandler.msgIndexW += length;
	//---执行结果
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置编程时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_SetProgClok(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T _return = OK_0;
	//---读取电压
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 1)
	{
		_return = JTAGTask_ReadChipPower(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 1);
		USARTx->msgTxdHandler.msgIndexW += 2;
	}
	//---设置电压
	else if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset] == 2)
	{
		_return = JTAGTask_WriteChipPower(JTAGx, USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 1);
	}
	else
	{
		//---设置编程时钟
		_return = JTAGTask_SetProgClock(JTAGx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset + 1]);
	}

	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：通过串口命令读取Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipFlash(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	UINT16_T length = 0;
	//---计算读取数据的大小
	if (USARTx->msgRxdHandler.msgSize < 0xFF)
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
	}
	else
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
		length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 3];
	}
	//---读取指定位置的Flash数据
	_return = JTAGTask_ReadChipFlashAddr(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW,
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1],
		length);
	//---数据的偏移
	USARTx->msgTxdHandler.msgIndexW += length;
	//---执行结果
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：通过串口命令编程Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_WriteChipFlashPage(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T dataOffset = 0;
	UINT16_T length = 0;
	//---计算写入数据的大小
	if (USARTx->msgRxdHandler.msgSize < 0xFF)
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
		dataOffset = 3;
	}
	else
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
		length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 3];
		dataOffset = 4;
	}
	//---从指定位置编程数据
	return JTAGTask_WriteChipFlashPage(JTAGx, USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + dataOffset,
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1],
		length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：通过串口命令读取eeprom数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ReadChipEeprom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T	_return = 0;
	UINT16_T length = 0;
	//---计算读取数据的大小
	if (USARTx->msgRxdHandler.msgSize < 0xFF)
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1];
	}
	else
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1];
		length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
	}
	//---读取指定位置的Eeprom数据
	_return = JTAGTask_ReadChipEepromAddr(JTAGx, USARTx->msgTxdHandler.pMsgVal + USARTx->msgTxdHandler.msgIndexW,
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset],
		length);
	//---数据的偏移
	USARTx->msgTxdHandler.msgIndexW += length;
	//---执行结果
	return _return;

}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：通过串口命令编程eeprom数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_WriteChipEeprom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T dataOffset = 0;
	UINT16_T length = 0;
	//---计算读取数据的大小
	if (USARTx->msgRxdHandler.msgSize < 0xFF)
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1];
		dataOffset = 2;
	}
	else
	{
		length = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 1];
		length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + 2];
		dataOffset = 3;
	}
	//---编程指定位置的Eeprom数据
	return JTAGTask_WriteChipEeprom(JTAGx, USARTx->msgRxdHandler.pMsgVal + USARTx->msgDataTwoIndex + USARTx->msgIndexOffset + dataOffset,
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex + USARTx->msgIndexOffset],
		USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + USARTx->msgIndexOffset],
		length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ChildTask(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx, UINT8_T isChildCmd)
{
	UINT8_T _return = 0;
	//---填充测试执行结果
	USARTTask_FillMode_AddByte(USARTx, 0x00);
	USARTx->msgIndexOffset = (isChildCmd == 0 ? 0 : 1);
	//---依据命令解析数据
	switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex + USARTx->msgIndexOffset])
	{
		case CMD_JTAG_OPEN_CLOSE:
			//---命令之后，第1字节0---代表关闭，1---代表打开；第2字节与打开有关，与关闭无关
			_return = JTAGTask_USARTCmd_OpenAndClose(JTAGx, USARTx);
			break;
		case CMD_JTAG_ERASE:
			//---擦除设备，第1字节0---代表擦除，1---代表查空Flash，2---代表查空Eeprom
			_return = JTAGTask_USARTCmd_EraseChip(JTAGx, USARTx);
			break;
		case CMD_JTAG_FLASH_PAGE_READ:
			//---读取Flash
			_return = JTAGTask_USARTCmd_ReadChipFlash(JTAGx, USARTx);
			break;
		case CMD_JTAG_FLASH_PAGE_WRITE:
			//---编程Flash
			_return = JTAGTask_USARTCmd_WriteChipFlashPage(JTAGx, USARTx);
			break;
		case CMD_JTAG_EEPROM_PAGE_READ:
			//---读取Eeprom
			_return = JTAGTask_USARTCmd_ReadChipEeprom(JTAGx, USARTx);
			break;
		case CMD_JTAG_EEPROM_PAGE_WRITE:
			//---编程Eeprom
			_return = JTAGTask_USARTCmd_WriteChipEeprom(JTAGx, USARTx);
			break;
		case CMD_JTAG_FUSE_LOCK_READ:
			//---读取熔丝位或者校验位
			_return = JTAGTask_USARTCmd_ReadChipFuseAndLock(JTAGx, USARTx);
			break;
		case CMD_JTAG_FUSE_WRITE:
			//---编程熔丝位
			_return = JTAGTask_USARTCmd_WriteChipFuse(JTAGx, USARTx);
			break;
		case CMD_JTAG_LOCK_WRITE:
			//---编程加密位
			_return = JTAGTask_USARTCmd_WriteChipLock(JTAGx, USARTx);
			break;
		case CMD_JTAG_ID_READ:
			//---读取设备的ID
			_return = JTAGTask_USARTCmd_ReadChipID(JTAGx, USARTx);
			break;
		case CMD_JTAG_CALIBRATIONBYTE_READ:
			//---读取校准字
			_return = JTAGTask_USARTCmd_ReadChipCalibration(JTAGx, USARTx);
			break;
		case CMD_JTAG_ROM_PAGE_READ:
			//---读取ROM页信息
			_return = JTAGTask_USARTCmd_ReadChipRom(JTAGx, USARTx);
			break;
		case CMD_JTAG_PROG_CLOCK_SET:
			//---设置编程时钟，多级参数，后续需要有电源的设置和读取
			_return = JTAGTask_USARTCmd_SetProgClok(JTAGx, USARTx);
			break;
		default:
			//---不识别的命令
			_return = ERROR_1;
			break;
	}
	//---填充测试结果
	USARTTask_FillMode_SetResultFlag(USARTx, _return);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ParentTask(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx, UINT8_T isChildCmd)
{
	if ((USARTx != NULL) && (JTAGx != NULL))
	{
		//---判断接收是否完成
		if (USARTTask_GetReadState(USARTx) == 1)
		{
			//---CRC的校验和设备ID校验
			if ((USARTTask_CRCTask_Read(USARTx) == OK_0) && (USARTTask_DeviceID(USARTx) == OK_0))
			{
				//---任务命令处理函数，数据报头，长度，地址ID,命令的处理
				USARTTask_FillMode_Init(USARTx, isChildCmd);
				//---处理任务
				JTAGTask_USARTCmd_ChildTask(JTAGx, USARTx, isChildCmd);
				//---是否需要增加换行符
				if (USARTx->msgTxdHandler.msgAddNewLine == 1)
				{
					USARTTask_FillMode_AddByte(USARTx, 0x0D);
					USARTTask_FillMode_AddByte(USARTx, 0x0A);
				}
				//---启动数据发送
				USARTTask_FillMode_WriteSTART(USARTx, 0);
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
	return ERROR_2;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_ParentTask_New(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx, UINT8_T isChildCmd)
{
	UINT8_T _return=OK_0;
	//---任务命令处理函数，数据报头，长度，地址ID,命令的处理
	USARTTask_FillMode_Init(USARTx, isChildCmd);
	//---处理任务
	_return=JTAGTask_USARTCmd_ChildTask(JTAGx, USARTx, isChildCmd);
	//---是否需要增加换行符
	if (USARTx->msgTxdHandler.msgAddNewLine == 1)
	{
		USARTTask_FillMode_AddByte(USARTx, 0x0D);
		USARTTask_FillMode_AddByte(USARTx, 0x0A);
	}
	//---启动数据发送
	USARTTask_FillMode_WriteSTART(USARTx, 0);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGTask_USARTCmd_Task(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx)
{
	UINT8_T _return = OK_0;
	//_return = JTAGTask_USARTCmd_ParentTask(JTAGx, USARTx, 0);
	_return = JTAGTask_USARTCmd_ParentTask_New(JTAGx, USARTx, 0);
	return _return;
}