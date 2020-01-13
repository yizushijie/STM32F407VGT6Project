#include "hvpp_cfg.h"

//===全局变量的定义
HVPP_HandlerType  g_HvppDevice0 = { 0 };
pHVPP_HandlerType pHvppDevice0 = &g_HvppDevice0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：数据总线为写入模式,并写入指定的数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_DATABUS_WRITE(HVPP_HandlerType* HVPPx,UINT8_T val )
{
	//---校验数据总线是不是写入状态
	if (HVPPx->msgDataBusState!=HVPP_DATA_BUS_MODE_WRITE)
	{
		//---设置为高阻态
		HVPP_DATA_BUS_DIR_TO_HZ;
		//---设置端口为输出模式
		HVPP_DATA_BUS_WRITE;
		HVPPx->msgDataBusState = HVPP_DATA_BUS_MODE_WRITE;
		//---切换数据流为主机到设备
		HVPP_DATA_BUS_DIR_TO_DEVICE;
		//---等待状态稳定
		HVPP_WAIT_STATE_STABLE;
	}
	//---发送数据
	((val & 0x80) == 0) ? HVPP_DATA0_OUT_0 : HVPP_DATA0_OUT_1;
	((val & 0x40) == 0) ? HVPP_DATA1_OUT_0 : HVPP_DATA1_OUT_1;
	((val & 0x20) == 0) ? HVPP_DATA2_OUT_0 : HVPP_DATA2_OUT_1;
	((val & 0x10) == 0) ? HVPP_DATA3_OUT_0 : HVPP_DATA3_OUT_1;
	((val & 0x08) == 0) ? HVPP_DATA4_OUT_0 : HVPP_DATA4_OUT_1;
	((val & 0x04) == 0) ? HVPP_DATA5_OUT_0 : HVPP_DATA5_OUT_1;
	((val & 0x02) == 0) ? HVPP_DATA6_OUT_0 : HVPP_DATA6_OUT_1;
	((val & 0x01) == 0) ? HVPP_DATA7_OUT_0 : HVPP_DATA7_OUT_1;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：数据总线为读取模式
//////输入参数: isReadDir---0切换总线为读取模式，1---读取总线数据
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_DATABUS_READ(HVPP_HandlerType* HVPPx,UINT8_T isReadDir)
{
	UINT8_T _return=0;
	//---校验数据总线是不是读取状态
	if (HVPPx->msgDataBusState != HVPP_DATA_BUS_MODE_READ)
	{
		//---设置为高阻态
		HVPP_DATA_BUS_DIR_TO_HZ;
		//---设置端口为输入模式
		HVPP_DATA_BUS_READ;
		HVPPx->msgDataBusState = HVPP_DATA_BUS_MODE_READ;
		//---切换数据流为设备到主机
		HVPP_DATA_BUS_DIR_TO_HOST;
		//---等待状态稳定
		HVPP_WAIT_STATE_STABLE;
	}
	if (isReadDir!= HVPP_DATA_BUS_READ_DIR)
	{
		//---读取数据
		_return |= (HVPP_DATA0_STATE != 0 ? 0x80 : 0x00);
		_return |= (HVPP_DATA1_STATE != 0 ? 0x40 : 0x00);
		_return |= (HVPP_DATA2_STATE != 0 ? 0x20 : 0x00);
		_return |= (HVPP_DATA3_STATE != 0 ? 0x10 : 0x00);
		_return |= (HVPP_DATA4_STATE != 0 ? 0x08 : 0x00);
		_return |= (HVPP_DATA5_STATE != 0 ? 0x04 : 0x00);
		_return |= (HVPP_DATA6_STATE != 0 ? 0x02 : 0x00);
		_return |= (HVPP_DATA7_STATE != 0 ? 0x01 : 0x00);
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
UINT8_T HVPP_GPIO_Init(HVPP_HandlerType* HVPPx)
{
	//---RST到地
	HVPP_DUT_RST_TO_GND;
	//---使能控制总线的端口时钟
	GPIOTask_Clock(HVPP_RDY_BSY_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_OE_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_WR_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_BS1_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_XA0_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_XA1_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_PAGEL_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_BS2_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_XTAL_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_CTRL_OE_PORT, PERIPHERAL_CLOCK_ENABLE);

	//---使能数据总线的端口时钟
	GPIOTask_Clock(HVPP_DATA0_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA1_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA2_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA3_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA4_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA5_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA6_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA7_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA_OE_PORT, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(HVPP_DATA_DIR_PORT, PERIPHERAL_CLOCK_ENABLE);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;																																		//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;																															//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;																															//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;																																			//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;																																		//---端口复用模式
#endif
	//---DATA_OE---输出为高，电平转换芯片为高阻态
	GPIO_InitStruct.Pin = HVPP_DATA_OE_BIT;
	LL_GPIO_Init(HVPP_DATA_OE_PORT, &GPIO_InitStruct);
	//---DATA_DIR---输出为高，数据从设备到主机
	GPIO_InitStruct.Pin = HVPP_DATA_DIR_BIT;
	LL_GPIO_Init(HVPP_DATA_DIR_PORT, &GPIO_InitStruct);
	//---数据流向为主机到设备
	HVPP_DATA_BUS_DIR_TO_DEVICE;

	//---CTRL_OE---输出为高，电平转换芯片为高阻态
	GPIO_InitStruct.Pin = HVPP_CTRL_OE_BIT;
	LL_GPIO_Init(HVPP_CTRL_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(HVPP_CTRL_OE_PORT, HVPP_CTRL_OE_BIT);

	//---HVPP_RDY_BSY---输入为高，该端口一直处于输入模式
	GPIO_InitStruct.Pin = HVPP_RDY_BSY_BIT;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(HVPP_RDY_BSY_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_RDY_BSY_PORT, HVPP_RDY_BSY_BIT);

	//---HVPP_OE---输入为高
	GPIO_InitStruct.Pin = HVPP_OE_BIT;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_OE_PORT, HVPP_OE_BIT);

	//---HVPP_WR---输入为高
	GPIO_InitStruct.Pin = HVPP_WR_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_WR_BIT);

	//---HVPP_BS1---输入为高
	GPIO_InitStruct.Pin = HVPP_BS1_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_BS1_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(HVPP_BS1_PORT, HVPP_BS1_BIT);
	
	//---HVPP_XA0---输入为高
	GPIO_InitStruct.Pin = HVPP_XA0_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_XA0_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA0_PORT, HVPP_XA0_BIT);

	//---HVPP_XA1---输入为高
	GPIO_InitStruct.Pin = HVPP_XA1_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_XA1_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA1_PORT, HVPP_XA1_BIT);

	//---HVPP_PAGEL---输入为高
	GPIO_InitStruct.Pin = HVPP_PAGEL_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_PAGEL_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(HVPP_PAGEL_PORT, HVPP_PAGEL_BIT);

	//---HVPP_BS2---输入为高
	GPIO_InitStruct.Pin = HVPP_BS2_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_BS2_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(HVPP_BS2_PORT, HVPP_BS2_BIT);

	//---HVPP_XTAL---输出为低
	GPIO_InitStruct.Pin = HVPP_XTAL_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_XTAL_PORT, &GPIO_InitStruct);
	GPIO_OUT_0(HVPP_XTAL_PORT, HVPP_XTAL_BIT);

	//---HVPP_DATA---输入为高
	GPIO_InitStruct.Pin = HVPP_DATA_BUS_BIT;
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_DATA_BUS_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_BUS_PORT, HVPP_DATA_BUS_BIT);
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(HVPP_DATA_BUS_PORT, &GPIO_InitStruct);
	//---设置数据总线状态为读取状态
	HVPPx->msgDataBusState = HVPP_DATA_BUS_MODE_READ;
	//---控制信号为输入模式
	HVPP_CTRL_DIR_TO_HZ;
	//---释放RST端口
	HVPP_DUT_RST_TO_HZ;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置端口为输出模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_GPIO_DeInit(HVPP_HandlerType* HVPPx)
{
	//---切换数据总线为设备到主机
	HVPP_DATA_BUS_DIR_TO_HOST;
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;																																		//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;																															//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;																															//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;																																			//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;																																		//---端口复用模式
#endif
	//---HVPP_RDY_BSY---输入为高，该端口一直处于输入模式
	GPIO_OUT_1(HVPP_RDY_BSY_PORT, HVPP_RDY_BSY_BIT);
	//---HVPP_OE---输入为高
	GPIO_InitStruct.Pin = HVPP_OE_BIT;
	LL_GPIO_Init(HVPP_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_OE_PORT, HVPP_OE_BIT);
	//---HVPP_WR---输入为高
	GPIO_InitStruct.Pin = HVPP_WR_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_WR_BIT);
	//---HVPP_BS1---输入为高
	GPIO_InitStruct.Pin = HVPP_BS1_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS1_BIT);
	//---HVPP_XA0---输入为高
	GPIO_InitStruct.Pin = HVPP_XA0_BIT;
	LL_GPIO_Init(HVPP_XA0_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA0_PORT, HVPP_XA0_BIT);
	//---HVPP_XA1---输入为高
	GPIO_InitStruct.Pin = HVPP_XA1_BIT;
	LL_GPIO_Init(HVPP_XA1_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA1_PORT, HVPP_XA1_BIT);
	//---HVPP_PAGEL---输入为高
	GPIO_InitStruct.Pin = HVPP_PAGEL_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_PAGEL_BIT);
	//---HVPP_BS2---输入为高
	GPIO_InitStruct.Pin = HVPP_BS2_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS2_BIT);
	//---HVPP_DATA---输入为高
	GPIO_InitStruct.Pin = HVPP_DATA_BUS_BIT;
	LL_GPIO_Init(HVPP_DATA_BUS_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_BUS_PORT, HVPP_DATA_BUS_BIT);
	//---控制信号为输入模式
	HVPP_CTRL_DIR_TO_HZ;
	//---数据为读取模式
	HVPPx->msgDataBusState=HVPP_DATA_BUS_MODE_READ;
	//---关闭数据总线
	HVPP_DATA_BUS_DIR_TO_HZ;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：时钟脉冲的个数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HVPP_XTAL_CLK(HVPP_HandlerType* HVPPx,UINT8_T clkNum)
{
	UINT8_T i=0;
	//---执行端口的翻转
	for (i=0;i<clkNum;i++)
	{
		HVPPx->msgDelayus(HVPPx->msgXtalPulseWidth);
		HVPP_XTAL_OUT_1;
		HVPPx->msgDelayus(HVPPx->msgXtalPulseWidth);
		HVPP_XTAL_OUT_0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：时钟脉冲的宽度
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HVPP_XTAL_PULSE(HVPP_HandlerType* HVPPx)
{
	HVPPx->msgDelayus(HVPPx->msgXtalPulseWidth);
	HVPP_XTAL_OUT_1;
	HVPPx->msgDelayus(HVPPx->msgXtalPulseWidth);
	HVPP_XTAL_OUT_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_Init(HVPP_HandlerType* HVPPx,void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	HVPPx->msgState = 0;
	HVPPx->msgXtalPulseWidth=5;
	HVPPx->msgHvFastMode=0;
	HVPPx->msgFactoryPattern=0;
	HVPPx->msgSelfPower=0;
	HVPPx->msgHvCmd=0x00;
	HVPPx->msgLoadCmd = 0x00;
	HVPPx->msgWaitms=1;
	HVPPx->msgProgWRLus=500;
	HVPPx->msgProgWRHus = 800;
	//---添加微妙延时
	if (pFuncDelayus!=NULL)
	{
		HVPPx->msgDelayus=pFuncDelayus;
	}
	else
	{
		HVPPx->msgDelayus =DelayTask_us;
	}
	//---添加毫秒延时
	if (pFuncDelayms != NULL)
	{
		HVPPx->msgDelayms = pFuncDelayms;
	}
	else
	{
		HVPPx->msgDelayms = DelayTask_ms;
	}
	//---添加定时节拍
	if (pFuncTimerTick!=NULL)
	{
		HVPPx->msgTimeTick=pFuncTimerTick;
	}
	else
	{
		HVPPx->msgTimeTick=SysTickTask_GetTick;
	}
	HVPP_GPIO_Init(HVPPx);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：注销初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_DeInit(HVPP_HandlerType* HVPPx)
{
	HVPPx->msgSelfPower = 0;
	HVPPx->msgXtalPulseWidth = 5;
	HVPP_GPIO_DeInit(HVPPx);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：添加设备1的监控函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HVPP_AddWatchDevice0(void)
{
	HVPP_WatchTask(HVPP_TASK_ONE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HVPP_WatchTask(HVPP_HandlerType* HVPPx)
{
	UINT32_T nowTime = 0;
	UINT32_T cnt = 0;
	if (HVPPx->msgState != 0)
	{
		//---获取当前时间节拍
		nowTime = HVPPx->msgTimeTick();
		//---计算时间间隔
		if (HVPPx->msgRecordTime > nowTime)
		{
			cnt = (0xFFFFFFFF - HVPPx->msgRecordTime + nowTime);
		}
		else
		{
			cnt = nowTime - HVPPx->msgRecordTime;
		}
		//if (cnt > ISP_STATE_TIME_OUT_MS)
		//---检查是否发生超时事件
		if (cnt > HVPPx->msgIntervalTime)
		{
			HVPP_ExitProg(HVPPx);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_AddWatch(HVPP_HandlerType* HVPPx)
{
	//---刷新时间
	HVPP_RefreshWatch(HVPPx);
	//---使用的HVPP的端口,并注册相应的任务函数
	SysTickTask_CreateTickTask(HVPP_AddWatchDevice0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_RemoveWatch(HVPP_HandlerType* HVPPx)
{
	SysTick_DeleteTickTask(HVPP_AddWatchDevice0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_RefreshWatch(HVPP_HandlerType* HVPPx)
{
	//---配置轮训间隔为最大值，单位是ms
	HVPPx->msgIntervalTime = HVPP_STATE_TIME_OUT_MS;
	//---刷新纪录时间
	HVPPx->msgRecordTime = HVPPx->msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_SetIntervalTime(HVPP_HandlerType* HVPPx, UINT16_T intervalTime)
{
	//---配置轮训间隔时间，单位是ms
	HVPPx->msgIntervalTime = intervalTime;
	//---刷新纪录时间
	HVPPx->msgRecordTime = HVPPx->msgTimeTick();
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_GetIntervalTime(HVPP_HandlerType* HVPPx)
{
	return HVPPx->msgIntervalTime;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：通过输入参数的不同，并行高压进入不容的模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ProgModeEnter(HVPP_HandlerType* HVPPx,UINT8_T pagel,UINT8_T xa1,UINT8_T xa0,UINT8_T bs1)
{
	//>>>---设置并行数据端口和并行数据控制端口未输入模式，且并行数据方向为设备到主机模式，之后端口处于高阻状态
	HVPP_DUT_RST_TO_GND;
	//---掉电，自供电的时候进行电源断电，否则为释放电源控制状态
	((HVPPx->msgSelfPower == 0) ? HVPP_DUT_POWER_OFF : HVPP_DUT_POWER_HZ);
	HVPPx->msgDelayms(1);
	//---切换数据宗先方向
	HVPP_DATA_BUS_DIR_TO_DEVICE;
	//---数据总线设置为写模式
	HVPP_DATA_BUS_WRITE;
	//---设置控制总线为写模式
	HVPP_CTRL_INIT_WRITE;
	//---等待状态稳定
	HVPP_WAIT_STATE_STABLE;
	//---上电，自供电的时候进行电源断电，否则为释放电源控制状态
	((HVPPx->msgSelfPower == 0) ? HVPP_DUT_POWER_ON : HVPP_DUT_POWER_HZ);
	HVPPx->msgDelayms(5);
	HVPP_XTAL_CLK(HVPPx,8);
	//---PAGEL
	((pagel != 0) ? HVPP_PAGEL_OUT_1 : HVPP_PAGEL_OUT_0);
	//---XA1
	((xa1 != 0) ? HVPP_XA1_OUT_1 : HVPP_XA1_OUT_0);
	//---XA0
	((xa0 != 0) ? HVPP_XA0_OUT_1 : HVPP_XA0_OUT_0);
	//---BS1
	((bs1 != 0) ? HVPP_BS1_OUT_1 : HVPP_BS1_OUT_0);
	//---保持一段时间不小于100ns
	HVPPx->msgDelayus(5);
	//---HV
	//HVPP_DUT_RST_TO_12V;
	//---保持一段时间不小于100ns
	HVPPx->msgDelayus(5);
	//---数据总线
	HVPPx->msgDataBusState = HVPP_DATA_BUS_MODE_WRITE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出并行高压模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ProgModeExit(HVPP_HandlerType* HVPPx)
{
	//---RST拉低到地
	HVPP_DUT_RST_TO_GND;
	//---数据端口设置为读取模式
	HVPP_DATA_BUS_READ;
	//---数据线方向使设备到主机
	HVPP_DATA_BUS_DIR_TO_HOST;
	//---控制端口设置为输出模式
	//HVPP_CTRL_BUS_VAL(0x7F);
	//HVPP_CTRL_INIT_READ;
	//---设置控制线为高阻态
	HVPP_CTRL_DIR_TO_HZ;
	//---关闭电源,自供电的时候进行电源断电，否则为释放电源控制状态
	((HVPPx->msgSelfPower == 0) ? HVPP_DUT_POWER_OFF : HVPP_DUT_POWER_HZ);
	HVPPx->msgDelayms(1);
	//---释放RST的控制
	HVPP_DUT_RST_TO_HZ;
	//---设置控制线为高阻态
	//HVPP_CTRL_DIR_TO_HZ;
	//---数据总线
	HVPPx->msgDataBusState = HVPP_DATA_BUS_MODE_READ;
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：进入编程模式
//////输入参数: hvModeCmd---高压命令，0x00---常规模式，0x0x---快速模式，或者其他模式
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_EnterProg(HVPP_HandlerType* HVPPx, UINT8_T hvProgCmd)
{
	HVPPx->msgState=1;
	//---校验命令是不是相同命令
	if (hvProgCmd!= HVPPx->msgHvCmd)
	{
		//---执行退出操作模式
		HVPP_ExitProg(HVPPx);
	}
	HVPPx->msgHvCmd=hvProgCmd;
	HVPPx->msgHvFastMode=((hvProgCmd!=0)?1:0);
	//---进入编程模式
	return  HVPP_ProgModeEnter(HVPPx,hvProgCmd&0x08, hvProgCmd & 0x04, hvProgCmd & 0x02,hvProgCmd & 0x01);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：退出编程模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ExitProg(HVPP_HandlerType* HVPPx)
{
	HVPPx->msgState = 0;
	HVPPx->msgHvFastMode = 0;
	HVPPx->msgHvCmd = 0x00;
	HVPPx->msgLoadCmd=0x00;
	return HVPP_ProgModeExit(HVPPx);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取准备好标志位
//////输入参数:
//////输出参数: 0---准备好，非0---未准备好，处于异常状态
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadReady(HVPP_HandlerType* HVPPx)
{
	UINT8_T _return = 0;
	//---获取时间标签
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT64_T cnt = 0;
	//---获取当前时间标签
	if (HVPPx->msgTimeTick != NULL)
	{
		oldTime = HVPPx->msgTimeTick();
	}
	//---查询忙标志位
	while (1)
	{
		//---检测RDY_BSY，0---忙碌状态，1---空闲状态
		_return=HVPP_RDY_BSY_STATE;
		//---校验是否是空闲状态，非0为空闲状态
		if (_return != 0x00)
		{
			_return = OK_0;
			break;
		}
		else
		{
			if (HVPPx->msgTimeTick != NULL)
			{
				//---当前时间
				nowTime = HVPPx->msgTimeTick();
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
//////功		能：芯片擦除操作
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_EraseChip(HVPP_HandlerType* HVPPx)
{
	UINT8_T _return=OK_0;
	UINT8_T i = 0;
	//---使能命令加载
	HVPP_ENABLE_CMD_LOAD;
	//---加载芯片擦除命令
	HVPP_DATABUS_WRITE(HVPPx, HVPP_CMD_CHIP_ERASE);
	//---给XTAL一个正脉冲用于加载命令
	HVPP_XTAL_PULSE(HVPPx);
	//---校验是不是快速模式
	if (HVPPx->msgHvFastMode==0)
	{
		//---等待擦除完成，标识是RDY_BSY从低电平变成高电平
		_return = HVPP_ReadReady(HVPPx);
	}
	else
	{
		//>>>---快速模式
		//---使能操作
		HVPP_WR_OUT_0;
		//---等待延时
		HVPPx->msgDelayus(HVPPx->msgProgWRLus);
		//---不使能操作
		HVPP_WR_OUT_1;
		//---等待延时
		HVPPx->msgDelayus(HVPPx->msgProgWRHus);
	}	
	//---释放状态,为空闲模式
	//HVPP_XA_LOAD_IDLE;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：并行高压模式下读取CHIP的ID
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadChipID(HVPP_HandlerType* HVPPx, UINT8_T* pVal)
{
	UINT8_T i=0;
	//---使能命令加载
	HVPP_ENABLE_CMD_LOAD;
	//---加载读取校准字和ID信息的命令
	HVPP_DATABUS_WRITE(HVPPx,HVPP_CMD_READ_ID_AND_CALIBRATION);
	//---给XTAL一个正脉冲用于加载命令
	HVPP_XTAL_PULSE(HVPPx);
	for (i=0;i<3;i++)
	{
		//---加载地址低字节
		HVPP_ENABLE_ADDR_LOAD_LOW;
		//---发送地址
		HVPP_DATABUS_WRITE(HVPPx, i);
		//---给XTAL一个正脉冲用于加载地址低字节
		HVPP_XTAL_PULSE(HVPPx);
		//---准备读取低字节数据
		HVPP_BS1_OUT_0;
		//---数据总线切换为读取模式
		HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_DIR);
		//---使能读取
		HVPP_OE_OUT_0;
		//---等待数据稳定
		HVPP_WAIT_STATE_STABLE;
		//---读取数据
		*(pVal++)= HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
		//---不使能读取
		HVPP_OE_OUT_1;
	}
	//---释放状态,为空闲模式
	//HVPP_XA_LOAD_IDLE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：并行高压模式下读取CHIP的校准字
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadChipCalibration(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T length)
{
	UINT8_T i = 0;
	//---使能命令加载
	HVPP_ENABLE_CMD_LOAD;
	//---加载读取校准字和ID信息的命令
	HVPP_DATABUS_WRITE(HVPPx, HVPP_CMD_READ_ID_AND_CALIBRATION);
	//---给XTAL一个正脉冲用于加载命令
	HVPP_XTAL_PULSE(HVPPx);
	for (i = 0; i < length; i++)
	{
		//---加载地址低字节
		HVPP_ENABLE_ADDR_LOAD_LOW;
		//---发送地址
		HVPP_DATABUS_WRITE(HVPPx, i);
		//---给XTAL一个正脉冲用于加载地址低字节
		HVPP_XTAL_PULSE(HVPPx);
		//---准备读取高字节数据
		HVPP_BS1_OUT_1;
		//---数据总线切换为读取模式
		HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_DIR);
		//---使能读取
		HVPP_OE_OUT_0;
		//---等待数据稳定
		HVPP_WAIT_STATE_STABLE;
		//---读取数据
		*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
		//---不使能读取
		HVPP_OE_OUT_1;
	}
	//---释放状态,为空闲模式
	//HVPP_XA_LOAD_IDLE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：并行高压模式下读取熔丝位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadChipFuse(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	//---使能命令加载
	HVPP_ENABLE_CMD_LOAD;
	//---加载读取熔丝位命令
	HVPP_DATABUS_WRITE(HVPPx, HVPP_CMD_READ_FUSE_AND_LOCK);
	//---给XTAL一个正脉冲用于加载命令
	HVPP_XTAL_PULSE(HVPPx);
	//---准备读取低位熔丝位
	HVPP_BS_OUT_00;
	//---数据总线切换为读取模式
	HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_DIR);
	//---使能读取
	HVPP_OE_OUT_0;
	//---等待数据稳定
	HVPP_WAIT_STATE_STABLE;
	//---读取数据
	*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
	//---准备读取高位熔丝位
	HVPP_BS_OUT_11;
	//---等待数据稳定
	HVPP_WAIT_STATE_STABLE;
	//---读取数据
	*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
	//---读取熔丝位拓展位
	if (isNeedExternFuse != 0x00)
	{
		//---准备读取高位熔丝位
		HVPP_BS_OUT_10;
		//---等待数据稳定
		HVPP_WAIT_STATE_STABLE;
		//---读取数据
		*(pVal) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
	}
	else
	{
		*pVal = 0xFF;
	}
	//---不使能读取
	HVPP_OE_OUT_1;
	//---释放状态,为空闲模式
	//HVPP_XA_LOAD_IDLE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：并行高压模式下读取加密位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadChipLock(HVPP_HandlerType* HVPPx, UINT8_T* pVal)
{
	//---使能命令加载
	HVPP_ENABLE_CMD_LOAD;
	//---加载读取熔丝位命令
	HVPP_DATABUS_WRITE(HVPPx, HVPP_CMD_READ_FUSE_AND_LOCK);
	//---给XTAL一个正脉冲用于加载命令
	HVPP_XTAL_PULSE(HVPPx);
	//---准备读取加密位
	HVPP_BS_OUT_01;
	//---数据总线切换为读取模式
	HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_DIR);
	//---使能读取
	HVPP_OE_OUT_0;
	//---等待数据稳定
	HVPP_WAIT_STATE_STABLE;
	//---读取数据
	*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
	//---不使能读取
	HVPP_OE_OUT_1;
	//---释放状态,为空闲模式
	//HVPP_XA_LOAD_IDLE;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_ReadChipRom(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T addr, UINT16_T length)
{
	UINT8_T i = 0;
	//---判断数据长度是否是偶数
	if ((length & 0x01) != 0)
	{
		return ERROR_1;
	}
	//---用移位运算当做除2运算
	length >>= 1;
	//---校验命令
	if (HVPPx->msgLoadCmd==0)
	{
		//---使能命令加载
		HVPP_ENABLE_CMD_LOAD;
		//---加载读取熔丝位命令
		HVPP_DATABUS_WRITE(HVPPx, HVPP_CMD_READ_ID_AND_CALIBRATION);
		//---给XTAL一个正脉冲用于加载命令
		HVPP_XTAL_PULSE(HVPPx);
	}
	//---读取ROM页信息，先低位后高位
	for ( i = 0; i < length; i++)
	{
		//---加载地址低字节
		HVPP_ENABLE_ADDR_LOAD_LOW;
		//---发送地址
		HVPP_DATABUS_WRITE(HVPPx, i+addr);
		//---给XTAL一个正脉冲用于加载地址低字节
		HVPP_XTAL_PULSE(HVPPx);
		//---准备读取低字节数据
		HVPP_BS1_OUT_0;
		//---数据总线切换为读取模式
		HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_DIR);
		//---使能读取
		HVPP_OE_OUT_0;
		//---等待数据稳定
		HVPP_WAIT_STATE_STABLE;
		*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
		//---准备读取高字节数据
		HVPP_BS1_OUT_1;
		//---等待数据稳定
		HVPP_WAIT_STATE_STABLE;
		//---读取数据
		*(pVal++) = HVPP_DATABUS_READ(HVPPx, HVPP_DATA_BUS_READ_VAL);
		//---不使能读取
		HVPP_OE_OUT_1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：擦除ROM页信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_EraseChipRom(HVPP_HandlerType* HVPPx)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程ID信息
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_WriteChipID(HVPP_HandlerType* HVPPx,UINT8_T* pVal)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程校准字
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_WriteChipCalibration(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T length)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程熔丝位,编程顺序依次是低位，高位，拓展位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_WriteChipFuse(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T isNeedExternFuse)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：编程加密位
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_WriteChipLock(HVPP_HandlerType* HVPPx, UINT8_T val)
{
	return OK_0;
}
