#ifndef W25QXX_CFG_H_
#define W25QXX_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	#include "spi_task.h"
	#include "systick_task.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////
	///擦除芯片的时候不要用全部擦除指令，全部擦除的指令，芯片耗时太长，建议选择用块擦除指令
	//////////////////////////////////////////////////////////////////////////////////////

	//===芯片ID
	#define W25Q80 								0xEF13
	#define W25Q16 								0xEF14
	#define W25Q32 								0xEF15
	#define W25Q64 								0xEF16
	
	//===命令指令集
	#define W25QXX_CMD_ENABLE_WRITE				0x06
	#define W25QXX_CMD_ENABLE_VOLATILE_WRITE	0x50
	#define W25QXX_CMD_DISABLE_WRITE			0x04
	#define W25QXX_CMD_READ_REG_SR1				0x05
	#define W25QXX_CMD_READ_REG_SR2				0x35
	#define W25QXX_CMD_WRITE_REG_SR				0x01
	#define W25QXX_CMD_READ_DATA				0x03
	#define W25QXX_CMD_READ_FAST				0x0B
	#define W25QXX_CMD_READ_DUAL_FAST			0x3B
	#define W25QXX_CMD_PAGE_PROGRAM				0x02
	#define W25QXX_CMD_ERASE_SECTOR				0x20
	#define W25QXX_CMD_ERASE_BLOCK_64KB			0xD8
	#define W25QXX_CMD_ERASE_BLOCK_32KB			0x52
	#define W25QXX_CMD_ERASE_CHIP				0xC7
	#define W25QXX_CMD_ERASE_SUSPEND			0x75
	#define W25QXX_CMD_ERASE_RESUME				0x7A
	#define W25QXX_CMD_POWER_DOWN				0xB9
	#define W25QXX_CMD_RELEASE_POWER_DOWN		0xAB
	#define W25QXX_CMD_DEVICE_ID				0xAB
	#define W25QXX_CMD_MANUFACT_DEVICE_ID		0x90
	#define W25QXX_CMD_READ_UNIQUE_ID			0x4B
	#define W25QXX_CMD_JEDEC_ID					0x9F
	#define W25QXX_CMD_ERASE_SECURITY_REG		0x44
	#define W25QXX_CMD_PROGRAM_SECURITY_REG		0x42
	#define W25QXX_CMD_READ_SECURITY_REG		0x48
	#define W25QXX_CMD_ENABLE_RESET				0x66
	#define W25QXX_CMD_RESET					0x99
	
	//===地址
	#define W25QXX_SECURITY_REG1				0x001000
	#define W25QXX_SECURITY_REG2				0x002000
	#define W25QXX_SECURITY_REG3				0x003000
	
	//===每页的字节数
	#define W25QXX_PAGE_BYTE_SIZE				256
	#define W25QXX_PAGE_NUM_MASK				0xFFFFFF00
	#define W25QXX_PAGE_BYTE_MASK				0xFF
	
	//===每个扇区的字节数
	#define W25QXX_SECTOR_BYTE_SIZE				4096
	#define W25QXX_SECTOR_NUM_MASK				0xFFFF0000
	#define W25QXX_SECTOR_BYTE_MASK				0xFFFF

	//===32Kb大小的块字节数
	#define W25QXX_BLOCK_32KB_BYTE_SIZE			4096*32
	#define W25QXX_BLOCK_32KB_NUM_MASK			0xFFFE0000
	#define W25QXX_BLOCK_32KB_BYTE_MASK			0x1FFFF

	//===64Kb大小的块字节数
	#define W25QXX_BLOCK_64KB_BYTE_SIZE			4096*64
	#define W25QXX_BLOCK_64KB_NUM_MASK			0xFFFC0000
	#define W25QXX_BLOCK_64KB_BYTE_MASK			0x3FFFF

	//===定义结构体
	typedef struct _W25QXX_HandlerType			W25QXX_HandlerType;
	//===定义指针结构体
	typedef	struct _W25QXX_HandlerType			*pW25QXX_HandlerType;

	//===SPI模式使用硬件写保护模式
	#define WM25QXX_SPI_USE_HWWP

	//===结构定义
	struct _W25QXX_HandlerType
	{
		UINT8_T		msgInit;													//---判断是否初始化过了 0---未初始化，1---初始化
		UINT8_T		msgBuffer[W25QXX_SECTOR_BYTE_SIZE];							//---数据缓存区
		UINT32_T	msgEraseDelayMS;											//---全部擦除后的延时时间，
		UINT16_T	msgEraseSectorDelayMS;										//---扇区的延时时间
		UINT16_T	msgErase32KbBlockDelayMS;									//---32Kb块的延时时间
		UINT16_T	msgErase64KbBlockDelayMS;									//---64Kb块的延时时间
		UINT16_T	msgChipID;													//---设备ID信息
#ifdef WM25QXX_SPI_USE_HWWP
		GPIO_HandlerType msgWP;													//---写保护控制端口，0---写保护使能；1---写保护不使能
#endif
		void(*msgDelayms)(UINT32_T delay);									//---延时参数
		SPI_HandlerType msgSPI;													//---使用的SPI模式
	};

	//===任务函数
	#define W25QXX_TASK_ONE						pW25qxxDevice0
	#define W25QXX_TASK_TWO						0
	#define W25QXX_TASK_THREE					0
	
	//===外部调用接口
	extern W25QXX_HandlerType					g_W25qxxDevice0;
	extern pW25QXX_HandlerType					pW25qxxDevice0;

	//===函数定义
	UINT8_T W25QXX_SPI_Init(W25QXX_HandlerType *W25Qx, void(*pFuncDelayus)(UINT32_T delay), void(*pFuncDelayms)(UINT32_T delay), UINT32_T(*pFuncTimerTick)(void), UINT8_T isHW);
	UINT8_T W25QXX_SPI_ReadRegSR1(W25QXX_HandlerType* W25Qx, UINT8_T isAutoInit);
	UINT8_T W25QXX_SPI_ReadRegSR2(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_WriteRegSR1(W25QXX_HandlerType *W25Qx, UINT8_T cmd, UINT8_T isAutoInit);
	void W25QXX_SPI_WriteRegSR2(W25QXX_HandlerType *W25Qx, UINT8_T cmd, UINT8_T isAutoInit);
	void W25QXX_SPI_EnableWrite(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_DisableWrite(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	UINT16_T W25QXX_SPI_ReadID(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	UINT8_T W25QXX_SPI_WaitBusy(W25QXX_HandlerType *W25Qx, UINT32_T timeOut, UINT8_T isAutoInit);
	void W25QXX_SPI_ReadData(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_ReadFast(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_WritePage(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseChip(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseResume(W25QXX_HandlerType* W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseSuspend(W25QXX_HandlerType* W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseSector(W25QXX_HandlerType *W25Qx, UINT32_T sectorAddr, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseBlock32KB(W25QXX_HandlerType *W25Qx, UINT32_T blockAddr, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseBlock64KB(W25QXX_HandlerType *W25Qx, UINT32_T blockAddr, UINT8_T isAutoInit);
	void W25QXX_SPI_PowerDown(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_WakeUp(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_ReadUniqueIDNumber(W25QXX_HandlerType *W25Qx, UINT8_T *pVal, UINT8_T isAutoInit);
	void W25QXX_SPI_ReadJEDECID(W25QXX_HandlerType *W25Qx, UINT8_T *pVal, UINT8_T isAutoInit);
	void W25QXX_SPI_EraseSecurityReg(W25QXX_HandlerType *W25Qx, UINT32_T regAddr, UINT8_T isAutoInit);
	void W25QXX_SPI_ProgramSecurityReg(W25QXX_HandlerType *W25Qx, UINT32_T regAddr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_ReadSecurityReg(W25QXX_HandlerType *W25Qx, UINT32_T regAddr, UINT8_T *pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_EnableReset(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_Reset(W25QXX_HandlerType *W25Qx, UINT8_T isAutoInit);
	void W25QXX_SPI_WriteNoCheck(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T* pVal, UINT16_T length, UINT8_T isAutoInit);
	void W25QXX_SPI_WriteAndCheck(W25QXX_HandlerType *W25Qx, UINT32_T addr, UINT8_T* pVal, UINT16_T length, UINT8_T isAutoInit);

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*W25QXX_CFG_H_ */
