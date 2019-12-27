#ifndef ISP_LIB_H_
#define ISP_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "isp_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T ISPLib_Init(ISP_HandlerType *ISPx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));
	UINT8_T ISPLib_DeInit(ISP_HandlerType *ISPx);
	UINT8_T ISPLib_SetProgClock(ISP_HandlerType* ISPx, UINT8_T clok);
	UINT8_T ISPLib_EnterProg(ISP_HandlerType *ISPx, UINT8_T isPollReady);
	UINT8_T ISPLib_EnterProgAndConfigInfo(ISP_HandlerType* ISPx, UINT8_T isPollReady, UINT8_T* pVal);
	UINT8_T ISPLib_ExitProg(ISP_HandlerType *ISPx);
	UINT8_T ISPLib_AddWatch(ISP_HandlerType* ISPx);
	UINT8_T ISPLib_RemoveWatch(ISP_HandlerType* ISPx);
	UINT8_T ISPLib_RefreshWatch(ISP_HandlerType* ISPx);
	UINT8_T ISPLib_SetIntervalTime(ISP_HandlerType* ISPx, UINT16_T intervalTime);
	UINT16_T ISPLib_GetIntervalTime(ISP_HandlerType* ISPx);
	UINT8_T ISPLib_ReadReady(ISP_HandlerType *ISPx);
	UINT8_T ISPLib_EraseChip(ISP_HandlerType *ISPx);
	UINT8_T ISPLib_ReadChipID(ISP_HandlerType *ISPx, UINT8_T *pVal);
	UINT8_T ISPLib_ReadChipCalibration(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T length);
	UINT8_T ISPLib_ReadChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse);
	UINT8_T ISPLib_ReadChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal);
	UINT8_T ISPLib_ReadChipRom(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T addr, UINT16_T length);
	UINT8_T ISPLib_WriteChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse);
	UINT8_T ISPLib_WriteChipLock(ISP_HandlerType *ISPx, UINT8_T val);
	UINT8_T ISPLib_ReadChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPLib_ReadChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPLib_WriteChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPLib_UpdateChipEepromPage(ISP_HandlerType* ISPx, UINT8_T* pVal);
	UINT8_T ISPLib_UpdateChipEepromAddr(ISP_HandlerType* ISPx, UINT8_T highAddr, UINT8_T lowAddr);
	UINT8_T ISPLib_UpdateChipEepromLongAddr(ISP_HandlerType* ISPx, UINT16_T addr);
	UINT8_T ISPLib_WriteChipEepromPage(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum);
	UINT8_T ISPLib_WriteChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPLib_WriteChipEepromAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPLib_WriteChipEepromLongAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length);
	UINT8_T ISPLib_WriteChipEeprom(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum);
	UINT8_T ISPLib_UpdateExternAddr(ISP_HandlerType *ISPx, UINT8_T addr);
	UINT8_T ISPLib_UpdateExternLongAddr(ISP_HandlerType *ISPx, UINT32_T addr);
	UINT8_T ISPLib_ReadChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPLib_ReadChipFlashLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT32_T addr, UINT16_T length);
	UINT8_T ISPLib_UpdateChipFlashBuffer(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T index, UINT16_T length);
	UINT8_T ISPLib_UpdateChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr);
	UINT8_T ISPLib_UpdateChipFlashLongAddr(ISP_HandlerType *ISPx, UINT32_T addr);
	UINT8_T ISPLib_WriteChipFlashPage(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T ISPLib_CheckChipFlashEmpty(ISP_HandlerType* ISPx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL);
	UINT8_T ISPLib_CheckChipFlashEmptyLong(ISP_HandlerType* ISPx, UINT16_T pageByteSize, UINT16_T pageNum);
	UINT8_T ISPLib_CheckChipEepromEmpty(ISP_HandlerType* ISPx, UINT8_T byteSize, UINT8_T num);
	UINT8_T ISPLib_SetConfigInfo(ISP_HandlerType* ISPx, UINT8_T* pVal);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ISP_LIB_H_ */
