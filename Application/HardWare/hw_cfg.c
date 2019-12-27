#include "hw_cfg.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void HardWare_Init(void)
{
	//---使能GPIO的时钟
	GPIOTask_Clock(RST_HEN_PORT, 1);
	GPIOTask_Clock(RST_LEN_PORT, 1);
	GPIOTask_Clock(RST_NEN_PORT, 1);
	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;														//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;												//---GPIO的速度---低速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;											//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;															//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;														//---端口复用模式
#endif
	//---LEN---输出为低
	GPIO_InitStruct.Pin = RST_LEN_BIT;
	LL_GPIO_Init(RST_LEN_PORT, &GPIO_InitStruct);
	RST_LEN_OUT_0;
	//---HEN---输出为低
	GPIO_InitStruct.Pin = RST_HEN_BIT;
	LL_GPIO_Init(RST_HEN_PORT, &GPIO_InitStruct);
	RST_HEN_OUT_0;
	//---LEN---输出为高
	GPIO_InitStruct.Pin = RST_NEN_BIT;
	LL_GPIO_Init(RST_NEN_PORT, &GPIO_InitStruct);
	RST_NEN_OUT_0;
}