	#ifndef HVPP_CFG_H_
#define HVPP_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "power_task.h"
	#include "hw_cfg.h"
	#include "delay_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//=============HVPP使用的端口和上电初始化的状态========================================
	//===HVPP_RDY_BSY	---设置为输入，在整个高压过程都是输入
	//===HVPP_OE		---设置为输出，默认是高电平，该引脚低电平有效
	//===HVPP_WR		---设置为输出，默认是高电平，该引脚低电平有效
	//===HVPP_BS1		---设置为输出，默认是低电平，默认选择低字节
	//===HVPP_XA0		---设置为输出，默认是高电平，和XA1的高电平组合为空闲模式
	//===HVPP_XA1		---设置为输出，默认是高电平，和XA0的高电平组合为空闲模式
	//===HVPP_PAGEL		---设置为输出，默认是低电平，默认选择低字节
	//===HVPP_BS2		---设置为输出，默认是低电平，默认选择低字节
	//===HVPP_XTAL		---设置为输出，默认是低电平，正脉冲信号，会执行动作
	//===HVPP_DATA		---设置为输入，默认需要读取外部数据
	//===================================================================================
	//===O---编程忙和1---等待新命令，device--->>>host---O
	#define HVPP_RDY_BSY_PORT								GPIOC
	#define HVPP_RDY_BSY_BIT								LL_GPIO_PIN_15
	#define HVPP_RDY_BSY_STATE								GPIO_GET_STATE(HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_WRITE								GPIO_SET_WRITE(HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_READ								GPIO_SET_READ( HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_0								GPIO_OUT_0(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_1								GPIO_OUT_1(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_C								GPIO_OUT_C(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)

	//===输出使能，低电平有效；device--->>>host---I
	#define HVPP_OE_PORT									GPIOE
	#define HVPP_OE_BIT										LL_GPIO_PIN_9
	#define HVPP_OE_STATE									GPIO_GET_STATE(HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_WRITE									GPIO_SET_WRITE(HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_READ									GPIO_SET_READ( HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_0									GPIO_OUT_0(    HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_1									GPIO_OUT_1(    HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_C									GPIO_OUT_C(    HVPP_OE_PORT,HVPP_OE_BIT)

	//===写脉冲，低电平有效；device--->>>host---I
	#define HVPP_WR_PORT									GPIOE
	#define HVPP_WR_BIT										LL_GPIO_PIN_10
	#define HVPP_WR_STATE									GPIO_GET_STATE(HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_WRITE									GPIO_SET_WRITE(HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_READ									GPIO_SET_READ( HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_0									GPIO_OUT_0(    HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_1									GPIO_OUT_1(    HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_C									GPIO_OUT_C(    HVPP_WR_PORT,HVPP_WR_BIT)

	//===字节选择1(0---选择低字节，1---选择高字节)；device--->>>host---I
	#define HVPP_BS1_PORT									GPIOE
	#define HVPP_BS1_BIT									LL_GPIO_PIN_11
	#define HVPP_BS1_STATE									GPIO_GET_STATE(HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_WRITE									GPIO_SET_WRITE(HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_READ									GPIO_SET_READ( HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_0									GPIO_OUT_0(    HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_1									GPIO_OUT_1(    HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_C									GPIO_OUT_C(    HVPP_BS1_PORT,HVPP_BS1_BIT)

	//===XTAL动作0；device--->>>host---I
	#define HVPP_XA0_PORT									GPIOE
	#define HVPP_XA0_BIT									LL_GPIO_PIN_12
	#define HVPP_XA0_STATE									GPIO_GET_STATE(HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_WRITE									GPIO_SET_WRITE(HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_READ									GPIO_SET_READ( HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_0									GPIO_OUT_0(    HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_1									GPIO_OUT_1(    HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_C									GPIO_OUT_C(    HVPP_XA0_PORT,HVPP_XA0_BIT)

	//===XTAL动作1；device--->>>host---I
	#define HVPP_XA1_PORT									GPIOE
	#define HVPP_XA1_BIT									LL_GPIO_PIN_13
	#define HVPP_XA1_STATE									GPIO_GET_STATE(HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_WRITE									GPIO_SET_WRITE(HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_READ									GPIO_SET_READ( HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_0									GPIO_OUT_0(    HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_1									GPIO_OUT_1(    HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_C									GPIO_OUT_C(    HVPP_XA1_PORT,HVPP_XA1_BIT)

	//===加载程序存储器和EEPROM数据页；device--->>>host---I
	#define HVPP_PAGEL_PORT									GPIOE
	#define HVPP_PAGEL_BIT									LL_GPIO_PIN_14
	#define HVPP_PAGEL_STATE								GPIO_GET_STATE(HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_WRITE								GPIO_SET_WRITE(HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_READ									GPIO_SET_READ( HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_0								GPIO_OUT_0(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_1								GPIO_OUT_1(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_C								GPIO_OUT_C(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)

	//===字节选择2(0---选择低字节，1---选择第二个高字节)；device--->>>host---I
	#define HVPP_BS2_PORT									GPIOE
	#define HVPP_BS2_BIT									LL_GPIO_PIN_8
	#define HVPP_BS2_STATE									GPIO_GET_STATE(HVPP_BS2_PORT,HVPP_BS2_BIT)
	#define HVPP_BS2_WRITE									GPIO_SET_WRITE(HVPP_BS2_PORT,HVPP_BS2_BIT)
	#define HVPP_BS2_READ									GPIO_SET_READ( HVPP_BS2_PORT,HVPP_BS2_BIT)
	#define HVPP_BS2_OUT_0									GPIO_OUT_0(    HVPP_BS2_PORT,HVPP_BS2_BIT)
	#define HVPP_BS2_OUT_1									GPIO_OUT_1(    HVPP_BS2_PORT,HVPP_BS2_BIT)
	#define HVPP_BS2_OUT_C									GPIO_OUT_C(    HVPP_BS2_PORT,HVPP_BS2_BIT)
															
	//===时钟引脚；device--->>>host---I						
	#define HVPP_XTAL_PORT									GPIOE
	#define HVPP_XTAL_BIT									LL_GPIO_PIN_15
	#define HVPP_XTAL_STATE									GPIO_GET_STATE(HVPP_XTAL_PORT,HVPP_XTAL_BIT)
	#define HVPP_XTAL_WRITE									GPIO_SET_WRITE(HVPP_XTAL_PORT,HVPP_XTAL_BIT)
	#define HVPP_XTAL_READ									GPIO_SET_READ( HVPP_XTAL_PORT,HVPP_XTAL_BIT)
	#define HVPP_XTAL_OUT_0									GPIO_OUT_0(    HVPP_XTAL_PORT,HVPP_XTAL_BIT)
	#define HVPP_XTAL_OUT_1									GPIO_OUT_1(    HVPP_XTAL_PORT,HVPP_XTAL_BIT)
	#define HVPP_XTAL_OUT_C									GPIO_OUT_C(    HVPP_XTAL_PORT,HVPP_XTAL_BIT)

	#define HVPP_CTRL_BUS_PORT								GPIOE
	#define HVPP_CTRL_BUS_BIT								(LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15)
	#define HVPP_CTRL_BUS_STATE								GPIO_GET_STATE(HVPP_CTRL_BUS_PORT,HVPP_CTRL_BUS_BIT)
	#define HVPP_CTRL_BUS_WRITE								GPIO_SET_WRITE(HVPP_CTRL_BUS_PORT,HVPP_CTRL_BUS_BIT)
	#define HVPP_CTRL_BUS_READ								GPIO_SET_READ( HVPP_CTRL_BUS_PORT,HVPP_CTRL_BUS_BIT)
	#define HVPP_CTRL_BUS_VAL(val)							GPIO_H8BITS_OUT(HVPP_CTRL_BUS_PORT,val)
															
	//===定义控制使能信号							 `			
	#define HVPP_CTRL_OE_PORT								GPIOD
	#define HVPP_CTRL_OE_BIT								LL_GPIO_PIN_4
	#define HVPP_CTRL_OE_STATE								GPIO_GET_STATE(HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_WRITE								GPIO_SET_WRITE(HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_READ								GPIO_SET_READ( HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_0								GPIO_OUT_0(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_1								GPIO_OUT_1(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_C								GPIO_OUT_C(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
															
	//===电平转换芯片的数据流向					   `		
	#define HVPP_CTRL_DIR_TO_DEVICE							HVPP_CTRL_OE_OUT_0					
	#define HVPP_CTRL_DIR_TO_HZ								HVPP_CTRL_OE_OUT_1

	//===双向数据总线Bit0(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA0_PORT									GPIOE
	#define HVPP_DATA0_BIT									LL_GPIO_PIN_7
	#define HVPP_DATA0_STATE								GPIO_GET_STATE(HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_WRITE								GPIO_SET_WRITE(HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_READ									GPIO_SET_READ( HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_0								GPIO_OUT_0(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_1								GPIO_OUT_1(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_C								GPIO_OUT_C(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)

	//===双向数据总线Bit1(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA1_PORT									GPIOE
	#define HVPP_DATA1_BIT									LL_GPIO_PIN_6
	#define HVPP_DATA1_STATE								GPIO_GET_STATE(HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_WRITE								GPIO_SET_WRITE(HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_READ									GPIO_SET_READ( HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_0								GPIO_OUT_0(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_1								GPIO_OUT_1(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_C								GPIO_OUT_C(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)

	//===双向数据总线Bit2(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA2_PORT									GPIOE
	#define HVPP_DATA2_BIT									LL_GPIO_PIN_5
	#define HVPP_DATA2_STATE								GPIO_GET_STATE(HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_WRITE								GPIO_SET_WRITE(HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_READ									GPIO_SET_READ( HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_0								GPIO_OUT_0(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_1								GPIO_OUT_1(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_C								GPIO_OUT_C(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)

	//===双向数据总线Bit3(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA3_PORT									GPIOE
	#define HVPP_DATA3_BIT									LL_GPIO_PIN_4
	#define HVPP_DATA3_STATE								GPIO_GET_STATE(HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_WRITE								GPIO_SET_WRITE(HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_READ									GPIO_SET_READ( HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_0								GPIO_OUT_0(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_1								GPIO_OUT_1(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_C								GPIO_OUT_C(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)

	//===双向数据总线Bit4(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA4_PORT									GPIOE
	#define HVPP_DATA4_BIT									LL_GPIO_PIN_3
	#define HVPP_DATA4_STATE								GPIO_GET_STATE(HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_WRITE								GPIO_SET_WRITE(HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_READ									GPIO_SET_READ( HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_0								GPIO_OUT_0(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_1								GPIO_OUT_1(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_C								GPIO_OUT_C(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)

	//===双向数据总线Bit5(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA5_PORT									GPIOE
	#define HVPP_DATA5_BIT									LL_GPIO_PIN_2
	#define HVPP_DATA5_STATE								GPIO_GET_STATE(HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_WRITE								GPIO_SET_WRITE(HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_READ									GPIO_SET_READ( HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_0								GPIO_OUT_0(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_1								GPIO_OUT_1(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_C								GPIO_OUT_C(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)

	//===双向数据总线Bit6(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA6_PORT									GPIOE
	#define HVPP_DATA6_BIT									LL_GPIO_PIN_1
	#define HVPP_DATA6_STATE								GPIO_GET_STATE(HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_WRITE								GPIO_SET_WRITE(HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_READ									GPIO_SET_READ( HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_0								GPIO_OUT_0(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_1								GPIO_OUT_1(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_C								GPIO_OUT_C(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)

	//===双向数据总线Bit7(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA7_PORT									GPIOE
	#define HVPP_DATA7_BIT									LL_GPIO_PIN_0
	#define HVPP_DATA7_STATE								GPIO_GET_STATE(HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_WRITE								GPIO_SET_WRITE(HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_READ									GPIO_SET_READ( HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_0								GPIO_OUT_0(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_1								GPIO_OUT_1(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_C								GPIO_OUT_C(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)

	//===双向数据总线(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA_BUS_PORT								GPIOE
	#define HVPP_DATA_BUS_BIT								(LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)
	#define HVPP_DATA_BUS_STATE								GPIO_GET_STATE(HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_WRITE								GPIO_SET_WRITE(HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_READ								GPIO_SET_READ( HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_VAL(val)							GPIO_L8BITS_OUT(HVPP_DATA_BUS_PORT,val)	

	//===定义数据线使能信号
	#define HVPP_DATA_OE_PORT								GPIOB
	#define HVPP_DATA_OE_BIT								LL_GPIO_PIN_15
	#define HVPP_DATA_OE_STATE								GPIO_GET_STATE(HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_WRITE								GPIO_SET_WRITE(HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_READ								GPIO_SET_READ( HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_0								GPIO_OUT_0(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_1								GPIO_OUT_1(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_C								GPIO_OUT_C(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)

	//===定义数据线使能方向信号
	#define HVPP_DATA_DIR_PORT								GPIOB
	#define HVPP_DATA_DIR_BIT								LL_GPIO_PIN_14
	#define HVPP_DATA_DIR_STATE								GPIO_GET_STATE(HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_WRITE								GPIO_SET_WRITE(HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_READ								GPIO_SET_READ( HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_0								GPIO_OUT_0(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_1								GPIO_OUT_1(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_C								GPIO_OUT_C(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)

	//===电平转换芯片的数据流向
	#define HVPP_DATA_BUS_DIR_TO_DEVICE						(HVPP_DATA_DIR_OUT_1,HVPP_DATA_OE_OUT_0)
	#define HVPP_DATA_BUS_DIR_TO_HOST						(HVPP_DATA_DIR_OUT_0,HVPP_DATA_OE_OUT_0)
	#define HVPP_DATA_BUS_DIR_TO_HZ							HVPP_DATA_OE_OUT_1
	
	//>>>===XA0,XA1组合编码功能---开始			
	#define	HVPP_XA_OUT_00									( HVPP_XA1_OUT_0,HVPP_XA0_OUT_0 )																						//---加载存储器地址
	#define	HVPP_XA_OUT_01									( HVPP_XA1_OUT_0,HVPP_XA0_OUT_1 )																						//---加载数据
	#define	HVPP_XA_OUT_10									( HVPP_XA1_OUT_1,HVPP_XA0_OUT_0 )																						//---加载命令
	#define	HVPP_XA_OUT_11									( HVPP_XA1_OUT_1,HVPP_XA0_OUT_1 )																						//---空闲模式
	#define HVPP_XA_LOAD_ADDR								HVPP_XA_OUT_00
	#define HVPP_XA_LOAD_DATA								HVPP_XA_OUT_01
	#define HVPP_XA_LOAD_CMD								HVPP_XA_OUT_10
	#define HVPP_XA_LOAD_IDLE								HVPP_XA_OUT_11
	//<<<===XA0,XA1组合编码功能---结束

	//>>>===BS2,BS1组合编码功能---开始			
	#define	HVPP_BS_OUT_00									( HVPP_BS2_OUT_0,HVPP_BS1_OUT_0 )																						//---
	#define	HVPP_BS_OUT_01									( HVPP_BS2_OUT_0,HVPP_BS1_OUT_1 )																						//---
	#define	HVPP_BS_OUT_10									( HVPP_BS2_OUT_1,HVPP_BS1_OUT_0 )																						//---
	#define	HVPP_BS_OUT_11									( HVPP_BS2_OUT_1,HVPP_BS1_OUT_0 )																						//---
	#define	HVPP_BS_SELECT_ADDR_LOW							HVPP_BS_OUT_00																											//---选择低位地址
	#define	HVPP_BS_SELECT_ADDR_HIGH						HVPP_BS_OUT_01																											//---选择高位地址
	#define	HVPP_BS_SELECT_ADDR_EXTERN						HVPP_BS_OUT_10																											//---选择拓展位地址
	#define	HVPP_BS_SELECT_DATA_LOW							HVPP_BS_OUT_00																											//---选择低位数据
	#define	HVPP_BS_SELECT_DATA_HIGH						HVPP_BS_OUT_01																											//---选择高位数据
	//<<<===BS2,BS1组合编码功能---结束

	//>>>===命令和地址加载的编码---开始
	#define HVPP_ENABLE_CMD_LOAD							( HVPP_XA_LOAD_CMD,HVPP_BS_OUT_00 )																						//---命令加载
	#define HVPP_ENABLE_ADDR_LOAD_LOW						( HVPP_XA_LOAD_ADDR,HVPP_BS_SELECT_ADDR_LOW )																						//---低位地址加载
	#define HVPP_ENABLE_ADDR_LOAD_HIGH						( HVPP_XA_LOAD_ADDR,HVPP_BS_SELECT_ADDR_HIGH )																						//---高位地址加载														
	#define HVPP_ENABLE_ADDR_LOAD_EXTERN					( HVPP_XA_LOAD_ADDR,HVPP_BS_SELECT_ADDR_EXTERN )																						//---拓展位地址加载	
	#define HVPP_ENABLE_DATA_LOAD_LOW						( HVPP_BS_SELECT_DATA_LOW,HVPP_XA_LOAD_DATA )																						//---低位数据加载
	#define HVPP_ENABLE_DATA_LOAD_HIGH						( HVPP_BS_SELECT_DATA_HIGH,HVPP_XA_LOAD_DATA )																						//---高位数据加载
	//<<<===命令和地址加载的编码---结束

	//===等待状态稳定的滞后时钟
	#define HVPP_WAIT_STATE_STABLE							DELAY_NOP_COUNT(8)																										//---等待几个时钟周期，用于数据的稳定

	//===进入高压模式之前，初始化控制端
	#define HVPP_CTRL_INIT_READ								( HVPP_CTRL_BUS_READ ,HVPP_CTRL_BUS_VAL(0x7F),HVPP_CTRL_DIR_TO_DEVICE )													//---进入高压模式之前，初始化控制端
	#define HVPP_CTRL_INIT_WRITE							( HVPP_CTRL_BUS_WRITE,HVPP_CTRL_BUS_VAL(0x36),HVPP_CTRL_DIR_TO_DEVICE )													//---进入高压模式之后，初始化控制端

	//===并行高压的DUT电源
	#define HVPP_DUT_POWER_ON								LM317_POWER_ON																											//---开启电源
	#define HVPP_DUT_POWER_HZ								LM317_POWER_HZ																											//---外部提供电源，这里电源开关处于不导通状态
	#define HVPP_DUT_POWER_OFF								LM317_POWER_OFF																											//---关闭电源，电源输入端通过MOS管短路到第
	
	//===并行高压的REST的操作
	#define HVPP_DUT_RST_TO_GND								RST_PORT_TO_GND																											//---RST接地
	#define HVPP_DUT_RST_TO_VCC								RST_PORT_TO_VCC																											//---RST接到供电电源
	#define HVPP_DUT_RST_TO_12V								RST_PORT_TO_12V																											//---RST接到高压12V
	#define HVPP_DUT_RST_TO_HZ								RST_PORT_TO_HZ																											//---RST处于浮空状态，即不上拉也不下拉
	
	//>>>===高压并行组合命令---开始
	#define HVPP_CMD_CHIP_ERASE								0B10000000																												//---芯片擦除
	#define HVPP_CMD_WRITE_CHIP_FUSE						0B01000000																												//---写熔丝位
	#define HVPP_CMD_WRITE_CHIP_LOCK						0B00100000																												//---写锁定位
	#define HVPP_CMD_WRITE_FLASH							0B00010000																												//---写FLASH
	#define HVPP_CMD_WRITE_EEPROM							0B00010001																												//---写EEPROM
	#define HVPP_CMD_READ_ID_AND_CALIBRATION				0B00001000																												//---读标识字和校准字节
	#define HVPP_CMD_READ_FUSE_AND_LOCK						0B00000100																												//---读取熔丝位和加密位
	#define HVPP_CMD_READ_FLASH								0B00000010																												//---读取FLASH
	#define HVPP_CMD_READ_EEPROM							0B00000011																												//---读取EEPROM
	//<<<===高压并行组合命令---结束

	//===数据总线的模式
	#define HVPP_DATA_BUS_MODE_READ							0																														//---输出总线处于读取模式
	#define HVPP_DATA_BUS_MODE_WRITE						1																														//---数据总线处于写入模式
				
	//===数据总线的读取模式	
	#define HVPP_DATA_BUS_READ_DIR							0																														//---切换数据总线的方向为读取模式
	#define HVPP_DATA_BUS_READ_VAL							1																														//---读取数据总线上的数据
			
	//===定义ISP状态保持的时间状态
	#define HVPP_STATE_TIME_OUT_MS							500

	//===定义结构体
	typedef struct _HVPP_HandlerType						HVPP_HandlerType;
	//===定义指针结构体
	typedef struct _HVPP_HandlerType						* pHVPP_HandlerType;
	//===结构体变量
	struct _HVPP_HandlerType
	{
		UINT8_T		msgState;																																						//---编程状态，0---空闲状态，1---编程状态
		UINT8_T		msgHvFastMode;																																					//---是否使快速编程模式，0---常规模式，1---快速模式
		UINT8_T		msgFactoryPattern;																																				//---是否打开了工厂模式，0---未打开工厂模式，1---打开工厂模式
		UINT8_T		msgSelfPower;																																					//---是不是自己供电0---自己供电，1---外部供电
		UINT8_T		msgXtalPulseWidth;																																				//---XTAL端口的时钟个数
		UINT8_T		msgHvCmd;																																						//---高压编程命令，0---常规模式，1---其他模式
		UINT8_T		msgLoadCmd;																																						//---并行高压模式下，是否加载了命令，0---未加载，1--已经加载
		UINT8_T		msgDataBusState;																																				//---DATABus总线的状态，0---读取状态，1---写入状态
		UINT8_T		msgWaitms;																																						//---编程之后的延时函数，单位是ms
		UINT8_T		msgPollReady;																																					//---是否轮询准备好信号，0---不需要；1---需要
		UINT8_T		msgEepromPageMode;																																				//---eeprom是否支持页编程模式，0---不支持，1---支持
		UINT16_T	msgFlashPerPageWordSize;																																		//---Flash的每页字数
		UINT16_T	msgEerpomPerPageByteSize;																																		//---Eeprom的每页字节数
		UINT16_T	msgPageWordIndex;																																				//---Flash缓存区的序号
		UINT16_T	msgIntervalTime;																																				//---轮询时间间隔,单位是ms
		UINT16_T	msgProgWRLus;																																					//---写低脉冲宽度
		UINT16_T	msgProgWRHus;																																					//---写高脉冲宽度
		UINT32_T	msgRecordTime;																																					//---记录的时间参数
		UINT32_T	(*msgTimeTick)(void);																																			//---用于超时计数
		void(*msgDelayus)(UINT32_T delay);																																			//---us延时参数
		void(*msgDelayms)(UINT32_T delay);																																			//---ms延时参数
	};
	
	//===任务函数
	#define HVPP_TASK_ONE								pHvppDevice0																												//---任务1
	#define HVPP_TASK_TWO								0																															//---任务2
	#define HVPP_TASK_THREE								0																															//---任务3

	//===外部调用函数
	extern HVPP_HandlerType								g_HvppDevice0;
	extern pHVPP_HandlerType							pHvppDevice0 ;

	//===函数定义
	UINT8_T HVPP_DATABUS_WRITE(HVPP_HandlerType* HVPPx, UINT8_T val);
	UINT8_T HVPP_DATABUS_READ(HVPP_HandlerType* HVPPx, UINT8_T isReadDir);
	UINT8_T HVPP_GPIO_Init(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_GPIO_DeInit(HVPP_HandlerType* HVPPx);
	void HVPP_XTAL_CLK(HVPP_HandlerType* HVPPx, UINT8_T clkNum);
	void HVPP_XTAL_PULSE(HVPP_HandlerType* HVPPx);
	void HVPP_AddWatchDevice0(void);
	void HVPP_WatchTask(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_AddWatch(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_RemoveWatch(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_RefreshWatch(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_SetIntervalTime(HVPP_HandlerType* HVPPx, UINT16_T intervalTime);
	UINT8_T HVPP_GetIntervalTime(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_Init(HVPP_HandlerType* HVPPx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void));
	UINT8_T HVPP_DeInit(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_ProgModeEnter(HVPP_HandlerType* HVPPx, UINT8_T pagel, UINT8_T xa1, UINT8_T xa0, UINT8_T bs1);
	UINT8_T HVPP_ProgModeExit(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_EnterProg(HVPP_HandlerType* HVPPx, UINT8_T hvModeCmd);
	UINT8_T HVPP_ExitProg(HVPP_HandlerType* HVPPx);
	UINT8_T HVPP_ReadChipID(HVPP_HandlerType* HVPPx, UINT8_T* pVal);
	UINT8_T HVPP_ReadChipCalibration(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T length);
	UINT8_T HVPP_ReadChipFuse(HVPP_HandlerType* HVPPx, UINT8_T* pVal, UINT8_T isNeedExternFuse);
	UINT8_T HVPP_ReadChipLock(HVPP_HandlerType* HVPPx, UINT8_T* pVal);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*HVPP_CFG_H_ */