#include "hvpp_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HVPP_DataBus_Write(HVPP_HandlerType* HVPPx,UINT8_T val )
{
	//---校验数据总线是不是写入状态
	if (HVPPx->msgDataBusState!=HVPP_DATA_BUS_STATE_WRITE)
	{
		//---设置为高阻态
		HVPP_DATA_BUS_TO_HZ;
		//---设置端口为输出模式
		HVPP_DATA_BUS_WRITE;
		HVPPx->msgDataBusState = HVPP_DATA_BUS_STATE_WRITE;
		//---切换数据流为主机到设备
		HVPP_DATA_BUS_TO_DEVICE;
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
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_DataBus_Read(HVPP_HandlerType* HVPPx)
{
	UINT8_T _return=0;
	//---校验数据总线是不是读取状态
	if (HVPPx->msgDataBusState != HVPP_DATA_BUS_STATE_READ)
	{
		//---设置为高阻态
		HVPP_DATA_BUS_TO_HZ;
		//---设置端口为输入模式
		HVPP_DATA_BUS_READ;
		HVPPx->msgDataBusState = HVPP_DATA_BUS_STATE_READ;
		//---切换数据流为设备到主机
		HVPP_DATA_BUS_TO_HOST;
	}
	//---读取数据
	_return |= (HVPP_DATA0_STATE != 0 ? 0x80 : 0x00);
	_return |= (HVPP_DATA1_STATE != 0 ? 0x40 : 0x00);
	_return |= (HVPP_DATA2_STATE != 0 ? 0x20 : 0x00);
	_return |= (HVPP_DATA3_STATE != 0 ? 0x10 : 0x00);
	_return |= (HVPP_DATA4_STATE != 0 ? 0x08 : 0x00);
	_return |= (HVPP_DATA5_STATE != 0 ? 0x04 : 0x00);
	_return |= (HVPP_DATA6_STATE != 0 ? 0x02 : 0x00);
	_return |= (HVPP_DATA7_STATE != 0 ? 0x01 : 0x00);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_Device_Init(HVPP_HandlerType* HVPPx)
{
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;														//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;											//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;											//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	//---DATA_OE---输出为高，电平转换芯片为高阻态
	GPIO_InitStruct.Pin = HVPP_DATA_OE_BIT;
	LL_GPIO_Init(HVPP_DATA_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_OE_PORT, HVPP_DATA_OE_BIT);

	//---DATA_DIR---输出为高，数据从主机到设备
	GPIO_InitStruct.Pin = HVPP_DATA_DIR_BIT;
	LL_GPIO_Init(HVPP_DATA_DIR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_DIR_PORT, HVPP_DATA_DIR_BIT);

	//---CTRL_OE---输出为高，电平转换芯片为高阻态
	GPIO_InitStruct.Pin = HVPP_CTRL_OE_BIT;
	LL_GPIO_Init(HVPP_CTRL_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_CTRL_OE_PORT, HVPP_CTRL_OE_BIT);

	//---HVPP_RDY_BSY---输入为高
	GPIO_InitStruct.Pin  = HVPP_RDY_BSY_BIT;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(HVPP_RDY_BSY_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_RDY_BSY_PORT, HVPP_RDY_BSY_BIT);

	//---HVPP_OE---输出为高
	GPIO_InitStruct.Pin = HVPP_OE_BIT;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(HVPP_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_OE_PORT, HVPP_OE_BIT);

	//---HVPP_WR---输出为高
	GPIO_InitStruct.Pin = HVPP_WR_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_WR_BIT);

	//---HVPP_BS1---输出为高
	GPIO_InitStruct.Pin = HVPP_BS1_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS1_BIT);

	//---HVPP_XA0---输出为高
	GPIO_InitStruct.Pin = HVPP_XA0_BIT;
	LL_GPIO_Init(HVPP_XA0_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA0_PORT, HVPP_XA0_BIT);

	//---HVPP_XA1---输出为高
	GPIO_InitStruct.Pin = HVPP_XA1_BIT;
	LL_GPIO_Init(HVPP_XA1_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA1_PORT, HVPP_XA1_BIT);

	//---HVPP_PAGEL---输出为高
	GPIO_InitStruct.Pin = HVPP_PAGEL_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_PAGEL_BIT);

	//---HVPP_BS2---输出为高
	GPIO_InitStruct.Pin = HVPP_BS2_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS2_BIT);

	//---HVPP_DATA---输入为高
	GPIO_InitStruct.Pin = HVPP_DATA_BUS_BIT;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(HVPP_DATA_BUS_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_BUS_PORT, HVPP_DATA_BUS_BIT);
	//---设置数据总线状态为读取状态
	HVPPx->msgDataBusState=HVPP_DATA_BUS_STATE_READ;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置端口为输出模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_Device_DeInit(HVPP_HandlerType* HVPPx)
{
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;														//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;											//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;											//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	//---HVPP_RDY_BSY---输入为高
	GPIO_InitStruct.Pin = HVPP_RDY_BSY_BIT;
	LL_GPIO_Init(HVPP_RDY_BSY_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_RDY_BSY_PORT, HVPP_RDY_BSY_BIT);

	//---HVPP_OE---输出为高
	GPIO_InitStruct.Pin = HVPP_OE_BIT;
	LL_GPIO_Init(HVPP_OE_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_OE_PORT, HVPP_OE_BIT);

	//---HVPP_WR---输出为高
	GPIO_InitStruct.Pin = HVPP_WR_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_WR_BIT);

	//---HVPP_BS1---输出为高
	GPIO_InitStruct.Pin = HVPP_BS1_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS1_BIT);

	//---HVPP_XA0---输出为高
	GPIO_InitStruct.Pin = HVPP_XA0_BIT;
	LL_GPIO_Init(HVPP_XA0_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA0_PORT, HVPP_XA0_BIT);

	//---HVPP_XA1---输出为高
	GPIO_InitStruct.Pin = HVPP_XA1_BIT;
	LL_GPIO_Init(HVPP_XA1_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_XA1_PORT, HVPP_XA1_BIT);

	//---HVPP_PAGEL---输出为高
	GPIO_InitStruct.Pin = HVPP_PAGEL_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_PAGEL_BIT);

	//---HVPP_BS2---输出为高
	GPIO_InitStruct.Pin = HVPP_BS2_BIT;
	LL_GPIO_Init(HVPP_WR_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_WR_PORT, HVPP_BS2_BIT);

	//---HVPP_DATA---输入为高
	GPIO_InitStruct.Pin = HVPP_DATA_BUS_BIT;
	LL_GPIO_Init(HVPP_DATA_BUS_PORT, &GPIO_InitStruct);
	GPIO_OUT_1(HVPP_DATA_BUS_PORT, HVPP_DATA_BUS_BIT);

	//---切换数据为设备到主机
	HVPP_DATA_BUS_TO_HOST;
	//---控制信号为输入模式
	HVPP_CTRL_TO_HZ;
	//---数据为读取模式
	HVPPx->msgDataBusState=HVPP_DATA_BUS_STATE_READ;
	//---关闭数据总线
	HVPP_DATA_BUS_TO_HZ;
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
	//---使能控制总线的端口时钟
	GPIOTask_Clock(HVPP_RDY_BSY_PORT, 1);
	GPIOTask_Clock(HVPP_OE_PORT, 1);
	GPIOTask_Clock(HVPP_WR_PORT, 1);
	GPIOTask_Clock(HVPP_BS1_PORT, 1);
	GPIOTask_Clock(HVPP_XA0_PORT, 1);
	GPIOTask_Clock(HVPP_XA1_PORT, 1);
	GPIOTask_Clock(HVPP_PAGEL_PORT, 1);
	GPIOTask_Clock(HVPP_BS2_PORT, 1);
	GPIOTask_Clock(HVPP_XTAL_PORT, 1);
	GPIOTask_Clock(HVPP_CTRL_OE_PORT, 1);

	//---使能数据总线的端口时钟
	GPIOTask_Clock(HVPP_DATA0_PORT, 1);
	GPIOTask_Clock(HVPP_DATA1_PORT, 1);
	GPIOTask_Clock(HVPP_DATA2_PORT, 1);
	GPIOTask_Clock(HVPP_DATA3_PORT, 1);
	GPIOTask_Clock(HVPP_DATA4_PORT, 1);
	GPIOTask_Clock(HVPP_DATA5_PORT, 1);
	GPIOTask_Clock(HVPP_DATA6_PORT, 1);
	GPIOTask_Clock(HVPP_DATA7_PORT, 1);
	GPIOTask_Clock(HVPP_DATA_OE_PORT, 1);
	GPIOTask_Clock(HVPP_DATA_DIR_PORT, 1);
	//---配置端口的初始化状态
	HVPP_Device_Init(HVPPx);
	//---注册读取函数
	HVPPx->msgFuncReadDataBus = HVPP_DataBus_Read;
	//---注册写入函数
	HVPPx->msgFuncWriteDataBus = HVPP_DataBus_Write;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T HVPP_Init(HVPP_HandlerType* HVPPx,void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	
}