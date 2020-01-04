#include "at24cxx_task.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_Init(AT24CXX_HandlerType *AT24CXXx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHWI2C)
{
	return AT24CXXLib_I2C_Init(AT24CXXx, pFuncDelayus,pFuncDelayms,pFuncTimerTick, isHWI2C);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_DeInit(AT24CXX_HandlerType *AT24CXXx)
{
	return AT24CXXLib_I2C_DeInit(AT24CXXx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_WriteOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T val)
{
	return AT24CXXLib_I2C_WriteOneByte(AT24CXXx, addr, val);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_WritePageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length)
{
	return AT24CXXLib_I2C_WritePageByte(AT24CXXx, addr, pVal, length);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_WriteData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length)
{
	return AT24CXXLib_I2C_WriteData(AT24CXXx, addr, pVal, length);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_ReadOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal)
{
	return AT24CXXLib_I2C_ReadOneByte(AT24CXXx, addr, pVal);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_ReadPageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length)
{
	return AT24CXXLib_I2C_ReadPageByte(AT24CXXx, addr, pVal, length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_ReadData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length)
{
	return AT24CXXLib_I2C_ReadData(AT24CXXx, addr, pVal, length);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T AT24CXXTask_I2C_EraseChip(AT24CXX_HandlerType *AT24CXXx)
{
	return AT24CXXLib_I2C_EraseChip(AT24CXXx);
}