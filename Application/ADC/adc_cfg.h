#ifndef ADC_CFG_H_
#define ADC_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "delay_task.h"
	#include "gpio_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	///////PC0---ADCChannel10
	///////PC1---ADCChannel11
	///////PC2---ADCChannel12
	///////PC3---ADCChannel13
	///////ADC1和ADC3才能发起DMA请求
	///////ADC部分的驱动需要依据不同的需要进行更改，不同的项目需要，配置方式不一致，需要重新设计修改
	#define  ADC_CHANNEL_NUM					2
	#define	 ADC_CHANNEL_SIZE					18
	#define  ADC_CHANNEL_MAX_SIZE				( ADC_CHANNEL_NUM*ADC_CHANNEL_SIZE )
	
	//===定义结构体
	typedef struct _ADCASK_HandlerType			ADCASK_HandlerType;
	//===定义指针结构体
	typedef struct _ADCASK_HandlerType			*pADCASK_HandlerType;
	//===结构体变量
	struct _ADCASK_HandlerType
	{
		UINT8_T  msgFlag;									//---0未转换 1 转换完成
		UINT16_T msgAChannelVal;							//---通道A的值
		UINT16_T msgBChannelVal;							//---通道B的值
		UINT16_T msgADCConvVal[ADC_CHANNEL_MAX_SIZE];		//---数据存储的缓存区
	};

	//===外部调用接口
	extern ADCASK_HandlerType	g_ABChannelADC;
	extern pADCASK_HandlerType	pABChannelADC;
	extern ADCASK_HandlerType	g_CDChannelADC;
	extern pADCASK_HandlerType	pCDChannelADC;

	//===函数定义
	UINT8_T ADC_GPIO_Init(void);
	UINT8_T ADC_Clock(ADC_TypeDef *ADCx, UINT8_T isEnable);
	UINT8_T ADC_ADC1_Init(void);
	UINT8_T ADC_StartCalibration(ADC_TypeDef *ADCx);
	UINT8_T ADC_ADC1_DMA_Init(void);
	UINT8_T ADC_ADC1_DMA_Restart(void);
	UINT8_T ADC_ADC1_DMA_Stop(void);
	UINT8_T ADC_ADC3_Init(void);
	UINT8_T ADC_ADC3_DMA_Init(void);
	UINT8_T ADC_ADC3_DMA_Restart(void);
	UINT8_T ADC_ADC3_DMA_Stop(void);
	UINT8_T ADC_SoftStart(ADC_TypeDef *ADCx1, ADC_TypeDef *ADCx2, ADC_TypeDef *ADCx3);
	UINT8_T ADC_ADC1Task_Init(void);
	UINT8_T ADC_ADC3Task_Init(void);
	UINT8_T ADC_ADCTask_Init(void);
	UINT8_T ADC_ADC_Init(void);
	UINT8_T ADC_ADCTask_START(ADC_TypeDef *ADCx);
	UINT8_T ADC_ADCTask_STOP(ADC_TypeDef *ADCx);
	UINT8_T ADC_HandleChannelVal(ADCASK_HandlerType *ADCASKx);
	UINT16_T ADC_GetChipPower(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*ADC_CFG_H_ */
