#include "usart_cfg.h"

//===全局变量定义
USART_HandlerType  g_Usart1 = { 0 };
pUSART_HandlerType pUsart1 = &g_Usart1;

//===printf函数使用的缓存区
#ifdef USE_USART_PRINTF
	char g_PrintfBuffer[USART_PRINTF_SIZE] = { 0 };
#endif

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_StructInit(USART_HandlerType*  USARTx)
{
	//---串口的数据处理流程
	USARTx->msgIndex = 0;
	USARTx->msgRxdID = 0;
	USARTx->msgTxdID = 0;
	USARTx->msgID = 0;
	USARTx->msgIDIndex = 0;
	USARTx->msgCmdIndex = 0;
	USARTx->msgDataOneIndex = 0;
	USARTx->msgDataTwoIndex = 0;
	USARTx->msgIndexOffset=0;
	USARTx->msgTxPort.msgPort = NULL;
	USARTx->msg485Port.msgPort = NULL;
	USARTx->msgUSART = NULL;

	//---接收缓存区
	USARTx->msgRxdHandler.msgDMAMode = 1;
	USARTx->msgRxdHandler.msgCheckSum = 0;
	USARTx->msgRxdHandler.msgCRCFlag = 0;
	USARTx->msgRxdHandler.msgStep = 0;
	USARTx->msgRxdHandler.msgAddNewLine = 0;
	USARTx->msgRxdHandler.msgWIndex = 0;
	USARTx->msgRxdHandler.msgRIndex = 0;
	USARTx->msgRxdHandler.msgFlagIndex =0;
	USARTx->msgRxdHandler.msgCount = 0;
	USARTx->msgRxdHandler.msgMaxSize = 0;
	USARTx->msgRxdHandler.msgState = USART_BUSY;
	USARTx->msgRxdHandler.msgOverFlow = 0;
	USARTx->msgRxdHandler.msgRecordTime = 0;
	USARTx->msgRxdHandler.msgMaxTime = 0;
	USARTx->msgRxdHandler.pMsgVal = NULL;
	//---计数器
	USARTx->msgRxdHandler.msgTimeTick = NULL;

	//---发送缓存区
	USARTx->msgTxdHandler.msgDMAMode = 1;
	USARTx->msgTxdHandler.msgCheckSum = 0;
	USARTx->msgTxdHandler.msgCRCFlag = 0;
	USARTx->msgTxdHandler.msgStep = 0;
	USARTx->msgTxdHandler.msgAddNewLine = 0;
	USARTx->msgTxdHandler.msgWIndex = 0;
	USARTx->msgTxdHandler.msgRIndex = 0;
	USARTx->msgTxdHandler.msgFlagIndex = 0;
	USARTx->msgTxdHandler.msgCount = 0;
	USARTx->msgTxdHandler.msgMaxSize = 0;
	USARTx->msgTxdHandler.msgState = USART_OK;
	USARTx->msgTxdHandler.msgOverFlow = 0;
	USARTx->msgTxdHandler.msgRecordTime = 0;
	USARTx->msgTxdHandler.msgMaxTime = 0;
	USARTx->msgTxdHandler.pMsgVal = NULL;
	//---计数器
	USARTx->msgTxdHandler.msgTimeTick = NULL;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Init(USART_HandlerType*  USARTx, UINT16_T rxSize, UINT8_T* pRxVal, UINT8_T rxCRCFlag, UINT16_T txSize, UINT8_T* pTxVal, UINT8_T txCRCFlag, UINT32_T(*pTimerTick)(void))
{
	USART_StructInit(USARTx);
	USARTx->msgRxdHandler.msgCRCFlag = rxCRCFlag;
	USARTx->msgRxdHandler.msgMaxSize = rxSize;
	USARTx->msgRxdHandler.pMsgVal = pRxVal;
	USARTx->msgTxdHandler.msgCRCFlag = txCRCFlag;
	USARTx->msgTxdHandler.msgMaxSize = txSize;
	USARTx->msgTxdHandler.pMsgVal = pTxVal;
	//---注册计数函数
	USARTx->msgTxdHandler.msgTimeTick = pTimerTick;
	USARTx->msgRxdHandler.msgTimeTick = pTimerTick;
	//---端口初始化
	if ((USARTx != NULL) && (USARTx == USART_TASK_ONE))
	{
		USART1_Init(USARTx);
	}
	else if ((USARTx != NULL) && (USARTx == USART_TASK_TWO))
	{
		USART2_Init(USARTx);
	}
	else if ((USARTx != NULL) && (USARTx == USART_TASK_THREE))
	{
		USART3_Init(USARTx);
	}
	else
	{
		return ERROR_2;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：串口的发送端口的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_TXGPIOInit(USART_HandlerType*  USARTx, UINT8_T isInput)
{
#ifdef USART_INIT_GPIO
	if (USARTx->msgTxPort.msgPort != NULL)
	{
		if (isInput == USART_TXGPIO_SET_OUTPUT)
		{
			LL_GPIO_SetPinMode(USARTx->msgTxPort.msgPort, USARTx->msgTxPort.msgBit, LL_GPIO_MODE_ALTERNATE);
		}
		else
		{
			LL_GPIO_SetPinMode(USARTx->msgTxPort.msgPort, USARTx->msgTxPort.msgBit, LL_GPIO_MODE_INPUT);
		}
	}
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：485方向控制端口的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_485GPIOInit(USART_HandlerType*  USARTx, UINT8_T isEnable)
{
	if (USARTx->msg485Port.msgPort != NULL)
	{
		if (isEnable == USART_485_TX_ENABLE)
		{
			GPIO_OUT_0(USARTx->msg485Port.msgPort, USARTx->msg485Port.msgBit);
		}
		else
		{
			GPIO_OUT_1(USARTx->msg485Port.msgPort, USARTx->msg485Port.msgBit);
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置CRC校验模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_SetCRC(USART_HandlerDef* USARTDefx, UINT8_T crcFlag)
{
	USARTDefx->msgCRCFlag = crcFlag;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：获取CRC校验模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_GetCRC(USART_HandlerDef* USARTDefx)
{
	return USARTDefx->msgCRCFlag;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_Init(USART_HandlerDef* USARTDefx)
{
	//---时间节点
	USARTDefx->msgRecordTime = ((USARTDefx->msgTimeTick != NULL)?(USARTDefx->msgTimeTick()):0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：注销超时计数器
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_DeInit(USART_HandlerDef* USARTDefx)
{
	//---清零溢出标志位
	USARTDefx->msgOverFlow = 0;
	//---清零当前时钟
	USARTDefx->msgRecordTime = 0;
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 时间计数是否发生超时
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_TimeTick_OverFlow(USART_HandlerDef* USARTDefx)
{
	UINT32_T temp = 0;
	//---获取当前时间
	temp = ((USARTDefx->msgTimeTick != NULL) ? (USARTDefx->msgTimeTick()+2) : 0);
	//---判断是否发生超时错误
	if (temp > USARTDefx->msgRecordTime)
	{
		//---计时器未发生溢出操作
		if ((temp - USARTDefx->msgRecordTime) > USARTDefx->msgMaxTime)
		{
			USARTDefx->msgOverFlow = 1;
			return ERROR_1;
		}
	}
	else 
	{
		//---计时器发生溢出操作
		if ((0xFFFFFFFF - USARTDefx->msgRecordTime + temp) > USARTDefx->msgMaxTime)
		{
			USARTDefx->msgOverFlow = 1;
			return ERROR_1;
		}
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
UINT8_T  USART_TimeTask_OverFlow(USART_HandlerType*USARTx, UINT8_T isRx)
{
	UINT8_T _return = OK_0;
	//---判断是接收还是发送并校验是不是DMA接收模式
	if ((isRx != 0) && (USARTx->msgRxdHandler.msgDMAMode == 0))
	{
		if ((USARTx->msgRxdHandler.msgRecordTime != 0) && (USARTx->msgRxdHandler.msgMaxTime != 0))
		{
			if (USARTx->msgRxdHandler.msgRecordTime == 0)
			{
				return OK_0;
			}
			//---获取超时值
			_return = USART_TimeTick_OverFlow(&(USARTx->msgRxdHandler));
			//---超时条件判断，发现某些状态下当前记录的时间值在改写为零的时候，时间记录点没有发生同步更新
			//if ((_return != OK_0))
			if ((_return != OK_0) && (USARTx->msgRxdHandler.msgRecordTime != 0))
			{
				//---打印超时的串口信息
				USART_Printf(USARTx, "=>>SP%d:Receive Mode Timeout Error!<<=\r\n", (USARTx->msgIndex - 1));
				//---复位接收数据缓存区
				USART_Read_Init(USARTx);
			}
		}
	}
	else if(USARTx->msgTxdHandler.msgDMAMode == 0)
	{
		if ((USARTx->msgTxdHandler.msgRecordTime != 0) && (USARTx->msgTxdHandler.msgMaxTime != 0))
		{
			if (USARTx->msgTxdHandler.msgRecordTime == 0)
			{
				return OK_0;
			}
			//---获取超时值
			_return = USART_TimeTick_OverFlow(&(USARTx->msgTxdHandler));
			//---超时条件判断，发现某些状态下当前记录的时间值在改写为零的时候，时间记录点没有发生同步更新
			if ((_return != OK_0) && (USARTx->msgTxdHandler.msgRecordTime != 0))
			{
				//---打印超时的串口信息
				USART_Printf(USARTx, "=>>SP%d:Send Mode Timeout Error!<<=\r\n", (USARTx->msgIndex - 1));
				//---复位发送数据缓存区
				USART_Write_Init(USARTx);
			}
			else
			{
				USARTx->msgTxdHandler.msgOverFlow = 0;
			}
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 接受到数据最大长度使用1字节表示
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITRead_8BitsTask(USART_HandlerType*USARTx, UINT8_T val)
{
	//---保存数到的数据
	static UINT8_T tempVal;
	tempVal = val;
	//---数据接收步序
	switch (USARTx->msgRxdHandler.msgStep)
	{
		//---接收数据的报头
		case 0:
			if (USARTx->msgRxdHandler.msgRIndex == 0)
			{
				if (USARTx->msgRxdID == tempVal)
				{
					USARTx->msgRxdHandler.pMsgVal[0] = tempVal;
					USARTx->msgRxdHandler.msgStep = 1;
					//---记录数据的个数
					USARTx->msgRxdHandler.msgCount = 1;
					//---收到第一个符合格式的数据，启动超时计数器
					USART_TimeTick_Init(&(USARTx->msgRxdHandler));
					//---清零超时标志
					USART_ClearOverFlow(&(USARTx->msgRxdHandler));
				}
			}
			break;

			//---接收数据的长度
		case 1:
			if ((tempVal > 0) && (tempVal < (UINT8_T)(USARTx->msgRxdHandler.msgMaxSize & 0xFF)))
			{
				USARTx->msgRxdHandler.pMsgVal[1] = tempVal;
				//---获取数据的接收长度
				USARTx->msgRxdHandler.msgWIndex = tempVal;
				//---记录数据的个数
				USARTx->msgRxdHandler.msgCount += 1;
				//---下一步骤
				USARTx->msgRxdHandler.msgStep = 2;
			}
			else
			{
				USARTx->msgRxdHandler.msgStep = 0;
			}
			//---复位超时计数器
			USART_TimeTick_Init(&(USARTx->msgRxdHandler));
			break;
			//---接收数据信息
		case 2:
			USARTx->msgRxdHandler.pMsgVal[USARTx->msgRxdHandler.msgRIndex + 2] = tempVal;
			USARTx->msgRxdHandler.msgRIndex++;
			//---记录数据的个数
			USARTx->msgRxdHandler.msgCount += 1;
			//---判断是否溢出
			if (USARTx->msgRxdHandler.msgRIndex >= USARTx->msgRxdHandler.msgMaxSize)
			{
				USARTx->msgRxdHandler.msgRIndex = 0;
			}
			//---复位超时计数器
			USART_TimeTick_Init(&(USARTx->msgRxdHandler));
			//---判断是否接收完成
			if (USARTx->msgRxdHandler.msgRIndex >= USARTx->msgRxdHandler.msgWIndex)
			{
				USARTx->msgRxdHandler.msgStep = 3;
				USARTx->msgRxdHandler.msgState = USART_OK;
				//---清零超时计数器
				//USART_TimeTick_DeInit(USARTx, 1);
			}
			break;
		case 3:
			//---清零超时计数器
			//USART_TimeTick_DeInit(USARTx,1);
			break;
		default:
			//USARTx->msgRxHandler.msgNowTime = 0;
			break;
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 接受到数据最大长度使用2字节表示
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITRead_16BitsTask(USART_HandlerType*USARTx, UINT8_T val)
{
	//---保存数到的数据
	static UINT8_T tempVal;
	tempVal = val;
	//---数据接收步序
	switch (USARTx->msgRxdHandler.msgStep)
	{
		//---接收数据的报头
		case 0:
			if (USARTx->msgRxdHandler.msgRIndex == 0)
			{
				if (USARTx->msgRxdID == tempVal)
				{
					USARTx->msgRxdHandler.pMsgVal[0] = tempVal;
					USARTx->msgRxdHandler.msgStep = 1;
					//---记录数据的个数
					USARTx->msgRxdHandler.msgCount = 1;
					//---收到第一个符合格式的数据，启动超时计数器
					USART_TimeTick_Init(&(USARTx->msgRxdHandler));
					//---清零超时标志
					USART_ClearOverFlow(&(USARTx->msgRxdHandler));
				}
			}
			break;

			//---接收数据的长度高位
		case 1:
			USARTx->msgRxdHandler.pMsgVal[1] = tempVal;
			USARTx->msgRxdHandler.msgWIndex = tempVal;
			USARTx->msgRxdHandler.msgWIndex <<= 8;
			USARTx->msgRxdHandler.msgStep = 2;
			//---记录数据的个数
			USARTx->msgRxdHandler.msgCount += 1;
			//---复位超时计数器
			USART_TimeTick_Init(&(USARTx->msgRxdHandler));
			break;
			//---接收数据的长度低位
		case 2:
			USARTx->msgRxdHandler.pMsgVal[2] = tempVal;
			USARTx->msgRxdHandler.msgWIndex += tempVal;
			//---记录数据的个数
			USARTx->msgRxdHandler.msgCount += 1;
			//---判断数据是否合法
			if ((USARTx->msgRxdHandler.msgWIndex > 0) && (USARTx->msgRxdHandler.msgWIndex < USARTx->msgRxdHandler.msgMaxSize))
			{
				//---记录数据的个数
				USARTx->msgRxdHandler.msgCount = USARTx->msgRxdHandler.msgWIndex;
				//---下一步骤
				USARTx->msgRxdHandler.msgStep = 3;
			}
			else
			{
				USARTx->msgRxdHandler.msgStep = 0;
			}
			//---复位超时计数器
			USART_TimeTick_Init(&(USARTx->msgRxdHandler));
			break;
			//---接收数据信息
		case 3:
			USARTx->msgRxdHandler.pMsgVal[USARTx->msgRxdHandler.msgRIndex + 3] = tempVal;
			USARTx->msgRxdHandler.msgRIndex++;
			//---记录数据的个数
			USARTx->msgRxdHandler.msgCount += 1;
			//---判断是否溢出
			if (USARTx->msgRxdHandler.msgRIndex >= USARTx->msgRxdHandler.msgMaxSize)
			{
				USARTx->msgRxdHandler.msgRIndex = 0;
			}
			//---复位超时计数器
			USART_TimeTick_Init(&(USARTx->msgRxdHandler));
			//---判断是否接收完成
			if (USARTx->msgRxdHandler.msgRIndex >= USARTx->msgRxdHandler.msgWIndex)
			{
				USARTx->msgRxdHandler.msgStep = 4;
				USARTx->msgRxdHandler.msgState = USART_OK;
				//---清零超时计数器
				//USART_TimeTick_DeInit(USARTx, 1);
			}
			break;
		case 4:
			//---清零超时计数器
			//USART_TimeTick_DeInit(USARTx,1);
			break;
		default:
			//USARTx->msgRxHandler.msgNowTime = 0;
			break;
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
UINT8_T  USART_ITRead_Task(USART_HandlerType*USARTx, UINT8_T val)
{
	if (USARTx->msgRxdHandler.msgMaxSize < 0xFF)
	{
		return USART_ITRead_8BitsTask(USARTx, val);
	}
	else
	{
		return USART_ITRead_16BitsTask(USARTx, val);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_WriteByte(USART_HandlerType*USARTx, UINT8_T  val)
{
	//---发送数据
	LL_USART_TransmitData8(USARTx->msgUSART, val);
	//---等待发送完成
	while (LL_USART_IsActiveFlag_TC(USARTx->msgUSART) == RESET)
	{
		WDT_RESET();
	}
	//---清除发送完成标志位
	LL_USART_ClearFlag_TC(USARTx->msgUSART);
	return OK_0;
}
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 发送字符串
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_WriteData(USART_HandlerType*USARTx, char *pVal)
{
	//---设置485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);
	//---切换发送端口为输出模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);
	//---关闭中断
	CLI();
	while (*pVal != '\0')
	{
		USART_PollMode_WriteByte(USARTx, (UINT8_T)*pVal);
		pVal++;
	}
	//---使能中断
	SEI();
	//---设置485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);
	//---切换发送端口为输入模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_ReadByte(USART_HandlerType*USARTx)
{
	//---等待接收完成
	while (LL_USART_IsActiveFlag_RXNE(USARTx->msgUSART) != RESET)
	{
		WDT_RESET();
	}
	return LL_USART_ReceiveData8(USARTx->msgUSART);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_PollMode_ReadData(USART_HandlerType*USARTx, char *pVal)
{
	while (*pVal != '\0')
	{
		*pVal = USART_PollMode_ReadByte(USARTx);
		pVal++;
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
UINT8_T  USART_ITWrite_TXETask(USART_HandlerType*USARTx)
{
	VLTUINT8_T tempFlag=OK_0;
	if (USARTx->msgTxdHandler.msgWIndex != 0)
	{
		if (USARTx->msgTxdHandler.msgRIndex != USARTx->msgTxdHandler.msgWIndex)
		{
			//---发送8Bit的数据
			LL_USART_TransmitData8(USARTx->msgUSART, USARTx->msgTxdHandler.pMsgVal[USARTx->msgTxdHandler.msgRIndex]);
			//---数据缓存区序号增加
			USARTx->msgTxdHandler.msgRIndex++;
			//---校验缓存区是否溢出
			if (USARTx->msgTxdHandler.msgRIndex >= USARTx->msgTxdHandler.msgMaxSize)
			{
				USARTx->msgTxdHandler.msgRIndex = 0;
			}
			//---校验数据是否都填入缓存区
			if ((USARTx->msgTxdHandler.msgRIndex >= USARTx->msgTxdHandler.msgWIndex) && (USARTx->msgTxdHandler.msgCount > 0) && (USARTx->msgTxdHandler.msgRIndex >= USARTx->msgTxdHandler.msgCount))
			{
				//---发送完成,发送数据寄存器空中断不使能
				LL_USART_DisableIT_TXE(USARTx->msgUSART);
				//---使能发送完成中断，用于切换TXD端口为输入状态
				LL_USART_EnableIT_TC(USARTx->msgUSART);
			}
		}
		else
		{
			tempFlag = ERROR_1;
		}
	}
	else
	{
		tempFlag = ERROR_2;
	}
	//---校验结果
	if (tempFlag)
	{
		//---发送完成,发送数据寄存器空中断不使能---检查是否存在异常；关闭发送数据寄存器空中断
		LL_USART_DisableIT_TXE(USARTx->msgUSART);
		//---清空数据发送缓存区
		USART_Write_Init(USARTx);
	}
	//---复位超时计数器
	USART_TimeTick_Init(&(USARTx->msgTxdHandler));
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITWrite_TCTask(USART_HandlerType*USARTx)
{
	//---定义了485，由于485响应的延迟问题,增加发送换行符，用于缓冲485的响应
	if ((USARTx->msg485Port.msgPort != NULL) && (USARTx->msgTxdHandler.msgStep == 0))
	{
		//---发送8Bit的数据
		LL_USART_TransmitData8(USARTx->msgUSART, 0x0D);
		USARTx->msgTxdHandler.msgStep = 1;
	}
	else if ((USARTx->msg485Port.msgPort != NULL) && (USARTx->msgTxdHandler.msgStep == 1))
	{
		//---发送8Bit的数据
		LL_USART_TransmitData8(USARTx->msgUSART, 0x0A);
		USARTx->msgTxdHandler.msgStep = 2;
	}
	else
	{
		//---清零发送步序
		USARTx->msgTxdHandler.msgStep = 0;
		//---发送完成,发送数据发送完成中断不使能
		LL_USART_DisableIT_TC(USARTx->msgUSART);
		//---清空数据发送缓存区
		USART_Write_Init(USARTx);
		//---清零超时计数器
		//USART_TimeTick_DeInit(USARTx, 0);
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
UINT8_T USART_RealTime_AddByte(USART_HandlerType*USARTx, UINT8_T val)
{
	if (USARTx->msgTxdHandler.pMsgVal != NULL)
	{
		if (USARTx->msgTxdHandler.msgWIndex >= USARTx->msgTxdHandler.msgMaxSize)
		{
			USARTx->msgTxdHandler.msgWIndex = 0;
		}
		USARTx->msgTxdHandler.pMsgVal[USARTx->msgTxdHandler.msgWIndex] = val;
		USARTx->msgTxdHandler.msgWIndex++;
		//---判断发送寄存器空中断是否使能
		if (LL_USART_IsEnabledIT_TXE(USARTx->msgUSART) == 0)
		{
			//---设置485为发送模式
			USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);
			//---切换发送端口为输出模式
			USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);
			//---使能发送空中断
			LL_USART_EnableIT_TXE(USARTx->msgUSART);
		}
		return OK_0;
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送数据的大小
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_RealTime_AddSize(USART_HandlerType*USARTx, UINT16_T val)
{
	UINT8_T _return = ERROR_1;
	if (USARTx->msgTxdHandler.pMsgVal != NULL)
	{
		if ((USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CHECKSUM) || (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC8))
		{
			val += 1;
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			val += 2;
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			val += 4;
		}
		//---判断是否有设备的ID信息
		if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
		{
			val += 1;
		}
		//---判断是否增加换行符
		if ((USARTx->msgTxdHandler.msgAddNewLine==1))
		{
			val += 2;
		}
		//---判断数据的格式
		if (USARTx->msgTxdHandler.msgMaxSize > 250)
		{
			USART_RealTime_AddByte(USARTx, (UINT8_T)(val >> 8));
			_return = USART_RealTime_AddByte(USARTx, (UINT8_T)(val & 0xFF));
			val += 3;
		}
		else
		{
			_return = USART_RealTime_AddByte(USARTx, (UINT8_T)(val & 0xFF));
			val += 2;
		}
		//---判断是否有设备的ID信息并填充ID信息
		if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
		{
			_return = USART_RealTime_AddByte(USARTx, USARTx->msgID);
		}		
		//---计算发送数据的个数
		USARTx->msgTxdHandler.msgCount = val;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：填充数据的CRC校验信息
//////输入参数:
//////输出参数:
//////说		明：使用CRC，数据的必须从0开始使用
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_RealTime_AddCRC(USART_HandlerType*USARTx)
{
	//---校验CRC是否初始化
	if ((USARTx->msgTxdHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		UINT32_T crcVal = 0;
		//---判断数据校验模式
		if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			crcVal = CRCTask_CheckSum(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			crcVal = CRCTask_CRC16(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			crcVal = CRCTask_CRC32(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 24));
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 16));
			USART_RealTime_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_RealTime_AddByte(USARTx, (UINT8_T)crcVal);
		}
		return OK_0;
	}
	//---判断是否增加换行符
	if ((USARTx->msgTxdHandler.msgAddNewLine==1))
	{
		USART_RealTime_AddByte(USARTx, 0x0D);
		USART_RealTime_AddByte(USARTx, 0x0A);
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 处理数据填充前的操作，约定好报头，和有效数据开始的位置
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_Init(USART_HandlerType*USARTx,UINT8_T isChildCmd)
{
	//---等待传输完成
	while ((USARTx->msgTxdHandler.msgState == USART_BUSY)||(USARTx->msgTxdHandler.msgState==USART_PRINTF))
	{
		WDT_RESET();
	}
	USARTx->msgTxdHandler.msgWIndex = 0;
	USARTx->msgTxdHandler.msgFlagIndex=0;
	//---填充数据报头
	USART_FillMode_AddByte( USARTx, USARTx->msgTxdID );
	//---填充数据长度
	USART_FillMode_AddByte( USARTx, 0x00 );
	if (USARTx->msgTxdHandler.msgMaxSize > 0xFF)
	{
		USART_FillMode_AddByte( USARTx, 0x00 );
		USARTx->msgTxdHandler.msgFlagIndex+=1;
	}
	//---填充多设备通信的设备ID
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		USART_FillMode_AddByte(USARTx, USARTx->msgID);
	}
	//---主命令/命令
	USART_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]);
	//---校验是否有子命令
	if (isChildCmd!=0)
	{
		//---子命令
		USART_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex + (isChildCmd == 0 ? 0 : 1)]);
	}
	//---标识返回执行结果在缓存区中断的序号
	USARTx->msgTxdHandler.msgFlagIndex = USARTx->msgTxdHandler.msgWIndex;
	//---返回执行结果
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：数据填充后在发送
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_AddByte(USART_HandlerType*USARTx, UINT8_T val)
{
	USARTx->msgTxdHandler.pMsgVal[USARTx->msgTxdHandler.msgWIndex] = val;
	USARTx->msgTxdHandler.msgWIndex++;
	if (USARTx->msgTxdHandler.msgWIndex >= USARTx->msgTxdHandler.msgMaxSize)
	{
		USARTx->msgTxdHandler.msgWIndex = 0;
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
UINT8_T USART_FillMode_AddData(USART_HandlerType*USARTx, UINT8_T *pVal, UINT16_T length)
{
	UINT16_T i = 0;
	UINT8_T _return = OK_0;
	for (i = 0; i < length; i++)
	{
		_return = USART_FillMode_AddByte(USARTx, pVal[i]);
		//---判断数据是否填充正确
		if (_return != OK_0)
		{
			break;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置执行结果标识
//////输入参数:
//////输出参数:
//////说		明：数据填充后在发送
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_SetResultFlag(USART_HandlerType* USARTx, UINT8_T val)
{
	USARTx->msgTxdHandler.pMsgVal[USARTx->msgTxdHandler.msgFlagIndex] = val;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：写的序号的增加值
//////输入参数:
//////输出参数:
//////说		明：数据填充后在发送
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_FillMode_AddIndexW(USART_HandlerType* USARTx, UINT16_T val)
{
	USARTx->msgTxdHandler.msgWIndex += val;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_CRCTask_Read(USART_HandlerType*USARTx)
{
	UINT32_T crcVal = 0;
	UINT32_T crcTemp = 0;
	//---接收数据的个数
	UINT16_T length = USARTx->msgRxdHandler.msgWIndex;
	//---校验CRC是否初始化
	if ((USARTx->msgRxdHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		//---判断数据校验模式
		if (USARTx->msgRxdHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			length -= 1;
			if (USARTx->msgRxdHandler.msgMaxSize < 250)
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length);
				//---获取校验和
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxdHandler.pMsgVal[2] = (UINT8_T)(length);
				//---获取校验和
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 3];
				//---参与校验和计算的数据总长度
				length += 3;
			}
			//---计算校验和
			crcVal = CRCTask_CheckSum(USARTx->msgRxdHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxdHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			length -= 1;
			if (USARTx->msgRxdHandler.msgMaxSize < 250)
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length);
				//---获取校验和
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxdHandler.pMsgVal[2] = (UINT8_T)(length);
				//---获取校验和
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 3];
				//---参与校验和计算的数据总长度
				length += 3;
			}
			//---计算CRC8校验值
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgRxdHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxdHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			length -= 2;
			if (USARTx->msgRxdHandler.msgMaxSize < 250)
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length);
				//---获取CRC16的高位值
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 1];
				//---获取CRC16的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxdHandler.pMsgVal[2] = (UINT8_T)(length);
				//---获取CRC16的高位值
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---获取CRC16的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 3];
				//---参与校验和计算的数据总长度
				length += 3;
			}
			//---计算CRC16校验值
			crcVal = CRCTask_CRC16(USARTx->msgRxdHandler.pMsgVal, length);
		}
		else if (USARTx->msgRxdHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			length -= 4;
			if (USARTx->msgRxdHandler.msgMaxSize < 250)
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length);
				//---获取CRC32的最高位值
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length - 1];
				//---获取CRC32的次高位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length];
				//---获取CRC32的次低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 1];
				//---获取CRC32的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---参与校验和计算的数据总长度
				length += 2;
			}
			else
			{
				USARTx->msgRxdHandler.pMsgVal[1] = (UINT8_T)(length >> 8);
				USARTx->msgRxdHandler.pMsgVal[2] = (UINT8_T)(length);
				//---获取CRC32的最高位值
				crcTemp = USARTx->msgRxdHandler.pMsgVal[length];
				//---获取CRC32的次高位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 1];
				//---获取CRC32的次低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 2];
				//---获取CRC32的低位值
				crcTemp = (crcTemp << 8) + USARTx->msgRxdHandler.pMsgVal[length + 3];
				length += 3;
			}
			crcVal = CRCTask_CRC32(USARTx->msgRxdHandler.pMsgVal, length);
		}
		//---比对CRC的值
		if (crcVal != crcTemp)
		{
			return  ERROR_1;
		}
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
UINT8_T USART_CRCTask_Write(USART_HandlerType*USARTx)
{
	//---校验CRC是否初始化
	if ((USARTx->msgTxdHandler.msgCRCFlag != USART_CRC_NONE) && (CRCTask_Enable() == OK_0))
	{
		UINT32_T crcVal = 0;
		//--要发送数据的总长度
		UINT16_T length = USARTx->msgTxdHandler.msgWIndex;
		//---判断是否增加换行符
		if ((USARTx->msgTxdHandler.msgAddNewLine==1))
		{
			length += 2;
		}
		//---数据大小的整理
		if (USARTx->msgTxdHandler.msgMaxSize < 0xFF)
		{
			USARTx->msgTxdHandler.pMsgVal[1] = (UINT8_T)(length- 2);
		}
		else
		{
			USARTx->msgTxdHandler.pMsgVal[1] = (UINT8_T)((length - 3) >> 8);
			USARTx->msgTxdHandler.pMsgVal[2] = (UINT8_T)((length - 3));
		}		
		//---判断数据校验模式
		if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CHECKSUM)
		{
			//---校验和
			crcVal = CRCTask_CheckSum(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC8)
		{
			//---CRC8校验
			crcVal = CRCTask_CRC8(USE_CRC8_07H, USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC16)
		{
			//---CRC16校验
			crcVal = CRCTask_CRC16(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		else if (USARTx->msgTxdHandler.msgCRCFlag == USART_CRC_CRC32)
		{
			//---CRC32校验
			crcVal = CRCTask_CRC32(USARTx->msgTxdHandler.pMsgVal, USARTx->msgTxdHandler.msgWIndex);
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 24));
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 16));
			USART_FillMode_AddByte(USARTx, (UINT8_T)(crcVal >> 8));
			USART_FillMode_AddByte(USARTx, (UINT8_T)crcVal);
		}
		//---发送数据的大小
		USARTx->msgTxdHandler.msgCount = USARTx->msgTxdHandler.msgWIndex;
		return OK_0;
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：填充模式启动发送，这里适合填充之后中断发送和DMA模式（DMA模式是数据填充完成之后，才能启动发送的）
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_FillMode_WriteSTART(USART_HandlerType*USARTx, UINT8_T isNeedID)
{
	//---等待传输完成
	while ((USARTx->msgTxdHandler.msgState == USART_BUSY)||(USARTx->msgTxdHandler.msgState==USART_PRINTF))
	{
		WDT_RESET();
	}
	//---设置发送状态为空闲中
	USARTx->msgTxdHandler.msgState = USART_BUSY;
	//---判断是否需要填充报头
	if (isNeedID == 1)
	{
		//---填充报头和数据长度
		USARTx->msgTxdHandler.pMsgVal[0] = USARTx->msgTxdID;
	}
	//---填充设备ID
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		USARTx->msgTxdHandler.pMsgVal[USARTx->msgIDIndex] = USARTx->msgID;
	}	
	//---发送CRC处理
	USART_CRCTask_Write(USARTx);
	//---判断是否增加换行符
	if (( USARTx->msgTxdHandler.msgAddNewLine == 1 ))
	{
		USART_FillMode_AddByte( USARTx, 0x0D );
		USART_FillMode_AddByte( USARTx, 0x0A );
	}	
	//---数据大小的整理
	if (USARTx->msgTxdHandler.msgMaxSize < 0xFF)
	{
		USARTx->msgTxdHandler.pMsgVal[1] = (UINT8_T)(USARTx->msgTxdHandler.msgWIndex - 2);
	}
	else
	{
		USARTx->msgTxdHandler.pMsgVal[1] = (UINT8_T)((USARTx->msgTxdHandler.msgWIndex - 3) >> 8);
		USARTx->msgTxdHandler.pMsgVal[2] = (UINT8_T)((USARTx->msgTxdHandler.msgWIndex - 3));
	}
	USARTx->msgTxdHandler.msgCount=USARTx->msgTxdHandler.msgWIndex;
	//---设置485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);
	//---切换发送端口为输出模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);
	//---校验是不是DMA模式
	if (USARTx->msgTxdHandler.msgDMAMode==0)
	{
		//---发送数据寄存器空中断使能
		LL_USART_EnableIT_TXE(USARTx->msgUSART);
	}
	else
	{
		//---设置数据地址
		USART_Write_DMA_SetMemoryAddress(USARTx, (USARTx->msgTxdHandler.pMsgVal));
		//---启动DMA发送
		USART_Write_DMA_RESTART(USARTx);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 获取溢出标志
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetOverFlow(USART_HandlerDef* USARTDefx)
{
	return USARTDefx->msgOverFlow ;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 清除溢出标志
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ClearOverFlow(USART_HandlerDef* USARTDefx)
{
	USARTDefx->msgOverFlow = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：获取状态标识
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_GetState(USART_HandlerDef* USARTDefx)
{
	return USARTDefx->msgState;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：清楚状态标识
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ClearState(USART_HandlerDef* USARTDefx)
{
	USARTDefx->msgState = USART_OK;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：复位接收参数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_Read_Init(USART_HandlerType*  USARTx)
{
	//---设置485位接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);
	//---清零步序
	USARTx->msgRxdHandler.msgStep = 0;
	//---清除写数组索引
	USARTx->msgRxdHandler.msgWIndex = 0;
	//---清除读数组索引
	USARTx->msgRxdHandler.msgRIndex = 0;
	//---清零接收数据个数
	USARTx->msgRxdHandler.msgCount = 0;
	//---清零接收完成标志
	USARTx->msgRxdHandler.msgState = USART_BUSY;
	//---清零超时计数
	USARTx->msgRxdHandler.msgRecordTime = 0;
	//---清零超时标志
	USARTx->msgRxdHandler.msgOverFlow = 0;
	//---校验是不是DMA模式
	if(USARTx->msgRxdHandler.msgDMAMode!=0)
	{
		USART_Read_DMA_RESTART(USARTx);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：复位发送参数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Write_Init(USART_HandlerType*  USARTx)
{
	//---设置发送状态为发送完成
	USARTx->msgTxdHandler.msgState = USART_OK;
	//---清除写数组索引
	USARTx->msgTxdHandler.msgWIndex = 0;
	//---清除读数组索引
	USARTx->msgTxdHandler.msgRIndex = 0;
	//---清零发送数据个数
	USARTx->msgTxdHandler.msgCount = 0;
	//---清零超时计数
	USARTx->msgTxdHandler.msgRecordTime = 0;
	//---清零超时标志
	USARTx->msgTxdHandler.msgOverFlow = 0;
	//---操作步序归零
	USARTx->msgTxdHandler.msgStep = 0;
	//---设置485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);
	//---数据发送完成，切换端口为输入模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：校验设备的ID信息
//////输入参数:
//////输出参数:
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DeviceID(USART_HandlerType*USARTx)
{
	if ((USARTx->msgCmdIndex - USARTx->msgIDIndex) != 0)
	{
		if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgIDIndex] != USARTx->msgID)
		{
			return ERROR_1;
		}
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：挂起发送
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_PrintfSuspend(USART_HandlerType* USARTx)
{
#ifdef USE_USART_PRINTF
	//---检查发送寄存器空中断是否使能
	if (LL_USART_IsEnabledIT_TXE(USARTx->msgUSART))
	{
		//---等待发送完成
		while (LL_USART_IsActiveFlag_TXE(USARTx->msgUSART) != RESET)
		{
			WDT_RESET();
		}
	}
	//---检查发送完成中断
	if (LL_USART_IsEnabledIT_TC(USARTx->msgUSART))
	{
		//---等待发送完成
		while ((LL_USART_IsActiveFlag_TC(USARTx->msgUSART) != RESET) && (USARTx->msgTxdHandler.msgStep == 0))
		{
			WDT_RESET();
		}
		LL_USART_ClearFlag_TC(USARTx->msgUSART);
	}
	//---检查发送状态，等待之前的数据发送完成;必须是空闲状态，总线上没有其他数据放
	while ((USARTx->msgTxdHandler.msgState == USART_BUSY) || (USARTx->msgTxdHandler.msgState == USART_PRINTF))
	{
		WDT_RESET();
	}
	//---定义485为发送模式
	USART_485GPIOInit(USARTx, USART_485_TX_ENABLE);
	//---切换发送端口为输出模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_OUTPUT);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：恢复发送
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_PrintfResume(USART_HandlerType* USARTx)
{
#ifdef USE_USART_PRINTF
	//---定义485为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);
	//---数据发送完成，切换端口为输入模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
#endif
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：自定义的Printf函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_Printf(USART_HandlerType*USARTx, char*fmt, ...)
{
#ifdef USE_USART_PRINTF
	//---校验串口是否已经初始化过
	if (USARTx->msgUSART!=NULL)
	{
		//---挂起操作
		USART_PrintfSuspend(USARTx);
		//---计算数据
		UINT16_T length = 0;
		va_list arg_ptr;
		va_start(arg_ptr, fmt);
		//---用于向字符串中打印数据、数据格式用户自定义;返回参数是最终生成字符串的长度
		length = (UINT16_T)vsnprintf(g_PrintfBuffer, USART_PRINTF_SIZE, fmt, arg_ptr);
		va_end(arg_ptr);
		//---判断数据
		if (length > USART_PRINTF_SIZE)
		{
			length = USART_PRINTF_SIZE;
		}
		//---校验是不是DMA模式
		if (USARTx->msgTxdHandler.msgDMAMode!=0)
		{
			//--->>>DMA发送模式
			USARTx->msgTxdHandler.msgCount = length;
			//---设置数据地址
			USART_Write_DMA_SetMemoryAddress(USARTx, (UINT8_T *)g_PrintfBuffer);
			//---启动DMA发送
			USART_Write_DMA_RESTART(USARTx);
		}
		else
		{
			//--->>>中断发送模式
			//---要发送数据的个数
			USARTx->msgPCount = length;
			//---使用的发送完成中断，这里需要首先发送一次数据
			USARTx->msgPIndex = 1;
			//---工作在使用PRINTF模式
			USARTx->msgTxdHandler.msgState = USART_PRINTF;
			//---发送完成,发送数据发送完成中断不使能
			LL_USART_EnableIT_TC(USARTx->msgUSART);
			//---发送8Bit的数据
			LL_USART_TransmitData8(USARTx->msgUSART, g_PrintfBuffer[0]);
		}
	}
#endif	
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T  USART_ITPrintf_TCTask(USART_HandlerType* USARTx)
{
#ifdef USE_USART_PRINTF
	if (USARTx->msgPIndex>=USARTx->msgPCount)
	{
		//---发送完成,发送数据发送完成中断不使能
		LL_USART_DisableIT_TC(USARTx->msgUSART);
		//---恢复操作
		USART_PrintfResume(USARTx);
		USARTx->msgTxdHandler.msgState = USART_OK;
	}
	else
	{
		//---发送8Bit的数据
		LL_USART_TransmitData8(USARTx->msgUSART, g_PrintfBuffer[USARTx->msgPIndex]);
		USARTx->msgPIndex++;
	}
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_IT_TCTask(USART_HandlerType* USARTx)
{
	if (USARTx->msgTxdHandler.msgState == USART_BUSY)
	{
		USART_ITWrite_TCTask(USARTx);
	}
	else if (USARTx->msgTxdHandler.msgState == USART_PRINTF)
	{
		USART_ITPrintf_TCTask(USARTx);
	}
	else
	{
		USARTx->msgTxdHandler.msgState = USART_OK;
		LL_USART_DisableIT_TC(USARTx->msgUSART);
	}
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：打印系统时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_PrintfClockFreq(USART_HandlerType*USARTx)
{
	LL_RCC_ClocksTypeDef rccClock = { 0 };
	//---获取系统时钟
	LL_RCC_GetSystemClocksFreq(&rccClock);
	//---打印系统时钟
	USART_Printf(USARTx, "SYSCLK=%UHz\r\n", rccClock.SYSCLK_Frequency);
	//---打印系统时钟
	USART_Printf(USARTx, "HCLK=%UHz\r\n", rccClock.HCLK_Frequency);
	//---打印系统时钟
	USART_Printf(USARTx, "PCLK1=%UHz\r\n", rccClock.PCLK1_Frequency);
	//---打印系统时钟
	USART_Printf(USARTx, "PCLK2=%UHz\r\n", rccClock.PCLK2_Frequency);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：USART的总线时钟
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Clock(USART_TypeDef* USARTx, UINT8_T isEnable)
{
	if (USARTx == USART1)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART1);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART1);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;

	}
	else if (USARTx == USART2)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#if defined(USART3)
	else if (USARTx == USART3)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART3);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART3);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* USART3 */
#if defined(USART6)
	else if (USARTx == USART6)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_USART6);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_USART6);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* USART6 */
#if defined(UART4)
	else if (USARTx == UART4)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART4);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART4);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART4 */
#if defined(UART5)
	else if (USARTx == UART5)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART5);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART5);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART5 */
#if defined(UART7)
	else if (USARTx == UART7)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART7);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART7);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART7 */
#if defined(UART8)
	else if (USARTx == UART8)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART8);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART8);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART8 */
#if defined(UART9)
	else if (USARTx == UART9)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_UART9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_UART9);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_UART9);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART9 */
#if defined(UART10)
	else if (USARTx == UART10)
	{
		if (isEnable == PERIPHERAL_CLOCK_DISABLE)
		{
			//---不使能SPI的时钟线
			LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_UART10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_ENABLE)
		{
			//---使能SPI的时钟线
			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_FORCE_RESET)
		{
			LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_UART10);
		}
		else if (isEnable == PERIPHERAL_CLOCK_RELEASE_RESET)
		{
			LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_UART10);
		}
		else
		{
			return ERROR_1;
		}
		return OK_0;
	}
#endif /* UART10 */
	else
	{
		return ERROR_2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：销毁配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DeInit(USART_HandlerType*USARTx)
{
	LL_USART_DeInit(USARTx->msgUSART);
	USART_Clock(USARTx->msgUSART, 0);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：参数配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_ParamInit(USART_HandlerType *USARTx, UINT8_T id, UINT8_T idIndex, UINT8_T cmdIndex, UINT8_T d1Index, UINT8_T d2Index)
{
	USARTx->msgID = id;
	USARTx->msgIDIndex = idIndex;
	USARTx->msgCmdIndex = cmdIndex;
	USARTx->msgDataOneIndex = d1Index;
	USARTx->msgDataTwoIndex = d2Index;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：usart1的参数配置
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART1_ConfigInit(USART_HandlerType* USARTx)
{
	//---USART1 GPIO Configuration
	//---PA9  ------> USART1_TX---端口复用为7
	//---PA10 ------> USART1_RX---端口复用为7
	//---使能端口时钟
	GPIOTask_Clock(GPIOA, PERIPHERAL_CLOCK_ENABLE);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	//---模式配置
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	//---工作速度
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	//---复用功能的推完输出
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	//---输入上拉使能
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
#ifdef USE_MCU_STM32F1
	//---TX
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//---RX
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#else
	//---端口号
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
	//---复用功能为USART1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
	//---配置消息结构体中的信息
	USARTx->msgTxPort.msgPort = GPIOA;
	USARTx->msgTxPort.msgBit = LL_GPIO_PIN_9;
	USARTx->msgUSART = USART1;
	//---串口序号
	USARTx->msgIndex = 1 + 1;
	//---注销串口的初始化
	LL_USART_DeInit(USARTx->msgUSART);
	//---使能USART1的时钟信号
	USART_Clock(USARTx->msgUSART, 1);
	//---USART的接口结构体
	LL_USART_InitTypeDef USART_InitStruct = { 0 };
	//---波特率
	USART_InitStruct.BaudRate = 115200;
	//---数据位
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	//---停止位
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	//---校验位
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	//---配置为收发模式
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	//---硬件流控制---默认为无
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	//---过采样配置
#ifdef USART_CR1_OVER8
	//---过采样次数---默认配置为16
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
#endif
	//---初始化串口
	LL_USART_Init(USARTx->msgUSART, &USART_InitStruct);
	//---串口异步模式配置
	LL_USART_ConfigAsyncMode(USARTx->msgUSART);
	//---校验是否接收需要超时函数
	if (USARTx->msgRxdHandler.msgTimeTick != NULL)
	{
		USARTx->msgRxdHandler.msgMaxTime = 100;
	}
	else
	{
		USARTx->msgRxdHandler.msgMaxTime = 0;
	}
	//---校验是否发送需要超时函数
	if (USARTx->msgTxdHandler.msgTimeTick != NULL)
	{
		USARTx->msgTxdHandler.msgMaxTime = 100;
	}
	else
	{
		USARTx->msgTxdHandler.msgMaxTime = 0;
	}
	//---配置CRC的等级
	USARTx->msgTxdHandler.msgCRCFlag = USART_CRC_NONE;
	//---配置报头和报尾
	USARTx->msgRxdID = 0x55;
	USARTx->msgTxdID = 0x5A;
	//---命令和地址配置
	USART_ParamInit(USARTx, USART1_DEVICE_ID, USART1_ID_INDEX, USART1_CMD_INDEX, USART1_DATA1_INDEX, USART1_DATA2_INDEX);
	//---定义485为接收模式--推完输出模式，配置为接收模式
	USART_485GPIOInit(USARTx, USART_485_RX_ENABLE);
	//---设置TX端口为输入模式
	USART_TXGPIOInit(USARTx, USART_TXGPIO_SET_INPUT);
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART1_Init(USART_HandlerType*USARTx)
{
	USART1_ConfigInit(USARTx);
	//---校验接收是不是DMA传输
	if (USARTx->msgRxdHandler.msgDMAMode==0)
	{
		//---使能接收中断
		LL_USART_EnableIT_RXNE(USART1);
	}
	else
	{
		//---使能接收空闲中断
		LL_USART_EnableIT_IDLE(USART1);
		//---初始化DMA设置
		USART1_Read_DMA_Init(USARTx);
	}
	//---校验发送是不是DMA传输方式
	if (USARTx->msgTxdHandler.msgDMAMode != 0)
	{
		USART1_Write_DMA_Init(USARTx);
	}
	//---USART1_IRQ中断配置---中断等级配置
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
	//---使能中断
	NVIC_EnableIRQ(USART1_IRQn);
	//---使能串口
	LL_USART_Enable(USART1);	
	//---打印初始化信息
	//USART_Printf(USARTx, "=>>串口1的初始化<<=\r\n");
	USART_Printf(USARTx, "=>>Init SP1<<=\r\n");
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART2_Init(USART_HandlerType*USARTx)
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
UINT8_T USART3_Init(USART_HandlerType*USARTx)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：中断处理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////、
void USART_IRQTask(USART_HandlerType* USARTx)
{
	//---数据接收中断处理函数---接收寄存器不为空
	if (LL_USART_IsActiveFlag_RXNE(USARTx->msgUSART) && LL_USART_IsEnabledIT_RXNE(USARTx->msgUSART))
	{
		//---中断处理函数
		USART_ITRead_Task(USARTx, LL_USART_ReceiveData8(USARTx->msgUSART));
		//---清楚中断标志位
		LL_USART_ClearFlag_RXNE(USARTx->msgUSART);
	}

	//---数据发送寄存器空发送中断处理函数
	if (LL_USART_IsActiveFlag_TXE(USARTx->msgUSART) && LL_USART_IsEnabledIT_TXE(USARTx->msgUSART))
	{
		//---中断处理函数
		USART_ITWrite_TXETask(USARTx);
	}
	//---数据发送完成中断处理函数
	if (LL_USART_IsActiveFlag_TC(USARTx->msgUSART) && LL_USART_IsEnabledIT_TC(USARTx->msgUSART))
	{
		//---中断处理函数
		USART_IT_TCTask(USARTx);
		//---清楚中断标志位
		LL_USART_ClearFlag_TC(USARTx->msgUSART);
	}
	//---UART5和UART5，不适用CTS检测
	if ((USARTx->msgUSART!=UART4)&& (USARTx->msgUSART != UART5))
	{
		//---CTS状态线变化中断
		if (LL_USART_IsActiveFlag_nCTS(USARTx->msgUSART) && LL_USART_IsEnabledIT_CTS(USARTx->msgUSART))
		{
			//---中断处理函数

			//---清楚中断标志位
			LL_USART_ClearFlag_nCTS(USARTx->msgUSART);
		}
	}
	//---LIN断路检测中断
	if (LL_USART_IsActiveFlag_LBD(USARTx->msgUSART) && LL_USART_IsEnabledIT_LBD(USARTx->msgUSART))
	{
		//---中断处理函数

		//---清楚中断标志位
		LL_USART_ClearFlag_LBD(USARTx->msgUSART);
	}
	//---空闲状态中断，主要是用于DMA接收不定长数据
	if (LL_USART_IsActiveFlag_IDLE(USARTx->msgUSART) && LL_USART_IsEnabledIT_IDLE(USARTx->msgUSART))
	{
		//---中断处理函数,DMA数据接收
		USART_DMA_IDLETask(USARTx);
		//---清楚空闲中断标志位
		LL_USART_ClearFlag_IDLE(USARTx->msgUSART);
		//---清楚DMA中断标识
		LL_DMA_ClearFlag(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
	}
	//---上溢错误中断
	if (LL_USART_IsActiveFlag_ORE(USARTx->msgUSART) && LL_USART_IsEnabledIT_ERROR(USARTx->msgUSART))
	{
		//---中断处理函数

		//---清楚中断标志位
		LL_USART_ClearFlag_ORE(USARTx->msgUSART);
	}
	//---噪声中断
	if (LL_USART_IsActiveFlag_NE(USARTx->msgUSART) && LL_USART_IsEnabledIT_ERROR(USARTx->msgUSART))
	{
		//---中断处理函数

		//---清楚中断标志位
		LL_USART_ClearFlag_NE(USARTx->msgUSART);
	}
	//---帧错误中断
	if (LL_USART_IsActiveFlag_FE(USARTx->msgUSART) && LL_USART_IsEnabledIT_ERROR(USARTx->msgUSART))
	{
		//---中断处理函数

		//---清楚中断标志位
		LL_USART_ClearFlag_FE(USARTx->msgUSART);
	}
	//---奇偶校验中断
	if (LL_USART_IsActiveFlag_PE(USARTx->msgUSART) && LL_USART_IsEnabledIT_PE(USARTx->msgUSART))
	{
		//---中断处理函数

		//---清楚中断标志位
		LL_USART_ClearFlag_PE(USARTx->msgUSART);
	}
}

////////////////////////////////////////////////////////////////////////////////
////DMA模式配置
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：接收DMA初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART1_Read_DMA_Init(USART_HandlerType* USARTx)
{
	//>>>---RX的DMA配置
	//---将DMA全部寄存器重新设置为缺省值
#ifdef USE_MCU_STM32F1
	//---F1对应是的DMA1的通道1对应ADC1
	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_1);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
#else
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_2);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
	USARTx->msgRxdHandler.msgDMA = DMA2;
	USARTx->msgRxdHandler.msgDMAChannelOrStream = LL_DMA_STREAM_2;
#endif
	//---DMA初始化结构体
	LL_DMA_InitTypeDef DMA_InitTypeDef = { 0 };
#ifndef USE_MCU_STM32F1
	//---DMA通道
	DMA_InitTypeDef.Channel = LL_DMA_CHANNEL_4;
#endif
	//---数据大小
	DMA_InitTypeDef.NbData = USARTx->msgRxdHandler.msgMaxSize;
	//---方向从外设到存储器
	DMA_InitTypeDef.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
#ifndef USE_MCU_STM32F1
	//---不用FIFO用直连模式
	DMA_InitTypeDef.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
	//---半字两字节
	DMA_InitTypeDef.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2;
#endif
	//---存储器地址
	DMA_InitTypeDef.MemoryOrM2MDstAddress = (UINT32_T)(USARTx->msgRxdHandler.pMsgVal);
	//---半字两字节
	DMA_InitTypeDef.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	//---存储器地址增加
	DMA_InitTypeDef.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	//---DMA正常模式
	DMA_InitTypeDef.Mode = LL_DMA_MODE_NORMAL; //LL_DMA_MODE_CIRCULAR;
	//---DMA外设地址
	DMA_InitTypeDef.PeriphOrM2MSrcAddress = (UINT32_T) & (USARTx->msgUSART->DR);
#ifndef USE_MCU_STM32F1
	//---无FIFO
	DMA_InitTypeDef.PeriphBurst = LL_DMA_PBURST_SINGLE;
#endif
	//---半字两字节
	DMA_InitTypeDef.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	//---外设基地址不变
	DMA_InitTypeDef.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	//---传输通道优先级为高
	DMA_InitTypeDef.Priority = LL_DMA_PRIORITY_HIGH;
	//---DMA初始化
#ifdef USE_MCU_STM32F1
	//---初始化DMA
	LL_DMA_Init(USARTx->msgRxHandler.msgDMA, USARTx->msgRxHandler.msgDMAChannelOrStream, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableChannel(USARTx->msgRxHandler.msgDMA, USARTx->msgRxHandler.msgDMAChannelOrStream);
#else
	//---初始化DMA
	LL_DMA_Init(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableStream(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
	//---是能串口接收DMA
	LL_USART_EnableDMAReq_RX(USARTx->msgUSART);
#endif
	//---中断配置
#ifdef USE_MCU_STM32F1
	//---DMA中断配置
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
#else
	//---DMA中断配置
	NVIC_SetPriority(DMA2_Stream2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);
#endif
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
	//<<<---RX的DMA配置结束
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：发送DMA的初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART1_Write_DMA_Init(USART_HandlerType* USARTx)
{
	//>>>---TX的DMA配置
//---将DMA全部寄存器重新设置为缺省值
#ifdef USE_MCU_STM32F1
	//---F1对应是的DMA1的通道1对应ADC1
	LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_1);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
#else
	LL_DMA_DeInit(DMA2, LL_DMA_STREAM_7);
	//---DMA时钟总线配置
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);
	USARTx->msgTxdHandler.msgDMA = DMA2;
	USARTx->msgTxdHandler.msgDMAChannelOrStream = LL_DMA_STREAM_7;
#endif
	//---DMA初始化结构体
	LL_DMA_InitTypeDef DMA_InitTypeDef = { 0 };
#ifndef USE_MCU_STM32F1
	//---DMA通道
	DMA_InitTypeDef.Channel = LL_DMA_CHANNEL_4;
#endif
	//---数据大小,如果是首次发送，这里的参数只能写0，否则容易发生数据不完整，可能只发送了接个字节就停止发送
	DMA_InitTypeDef.NbData = 0;//USARTx->msgTxdHandler.msgSize;
	//---方向从存储器到外设
	DMA_InitTypeDef.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
#ifndef USE_MCU_STM32F1
	//---不用FIFO用直连模式
	DMA_InitTypeDef.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
	//---半字两字节
	DMA_InitTypeDef.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_2;
#endif
	//---存储器地址
	DMA_InitTypeDef.MemoryOrM2MDstAddress = (UINT32_T)(USARTx->msgTxdHandler.pMsgVal);
	//---半字两字节
	DMA_InitTypeDef.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	//---存储器地址增加
	DMA_InitTypeDef.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	//---DMA正常模式
	DMA_InitTypeDef.Mode = LL_DMA_MODE_NORMAL; //LL_DMA_MODE_CIRCULAR;
	//---DMA外设地址
	DMA_InitTypeDef.PeriphOrM2MSrcAddress = (UINT32_T) & (USARTx->msgUSART->DR);
#ifndef USE_MCU_STM32F1
	//---无FIFO
	DMA_InitTypeDef.PeriphBurst = LL_DMA_PBURST_SINGLE;
#endif
	//---半字两字节
	DMA_InitTypeDef.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	//---外设基地址不变
	DMA_InitTypeDef.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	//---传输通道优先级为高
	DMA_InitTypeDef.Priority = LL_DMA_PRIORITY_HIGH;
	//---DMA初始化
#ifdef USE_MCU_STM32F1
	//---初始化DMA
	LL_DMA_Init(USARTx->msgTxHandler.msgDMA, USARTx->msgTxHandler.msgDMAChannelOrStream, &DMA_InitTypeDef);
	//---使能DMA
	LL_DMA_EnableChannel(USARTx->msgTxHandler.msgDMA, USARTx->msgTxHandler.msgDMAChannelOrStream);
#else
	//---初始化DMA
	LL_DMA_Init(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream, &DMA_InitTypeDef);
	//---使能DMA,注意发送状态下不能使能DMA，只有需要的时候才能打开
	//LL_DMA_EnableStream(USARTx->msgTxHandler.msgDMA, USARTx->msgTxHandler.msgDMAChannelOrStream);
	//---使能串口发送DMA
	LL_USART_EnableDMAReq_TX(USARTx->msgUSART);
#endif
	//---中断配置
#ifdef USE_MCU_STM32F1
	//---DMA中断配置
	NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
#else
	//---DMA中断配置
	NVIC_SetPriority(DMA2_Stream7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
	//---使能中断
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);
#endif
	//---使能DMA传输结束中断
	LL_DMA_EnableIT_TC(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
	//<<<---TX的DMA配置结束
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：关闭DMA的接受模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T USART_Read_DMA_STOP(USART_HandlerType* USARTx)
{
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_DisableChannel(USARTx->msgRxHandler.msgDMA, USARTx->msgRxHandler.msgDMAChannelOrStream);
	LL_DMA_ClearFlag_GI1(USARTx->msgRxHandler.msgDMA);
#else
	//---不使能DMA
	LL_DMA_DisableStream(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
#endif
	LL_DMA_ClearFlag(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
	//---计算接收数据的长度
	UINT16_T length = USARTx->msgRxdHandler.msgMaxSize - LL_DMA_GetDataLength(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
	return length;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：重启DMA的接受模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Read_DMA_RESTART(USART_HandlerType* USARTx)
{
	//---设置DMA读取数据的大小
	LL_DMA_SetDataLength(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream, USARTx->msgRxdHandler.msgMaxSize);
	//---使能DMA
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_EnableChannel(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
#else
	//---使能DMA
	LL_DMA_EnableStream(USARTx->msgRxdHandler.msgDMA, USARTx->msgRxdHandler.msgDMAChannelOrStream);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置DMA发送模式下，数据存储的地址
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Write_DMA_SetMemoryAddress(USART_HandlerType* USARTx,UINT8_T *pVal)
{
	#ifdef USE_MCU_STM32F1
		
	#else
		LL_DMA_SetMemoryAddress(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream,(UINT32_T)pVal);
	#endif
	return OK_0;
}


///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：停止DMA的发送模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T USART_Write_DMA_STOP(USART_HandlerType* USARTx)
{
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_DisableChannel(USARTx->msgRxHandler.msgDMA, USARTx->msgRxHandler.msgDMAChannelOrStream);
	LL_DMA_ClearFlag_GI1(USARTx->msgRxHandler.msgDMA);
#else
	//---不使能DMA
	LL_DMA_DisableStream(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
#endif
	LL_DMA_ClearFlag(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
	//---计算接收数据的长度
	UINT16_T length = USARTx->msgTxdHandler.msgMaxSize - LL_DMA_GetDataLength(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
	return length;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：重启DMA的发送模式
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_Write_DMA_RESTART(USART_HandlerType* USARTx)
{
	//---设置DMA发送数据的大小
	LL_DMA_SetDataLength(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream, USARTx->msgTxdHandler.msgCount);
#ifdef USE_MCU_STM32F1
	//---使能DMA
	LL_DMA_EnableChannel(USARTx->msgTxHandler.msgDMA, USARTx->msgTxHandler.msgDMAChannelOrStream);
#else
	//---使能DMA
	LL_DMA_EnableStream(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
#endif
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T USART_DMA_IDLETask(USART_HandlerType* USARTx)
{
	UINT8_T _return = OK_0;
	//---停止接收DMA模式,并获取接收数据的长度
	UINT16_T dataLength= USART_Read_DMA_STOP(USARTx);
	//---校验报头是否正确
	if (USARTx->msgRxdHandler.pMsgVal[0] == USARTx->msgRxdID)
	{
		//---数据长度信息
		UINT16_T length = USARTx->msgRxdHandler.pMsgVal[1];
		//---校验数据长度信息
		if (USARTx->msgRxdHandler.msgMaxSize < 0xFF)
		{
			length += 2;
		}
		else
		{
			length = (length << 8) + USARTx->msgRxdHandler.pMsgVal[1];
			length += 3;
		}
		//---校验数据是否合法
		if (length == dataLength)
		{
			USARTx->msgRxdHandler.msgState = USART_OK;
			USARTx->msgRxdHandler.msgCount=dataLength;
		}
		else
		{
			//---数据不符合规则，使能重新接收
			USART_Read_Init(USARTx);
			//---返回错误信息
			_return = ERROR_2;
		}
	}
	else
	{
		_return = ERROR_1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：接收中断DMA处理程序
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_Read_DMA_IRQTask(USART_HandlerType* USARTx)
{
	//---DMA处理模式
	USART_DMA_IDLETask(USARTx);
	//---清楚空闲中断标志位
	LL_USART_ClearFlag_IDLE(USARTx->msgUSART);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：接收中断DMA处理程序
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void USART_Write_DMA_IRQTask(USART_HandlerType* USARTx)
{
#ifdef USE_MCU_STM32F1
	//---不使能DMA
	LL_DMA_DisableChannel(USARTx->msgTxHandler.msgDMA, USARTx->msgTxHandler.msgDMAChannelOrStream;
	LL_DMA_ClearFlag_GI1(DMA1);
#else
	//---不使能DMA
	LL_DMA_DisableStream(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
#endif
	//---清除标识
	LL_DMA_ClearFlag(USARTx->msgTxdHandler.msgDMA, USARTx->msgTxdHandler.msgDMAChannelOrStream);
	//---清零发送
	//USART_Write_Init(USARTx);
	//---使能发送完成中断，用于切换TXD端口为输入状态
	LL_USART_EnableIT_TC(USARTx->msgUSART);
}
