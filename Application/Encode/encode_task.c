#include "encode_task.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EncodeTask_Init( void )
{
	EncodeLib_Init();
	EnCodeTask_EepromInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EnCodeTask_IRQTask( void )
{
	EnCodeLib_IRQTask();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void EnCodeTask_SetChannel(UINT8_T chIndex, UINT8_T isEnCode,UINT8_T isHigh)
{
	EnCodeLib_SetChannel(chIndex, isEnCode,isHigh);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T EnCodeTask_USART_MenuTask(USART_HandlerType* USARTx)
{
	UINT8_T _return = OK_0;
	//---判断主命令
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex] == CMD_ENCODE)
	{
		USARTTask_FillMode_Init(USARTx,0);
		//USARTTask_FillMode_AddByte(USARTx, CMD_ENCODE);
		//---解析命令
		switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
		{
			case CMD_ENCODE_CHA_OPEN	:
				EnCodeTask_SetChannel(1, 1,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHA_CLOSE	:
				EnCodeTask_SetChannel(1, 0,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHB_OPEN	:
				EnCodeTask_SetChannel(2, 1,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHB_CLOSE	:
				EnCodeTask_SetChannel(2, 0,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHC_OPEN	:
				EnCodeTask_SetChannel(3, 1,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHC_CLOSE	:
				EnCodeTask_SetChannel(3, 0,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHD_OPEN	:
				EnCodeTask_SetChannel(4, 1,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHD_CLOSE	:
				EnCodeTask_SetChannel(4, 0,0);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_DECODE_RST			:
				break;
			case CMD_DECODE_READ		:
				break;
			case CMD_ENCODE_CHA_HIGH	:
				EnCodeTask_SetChannel(1, 1,1);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHB_HIGH	:
				EnCodeTask_SetChannel(2, 1, 1);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHC_HIGH	:
				EnCodeTask_SetChannel(3, 1, 1);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_CHD_HIGH	:
				EnCodeTask_SetChannel(4, 1, 1);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---参数保存
				EnCodeTask_EepromWrite();
				break;
			case CMD_ENCODE_NONE:
			default:
				_return = ERROR_2;
				break;
		}

		if (_return == OK_0)
		{
			//---启动发送
			USARTTask_FillMode_WriteSTART(USARTx, 1);
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T EnCodeTask_EepromInit(void)
{
	//---读取第一次的参数
	UINT8_T tempAddrX1[2] = { 0x00,0x00 };
	//---读取第二次的参数
	UINT8_T tempAddrX2[2] = { 0x00,0x00 };
	
	//---信号源CHA的默认功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X1, tempAddrX1, 2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X2, tempAddrX2, 2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 2) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X3, tempAddrX1, 2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X1, tempAddrX1, 2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X2, tempAddrX1, 2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0] != 0xFF)
	{
		g_GenCodeChannelMask1 = tempAddrX1[0];
		g_GenCodeChannelMask2 = tempAddrX1[1];
	}	
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T EnCodeTask_EepromWrite(void)
{
	UINT8_T temp[2] = { g_GenCodeChannelMask1,g_GenCodeChannelMask2 };
	UINT8_T _return = 0;
	//---第一次保存的参数的位置
	_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X1, temp, 2);
	if (_return != 0)
	{
		_return += 1;
	}
	//---第二次保存参数的位置
	_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, ENCODE_GEN_CHANNEL_MASK1_ADDR_X2, temp, 2);
	if (_return != 0)
	{
		_return += 1;
	}
	//---第三次保存参数的位置
	_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,ENCODE_GEN_CHANNEL_MASK1_ADDR_X3, temp, 2);
	if (_return != 0)
	{
		_return += 1;
	}
	return _return;
}