#ifndef HVPP_CFG_H_
#define HVPP_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//=============HVPP使用的端口和上电初始化的状态========================================
	//===HVPP_RDY_BSY	---设置为输入，在整个高压过程都是输入
	//===HVPP_OE		---设置为输出，默认是高电平，该引脚低电平有效
	//===HVPP_WR		---设置为输出，默认是高电平，该引脚低电平有效
	//===HVPP_BS1		---设置为输出，
	//===HVPP_XA0		---设置为输出，默认是高电平，和XA1的高电平组合为空闲模式
	//===HVPP_XA1		---设置为输出，默认是高电平，和XA0的高电平组合为空闲模式
	//===HVPP_PAGEL		---设置为输出，
	//===HVPP_BS2		---设置为输出，
	//===HVPP_DATA		---设置为输入，默认需要读取外部数据
	//===================================================================================
	//===O---编程忙和1---等待新命令，device--->>>host---O
	#define HVPP_RDY_BSY_PORT						GPIOC
	#define HVPP_RDY_BSY_BIT						LL_GPIO_PIN_15
	#define HVPP_RDY_BSY_STATE						GPIO_GET_STATE(HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_WRITE						GPIO_SET_WRITE(HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_READ						GPIO_SET_READ( HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_0						GPIO_OUT_0(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_1						GPIO_OUT_1(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)
	#define HVPP_RDY_BSY_OUT_C						GPIO_OUT_C(    HVPP_RDY_BSY_PORT,HVPP_RDY_BSY_BIT)

	//===输出使能，低电平有效；device--->>>host---I
	#define HVPP_OE_PORT							GPIOE
	#define HVPP_OE_BIT								LL_GPIO_PIN_9
	#define HVPP_OE_STATE							GPIO_GET_STATE(HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_WRITE							GPIO_SET_WRITE(HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_READ							GPIO_SET_READ( HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_0							GPIO_OUT_0(    HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_1							GPIO_OUT_1(    HVPP_OE_PORT,HVPP_OE_BIT)
	#define HVPP_OE_OUT_C							GPIO_OUT_C(    HVPP_OE_PORT,HVPP_OE_BIT)

	//===写脉冲，低电平有效；device--->>>host---I
	#define HVPP_WR_PORT							GPIOE
	#define HVPP_WR_BIT								LL_GPIO_PIN_10
	#define HVPP_WR_STATE							GPIO_GET_STATE(HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_WRITE							GPIO_SET_WRITE(HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_READ							GPIO_SET_READ( HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_0							GPIO_OUT_0(    HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_1							GPIO_OUT_1(    HVPP_WR_PORT,HVPP_WR_BIT)
	#define HVPP_WR_OUT_C							GPIO_OUT_C(    HVPP_WR_PORT,HVPP_WR_BIT)

	//===字节选择1(0---选择低字节，1---选择高字节)；device--->>>host---I
	#define HVPP_BS1_PORT							GPIOE
	#define HVPP_BS1_BIT							LL_GPIO_PIN_11
	#define HVPP_BS1_STATE							GPIO_GET_STATE(HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_WRITE							GPIO_SET_WRITE(HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_READ							GPIO_SET_READ( HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_0							GPIO_OUT_0(    HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_1							GPIO_OUT_1(    HVPP_BS1_PORT,HVPP_BS1_BIT)
	#define HVPP_BS1_OUT_C							GPIO_OUT_C(    HVPP_BS1_PORT,HVPP_BS1_BIT)

	//===XTAL动作0；device--->>>host---I
	#define HVPP_XA0_PORT							GPIOE
	#define HVPP_XA0_BIT							LL_GPIO_PIN_12
	#define HVPP_XA0_STATE							GPIO_GET_STATE(HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_WRITE							GPIO_SET_WRITE(HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_READ							GPIO_SET_READ( HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_0							GPIO_OUT_0(    HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_1							GPIO_OUT_1(    HVPP_XA0_PORT,HVPP_XA0_BIT)
	#define HVPP_XA0_OUT_C							GPIO_OUT_C(    HVPP_XA0_PORT,HVPP_XA0_BIT)

	//===XTAL动作1；device--->>>host---I
	#define HVPP_XA1_PORT							GPIOE
	#define HVPP_XA1_BIT							LL_GPIO_PIN_13
	#define HVPP_XA1_STATE							GPIO_GET_STATE(HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_WRITE							GPIO_SET_WRITE(HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_READ							GPIO_SET_READ( HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_0							GPIO_OUT_0(    HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_1							GPIO_OUT_1(    HVPP_XA1_PORT,HVPP_XA1_BIT)
	#define HVPP_XA1_OUT_C							GPIO_OUT_C(    HVPP_XA1_PORT,HVPP_XA1_BIT)

	//===加载程序存储器和EEPROM数据页；device--->>>host---I
	#define HVPP_PAGEL_PORT							GPIOE
	#define HVPP_PAGEL_BIT							LL_GPIO_PIN_14
	#define HVPP_PAGEL_STATE						GPIO_GET_STATE(HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_WRITE						GPIO_SET_WRITE(HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_READ							GPIO_SET_READ( HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_0						GPIO_OUT_0(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_1						GPIO_OUT_1(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)
	#define HVPP_PAGEL_OUT_C						GPIO_OUT_C(    HVPP_PAGEL_PORT,HVPP_PAGEL_BIT)

	//===字节选择2(0---选择低字节，1---选择第二个高字节)；device--->>>host---I
	#define HVPP_BS2_PORT							GPIOE
	#define HVPP_BS2_BIT							LL_GPIO_PIN_8
	#define HVPP_BS2_STATE							GPIO_GET_STATE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_BS2_WRITE							GPIO_SET_WRITE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_BS2_READ							GPIO_SET_READ( DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_BS2_OUT_0							GPIO_OUT_0(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_BS2_OUT_1							GPIO_OUT_1(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_BS2_OUT_C							GPIO_OUT_C(    DECA_CH_PORT,DECA_CH_BIT)

	//===时钟引脚；device--->>>host---I
	#define HVPP_XTAL_PORT							GPIOE
	#define HVPP_XTAL_BIT							LL_GPIO_PIN_15
	#define HVPP_XTAL_STATE							GPIO_GET_STATE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_XTAL_WRITE							GPIO_SET_WRITE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_XTAL_READ							GPIO_SET_READ( DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_XTAL_OUT_0							GPIO_OUT_0(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_XTAL_OUT_1							GPIO_OUT_1(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_XTAL_OUT_C							GPIO_OUT_C(    DECA_CH_PORT,DECA_CH_BIT)

	//===定义控制使能信号
	#define HVPP_CTRL_OE_PORT						GPIOD
	#define HVPP_CTRL_OE_BIT						LL_GPIO_PIN_4
	#define HVPP_CTRL_OE_STATE						GPIO_GET_STATE(HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_WRITE						GPIO_SET_WRITE(HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_READ						GPIO_SET_READ( HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_0						GPIO_OUT_0(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_1						GPIO_OUT_1(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)
	#define HVPP_CTRL_OE_OUT_C						GPIO_OUT_C(    HVPP_CTRL_OE_PORT,HVPP_CTRL_OE_BIT)

	//===电平转换芯片的数据流向
	#define HVPP_CTRL_TO_DEVICE						HVPP_CTRL_OE_OUT_0					
	#define HVPP_CTRL_TO_HZ							HVPP_CTRL_OE_OUT_1

	//===双向数据总线Bit0(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA0_PORT							GPIOE
	#define HVPP_DATA0_BIT							LL_GPIO_PIN_7
	#define HVPP_DATA0_STATE						GPIO_GET_STATE(HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_WRITE						GPIO_SET_WRITE(HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_READ							GPIO_SET_READ( HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_0						GPIO_OUT_0(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_1						GPIO_OUT_1(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)
	#define HVPP_DATA0_OUT_C						GPIO_OUT_C(    HVPP_DATA0_PORT,HVPP_DATA0_BIT)

	//===双向数据总线Bit1(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA1_PORT							GPIOE
	#define HVPP_DATA1_BIT							LL_GPIO_PIN_6
	#define HVPP_DATA1_STATE						GPIO_GET_STATE(HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_WRITE						GPIO_SET_WRITE(HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_READ							GPIO_SET_READ( HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_0						GPIO_OUT_0(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_1						GPIO_OUT_1(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)
	#define HVPP_DATA1_OUT_C						GPIO_OUT_C(    HVPP_DATA1_PORT,HVPP_DATA1_BIT)

	//===双向数据总线Bit2(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA2_PORT							GPIOE
	#define HVPP_DATA2_BIT							LL_GPIO_PIN_5
	#define HVPP_DATA2_STATE						GPIO_GET_STATE(HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_WRITE						GPIO_SET_WRITE(HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_READ							GPIO_SET_READ( HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_0						GPIO_OUT_0(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_1						GPIO_OUT_1(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)
	#define HVPP_DATA2_OUT_C						GPIO_OUT_C(    HVPP_DATA2_PORT,HVPP_DATA2_BIT)

	//===双向数据总线Bit3(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA3_PORT							GPIOE
	#define HVPP_DATA3_BIT							LL_GPIO_PIN_4
	#define HVPP_DATA3_STATE						GPIO_GET_STATE(HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_WRITE						GPIO_SET_WRITE(HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_READ							GPIO_SET_READ( HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_0						GPIO_OUT_0(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_1						GPIO_OUT_1(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)
	#define HVPP_DATA3_OUT_C						GPIO_OUT_C(    HVPP_DATA3_PORT,HVPP_DATA3_BIT)

	//===双向数据总线Bit4(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA4_PORT							GPIOE
	#define HVPP_DATA4_BIT							LL_GPIO_PIN_3
	#define HVPP_DATA4_STATE						GPIO_GET_STATE(HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_WRITE						GPIO_SET_WRITE(HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_READ							GPIO_SET_READ( HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_0						GPIO_OUT_0(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_1						GPIO_OUT_1(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)
	#define HVPP_DATA4_OUT_C						GPIO_OUT_C(    HVPP_DATA4_PORT,HVPP_DATA4_BIT)

	//===双向数据总线Bit5(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA5_PORT							GPIOE
	#define HVPP_DATA5_BIT							LL_GPIO_PIN_2
	#define HVPP_DATA5_STATE						GPIO_GET_STATE(HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_WRITE						GPIO_SET_WRITE(HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_READ							GPIO_SET_READ( HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_0						GPIO_OUT_0(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_1						GPIO_OUT_1(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)
	#define HVPP_DATA5_OUT_C						GPIO_OUT_C(    HVPP_DATA5_PORT,HVPP_DATA5_BIT)

	//===双向数据总线Bit6(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA6_PORT							GPIOE
	#define HVPP_DATA6_BIT							LL_GPIO_PIN_1
	#define HVPP_DATA6_STATE						GPIO_GET_STATE(HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_WRITE						GPIO_SET_WRITE(HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_READ							GPIO_SET_READ( HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_0						GPIO_OUT_0(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_1						GPIO_OUT_1(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)
	#define HVPP_DATA6_OUT_C						GPIO_OUT_C(    HVPP_DATA6_PORT,HVPP_DATA6_BIT)

	//===双向数据总线Bit7(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA7_PORT							GPIOE
	#define HVPP_DATA7_BIT							LL_GPIO_PIN_0
	#define HVPP_DATA7_STATE						GPIO_GET_STATE(HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_WRITE						GPIO_SET_WRITE(HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_READ							GPIO_SET_READ( HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_0						GPIO_OUT_0(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_1						GPIO_OUT_1(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)
	#define HVPP_DATA7_OUT_C						GPIO_OUT_C(    HVPP_DATA7_PORT,HVPP_DATA7_BIT)

	//===双向数据总线(OE为低时输出)；device--->>>host---I/O
	#define HVPP_DATA_BUS_PORT						GPIOE
	#define HVPP_DATA_BUS_BIT						(LL_GPIO_PIN_0|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)
	#define HVPP_DATA_BUS_STATE						GPIO_GET_STATE(HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_WRITE						GPIO_SET_WRITE(HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_READ						GPIO_SET_READ( HVPP_DATA_BUS_PORT,HVPP_DATA_BUS_BIT)
	#define HVPP_DATA_BUS_VAL(val)					GPIO_L8BITS_OUT(val)	

	//===定义数据线使能信号
	#define HVPP_DATA_OE_PORT						GPIOB
	#define HVPP_DATA_OE_BIT						LL_GPIO_PIN_15
	#define HVPP_DATA_OE_STATE						GPIO_GET_STATE(HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_WRITE						GPIO_SET_WRITE(HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_READ						GPIO_SET_READ( HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_0						GPIO_OUT_0(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_1						GPIO_OUT_1(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)
	#define HVPP_DATA_OE_OUT_C						GPIO_OUT_C(    HVPP_DATA_OE_PORT,HVPP_DATA_OE_BIT)

	//===定义数据线使能方向信号
	#define HVPP_DATA_DIR_PORT						GPIOB
	#define HVPP_DATA_DIR_BIT						LL_GPIO_PIN_14
	#define HVPP_DATA_DIR_STATE						GPIO_GET_STATE(HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_WRITE						GPIO_SET_WRITE(HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_READ						GPIO_SET_READ( HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_0						GPIO_OUT_0(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_1						GPIO_OUT_1(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)
	#define HVPP_DATA_DIR_OUT_C						GPIO_OUT_C(    HVPP_DATA_DIR_PORT,HVPP_DATA_DIR_BIT)

	//===电平转换芯片的数据流向
	#define HVPP_DATA_BUS_TO_DEVICE					(HVPP_DATA_DIR_OUT_1,HVPP_DATA_OE_OUT_0)
	#define HVPP_DATA_BUS_TO_HOST					(HVPP_DATA_DIR_OUT_0,HVPP_DATA_OE_OUT_0)
	#define HVPP_DATA_BUS_TO_HZ						HVPP_DATA_OE_OUT_1

	//>>>===XA0,XA1组合编码功能---开始
	#define HVPP_XA_LOAD_ADDR						( HVPP_XA1_OUT_0,HVPP_XA0_OUT_0 )				//---加载存储器地址
	#define HVPP_XA_LOAD_DATA						( HVPP_XA1_OUT_0,HVPP_XA0_OUT_1 )				//---加载数据
	#define HVPP_XA_LOAD_CMD						( HVPP_XA1_OUT_1,HVPP_XA0_OUT_0 )				//---加载命令
	#define HVPP_XA_LOAD_IDLE						( HVPP_XA1_OUT_1,HVPP_XA0_OUT_1 )				//---空闲模式
	//<<<===XA0,XA1组合编码功能---结束
	
	//===高压编程模式的进入
	#define HVPP_ENTER_PROG_ENABLE_MODE(a,b,c,d)
	
	//>>>===高压并行组合命令---开始
	#define HVPP_CMD_CHIP_ERASE						0B10000000										//---芯片擦除
	#define HVPP_CMD_WRITE_CHIP_FUSE				0B01000000										//---写熔丝位
	#define HVPP_CMD_WRITE_CHIP_LOCK				0B00100000										//---写锁定位
	#define HVPP_CMD_WRITE_FLASH					0B00010000										//---写FLASH
	#define HVPP_CMD_WRITE_EEPROM					0B00010001										//---写EEPROM
	#define HVPP_CMD_READ_ID_AND_CALIBRATION		0B00001000										//---读标识字和校准字节
	#define HVPP_CMD_READ_FUSE_AND_LOCK				0B00000100										//---读取熔丝位和加密位
	#define HVPP_CMD_READ_FLASH						0B00000010										//---读取FLASH
	#define HVPP_CMD_READ_EEPROM					0B00000011										//---读取EEPROM
	//<<<===高压并行组合命令---结束

	//===数据总线的状态
	#define HVPP_DATA_BUS_STATE_READ				0												//---输出总线处于读取状态
	#define HVPP_DATA_BUS_STATE_WRITE				1												//---数据总线处于写入状态
				
	//===定义结构体
	typedef struct _HVPP_HandlerType				HVPP_HandlerType;
	//===定义指针结构体
	typedef struct _HVPP_HandlerType				* pHVPP_HandlerType;
	//===结构体变量
	struct _HVPP_HandlerType
	{
		UINT8_T msgDataBusState;																	//---DATABus总线的状态，0---读取状态，1---写入状态
		UINT32_T(*msgTimeTick)(void);																//---用于超时计数
		void(*msgDelayus)(UINT32_T delay);															//---us延时参数
		void(*msgDelayms)(UINT32_T delay);															//---ms延时参数
	};
	
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*HVPP_CFG_H_ */