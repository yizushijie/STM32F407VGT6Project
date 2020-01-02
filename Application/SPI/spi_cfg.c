#include "spi_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： SPI的主机模式的查询方式初始化
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MHW_PollMode_Init(SPI_HandlerType *SPIx, LL_SPI_InitTypeDef SPI_InitStruct)
{
	//---恢复当前配置为初始值
	LL_SPI_DeInit(SPIx->msgSPIx);

	//---使能SPI的时钟线
	SPI_Clock(SPIx, 1);

	//---SPI初始化
	LL_SPI_Init(SPIx->msgSPIx, &(SPI_InitStruct));
	#ifndef USE_MCU_STM32F1
		//---SPI1的标准协议的支持
		LL_SPI_SetStandard(SPIx->msgSPIx, SPIx->msgStandard);
	#endif
	//---使能SPI
	LL_SPI_Enable(SPIx->msgSPIx);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 设置是高位在在前还是低位在前
//////////////////////////////////////////////////////////////////////////////
void SPI_MHW_SetTransferBitOrder(SPI_HandlerType *SPIx, UINT32_T BitOrder)
{
	LL_SPI_SetTransferBitOrder(SPIx->msgSPIx, BitOrder);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 端口初始化
//////输入参数:
//////输出参数:
//////说		明： 硬件SPI资源时的GPIO端口初始化
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MHW_GPIO_Init(SPI_HandlerType *SPIx)
{
	//---端口时钟的配置
	GPIOTask_Clock(SPIx->msgSCK.msgPort,  PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(SPIx->msgMOSI.msgPort, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(SPIx->msgMISO.msgPort, PERIPHERAL_CLOCK_ENABLE);
	//---端口的配置
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;//LL_GPIO_PULL_UP;
	#ifndef USE_MCU_STM32F1
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	#endif
	if (SPIx->msgCS.msgPort!=NULL)	
	{
		GPIOTask_Clock(SPIx->msgCS.msgPort, PERIPHERAL_CLOCK_ENABLE);
		//---SS---设置为输出
		GPIO_InitStruct.Pin = SPIx->msgCS.msgBit;
		LL_GPIO_Init(SPIx->msgCS.msgPort, &GPIO_InitStruct);
		GPIO_OUT_1(SPIx->msgCS.msgPort, SPIx->msgCS.msgBit);
	}
	#ifndef USE_MCU_STM32F1
		GPIO_InitStruct.Alternate = SPIx->msgGPIOAlternate;
	#endif
	//---SCK---设置为输出
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pin = SPIx->msgSCK.msgBit;
	LL_GPIO_Init(SPIx->msgSCK.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);

	//---MOSI---设置为输出
	//GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Pin = SPIx->msgMOSI.msgBit;
	LL_GPIO_Init(SPIx->msgMOSI.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);

	//---MISO---设置为输入
	GPIO_InitStruct.Pin = SPIx->msgMISO.msgBit;
	LL_GPIO_Init(SPIx->msgMISO.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_GPIO_Init(SPI_HandlerType *SPIx)
{
	//---端口时钟的配置
	GPIOTask_Clock(SPIx->msgSCK.msgPort,  PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(SPIx->msgMOSI.msgPort, PERIPHERAL_CLOCK_ENABLE);
	GPIOTask_Clock(SPIx->msgMISO.msgPort, PERIPHERAL_CLOCK_ENABLE);
	//---端口的配置
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	#ifndef USE_MCU_STM32F1
		GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	#endif
	if (SPIx->msgCS.msgPort!=NULL)
	{
		GPIOTask_Clock(SPIx->msgCS.msgPort, PERIPHERAL_CLOCK_ENABLE);
		//---SS---设置为输出
		GPIO_InitStruct.Pin = SPIx->msgCS.msgBit;
		LL_GPIO_Init(SPIx->msgCS.msgPort, &GPIO_InitStruct);
		GPIO_OUT_1(SPIx->msgCS.msgPort, SPIx->msgCS.msgBit);
	}
	//---SCK---设置为输出
	GPIO_InitStruct.Pin = SPIx->msgSCK.msgBit;
	LL_GPIO_Init(SPIx->msgSCK.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
	//---MOSI---设置为输出
	GPIO_InitStruct.Pin = SPIx->msgMOSI.msgBit;
	LL_GPIO_Init(SPIx->msgMOSI.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	//---MISO---设置为输入
	GPIO_InitStruct.Pin = SPIx->msgMISO.msgBit;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(SPIx->msgMISO.msgPort, &GPIO_InitStruct);	
	GPIO_OUT_1(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_GPIO_DeInit(SPI_HandlerType *SPIx,UINT8_T isInitSS)
{
	//---端口的配置
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
#endif
	if ((isInitSS!=0)&&(SPIx->msgCS.msgPort!=NULL))
	{
		//---SS---设置为输入
		GPIO_InitStruct.Pin = SPIx->msgCS.msgBit;
		LL_GPIO_Init(SPIx->msgCS.msgPort, &GPIO_InitStruct);
		GPIO_OUT_1(SPIx->msgCS.msgPort, SPIx->msgCS.msgBit);
	}
	//---SCK---设置为输入
	GPIO_InitStruct.Pin = SPIx->msgSCK.msgBit;
	LL_GPIO_Init(SPIx->msgSCK.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
	//---MOSI---设置为输入
	GPIO_InitStruct.Pin = SPIx->msgMOSI.msgBit;
	LL_GPIO_Init(SPIx->msgMOSI.msgPort, &GPIO_InitStruct);
	GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	//---MISO---设置为输入
	GPIO_InitStruct.Pin = SPIx->msgMISO.msgBit;
	LL_GPIO_Init(SPIx->msgMISO.msgPort, &GPIO_InitStruct);	
	GPIO_OUT_1(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit);
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_DeInit(SPI_HandlerType *SPIx, UINT8_T isInitSS)
{
	if (SPIx->msgSPIx != NULL)
	{
		//---恢复当前配置为初始值
		LL_SPI_DeInit(SPIx->msgSPIx);
		//---不使能SPI的时钟线
		SPI_Clock(SPIx, 0);
	}
	return SPI_GPIO_DeInit(SPIx, isInitSS);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_Clock(SPI_HandlerType *SPIx, UINT8_T isEnable)
{
	//---选择SPI接口
	if (SPIx->msgSPIx == SPI1)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI1);
		}
	}
	else if (SPIx->msgSPIx == SPI2)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI2);
		}
	}
	else if (SPIx->msgSPIx == SPI3)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_SPI3);
		}
	}
#ifdef SPI4
	else if (SPIx->msgSPIx == SPI4)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI4);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI4);
		}
	}
#endif
#ifdef SPI5
	else if (SPIx->msgSPIx == SPI5)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI5);
		}
	}
