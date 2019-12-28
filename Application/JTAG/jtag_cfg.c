#include"jtag_cfg.h"

//===全局变量定义
JTAG_HandlerType	g_JtagDevice0 = { 0 };
//===全局指针变量
pJTAG_HandlerType	pJtagDevice0 = &g_JtagDevice0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_Device0_RST(UINT8_T rstState)
{
	if (rstState == JTAG_RST_TO_GND)
	{
		RST_TO_GND;
	}
	else if (rstState == JTAG_RST_TO_VCC)
	{
		RST_TO_VCC;
	}
	else
	{
		RST_TO_HZ;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_Device1_RST(UINT8_T rstState)
{
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_Device2_RST(UINT8_T rstState)
{
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_Device0_Init(JTAG_HandlerType* JTAGx)
{
	//---默认状态是test_logic_reset
	JTAGx->msgTapState = TEST_LOGIC_RESET;
#ifdef JTAG_USE_HV_RESET
	JTAGx->msgPortRst = JTAG_Device0_RST;
	//---TCK->PD1---host->device
	JTAGx->msgTCK.msgGPIOPort = GPIOD;
	JTAGx->msgTCK.msgGPIOBit = LL_GPIO_PIN_1;
	//---TMS->PD0---host->device
	JTAGx->msgTMS.msgGPIOPort = GPIOD;
	JTAGx->msgTMS.msgGPIOBit = LL_GPIO_PIN_0;
	//---TDI->PD3---host->device
	JTAGx->msgTDI.msgGPIOPort = GPIOD;
	JTAGx->msgTDI.msgGPIOBit = LL_GPIO_PIN_3;
	//---TDO->PD2---device->host
	JTAGx->msgTDO.msgGPIOPort = GPIOA;
	JTAGx->msgTDO.msgGPIOBit = LL_GPIO_PIN_2;
#elif
	//---TCK->PB3---host->device
	JTAGx->msgTCK.msgGPIOPort = GPIOB;
	JTAGx->msgTCK.msgGPIOBit = LL_GPIO_PIN_3;
	//---TMS->PC5---host->device
	JTAGx->msgTMS.msgGPIOPort = GPIOC;
	JTAGx->msgTMS.msgGPIOBit = LL_GPIO_PIN_5;
	//---RST->PC4---host->device
	JTAGx->msgRST.msgGPIOPort = GPIOC;
	JTAGx->msgRST.msgGPIOBit = LL_GPIO_PIN_4;
	//---TDI->PA7---host->device
	JTAGx->msgTDI.msgGPIOPort = GPIOA;
	JTAGx->msgTDI.msgGPIOBit = LL_GPIO_PIN_7;
	//---TDO->PA6---device->host
	JTAGx->msgTDO.msgGPIOPort = GPIOA;
	JTAGx->msgTDO.msgGPIOBit = LL_GPIO_PIN_6;
#endif
	//---OE使用的端口
#ifdef JTAG_USE_lEVEL_SHIFT
#ifdef JTAG_USE_HV_RESET
	//---OE->PD13---控制电平装换的使能
	JTAGx->msgOE.msgGPIOPort = GPIOD;
	JTAGx->msgOE.msgGPIOBit = LL_GPIO_PIN_11;
#else
	 //---OE->PD13---控制电平装换的使能
	JTAGx->msgOE.msgGPIOPort = GPIOD;
	JTAGx->msgOE.msgGPIOBit = LL_GPIO_PIN_13;
#endif
#endif
	JTAGx->msgPluseWidth = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_Device1_Init(JTAG_HandlerType* JTAGx)
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
UINT8_T JTAG_Device2_Init(JTAG_HandlerType* JTAGx)
{
	JTAGx->msgTapState = TEST_LOGIC_RESET;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_GPIO_Init(JTAG_HandlerType* JTAGx)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(JTAGx->msgTDI.msgGPIOPort, 1);
	GPIOTask_Clock(JTAGx->msgTDO.msgGPIOPort, 1);
	GPIOTask_Clock(JTAGx->msgTMS.msgGPIOPort, 1);
	GPIOTask_Clock(JTAGx->msgTCK.msgGPIOPort, 1);
	//---JTAG的OE使能端
#ifdef JTAG_USE_lEVEL_SHIFT
	GPIOTask_Clock(JTAGx->msgOE.msgGPIOPort, 1);
#endif
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;														//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;											//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;											//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	//---TDI---输出为低
	GPIO_InitStruct.Pin = JTAGx->msgTDI.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTDI.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_0(JTAGx->msgTDI);
	//---TCK---输出为低
	GPIO_InitStruct.Pin = JTAGx->msgTCK.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTCK.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_0(JTAGx->msgTCK);
	//---TMS---输出为高
	GPIO_InitStruct.Pin = JTAGx->msgTMS.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTMS.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTMS);
#ifndef JTAG_USE_HV_RESET
	//---使能端口时钟
	GPIOTask_Clock(JTAGx->msgRST.msgGPIOPort, 1);
	//---RST---输出为高
	GPIO_InitStruct.Pin = JTAGx->msgRST.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgRST.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgRST);
#endif
#ifdef JTAG_USE_lEVEL_SHIFT
	//---OE---输出为低，低有效
	GPIO_InitStruct.Pin = JTAGx->msgOE.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgOE.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_0(JTAGx->msgOE);
#endif
	//---TDO---输入，上拉使能
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;														//---配置状态为输入模式
	GPIO_InitStruct.Pin = JTAGx->msgTDO.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTDO.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTDO);
	//---端口初始化了
	JTAGx->msgInit = 1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_GPIO_DeInit(JTAG_HandlerType* JTAGx)
{
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;														//---配置状态为输入模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;												//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;											//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	//---TDI---输入上拉
	GPIO_InitStruct.Pin = JTAGx->msgTDI.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTDI.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTDI);
	//---TCK---输入上拉
	GPIO_InitStruct.Pin = JTAGx->msgTCK.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTCK.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTCK);
	//---TMS---输入上拉
	GPIO_InitStruct.Pin = JTAGx->msgTMS.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTMS.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTMS);
	//---RST端口配置
#ifdef JTAG_USE_HV_RESET
	//---设置为高阻态
	JTAGx->msgPortRst(JTAG_RST_TO_HZ);
#else
	//---RST---输入上拉
	GPIO_InitStruct.Pin = JTAGx->msgRST.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgRST.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgRST);
