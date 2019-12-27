#include "stm32_cfg.h"

HardFault_HandlerType	g_HardFaultErrMsg = { 0 };
pHardFault_HandlerType	pHardFaultErrMsg  = &g_HardFaultErrMsg;

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void _irq_disable(void)
{
	__asm volatile("cpsid i");
}

///////////////////////////////////////////////////////////////////////////////
//////函	   数：
//////功	   能：
//////输入参数:
//////输出参数:
//////说	   明：
//////////////////////////////////////////////////////////////////////////////
void _irq_enable(void)
{
	__asm volatile("cpsie i");
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void _Error_Handler(char * file, int line)
{
	while (1)
	{
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 进入临界区
//////////////////////////////////////////////////////////////////////////////
void CrisEnter(void)
{
	__set_PRIMASK(1);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 退出临界区
//////////////////////////////////////////////////////////////////////////////
void CrisExit(void)
{
	__set_PRIMASK(0);
}

#ifdef USE_MCU_STM32F1
///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_EnableDataCache(void)
{
	SET_BIT(FLASH->ACR, (1 << 10));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_DisableDataCache(void)
{
	CLEAR_BIT(FLASH->ACR, (1 << 10));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_EnableInstCache(void)
{
	SET_BIT(FLASH->ACR, (1 << 9));
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_DisableInstCache(void)
{
	CLEAR_BIT(FLASH->ACR, (1 << 9));
}

#endif

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 解锁Flash的写保护
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_Unlock(void)
{
	//---禁止数据缓存
	LL_FLASH_DisableDataCache();

	//---禁止Flash的预读取功能
	LL_FLASH_DisablePrefetch();

	//---写入解锁序列
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
#if defined(FLASH_BANK2_END)
	FLASH->KEYR2 = FLASH_KEY1;
	FLASH->KEYR2 = FLASH_KEY2;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 开启Flash的写保护
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
#if defined(FLASH_BANK2_END)
	FLASH->CR2 |= FLASH_CR_LOCK;
#endif

	//---开启数据缓存
	LL_FLASH_EnableDataCache();

	//---使能Flash的预读取功能
	LL_FLASH_EnablePrefetch();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_OB_Unlock(void)
{
#ifdef USE_MCU_STM32F1
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY1);
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY2);
#else
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPT_KEY1);
	WRITE_REG(FLASH->OPTKEYR, FLASH_OPT_KEY2);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_OB_Lock(void)
{
#ifdef USE_MCU_STM32F1
	CLEAR_BIT(FLASH->CR, FLASH_CR_OPTWRE);
#else
	CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTLOCK);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 获取Flash 状态
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_GetBank1State(void)
{
	UINT32_T res = FLASH->SR;
	if ((res&LL_FLASH_FLAG_BSY) == LL_FLASH_FLAG_BSY)
	{
		//----忙
		return 1;
	}
	else if ((res&LL_FLASH_FLAG_PGERR) != 0)
	{
		//---编程错误
		return 2;
	}
	else if ((res&LL_FLASH_FLAG_WRPERR) != 0)
	{
		//---写保护错误
		return 3;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_GetBank2State(void)
{
#if defined(FLASH_BANK2_END)
	UINT32_T res = FLASH->SR2;
	if ((res&(LL_FLASH_FLAG_BSY_BANK2 & 0x7FFFFFFF)) == (LL_FLASH_FLAG_BSY_BANK2 & 0x7FFFFFFF))
	{
		//----忙
		return 1;
	}
	else if ((res&(LL_FLASH_FLAG_PGERR_BANK2 & 0x7FFFFFFF)) != 0)
	{
		//---编程错误
		return 2;
	}
	else if ((res&(LL_FLASH_FLAG_WRPERR_BANK2 & 0x7FFFFFFF)) != 0)
	{
		//---写保护错误
		return 3;
	}
	return 0;
#else
	return 0;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_ClearFlag(void)
{
	CLEAR_REG(FLASH->SR);
#if defined(FLASH_BANK2_END)
	CLEAR_REG(FLASH->SR2);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 获取Flash写保护配置字
//////////////////////////////////////////////////////////////////////////////
UINT32_T LL_FLASH_OB_GetWRP(void)
{
#ifdef USE_MCU_STM32F1
	return (UINT32_T)(READ_REG(FLASH->WRPR));
#else
	return (*(VLTUINT16_T *)(OPTCR_BYTE2_ADDRESS));
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 获取Flash读保护等级
//////////////////////////////////////////////////////////////////////////////
UINT32_T LL_FLASH_OB_GetRDP(void)
{
	UINT32_T readstatus = LL_OB_RDP_LEVEL_0;
	UINT32_T tmp_reg = 0U;

	//---获取读保护的配置
#ifdef USE_MCU_STM32F1
	tmp_reg = READ_BIT(FLASH->OBR, FLASH_OBR_RDPRT);
	if (tmp_reg == FLASH_OBR_RDPRT)
	{
		readstatus = LL_OB_RDP_LEVEL_1;
	}
	else
	{
		readstatus = LL_OB_RDP_LEVEL_0;
	}
#else
	tmp_reg = (*(VLTUINT8_T*)(OPTCR_BYTE1_ADDRESS));
	if (tmp_reg == LL_OB_RDP_LEVEL_2)
	{
		readstatus = LL_OB_RDP_LEVEL_2;
	}
	else if (tmp_reg == LL_OB_RDP_LEVEL_1)
	{
		readstatus = LL_OB_RDP_LEVEL_1;
	}
	else
	{
		readstatus = LL_OB_RDP_LEVEL_0;
	}
#endif

	return readstatus;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 获取用户配置
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_OB_GetUser(void)
{
#ifdef USE_MCU_STM32F1
	/* Return the User Option Byte */
	return (UINT8_T)((READ_REG(FLASH->OBR) & FLASH_OBR_USER) >> FLASH_OBR_IWDG_SW_Pos);
#else
	return ((uint8_t)(FLASH->OPTCR & 0xE0));
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 配置读保护等级
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_OB_RDP_LevelConfig(UINT8_T rdpLevel)
{
	UINT8_T _return = LL_FLASH_WaitBank1Operation(50000);
	if (_return == 0)
	{
	#ifdef USE_MCU_STM32F1
		/* If the previous operation is completed, proceed to erase the option bytes */
		SET_BIT(FLASH->CR, FLASH_CR_OPTER);
		SET_BIT(FLASH->CR, FLASH_CR_STRT);
		/* Wait for last operation to be completed */
		_return = LL_FLASH_WaitBank1Operation(50000);
		/* If the erase operation is completed, disable the OPTER Bit */
		CLEAR_BIT(FLASH->CR, FLASH_CR_OPTER);
		if (_return == 0)
		{
			/* Enable the Option Bytes Programming operation */
			SET_BIT(FLASH->CR, FLASH_CR_OPTPG);
			WRITE_REG(OB->RDP, rdpLevel);
			/* Wait for last operation to be completed */
			_return = LL_FLASH_WaitBank1Operation(50000);
			/* if the program operation is completed, disable the OPTPG Bit */
			CLEAR_BIT(FLASH->CR, FLASH_CR_OPTPG);
		}
	#else
		*(VLTUINT8_T*)OPTCR_BYTE1_ADDRESS = rdpLevel;
	#endif
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 等待操作完成
//////输入参数:
//////输出参数:
//////说		明： 0 - 操作完成, 可以进行其他读写操作
//////////////   1 - 忙,指示闪存操作正在进行,
//////////////   2 - 编程错误, 试图对内容不是0xFFFF的地址进行写入
//////////////   3 - 写保护错误, 试图对写保护的闪存地址编程
//////////////   0xFF - 等待超时
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_WaitBank1Operation(UINT32_T timeout)
{
	//---获取编程状态
	UINT8_T  _return = LL_FLASH_GetBank1State();
	while ((_return != 0) && (timeout != 0))
	{
		_return = LL_FLASH_GetBank1State();
		timeout--;
	}
	if (timeout == 0)
	{
		//---超时错误
		_return = 0xFF;
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
UINT8_T LL_FLASH_WaitBank2Operation(UINT32_T timeout)
{
	//---获取编程状态
	UINT8_T  _return = LL_FLASH_GetBank2State();
	while ((_return != 0) && (timeout != 0))
	{
		_return = LL_FLASH_GetBank2State();
		timeout--;
	}
	if (timeout == 0)
	{
		//---超时错误
		_return = 0xFF;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 0---成功，非0---失败
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_ErasePage(UINT32_T pageAddr)
{
	UINT8_T _return = 0;
#if defined(FLASH_BANK2_END)
	if (pageAddr > FLASH_BANK1_END)
	{
		//---等待操作完成
		_return = LL_FLASH_WaitBank2Operation(LL_FLASH_ERASE_TIMEOUT);
		if (_return == 0)
		{
			//---页擦除
			SET_BIT(FLASH->CR2, FLASH_CR2_PER);
		#ifdef USE_MCU_STM32F1

			//---选择擦除的页
			WRITE_REG(FLASH->AR2, pageAddr);
		#else

			//---选择擦除的页
			WRITE_REG(FLASH->ACR2, pageAddr);
		#endif

			//---开始擦除
			SET_BIT(FLASH->CR2, FLASH_CR2_STRT);

			//---等待操作完成
			_return = LL_FLASH_WaitBank2Operation(LL_FLASH_ERASE_TIMEOUT);

			//---不使能擦除
			CLEAR_BIT(FLASH->CR2, FLASH_CR2_PER);
		}
	}
	else
	{
	#endif

		//---等待操作完成
		_return = LL_FLASH_WaitBank1Operation(LL_FLASH_ERASE_TIMEOUT);
		if (_return == 0)
		{
		#ifdef USE_MCU_STM32F1

			//---页擦除
			SET_BIT(FLASH->CR, FLASH_CR_PER);
		#else

			//---页擦除
			SET_BIT(FLASH->CR, FLASH_CR_SER);
		#endif

			//---选择擦除的页
		#ifdef USE_MCU_STM32F1
			WRITE_REG(FLASH->AR, pageAddr);
		#else
			WRITE_REG(FLASH->ACR, pageAddr);
		#endif

			//---开始擦除
			SET_BIT(FLASH->CR, FLASH_CR_STRT);

			//---等待操作完成
			_return = LL_FLASH_WaitBank1Operation(LL_FLASH_ERASE_TIMEOUT);
		#ifdef USE_MCU_STM32F1

			//---清除页擦除标志
			CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
		#else

			//---清除页擦除标志
			CLEAR_BIT(FLASH->CR, FLASH_CR_SER);
		#endif
		}
	#if defined(FLASH_BANK2_END)
	}
#endif
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T LL_FLASH_EraseAllPage(void)
{
	UINT8_T _return = 0;
#if defined(FLASH_BANK2_END)

	//---等待操作完成
	_return = LL_FLASH_WaitBank2Operation(LL_FLASH_ERASE_TIMEOUT);
	if (_return == 0)
	{
		//---页擦除
		SET_BIT(FLASH->CR2, FLASH_CR2_MER);

		//---开始擦除
		SET_BIT(FLASH->CR2, FLASH_CR2_STRT);

		//---等待操作完成
		_return = LL_FLASH_WaitBank2Operation(LL_FLASH_ERASE_TIMEOUT);

		//---不使能擦除
		CLEAR_BIT(FLASH->CR2, FLASH_CR2_MER);
	}
#endif

	//---等待操作完成
	_return = LL_FLASH_WaitBank1Operation(LL_FLASH_ERASE_TIMEOUT);
	if (_return == 0)
	{
	#ifdef USE_MCU_STM32F1

		//---页擦除
		SET_BIT(FLASH->CR, FLASH_CR_MER);
	#else

		//---页擦除
		SET_BIT(FLASH->CR, FLASH_CR_MER);
	#endif

		//---开始擦除
		SET_BIT(FLASH->CR, FLASH_CR_STRT);

		//---等待操作完成
		_return = LL_FLASH_WaitBank1Operation(LL_FLASH_ERASE_TIMEOUT);
	#ifdef USE_MCU_STM32F1

		//---清除页擦除标志
		CLEAR_BIT(FLASH->CR, FLASH_CR_MER);
	#else

		//---清除页擦除标志
		CLEAR_BIT(FLASH->CR, FLASH_CR_MER);
	#endif
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
UINT8_T LL_FLASH_WriteHalfWord(UINT32_T pageAddr, UINT16_T val)
{
	UINT8_T _return = 0;
#if defined(FLASH_BANK2_END)
	if (pageAddr <= FLASH_BANK1_END)
	{
	#endif

		//---编程是能
		SET_BIT(FLASH->CR, FLASH_CR_PG);
	#if defined(FLASH_BANK2_END)
	}
	else
	{
		//---编程使能
		SET_BIT(FLASH->CR2, FLASH_CR2_PG);
	}
#endif

	//---写入数据
	*(VLTUINT16_T*)pageAddr = val;
#if defined(FLASH_BANK2_END)
	if (pageAddr <= FLASH_BANK1_END)
	{
	#endif

		//---等待操作完成
		_return = LL_FLASH_WaitBank1Operation(LL_FLASH_ERASE_TIMEOUT);

		//---清除页编程标志
		CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	#if defined(FLASH_BANK2_END)
	}
	else
	{
		//---等待操作完成
		_return = LL_FLASH_WaitBank2Operation(LL_FLASH_ERASE_TIMEOUT);

		//---清除页擦除标志
		CLEAR_BIT(FLASH->CR2, FLASH_CR2_PG);
	}
#endif
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 读取半字(16Bits)
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT16_T LL_FLASH_ReadHalfWord(UINT32_T pageAddr)
{
	//---读取数据
	return *(VLTUINT16_T*)pageAddr;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取全字(32Bits)
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T LL_FLASH_ReadFullWord(UINT32_T pageAddr)
{
	uint32_t temp1, temp2;
	temp1 = *(VLTUINT16_T*)pageAddr;
	temp2 = *(VLTUINT16_T*)(pageAddr + 2);

	//---读取数据
	return (temp2 << 16) + temp1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_FLASH_SetProgramSize(UINT32_T psize)
{
#ifdef FLASH_CR_PSIZE
	MODIFY_REG(FLASH->CR, FLASH_CR_PSIZE, psize);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：读取设备的ID信息
//////////////////////////////////////////////////////////////////////////////
void LL_GetUID(UINT32_T *uid)
{
	uid[0] = LL_GetUID_Word0();
	uid[1] = LL_GetUID_Word1();
	uid[2] = LL_GetUID_Word2();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：恢复NVIC为复位状态.使中断不再发生.
//////////////////////////////////////////////////////////////////////////////
void LL_NVIC_DeInit(void)
{
	UINT8_T index = 0;

	NVIC->ICER[0] = 0xFFFFFFFF;
	NVIC->ICER[1] = 0x000007FF;
	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICPR[1] = 0x000007FF;

	for (index = 0; index < 0x0B; index++)
	{
		NVIC->IP[index] = 0x00000000;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_TIM_ClearFlag(TIM_TypeDef *TIMx)
{
	TIMx->SR = 0xFFFFFFFF;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T  LL_TIM_GetFlag(TIM_TypeDef *TIMx)
{
	return (TIMx->SR);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LL_USART_ClearFlag(USART_TypeDef *USARTx)
{
	USARTx->SR = 0xFFFFFFFF;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T LL_USART_GetFlag(USART_TypeDef *USARTx)
{
	return (USARTx->SR);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 获取硬件错误
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HardFault_Msg( unsigned int * hardfault_args )
{
	memset( &g_HardFaultErrMsg, 0, sizeof( HardFault_HandlerType ) );

	g_HardFaultErrMsg.stacked_r0	= ( (unsigned int)hardfault_args[0] );
	g_HardFaultErrMsg.stacked_r1	= ( (unsigned int)hardfault_args[1] );
	g_HardFaultErrMsg.stacked_r2	= ( (unsigned int)hardfault_args[2] );
	g_HardFaultErrMsg.stacked_r3	= ( (unsigned int)hardfault_args[3] );

	g_HardFaultErrMsg.stacked_r12	= ( (unsigned int)hardfault_args[4] );
	g_HardFaultErrMsg.stacked_lr	= ( (unsigned int)hardfault_args[5] );
	g_HardFaultErrMsg.stacked_pc	= ( (unsigned int)hardfault_args[6] );
	g_HardFaultErrMsg.stacked_psr	= ( (unsigned int)hardfault_args[7] );

	__ASM volatile("BKPT #01");
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 硬件错误处理函数
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HardFault_IRQTask( void )
{
	__ASM(	"TST LR,#4");
	__ASM(	"ITE EQ\n"
			"MRSEQ R0, MSP\n"
			"MRSNE R0, PSP"
		);
	__ASM(	"B HardFault_Msg");
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能：清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag_HT(DMA_TypeDef* DMAx,UINT32_T channelOrStream )
{
	#ifdef USE_MCU_STM32F1
		if (channelOrStream == LL_DMA_CHANNEL_0)
		{
			LL_DMA_ClearFlag_HT0(DMAx);
}
		else if (channelOrStream == LL_DMA_CHANNEL_1)
		{
			LL_DMA_ClearFlag_HT1(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_2)
		{
			LL_DMA_ClearFlag_HT2(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_3)
		{
			LL_DMA_ClearFlag_HT3(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_4)
		{
			LL_DMA_ClearFlag_HT4(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_5)
		{
			LL_DMA_ClearFlag_HT5(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_6)
		{
			LL_DMA_ClearFlag_HT6(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_7)
		{
			LL_DMA_ClearFlag_HT7(DMAx);
		}
	#elif defined(USE_MCU_STM32F4)
		if (channelOrStream == LL_DMA_STREAM_0)
		{
			LL_DMA_ClearFlag_HT0(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_1)
		{
			LL_DMA_ClearFlag_HT1(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_2)
		{
			LL_DMA_ClearFlag_HT2(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_3)
		{
			LL_DMA_ClearFlag_HT3(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_4)
		{
			LL_DMA_ClearFlag_HT4(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_5)
		{
			LL_DMA_ClearFlag_HT5(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_6)
		{
			LL_DMA_ClearFlag_HT6(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_7)
		{
			LL_DMA_ClearFlag_HT7(DMAx);
		}
	#else
		#error "不支持的STM32芯片"
	#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag_TC(DMA_TypeDef* DMAx, UINT32_T channelOrStream)
{
	#ifdef USE_MCU_STM32F1
		if (channelOrStream == LL_DMA_CHANNEL_0)
		{
			LL_DMA_ClearFlag_TC0(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_1)
		{
			LL_DMA_ClearFlag_TC1(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_2)
		{
			LL_DMA_ClearFlag_TC2(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_3)
		{
			LL_DMA_ClearFlag_TC3(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_4)
		{
			LL_DMA_ClearFlag_TC4(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_5)
		{
			LL_DMA_ClearFlag_TC5(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_6)
		{
			LL_DMA_ClearFlag_TC6(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_7)
		{
			LL_DMA_ClearFlag_TC7(DMAx);
		}
	#elif defined(USE_MCU_STM32F4)
		if (channelOrStream == LL_DMA_STREAM_0)
		{
			LL_DMA_ClearFlag_TC0(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_1)
		{
			LL_DMA_ClearFlag_TC1(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_2 )
		{
			LL_DMA_ClearFlag_TC2(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_3)
		{
			LL_DMA_ClearFlag_TC3(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_4)
		{
			LL_DMA_ClearFlag_TC4(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_5)
		{
			LL_DMA_ClearFlag_TC5(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_6)
		{
			LL_DMA_ClearFlag_TC6(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_7)
		{
			LL_DMA_ClearFlag_TC7(DMAx);
		}
	#else
		#error "不支持的STM32芯片"
	#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag_TE(DMA_TypeDef* DMAx, UINT32_T channelOrStream)
{
#ifdef USE_MCU_STM32F1
	if (channelOrStream == LL_DMA_CHANNEL_0)
	{
		LL_DMA_ClearFlag_TC0(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_1)
	{
		LL_DMA_ClearFlag_TC1(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_2)
	{
		LL_DMA_ClearFlag_TC2(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_3)
	{
		LL_DMA_ClearFlag_TC3(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_4)
	{
		LL_DMA_ClearFlag_TC4(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_5)
	{
		LL_DMA_ClearFlag_TC5(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_6)
	{
		LL_DMA_ClearFlag_TC6(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_7)
	{
		LL_DMA_ClearFlag_TC7(DMAx);
	}
#elif defined(USE_MCU_STM32F4)
	if (channelOrStream == LL_DMA_STREAM_0)
	{
		LL_DMA_ClearFlag_TE0(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_1)
	{
		LL_DMA_ClearFlag_TE1(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_2)
	{
		LL_DMA_ClearFlag_TE2(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_3)
	{
		LL_DMA_ClearFlag_TE3(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_4)
	{
		LL_DMA_ClearFlag_TE4(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_5)
	{
		LL_DMA_ClearFlag_TE5(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_6)
	{
		LL_DMA_ClearFlag_TE6(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_7)
	{
		LL_DMA_ClearFlag_TE7(DMAx);
	}
#else
	#error "不支持的STM32芯片"
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag_DME(DMA_TypeDef* DMAx, UINT32_T channelOrStream)
{
#ifdef USE_MCU_STM32F1
	if (channelOrStream == LL_DMA_CHANNEL_0)
	{
		LL_DMA_ClearFlag_TC0(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_1)
	{
		LL_DMA_ClearFlag_TC1(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_2)
	{
		LL_DMA_ClearFlag_TC2(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_3)
	{
		LL_DMA_ClearFlag_TC3(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_4)
	{
		LL_DMA_ClearFlag_TC4(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_5)
	{
		LL_DMA_ClearFlag_TC5(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_6)
	{
		LL_DMA_ClearFlag_TC6(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_7)
	{
		LL_DMA_ClearFlag_TC7(DMAx);
	}
#elif defined(USE_MCU_STM32F4)
	if (channelOrStream == LL_DMA_STREAM_0)
	{
		LL_DMA_ClearFlag_DME0(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_1)
	{
		LL_DMA_ClearFlag_DME1(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_2)
	{
		LL_DMA_ClearFlag_DME2(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_3)
	{
		LL_DMA_ClearFlag_DME3(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_4)
	{
		LL_DMA_ClearFlag_DME4(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_5)
	{
		LL_DMA_ClearFlag_DME5(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_6)
	{
		LL_DMA_ClearFlag_DME6(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_7)
	{
		LL_DMA_ClearFlag_DME7(DMAx);
	}
#else
	#error "不支持的STM32芯片"
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag_FE(DMA_TypeDef* DMAx, UINT32_T channelOrStream)
{
#ifdef USE_MCU_STM32F1
	if (channelOrStream == LL_DMA_CHANNEL_0)
	{
		LL_DMA_ClearFlag_TC0(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_1)
	{
		LL_DMA_ClearFlag_TC1(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_2)
	{
		LL_DMA_ClearFlag_TC2(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_3)
	{
		LL_DMA_ClearFlag_TC3(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_4)
	{
		LL_DMA_ClearFlag_TC4(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_5)
	{
		LL_DMA_ClearFlag_TC5(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_6)
	{
		LL_DMA_ClearFlag_TC6(DMAx);
	}
	else if (channelOrStream == LL_DMA_CHANNEL_7)
	{
		LL_DMA_ClearFlag_TC7(DMAx);
	}
#elif defined(USE_MCU_STM32F4)
	if (channelOrStream == LL_DMA_STREAM_0)
	{
		LL_DMA_ClearFlag_FE0(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_1)
	{
		LL_DMA_ClearFlag_FE1(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_2)
	{
		LL_DMA_ClearFlag_FE2(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_3)
	{
		LL_DMA_ClearFlag_FE3(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_4)
	{
		LL_DMA_ClearFlag_FE4(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_5)
	{
		LL_DMA_ClearFlag_FE5(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_6)
	{
		LL_DMA_ClearFlag_FE6(DMAx);
	}
	else if (channelOrStream == LL_DMA_STREAM_7)
	{
		LL_DMA_ClearFlag_FE7(DMAx);
	}
#else
	#error "不支持的STM32芯片"
#endif
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 清除标志位
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void LL_DMA_ClearFlag(DMA_TypeDef* DMAx, UINT32_T channelOrStream)
{
	#ifdef USE_MCU_STM32F1
		if (channelOrStream == LL_DMA_CHANNEL_0)
		{
			LL_DMA_ClearFlag_HT0(DMAx);
			LL_DMA_ClearFlag_TC0(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_1)
		{
			LL_DMA_ClearFlag_HT1(DMAx);
			LL_DMA_ClearFlag_TC1(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_2)
		{
			LL_DMA_ClearFlag_HT2(DMAx);
			LL_DMA_ClearFlag_TC2(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_3)
		{
			LL_DMA_ClearFlag_HT3(DMAx);
			LL_DMA_ClearFlag_TC3(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_4)
		{
			LL_DMA_ClearFlag_HT4(DMAx);
			LL_DMA_ClearFlag_TC4(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_5)
		{
			LL_DMA_ClearFlag_HT5(DMAx);
			LL_DMA_ClearFlag_TC5(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_6)
		{
			LL_DMA_ClearFlag_HT6(DMAx);
			LL_DMA_ClearFlag_TC6(DMAx);
		}
		else if (channelOrStream == LL_DMA_CHANNEL_7)
		{
			LL_DMA_ClearFlag_HT7(DMAx);
			LL_DMA_ClearFlag_TC7(DMAx);
		}
	#elif defined(USE_MCU_STM32F4)
		if (channelOrStream == LL_DMA_STREAM_0)
		{
			LL_DMA_ClearFlag_HT0(DMAx);
			LL_DMA_ClearFlag_TC0(DMAx);
			LL_DMA_ClearFlag_TE0(DMAx);
			LL_DMA_ClearFlag_FE0(DMAx);
			LL_DMA_ClearFlag_DME0(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_1)
		{
			LL_DMA_ClearFlag_HT1(DMAx);
			LL_DMA_ClearFlag_TC1(DMAx);
			LL_DMA_ClearFlag_TE1(DMAx);
			LL_DMA_ClearFlag_FE1(DMAx);
			LL_DMA_ClearFlag_DME1(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_2)
		{
			LL_DMA_ClearFlag_HT2(DMAx);
			LL_DMA_ClearFlag_TC2(DMAx);
			LL_DMA_ClearFlag_TE2(DMAx);
			LL_DMA_ClearFlag_FE2(DMAx);
			LL_DMA_ClearFlag_DME2(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_3)
		{
			LL_DMA_ClearFlag_HT3(DMAx);
			LL_DMA_ClearFlag_TC3(DMAx);
			LL_DMA_ClearFlag_TE3(DMAx);
			LL_DMA_ClearFlag_FE3(DMAx);
			LL_DMA_ClearFlag_DME3(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_4)
		{
			LL_DMA_ClearFlag_HT4(DMAx);
			LL_DMA_ClearFlag_TC4(DMAx);
			LL_DMA_ClearFlag_TE4(DMAx);
			LL_DMA_ClearFlag_FE4(DMAx);
			LL_DMA_ClearFlag_DME4(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_5)
		{
			LL_DMA_ClearFlag_HT5(DMAx);
			LL_DMA_ClearFlag_TC5(DMAx);
			LL_DMA_ClearFlag_TE5(DMAx);
			LL_DMA_ClearFlag_FE5(DMAx);
			LL_DMA_ClearFlag_DME5(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_6)
		{
			LL_DMA_ClearFlag_HT6(DMAx);
			LL_DMA_ClearFlag_TC6(DMAx);
			LL_DMA_ClearFlag_TE6(DMAx);
			LL_DMA_ClearFlag_FE6(DMAx);
			LL_DMA_ClearFlag_DME6(DMAx);
		}
		else if (channelOrStream == LL_DMA_STREAM_7)
		{
			LL_DMA_ClearFlag_HT7(DMAx);
			LL_DMA_ClearFlag_TC7(DMAx);
			LL_DMA_ClearFlag_TE7(DMAx);
			LL_DMA_ClearFlag_FE7(DMAx);
			LL_DMA_ClearFlag_DME7(DMAx);
		}
	#else
	#error "不支持的STM32芯片"
	#endif
}
