#ifndef STM32_CFG_H_
#define STM32_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	////////////////////////////////////////////////////////////////////////////////////////////
	//---定义使用的头文件
	//#define USE_MCU_STM32F0
	//#define USE_MCU_STM32F1
	//#define USE_MCU_STM32F2
	//#define USE_MCU_STM32F3
	#define USE_MCU_STM32F4	
	//===检查是否定义了外部使用的晶振
	#ifndef HSE_VALUE
		#define	HSE_VALUE			8000000UL
	#endif	
	//===选择系列---寄存器的定义
	#if  defined(USE_MCU_STM32F0)
		#include "stm32f0xx.h"
		#include "system_stm32f0xx.h"
	#elif defined(USE_MCU_STM32F1)
		#include "stm32f1xx.h"
		#include "system_stm32f1xx.h"
	#elif defined(USE_MCU_STM32F2)
		#include "stm32f2xx.h"
		#include "system_stm32f2xx.h"
	#elif defined(USE_MCU_STM32F3)
		#include "stm32f3xx.h"
		#include "system_stm32f3xx.h"
	#elif defined(USE_MCU_STM32F4)
		#include "stm32f4xx.h"
		#include "system_stm32f4xx.h"
	#else
		#error "STM32器件的型号选择错误!"
	#endif
	
	//===LL库中的BUG，这样是为了不用动底层的库，提供的兼容方式
	#ifdef ADC3
		#ifndef ADC3_COMMON
			#define ADC3_COMMON 	((ADC_Common_TypeDef *)ADC3_BASE)
		#endif
	#endif
	
	//===使用的库文件
	#if defined(USE_HAL_DRIVER)||defined(USE_FULL_LL_DRIVER)
	
		//===LL库
		#if defined (USE_FULL_LL_DRIVER )
		
			//===定义NVIC使用的分组
			#ifndef NVIC_PRIORITYGROUP_4
				#define NVIC_PRIORITYGROUP_0         0x00000007U
				#define NVIC_PRIORITYGROUP_1         0x00000006U
				#define NVIC_PRIORITYGROUP_2         0x00000005U
				#define NVIC_PRIORITYGROUP_3         0x00000004U
				#define NVIC_PRIORITYGROUP_4         0x00000003U
			#endif
		
			//===定义FLASH操作参数
			#if defined(FLASH_BANK2_END)
		
				#define LL_FLASH_FLAG_BSY             LL_FLASH_FLAG_BSY_BANK1       /*!< FLASH Bank1 Busy flag                   */
				#define LL_FLASH_FLAG_PGERR           LL_FLASH_FLAG_PGERR_BANK1     /*!< FLASH Bank1 Programming error flag      */
				#define LL_FLASH_FLAG_WRPERR          LL_FLASH_FLAG_WRPERR_BANK1    /*!< FLASH Bank1 Write protected error flag  */
				#define LL_FLASH_FLAG_EOP             LL_FLASH_FLAG_EOP_BANK1       /*!< FLASH Bank1 End of Operation flag       */
				
				#define LL_FLASH_FLAG_BSY_BANK1       FLASH_SR_BSY					/*!< FLASH Bank1 Busy flag                   */
				#define LL_FLASH_FLAG_PGERR_BANK1     FLASH_SR_PGERR				/*!< FLASH Bank1 Programming error flag      */
				#define LL_FLASH_FLAG_WRPERR_BANK1    FLASH_SR_WRPRTERR				/*!< FLASH Bank1 Write protected error flag  */
				#define LL_FLASH_FLAG_EOP_BANK1       FLASH_SR_EOP					/*!< FLASH Bank1 End of Operation flag       */
				
				#define LL_FLASH_FLAG_BSY_BANK2       (FLASH_SR2_BSY |0x80000000)		/*!< FLASH Bank2 Busy flag                   */
				#define LL_FLASH_FLAG_PGERR_BANK2     (FLASH_SR2_PGERR |0x80000000)		/*!< FLASH Bank2 Programming error flag      */
				#define LL_FLASH_FLAG_WRPERR_BANK2    (FLASH_SR2_WRPRTERR |0x80000000)	/*!< FLASH Bank2 Write protected error flag  */
				#define LL_FLASH_FLAG_EOP_BANK2       (FLASH_SR2_EOP |0x80000000)		/*!< FLASH Bank2 End of Operation flag       */
	
			#else
	
				#define LL_FLASH_FLAG_BSY             FLASH_SR_BSY				    /*!< FLASH Busy flag                          */
	
				#ifdef FLASH_SR_PGERR
					#define LL_FLASH_FLAG_PGERR           FLASH_SR_PGERR				/*!< FLASH Programming error flag             */
				#else
					#define LL_FLASH_FLAG_PGERR           FLASH_SR_PGPERR				/*!< FLASH Programming error flag             */
				#endif
	
				#ifdef FLASH_SR_WRPRTERR
					#define LL_FLASH_FLAG_WRPERR          FLASH_SR_WRPRTERR				/*!< FLASH Write protected error flag         */
				#else
					#define LL_FLASH_FLAG_WRPERR          FLASH_SR_WRPERR				/*!< FLASH Write protected error flag         */
				#endif
	
				#define LL_FLASH_FLAG_EOP             FLASH_SR_EOP					/*!< FLASH End of Operation flag              */
	
				#ifdef FLASH_CR_PSIZE
					#define LL_FLASH_PSIZE_8BITS			(0<<FLASH_CR_PSIZE_Pos)
					#define LL_FLASH_PSIZE_16BITS			(1<<FLASH_CR_PSIZE_Pos)
					#define LL_FLASH_PSIZE_32BITS			(2<<FLASH_CR_PSIZE_Pos)
					#define LL_FLASH_PSIZE_64BITS			(3<<FLASH_CR_PSIZE_Pos)
				#endif
	
			#endif
		
			//===定义Flash擦除的超时试讲
			#define LL_FLASH_ERASE_TIMEOUT          ((UINT32_T)0x000B0000)
			
			#ifdef USE_MCU_STM32F1
				#define LL_OB_RDP_LEVEL_0				((UINT8_T)0xA5)
				#define LL_OB_RDP_LEVEL_1			    ((UINT8_T)0x00)
			#else
				#define LL_OB_RDP_LEVEL_0				((UINT8_T)0xAA)
				#define LL_OB_RDP_LEVEL_1				((UINT8_T)0x55)
				#define LL_OB_RDP_LEVEL_2				((UINT8_T)0xCC)
			#endif
			
			//===根据不同不同型号的MCU选择头文件
			#ifdef USE_MCU_STM32F0
				#include "stm32f0xx_ll.h"
			#elif defined USE_MCU_STM32F1
				#include "stm32f1xx_ll.h"
		
				//===GPIO的翻转时钟定义
				#ifndef LL_GPIO_SPEED_FREQ_VERY_HIGH
					#define LL_GPIO_SPEED_FREQ_VERY_HIGH		LL_GPIO_SPEED_FREQ_HIGH
				#endif
		
				#ifndef LL_DMA_PRIORITY_VERYHIGH
					#define LL_DMA_PRIORITY_VERYHIGH				LL_DMA_PRIORITY_HIGH
				#endif
				
				//===定义端口无上下拉，F1系列只有上拉和下拉，这是为了兼容其他系列，直接定义使用上拉替代
				#ifndef LL_GPIO_PULL_NO
					#define LL_GPIO_PULL_NO				LL_GPIO_PULL_UP
				#endif
		
			#elif defined(USE_MCU_STM32F2)
				#include "stm32f2xx_ll.h"
			#elif defined(USE_MCU_STM32F3)
				#include "stm32f3xx_ll.h"
			#elif defined(USE_MCU_STM32F4)
				#include "stm32f4xx_ll.h"
			#else
				#error "STM32的LL库导入错误!"
			#endif
		#endif
		
		//===Hal库
		#if defined (USE_HAL_DRIVER)
			#ifdef USE_MCU_STM32F0
				#include "stm32f0xx_hal.h"
			#elif defined(USE_MCU_STM32F1)
				#include "stm32f1xx_hal.h"
			#elif defined(USE_MCU_STM32F2)
				#include "stm32f2xx_hal.h"
			#elif defined(USE_MCU_STM32F3)
				#include "stm32f3xx_hal.h"
			#elif defined(USE_MCU_STM32F4)
				#include "stm32f4xx_hal.h"
			#else
				#error "STM32的Hal库导入错误!"
			#endif
		#endif
	#else
		#error "STM32库文件导入错误!"
	#endif

	//===定时时钟的配置
	#define PERIPHERAL_CLOCK_DISABLE			0													//---不使能外设时钟
	#define PERIPHERAL_CLOCK_ENABLE				1													//---是能外设时钟
	#define PERIPHERAL_CLOCK_FORCE_RESET		2													//---复位外设时钟
	#define PERIPHERAL_CLOCK_RELEASE_RESET		3													//---释放复位的外设时钟
	
	//===定义使用的调试方式
	#define USE_DEBUG_SWD
	//===结构体定义
	typedef struct _HardFault_HandlerType		HardFault_HandlerType;
	//===指针结构体定义
	typedef struct _HardFault_HandlerType		*pHardFault_HandlerType;
	//===结构体定义
	struct _HardFault_HandlerType
	{
		unsigned  int	stacked_r0;
		unsigned  int	stacked_r1;
		unsigned  int	stacked_r2;
		unsigned  int	stacked_r3;
		unsigned  int	stacked_r12;
		unsigned  int	stacked_lr;
		unsigned  int	stacked_pc;
		unsigned  int	stacked_psr;
	};
	
	//===外部调用接口
	extern HardFault_HandlerType	g_HardFaultErrMsg;
	extern pHardFault_HandlerType	pHardFaultErrMsg;

	//===独立看门狗文件
	#include "iwdg_task.h"	
	//===获取系统时钟的定义
	#define	SYS_CLOCK_HZ						( SystemCoreClock )
	#define	SYS_CLOCK_KHZ						( (SYS_CLOCK_HZ/1000U) )
	#define SYS_CLOCK_MHZ						( (SYS_CLOCK_HZ/1000000U) )
	#define SYS_CLOCK_ADD						( SYS_CLOCK_MHZ-20 )
	#define FLASH_PROG_WAIT						( 1000UL*SYS_CLOCK_MHZ )
	
	//===使能全局中断
	#define SEI()								_irq_enable()	
	//===禁止全局中断
	#define CLI()								_irq_disable()	
	//===喂狗
	#define WDT_RESET()							IWDGTask_Reset();	
	//===软件复位
	#define SOFT_RESET()						( CLI(),NVIC_SystemReset() )	
	//===错误处理
	#define Error_Handler()						_Error_Handler(__FILE__, __LINE__)	
	//===函数定义
	void _irq_enable(void);
	void _irq_disable(void);
	void _Error_Handler(char * file, int line);
	void CrisEnter(void);
	void CrisExit(void);
	#ifdef USE_MCU_STM32F1
		void LL_FLASH_EnableDataCache(void);
		void LL_FLASH_DisableDataCache(void);
		void LL_FLASH_EnableInstCache(void);
		void LL_FLASH_DisableInstCache(void);
	#endif
	void LL_FLASH_Unlock(void);
	void LL_FLASH_Lock(void);
	void LL_FLASH_OB_Unlock(void);
	void LL_FLASH_OB_Lock(void);
	UINT8_T LL_FLASH_GetBank1State(void);
	UINT8_T LL_FLASH_GetBank2State(void);
	void LL_FLASH_ClearFlag(void);
	UINT32_T LL_FLASH_OB_GetWRP(void);
	UINT32_T LL_FLASH_OB_GetRDP(void);
	UINT8_T LL_FLASH_OB_GetUser(void);
	UINT8_T LL_FLASH_OB_RDP_LevelConfig(UINT8_T rdpLevel);
	UINT8_T LL_FLASH_WaitBank1Operation(UINT32_T timeout);
	UINT8_T LL_FLASH_WaitBank2Operation(UINT32_T timeout);
	UINT8_T LL_FLASH_ErasePage(UINT32_T pageAddr);
	UINT8_T LL_FLASH_EraseAllPage(void);
	UINT8_T LL_FLASH_WriteHalfWord(UINT32_T pageAddr, UINT16_T val);
	UINT16_T LL_FLASH_ReadHalfWord(UINT32_T pageAddr);
	UINT32_T LL_FLASH_ReadFullWord(UINT32_T pageAddr);
	void LL_FLASH_SetProgramSize(UINT32_T psize);
	void LL_GetUID(UINT32_T *uid);
	void LL_NVIC_DeInit(void);
	void LL_TIM_ClearFlag(TIM_TypeDef *TIMx);
	UINT32_T LL_TIM_GetFlag(TIM_TypeDef *TIMx);
	void LL_USART_ClearFlag(USART_TypeDef *USARTx);
	UINT32_T LL_USART_GetFlag(USART_TypeDef *USARTx);
	void HardFault_IRQTask( void );
	void LL_GPIO_SetPinsMode(GPIO_TypeDef* GPIOx, UINT32_T Pin, UINT32_T Mode);
	void LL_GPIO_SetOutputPins(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	void LL_GPIO_ResetOutputPins(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	void LL_GPIO_TogglePins(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	void LL_DMA_ClearFlag_HT(DMA_TypeDef* DMAx, UINT32_T channelOrStream);
	void LL_DMA_ClearFlag_TC(DMA_TypeDef* DMAx, UINT32_T channelOrStream);
	void LL_DMA_ClearFlag_TE(DMA_TypeDef* DMAx, UINT32_T channelOrStream);
	void LL_DMA_ClearFlag_DME(DMA_TypeDef* DMAx, UINT32_T channelOrStream);
	void LL_DMA_ClearFlag_FE(DMA_TypeDef* DMAx, UINT32_T channelOrStream);
	void LL_DMA_ClearFlag(DMA_TypeDef* DMAx, UINT32_T channelOrStream);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* STM32_CFG_H_ */