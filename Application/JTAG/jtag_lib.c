#include "jtag_lib.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	return JTAG_Init(JTAGx, pFuncDelayus, pFuncDelayms, pFuncTimerTick);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ExitProg(JTAG_HandlerType* JTAGx)
{
	return JTAG_ExitProg(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_EnterProg(JTAG_HandlerType* JTAGx)
{
	return JTAG_EnterProg(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：进入编程模式并配置Memery信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_EnterProgAndConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T isPollReady, UINT8_T* pVal)
{
	UINT8_T _return = JTAG_EnterProg(JTAGx);
	//---校验进入编程模式
	if (_return == OK_0)
	{
		_return = JTAG_SetConfigInfo(JTAGx, pVal);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_EraseChip(JTAG_HandlerType* JTAGx)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---擦除
		_return = JTAG_EraseChip(JTAGx);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取JTAG的ID信息
		_return = JTAG_ReadIDChip(JTAGx, pVal);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取Chip的ID信息
		_return = JTAG_ReadChipID(JTAGx, pVal);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取ID信息
		_return = JTAG_ReadChip(JTAGx, pVal);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取校准字信息
		_return = JTAG_ReadChipCalibration(JTAGx, pVal, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取熔丝位信息
		_return = JTAG_ReadChipFuse(JTAGx, pVal, isNeedExternFuse);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取加密位信息
		_return = JTAG_ReadChipLock(JTAGx, pVal);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取ROM页信息
		_return = JTAG_ReadChipRom(JTAGx, pVal, addr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---编程熔丝位信息
		_return = JTAG_WriteChipFuse(JTAGx, pVal, isNeedExternFuse);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---编程加密位信息
		_return = JTAG_WriteChipLock(JTAGx, lockVal);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取EEPROM信息
		_return = JTAG_ReadChipEepromAddr(JTAGx, pVal, highAddr, lowAddr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取EEPROM信息
		_return = JTAG_ReadChipEepromLongAddr(JTAGx, pVal, addr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---编程EEPROM信息
		_return = JTAG_WriteChipEeprom(JTAGx, pVal, highAddr, lowAddr, pageNum);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取FLASH信息
		_return = JTAG_ReadChipFlashAddr(JTAGx, pVal, externAddr, highAddr, lowAddr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---读取FLASH信息
		_return = JTAG_ReadChipFlashLongAddr(JTAGx, pVal, addr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---编程FLASH信息
		_return = JTAG_WriteChipFlashPage(JTAGx, pVal, externAddr, highAddr, lowAddr, length);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---FLASH查空操作
		_return = JTAG_CheckChipFlashEmpty(JTAGx, pageByteSizeH, pageByteSizeL, pageNumH, pageNumL);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---FLASH查空操作
		_return = JTAG_CheckChipFlashEmptyLong(JTAGx, pageByteSize, pageNum);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num)
{
	UINT8_T _return = OK_0;
	//---检查编程状态
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---Eeprom查空操作
		_return = JTAG_CheckChipEepromEmpty(JTAGx, byteSize, num);
		_return = (_return == OK_0 ? OK_0 : ERROR_2);
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAGLib_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	return JTAG_SetConfigInfo(JTAGx, pVal);
}