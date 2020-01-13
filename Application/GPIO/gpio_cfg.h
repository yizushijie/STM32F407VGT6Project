#ifndef GPIO_CFG_H_
#define GPIO_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===使用包含的头文件
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	//////////////位带操作,实现51类似的GPIO控制功能////////////////////////////////////////
	//////////////具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).///////////////////////
	//////////////IO口操作宏定义/////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	#define BIT_BAND(addr, bitIndex)	((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitIndex<<2)) 
	#define MEM_ADDR(addr)				*((volatile unsigned long  *)(addr)) 
	#define BIT_ADDR(addr, bitIndex)	MEM_ADDR(BIT_BAND(addr, bitIndex)) 
	
	#ifdef USE_MCU_STM32F1
		//====IO输出的偏移
		#define	GPIO_ODR_OFFSET				0x0C
		//====IO输如的偏移
		#define	GPIO_IDR_OFFSET				0x08
	#elif defined (USE_MCU_STM32F4)
		//====IO输出的偏移
		#define	GPIO_ODR_OFFSET				0x14
		//====IO输如的偏移
		#define	GPIO_IDR_OFFSET				0x10
	#else
		#error "IO端口的输入输出基地址偏移设置错误！"
	#endif

	//===IO口输出地址映射
	#define GPIOA_ODR_ADDR				(GPIOA_BASE+GPIO_ODR_OFFSET) //0x4001080C 
	#define GPIOB_ODR_ADDR				(GPIOB_BASE+GPIO_ODR_OFFSET) //0x40010C0C 
	#define GPIOC_ODR_ADDR				(GPIOC_BASE+GPIO_ODR_OFFSET) //0x4001100C 
	#define GPIOD_ODR_ADDR			    (GPIOD_BASE+GPIO_ODR_OFFSET) //0x4001140C 
	#define GPIOE_ODR_ADDR			    (GPIOE_BASE+GPIO_ODR_OFFSET) //0x4001180C 
	#define GPIOF_ODR_ADDR			    (GPIOF_BASE+GPIO_ODR_OFFSET) //0x40011A0C    
	#define GPIOG_ODR_ADDR			    (GPIOG_BASE+GPIO_ODR_OFFSET) //0x40011E0C    
	
	//===IO口输入地址映射
	#define GPIOA_IDR_ADDR			    (GPIOA_BASE+GPIO_IDR_OFFSET) //0x40010808 
	#define GPIOB_IDR_ADDR			    (GPIOB_BASE+GPIO_IDR_OFFSET) //0x40010C08 
	#define GPIOC_IDR_ADDR			    (GPIOC_BASE+GPIO_IDR_OFFSET) //0x40011008 
	#define GPIOD_IDR_ADDR			    (GPIOD_BASE+GPIO_IDR_OFFSET) //0x40011408 
	#define GPIOE_IDR_ADDR			    (GPIOE_BASE+GPIO_IDR_OFFSET) //0x40011808 
	#define GPIOF_IDR_ADDR			    (GPIOF_BASE+GPIO_IDR_OFFSET) //0x40011A08 
	#define GPIOG_IDR_ADDR			    (GPIOG_BASE+GPIO_IDR_OFFSET) //0x40011E08 
	
	//===IO口操作,只对单一的IO口!
	#ifdef GPIOA
		//===输出
		#define PA_OUT(n)				BIT_ADDR(GPIOA_ODR_ADDR,n)
		//===输入
		#define PA_PIN(n)				BIT_ADDR(GPIOA_IDR_ADDR,n)  
	#endif
	
	#ifdef GPIOB
		//===输出
		#define PB_OUT(n)				BIT_ADDR(GPIOB_ODR_ADDR,n)
		//===输入
		#define PB_PIN(n)				BIT_ADDR(GPIOB_IDR_ADDR,n)  
	#endif
	
	#ifdef GPIOC
		//===输出
		#define PC_OUT(n)				BIT_ADDR(GPIOC_ODR_ADDR,n)
		//===输入
		#define PC_PIN(n)				BIT_ADDR(GPIOC_IDR_ADDR,n)  
	#endif

	#ifdef GPIOD
		//===输出
		#define PD_OUT(n)				BIT_ADDR(GPIOD_ODR_ADDR,n)
		//===输入
		#define PD_PIN(n)				BIT_ADDR(GPIOD_IDR_ADDR,n)  
	#endif

	#ifdef GPIOE
		//===输出
		#define PE_OUT(n)				BIT_ADDR(GPIOE_ODR_ADDR,n)
		//===输入
		#define PE_PIN(n)				BIT_ADDR(GPIOE_IDR_ADDR,n)  
	#endif

	#ifdef GPIOF
		//===输出
		#define PF_OUT(n)				BIT_ADDR(GPIOF_ODR_ADDR,n)
		//===输入
		#define PF_PIN(n)				BIT_ADDR(GPIOF_IDR_ADDR,n)  
	#endif

	#ifdef GPIOG
		//===输出
		#define PG_OUT(n)				BIT_ADDR(GPIOG_ODR_ADDR,n)
		//===输入
		#define PG_PIN(n)				BIT_ADDR(GPIOG_IDR_ADDR,n)  
	#endif

	#ifdef GPIOH
		//===输出
		#define PH_OUT(n)				BIT_ADDR(GPIOH_ODR_Addr,n)
		//===输入
		#define PH_PIN(n)				BIT_ADDR(GPIOH_IDR_Addr,n)  
	#endif
	
	/////////////////////////////////////////////////////////////////////////////////////
	//===设置端口为输出模式
	#define GPIO_SET_WRITE(	name, index )			GPIO_SetPinMode( name, index,LL_GPIO_MODE_OUTPUT)	
	//===设置端口为输入模式
	#define GPIO_SET_READ(	name, index )			GPIO_SetPinMode( name, index,LL_GPIO_MODE_INPUT)	
	//===设置端口输出高
	#define GPIO_OUT_1(	name, index )				GPIO_SetOutputPin( name, index )	
	//===设置端口输出低
	#define GPIO_OUT_0(	name, index )				GPIO_ResetOutputPins( name, index )	
	//===设置端口输出取反
	#define GPIO_OUT_C(	name, index )				GPIO_TogglePin( name, index )	
	//===获取端口的输入状态,0---低电平，1---高电平
	#define GPIO_GET_STATE(	name, index )			GPIO_ReadInputPort( name, index )//((LL_GPIO_ReadInputPort(name)&index ))
	//===GPIO低八位的数据
	#define GPIO_L8BITS_OUT( name, val	)			( name->ODR=(name->ODR&0xFF00)| (val&0x00FF) )	
	//===GPIO高八位的数据
	#define GPIO_H8BITS_OUT( name, val	)			( name->ODR=(name->ODR&0x00FF)| ((val<<8)&0xFF00) )	
	//===GPIO16位的数据
	#define GPIO_16BITS_OUT( name, val	)			( name->ODR=(val&0xFFFF) )
	
	//===定义结构体
	typedef struct _GPIO_HandlerType				GPIO_HandlerType;
	//===定义指针结构体
	typedef	struct _GPIO_HandlerType				*pGPIO_HandlerType;
	//===结构定义
	struct _GPIO_HandlerType
	{
		GPIO_TypeDef	*msgPort;																																					//---端口
		UINT32_T		msgBit;																																						//---序号
	};

	//===函数定义
	UINT8_T GPIO_Clock(GPIO_TypeDef *GPIOx, UINT8_T isEnable);
	void GPIO_Init(void);
	void GPIO_SetPinMode(GPIO_TypeDef* GPIOx, UINT32_T Pin, UINT32_T Mode);
	void GPIO_SetOutputPin(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	void GPIO_ResetOutputPins(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	void GPIO_TogglePin(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	UINT32_T GPIO_ReadInputPort(GPIO_TypeDef* GPIOx, UINT32_T PinMask);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* GPIO_CFG_H_ */