#endif
#ifdef JTAG_USE_lEVEL_SHIFT
	//---OE---输出为低，低有效
	JTAG_GPIO_OUT_0(JTAGx->msgOE);
#endif
	//---TDO---输入上拉
	GPIO_InitStruct.Pin = JTAGx->msgTDO.msgGPIOBit;
	LL_GPIO_Init(JTAGx->msgTDO.msgGPIOPort, &GPIO_InitStruct);
	JTAG_GPIO_OUT_1(JTAGx->msgTDO);
	//---端口未初始化了
	JTAGx->msgInit = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_Init(JTAG_HandlerType* JTAGx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	//---使用的资源
	if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_ONE))
	{
		JTAG_Device0_Init(JTAGx);
	}
	else if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_TWO))
	{
		JTAG_Device1_Init(JTAGx);
	}
	else if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_THREE))
	{
		JTAG_Device2_Init(JTAGx);
	}
	else
	{
		return ERROR_1;
	}
	JTAG_GPIO_Init(JTAGx);
	//---注册ms的延时函数
	if (pFuncDelayms != NULL)
	{
		JTAGx->msgDelayms = pFuncDelayms;
	}
	else
	{
		JTAGx->msgDelayms = DelayTask_us;
	}
	//---注册us延时函数
	if (pFuncDelayus != NULL)
	{
		JTAGx->msgDelayus = pFuncDelayus;
	}
	else
	{
		JTAGx->msgDelayus = DelayTask_us;
	}
	//---注册滴答函数
	if (pFuncTimerTick != NULL)
	{
		JTAGx->msgTimeTick = pFuncTimerTick;
	}
	else
	{
		JTAGx->msgTimeTick = SysTickTask_GetTick;;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：TMS为高，TCK给5个脉冲，能够直接回到TestlogicReset状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunTestlogicReset(JTAG_HandlerType* JTAGx)
{
	UINT8_T i = 0;
	JTAG_GPIO_OUT_1(JTAGx->msgTMS);
	for (i = 0; i < 5; i++)
	{
		JTAG_TCK_PULSE(JTAGx);
	}
	JTAGx->msgTapState = TEST_LOGIC_RESET;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在空闲状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunTestIdle(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			break;
		case SELECT_DR_SCAN:
			//---capture dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			break;
	}
	//---run_test_idle
	JTAG_TMS_OUT_0(JTAGx);
	JTAGx->msgTapState = RUN_TEST_IDLE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在SelectDRScan状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunSelectDRScan(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			break;
		case SELECT_DR_SCAN:
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);;
			break;
	}
	//---校验是否保持时钟不变
	if (JTAGx->msgTapState != SELECT_DR_SCAN)
	{
		//---select_dr_scan
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = SELECT_DR_SCAN;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在Capture DR
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunCaptureDR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			break;
		case CAPTURE_DR:
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != CAPTURE_DR)
	{
		//---capture_dr
		JTAG_TMS_OUT_0(JTAGx);
	}
	JTAGx->msgTapState = CAPTURE_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在ShiftDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunShiftDR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_DR:
			break;
		case SHIFT_DR:
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_DR:
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	JTAGx->msgTapState = SHIFT_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在Exit1DR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunExit1DR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_DR:
			break;
		case SHIFT_DR:
			break;
		case EXIT1_DR:
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_DR:
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != EXIT1_DR)
	{
		//---exit1_dr
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = EXIT1_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在PauseDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunPauseDR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			break;
		case PAUSE_DR:
			break;
		case EXIT2_DR:
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != PAUSE_DR)
	{
		//---pause_dr
		JTAG_TMS_OUT_0(JTAGx);
	}
	JTAGx->msgTapState = PAUSE_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在Exit2DR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunExit2DR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case EXIT1_DR:
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case PAUSE_DR:
			break;
		case EXIT2_DR:
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_GPIO_OUT_1(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---capture_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_GPIO_OUT_0(JTAGx->msgTMS);
			JTAG_TCK_PULSE(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != EXIT2_DR)
	{
		//---exit2_dr
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = EXIT2_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在UpdateDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunUpdateDR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			break;
		case UPDATE_DR:
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_dr
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != UPDATE_DR)
	{
		//---update_dr
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = UPDATE_DR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在SelectiRScan状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunSelectIRScan(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			break;
		case SELECT_DR_SCAN:
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			break;
		case SELECT_IR_SCAN:
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	//---select_dr_scan
	JTAG_TMS_OUT_1(JTAGx);
	//---校验是否保持时钟不变
	if (JTAGx->msgTapState != SELECT_IR_SCAN)
	{
		//---select_ir_scan
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = SELECT_IR_SCAN;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在CaptureIR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunCaptureIR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_IR_SCAN:
			break;
		case CAPTURE_IR:
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---update_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != CAPTURE_IR)
	{
		//---capture_ir
		JTAG_TMS_OUT_0(JTAGx);
	}
	JTAGx->msgTapState = CAPTURE_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在ShiftDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunShiftIR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_IR:
			break;
		case SHIFT_IR:
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_IR:
			//---pasuse_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	JTAGx->msgTapState = SHIFT_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在PauseDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunExit1IR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_IR:
			break;
		case SHIFT_IR:
			break;
		case EXIT1_IR:
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_IR:
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != EXIT1_IR)
	{
		//---exit1_ir
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = EXIT1_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在PauseDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunPauseIR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			break;
		case PAUSE_IR:
			break;
		case EXIT2_IR:
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---shift_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	//---pause_ir
	JTAG_TMS_OUT_0(JTAGx);
	JTAGx->msgTapState = PAUSE_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在PauseDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunExit2IR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case EXIT1_IR:
			//---pasuse_ir
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			break;
		case UPDATE_IR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			//---pause_dr
			JTAG_TMS_OUT_0(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != EXIT2_IR)
	{
		//---exit2_ir
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = EXIT2_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：运行在PauseDR状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RunUpdateIR(JTAG_HandlerType* JTAGx)
{
	switch (JTAGx->msgTapState)
	{
		case RUN_TEST_IDLE:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_DR_SCAN:
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_DR:
			//---exit1_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case PAUSE_DR:
			//---exit2_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_DR:
			//---update_dr
			JTAG_TMS_OUT_1(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case UPDATE_DR:
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SELECT_IR_SCAN:
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case CAPTURE_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case SHIFT_IR:
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT1_IR:
			break;
		case PAUSE_IR:
			//---exit2_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
		case EXIT2_IR:
			break;
		case UPDATE_IR:
			break;
		case TEST_LOGIC_RESET:
		default:
			JTAG_RunTestlogicReset(JTAGx);
			//---run_test_idle
			JTAG_TMS_OUT_0(JTAGx);
			//---select_dr_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---select_ir_scan
			JTAG_TMS_OUT_1(JTAGx);
			//---capture_ir
			JTAG_TMS_OUT_0(JTAGx);
			//---exit1_ir
			JTAG_TMS_OUT_1(JTAGx);
			break;
	}
	if (JTAGx->msgTapState != UPDATE_IR)
	{
		//---update_ir
		JTAG_TMS_OUT_1(JTAGx);
	}
	JTAGx->msgTapState = UPDATE_IR;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送IR命令
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ShiftIR_BIT(JTAG_HandlerType* JTAGx, UINT8_T irCmd, UINT8_T bitCount, UINT8_T isRunTestIdle)
{
	UINT8_T i = 0;
	UINT8_T _return = OK_0;
	//---capture_ir
	JTAG_RunCaptureIR(JTAGx);
	//---发送命令
	for (i = 0; i < bitCount; i++)
	{
		//---时钟脉冲
		JTAG_TCK_PULSE(JTAGx);
		//---发送数据
		((irCmd & 0x01) != 0) ? JTAG_GPIO_OUT_1(JTAGx->msgTDI) : JTAG_GPIO_OUT_0(JTAGx->msgTDI);
		//---移位数据
		irCmd >>= 1;
		//---读取结果
		_return |= ((JTAG_GPIO_STATE(JTAGx->msgTDO) != 0) ? (1 << i) : 0);
	}
	JTAGx->msgTapState = SHIFT_IR;
	//---校验是否运行在空闲状态
	if (isRunTestIdle)
	{
		//---run_test_idle
		JTAG_RunTestIdle(JTAGx);
	}
	//---select_dr_scan
	JTAG_RunSelectDRScan(JTAGx);
	return ((_return == 0x01) ? OK_0 : ERROR_1);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T JTAG_ShiftDR_BIT(JTAG_HandlerType* JTAGx, UINT32_T drCmd, UINT8_T bitCount, UINT8_T isRunTestIdle)
{
	UINT32_T i = 0;
	UINT32_T _return = 0;
	//---capture_dr捕获数据寄存器状态
	JTAG_RunCaptureDR(JTAGx);
	for (i = 0; i < bitCount; i++)
	{
		//---时钟脉冲
		JTAG_TCK_PULSE(JTAGx);
		//---发送数据
		((drCmd & 0x01) != 0) ? JTAG_GPIO_OUT_1(JTAGx->msgTDI) : JTAG_GPIO_OUT_0(JTAGx->msgTDI);
		//---移位数据
		drCmd >>= 1;
		//---读取结果
		_return |= ((JTAG_GPIO_STATE(JTAGx->msgTDO) != 0) ? (1 << i) : 0);
	}
	JTAGx->msgTapState = SHIFT_DR;
	//---校验是否运行在空闲状态
	if (isRunTestIdle != 0)
	{
		//---run_test_idle
		JTAG_RunTestIdle(JTAGx);
	}
	//---select_dr_scan
	JTAG_RunSelectDRScan(JTAGx);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：猜测是清楚TAP的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_TAPReset(JTAG_HandlerType* JTAGx, UINT8_T bit)
{
	JTAG_ShiftIR_BIT(JTAGx, AVR_RESET, 4, 1);
	JTAG_ShiftDR_BIT(JTAGx, bit, 1, 1);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：猜测是清楚TAP的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_TAPClear(JTAG_HandlerType* JTAGx)
{
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PRIVATEE3, 4, 0);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---读写数据
	JTAG_ShiftDR_BIT(JTAGx, 0x0D, 5, 0);
	//---读写数据
	JTAG_ShiftDR_BIT(JTAGx, 0, 16, 0);
	//---设置Reset 寄存器
	JTAG_TAPReset(JTAGx, 1);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：猜测是清楚TAP的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_TAPPreEnter(JTAG_HandlerType* JTAGx)
{
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_ENABLE, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---读取数据
	return ((JTAG_ShiftDR_BIT(JTAGx, 0xA370, 16, 1) == 0) ? OK_0 : ERROR_1);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_TapNoOperationCommand(JTAG_HandlerType* JTAGx)
{
	UINT16_T _return = OK_0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---发送空命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2300, 15, 1);
	//---发送空命令
	_return = JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	//---校验数据
	if (_return != 0x2300)
	{
		return ERROR_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：猜测是清楚TAP的状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_TAPExit(JTAG_HandlerType* JTAGx)
{
	UINT16_T _return = JTAG_TapNoOperationCommand(JTAGx);
	//---校验操作
	if (_return != OK_0)
	{
		return ERROR_1;
	}
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_ENABLE, 4, 0);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	_return = JTAG_ShiftDR_BIT(JTAGx, 0x0000, 16, 0);
	//---清零Reset 寄存器
	JTAG_TAPReset(JTAGx, 0);
	//---设置端口未输入模式
	JTAG_GPIO_DeInit(JTAGx);
	//---校验数据
	if (_return != 0xA370)
	{
		return ERROR_2;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_EnterProg(JTAG_HandlerType* JTAGx)
{
	if (JTAGx->msgInit == 0)
	{
		//---初始化端口
		JTAG_GPIO_Init(JTAGx);
	}
	JTAG_TAPClear(JTAGx);
	UINT8_T _return = JTAG_TAPPreEnter(JTAGx);
	//---校验进入结果
	if (_return == OK_0)
	{
		JTAGx->msgState = JTAG_PROG_PREPARE;
	}
	else
	{
		JTAGx->msgState = JTAG_PROG_NONE;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ExitProg(JTAG_HandlerType* JTAGx)
{
	//---清除Eeprom页编程模式
	JTAGx->msgEepromIsPageMode = 0;
	//---清除数据缓存区的序号
	JTAGx->msgPageWordIndex = 0;
	//---编程状态为空闲模式
	JTAGx->msgState = JTAG_PROG_NONE;
	//---置位标签
	JTAGx->msgJtagCmd = 0;
	//---移除注册的监控函数
	JTAG_RemoveWatch(JTAGx);
	//---执行退出操作
	return JTAG_TAPExit(JTAGx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_WatchTask(JTAG_HandlerType* JTAGx)
{
	UINT32_T nowTime = 0;
	UINT32_T cnt = 0;
	if (JTAGx->msgState != JTAG_PROG_NONE)
	{
		//---获取当前时间节拍
		nowTime = JTAGx->msgTimeTick();
		//---计算时间间隔
		if (JTAGx->msgRecordTime > nowTime)
		{
			cnt = (0xFFFFFFFF - JTAGx->msgRecordTime + nowTime);
		}
		else
		{
			cnt = nowTime - JTAGx->msgRecordTime;
		}
		//---检查是否发生超时事件
		if (cnt > JTAGx->msgIntervalTime)
		{
			JTAG_ExitProg(JTAGx);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备1的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_AddWatchDevice0(void)
{
	JTAG_WatchTask(JTAG_TASK_ONE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备2的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_AddWatchDevice1(void)
{
	JTAG_WatchTask(JTAG_TASK_TWO);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备3的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void JTAG_AddWatchDevice2(void)
{
	JTAG_WatchTask(JTAG_TASK_THREE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RemoveWatch(JTAG_HandlerType* JTAGx)
{
	//---使用的JTAG的端口
	if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_ONE))
	{
		SysTick_DeleteTickTask(JTAG_AddWatchDevice0);
	}
	else if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_TWO))
	{
		SysTick_DeleteTickTask(JTAG_AddWatchDevice1);
	}
	else if ((JTAGx != NULL) && (JTAGx == JTAG_TASK_THREE))
	{
		SysTick_DeleteTickTask(JTAG_AddWatchDevice2);
	}
	else
	{
		return ERROR_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_RefreshWatch(JTAG_HandlerType* JTAGx)
{
	//---配置轮训间隔为最大值，单位是ms
	JTAGx->msgIntervalTime = JTAG_STATE_TIME_OUT_MS;
	//---刷新纪录时间
	JTAGx->msgRecordTime = JTAGx->msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_AddWatch(JTAG_HandlerType* JTAGx)
{
	UINT8_T _return = OK_0;
	if (JTAGx != NULL)
	{
		//---刷新时间
		_return = JTAG_RefreshWatch(JTAGx);
		//---使用的JTAG的端口,并注册相应的任务函数
		if (JTAGx == JTAG_TASK_ONE)
		{
			SysTickTask_CreateTickTask(JTAG_AddWatchDevice0);
		}
		else if (JTAGx == JTAG_TASK_TWO)
		{
			SysTickTask_CreateTickTask(JTAG_AddWatchDevice1);
		}
		else if (JTAGx == JTAG_TASK_THREE)
		{
			SysTickTask_CreateTickTask(JTAG_AddWatchDevice2);
		}
		else
		{
			_return = ERROR_1;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_SetIntervalTime(JTAG_HandlerType* JTAGx, UINT16_T intervalTime)
{
	//---配置轮训间隔时间，单位是ms
	JTAGx->msgIntervalTime = intervalTime;
	//---刷新纪录时间
	JTAGx->msgRecordTime = JTAGx->msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T JTAG_GetIntervalTime(JTAG_HandlerType* JTAGx)
{
	return JTAGx->msgIntervalTime;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：等待轮训完成完成
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WaitPollChipComplete(JTAG_HandlerType* JTAGx, UINT16_T cmd)
{
	UINT8_T _return = OK_0;
	UINT32_T tempID = 0;
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;
	if (JTAGx->msgTimeTick != NULL)
	{
		//nowTime = JTAGx->msgSPI.msgFuncTick();
		oldTime = JTAGx->msgTimeTick();
	}
	while (1)
	{
		tempID = JTAG_ShiftDR_BIT(JTAGx, cmd, 15, 0);
		//---校验返回值
		if ((tempID & 0x0200) != 0)
		{
			_return = OK_0;
			break;
		}
		else
		{
			if (JTAGx->msgTimeTick != NULL)
			{
				//---当前时间
				nowTime = JTAGx->msgTimeTick();
				//---判断滴答定时是否发生溢出操作
				if (nowTime < oldTime)
				{
					cnt = (0xFFFFFFFF - oldTime + nowTime);
				}
				else
				{
					cnt = nowTime - oldTime;
				}
				//---判断是否超时
				if (cnt > 100)
				{
					//---发送发生超时错误
					_return = ERROR_2;
					break;
				}
			}
			else
			{
				nowTime++;
				if (nowTime > 100000)
				{
					//---发送发生超时错误
					_return = ERROR_3;
					break;
				}
			}
		}
		WDT_RESET();
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：擦除设备
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_EraseChip(JTAG_HandlerType* JTAGx)
{
	UINT8_T i = 0;
	//---使能JTAG指令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载擦除命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2380, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3180, 15, 0);
	JTAG_ShiftDR_BIT(JTAGx, 0x3380, 15, 0);
	JTAG_ShiftDR_BIT(JTAGx, 0x3380, 15, 0);
	//---等待擦除完成
	return JTAG_WaitPollChipComplete(JTAGx, 0x3380);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取JTAGID信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadIDChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T i = 0;
	UINT32_T tempID = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, IDCODE, 4, 0);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---读取数据
	tempID = JTAG_ShiftDR_BIT(JTAGx, 0, 32, 1);
	//---忽略版本信息
	tempID &= 0x0FFFFFFF;
	//---校验缓存区
	if (pVal != NULL)
	{
		for (i = 0; i < 4; i++)
		{
			pVal[i] = (UINT8_T)tempID;
			tempID >>= 8;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取JTAGID信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipID(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT8_T i = 0;
	UINT32_T tempID = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载读取命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2308, 15, 1);
	//---轮训读取ChipID
	for (i = 0; i < 3; i++)
	{
		//---加载地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 + i, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3200, 15, 0);
		//---读取结果
		tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 0);
		*(pVal++) = (UINT8_T)(tempID);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取芯片信息，包含ChipID和JtagID信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChip(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	JTAG_ReadIDChip(JTAGx, pVal);
	return JTAG_ReadChipID(JTAGx, pVal + 4);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取校准字信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipCalibration(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T length)
{
	UINT8_T i = 0;
	UINT32_T tempID = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载读取命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2308, 15, 1);
	//---读取指定大小的数据
	for (i = 0; i < length; i++)
	{
		//---加载地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 + i, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3600, 15, 0);
		//---读取结果
		tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 0);
		*(pVal++) = (UINT8_T)(tempID);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取熔丝位信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T i = 0;
	UINT32_T tempID = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载读取命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2304, 15, 1);
	//---读取熔丝位低位值
	JTAG_ShiftDR_BIT(JTAGx, 0x3200, 15, 1);
	//---获取熔丝位低位值
	tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	*(pVal++) = (UINT8_T)tempID;
	//---读取熔丝位高位值
	JTAG_ShiftDR_BIT(JTAGx, 0x3E00, 15, 1);
	//---获取熔丝位高位值
	tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3F00, 15, 1);
	*(pVal++) = (UINT8_T)tempID;
	//---校验拓展位
	if (isNeedExternFuse != 0)
	{
		//---读取熔丝位拓展位值
		JTAG_ShiftDR_BIT(JTAGx, 0x3A00, 15, 1);
		//---获取熔丝位拓展位值
		tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3B00, 15, 1);
		*(pVal++) = (UINT8_T)tempID;
	}
	else
	{
		*pVal = 0xFF;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取加密位信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipLock(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	UINT32_T tempID = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载读取命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2304, 15, 1);
	//---读取加密位值
	JTAG_ShiftDR_BIT(JTAGx, 0x3600, 15, 1);
	//---获取加密位值
	tempID = JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	*(pVal++) = ((UINT8_T)tempID) | 0xFC;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取ROM页信息
//////输入参数:	pVal---数据缓存区
//////			addr---数据地址，地址是字地址
//////			length---读取数据的长度，数据长度是字节长度，内部处理成字长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipRom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T addr, UINT16_T length)
{
	UINT8_T i = 0;
	UINT32_T tempROM = 0;
	//---判断数据长度是否是偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_2;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---判断是否已近进入编程命令
	if ((JTAGx->msgJtagCmd == 0) || (JTAGx->msgState != JTAG_PROG_READ_ROM))
	{
		//---发送命令
		JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
		//---准备发送数据
		JTAG_RunCaptureDR(JTAGx);
		//---加载读取命令
		JTAG_ShiftDR_BIT(JTAGx, 0x2308, 15, 1);
		//---置位标签
		JTAGx->msgJtagCmd = 1;
		JTAGx->msgState = JTAG_PROG_READ_ROM;
	}
	//---读取ROM页信息
	for (i = 0; i < length; i++)
	{
		//---读取低字节数据,加载地址
		JTAG_ShiftDR_BIT(JTAGx, (0x0300 | (i + addr)), 15, 1);
		//---读取结果
		JTAG_ShiftDR_BIT(JTAGx, 0x3200, 15, 0);
		//---获取低字节数据
		tempROM = JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 0);
		*(pVal++) = (UINT8_T)(tempROM);
		//---读取高字节数据，加载地址
		JTAG_ShiftDR_BIT(JTAGx, (0x0300 | (i + addr)), 15, 1);
		//---读取结果
		JTAG_ShiftDR_BIT(JTAGx, 0x3600, 15, 0);
		//---获取高字节数据
		tempROM = JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 0);
		*(pVal++) = (UINT8_T)(tempROM);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程熔丝位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WriteChipFuse(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = OK_0;
	UINT32_T tempFuse = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载编程熔丝位命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2340, 15, 1);
	//---加载低位数据
	JTAG_ShiftDR_BIT(JTAGx, 0x1300 | *(pVal++), 15, 1);
	//---写熔丝位低位
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3100, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	//---等待编程低位熔丝位结束
	_return = JTAG_WaitPollChipComplete(JTAGx, 0x3300);
	if (_return != OK_0)
	{
		return ERROR_1;
	}
	//---加载高位数据
	JTAG_ShiftDR_BIT(JTAGx, 0x1300 | *(pVal++), 15, 1);
	//---写熔丝位高位
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3500, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	//---等待编程高位熔丝位结束
	_return = JTAG_WaitPollChipComplete(JTAGx, 0x3700);
	if (_return != OK_0)
	{
		return ERROR_2;
	}
	//---校验编程拓展熔丝位
	if (isNeedExternFuse)
	{
		//---加载拓展位数据
		JTAG_ShiftDR_BIT(JTAGx, 0x1300 | *pVal, 15, 1);
		//---写熔丝位高位
		JTAG_ShiftDR_BIT(JTAGx, 0x3B00, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3900, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3B00, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3B00, 15, 1);
		//---等待编程拓展位熔丝位结束
		_return = JTAG_WaitPollChipComplete(JTAGx, 0x3700);
		if (_return != OK_0)
		{
			return ERROR_3;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程加密位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WriteChipLock(JTAG_HandlerType* JTAGx, UINT8_T lockVal)
{
	UINT32_T tempLock = 0;
	//---发送命令
	JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
	//---准备发送数据
	JTAG_RunCaptureDR(JTAGx);
	//---加载编程加密位命令
	JTAG_ShiftDR_BIT(JTAGx, 0x2320, 15, 1);
	//---加载加密位数据
	JTAG_ShiftDR_BIT(JTAGx, 0x13C0 | lockVal, 15, 1);
	//---写加密位
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3100, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	//---等待编程加密位结束
	return ((JTAG_WaitPollChipComplete(JTAGx, 0x3300) == OK_0) ? OK_0 : ERROR_5);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取EEPROM
//////输入参数:  pVal	---数据缓存区
//////			highAddr---数据地址的高字节，地址是字节地址
//////			lowAddr ---数据地址的低字节，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipEepromAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT32_T tempeEPROM = 0;
	UINT16_T i = 0;
	//---校验是否进入编程命令
	if ((JTAGx->msgJtagCmd == 0) || (JTAGx->msgState != JTAG_PROG_READ_EEPROM))
	{
		//---发送命令
		JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
		//---准备发送数据
		JTAG_RunCaptureDR(JTAGx);
		//---加载读取命令
		JTAG_ShiftDR_BIT(JTAGx, 0x2303, 15, 1);
		JTAGx->msgJtagCmd = 1;
		JTAGx->msgState = JTAG_PROG_READ_EEPROM;
	}
	//---读取EEPROM信息
	for (i = 0; i < length; i++)
	{
		//---加载高地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0700 | highAddr, 15, 1);
		//---加载低地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 | lowAddr, 15, 1);
		//---准备读取数据
		JTAG_ShiftDR_BIT(JTAGx, 0x3300 | lowAddr, 15, 0);
		JTAG_ShiftDR_BIT(JTAGx, 0x3200, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 0);
		*(pVal++) = (UINT8_T)(tempeEPROM);
		//---低位地址偏移
		lowAddr++;
		//---校验高位地址偏移
		if (lowAddr == 0)
		{
			//---高位地址偏移
			highAddr++;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取EEPROM
//////输入参数:  pVal	---数据缓存区
//////			addr	---数据地址，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipEepromLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT16_T addr, UINT16_T length)
{
	return JTAG_ReadChipEepromAddr(JTAGx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页编程模式，更新数据到EEPROM的编程缓存区
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_UpdateChipEepromPage(JTAG_HandlerType* JTAGx, UINT8_T highAddr, UINT8_T lowAddr, UINT8_T* pVal)
{
	UINT32_T tempVal = 0;
	UINT8_T i = 0;
	//---校验是否进入Flash写入模式
	if ((JTAGx->msgJtagCmd == 0) || (JTAGx->msgState != JTAG_PROG_WRITE_EEPROM))
	{
		//---发送命令
		JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
		//---准备发送数据
		JTAG_RunCaptureDR(JTAGx);
		//---编程EEPROM
		JTAG_ShiftDR_BIT(JTAGx, 0x2311, 15, 1);
		JTAGx->msgJtagCmd = 1;
		JTAGx->msgState = JTAG_PROG_WRITE_EEPROM;
	}
	//---加载高位地址
	JTAG_ShiftDR_BIT(JTAGx, 0x0700 | highAddr, 15, 1);
	//---填充缓存区
	for (i = 0; i < JTAGx->msgEerpomPerPageByteSize; i++)
	{
		//---加载低位地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 | lowAddr, 15, 1);
		//---加载数据
		JTAG_ShiftDR_BIT(JTAGx, 0x1300 | *(pVal++), 15, 1);
		//---锁存数据
		JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x7700, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
		lowAddr++;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页编程模式，执行页写入操作
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_UpdateChipEeprom(JTAG_HandlerType* JTAGx)
{
	//---使能页写入
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3100, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3300, 15, 1);
	//---等待页写入完成
	return JTAG_WaitPollChipComplete(JTAGx, 0x3300);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页编程模式，进行EEPROM的编程
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WriteChipEepromPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	UINT8_T _return = OK_0;
	UINT32_T pageAddr = 0;
	UINT16_T i = 0;
	//---检查当前编程模式
	if (JTAGx->msgState == JTAG_PROG_NONE)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---计算编程的页数
		pageNum /= (JTAGx->msgEerpomPerPageByteSize);
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---计算地址，对于Eeprom来说，字地址也是字节地址
		pageAddr = highAddr;
		pageAddr = (pageAddr << 8) + lowAddr;
		//---逐页编程Eeprom数据
		for (i = 0; i < pageNum; i++)
		{
			//---填充数据缓存
			_return = JTAG_UpdateChipEepromPage(JTAGx, (UINT8_T)(pageAddr >> 8), (UINT8_T)(pageAddr), pVal);
			//---换算返回结果
			_return = (_return == OK_0 ? OK_0 : ERROR_2);
			//---执行页写入操作
			_return = JTAG_UpdateChipEeprom(JTAGx);
			//---换算返回结果
			_return = (_return == OK_0 ? OK_0 : ERROR_3);
			//---校验页编程的结果
			if (_return != OK_0)
			{
				//---错误，退出编程
				_return = ERROR_4;
				break;
			}
			else
			{
				//---计算地址偏移
				pageAddr += JTAGx->msgEerpomPerPageByteSize;
				//---数据地址偏移
				pVal += JTAGx->msgEerpomPerPageByteSize;
			}
		}
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	else
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：JTAG编程EEPROM，JTAG只支持页编程，不支持字节编程
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WriteChipEeprom(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	//---编程指定位置的Eeprom数据,编程模式页模式
	return JTAG_WriteChipEepromPage(JTAGx, pVal, highAddr, lowAddr, pageNum);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：按地址读取Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipFlashAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT32_T tempeFlash = 0;
	UINT16_T i = 0;
	if ((JTAGx->msgJtagCmd == 0) || (JTAGx->msgState != JTAG_PROG_READ_FLASH))
	{
		//---发送命令
		JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
		//---准备发送数据
		JTAG_RunCaptureDR(JTAGx);
		//---编程熔丝位
		JTAG_ShiftDR_BIT(JTAGx, 0x2302, 15, 1);
		JTAGx->msgJtagCmd = 1;
		JTAGx->msgState = JTAG_PROG_READ_FLASH;
	}
	//---判断是否加载拓展位地址
	if (externAddr != 0)
	{
		//---加载拓展位地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0B00 | externAddr, 15, 1);
	}
	//---读取ROM页信息
	for (i = 0; i < length; i++)
	{
		//---加载高地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0700 | highAddr, 15, 1);
		//---加载低地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 | lowAddr, 15, 1);
		//---准备读取数据
		JTAG_ShiftDR_BIT(JTAGx, 0x3200, 15, 1);
		//---读取低位数据
		tempeFlash = JTAG_ShiftDR_BIT(JTAGx, 0x3600, 15, 0);
		*(pVal++) = (UINT8_T)tempeFlash;
		//---读取高位数据
		tempeFlash = JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 0);
		*(pVal++) = (UINT8_T)(tempeFlash);
		//---低位地址偏移
		lowAddr++;
		if (lowAddr == 0)
		{
			//---高位地址偏移
			highAddr++;
			//---判断是否发生拓展位变化
			if (highAddr == 0)
			{
				//---拓展位地址偏移
				externAddr++;
				//---加载拓展位地址
				JTAG_ShiftDR_BIT(JTAGx, 0x0B00 | externAddr, 15, 1);
			}
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：按地址读取Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_ReadChipFlashLongAddr(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT32_T addr, UINT16_T length)
{
	return  JTAG_ReadChipFlashAddr(JTAGx, pVal, (UINT8_T)(addr >> 16), (UINT8_T)(addr >> 8), (UINT8_T)(addr), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页编程模式，更新数据到FLASH的编程缓存区
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_UpdateChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT8_T* pVal, UINT16_T length)
{
	UINT32_T tempVal = 0;
	UINT8_T i = 0;
	//---判断数据长度是否为偶数
	if ((length & 0x01) != 0x00)
	{
		return ERROR_2;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---校验是否进入Flash写入模式
	if ((JTAGx->msgJtagCmd == 0) || (JTAGx->msgState != JTAG_PROG_WRITE_FLASH))
	{
		//---发送命令
		JTAG_ShiftIR_BIT(JTAGx, PROG_COMMANDS, 4, 1);
		//---准备发送数据
		JTAG_RunCaptureDR(JTAGx);
		//---编程熔丝位
		JTAG_ShiftDR_BIT(JTAGx, 0x2310, 15, 1);
		JTAGx->msgJtagCmd = 1;
		JTAGx->msgState = JTAG_PROG_WRITE_FLASH;
	}
	//---检验是否需要加载高地址和低地址
	if (JTAGx->msgPageWordIndex == 0)
	{
		if (externAddr != 0)
		{
			//---加载拓展位地址
			JTAG_ShiftDR_BIT(JTAGx, 0x0B00 | externAddr, 15, 1);
		}
		//---加载高位地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0700 | externAddr, 15, 1);
	}
	//---填充缓存区
	for (i = 0; i < length; i++)
	{
		//---加载低位地址
		JTAG_ShiftDR_BIT(JTAGx, 0x0300 | lowAddr, 15, 1);
		//---加载低位数据
		JTAG_ShiftDR_BIT(JTAGx, 0x1300 | *(pVal++), 15, 1);
		//---加载高位数据
		JTAG_ShiftDR_BIT(JTAGx, 0x1700 | *(pVal++), 15, 1);
		//---锁存数据
		JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x7700, 15, 1);
		JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
		lowAddr++;
	}
	//---更新缓存区的序号；
	JTAGx->msgPageWordIndex += length;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页编程模式，执行页写入操作
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_UpdateChipFlash(JTAG_HandlerType* JTAGx)
{
	//---使能页写入
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3500, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	JTAG_ShiftDR_BIT(JTAGx, 0x3700, 15, 1);
	//---等待页写入完成
	return JTAG_WaitPollChipComplete(JTAGx, 0x3700);
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页模式，将指定的数据写入Flash
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_WriteChipFlashPage(JTAG_HandlerType* JTAGx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT32_T pageAddr = 0;
	//---检查当前编程模式
	if (JTAGx->msgState == 0)
	{
		//---进入编程模式
		_return = JTAG_EnterProg(JTAGx);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		JTAG_RefreshWatch(JTAGx);
		//---填充数据缓存
		_return = JTAG_UpdateChipFlashPage(JTAGx, externAddr, highAddr, lowAddr, pVal, length);
		//---换算返回结果
		_return = (_return == OK_0 ? OK_0 : ERROR_1);
		//---缓存区填满，执行数据写入操作
		if ((_return == OK_0) && (JTAGx->msgPageWordIndex == JTAGx->msgFlashPerPageWordSize))
		{
			//---执行写页操作
			_return = JTAG_UpdateChipFlash(JTAGx);
			//---数据缓存区的
			JTAGx->msgPageWordIndex = 0;
			//---换算返回结果
			_return = (_return == OK_0 ? OK_0 : ERROR_2);
		}
		//---设置时间间隔
		JTAG_SetIntervalTime(JTAGx, 100);
	}
	else
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验Flash数据是否为空
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_CheckChipFlashEmpty(JTAG_HandlerType* JTAGx, UINT8_T pageByteSizeH, UINT8_T pageByteSizeL, UINT8_T pageNumH, UINT8_T pageNumL)
{
	UINT8_T _return = OK_0;
	UINT16_T length = 0;
	UINT16_T pageNum = 0;
	UINT16_T i = 0;
	UINT32_T addr = 0;
	//---计算每页字节数
	length = pageByteSizeH;
	length = (length << 8) + pageByteSizeL;
	//---计算页数
	pageNum = pageNumH;
	pageNum = (pageNum << 8) + pageNumL;
	//---申请内存
	UINT8_T* pFlashBuffer = (UINT8_T*)MyMalloc(length);
	//---判断缓存空间申请是否成功
	if (pFlashBuffer == NULL)
	{
		_return = ERROR_1;
		goto GoToExit;
	}
	for (i = 0; i < pageNum; i++)
	{
		_return = JTAG_ReadChipFlashLongAddr(JTAGx, pFlashBuffer, addr, length);
		if (_return != OK_0)
		{
			_return = ERROR_2;
			goto GoToExit;
		}
		//---校验数据是否全部为0xFF
		_return = CompareByte(pFlashBuffer, 0xFF, length);
		if (_return != OK_0)
		{
			_return = ERROR_3;
			goto GoToExit;
		}
		//---计算地址偏移
		addr += (length >> 1);
	}
	//---退出入口
GoToExit:
	//---释放缓存空间
	MyFree(pFlashBuffer);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验Eeprom数据是否为空
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_CheckChipFlashEmptyLong(JTAG_HandlerType* JTAGx, UINT16_T pageByteSize, UINT16_T pageNum)
{
	return JTAG_CheckChipFlashEmpty(JTAGx, (UINT8_T)(pageByteSize >> 8), (UINT8_T)(pageByteSize), (UINT8_T)(pageNum >> 8), (UINT8_T)(pageNum));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验Eeprom数据是否为空
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_CheckChipEepromEmpty(JTAG_HandlerType* JTAGx, UINT8_T byteSize, UINT8_T num)
{
	UINT8_T _return = OK_0;
	UINT8_T i = 0;
	UINT16_T addr = 0;
	//---申请内存
	UINT8_T* pEepromBuffer = (UINT8_T*)MyMalloc(byteSize);
	//---判断缓存空间申请是否成功
	if (pEepromBuffer == NULL)
	{
		_return = ERROR_1;
		goto GoToExit;
	}
	for (i = 0; i < num; i++)
	{
		_return = JTAG_ReadChipEepromLongAddr(JTAGx, pEepromBuffer, addr, byteSize);
		if (_return != OK_0)
		{
			_return = ERROR_2;
			goto GoToExit;
		}
		//---校验数据是否全部为0xFF
		_return = CompareByte(pEepromBuffer, 0xFF, byteSize);
		if (_return != OK_0)
		{
			_return = ERROR_3;
			goto GoToExit;
		}
		//---计算地址偏移
		addr += byteSize;
	}
	//---退出入口
GoToExit:
	//---释放缓存空间
	MyFree(pEepromBuffer);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：配置配置信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T JTAG_SetConfigInfo(JTAG_HandlerType* JTAGx, UINT8_T* pVal)
{
	//---Flash每页字数
	JTAGx->msgFlashPerPageWordSize = *(pVal++);
	JTAGx->msgFlashPerPageWordSize = (JTAGx->msgFlashPerPageWordSize << 8) + *(pVal++);
	//---Eeprom每页字节数
	JTAGx->msgEerpomPerPageByteSize = *(pVal++);
	JTAGx->msgEerpomPerPageByteSize = (JTAGx->msgEerpomPerPageByteSize << 8) + *(pVal++);
	//---Eeprom是否支持页编程模式
	JTAGx->msgEepromIsPageMode = *(pVal++);
	return OK_0;
}