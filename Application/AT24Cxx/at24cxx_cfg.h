#ifndef AT24CXX_CFG_H_
#define AT24CXX_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
////如果在单页的范围内，存储地址累加，若超过该页的最大地址，存储地址回到页开始处。
////所以对于连续读和连续写而言，最大的操作字节数为8。
////若需要操作的字节内容超过8字节，则需要进行翻页操作，即写入下一页的起始存储地址
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "gpio_task.h"
	#include "i2c_task.h"
	#include "my_malloc.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===支持的系列型号
	#define AT24C01							0																																		//---16PX8B		---16页，每页8字节
	#define AT24C02							1																																		//---32PX8B		---32页，每页8字节
	#define AT24C04							2																																		//---32PX16B	---32页，每页16字节
	#define AT24C08							3																																		//---64PX16B	---64页，每页16字节
	#define AT24C16							4																																		//---128PX16B	---128页，每页16字节
	#define AT24C32							5																																		//---128PX32B	---128页，每页32字节
	#define AT24C64							6																																		//---256PX32B	---256页，每页32字节
	#define AT24C128						7																																		//---256PX64B	---256页，每页64字节
	#define AT24C256						8																																		//---512PX64B	---512页，每页64字节
	#define AT24C512						9																																		//---512PX128B	---512页，每页128字节
	
	//===定义是否使用全局变量，针对ram小的芯片，无法使用自定义内存管理函数
	//#define USE_GOBAL_RAM
	//===定义宏开关
	#ifdef USE_GOBAL_RAM
		//===定义AT24Cxx的每页直接最大值
		#define	AT24CXX_PAGE_BYTE_MAX_SIZE		128
	#endif
	//===结构体定义
	typedef struct _AT24CXX_HandlerType AT24CXX_HandlerType;
	//===指针结构体定义
	typedef struct _AT24CXX_HandlerType *pAT24CXX_HandlerType;

	//===I2C模式使用硬件写保护模式
	#define AT24CXX_I2C_USE_HWWP

	//===AT24Cxx的数据结构体
	struct _AT24CXX_HandlerType
	{
		UINT8_T  msgAT24CXXType;																																					//---使用的设备
		UINT8_T  msgAT24CXXPageByte;																																				//---每页字节数
		UINT16_T msgAT24CXXPageNum;																																					//---页数
		#ifdef USE_GOBAL_RAM
			UINT8_T msgPageByteBuffer[AT24CXX_PAGE_BYTE_MAX_SIZE];																													//---定义缓存区
		#endif
#ifdef AT24CXX_I2C_USE_HWWP
		GPIO_HandlerType msgWP;																																						//---写保护控制端口，0---写保护不是能；1---写保护使能
#endif
		I2C_HandlerType msgI2C;																																						//---使用的I2C
		void(*msgDelayms)(UINT32_T delay);																																			//---毫秒延时函数,编程结束后需要等待5ms
	};

	//===定义的任务函数
	#define AT24CXX_TASK_ONE		pAT24cxxDevice0
	#define AT24CXX_TASK_TWO		0
	#define AT24CXX_TASK_THREE		0

	//===外部调用接口
	extern AT24CXX_HandlerType		g_AT24cxxDevice0;
	extern pAT24CXX_HandlerType		pAT24cxxDevice0;

	//===函数定义
	UINT8_T AT24CXX_I2C_Init(AT24CXX_HandlerType *AT24CXXx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHWI2C);
	UINT8_T AT24CXX_I2C_Device0_Init(AT24CXX_HandlerType *AT24CXXx);
	UINT8_T AT24CXX_I2C_Device1_Init(AT24CXX_HandlerType *AT24CXXx);
	UINT8_T AT24CXX_I2C_Device2_Init(AT24CXX_HandlerType *AT24CXXx);
	UINT8_T AT24CXX_I2C_DeInit(AT24CXX_HandlerType *AT24CXXx);
	UINT8_T AT24CXX_SWI2C_WriteOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T val);
	UINT8_T AT24CXX_SWI2C_WritePageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T pageAddr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_HWI2C_WriteOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T val);
	UINT8_T AT24CXX_HWI2C_WritePageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T pageAddr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_I2C_WriteOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T val);
	UINT8_T AT24CXX_I2C_WritePageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_I2C_WriteData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_SWI2C_ReadOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal);
	UINT8_T AT24CXX_SWI2C_ReadPageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_HWI2C_ReadOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal);
	UINT8_T AT24CXX_HWI2C_ReadPageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_I2C_ReadOneByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal);
	UINT8_T AT24CXX_I2C_ReadPageByte(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_I2C_ReadData(AT24CXX_HandlerType *AT24CXXx, UINT16_T addr, UINT8_T *pVal, UINT16_T length);
	UINT8_T AT24CXX_I2C_EraseChip(AT24CXX_HandlerType *AT24CXXx);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /* AT24CXX_CFG_H_ */