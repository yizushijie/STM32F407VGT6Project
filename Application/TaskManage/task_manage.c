#include"task_manage.h"

//===变量定义
VLTUINT8_T 	g_TaskMangeIndex= TASK_MANAGE_ISP_USART;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：第1个任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage_1()
{
	return ISPTask_USARTCmd_Task(pIspDevice0, pUsart1);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：第2个任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage_2()
{
	return JTAGTask_USARTCmd_Task(pJtagDevice0, pUsart1);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：第2个任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage_3()
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：第2个任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage_4()
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：第2个任务
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage_Step()
{
	if (g_TaskMangeIndex == TASK_MANAGE_ISP_USART)
	{
		return Task_Manage_1();
	}
	else if (g_TaskMangeIndex == TASK_MANAGE_JTAG_USART)
	{
		return Task_Manage_2();
	}
	else if (g_TaskMangeIndex == TASK_MANAGE_HVPP_USART)
	{
		return Task_Manage_3();
	}
	else if (g_TaskMangeIndex == TASK_MANAGE_HVPP_USART)
	{
		return Task_Manage_4();
	}
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：任务管理函数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T Task_Manage(void)
{
	//---判断接收是否完成
	if (USARTTask_GetReadState(pUsart1) == 1)
	{
		//---CRC的校验和设备ID校验
		if ((USARTTask_CRCTask_Read(pUsart1) == OK_0) && (USARTTask_DeviceID(pUsart1) == OK_0))
		{
			if ((pUsart1->msgRxdHandler.pMsgVal[pUsart1->msgCmdIndex + pUsart1->msgIndexOffset]>= CMD_ISP_BASE_CMD)&&
				(pUsart1->msgRxdHandler.pMsgVal[pUsart1->msgCmdIndex + pUsart1->msgIndexOffset] <=CMD_ISP_END_CMD))
			{
				g_TaskMangeIndex = TASK_MANAGE_ISP_USART;
			}
			else if ((pUsart1->msgRxdHandler.pMsgVal[pUsart1->msgCmdIndex + pUsart1->msgIndexOffset] >= CMD_JTAG_BASE_CMD) &&
				(pUsart1->msgRxdHandler.pMsgVal[pUsart1->msgCmdIndex + pUsart1->msgIndexOffset] <= CMD_JTAG_END_CMD))
			{
				g_TaskMangeIndex = TASK_MANAGE_JTAG_USART;
			}
			else
			{
				g_TaskMangeIndex = TASK_MANAGE_NONE;
			}
			//---命令任务步序
			Task_Manage_Step();
		}
		else
		{
			//---发生CRC校验错误
			USART_Printf(pUsart1, "=>>串口%d:发生CRC校验错误<<=\r\n", (pUsart1->msgIndex - 1));
		}
		return USARTTask_Read_Init(pUsart1);
	}
	return USARTTask_TimeOVFTask(pUsart1);
}