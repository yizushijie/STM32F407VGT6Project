#ifndef W25QXX_TASK_H_
#define W25QXX_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "w25qxx_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T W25QXXTask_SPI_Init(W25QXX_HandlerType *W25Qx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T W25QXXTask_SPI_ReadRegSR1(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	UINT8_T W25QXXTask_SPI_ReadRegSR2(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WriteRegSR1(W25QXX_HandlerType *W25Qx, UINT8_T cmd, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WriteRegSR2(W25QXX_HandlerType *W25Qx, UINT8_T cmd, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EnableWrite(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_DisableWrite(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	UINT16_T W25QXXTask_SPI_ReadID(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	UINT8_T W25QXXTask_SPI_WaitBusy(W25QXX_HandlerType *W25Qx, UINT32_T timeOut, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ReadData(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ReadFast(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WritePage(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EraseChip(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EraseSector(W25QXX_HandlerType* W25Qx, UINT32_T sectorAddr, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EraseBlock32KB(W25QXX_HandlerType *W25Qx, UINT32_T blockAddr, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EraseBlock64KB(W25QXX_HandlerType *W25Qx, UINT32_T blockAddr, UINT8_T isAutoInit);
	void W25QXXTask_SPI_PowerDown(W25QXX_HandlerType* W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WakeUp(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ReadUniqueIDNumber(W25QXX_HandlerType *W25Qx, UINT8_T *pVal, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ReadJEDECID(W25QXX_HandlerType *W25Qx, UINT8_T *pVal, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EraseSecurityReg(W25QXX_HandlerType *W25Qx, UINT32_T regAddr, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ProgramSecurityReg(W25QXX_HandlerType *W25Qx, UINT32_T regAddr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_ReadSecurityReg(W25QXX_HandlerType* W25Qx, UINT32_T regAddr, UINT8_T* pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_EnableReset(W25QXX_HandlerType* W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_Reset(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WriteNoCheck(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T* pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXXTask_SPI_WriteAndCheck(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T* pVal, UINT16_T length, UINT8_T isAutoInit);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*W25QXX_TASK_H_ */
