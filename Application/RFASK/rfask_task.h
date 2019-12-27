#ifndef RFASK_TASK_H_
#define RFASK_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "rfask_lib.h"
	#include "timer_task.h"
	#include "wm8510_task.h"
	#include "usart_task.h"
	#include "power_task.h"
	#include "adc_task.h"
	#include "led_task.h"
	#include "key_task.h"
	#include "decode_task.h"
	#include "rfask_eeprom.h" 
	//////////////////////////////////////////////////////////////////////////////////////
	//===获取编译时间和版本信息
	#define CMD_RFASK_CMD1_VERSIOM									0x00
	//===设置WM8510的输出---主命令
	#define CMD_RFASK_CMD1_FREQ_WM8510								0x01
	//===设置WM8510输出频率
	#define CMD_RFASK_CMD2_SET_WM8510								0x01
	//===获取WM8510的输出频率
	#define CMD_RFASK_CMD2_GET_WM8510								0x02
	//===复位WM8510
	#define CMD_RFASK_CMD2_RESET_WM8510								0x03
	//===时钟输出通道A的使能控制
	#define CMD_RFASK_CMD2_CHANNELA_WM8510							0x04
	//===时钟输出通道B的使能控制
	#define CMD_RFASK_CMD2_CHANNELB_WM8510							0x05
	//===时钟输出通道C的使能控制
	#define CMD_RFASK_CMD2_CHANNELC_WM8510							0x06
	//===时钟输出通道D的使能控制
	#define CMD_RFASK_CMD2_CHANNELD_WM8510							0x07
	//===时钟所有输出通道的使能控制
	#define CMD_RFASK_CMD2_CHANNELS_WM8510							0x08
	//===设置默认频率点输出
	#define CMD_RFASK_CMD1_YSEL_FREQ								0x02
	//===设置预设频率点1
	#define CMD_RFASK_CMD2_YSEL1_FREQ_SET							0x01
	//===设置预设频率点2
	#define CMD_RFASK_CMD2_YSEL2_FREQ_SET							0x02
	//===设置预设频率点3
	#define CMD_RFASK_CMD2_YSEL3_FREQ_SET							0x03
	//===设置预设频率点4
	#define CMD_RFASK_CMD2_YSEL4_FREQ_SET							0x04
	//===获取预设频率点1
	#define CMD_RFASK_CMD2_YSEL1_FREQ_GET							0x05
	//===获取预设频率点2
	#define CMD_RFASK_CMD2_YSEL2_FREQ_GET							0x06
	//===获取预设频率点3
	#define CMD_RFASK_CMD2_YSEL3_FREQ_GET							0x07
	//===获取预设频率点4
	#define CMD_RFASK_CMD2_YSEL4_FREQ_GET							0x08
	//===设置所有预设频率点
	#define CMD_RFASK_CMD2_YSEL_FREQ_SET							0x09
	//===获取所有预设频率点
	#define CMD_RFASK_CMD2_YSEL_FREQ_GET							0x0A
	//===设置预设频率点1输出
	#define CMD_RFASK_CMD2_YSEL1_FREQ_OUT							0x0B
	//===设置预设频率点2输出
	#define CMD_RFASK_CMD2_YSEL2_FREQ_OUT							0x0C
	//===设置预设频率点3输出
	#define CMD_RFASK_CMD2_YSEL3_FREQ_OUT							0x0D
	//===设置预设频率点4输出
	#define CMD_RFASK_CMD2_YSEL4_FREQ_OUT							0x0E
	//===频率电流测试，基本参数的配置
	#define CMD_RFASK_CMD1_FREQ_CURRENT								0x03
	//===设备类型
	#define CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_TYPE_GET				0x01
	//===采样电阻
	#define CMD_RFASK_CMD1_FREQ_CURRENT_SAMPLE_RES_GET				0x02
	//===放大倍数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_AMP_TIMES_GET				0x03
	//===设备类型
	#define CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_TYPE_SET				0x04
	//===采样电阻
	#define CMD_RFASK_CMD1_FREQ_CURRENT_SAMPLE_RES_SET				0x05
	//===放大倍数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_AMP_TIMES_SET				0x06
	//===设备的基本参数获取
	#define CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_GET					0x07
	//===设备的基本参数设置
	#define CMD_RFASK_CMD1_FREQ_CURRENT_DEVICE_SET					0x08
	//===第一个频率电流扫描电压点	
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_ONE					0x04
	//===第二个频率电流扫描电压点
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_TWO					0x05
	//===获取频率参数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_GET				0x01
	//===设置频率参数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_FREQ_SET				0x02
	//===获取电流参数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_GET			0x03
	//===设置电流参数
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_CURRENT_SET			0x04
	//===频率电流扫描工作
	#define CMD_RFASK_CMD1_FREQ_CURRENT_POINT_DO					0x05

	//===允许连续相等的点的个数	
	#define RFASK_FREQ_CURRENT_CONT_EQUAL_POINT_MAX_NUM				3
	
	//===函数定义
	void     RFASKTask_Init(RFASK_HandlerType *rfask, AT24CXX_HandlerType* AT24CXXx);
	UINT32_T RFASKTask_CalcXTAL(RFASK_HandlerType *rfask, UINT32_T rfX100MHz);
	UINT8_T  RFASKTask_SetDeviceType(RFASK_HandlerType *rfask, UINT8_T deviceType);
	UINT32_T RFASKTask_GetDeviceType(RFASK_HandlerType *rfask);
	UINT32_T RFASKTask_SetClockFreq(RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, UINT32_T rfX100MHz);
	UINT8_T  RFASKTask_WM8510Task(USART_HandlerType*USARTx, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx);
	UINT8_T  RFASKTask_YSELTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx);
	UINT8_T  RFASKTask_ActivateSites(RFASK_HandlerType *rfask, UINT8_T activateSites);
	UINT8_T  RFASKTask_SitesCurrent(RFASK_HandlerType *rfask);
	UINT8_T  RFASKTask_SetFreqCurrentPointCmd(FREQ_CURRENT_HandlerType *rfaskFreqCurrent, UINT8_T cmd);
	UINT8_T  RFASKTask_FreqCurrentScan(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, FREQ_CURRENT_HandlerType *rfaskFreqCurrent, WM8510_HandlerType *WM8510x);
	UINT8_T  RFASKTask_HandlerTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx);
	UINT8_T  RFASKTask_ScanYSEL(void);
	UINT8_T  RFASKTask_Task(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, AT24CXX_HandlerType* AT24CXXx);
	UINT8_T  RFASKTask_KeyTask(USART_HandlerType*USARTx, RFASK_HandlerType *rfask, WM8510_HandlerType *WM8510x, UINT8_T activateSites);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*RFASK_TASK_H_ */