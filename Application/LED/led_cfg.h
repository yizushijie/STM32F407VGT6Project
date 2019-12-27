#ifndef LED_CFG_H_
#define LED_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "systick_task.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===重新交换了一下D通道解码成功的端口
	#define LED_WORK_PORT				GPIOC
	#define LED_WORK_BIT				LL_GPIO_PIN_5
	#define LED_WORK_WRITE				GPIO_SET_WRITE(LED_WORK_PORT,LED_WORK_BIT)
	#define LED_WORK_READ				GPIO_SET_READ(LED_WORK_PORT,LED_WORK_BIT)
	#define LED_WORK_OUT_0				GPIO_OUT_0(LED_WORK_PORT,LED_WORK_BIT)
	#define LED_WORK_OUT_1				GPIO_OUT_1(LED_WORK_PORT,LED_WORK_BIT)
	#define LED_WORK_OUT_C				GPIO_OUT_C(LED_WORK_PORT,LED_WORK_BIT)
	
	//===函数定义
	void LEDWork_Init(void);
	void LEDWork_Task(void);
	void LED_Init(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*LED_CFG_H_ */
