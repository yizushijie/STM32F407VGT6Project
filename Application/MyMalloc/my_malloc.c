#include "my_malloc.h"

//__align(4)
//===SRAM内存池(4字节对齐)
#pragma pack (4)
	UINT8_T g_MemPool[MEM_MAX_SIZE] = { 0 };
#pragma pack ()
//===SRAM内存池MAP
UINT16_T g_MemMap[MEM_TABLE_SIZE] = { 0 };
//===内存表大小
//const UINT32_T g_MemTableSize = MEM_TABLE_SIZE;
//===内存分块大小
//const UINT32_T g_MemBlockSize = MEM_BLOCK_SIZE;
//===内存总大小
//const UINT32_T g_MemTotalSize = MEM_MAX_SIZE;
//===内存管理控制器
MyMalloc_HandlerType g_MyMallocDev =
{
	MyMemInit,			//内存初始化
	MyMemusedRate,		//内存使用率
	g_MemPool,			//内存池
	g_MemMap,			//内存管理状态表
	0,  				//内存管理未就绪
};

pMyMalloc_HandlerType	pMyMallocDev=&g_MyMallocDev;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 复制内存
//////输入参数: *des:目的地址;*src:源地址;n:需要复制的内存长度(字节为单位)
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void MyMemcpy(void *des, void *src, UINT32_T nSize)
{
	UINT8_T *xdes = des;
	UINT8_T *xsrc = src;
	while (nSize--)
	{
		*xdes++ = *xsrc++;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 设置内存
//////输入参数: *s:内存首地址;c :要设置的值;count:需要设置的内存大小(字节为单位)
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void MyMemset(void *s, UINT8_T c, UINT32_T nSize)
{
	UINT8_T *xs = s;
	while (nSize--)
	{
		*xs++ = c;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 内存管理初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void MyMemInit(void)
{
	//---内存状态表数据清零
	MyMemset(g_MyMallocDev.msgMemMap, 0, MEM_TABLE_SIZE * 2);
	//---内存池所有数据清零
	MyMemset(g_MyMallocDev.msgMemPool, 0, MEM_MAX_SIZE);
	//---内存管理初始化OK
	g_MyMallocDev.msgMemReady = 1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 获取内存使用率
//////输入参数:
//////输出参数: 返回值:使用率(0~100)
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T MyMemusedRate(void)
{
	UINT32_T used = 0;
	UINT32_T i = 0;
	for (i = 0;i < MEM_TABLE_SIZE;i++)
	{
		if (g_MyMallocDev.msgMemMap[i])
		{
			used++;
		}
	}
	return (used * 100) / (MEM_TABLE_SIZE);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：内存分配(内部调用)
//////输入参数: memx:所属内存块;size:要分配的内存大小(字节)
//////输出参数: 返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT32_T MyMemMalloc(UINT32_T size)
{
	signed long offset = 0;
	//---需要的内存块数
	UINT16_T nmemb = 0;
	//---连续空内存块数
	UINT16_T cmemb = 0;
	UINT32_T i = 0;
	//---未初始化,先执行初始化
	if (!g_MyMallocDev.msgMemReady)
	{
		g_MyMallocDev.msgInit();
	}
	//---不需要分配
	if (size == 0)
	{
		return 0xFFFFFFFF;
	}
	//---获取需要分配的连续内存块数
	nmemb = size / MEM_BLOCK_SIZE;
	//---是不是整数块
	if (size% MEM_BLOCK_SIZE)
	{
		nmemb++;
	}
	//---搜索整个内存控制区
	for (offset = MEM_TABLE_SIZE - 1;offset >= 0;offset--)
	{
		//---连续空内存块数增加
		if (!g_MyMallocDev.msgMemMap[offset])
		{
			cmemb++;
		}
		else
		{
			//---连续内存块清零
			cmemb = 0;
		}
		//---找到了连续nmemb个空内存块
		if (cmemb == nmemb)
		{
			//---标注内存块非空
			for (i = 0;i < nmemb;i++)
			{
				g_MyMallocDev.msgMemMap[offset + i] = nmemb;
			}
			//---返回偏移地址
			return (offset* MEM_BLOCK_SIZE);
		}
	}
	//---未找到符合分配条件的内存块
	return 0XFFFFFFFF;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：释放内存(内部调用)
//////输入参数: offset:内存地址偏移
//////输出参数: 返回值:0,释放成功;1,释放失败; 2,地址错误
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T MyMemFree(UINT32_T offset)
{
	int i = 0;
	//---未初始化,先执行初始化
	if (!g_MyMallocDev.msgMemReady)
	{
		g_MyMallocDev.msgInit();
		//---未初始化
		return 1;
	}
	//---偏移在内存池内
	if (offset < MEM_MAX_SIZE)
	{
		//---偏移所在内存块号码
		int index = offset / MEM_BLOCK_SIZE;
		//---内存块数量
		int nmemb = g_MyMallocDev.msgMemMap[index];
		//---内存块清零
		for (i = 0;i < nmemb;i++)
		{
			g_MyMallocDev.msgMemMap[index + i] = 0;
		}
		return 0;
	}
	else
	{
		//---偏移超区了.
		return 2;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 释放内存(外部调用)
//////输入参数:  pMemAddr:内存首地址
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void MyFree(void *pMemAddr)
{
	UINT32_T offset;
	if (pMemAddr == NULL)
	{
		//---地址为0
		return;
	}
	offset = (UINT32_T)pMemAddr - (UINT32_T)g_MyMallocDev.msgMemPool;
	//---释放内存
	MyMemFree(offset);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 分配内存(外部调用)
//////输入参数:  size:内存大小(字节)
//////输出参数:  返回值:分配到的内存首地址.
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void *MyMalloc(UINT32_T nSize)
{
	UINT32_T offset;
	//---申请内存大小
	offset = MyMemMalloc(nSize);
	if (offset == 0XFFFFFFFF)
	{
		//---申请失败，返回空
		return NULL;
	}
	else
	{
		return (void*)((UINT32_T)g_MyMallocDev.msgMemPool + offset);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：重新分配内存(外部调用)
//////输入参数: *pMemAddr:旧内存首地址;size:要分配的内存大小(字节)
//////输出参数: 返回值:新分配到的内存首地址.
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void *MyRealloc(void *pMemAddr, UINT32_T nSize)
{
	UINT32_T offset;
	//---申请内存大小
	offset = MyMemMalloc(nSize);
	//---校验内存是否申请成功
	if (offset == 0XFFFFFFFF)
	{
		//---申请失败，返回空
		return NULL;
	}
	else
	{
		//---拷贝旧内存内容到新内存
		MyMemcpy((void*)((UINT32_T)g_MyMallocDev.msgMemPool + offset), pMemAddr, nSize);
		//---释放旧内存
		MyFree(pMemAddr);
		//---返回新内存首地址
		return (void*)((UINT32_T)g_MyMallocDev.msgMemPool + offset);
	}
}