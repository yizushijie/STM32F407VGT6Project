#ifndef MY_MALLOC_H_
#define MY_MALLOC_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//// 自定义的内存管理函数
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===内存块大小为32字节
	#define MEM_BLOCK_SIZE							32	
	//===最大管理内存 42K
	#define MEM_MAX_SIZE							10*1024	
	//===内存表大小
	#define MEM_TABLE_SIZE							( MEM_MAX_SIZE/MEM_BLOCK_SIZE )
	
	//===结构体定义
	typedef struct _MyMalloc_HandlerType			MyMalloc_HandlerType;
	//===指针结构体定义
	typedef struct _MyMalloc_HandlerType			*pMyMalloc_HandlerType;
	//===内存管理控制器
	struct _MyMalloc_HandlerType
	{
		void		(*msgInit)(void);																																				//初始化
		UINT8_T		(*msgUsedRate)(void);	  																																		//内存使用率
		UINT8_T		*msgMemPool;																																					//内存池
		UINT16_T	*msgMemMap; 																																					//内存管理状态表
		UINT8_T		msgMemReady; 																																					//内存管理是否就绪，0---未准备好，1准备好
	};

	//===外部调用参数
	extern MyMalloc_HandlerType						g_MyMallocDev;																													//在myMallco.c里面定义
	extern pMyMalloc_HandlerType					pMyMallocDev;
	//===函数调用
	void MyMemcpy(void *des, void *src, UINT32_T nSize);
	void MyMemset(void *s, UINT8_T c, UINT32_T nSize);
	void MyMemInit(void);
	UINT8_T MyMemusedRate(void);
	UINT32_T MyMemMalloc(UINT32_T size);
	UINT8_T MyMemFree(UINT32_T offset);
	void MyFree(void *pMemAddr);
	void *MyMalloc(UINT32_T nSize);
	void *MyRealloc(void *pMemAddr, UINT32_T nSize);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*MY_MALLOC_H_ */