#include "isp_cfg.h"

//===全局变量的定义
ISP_HandlerType  g_IspDevice0 = { 0 };
pISP_HandlerType pIspDevice0 = &g_IspDevice0;

//===统一发送函数
UINT8_T(*ISP_SEND_CMD)(ISP_HandlerType *, UINT8_T, UINT8_T, UINT8_T, UINT8_T);

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_Device0_RST(UINT8_T rstState)
{
	if (rstState == ISP_RST_TO_GND)
	{
		RST_PORT_TO_GND;
	}
	else if (rstState == ISP_RST_TO_VCC)
	{
		RST_PORT_TO_VCC;
	}
	else
	{
		RST_PORT_TO_HZ;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_Device1_RST(UINT8_T rstState)
{
	
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_Device2_RST(UINT8_T rstState)
{

}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：硬件初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_HW_Init(ISP_HandlerType *ISPx)
{
	//---注销当前的所有配置
	SPITask_DeInit(&(ISPx->msgSPI),1);	
	//---硬件端口的配置---硬件实现
	SPITask_MHW_GPIO_Init(&(ISPx->msgSPI));
	//---硬件SPI的初始化
	LL_SPI_InitTypeDef SPI_InitStruct={0};
	//---SPI的模式配置
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;														//---主机模式
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;												//---8位数据	
	//---时钟空闲时的极性
	if(ISPx->msgSPI.msgCPOL==0)
	{
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;											//---CLK空闲时为低电平 (CLK空闲是只能是低电平)
		GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
	}
	else
	{
		SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;										//---CLK空闲时为高电平 (CLK空闲是只能是低电平)
	}	
	//---数据采样的时钟边沿位置
	if (ISPx->msgSPI.msgCPOL == 0)
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	}
	else
	{
		SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	}	
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;															//---软件控制
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;										//---系统时钟256分频
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;														//---高位在前
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;									//---硬件CRC不使能
	SPI_InitStruct.CRCPoly = 7;
	SPITask_MHW_PollMode_Init(&(ISPx->msgSPI), SPI_InitStruct);
	ISPx->msgInit = 1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：软件模拟初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SW_Init(ISP_HandlerType *ISPx)
{
	SPITask_DeInit(&(ISPx->msgSPI),1);
	//---硬件端口的配置---软件实现
	SPITask_MSW_GPIO_Init(&(ISPx->msgSPI));
	//---时钟空闲时的极性
	if (ISPx->msgSPI.msgCPOL == 0)
	{
		GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
	}
	else
	{
		GPIO_OUT_1(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
	}
	//---除片选信号输出高电平，其余端口都输出低电平，默认的初始化是高电平，在这里需要改动
	GPIO_OUT_0(ISPx->msgSPI.msgMOSI.msgPort, ISPx->msgSPI.msgMOSI.msgBit);
	GPIO_OUT_0(ISPx->msgSPI.msgMISO.msgPort, ISPx->msgSPI.msgMISO.msgBit);
	ISPx->msgInit = 1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设备0的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Device0_Init(ISP_HandlerType *ISPx)
{
	//---设定编程状态为空闲模式
	ISPx->msgState=0;
	//---设定初始化状态
	ISPx->msgInit = 0;
	//---设定硬件的时钟
	ISPx->msgSetClok = ISP_SCK_DEFAULT_CLOCK;
	//---初始化
	ISPx->msgDelayms = 0;
	//---初始化
	ISPx->msgHideAddr = 0;
	//---设置为延时模式
	ISPx->msgPollReady=0;
	//---设置Eeprom不支持页编程模式
	ISPx->msgEepromPageMode=0;
	//---设置Flash每页的字节数
	ISPx->msgFlashPerPageWordSize=0;
	//---初始化缓存区的序号
	ISPx->msgPageWordIndex=0;
	//---配置轮询时间
	ISPx->msgIntervalTime=ISP_STATE_TIME_OUT_MS;
	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);
	//---电平转换使能控制端
#ifdef ISP_USE_lEVEL_SHIFT
    #ifdef ISP_USE_HV_RESET
	   ISPx->msgOE.msgPort=GPIOD;
	   ISPx->msgOE.msgBit = LL_GPIO_PIN_10;
	#else
	   ISPx->msgOE.msgPort=GPIOD;
	   ISPx->msgOE.msgBit = LL_GPIO_PIN_14;
    #endif
#endif
	//---常规板子
	/**SPI2 GPIO Configuration
	PB12   ------> SPI2_NSS
	PB13   ------> SPI2_SCK
	PB14   ------> SPI2_MISO
	PB15   ------> SPI2_MOSI
	*/
	//---高压板
	/**SPI2 GPIO Configuration
	PB13   ------> SPI2_SCK
	PC2    ------> SPI2_MISO
	PC3    ------> SPI2_MOSI
	*/
	//---CS
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst = ISP_Device0_RST;
	//---SCK
	ISPx->msgSPI.msgSCK.msgPort = GPIOB;
	ISPx->msgSPI.msgSCK.msgBit = LL_GPIO_PIN_13;
	//---MISO
	ISPx->msgSPI.msgMISO.msgPort = GPIOC;
	ISPx->msgSPI.msgMISO.msgBit = LL_GPIO_PIN_2;
	//---MOSI
	ISPx->msgSPI.msgMOSI.msgPort = GPIOC;
	ISPx->msgSPI.msgMOSI.msgBit = LL_GPIO_PIN_3;
#else
	ISPx->msgSPI.msgCS.msgPort = GPIOB;
	ISPx->msgSPI.msgCS.msgBit = LL_GPIO_PIN_12;
	//---SCK
	ISPx->msgSPI.msgSCK.msgPort = GPIOB;
	ISPx->msgSPI.msgSCK.msgBit = LL_GPIO_PIN_13;
	//---MISO
	ISPx->msgSPI.msgMISO.msgPort = GPIOB;
	ISPx->msgSPI.msgMISO.msgBit = LL_GPIO_PIN_14;
	//---MOSI
	ISPx->msgSPI.msgMOSI.msgPort = GPIOB;
	ISPx->msgSPI.msgMOSI.msgBit = LL_GPIO_PIN_15;
#endif
	
	//---复用模式
#ifndef USE_MCU_STM32F1
	ISPx->msgSPI.msgGPIOAlternate = LL_GPIO_AF_5;
#endif
	//---SPI序号
	ISPx->msgSPI.msgSPIx = SPI2;
#ifndef USE_MCU_STM32F1
	//---SPI的协议
	ISPx->msgSPI.msgStandard = LL_SPI_PROTOCOL_MOTOROLA;
#endif	
	//---时钟空闲为低电平
	ISPx->msgSPI.msgCPOL=0;
	//---数据采样在第一个时钟边沿
	ISPx->msgSPI.msgCPOH=0;	
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设备1的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Device1_Init(ISP_HandlerType *ISPx)
{
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设备2的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Device2_Init(ISP_HandlerType *ISPx)
{
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_Init(ISP_HandlerType *ISPx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	//---使用的DHT11的端口
	if ((ISPx != NULL) && (ISPx == ISP_TASK_ONE))
	{
		ISP_Device0_Init(ISPx);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_TWO))
	{
		ISP_Device1_Init(ISPx);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_THREE))
	{
		ISP_Device2_Init(ISPx);
	}
	else
	{
		return ERROR_1;
	}
	//---注册ms的延时函数
	if (pFuncDelayms!=NULL)
	{
		ISPx->msgDelayms = pFuncDelayms;
	}
	else
	{
		ISPx->msgDelayms = DelayTask_ms;
	}
	//---注册us延时函数
	if (pFuncDelayus!=NULL)
	{
		ISPx->msgSPI.msgDelayus = pFuncDelayus;
	}
	else
	{
		ISPx->msgSPI.msgDelayus = DelayTask_us;
	}
	//---注册滴答函数
	if (pFuncTimerTick != NULL)
	{
		ISPx->msgSPI.msgTimeTick = pFuncTimerTick;
	}
	else
	{
		ISPx->msgSPI.msgTimeTick = SysTickTask_GetTick;
	}
	//---配置OE端口
#ifdef ISP_USE_lEVEL_SHIFT
	if (ISPx->msgOE.msgPort != NULL)
	{
		//---使能GPIO的时钟
		GPIOTask_Clock(ISPx->msgOE.msgPort, 1);
		//---GPIO的结构体
		LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;													//---配置状态为输出模式
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;											//---GPIO的速度---低速设备
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;										//---输出模式---推挽输出
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;														//---上拉
#ifndef USE_MCU_STM32F1
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;													//---端口复用模式
#endif
		//---ISP_OE_BIT的初始化
		GPIO_InitStruct.Pin = ISPx->msgOE.msgBit;
		LL_GPIO_Init(ISPx->msgOE.msgPort, &GPIO_InitStruct);
		GPIO_OUT_1(ISPx->msgOE.msgPort, ISPx->msgOE.msgBit);
	}
#endif
	//---当前时间戳
	ISPx->msgRecordTime=ISPx->msgSPI.msgTimeTick();
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：销毁初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_DeInit(ISP_HandlerType *ISPx)
{
	SPITask_DeInit(&(ISPx->msgSPI),1);
	ISPx->msgInit = 0;
	//---处理高压端口信息
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_HZ);
#endif
	//---处理电平转换芯片
#ifdef ISP_USE_lEVEL_SHIFT
	GPIO_OUT_1(ISPx->msgOE.msgPort, ISPx->msgOE.msgBit);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：销毁初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_AutoInit(ISP_HandlerType* ISPx)
{
	if (ISPx->msgSPI.msgModelIsHW != 0)
	{
		ISP_HW_Init(ISPx);
#ifdef USE_MCU_STM32
		//---限制编程的最大速度，小于1MHz；注意这里两个变量的使用，一个是设置ISP的时钟等级，一个是设置硬件SPI的时钟
		if (ISPx->msgSetClok > ISP_SCK_PRE_32)
		{
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV32;
		}
#endif
		//---设置SPI的硬件时钟
		SPITask_MHW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgClockSpeed);
		//---传递发送命令函数
		ISP_SEND_CMD = ISP_HW_SendCmd;
	}
	else
	{
		ISP_SW_Init(ISPx);
		//---设置软件模拟的脉宽
		SPITask_MSW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgPluseWidth);
		//---传递发送命令函数
		ISP_SEND_CMD = ISP_SW_SendCmd;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：自动注销
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_AutoDeInit(ISP_HandlerType* ISPx)
{
	//---注销当前的所有配置
	return	ISP_DeInit(ISPx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置自动编程编程时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SetClock(ISP_HandlerType *ISPx, UINT8_T clok)
{
	switch (clok)
	{
		case ISP_SCK_KHZ_0_5:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 1000;
			break;
		case ISP_SCK_KHZ_1:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 500;
			break;
		case ISP_SCK_KHZ_2:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 250;
			break;
		case ISP_SCK_KHZ_4:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 124;
			break;
		case ISP_SCK_KHZ_8:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 60;
			break;
		case ISP_SCK_KHZ_16:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 28;
			break;
		case ISP_SCK_KHZ_32:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 12;
			break;
		case ISP_SCK_KHZ_64:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 4;
			break;
		case ISP_SCK_KHZ_128	:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 3;
			break;
		case ISP_SCK_KHZ_256	:
			ISPx->msgSPI.msgModelIsHW  = 0;
			ISPx->msgSPI.msgPluseWidth = 0;
			break;
		case ISP_SCK_PRE_256:
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV256;
			break;
		case ISP_SCK_PRE_128:
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV128;
			break;
		case ISP_SCK_PRE_64:
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV64;
			break;
		case ISP_SCK_PRE_32:
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV32;
			break;
		case ISP_SCK_PRE_16:
		#ifdef USE_MCU_STM32
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV16;
		#endif
			break;
		case ISP_SCK_PRE_8:
			ISPx->msgSPI.msgModelIsHW  = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV8;
			break;
		case ISP_SCK_PRE_4:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV4;
			break;
		case ISP_SCK_PRE_2:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV2;
			break;
		default:
			ISPx->msgSPI.msgModelIsHW = 1;
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV256;
			break;
	}
	if (ISPx->msgSPI.msgModelIsHW == 1)
	{
		//---第一次需要初始化端口，或则从模拟方式切换到硬件方式，也需要重新初始化一下端口
		if ((ISPx->msgInit == 0) || (ISP_SEND_CMD == ISP_SW_SendCmd))
		{
			ISP_HW_Init(ISPx);
		}
	#ifdef USE_MCU_STM32
		//---限制编程的最大速度，小于1MHz；注意这里两个变量的使用，一个是设置ISP的时钟等级，一个是设置硬件SPI的时钟
		if (ISPx->msgSetClok > ISP_SCK_MAX_CLOCK)
		{
			ISPx->msgSPI.msgClockSpeed = LL_SPI_BAUDRATEPRESCALER_DIV32;
		}
	#endif
		//---设置SPI的硬件时钟
		SPITask_MHW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgClockSpeed);
		//---传递发送命令函数
		ISP_SEND_CMD = ISP_HW_SendCmd;
	}
	else
	{
		//---第一次需要初始化端口，或则从硬件方式切换到模拟方式，也需要重新初始化一下端口
		if ((ISPx->msgInit == 0) || (ISP_SEND_CMD == ISP_HW_SendCmd))
		{
			ISP_SW_Init(ISPx);
		}
		//---设置软件模拟的脉宽
		SPITask_MSW_SetClock(&(ISPx->msgSPI), ISPx->msgSPI.msgPluseWidth);
		//---传递发送命令函数
		ISP_SEND_CMD = ISP_SW_SendCmd;
	}
#ifdef ISP_USE_lEVEL_SHIFT
	GPIO_OUT_0(ISPx->msgOE.msgPort, ISPx->msgOE.msgBit);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置固定编程时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SetProgClock(ISP_HandlerType* ISPx, UINT8_T clock)
{
	if(((clock<ISP_SCK_AUTO_MAX_COUNT)||(clock==ISP_SCK_AUTO_MAX_COUNT))&&(clock>0))
	{
		ISPx->msgAutoClock=1;
		ISPx->msgSetClok=clock;
	}
	else
	{
		ISPx->msgAutoClock = 0;
		//---恢复为默认时钟
		ISPx->msgSetClok = ISP_SCK_DEFAULT_CLOCK;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：软件发送命令
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SW_SendCmd(ISP_HandlerType *ISPx, UINT8_T val1, UINT8_T Val2, UINT8_T val3, UINT8_T val4)
{
	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);
	//---填充发送缓存区
	ISPx->msgWriteByte[0] = val1;
	ISPx->msgWriteByte[1] = Val2;
	ISPx->msgWriteByte[2] = val3;
	ISPx->msgWriteByte[3] = val4;
	//---数据发送，高位在前
	return SPITask_MSW_WriteAndReadDataMSB(&(ISPx->msgSPI), ISPx->msgWriteByte, ISPx->msgReadByte, ISP_COMM_MAX_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：硬件发送命令
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_HW_SendCmd(ISP_HandlerType *ISPx, UINT8_T val1, UINT8_T Val2, UINT8_T val3, UINT8_T val4)
{
	//---清零发送缓存区
	memset(ISPx->msgWriteByte, 0x00, 4);
	memset(ISPx->msgReadByte, 0x00, 4);
	//---填充发送缓存区
	ISPx->msgWriteByte[0] = val1;
	ISPx->msgWriteByte[1] = Val2;
	ISPx->msgWriteByte[2] = val3;
	ISPx->msgWriteByte[3] = val4;
	//---数据发送
	return SPITask_MHW_PollMode_WriteAndReadData(&(ISPx->msgSPI), ISPx->msgWriteByte, ISPx->msgReadByte, ISP_COMM_MAX_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：准备进入编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_PreEnterProg(ISP_HandlerType *ISPx)
{
	//---设置端口CS端口为输出模式
	GPIO_SET_WRITE(ISPx->msgSPI.msgCS.msgPort, ISPx->msgSPI.msgCS.msgBit);
	//---首先拉低时钟线
	GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_GND);
#else
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
#endif
	//---打开电源
	//POWER_DUT_ON;
	//---首先拉低时钟线
	GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_GND);
#else
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgPort, ISPx->msgSPI.msgCS.msgBit);
#endif
	ISPx->msgDelayms(1);
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_VCC);
#else
	GPIO_OUT_1(ISPx->msgSPI.msgCS.msgPort, ISPx->msgSPI.msgCS.msgBit);
#endif
	ISPx->msgDelayms(1);
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_GND);
#else
	GPIO_OUT_0(ISPx->msgSPI.msgCS.msgPort, ISPx->msgSPI.msgCS.msgBit);
#endif
	ISPx->msgDelayms(1);
	//---解除64K的限制
	ISPx->msgHideAddr = 0xFF;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：进入编程模式
//////输入参数: isPollReady：0---延时模式，1---轮询准备好信号
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_EnterProg(ISP_HandlerType *ISPx,UINT8_T isPollReady)
{
	UINT8_T count = ISP_SCK_AUTO_MAX_COUNT;
	//---设置时钟
	ISP_SetClock(ISPx, ISPx->msgSetClok);
	//---编程模式准备
	ISP_PreEnterProg(ISPx);
	//---自动获取合适的速度
	while (count--)
	{
		//---发送编程命令
		ISP_SEND_CMD(ISPx, 0xAC, 0x53, 0x00, 0x00);
		//---检查是否进入成功
		if (ISPx->msgReadByte[2] == 0x53)
		{
			//---编程状态为编程模式
			ISPx->msgState = 1;
			//---配置查询准备好信号的标志
			ISPx->msgPollReady=isPollReady;
			//---增加监控函数
			ISP_AddWatch(ISPx);
			return OK_0;
		}
		//---校验是否是自动调速模式
		if (ISPx->msgAutoClock==0)
		{
			//---自动降速处理
			if (ISPx->msgSetClok >ISP_SCK_KHZ_2)
			{
				//---降速处理
				ISPx->msgSetClok -= 1;
			}
			else
			{
				//---限制最低时速
				ISPx->msgSetClok = ISP_SCK_KHZ_2;
			}
		}
		else
		{
			count-=1;
		}
		//---设置时钟
		ISP_SetClock(ISPx, ISPx->msgSetClok);
		//---置位时钟线和片选端
		//GPIO_OUT_1(ISPx->msgSPI.msgSCK.msgGPIOPort, ISPx->msgSPI.msgSCK.msgGPIOBit);
#ifdef ISP_USE_HV_RESET
		ISPx->msgPortRst(ISP_RST_TO_VCC);
#else
		GPIO_OUT_1(ISPx->msgSPI.msgCS.msgGPIOPort, ISPx->msgSPI.msgCS.msgGPIOBit);
#endif
		ISPx->msgDelayms(1);
		//---清零时钟线和片选端
		//GPIO_OUT_0(ISPx->msgSPI.msgSCK.msgPort, ISPx->msgSPI.msgSCK.msgBit);
#ifdef ISP_USE_HV_RESET
		ISPx->msgPortRst(ISP_RST_TO_GND);
#else
		GPIO_OUT_0(ISPx->msgSPI.msgCS.msgPort, ISPx->msgSPI.msgCS.msgBit);
#endif
		ISPx->msgDelayms(1);
	}
	//---设置RST端口未HZ状态，避免高压倒灌
#ifdef ISP_USE_HV_RESET
	ISPx->msgPortRst(ISP_RST_TO_HZ);
#endif 
#ifdef ISP_USE_lEVEL_SHIFT
	GPIO_OUT_1(ISPx->msgOE.msgPort, ISPx->msgOE.msgBit);
#endif
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ExitProg(ISP_HandlerType *ISPx)
{
	ISP_DeInit(ISPx);
	//---自动编程时钟校验
	if (ISPx->msgAutoClock==0)
	{
		//---恢复时钟的速度
		ISPx->msgSetClok = ISP_SCK_DEFAULT_CLOCK;
	}
	//---清除Eeprom页编程模式
	ISPx->msgEepromPageMode = 0;
	//---清除数据缓存区的序号
	ISPx->msgPageWordIndex = 0;
	//---解除64K的限制
	ISPx->msgHideAddr = 0xFF;
	//---编程状态为空闲模式
	ISPx->msgState=0;
	//---检查编程结束模式设置为延时等待
	ISPx->msgPollReady=0;
	//---移除注册的监控函数
	ISP_RemoveWatch(ISPx);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程监控任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_WatchTask(ISP_HandlerType* ISPx)
{
	UINT32_T nowTime = 0;
	UINT32_T cnt = 0;
	if (ISPx->msgState!=0)
	{
		//---获取当前时间节拍
		nowTime= ISPx->msgSPI.msgTimeTick();
		//---计算时间间隔
		if (ISPx->msgRecordTime > nowTime)
		{
			cnt = (0xFFFFFFFF - ISPx->msgRecordTime + nowTime);
		}
		else
		{
			cnt = nowTime - ISPx->msgRecordTime;
		}
		//if (cnt > ISP_STATE_TIME_OUT_MS)
		//---检查是否发生超时事件
		if (cnt>ISPx->msgIntervalTime)
		{
			ISP_ExitProg(ISPx);
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
void ISP_AddWatchDevice0(void)
{
	ISP_WatchTask(ISP_TASK_ONE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备2的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_AddWatchDevice1(void)
{
	ISP_WatchTask(ISP_TASK_TWO);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备3的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void ISP_AddWatchDevice2(void)
{
	ISP_WatchTask(ISP_TASK_THREE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加监控
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_AddWatch(ISP_HandlerType* ISPx)
{
	UINT8_T _return=OK_0;
	if (ISPx!=NULL)
	{
		//---刷新时间
		_return=ISP_RefreshWatch(ISPx);
		//---使用的ISP的端口,并注册相应的任务函数
		if (ISPx == ISP_TASK_ONE)
		{
			SysTickTask_CreateTickTask(ISP_AddWatchDevice0);
		}
		else if (ISPx == ISP_TASK_TWO)
		{
			SysTickTask_CreateTickTask(ISP_AddWatchDevice1);
		}
		else if (ISPx == ISP_TASK_THREE)
		{
			SysTickTask_CreateTickTask(ISP_AddWatchDevice2);
		}
		else
		{
			_return= ERROR_1;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：移除监控
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_RemoveWatch(ISP_HandlerType* ISPx)
{
	//---使用的ISP的端口
	if ((ISPx != NULL) && (ISPx == ISP_TASK_ONE))
	{
		SysTick_DeleteTickTask(ISP_AddWatchDevice0);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_TWO))
	{
		SysTick_DeleteTickTask(ISP_AddWatchDevice1);
	}
	else if ((ISPx != NULL) && (ISPx == ISP_TASK_THREE))
	{
		SysTick_DeleteTickTask(ISP_AddWatchDevice2);
	}
	else
	{
		return ERROR_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：刷新监控
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_RefreshWatch(ISP_HandlerType* ISPx)
{
	//---配置轮训间隔为最大值，单位是ms
	ISPx->msgIntervalTime = ISP_STATE_TIME_OUT_MS;
	//---刷新纪录时间
	ISPx->msgRecordTime = ISPx->msgSPI.msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置间隔时间
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_SetIntervalTime(ISP_HandlerType* ISPx,UINT16_T intervalTime)
{
	//---配置轮训间隔时间，单位是ms
	ISPx->msgIntervalTime= intervalTime;
	//---刷新纪录时间
	ISPx->msgRecordTime = ISPx->msgSPI.msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：获取间隔时间
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T ISP_GetIntervalTime(ISP_HandlerType* ISPx)
{
	return ISPx->msgIntervalTime;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取准备好标志位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadReady(ISP_HandlerType *ISPx)
{
	UINT8_T _return = 0;
	//---获取时间标签
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;
	if (ISPx->msgSPI.msgTimeTick != NULL)
	{
		//nowTime = ISPx->msgSPI.msgFuncTick();
		oldTime = ISPx->msgSPI.msgTimeTick();
	}
	//---查询忙标志位
	while (1)
	{
		//---发送读取忙标志的命令
		_return = ISP_SEND_CMD(ISPx, 0xF0, 0x00, 0x00, 0x00);
		//---检查是否读取到忙标志
		if (_return)
		{
			break;
		}
		else
		{
			if ((ISPx->msgReadByte[3] & 0x01) == 0x00)
			{
				_return = OK_0;
				break;
			}
			else
			{
				if (ISPx->msgSPI.msgTimeTick != NULL)
				{
					//---当前时间
					nowTime = ISPx->msgSPI.msgTimeTick();
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
UINT8_T ISP_EraseChip(ISP_HandlerType *ISPx)
{
	UINT8_T _return = 0;
	//---发送擦除命令
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0x80, 0x00, 0x00);
	if (_return == 0)
	{
		//---检查轮询方式
		if (ISPx->msgPollReady!=0)
		{
			_return = ISP_ReadReady(ISPx);
			_return+=0x80;
		}
		else
		{
			//---擦除之后的等待延时
			ISPx->msgDelayms(10 + ISPx->msgWaitms);
		}
		
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取设备ID
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipID(ISP_HandlerType *ISPx, UINT8_T *pVal)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	for (i = 0; i < 3; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0x30, 0x00, i, 0x0);
		if (_return != 0)
		{
			_return = (i + 2);
			break;
		}
		*(pVal++) = ISPx->msgReadByte[3];
	}
	return _return;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取叫准字
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipCalibration(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0x38, 0x00, i, 0x00);
		if (_return != 0)
		{
			_return = (i + 2);
			break;
		}
		*(pVal++) = ISPx->msgReadByte[3];
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取熔丝位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = 0;
	//---读取熔丝位低位
	_return = ISP_SEND_CMD(ISPx, 0x50, 0x00, 0x00, 0x00);
	if (_return != 0x00)
	{
		return ERROR_2;
	}
	//---保存低位值
	*(pVal++) = ISPx->msgReadByte[3];
	//---读取熔丝位高位
	_return = ISP_SEND_CMD(ISPx, 0x58, 0x08, 0x00, 0x00);
	if (_return != 0x00)
	{
		return ERROR_3;
	}
	//---保存高位值
	*(pVal++) = ISPx->msgReadByte[3];
	//---读取熔丝位拓展位
	if (isNeedExternFuse != 0x00)
	{
		//---读取熔丝的拓展位值
		_return = ISP_SEND_CMD(ISPx, 0x50, 0x08, 0x00, 0x00);
		//---保存拓展位值
		*(pVal) = ISPx->msgReadByte[3];
	}
	else
	{
		*pVal = 0xFF;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取加密位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipLock(ISP_HandlerType *ISPx, UINT8_T *pVal)
{
	//---读取加密位
	UINT8_T _return = ISP_SEND_CMD(ISPx, 0x58, 0x00, 0x00, 0x00);
	*(pVal) = ISPx->msgReadByte[3];
	return _return;
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
UINT8_T ISP_ReadChipRom(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T addr, UINT16_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	//---判断数据长度是否是偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_2;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---读取ROM页信息
	for (i = 0; i < length; i++)
	{
		//---ROM页低位数据
		_return = ISP_SEND_CMD(ISPx, 0x30, 0x00, (addr + i), 0x00);
		if (_return != 0)
		{
			return ERROR_3;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}
		//---ROM页高字节数据
		_return = ISP_SEND_CMD(ISPx, 0x38, 0x00, (addr + i), 0x00);
		if (_return != 0)
		{
			return 4;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程熔丝位,编程顺序依次是低位，高位，拓展位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipFuse(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T isNeedExternFuse)
{
	UINT8_T _return = 0;
	//---写入熔丝位低位
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA0, 0x00, pVal[0]);
	if (_return != 0x00)
	{
		return ERROR_2;
	}
	//---检查轮询方式
	if (ISPx->msgPollReady != 0)
	{
		_return = ISP_ReadReady(ISPx);
		_return += 0x80;
	}
	else
	{
		//---写入之后延时等待
		ISPx->msgDelayms(5 + ISPx->msgWaitms);
	}
	//---写入熔丝位高位
	_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA8, 0x00, pVal[1]);
	if (_return != 0x00)
	{
		return ERROR_3;
	}
	//---检查轮询方式
	if (ISPx->msgPollReady != 0)
	{
		_return = ISP_ReadReady(ISPx);
		_return += 0x80;
	}
	else
	{
		//---写入之后延时等待
		ISPx->msgDelayms(5 + ISPx->msgWaitms);
	}
	//---写入熔丝位拓展位
	if (isNeedExternFuse != 0x00)
	{
		//---写入拓展位
		_return = ISP_SEND_CMD(ISPx, 0xAC, 0xA4, 0x00, pVal[2]);
		//---判断写入是否成功
		if (_return == OK_0)
		{
			//---检查轮询方式
			if (ISPx->msgPollReady != 0)
			{
				_return = ISP_ReadReady(ISPx);
				_return += 0x80;
			}
			else
			{
				//---写入之后延时等待
				ISPx->msgDelayms(5 + ISPx->msgWaitms);
			}
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
UINT8_T ISP_WriteChipLock(ISP_HandlerType *ISPx, UINT8_T val)
{
	//---写入加密位
	UINT8_T _return = ISP_SEND_CMD(ISPx, 0xAC, 0xE0, 0x00, val|0xC0);
	//---判断写入是否成功
	if (_return == OK_0)
	{
		//---检查轮询方式
		if (ISPx->msgPollReady != 0)
		{
			_return = ISP_ReadReady(ISPx);
			_return += 0x80;
		}
		else
		{
			//---写入之后延时等待
			ISPx->msgDelayms(5 + ISPx->msgWaitms);
		}
	}
	return _return;
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
UINT8_T ISP_ReadChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0xA0, highAddr, lowAddr, 0x00);
		if (_return != OK_0)
		{
			break;
		}
		//---保存读取的数据
		*(pVal++) = ISPx->msgReadByte[3];
		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
		}
	}
	return _return;
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
UINT8_T ISP_ReadChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_ReadChipEepromAddr(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程EEPROM
//////输入参数:  pVal	---数据缓存区
//////			highAddr---数据地址的高字节，地址是字节地址
//////			lowAddr ---数据地址的低字节，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT8_T refreshFlag=0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		_return = ISP_SEND_CMD(ISPx, 0xC0, highAddr, lowAddr, pVal[i]);
		if (_return != OK_0)
		{
			break;
		}
		//---检查轮询方式
		if (ISPx->msgPollReady != 0)
		{
			_return = ISP_ReadReady(ISPx);
			//_return += 0x80;
		}
		else
		{
			//---写入之后延时等待
			ISPx->msgDelayms(10 + ISPx->msgWaitms);
		}
		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
		}
		refreshFlag++;
		//---检查次数，避免延时等待的时候退出了编程模式
		if (refreshFlag>20)
		{
			refreshFlag=0;
			//---刷新时间
			ISP_RefreshWatch(ISPx);
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新存储器的缓存区
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipEepromPage(ISP_HandlerType* ISPx, UINT8_T* pVal)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	//---填充缓存区
	for (i = 0; i < ISPx->msgEerpomPerPageByteSize; i++)
	{
		//---将数据写入缓存区
		_return = ISP_SEND_CMD(ISPx, 0xC1, 0x00, i, *(pVal++));
		//---校验写入结果
		if (_return != 0x00)
		{
			return ERROR_3;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新数据到指定页地址
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipEepromAddr(ISP_HandlerType* ISPx,UINT8_T highAddr, UINT8_T lowAddr)
{
	UINT8_T _return = OK_0;
	//---将数据写入存储器指定的页
	_return = ISP_SEND_CMD(ISPx, 0xC2, highAddr, lowAddr, 0x00);
	//---校验写入结果
	if (_return == OK_0)
	{
		//---检查轮询方式
		if (ISPx->msgPollReady != 0)
		{
			_return = ISP_ReadReady(ISPx);
			_return += 0x80;
		}
		else
		{
			//---写入之后延时等待
			ISPx->msgDelayms(10 + ISPx->msgWaitms);
		}
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
UINT8_T ISP_UpdateChipEepromLongAddr(ISP_HandlerType* ISPx, UINT16_T addr)
{
	return ISP_UpdateChipEepromAddr(ISPx, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程EEPROM
//////输入参数:  pVal	---数据缓存区
//////			addr	---数据地址，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_WriteChipEepromAddr(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromPage(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	UINT8_T _return = OK_0;
	UINT32_T pageAddr = 0;
	UINT16_T i = 0;
	//---检查当前编程模式
	if (ISPx->msgState == 0)
	{
		//---进入编程模式
		_return = ISP_EnterProg(ISPx, ISPx->msgPollReady);
	}
	if (_return == OK_0)
	{
		//---计算编程的页数
		pageNum /= (ISPx->msgEerpomPerPageByteSize);
		//---刷新时间
		ISP_RefreshWatch(ISPx);
		//---计算地址，对于Eeprom来说，字地址也是字节地址
		pageAddr = highAddr;
		pageAddr = (pageAddr << 8) + lowAddr;
		//---逐页编程Eeprom数据
		for (i = 0; i < pageNum; i++)
		{
			//---填充数据缓存
			_return = ISP_UpdateChipEepromPage(ISPx, pVal);
			//---换算返回结果
			_return = (_return == OK_0 ? OK_0 : ERROR_2);
			//---更新数据到指定的页地址
			_return = ISP_UpdateChipEepromLongAddr(ISPx, pageAddr);
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
				pageAddr += ISPx->msgEerpomPerPageByteSize;
				//---数据地址偏移
				pVal += ISPx->msgEerpomPerPageByteSize;
			}
		}
		//---设置时间间隔
		ISP_SetIntervalTime(ISPx, 100);
	}
	else
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：跳空编程EEPROM
//////输入参数:  pVal	---数据缓存区
//////			highAddr---数据地址的高字节，地址是字节地址
//////			lowAddr ---数据地址的低字节，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：如果写入的数据是空数据，那么就跳过写入
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	for (i = 0; i < length; i++)
	{
		if (pVal[i] != 0xFF)
		{
			_return = ISP_SEND_CMD(ISPx, 0xC0, highAddr, lowAddr, pVal[i]);
			if (_return != OK_0)
			{
				break;
			}
			//---检查轮询方式
			if (ISPx->msgPollReady != 0)
			{
				_return = ISP_ReadReady(ISPx);
				_return += 0x80;
			}
			else
			{
				//---写入之后延时等待
				ISPx->msgDelayms(10 + ISPx->msgWaitms);
			}
		}
		//---地址偏移
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：跳空编程EEPROM
//////输入参数:  pVal	---数据缓存区
//////			addr	---数据地址，地址是字节地址
//////			length	---读取数据的长度，数据长度是字节长度
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEepromLongAddrWithJumpEmpty(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT16_T addr, UINT16_T length)
{
	return ISP_WriteChipEepromAddrWithJumpEmpty(ISPx, pVal, (UINT8_T)(addr >> 8), (UINT8_T)(addr & 0xFF), length);
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipEeprom(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T pageNum)
{
	//---校验编程模式
	if (ISPx->msgEepromPageMode != 0)
	{
		//---编程指定位置的Eeprom数据,编程模式页模式
		return ISP_WriteChipEepromPage(ISPx,pVal,highAddr,lowAddr, pageNum);
	}
	else
	{
		//---编程指定位置的Eeprom数据，编程模式字模式
		return ISP_WriteChipEepromAddr(ISPx, pVal, highAddr, lowAddr, pageNum);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新拓展位地址
//////输入参数: addr---数据拓展位地址，地址是字地址
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateExternAddr(ISP_HandlerType *ISPx, UINT8_T addr)
{
	UINT8_T _return = OK_0;
	if (ISPx->msgHideAddr != addr)
	{
		ISPx->msgHideAddr = addr;
		_return = ISP_SEND_CMD(ISPx, 0x4D, 0x00, addr, 0x00);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新拓展位地址
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateExternLongAddr(ISP_HandlerType *ISPx, UINT32_T addr)
{
	return ISP_UpdateExternAddr(ISPx, (UINT8_T)(addr >> 16));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：按地址读取Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	//---判断数据长度是否为偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_2;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---更新拓展位
	if (externAddr!=0)
	{
		_return = ISP_UpdateExternAddr(ISPx, externAddr);
		if (_return != OK_0)
		{
			return ERROR_3;
		}
	}
	//---一次读取数据
	for (i = 0; i < length; i++)
	{
		//---读取低位数据
		_return = ISP_SEND_CMD(ISPx, 0x20, highAddr, lowAddr, 0x00);
		if (_return != OK_0)
		{
			return ERROR_4;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}
		//---读取高位数据
		_return = ISP_SEND_CMD(ISPx, 0x28, highAddr, lowAddr, 0x00);
		if (_return != 0x00)
		{
			return ERROR_5;
		}
		else
		{
			*(pVal++) = ISPx->msgReadByte[3];
		}
		lowAddr++;
		if (lowAddr == 0x00)
		{
			highAddr++;
			if (highAddr == 0x00)
			{
				externAddr++;
				//---更新拓展位
				_return = ISP_UpdateExternAddr(ISPx, externAddr);
				if (_return != 0x00)
				{
					return ERROR_6;
				}
			}
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：按地址读取Flash数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_ReadChipFlashLongAddr(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT32_T addr, UINT16_T length)
{
	return  ISP_ReadChipFlashAddr(ISPx, pVal, (UINT8_T)(addr >> 16), (UINT8_T)(addr >> 8), (UINT8_T)(addr), length);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新Flash缓存区
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipFlashPage(ISP_HandlerType *ISPx, UINT8_T *pVal, UINT8_T index, UINT16_T length)
{
	UINT8_T _return = 0;
	UINT8_T i = 0;
	//---判断数据长度是否为偶数
	if ((length & 0x01) != 0x00)
	{
		return ERROR_2;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---填充缓存区
	for (i = 0; i < length; i++)
	{
		//---更新低位地址的缓存区
		_return = ISP_SEND_CMD(ISPx, 0x40, 0x00, (index + i), *(pVal++));
		if (_return != 0x00)
		{
			return ERROR_3;
		}
		//---更新高位地址的缓存区
		_return = ISP_SEND_CMD(ISPx, 0x48, 0x00, (index + i), *(pVal++));
		if (_return != 0x00)
		{
			return ERROR_4;
		}
	}
	//---更新缓存区的序号；
	ISPx->msgPageWordIndex+=length;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新数据到指定页地址
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipFlashAddr(ISP_HandlerType *ISPx, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr)
{
	UINT8_T _return = OK_0;
	if (externAddr!=0)
	{
		_return= ISP_UpdateExternAddr(ISPx, externAddr);
	}	
	if (_return == OK_0)
	{
		_return = ISP_SEND_CMD(ISPx, 0x4C, highAddr, lowAddr, 0x00);
		if (_return == OK_0)
		{
			//---检查轮询方式
			if (ISPx->msgPollReady != 0)
			{
				_return = ISP_ReadReady(ISPx);
				_return += 0x80;
			}
			else
			{
				//---写入之后延时等待
				ISPx->msgDelayms(5 + ISPx->msgWaitms);
			}
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：更新数据到指定页地址
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_UpdateChipFlashLongAddr(ISP_HandlerType *ISPx, UINT32_T addr)
{
	return ISP_UpdateChipFlashAddr(ISPx, (UINT8_T)(addr >> 16), (UINT8_T)(addr >> 8), (UINT8_T)(addr));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：页模式向指定的数据写入Flash
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_WriteChipFlashPage(ISP_HandlerType* ISPx, UINT8_T* pVal, UINT8_T externAddr, UINT8_T highAddr, UINT8_T lowAddr, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT32_T pageAddr = 0;
	//---检查当前编程模式
	if (ISPx->msgState == 0)
	{
		//---进入编程模式
		_return = ISP_EnterProg(ISPx, ISPx->msgPollReady);
	}
	if (_return == OK_0)
	{
		//---刷新时间
		ISP_RefreshWatch(ISPx);
		//---填充数据缓存
		_return = ISP_UpdateChipFlashPage(ISPx, pVal, (UINT8_T)ISPx->msgPageWordIndex, length);
		//---换算返回结果
		_return = (_return == OK_0 ? OK_0 : ERROR_1);
		//---缓存区填满，执行数据写入操作
		if ((_return == OK_0) && (ISPx->msgPageWordIndex == ISPx->msgFlashPerPageWordSize))
		{
			//---计算字地址，传输的地址是字地址
			pageAddr = externAddr;
			pageAddr = (pageAddr << 8) + highAddr;
			pageAddr = (pageAddr << 8) + lowAddr;
			//---更新数据到指定的页地址
			_return = ISP_UpdateChipFlashLongAddr(ISPx, pageAddr);
			//---数据缓存区的
			ISPx->msgPageWordIndex = 0;
			//---换算返回结果
			_return = (_return == OK_0 ? OK_0 : ERROR_2);
		}
		//---设置时间间隔
		ISP_SetIntervalTime(ISPx, 100);
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
UINT8_T ISP_CheckChipFlashEmpty(ISP_HandlerType* ISPx,UINT8_T pageByteSizeH,UINT8_T pageByteSizeL,UINT8_T pageNumH,UINT8_T pageNumL)
{
	UINT8_T _return = OK_0;
	UINT16_T length=0;
	UINT16_T pageNum=0;
	UINT16_T i=0;
	UINT32_T addr=0;
	//---计算每页字节数
	length=pageByteSizeH;
	length=(length<<8)+pageByteSizeL;
	//---计算页数
	pageNum=pageNumH;
	pageNum=(pageNum<<8)+pageNumL;
	//---申请内存
	UINT8_T *pFlashBuffer= (UINT8_T*)MyMalloc(length);
	//---判断缓存空间申请是否成功
	if (pFlashBuffer == NULL)
	{
		_return = ERROR_1;
		goto GoToExit;
	}
	for (i=0;i<pageNum;i++)
	{
		_return=ISP_ReadChipFlashLongAddr(ISPx,pFlashBuffer,addr,length);
		if (_return!=OK_0)
		{
			_return=ERROR_2;
			goto GoToExit;
		}
		//---校验数据是否全部为0xFF
		_return=CompareByte(pFlashBuffer,0xFF,length);
		if (_return!=OK_0)
		{
			_return = ERROR_3;
			goto GoToExit;
		}
		//---计算地址偏移
		addr+=(length>>1);
	}
	//---退出入口
GoToExit:
	//---释放缓存空间
	MyFree(pFlashBuffer);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验Eeprom数据是否为空,参数也大小
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_CheckChipFlashEmptyLong(ISP_HandlerType* ISPx, UINT16_T pageByteSize, UINT16_T pageNum)
{
	return ISP_CheckChipFlashEmpty(ISPx,(UINT8_T)(pageByteSize>>8),(UINT8_T)(pageByteSize),(UINT8_T)(pageNum>>8),(UINT8_T)(pageNum));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验Eeprom数据是否为空
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T ISP_CheckChipEepromEmpty(ISP_HandlerType* ISPx, UINT8_T byteSize, UINT8_T num)
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
		_return = ISP_ReadChipEepromLongAddr(ISPx, pEepromBuffer, addr, byteSize);
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
UINT8_T ISP_SetConfigInfo(ISP_HandlerType* ISPx,UINT8_T *pVal)
{
	//---Flash每页字数
	ISPx->msgFlashPerPageWordSize= *(pVal++);
	ISPx->msgFlashPerPageWordSize=(ISPx->msgFlashPerPageWordSize<<8)+ *(pVal++);
	//---Eeprom每页字节数
	ISPx->msgEerpomPerPageByteSize= *(pVal++);
	ISPx->msgEerpomPerPageByteSize = (ISPx->msgEerpomPerPageByteSize<<8)+*(pVal++);
	//---Eeprom是否支持页编程模式
	ISPx->msgEepromPageMode= *(pVal++);
	return OK_0;
}