#endif
#ifdef SPI6
	else if (SPIx->msgSPIx == SPI6)
	{
		if (isEnable)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI6);
		}
		else
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SPI6);
		}
	}
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数: 0---成功，!=0---失败
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MHW_PollMode_WriteAndReadByte(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	//---获取当前时间节拍
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT32_T cnt = 0;
	//---收发完成标志位
	UINT8_T _return = 2;
	//---获取当前时间节拍
	if (SPIx->msgTimeTick != NULL)
	{
		oldTime = SPIx->msgTimeTick();
	}
	//---切换工作状态为工作模式
	SPIx->msgState = 1;
	//---数据收发
	while (1)
	{
		//---等待发送缓冲区为空，TXE 事件---TXE=1，开始发送下一个数据
		if ((_return==2)&&(LL_SPI_IsActiveFlag_TXE(SPIx->msgSPIx)))
		{
			//---写入数据寄存器，把要写入的数据写入发送缓冲区
			LL_SPI_TransmitData8(SPIx->msgSPIx, wVal);
			//---1
			_return = 1;
		}
		//---等待接收缓冲区非空，RXNE 事件---等待RXNE=1，读取收到的数据
		if (LL_SPI_IsActiveFlag_RXNE(SPIx->msgSPIx))
		{
			//---读取数据寄存器，获取接收缓冲区数据
			_return = LL_SPI_ReceiveData8(SPIx->msgSPIx);
			//---判断数据缓存区是否为空
			if (pRVal!=NULL)
			{
				*pRVal = _return;
			}
			//---0
			_return =0;
		}
		//---超时判断
		if (SPIx->msgTimeTick != NULL)
		{
			//---当前时间
			nowTime = SPIx->msgTimeTick();
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
				_return = 3;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 100000)
			{
				//---发送发生超时错误
				_return = 4;
			}
		}
		//---退出循环
		if ((_return==0)||(_return>2))
		{
			break;
		}
	}
	//---切换工作状态为空闲模式
	SPIx->msgState = 0;
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MHW_PollMode_WriteAndReadData(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	//---获取当前时间节拍
	UINT32_T nowTime = 0;
	UINT32_T oldTime = 0;
	UINT32_T cnt = 0;
	//---收发完成标志位
	UINT8_T txAllowed = 1;
	//---获取当前时间节拍
	if (SPIx->msgTimeTick != NULL)
	{
		oldTime = SPIx->msgTimeTick();
	}
	nowTime = 0;
	//---切换工作状态为工作模式
	SPIx->msgState=1;
	//---数据收发
	while (1)
	{
		//---等待发送缓冲区为空，TXE 事件---TXE=1，开始发送下一个数据
		if ((txAllowed == 1) && (LL_SPI_IsActiveFlag_TXE(SPIx->msgSPIx)))
		{
			//---写入数据寄存器，把要写入的数据写入发送缓冲区
			LL_SPI_TransmitData8(SPIx->msgSPIx, pWVal[i]);
			//---接收
			txAllowed = 0;
		}
		//---等待接收缓冲区非空，RXNE 事件---等待RXNE=1，读取收到的数据
		if ((txAllowed==0)&&(LL_SPI_IsActiveFlag_RXNE(SPIx->msgSPIx)))
		{
			//---读取数据寄存器，获取接收缓冲区数据
			_return = LL_SPI_ReceiveData8(SPIx->msgSPIx);
			//---判断数据缓存区是否为空
			if (pRVal != NULL)
			{
				pRVal[i] = _return;
			}
			_return=0;
			//---发送
			txAllowed = 1;
			//---下一个数据
			i++;
		}
		//---超时判断
		if (SPIx->msgTimeTick != NULL)
		{
			//---当前时间
			nowTime = SPIx->msgTimeTick();
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
				_return = 0x80+i;
			}
		}
		else
		{
			nowTime++;
			if (nowTime > 100000)
			{
				//---发送发生超时错误
				_return = 0x40+i;
			}
		}
		//---退出循环
		if ((_return != 0)||(i==length))
		{
			break;
		}
	}
	//---切换工作状态为空闲模式
	SPIx->msgState = 1;
	return _return;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：高位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteBitMSB(SPI_HandlerType *SPIx, UINT8_T wVal)
{
	//---发送1bit的数据
	/*if ((wVal & 0x80) == 0x00)
	{
		GPIO_OUT_0(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}*/
	((wVal & 0x80) != 0x00) ? GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit) : GPIO_OUT_0(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 高位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_ReadBitMSB(SPI_HandlerType *SPIx,UINT8_T *pRVal)
{
	//---读取1bit的数据
	if (pRVal != NULL)
	{
		/*if (GPIO_GET_STATE(SPIx->msgMISO.msgGPIOPort, SPIx->msgMISO.msgGPIOBit) != 0)
		{
			*pRVal |= 1;
		}*/
		*pRVal |= ((GPIO_GET_STATE(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit) != 0) ? 1 : 0);
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
UINT8_T SPI_MSW_BitMSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	//---发送1bit的数据
	/*if ((wVal & 0x80) == 0x00)
	{
		GPIO_OUT_0(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}*/
	((wVal & 0x80) != 0x00) ? GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit) : GPIO_OUT_0(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	//---读取1bit的数据
	if (pRVal != NULL)
	{
		/*if (GPIO_GET_STATE(SPIx->msgMISO.msgGPIOPort, SPIx->msgMISO.msgGPIOBit) != 0)
		{
			*pRVal |= 1;
		}*/
		*pRVal |=((GPIO_GET_STATE(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit) != 0)?1:0);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 读写1Bit的数据，高位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadBitMSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	//---00---11
	if (((SPIx->msgCPOL == 0) && (SPIx->msgCPOH == 0)) || ((SPIx->msgCPOL == 1) && (SPIx->msgCPOH == 1)))
	{
		GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
		DELAY_NOP_COUNT(4);
		SPI_MSW_BitMSB(SPIx, wVal, pRVal);
		GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
	}
	//---10---01
	//if (((SPIx->msgCPOL == 1) && (SPIx->msgCPOH == 0))|| ((SPIx->msgCPOL == 0) && (SPIx->msgCPOH == 1)))
	else
	{
		GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
		DELAY_NOP_COUNT(4);
		SPI_MSW_BitMSB(SPIx, wVal, pRVal);
		GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：低位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteBitLSB(SPI_HandlerType *SPIx, UINT8_T wVal)
{
	//---发送1bit的数据
	/*if ((wVal & 0x01) == 0x00)
	{
		GPIO_OUT_0(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}*/
	((wVal & 0x01) != 0x00) ? GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit) : GPIO_OUT_0(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：低位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_ReadBitLSB(SPI_HandlerType *SPIx,UINT8_T *pRVal)
{
	//---读取1bit的数据
	if (pRVal != NULL)
	{
	/*	if (GPIO_GET_STATE(SPIx->msgMISO.msgGPIOPort, SPIx->msgMISO.msgGPIOBit) != 0)
		{
			*pRVal |= 0x80;
		}*/
		*pRVal |= ((GPIO_GET_STATE(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit) != 0) ? 0x80 : 0);
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
UINT8_T SPI_MSW_BitLSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	//---发送1bit的数据
	/*if ((wVal & 0x01) == 0x00)
	{
		GPIO_OUT_0(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}
	else
	{
		GPIO_OUT_1(SPIx->msgMOSI.msgGPIOPort, SPIx->msgMOSI.msgGPIOBit);
	}*/
	((wVal&0x01)!=0x00)? GPIO_OUT_1(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit): GPIO_OUT_0(SPIx->msgMOSI.msgPort, SPIx->msgMOSI.msgBit);
	//---读取1bit的数据
	if (pRVal != NULL)
	{
		/*if (GPIO_GET_STATE(SPIx->msgMISO.msgGPIOPort, SPIx->msgMISO.msgGPIOBit) != 0)
		{
			*pRVal |= 0x80;
		}*/
		*pRVal |= ((GPIO_GET_STATE(SPIx->msgMISO.msgPort, SPIx->msgMISO.msgBit) != 0) ? 0x80 : 0);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 读写1Bit的数据,低位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadBitLSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	//---00---11
	if (((SPIx->msgCPOL == 0) && (SPIx->msgCPOH == 0)) || ((SPIx->msgCPOL == 1) && (SPIx->msgCPOH == 1)))
	{
		GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
		DELAY_NOP_COUNT(4);
		SPI_MSW_BitLSB(SPIx, wVal, pRVal);
		GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
	}
	//---10---01
	//if (((SPIx->msgCPOL == 1) && (SPIx->msgCPOH == 0))|| ((SPIx->msgCPOL == 0) && (SPIx->msgCPOH == 1)))
	else
	{
		GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
		DELAY_NOP_COUNT(4);
		SPI_MSW_BitLSB(SPIx, wVal, pRVal);
		GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
		SPIx->msgDelayus(SPIx->msgPluseWidth);
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
UINT8_T SPI_MSW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock)
{
	SPIx->msgPluseWidth = clock;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MHW_SetClock(SPI_HandlerType *SPIx, UINT32_T clock)
{
	LL_SPI_SetBaudRatePrescaler(SPIx->msgSPIx, clock);
	return OK_0;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadByteMSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	UINT8_T i = 0;

	if (pRVal != NULL)
	{
		*pRVal = 0;
	}
	for (i = 0; i < 8; i++)
	{
		if (pRVal!=NULL)
		{
			*pRVal <<= 1;
		}
		SPI_MSW_WriteAndReadBitMSB(SPIx, wVal, pRVal);
		wVal <<= 1;
	}
	//---时钟线的极性
	(SPIx->msgCPOL == 0) ? GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit) : GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadByteLSB(SPI_HandlerType *SPIx, UINT8_T wVal, UINT8_T *pRVal)
{
	UINT8_T i = 0;
	if (pRVal != NULL)
	{
		*pRVal = 0;
	}
	for (i = 0; i < 8; i++)
	{
		if (pRVal != NULL)
		{
			*pRVal >>= 1;
		}
		SPI_MSW_WriteAndReadBitLSB(SPIx, wVal, pRVal);
		wVal >>= 1;
	}
	//---时钟线的极性
	(SPIx->msgCPOL == 0)? GPIO_OUT_0(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit): GPIO_OUT_1(SPIx->msgSCK.msgPort, SPIx->msgSCK.msgBit);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：高位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadDataMSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	UINT8_T readVal = 0;
	for (i = 0; i < length; i++)
	{
		_return = SPI_MSW_WriteAndReadByteMSB(SPIx, pWVal[i], &readVal);
		//---判断数据是否有效
		if (pRVal != NULL)
		{
			pRVal[i] = readVal;
		}
		//---判断读取正确性
		if (_return != OK_0)
		{
			break;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：低位在前
//////////////////////////////////////////////////////////////////////////////
UINT8_T SPI_MSW_WriteAndReadBDataLSB(SPI_HandlerType *SPIx, UINT8_T *pWVal, UINT8_T *pRVal, UINT16_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	UINT8_T readVal = 0;
	for (i = 0; i < length; i++)
	{
		_return = SPI_MSW_WriteAndReadByteLSB(SPIx, pWVal[i], &readVal);
		//---判断数据是否有效
		if (pRVal != NULL)
		{
			pRVal[i] = readVal;
		}
		//---判断读取正确性
		if (_return != OK_0)
		{
			break;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：8Bit数据中断处理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void  SPI_IRQ8BitTask(SPI_HandlerType* SPIx)
{
	static UINT16_T i = 0;
	//---收发完成标志位
	static UINT8_T txAllowed = 1;
	//---切换工作状态为工作模式
	SPIx->msgState = 1;
	//---等待发送缓冲区为空，TXE 事件---TXE=1，开始发送下一个数据
	if ((txAllowed == 1) && (LL_SPI_IsActiveFlag_TXE(SPIx->msgSPIx))&&(LL_SPI_IsEnabledIT_TXE(SPIx->msgSPIx)))
	{
		//---写入数据寄存器，把要写入的数据写入发送缓冲区
		LL_SPI_TransmitData8(SPIx->msgSPIx, SPIx->msg8BitTxPtr[i]);
		//---切换为接收
		txAllowed = 0;
	}
	//---等待接收缓冲区非空，RXNE 事件---等待RXNE=1，读取收到的数据
	if ((txAllowed == 0) && (LL_SPI_IsActiveFlag_RXNE(SPIx->msgSPIx)) && (LL_SPI_IsEnabledIT_RXNE(SPIx->msgSPIx)))
	{
		//---读取数据寄存器，获取接收缓冲区数据
		SPIx->msg8BitRxPtr[i] = LL_SPI_ReceiveData8(SPIx->msgSPIx);
		//---切换为发送
		txAllowed = 1;
		//---下一个数据
		i++;
	}
	//---判断数据是否收发完成
	if (i==SPIx->msgDataCount)
	{
		i=0;
		txAllowed=0;
		//---切换工作状态为空闲模式
		SPIx->msgState = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：8Bit数据中断处理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void  SPI_IRQ16BitTask(SPI_HandlerType* SPIx)
{
	static UINT16_T i = 0;
	//---收发完成标志位
	static UINT8_T txAllowed = 1;
	//---切换工作状态为工作模式
	SPIx->msgState = 1;
	//---等待发送缓冲区为空，TXE 事件---TXE=1，开始发送下一个数据
	if ((txAllowed == 1) && (LL_SPI_IsActiveFlag_TXE(SPIx->msgSPIx)) && (LL_SPI_IsEnabledIT_TXE(SPIx->msgSPIx)))
	{
		//---写入数据寄存器，把要写入的数据写入发送缓冲区
		LL_SPI_TransmitData16(SPIx->msgSPIx, SPIx->msg16BitTxPtr[i]);
		//---切换为接收
		txAllowed = 0;
	}
	//---等待接收缓冲区非空，RXNE 事件---等待RXNE=1，读取收到的数据
	if ((txAllowed == 0) && (LL_SPI_IsActiveFlag_RXNE(SPIx->msgSPIx)) && (LL_SPI_IsEnabledIT_RXNE(SPIx->msgSPIx)))
	{
		//---读取数据寄存器，获取接收缓冲区数据
		SPIx->msg16BitRxPtr[i] = LL_SPI_ReceiveData16(SPIx->msgSPIx);
		//---切换为发送
		txAllowed = 1;
		//---下一个数据
		i++;
	}
	//---判断数据是否收发完成
	if (i == SPIx->msgDataCount)
	{
		i = 0;
		txAllowed = 0;
		//---切换工作状态为空闲模式
		SPIx->msgState = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：中断处理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void  SPI_IRQTask(SPI_HandlerType* SPIx, UINT8_T is16Bit)
{
	if (is16Bit==0)
	{
		SPI_IRQ8BitTask(SPIx);
	}
	else
	{
		SPI_IRQ16BitTask(SPIx);
	}
}