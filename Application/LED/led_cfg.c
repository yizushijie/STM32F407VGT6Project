#include "led_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LEDWork_Init(void)
{
	GPIOTask_Clock(LED_WORK_PORT, PERIPHERAL_CLOCK_ENABLE);

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	//---GPIO的初始化
	GPIO_InitStruct.Pin = LED_WORK_BIT;								//---对应的GPIO的引脚
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;					//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;						//---下拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif

	//---工作指示灯的初始化
	LL_GPIO_Init(LED_WORK_PORT, &GPIO_InitStruct);
	LED_WORK_OUT_0;

	//---注册闪灯程序
	/*
	SysTickTask_FuncTick(LEDWork_Task);
	*/
	SysTickTask_CreateTickTask(LEDWork_Task);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LEDWork_Task(void)
{
	static UINT32_T ledCount = 0;
	ledCount++;
	if (ledCount > 500)
	{
		ledCount = 0;
		LED_WORK_OUT_C;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void LED_Init(void)
{
	LEDWork_Init();
}