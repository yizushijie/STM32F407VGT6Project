#ifndef KEY_CFG_H_
#define KEY_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
#include "complier_lib.h"
#include "gpio_task.h"
#include "delay_task.h"
#include "rfaskhw_cfg.h"
#include "rfask_task.h"
//////////////////////////////////////////////////////////////////////////////////////
//===>>MultiButton START=========================================================
//===轮训时间调度
#define TICKS_INTERVAL			5	//ms

//===消抖处理
#define DEBOUNCE_TICKS			4	//MAX 8

//===短按
#define SHORT_TICKS				(300 /TICKS_INTERVAL)

//===长按
#define LONG_TICKS				(1000 /TICKS_INTERVAL)

//===按键回调函数
	typedef void(*KeyButtonCallBack)(void*);

	//===按键事件枚举
	typedef enum
	{
		PRESS_DOWN = 0,			//---按键按下，每次按下都触发
		PRESS_UP,				//---按键弹起，每次松开都触发
		PRESS_REPEAT,			//---重复按下触发，变量repeat计数连击次数
		SINGLE_CLICK,			//---单击按键事件
		DOUBLE_CLICK,			//---双击按键事件
		LONG_RRESS_START,		//---达到长按时间阈值时触发一次
		LONG_PRESS_HOLD,		//---长按期间一直触发
		NUM_OF_EVENT,			//---按键事件的个数
		NONE_PRESS				//---没有按键按下
	}KeyButtonPressEvent;

	//===定义结构体
	typedef struct _KeyButton_HandlerType	 KeyButton_HandlerType;

	//===定义指针结构体
	typedef struct _KeyButton_HandlerType	*pKeyButton_HandlerType;

	//===按键按钮结构
	struct _KeyButton_HandlerType
	{
		UINT32_T msgTicks;					//---计数
		UINT8_T  msgRepeat;					//---重复次数
		UINT8_T  msgEvent;					//---事件类型
		UINT8_T  msgState;					//---状态
		UINT8_T  msgDebounceCNT;					//---软件消抖计数
		UINT8_T  msgActiveLevel;					//---按键有效电平
		UINT8_T  msgButtonLevel;					//---按键电平
		UINT8_T(*msgFuncReadPinLevel)(void);			//---按键读取函数
		KeyButtonCallBack  msgCB[NUM_OF_EVENT];			//---按键响应事件
		KeyButton_HandlerType* msgNext;					//---指向下一按键处理函数
	};

	//===<<MultiButton END=========================================================

	//===SOTA的配置
	#define SOTA_CTR_PORT								SOTA_PORT//GPIOE
	#define SOTA_CTR_BIT								SOTA_BIT //LL_GPIO_PIN_11
	#define SOTA_CTR_STATE								GPIO_GET_STATE(SOTA_CTR_PORT,SOTA_CTR_BIT)
	#define SOTA_CTR_WRITE								GPIO_SET_WRITE(SOTA_CTR_PORT,SOTA_CTR_BIT)
	#define SOTA_CTR_READ								GPIO_SET_READ( SOTA_CTR_PORT,SOTA_CTR_BIT)
	#define SOTA_CTR_OUT_0								GPIO_OUT_0(    SOTA_CTR_PORT,SOTA_CTR_BIT)
	#define SOTA_CTR_OUT_1								GPIO_OUT_1(    SOTA_CTR_PORT,SOTA_CTR_BIT)
	#define SOTA_CTR_OUT_C								GPIO_OUT_C(    SOTA_CTR_PORT,SOTA_CTR_BIT)
	
	//===EOTA的配置
	#define EOTA_CTR_PORT								EOTA_PORT//GPIOE
	#define EOTA_CTR_BIT								EOTA_BIT //LL_GPIO_PIN_12
	#define EOTA_CTR_STATE								GPIO_GET_STATE(EOTA_CTR_PORT,EOTA_CTR_BIT)
	#define EOTA_CTR_WRITE								GPIO_SET_WRITE(EOTA_CTR_PORT,EOTA_CTR_BIT)
	#define EOTA_CTR_READ								GPIO_SET_READ( EOTA_CTR_PORT,EOTA_CTR_BIT)
	#define EOTA_CTR_OUT_0								GPIO_OUT_0(    EOTA_CTR_PORT,EOTA_CTR_BIT)
	#define EOTA_CTR_OUT_1								GPIO_OUT_1(    EOTA_CTR_PORT,EOTA_CTR_BIT)
	#define EOTA_CTR_OUT_C								GPIO_OUT_C(    EOTA_CTR_PORT,EOTA_CTR_BIT)
	
	//===BINA的配置
	#define BINA_CTR_PORT								BINA_PORT//GPIOE
	#define BINA_CTR_BIT								BINA_BIT //LL_GPIO_PIN_13
	#define BINA_CTR_STATE								GPIO_GET_STATE(BINA_CTR_PORT,BINA_CTR_BIT)
	#define BINA_CTR_WRITE								GPIO_SET_WRITE(BINA_CTR_PORT,BINA_CTR_BIT)
	#define BINA_CTR_READ								GPIO_SET_READ( BINA_CTR_PORT,BINA_CTR_BIT)
	#define BINA_CTR_OUT_0								GPIO_OUT_0(    BINA_CTR_PORT,BINA_CTR_BIT)
	#define BINA_CTR_OUT_1								GPIO_OUT_1(    BINA_CTR_PORT,BINA_CTR_BIT)
	#define BINA_CTR_OUT_C								GPIO_OUT_C(    BINA_CTR_PORT,BINA_CTR_BIT)
	
	//===SOTB的配置
	#define SOTB_CTR_PORT								SOTB_PORT//GPIOE
	#define SOTB_CTR_BIT								SOTB_BIT //LL_GPIO_PIN_2
	#define SOTB_CTR_STATE								GPIO_GET_STATE(SOTB_CTR_PORT,SOTB_CTR_BIT)
	#define SOTB_CTR_WRITE								GPIO_SET_WRITE(SOTB_CTR_PORT,SOTB_CTR_BIT)
	#define SOTB_CTR_READ								GPIO_SET_READ( SOTB_CTR_PORT,SOTB_CTR_BIT)
	#define SOTB_CTR_OUT_0								GPIO_OUT_0(    SOTB_CTR_PORT,SOTB_CTR_BIT)
	#define SOTB_CTR_OUT_1								GPIO_OUT_1(    SOTB_CTR_PORT,SOTB_CTR_BIT)
	#define SOTB_CTR_OUT_C								GPIO_OUT_C(    SOTB_CTR_PORT,SOTB_CTR_BIT)
	
	//===EOTB的配置
	#define EOTB_CTR_PORT								EOTB_PORT//GPIOE
	#define EOTB_CTR_BIT								EOTB_BIT //LL_GPIO_PIN_3
	#define EOTB_CTR_STATE								GPIO_GET_STATE(EOTB_CTR_PORT,EOTB_CTR_BIT)
	#define EOTB_CTR_WRITE								GPIO_SET_WRITE(EOTB_CTR_PORT,EOTB_CTR_BIT)
	#define EOTB_CTR_READ								GPIO_SET_READ( EOTB_CTR_PORT,EOTB_CTR_BIT)
	#define EOTB_CTR_OUT_0								GPIO_OUT_0(    EOTB_CTR_PORT,EOTB_CTR_BIT)
	#define EOTB_CTR_OUT_1								GPIO_OUT_1(    EOTB_CTR_PORT,EOTB_CTR_BIT)
	#define EOTB_CTR_OUT_C								GPIO_OUT_C(    EOTB_CTR_PORT,EOTB_CTR_BIT)
	
	//===BINB的配置
	#define BINB_CTR_PORT								BINB_PORT//GPIOE
	#define BINB_CTR_BIT								BINB_BIT //LL_GPIO_PIN_4
	#define BINB_CTR_STATE								GPIO_GET_STATE(BINB_CTR_PORT,BINB_CTR_BIT)
	#define BINB_CTR_WRITE								GPIO_SET_WRITE(BINB_CTR_PORT,BINB_CTR_BIT)
	#define BINB_CTR_READ								GPIO_SET_READ( BINB_CTR_PORT,BINB_CTR_BIT)
	#define BINB_CTR_OUT_0								GPIO_OUT_0(    BINB_CTR_PORT,BINB_CTR_BIT)
	#define BINB_CTR_OUT_1								GPIO_OUT_1(    BINB_CTR_PORT,BINB_CTR_BIT)
	#define BINB_CTR_OUT_C								GPIO_OUT_C(    BINB_CTR_PORT,BINB_CTR_BIT)
	
	//===SOTC的配置
	#define SOTC_CTR_PORT								SOTC_PORT//GPIOD
	#define SOTC_CTR_BIT								SOTC_BIT //LL_GPIO_PIN_3
	#define SOTC_CTR_STATE								GPIO_GET_STATE(SOTC_CTR_PORT,SOTC_CTR_BIT)
	#define SOTC_CTR_WRITE								GPIO_SET_WRITE(SOTC_CTR_PORT,SOTC_CTR_BIT)
	#define SOTC_CTR_READ								GPIO_SET_READ( SOTC_CTR_PORT,SOTC_CTR_BIT)
	#define SOTC_CTR_OUT_0								GPIO_OUT_0(    SOTC_CTR_PORT,SOTC_CTR_BIT)
	#define SOTC_CTR_OUT_1								GPIO_OUT_1(    SOTC_CTR_PORT,SOTC_CTR_BIT)
	#define SOTC_CTR_OUT_C								GPIO_OUT_C(    SOTC_CTR_PORT,SOTC_CTR_BIT)
	
	//===EOTC的配置
	#define EOTC_CTR_PORT								EOTC_PORT//GPIOD
	#define EOTC_CTR_BIT								EOTC_BIT //LL_GPIO_PIN_2
	#define EOTC_CTR_STATE								GPIO_GET_STATE(EOTC_CTR_PORT,EOTC_CTR_BIT)
	#define EOTC_CTR_WRITE								GPIO_SET_WRITE(EOTC_CTR_PORT,EOTC_CTR_BIT)
	#define EOTC_CTR_READ								GPIO_SET_READ( EOTC_CTR_PORT,EOTC_CTR_BIT)
	#define EOTC_CTR_OUT_0								GPIO_OUT_0(    EOTC_CTR_PORT,EOTC_CTR_BIT)
	#define EOTC_CTR_OUT_1								GPIO_OUT_1(    EOTC_CTR_PORT,EOTC_CTR_BIT)
	#define EOTC_CTR_OUT_C								GPIO_OUT_C(    EOTC_CTR_PORT,EOTC_CTR_BIT)
	
	//===BINC的配置
	#define BINC_CTR_PORT								BINC_PORT//GPIOD
	#define BINC_CTR_BIT								BINC_BIT //LL_GPIO_PIN_1
	#define BINC_CTR_STATE								GPIO_GET_STATE(BINC_CTR_PORT,BINC_CTR_BIT)
	#define BINC_CTR_WRITE								GPIO_SET_WRITE(BINC_CTR_PORT,BINC_CTR_BIT)
	#define BINC_CTR_READ								GPIO_SET_READ( BINC_CTR_PORT,BINC_CTR_BIT)
	#define BINC_CTR_OUT_0								GPIO_OUT_0(    BINC_CTR_PORT,BINC_CTR_BIT)
	#define BINC_CTR_OUT_1								GPIO_OUT_1(    BINC_CTR_PORT,BINC_CTR_BIT)
	#define BINC_CTR_OUT_C								GPIO_OUT_C(    BINC_CTR_PORT,BINC_CTR_BIT)
	
	//===SOTD的配置
	#define SOTD_CTR_PORT								SOTD_PORT//GPIOD
	#define SOTD_CTR_BIT								SOTD_BIT //LL_GPIO_PIN_11
	#define SOTD_CTR_STATE								GPIO_GET_STATE(SOTD_CTR_PORT,SOTD_CTR_BIT)
	#define SOTD_CTR_WRITE								GPIO_SET_WRITE(SOTD_CTR_PORT,SOTD_CTR_BIT)
	#define SOTD_CTR_READ								GPIO_SET_READ( SOTD_CTR_PORT,SOTD_CTR_BIT)
	#define SOTD_CTR_OUT_0								GPIO_OUT_0(    SOTD_CTR_PORT,SOTD_CTR_BIT)
	#define SOTD_CTR_OUT_1								GPIO_OUT_1(    SOTD_CTR_PORT,SOTD_CTR_BIT)
	#define SOTD_CTR_OUT_C								GPIO_OUT_C(    SOTD_CTR_PORT,SOTD_CTR_BIT)
	
	//===EOTD的配置
	#define EOTD_CTR_PORT								EOTD_PORT//GPIOD
	#define EOTD_CTR_BIT								EOTD_BIT //LL_GPIO_PIN_10
	#define EOTD_CTR_STATE								GPIO_GET_STATE(EOTD_CTR_PORT,EOTD_CTR_BIT)
	#define EOTD_CTR_WRITE								GPIO_SET_WRITE(EOTD_CTR_PORT,EOTD_CTR_BIT)
	#define EOTD_CTR_READ								GPIO_SET_READ( EOTD_CTR_PORT,EOTD_CTR_BIT)
	#define EOTD_CTR_OUT_0								GPIO_OUT_0(    EOTD_CTR_PORT,EOTD_CTR_BIT)
	#define EOTD_CTR_OUT_1								GPIO_OUT_1(    EOTD_CTR_PORT,EOTD_CTR_BIT)
	#define EOTD_CTR_OUT_C								GPIO_OUT_C(    EOTD_CTR_PORT,EOTD_CTR_BIT)
	
	//===BIND的配置
	#define BIND_CTR_PORT								BIND_PORT//GPIOD
	#define BIND_CTR_BIT								BIND_BIT //LL_GPIO_PIN_9
	#define BIND_CTR_STATE								GPIO_GET_STATE(BIND_CTR_PORT,BIND_CTR_BIT)
	#define BIND_CTR_WRITE								GPIO_SET_WRITE(BIND_CTR_PORT,BIND_CTR_BIT)
	#define BIND_CTR_READ								GPIO_SET_READ( BIND_CTR_PORT,BIND_CTR_BIT)
	#define BIND_CTR_OUT_0								GPIO_OUT_0(    BIND_CTR_PORT,BIND_CTR_BIT)
	#define BIND_CTR_OUT_1								GPIO_OUT_1(    BIND_CTR_PORT,BIND_CTR_BIT)
	#define BIND_CTR_OUT_C								GPIO_OUT_C(    BIND_CTR_PORT,BINC_CTR_BIT)
	
	#define SOT_CTR_OUT_1								( SOTA_CTR_OUT_1,SOTB_CTR_OUT_1,SOTC_CTR_OUT_1,SOTD_CTR_OUT_1 )
	#define SOT_CTR_OUT_0								( SOTA_CTR_OUT_0,SOTB_CTR_OUT_0,SOTC_CTR_OUT_0,SOTD_CTR_OUT_0 )
	
	#define EOT_CTR_OUT_1								( EOTA_CTR_OUT_1,EOTB_CTR_OUT_1,EOTC_CTR_OUT_1,EOTD_CTR_OUT_1 )
	#define EOT_CTR_OUT_0								( EOTA_CTR_OUT_0,EOTB_CTR_OUT_0,EOTC_CTR_OUT_0,EOTD_CTR_OUT_0)
	
	#define BIN_CTR_OUT_1								( BINA_CTR_OUT_1,BINB_CTR_OUT_1,BINC_CTR_OUT_1,BIND_CTR_OUT_1 )
	#define BIN_CTR_OUT_0								( BINA_CTR_OUT_0,BINB_CTR_OUT_0,BINC_CTR_OUT_0,BIND_CTR_OUT_0 )

	//===信号功率控制按键SEL0
	#define GEN_SEL0_PORT								GPIOB
	#define GEN_SEL0_BIT								LL_GPIO_PIN_15
	#define GEN_SEL0_STATE								GPIO_GET_STATE(GEN_SEL0_PORT,GEN_SEL0_BIT)
	#define GEN_SEL0_WRITE								GPIO_SET_WRITE(GEN_SEL0_PORT,GEN_SEL0_BIT)
	#define GEN_SEL0_READ								GPIO_SET_READ( GEN_SEL0_PORT,GEN_SEL0_BIT)
	#define GEN_SEL0_OUT_0								GPIO_OUT_0(    GEN_SEL0_PORT,GEN_SEL0_BIT)
	#define GEN_SEL0_OUT_1								GPIO_OUT_1(    GEN_SEL0_PORT,GEN_SEL0_BIT)
	#define GEN_SEL0_OUT_C

	//===信号功率控制按键SEL1
	#define GEN_SEL1_PORT								GPIOB
	#define GEN_SEL1_BIT								LL_GPIO_PIN_14
	#define GEN_SEL1_STATE								GPIO_GET_STATE(GEN_SEL1_PORT,GEN_SEL1_BIT)
	#define GEN_SEL1_WRITE								GPIO_SET_WRITE(GEN_SEL1_PORT,GEN_SEL1_BIT)
	#define GEN_SEL1_READ								GPIO_SET_READ( GEN_SEL1_PORT,GEN_SEL1_BIT)
	#define GEN_SEL1_OUT_0								GPIO_OUT_0(    GEN_SEL1_PORT,GEN_SEL1_BIT)
	#define GEN_SEL1_OUT_1								GPIO_OUT_1(    GEN_SEL1_PORT,GEN_SEL1_BIT)
	#define GEN_SEL1_OUT_C
	
	//===信号功率控制按键SEL2
	#define GEN_SEL2_PORT								GPIOB
	#define GEN_SEL2_BIT								LL_GPIO_PIN_13
	#define GEN_SEL2_STATE								GPIO_GET_STATE(GEN_SEL2_PORT,GEN_SEL2_BIT)
	#define GEN_SEL2_WRITE								GPIO_SET_WRITE(GEN_SEL2_PORT,GEN_SEL2_BIT)
	#define GEN_SEL2_READ								GPIO_SET_READ( GEN_SEL2_PORT,GEN_SEL2_BIT)
	#define GEN_SEL2_OUT_0								GPIO_OUT_0(    GEN_SEL2_PORT,GEN_SEL2_BIT)
	#define GEN_SEL2_OUT_1								GPIO_OUT_1(    GEN_SEL2_PORT,GEN_SEL2_BIT)
	#define GEN_SEL2_OUT_C

	//===信号功率控制按键SEL3
	#define GEN_SEL3_PORT								GPIOC
	#define GEN_SEL3_BIT								LL_GPIO_PIN_0
	#define GEN_SEL3_STATE								GPIO_GET_STATE(GEN_SEL3_PORT,GEN_SEL3_BIT)
	#define GEN_SEL3_WRITE								GPIO_SET_WRITE(GEN_SEL3_PORT,GEN_SEL3_BIT)
	#define GEN_SEL3_READ								GPIO_SET_READ( GEN_SEL3_PORT,GEN_SEL3_BIT)
	#define GEN_SEL3_OUT_0								GPIO_OUT_0(    GEN_SEL3_PORT,GEN_SEL3_BIT)
	#define GEN_SEL3_OUT_1								GPIO_OUT_1(    GEN_SEL3_PORT,GEN_SEL3_BIT)
	#define GEN_SEL3_OUT_C

	//===信号功率控制按键SEL4
	#define GEN_SEL4_PORT								GPIOC
	#define GEN_SEL4_BIT								LL_GPIO_PIN_1
	#define GEN_SEL4_STATE								GPIO_GET_STATE(GEN_SEL4_PORT,GEN_SEL4_BIT)
	#define GEN_SEL4_WRITE								GPIO_SET_WRITE(GEN_SEL4_PORT,GEN_SEL4_BIT)
	#define GEN_SEL4_READ								GPIO_SET_READ( GEN_SEL4_PORT,GEN_SEL4_BIT)
	#define GEN_SEL4_OUT_0								GPIO_OUT_0(    GEN_SEL4_PORT,GEN_SEL4_BIT)
	#define GEN_SEL4_OUT_1								GPIO_OUT_1(    GEN_SEL4_PORT,GEN_SEL4_BIT)
	#define GEN_SEL4_OUT_C
	
	//===信号功率控制按键SEL5
	#define GEN_SEL5_PORT								GPIOC
	#define GEN_SEL5_BIT								LL_GPIO_PIN_2
	#define GEN_SEL5_STATE								GPIO_GET_STATE(GEN_SEL5_PORT,GEN_SEL5_BIT)
	#define GEN_SEL5_WRITE								GPIO_SET_WRITE(GEN_SEL5_PORT,GEN_SEL5_BIT)
	#define GEN_SEL5_READ								GPIO_SET_READ( GEN_SEL5_PORT,GEN_SEL5_BIT)
	#define GEN_SEL5_OUT_0								GPIO_OUT_0(    GEN_SEL5_PORT,GEN_SEL5_BIT)
	#define GEN_SEL5_OUT_1								GPIO_OUT_1(    GEN_SEL5_PORT,GEN_SEL5_BIT)
	#define GEN_SEL5_OUT_C
	
	//===信号功率控制按键SEL6
	#define GEN_SEL6_PORT								GPIOC
	#define GEN_SEL6_BIT								LL_GPIO_PIN_3
	#define GEN_SEL6_STATE								GPIO_GET_STATE(GEN_SEL6_PORT,GEN_SEL6_BIT)
	#define GEN_SEL6_WRITE								GPIO_SET_WRITE(GEN_SEL6_PORT,GEN_SEL6_BIT)
	#define GEN_SEL6_READ								GPIO_SET_READ( GEN_SEL6_PORT,GEN_SEL6_BIT)
	#define GEN_SEL6_OUT_0								GPIO_OUT_0(    GEN_SEL6_PORT,GEN_SEL6_BIT)
	#define GEN_SEL6_OUT_1								GPIO_OUT_1(    GEN_SEL6_PORT,GEN_SEL6_BIT)
	#define GEN_SEL6_OUT_C


	//===函数调用
	void Key_Init(void);
	UINT8_T Key_GetSOT(void);
	UINT8_T Key_ScanSOT(void);
	void Key_EOTSTART(void);
	void Key_EOTSTOP(void);
	void Key_BINAPass(UINT8_T isPass);
	void Key_BINBPass(UINT8_T isPass);
	void Key_BINCPass(UINT8_T isPass);
	void Key_BINDPass(UINT8_T isPass);
	void Key_GenSelInit(void);
	UINT8_T Key_GenSelScan(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*KEY_CFG_H_ */