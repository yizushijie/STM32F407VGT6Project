#include "i2c_cfg.h"

//>>>===模拟I2C主机---开始
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_Init(I2C_HandlerType *I2Cx, void(*pFuncDelayus)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void))
{
	//---使能GPIO的时钟
	GPIOTask_Clock(I2Cx->msgSCL.msgPort, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(I2Cx->msgSDA.msgPort, PERIPHERAL_CLOCK_ENABLE);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---GPIO的初始化
	//GPIO_InitStruct.Pin = I2Cx->msgSclBit;						//---对应的GPIO的引脚
	GPIO_InitStruct.Pin = I2Cx->msgSCL.msgBit;					//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;				//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;					//---端口复用模式
#endif
	//---SCL的初始化
	LL_GPIO_Init(I2Cx->msgSCL.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---SDA的初始化
	GPIO_InitStruct.Pin = I2Cx->msgSDA.msgBit;
	LL_GPIO_Init(I2Cx->msgSDA.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	//---注册延时函数
	if (pFuncDelayus != NULL)
	{
		I2Cx->msgDelayus = pFuncDelayus;
	}
	else
	{
		I2Cx->msgDelayus = DelayTask_us;
	}
	if (pFuncTimerTick!=NULL)
	{
		I2Cx->msgTimeTick= pFuncTimerTick;
	}
	else
	{
		I2Cx->msgTimeTick = SysTickTask_GetTick;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_DeInit(I2C_HandlerType *I2Cx)
{
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---GPIO的初始化
	GPIO_InitStruct.Pin = I2Cx->msgSCL.msgBit;					//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;					//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif
	//---SCL的初始化
	LL_GPIO_Init(I2Cx->msgSCL.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---SDA的初始化
	GPIO_InitStruct.Pin = I2Cx->msgSDA.msgBit;
	LL_GPIO_Init(I2Cx->msgSDA.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_START(I2C_HandlerType *I2Cx)
{
	//---发送起始条件的数据信号
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---发送起始信号;
	GPIO_OUT_0(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---判断I2C启动信号是否成功，读取SDA的状态信号，成功SDA---0；失败SDA---1
	return  ((GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit) != 0x00)?ERROR_1:OK_0);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_STOP(I2C_HandlerType *I2Cx)
{
	//---发送停止条件的数据信号
	GPIO_OUT_0(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---判断I2C启动信号是否成功，读取SDA的状态信号，成功SDA---1；失败SDA---0
	return ((GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit) == 0x00) ? ERROR_1 : OK_0);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_ACK(I2C_HandlerType *I2Cx)
{
	GPIO_OUT_0(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_NACK(I2C_HandlerType *I2Cx)
{
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_ReadACK(I2C_HandlerType *I2Cx)
{
	UINT8_T _return = OK_0;
	//---读取应答信号
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---延时等待
	if (I2Cx->msgPluseWidth>0)
	{
		I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	}
	//---读取SDA的状态信号---ACK状态下SDA为低电平
	_return= ((GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit) != 0x00) ? ERROR_1 : OK_0);
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_WaitACK(I2C_HandlerType *I2Cx)
{
	UINT8_T i = 0;
	//---读取应答信号
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---等待应答信号
	for (i = 255; i > 0; i--)
	{
		//---读取SDA的状态
		if (GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit))
		{
			break;
		}
	}
	if (i == 0)
	{
		I2C_MSW_STOP(I2Cx);
		return ERROR_1;
	}
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_SendACK(I2C_HandlerType *I2Cx, UINT8_T isNACK)
{
	if (isNACK)
	{
		return I2C_MSW_NACK(I2Cx);
	}
	else
	{
		return I2C_MSW_ACK(I2Cx);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_SendBit(I2C_HandlerType *I2Cx, UINT8_T bitVal)
{
	((bitVal&0x80)!=0x00)? GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit): GPIO_OUT_0(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	//---一个时钟脉冲
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);	
	I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	DELAY_NOP_COUNT(2);
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);	
	I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_SendByte(I2C_HandlerType *I2Cx, UINT8_T val)
{
	UINT8_T i = 0;
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---发送1字节的数据
	for (i = 0; i < 8; i++)
	{
		//---读取1Bit的数据
		I2C_MSW_SendBit(I2Cx, val);
		val <<= 1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_SendBits(I2C_HandlerType *I2Cx, UINT8_T *pVal, UINT8_T bitNum)
{
	//---计算字节个数
	UINT8_T byteCount = (bitNum / 8);
	//---计算bit个数
	UINT8_T bitCount = (bitNum % 8);
	UINT8_T i = 0;
	//---发送整数字节的个数
	for (i = 0; i < byteCount; i++)
	{
		I2C_MSW_SendByte(I2Cx, pVal[i]);
	}
	//---数据左移---数据的发送，首先发送高位
	pVal[byteCount] <<= (8 - bitCount);
	//---判断发送数据是不是不满足1字节
	if (byteCount == 0)
	{
		//---清时钟线,钳住I2C总线，准备发送或接收数据
		GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	}
	//---发送指定个数的bit
	for (i = 0; i < bitCount; i++)
	{
		I2C_MSW_SendBit(I2Cx, pVal[byteCount]);
		pVal[byteCount] <<= 1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_SendWord(I2C_HandlerType *I2Cx, UINT16_T val)
{
	UINT8_T valH = (UINT8_T)(val >> 8);
	UINT8_T valL = (UINT8_T)(val);
	UINT8_T i = 0;
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---发送1字节的数据
	for (i = 0; i < 8; i++)
	{
		//---读取1Bit的数据
		I2C_MSW_SendBit(I2Cx, valH);
		//---数据左移动
		valH <<= 1;
	}
	//---发送1字节的数据
	for (i = 0; i < 8; i++)
	{
		//---读取1Bit的数据
		I2C_MSW_SendBit(I2Cx, valL);
		//---数据右移
		valL <<= 1;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_ReadBit(I2C_HandlerType *I2Cx)
{
	UINT8_T _return = OK_0;
	//---时钟正脉冲
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	DELAY_NOP_COUNT(4);
	//---读取数据位
	_return= ((GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit) != 0x00) ? 1 : 0);
	//---时钟负脉冲
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	I2Cx->msgDelayus(I2Cx->msgPluseWidth);
	return  _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_ReadByte(I2C_HandlerType *I2Cx)
{
	UINT8_T i = 0;
	UINT8_T _return = 0;
	//---准备数据的读取
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
	//---清时钟线,钳住I2C总线，准备发送或接收数据
	GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---发送1字节的数据
	for (i = 0; i < 8; i++)
	{
		_return <<= 1;
		_return |= I2C_MSW_ReadBit(I2Cx);
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
UINT8_T I2C_MSW_ReadBits(I2C_HandlerType *I2Cx, UINT8_T *pVal, UINT8_T bitNum)
{
	//---读取字节个数
	UINT8_T byteCount = (bitNum / 8);
	//---读取bit个数
	UINT8_T bitCount = (bitNum % 8);
	UINT8_T i = 0;
	//---读取整数字节的个数
	for (i = 0; i < byteCount; i++)
	{
		pVal[i] = I2C_MSW_ReadByte(I2Cx);
	}
	//---清零处理
	pVal[byteCount] = 0x00;
	//---判断读取的Bit数据是不是不满足1字节
	if (byteCount == 0)
	{
		//---准备数据的读取
		GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);
		//---清时钟线,钳住I2C总线，准备发送或接收数据
		GPIO_OUT_0(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	}
	//---读取指定个数的bit
	for (i = 0; i < bitCount; i++)
	{
		pVal[byteCount] <<= 1;
		pVal[byteCount] |= I2C_MSW_ReadBit(I2Cx);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MSW_CheckDevice(I2C_HandlerType *I2Cx)
{
	UINT8_T _return = OK_0;
	if ((GPIO_GET_STATE(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit)) && (GPIO_GET_STATE(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit)))
	{
		//---发送启动信号
		I2C_MSW_START(I2Cx);
		//---发送读取操作命令
		I2Cx->msgAddr |= 0x01;
		//---发送从机地址
		I2C_MSW_SendByte(I2Cx, (UINT8_T)I2Cx->msgAddr);
		//---读取应答信号
		_return = I2C_MSW_ReadACK(I2Cx);
	}
	else
	{
		_return = ERROR_2;
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
UINT8_T I2C_MSW_SendCMD(I2C_HandlerType *I2Cx, UINT8_T cmd, UINT8_T isStart, UINT8_T isStop)
{
	UINT8_T _return = OK_0;
	if (isStart)
	{
		//---发送启动启动信号
		_return = I2C_MSW_START(I2Cx);
		if (_return)
		{
			I2C_MSW_STOP(I2Cx);
			return ERROR_1;
		}
	}
	//---发送数据
	_return = I2C_MSW_SendByte(I2Cx, cmd);
	//---读取应答
	_return = I2C_MSW_ReadACK(I2Cx);
	//---停止条件的满足
	if ((isStop) || (_return))
	{
		I2C_MSW_STOP(I2Cx);
	}
	return _return;
}
//////////////////////////////////////////////////////////////////////////////
//<<<===模拟I2C主机---结束

//////////////////////////////////////////////////////////////////////////////
//>>>===硬件I2C主机---开始


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：使能I2C的外设时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_Clock(I2C_HandlerType* I2Cx, UINT8_T isEnable)
{
	if (I2Cx->msgI2Cx == I2C1)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
	else if (I2Cx->msgI2Cx == I2C2)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C2);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#ifdef I2C3
	else if (I2Cx->msgI2Cx == I2C3)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C3);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif
	else
	{
		return ERROR_2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MHW_Init(I2C_HandlerType* I2Cx)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(I2Cx->msgSCL.msgPort, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(I2Cx->msgSDA.msgPort, PERIPHERAL_CLOCK_ENABLE);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---GPIO的初始化
	//GPIO_InitStruct.Pin = I2Cx->msgSclBit;														//---对应的GPIO的引脚
	GPIO_InitStruct.Pin = I2Cx->msgSCL.msgBit;														//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;													//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;												//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;											//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = I2Cx->msgGPIOAlternate;												//---端口复用模式
#endif
	//---SCL的初始化
	LL_GPIO_Init(I2Cx->msgSCL.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSCL.msgPort, I2Cx->msgSCL.msgBit);
	//---SDA的初始化
	GPIO_InitStruct.Pin = I2Cx->msgSDA.msgBit;
	LL_GPIO_Init(I2Cx->msgSDA.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(I2Cx->msgSDA.msgPort, I2Cx->msgSDA.msgBit);

	//---开启I2C外设时钟
	I2C_Clock(I2Cx, PERIPHERAL_CLOCK_ENABLE);
	//---复位I2C
	LL_I2C_DeInit(I2Cx->msgI2Cx);
	//---初始化I2C
	LL_I2C_InitTypeDef I2C_InitStruct={0};
	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;												//---工作模式
	I2C_InitStruct.ClockSpeed = 100000;																//---SCL时钟频率
	I2C_InitStruct.DutyCycle = LL_I2C_DUTYCYCLE_2;													//---时钟占空比
	I2C_InitStruct.OwnAddress1 = 0;																	//---自身的I2C设备地址
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;													//---使能或者关闭相应(默认一般开启)
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;											//---指定地址的长度，7Bit或者10BIt
	LL_I2C_Init(I2Cx->msgI2Cx, &I2C_InitStruct);
	//---第二个地址
	LL_I2C_SetOwnAddress2(I2Cx->msgI2Cx, 0);
	//---使能I2C
	LL_I2C_Enable(I2Cx->msgI2Cx);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送启动信号
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MHW_START(I2C_HandlerType* I2Cx)
{
	/* 1.保证I2C外设不在使用中. */
	while (LL_I2C_IsActiveFlag_BUSY(I2Cx->msgI2Cx))
	{
	}
	/* 2.发送START信号 */
	LL_I2C_GenerateStartCondition(I2Cx->msgI2Cx);
	while (!LL_I2C_IsActiveFlag_SB(I2Cx->msgI2Cx))
	{
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送停止信号
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MHW_STOP(I2C_HandlerType* I2Cx)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MHW_SendByte(I2C_HandlerType* I2Cx, UINT8_T val)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取数据
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T I2C_MHW_ReadByte(I2C_HandlerType* I2Cx)
{
	return OK_0;
}


void I2C_HW_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
	/* 1.保证I2C外设不在使用中. */
	while (LL_I2C_IsActiveFlag_BUSY(I2C1))
	{
	}
	/* 2.发送START信号 */
	LL_I2C_GenerateStartCondition(I2C1);
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
	}
	/* 2.写器件地址 */
	LL_I2C_TransmitData8(I2C1, Addr);
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	/* 3.地址位已经置位,通常TXE也会完成,为了谨慎,再查一下. */
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
	{
	}
	/* 4.发送器件寄存器地址. */
	LL_I2C_TransmitData8(I2C1, Reg);
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
	{
	}
	/* 5.写入寄存器内容 */
	LL_I2C_TransmitData8(I2C1, Value);
	while (!LL_I2C_IsActiveFlag_BTF(I2C1))
	{
	}
	/* 6.传送结束条件. */
	LL_I2C_GenerateStopCondition(I2C1);
}
uint8_t I2C_HW_Read(uint8_t Addr, uint8_t Reg)
{
	uint8_t Value = 0x00;
	/* 1.保证I2C外设不在使用中. */
	while (LL_I2C_IsActiveFlag_BUSY(I2C1))
	{
	}
	/* 2.发送START信号 */
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
	LL_I2C_GenerateStartCondition(I2C1);
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
	}
	/* 2.写器件地址 */
	LL_I2C_TransmitData8(I2C1, Addr);
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
	}
	LL_I2C_ClearFlag_ADDR(I2C1);
	/* 3.地址位已经置位,通常TXE也会完成,为了谨慎,再查一下. */
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
	{
	}
	/* 4.发送器件寄存器地址. */
	LL_I2C_TransmitData8(I2C1, Reg);
	while (!LL_I2C_IsActiveFlag_TXE(I2C1))
	{
	}
	/* 5.提供RESTART信号. */
	LL_I2C_GenerateStopCondition(I2C1);
	LL_I2C_GenerateStartCondition(I2C1);
	while (!LL_I2C_IsActiveFlag_SB(I2C1))
	{
	}
	/* 6.重新发送地址,并附带读标记. */
	LL_I2C_TransmitData8(I2C1, Addr | 0x01);
	while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
	{
	}
	/* 7.标记读完就STOP. */
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
	LL_I2C_ClearFlag_ADDR(I2C1);
	LL_I2C_GenerateStopCondition(I2C1);
	while (!LL_I2C_IsActiveFlag_RXNE(I2C1))
	{
	}
	/* 8.数据已读取. */
	Value = LL_I2C_ReceiveData8(I2C1);
	return Value;
}

//////////////////////////////////////////////////////////////////////////////
//<<<===硬件I2C主机---结束