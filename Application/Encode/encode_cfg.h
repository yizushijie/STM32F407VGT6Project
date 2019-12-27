#ifndef ENCODE_CFG_H_
#define ENCODE_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include"complier_lib.h"
	#include "gpio_task.h"
	#include"timer_task.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===编码器使用的定时器
	#define ENCODE_USE_TIMER							TIM2
	//===编码器定时器中断
	#define ENCODE_USE_TIMER_IRQ						TIM2_IRQn
	
	//===CH1的端口定义---PA0
	#define ENCODE_CH1_PORT								GPIOA
	#define ENCODE_CH1_BIT								LL_GPIO_PIN_0
	#define ENCODE_CH1_WRITE							GPIO_SET_WRITE(	ENCODE_CH1_PORT,ENCODE_CH1_BIT)
	#define ENCODE_CH1_READ								GPIO_SET_READ(	ENCODE_CH1_PORT,ENCODE_CH1_BIT)
	#define ENCODE_CH1_STATE							GPIO_GET_STATE(	ENCODE_CH1_PORT,ENCODE_CH1_BIT)
	#define ENCODE_CH1_OUT_0							GPIO_OUT_0(		ENCODE_CH1_PORT,ENCODE_CH1_BIT)
	#define ENCODE_CH1_OUT_1							GPIO_OUT_1(		ENCODE_CH1_PORT,ENCODE_CH1_BIT)
	#define ENCODE_CH1_OUT_C							GPIO_OUT_C(		ENCODE_CH1_PORT,ENCODE_CH1_BIT)

	//===CH2的端口定义---PA1
	#define ENCODE_CH2_PORT								GPIOA
	#define ENCODE_CH2_BIT								LL_GPIO_PIN_1
	#define ENCODE_CH2_WRITE							GPIO_SET_WRITE(	ENCODE_CH2_PORT,ENCODE_CH2_BIT)
	#define ENCODE_CH2_READ								GPIO_SET_READ(	ENCODE_CH2_PORT,ENCODE_CH2_BIT)
	#define ENCODE_CH2_STATE							GPIO_GET_STATE(	ENCODE_CH2_PORT,ENCODE_CH2_BIT)
	#define ENCODE_CH2_OUT_0							GPIO_OUT_0(		ENCODE_CH2_PORT,ENCODE_CH2_BIT)
	#define ENCODE_CH2_OUT_1							GPIO_OUT_1(		ENCODE_CH2_PORT,ENCODE_CH2_BIT)
	#define ENCODE_CH2_OUT_C							GPIO_OUT_C(		ENCODE_CH2_PORT,ENCODE_CH2_BIT)

	//===CH3的端口定义---PA2
	#define ENCODE_CH3_PORT								GPIOA
	#define ENCODE_CH3_BIT								LL_GPIO_PIN_2
	#define ENCODE_CH3_WRITE							GPIO_SET_WRITE(	ENCODE_CH3_PORT,ENCODE_CH3_BIT)
	#define ENCODE_CH3_READ								GPIO_SET_READ(	ENCODE_CH3_PORT,ENCODE_CH3_BIT)
	#define ENCODE_CH3_STATE							GPIO_GET_STATE(	ENCODE_CH3_PORT,ENCODE_CH3_BIT)
	#define ENCODE_CH3_OUT_0							GPIO_OUT_0(		ENCODE_CH3_PORT,ENCODE_CH3_BIT)
	#define ENCODE_CH3_OUT_1							GPIO_OUT_1(		ENCODE_CH3_PORT,ENCODE_CH3_BIT)
	#define ENCODE_CH3_OUT_C							GPIO_OUT_C(		ENCODE_CH3_PORT,ENCODE_CH3_BIT)

	//===CH4的端口定义---PA3
	#define ENCODE_CH4_PORT								GPIOA
	#define ENCODE_CH4_BIT								LL_GPIO_PIN_3
	#define ENCODE_CH4_WRITE							GPIO_SET_WRITE(	ENCODE_CH4_PORT,ENCODE_CH4_BIT)
	#define ENCODE_CH4_READ								GPIO_SET_READ(	ENCODE_CH4_PORT,ENCODE_CH4_BIT)
	#define ENCODE_CH4_STATE							GPIO_GET_STATE(	ENCODE_CH4_PORT,ENCODE_CH4_BIT)
	#define ENCODE_CH4_OUT_0							GPIO_OUT_0(		ENCODE_CH4_PORT,ENCODE_CH4_BIT)
	#define ENCODE_CH4_OUT_1							GPIO_OUT_1(		ENCODE_CH4_PORT,ENCODE_CH4_BIT)
	#define ENCODE_CH4_OUT_C							GPIO_OUT_C(		ENCODE_CH4_PORT,ENCODE_CH4_BIT)

	//===编码器端口总线
	#define ENCODE_BUS_PORT								GPIOA
	#define ENCODE_BUS_BITS								( LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_1|LL_GPIO_PIN_0 )
	#define ENCODE_BUS_MASK								0xFFF0
	#define ENCODE_BUS_OUT(val)							( ENCODE_BUS_PORT->ODR=(ENCODE_BUS_PORT->ODR&ENCODE_BUS_MASK)|(val&ENCODE_BUS_BITS) )
	
	//===定义结构体
	typedef struct _EnCode_HandlerType		EnCode_HandlerType;

	//===定义结构体指针
	typedef struct _EnCode_HandlerType		*pEnCode_HandlerType;

	//===定义编码器的编码结构体
	struct _EnCode_HandlerType
	{
		UINT8_T codeLength;								// #1 仿真PT2262的有效编码码长
		UINT8_T codeMode;								// 编码模式 0：2262编码,1:1KHz方波;2:500Hz方波
		UINT8_T codeVal[20];							// 编码数组，只对2262有效
	};

	//===定义编码的长度
	#define GEN_CODE_FRAME_LENGTH						16		
	//===编码总帧数,最大值为7
	#define GEN_CODE_FRAME_COUNT						3
	//===计算数据总字节数数
	#define	GEN_CODE_BYTE_OF_CYCLE_COUNT				( GEN_CODE_FRAME_COUNT*GEN_CODE_FRAME_LENGTH*8 )	

	//===编码器通道1	---433M		小信号
	#define ENCODE_CH1_INDEX							0x01
	//===编码器通道2	---4333M	大信号
	#define ENCODE_CH2_INDEX							0x02
	//===编码器通道3	---315M		信号
	#define ENCODE_CH3_INDEX							0x04
	//===编码器通道4	---2xxM		信号	
	#define ENCODE_CH4_INDEX							0x08

	//===编码器的编码时基
	#define ENCODE_TIMER_BASE_US						500
	//===定时器的分频数，72MHz时钟，每500us溢出中断一次，
	#define ENCODE_TIMER_ARR							( SYS_CLOCK_MHZ-1 )
	//===定时器的重新装载值
	#define ENCODE_TIMER_RELOAD							( 500-1 )
	
	//===编码命令
	typedef enum
	{
		CMD_ENCODE_NONE				= 0,
		CMD_ENCODE_CHA_OPEN			= 1,				//打开RF信号源通道1
		CMD_ENCODE_CHA_CLOSE		= 2,				//关闭RF信号源通道1
		CMD_ENCODE_CHB_OPEN			= 3,				//打开RF信号源通道2
		CMD_ENCODE_CHB_CLOSE		= 4,				//关闭RF信号源通道2
		CMD_ENCODE_CHC_OPEN			= 5,				//打开RF信号源通道3
		CMD_ENCODE_CHC_CLOSE		= 6,				//关闭RF信号源通道3
		CMD_ENCODE_CHD_OPEN			= 7,				//打开RF信号源通道4
		CMD_ENCODE_CHD_CLOSE		= 8,				//关闭RF信号源通道5
		CMD_DECODE_RST				= 9,				//复位解码的状态
		CMD_DECODE_READ				= 10,				//读取解码的状态
		CMD_ENCODE_CHA_HIGH			= 11,				//信号源通道输出高
		CMD_ENCODE_CHB_HIGH			= 12,				//信号源通道输出高
		CMD_ENCODE_CHC_HIGH			= 13,				//信号源通道输出高
		CMD_ENCODE_CHD_HIGH			= 14,				//信号源通道输出高

	}RF_CMD_CODE;

	//===编码信号源控制主命令
	#define CMD_ENCODE				0xA1

	//===外部调用接口
	extern VLTUINT8_T g_GenCodeChannelMask1;
	extern VLTUINT8_T g_GenCodeChannelMask2;
	//===函数定义
	void Encode_Init(void);
	void EnCode_IRQTask( void );
	void EnCode_SetChannel(UINT8_T chIndex, UINT8_T isEnCode, UINT8_T isHigh);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ENCODE_CFG_H_ */