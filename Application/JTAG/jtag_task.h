#ifndef JTAG_TASK_H_
#define JTAG_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include"jtag_lib.h"
	#include "usart_task.h"
	#include "power_task.h"
	#include "adc_Task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===JTAG的编程的命令的基地址							
	#define CMD_JTAG_BASE_CMD							0x20
	//===打开或者关闭JTAG
	#define	CMD_JTAG_OPEN_CLOSE							(	CMD_JTAG_BASE_CMD+0		)	
	//===JTAG擦除芯片										 
	#define	CMD_JTAG_ERASE								(	CMD_JTAG_BASE_CMD+1		)	
	//===JTAG按页读取Flash									 
	#define CMD_JTAG_FLASH_PAGE_READ					(	CMD_JTAG_BASE_CMD+2		)	
	//===JTAG按页写入Flash									 
	#define CMD_JTAG_FLASH_PAGE_WRITE					(	CMD_JTAG_BASE_CMD+3		)		
	//===JTAG按页读取Eeeprom							   
	#define	CMD_JTAG_EEPROM_PAGE_READ					(	CMD_JTAG_BASE_CMD+4		)			
	//===JTAG按页写入Eeprom									 
	#define CMD_JTAG_EEPROM_PAGE_WRITE					(	CMD_JTAG_BASE_CMD+5		)			
	//===JTAG读取熔丝位和锁定位								
	#define CMD_JTAG_FUSE_LOCK_READ						(	CMD_JTAG_BASE_CMD+6		)		
	//===JTAG写入熔丝位										
	#define CMD_JTAG_FUSE_WRITE							(	CMD_JTAG_BASE_CMD+7		)		
	//===JTAG写入锁定位										
	#define CMD_JTAG_LOCK_WRITE							(	CMD_JTAG_BASE_CMD+8		)		
	//===JTAG读取芯片标识									
	#define CMD_JTAG_ID_READ							(	CMD_JTAG_BASE_CMD+9		)
	//===JTAG读取校准字										
	#define CMD_JTAG_CALIBRATIONBYTE_READ				(	CMD_JTAG_BASE_CMD+10	)	
	//===JTAG读取ROM信息								   
	#define CMD_JTAG_ROM_PAGE_READ						(	CMD_JTAG_BASE_CMD+11	)	
	//===JTAG编程时钟设置									
	#define CMD_JTAG_PROG_CLOCK_SET						(	CMD_JTAG_BASE_CMD+12	)
	//===命令结束地址
	#define CMD_JTAG_END_CMD							(   CMD_JTAG_BASE_CMD+12	)

	//===函数定义
	UINT8_T JTAGTask_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));
	UINT8_T JTAGTask_EnterProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGTask_ExitProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGTask_EnterProgAndConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T isPollReady, UINT8_T* pVal);
	UINT8_T JTAGTask_EraseChip(JTAG_HandlerType* JTAGx);
	UINT8_T JTAGTask_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGTask_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGTask_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGTask_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length);
	UINT8_T JTAGTask_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAGTask_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAGTask_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length);
	UINT8_T JTAGTask_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAGTask_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal);
	UINT8_T JTAGTask_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGTask_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length);
	UINT8_T JTAGTask_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum);
	UINT8_T JTAGTask_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGTask_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length);
	UINT8_T JTAGTask_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAGTask_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL);
	UINT8_T JTAGTask_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum);
	UINT8_T JTAGTask_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num);
	UINT8_T JTAGTask_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal);

	//===带通讯端口的函数定义
	UINT8_T JTAGTask_USARTCmd_OpenAndClose(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_EraseChip(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipID(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipCalibration(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipFuseAndLock(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_WriteChipFuse(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_WriteChipFuse(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_WriteChipLock(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipRom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_SetProgClok(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipFlash(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_WriteChipFlashPage(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ReadChipEeprom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_WriteChipEeprom(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);
	UINT8_T JTAGTask_USARTCmd_ChildTask(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx, UINT8_T isChildCmd);
	UINT8_T JTAGTask_USARTCmd_ParentTask(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx, UINT8_T isChildCmd);
	UINT8_T JTAGTask_USARTCmd_Task(JTAG_HandlerType* JTAGx, USART_HandlerType* USARTx);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*JTAG_TASK_H_ */