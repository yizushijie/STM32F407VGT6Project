#ifndef JTAG_LIB_H_
#define JTAG_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include"jtag_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===º¯Êý¶¨Òå
	UINT8_T JTAGLib_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));	
	UINT8_T JTAGLib_EnterProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGLib_ExitProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGLib_EnterProgAndConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T isPollReady, UINT8_T* pVal);
	UINT8_T JTAGLib_EraseChip(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGLib_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGLib_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGLib_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGLib_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length);
	UINT8_T JTAGLib_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAGLib_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGLib_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length);
	UINT8_T JTAGLib_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAGLib_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal);
	UINT8_T JTAGLib_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGLib_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length);
	UINT8_T JTAGLib_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum);
	UINT8_T JTAGLib_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGLib_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length);
	UINT8_T JTAGLib_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGLib_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL);
	UINT8_T JTAGLib_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum);
	UINT8_T JTAGLib_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num);
	UINT8_T JTAGLib_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*JTAG_LIB_H_ */