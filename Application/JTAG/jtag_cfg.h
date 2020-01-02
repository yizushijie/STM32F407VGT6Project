#ifndef JTAG_CFG_H_
#define JTAG_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "spi_task.h"
	#include "systick_task.h"
	#include "delay_task.h"
	#include "my_malloc.h"
	#include "hw_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===定义JTAG状态保持的时间状态
	#define JTAG_STATE_TIME_OUT_MS				500
	//===定义是否使用电平转换芯片，带OE控制端的
	#define JTAG_USE_lEVEL_SHIFT 	
	//===定义使用了高压HVSET模式
	#define JTAG_USE_HV_RESET

	//===JTAG的GPIO的操作定义
	#define	JTAG_GPIO_STATE(tp)					GPIO_GET_STATE(tp.msgPort,tp.msgBit)
	#define	JTAG_GPIO_OUT_1(tp)					GPIO_OUT_1(tp.msgPort,tp.msgBit)
	#define	JTAG_GPIO_OUT_0(tp)					GPIO_OUT_0(tp.msgPort,tp.msgBit)
	//===TCK的脉冲宽度
	#define JTAG_TCK_PULSE(tck)					(	JTAG_GPIO_OUT_1(tck->msgTCK),\
													DELAY_NOP_COUNT(4),\
													tck->msgDelayus(tck->msgPluseWidth),\
													JTAG_GPIO_OUT_0(tck->msgTCK),\
													tck->msgDelayus(tck->msgPluseWidth)\
												)
	//===TMS输出1变化一次
	#define JTAG_TMS_OUT_1(tck)					(	JTAG_GPIO_OUT_1(tck->msgTMS),\
													JTAG_GPIO_OUT_1(tck->msgTCK),\
													tck->msgDelayus(tck->msgPluseWidth),\
													JTAG_GPIO_OUT_0(tck->msgTCK),\
													tck->msgDelayus(tck->msgPluseWidth)\
												)
	//===TMS输出0变化一次
	#define JTAG_TMS_OUT_0(tck)					(	JTAG_GPIO_OUT_0(tck->msgTMS),\
													JTAG_GPIO_OUT_1(tck->msgTCK),\
													tck->msgDelayus(tck->msgPluseWidth),\
													JTAG_GPIO_OUT_0(tck->msgTCK),\
													tck->msgDelayus(tck->msgPluseWidth)\
												)
	//===TAP的运行状态
	typedef enum
	{
		TEST_LOGIC_RESET	= 0,
		RUN_TEST_IDLE		= 1,

		SELECT_DR_SCAN		= 2,
		CAPTURE_DR			= 3,
		SHIFT_DR			= 4,
		EXIT1_DR			= 5,
		PAUSE_DR			= 6,
		EXIT2_DR			= 7,
		UPDATE_DR			= 8,

		SELECT_IR_SCAN		= 12,
		CAPTURE_IR			= 13,
		SHIFT_IR			= 14,
		EXIT1_IR			= 15,
		PAUSE_IR			= 16,
		EXIT2_IR			= 17,
		UPDATE_IR			= 18,
	}TAP_STATE;
	//===JTAG开源指令
	typedef enum
	{
		EXTEST				= 0x00,
		IDCODE				= 0x01,
		SAMPLE				= 0x02,
		AVR_RESET			= 0x0C,
		BYPASS				= 0x0F,
		PROG_ENABLE			= 0x04,
		PROG_COMMANDS		= 0x05,
		PROG_PAGELOAD		= 0x06,
		PROG_PAGEREAD		= 0x07,

		PRIVATEE0			= 0x08,																																					//---强制断点
		PRIVATEE1			= 0x09,																																					//---运行
		PRIVATEE2			= 0x0A,																																					//---指令执行
		PRIVATEE3			= 0x0B,																																					//---访问OCD寄存器
	}TAP_CMD;
	//===定义结构体
	typedef struct _JTAG_HandlerType			JTAG_HandlerType;
	//===定义指针结构体
	typedef struct _JTAG_HandlerType			* pJTAG_HandlerType;
	//===结构体变量
	struct _JTAG_HandlerType
	{
		TAP_STATE			msgTapState;																																			//---当前jtag的状态
		UINT8_T				msgWaitms;																																				//---编程之后的延时函数，单位是ms
		GPIO_HandlerType	msgTDI;																																					//---TDI使用的端口
		GPIO_HandlerType	msgTDO;																																					//---TDO使用的端口
		GPIO_HandlerType	msgTCK;																																					//---TCK使用的端口
		GPIO_HandlerType	msgTMS;																																					//---TMS使用的端口
#ifdef JTAG_USE_HV_RESET
		void (*msgPortRst)(UINT8_T rstState);																																		//---高压模式操作RST端口的函数
#else
		GPIO_HandlerType	msgRST;																																					//---RST使用的端口,硬件复位端口
#endif
		
	#ifdef JTAG_USE_lEVEL_SHIFT
		GPIO_HandlerType	msgOE;																																					//---OE使用的端口，用于控制电平装换的开关
	#endif
		UINT8_T				msgJtagCmd;																																				//---是否进入JTAG指令PROG_COMMANDS，0---位使用，1---使用
		UINT8_T				msgState;																																				//---编程状态，0---空闲状态，1---编程状态
		UINT8_T				msgInit;																																				//---判断是否初始化过了 0---未初始化，1---初始化
		UINT8_T				msgEepromPageMode;																																	//---eeprom是否支持页编程模式，0---不支持，1---支持
		UINT16_T			msgFlashPerPageWordSize;																																//---Flash的每页字数
		UINT16_T			msgEerpomPerPageByteSize;																																//---Eeprom的每页字节数
		UINT16_T			msgPageWordIndex;																																		//---缓存区的序号
		UINT16_T			msgPluseWidth;																																			//---非编程状态下TCK的脉冲宽度
		UINT16_T			msgIntervalTime;																																		//---轮询时间间隔,单位是ms
		UINT32_T			msgRecordTime;																																			//---记录的时间参数
		void(*msgDelayus)(UINT32_T delay);																																			//---us延时参数
		void(*msgDelayms)(UINT32_T delay);																																			//---ms延时参数
		UINT32_T(*msgTimeTick)(void);																																				//---用于超时计数
		//SPI_HandlerType msgSPI;																																					//---使用的SPI模式
	};

	//===定义状态
	#define	JTAG_PROG_NONE						0																																	//---无编程任务
	#define JTAG_PROG_PREPARE					1																																	//---编程准备中
	#define JTAG_PROG_CMD						2																																	//---使能编程命令
	#define JTAG_PROG_PAGELOAD					3																																	//---高效编程Flash模式
	#define JTAG_PROG_PAGEREAD					4																																	//---高效读取Flash模式
	#define JTAG_PROG_READ_EEPROM				5																																	//---读取Eeprom数据
	#define JTAG_PROG_READ_FLASH				6																																	//---读取Flash数据
	#define JTAG_PROG_READ_ROM					7																																	//---读取ROM信息
	#define JTAG_PROG_WRITE_EEPROM				8																																	//---编程Eeprom数据
	#define JTAG_PROG_WRITE_FLASH				9																																	//---编程Flash数据


	//===定义RST的状态
	#define JTAG_RST_TO_HZ						0																																	//---RST处于高阻状态
	#define JTAG_RST_TO_GND						1																																	//---RST处于接地
	#define JTAG_RST_TO_VCC						2																																	//---RST接工作电压

	//===任务函数
	#define JTAG_TASK_ONE						pJtagDevice0																														//---任务1
	#define JTAG_TASK_TWO						0																																	//---任务2
	#define JTAG_TASK_THREE						0																																	//---任务3

	//===外部调用接口
	extern JTAG_HandlerType						g_JtagDevice0;
	extern pJTAG_HandlerType					pJtagDevice0;

	//===函数定义
	UINT8_T JTAG_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));	
	UINT8_T JTAG_EnterProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_ExitProg(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_RemoveWatch(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_RefreshWatch(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_AddWatch(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_SetIntervalTime(JTAG_HandlerType* JTAGx, UINT16_T intervalTime);
	UINT16_T JTAG_GetIntervalTime(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_EraseChip(JTAG_HandlerType* JTAGx);
	UINT8_T JTAG_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAG_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAG_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAG_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length);
	UINT8_T JTAG_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAG_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	UINT8_T JTAG_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length);
	UINT8_T JTAG_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T JTAG_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal);
	UINT8_T JTAG_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAG_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length);
	UINT8_T JTAG_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum);
	UINT8_T JTAG_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAG_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length);
	UINT8_T JTAG_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length);
	UINT8_T JTAG_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL);
	UINT8_T JTAG_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum);
	UINT8_T JTAG_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num);
	UINT8_T JTAG_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*JTAG_CFG_H_ */