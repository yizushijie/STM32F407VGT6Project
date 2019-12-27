#ifndef USART_TASK_H_
#define USART_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===头文件定义
	#include "usart_lib.h"
	#include "timer_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===外部调用接口
	extern UINT8_T USART1_RX_BUFFER[USART1_RX_MAX_SIZE];
	extern UINT8_T USART1_TX_BUFFER[USART1_TX_MAX_SIZE];
	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////串口1的配置参数结束//////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	//===函数定义
	UINT8_T  USARTTask_Init(USART_HandlerType*  USARTx, UINT16_T rxSize, UINT8_T* pRxVal, UINT8_T rxCRCFlag, UINT16_T txSize, UINT8_T* pTxVal, UINT8_T txCRCFlag, UINT32_T(*pTimerTick)(void));
	UINT8_T  USARTTask_DeInit(USART_HandlerType*  USARTx);
	UINT8_T  USARTTask_ParamInit(USART_HandlerType *USARTx, UINT8_T id, UINT8_T idIndex, UINT8_T cmdIndex, UINT8_T d1Index, UINT8_T d2Index);
	UINT8_T  USARTTask_Read_Init(USART_HandlerType*  USARTx);
	UINT8_T  USARTTask_GetReadState(USART_HandlerType* USARTx);
	UINT8_T  USARTTask_ClearReadState(USART_HandlerType* USARTx);
	UINT8_T  USARTTask_GetWriteState(USART_HandlerType* USARTx);
	UINT8_T  USARTTask_ClearWriteState(USART_HandlerType* USARTx);
	UINT8_T  USARTTask_TimeOVFTask(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_ITRead_8BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTTask_ITRead_16BitsTask(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTTask_ITRead_Task(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTTask_Write_Init(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_ITWrite_TXETask(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_ITWrite_TCTask(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_RealTime_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTTask_RealTime_AddSize(USART_HandlerType*USARTx, UINT16_T val);
	UINT8_T  USARTTask_RealTime_AddCRC(USART_HandlerType*USARTx);
	UINT8_T	 USARTTask_FillMode_Init( USART_HandlerType*USARTx, UINT8_T isChildCmd);
	UINT8_T  USARTTask_FillMode_AddByte(USART_HandlerType*USARTx, UINT8_T val);
	UINT8_T  USARTTask_FillMode_AddData(USART_HandlerType*USARTx, UINT8_T *pVal, UINT16_T length);
	UINT8_T	 USARTTask_FillMode_SetResultFlag(USART_HandlerType* USARTx, UINT8_T val);
	UINT8_T  USARTTask_FillMode_AddIndexW(USART_HandlerType* USARTx, UINT16_T val);
	UINT8_T  USARTTask_FillMode_WriteSTART(USART_HandlerType* USARTx, UINT8_T isNeedID);
	UINT8_T  USARTTask_CRCTask_Read(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_CRCTask_Write(USART_HandlerType*USARTx);
	UINT8_T  USARTTask_PollMode_WriteByte(USART_HandlerType*USARTx, UINT8_T  val);
	UINT8_T  USARTTask_PollMode_WriteData(USART_HandlerType*USARTx, char *pVal);
	UINT8_T  USARTTask_IT_TCTask(USART_HandlerType* USARTx);
	UINT8_T	 USARTTask_DeviceID(USART_HandlerType*USARTx);
	void	 USARTTask_IRQTask(USART_HandlerType* USARTx);
	void	 USARTTask_Read_DMA_IRQTask(USART_HandlerType* USARTx);
	void	 USARTTask_Write_DMA_IRQTask(USART_HandlerType* USARTx);
	UINT8_T  USARTTask_FuncTask(USART_HandlerType*USARTx, UINT8_T(*pFuncTask)(UINT8_T *, UINT8_T *));
	UINT8_T  USARTTask_DebugPollFuncTask(USART_HandlerType* USARTx, UINT8_T(*pFuncTask)(UINT8_T*, UINT8_T*));

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*USART_LIB_H_ */
