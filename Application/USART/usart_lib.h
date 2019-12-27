#ifndef USART_LIB_H_
#define USART_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "usart_cfg.h"
	#include "crc_task.h"
	//////////////////////////////////////////////////////////////////////////////////////

	UINT8_T  USARTLib_Init(USART_HandlerType*  USARTx, UINT16_T rxSize, UINT8_T* pRxVal, UINT8_T rxCRCFlag, UINT16_T txSize, UINT8_T* pTxVal, UINT8_T txCRCFlag, UINT32_T(*pTimerTick)(void));
	UINT8_T  USARTLib_DeInit(USART_HandlerType*  USARTx);
	UINT8_T  USARTLib_ParamInit(USART_HandlerType *USARTx, UINT8_T id, UINT8_T idIndex, UINT8_T cmdIndex, UINT8_T d1Index, UINT8_T d2Index);
	UINT8_T  USARTLib_Read_Init(USART_HandlerType*  USARTx);
	UINT8_T  USARTLib_GetReadState(USART_HandlerType* USARTx);
	UINT8_T  USARTLib_ClearReadState(USART_HandlerType* USARTx);
	UINT8_T  USARTLib_GetWriteState(USART_HandlerType* USARTx);
	UINT8_T  USARTLib_ClearWriteState(USART_HandlerType* USARTx);
	UINT8_T  USARTLib_TimeOVFTask(USART_HandlerType*USARTx, UINT8_T isRx);
	UINT8_T  USARTLib_ITRead_8BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTLib_ITRead_16BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTLib_ITRead_Task(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTLib_Write_Init(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_DeviceID(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_ITWrite_TXETask(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_ITWrite_TCTask(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_RealTime_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTLib_RealTime_AddSize(USART_HandlerType*USARTx, UINT16_T val);
	UINT8_T  USARTLib_RealTime_AddCRC(USART_HandlerType*USARTx);
	UINT8_T	 USARTLib_FillMode_Init( USART_HandlerType*USARTx, UINT8_T isChildCmd);
	UINT8_T  USARTLib_FillMode_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTLib_FillMode_AddData(USART_HandlerType*USARTx, UINT8_T *pVal, UINT16_T length);
	UINT8_T	 USARTLib_FillMode_SetResultFlag(USART_HandlerType* USARTx, UINT8_T val);
	UINT8_T  USARTLib_FillMode_AddIndexW(USART_HandlerType* USARTx, UINT16_T val);
	UINT8_T  USARTLib_FillMode_WriteSTART(USART_HandlerType* USARTx, UINT8_T isNeedID);
	UINT8_T  USARTLib_CRCTask_Read(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_CRCTask_Write(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_PollMode_WriteByte(USART_HandlerType*USARTx, UINT8_T  val);
	UINT8_T  USARTLib_PollMode_WriteData(USART_HandlerType*USARTx, char *pVal);
	void	 USARTLib_PrintfClockFreq(USART_HandlerType*USARTx);
	UINT8_T  USARTLib_IT_TCTask(USART_HandlerType* USARTx);
	void	 USARTLib_IRQTask(USART_HandlerType* USARTx);
	void	 USARTLib_Read_DMA_IRQTask(USART_HandlerType* USARTx);
	void	 USARTLib_Write_DMA_IRQTask(USART_HandlerType* USARTx);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*USART_LIB_H_ */
