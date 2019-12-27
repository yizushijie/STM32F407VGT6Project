#include "hmc472_task.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472Task_Init( void )
{
	HMC472Lib_Init();
	HMC472Task_EepromInit();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472Task_SetGain( UINT8_T val )
{
	HMC472Lib_SetGain( val );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_SetGenGain( UINT8_T genCh, UINT8_T gainVal )
{
	return HMC472Lib_SetGenGain( genCh, gainVal );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_SetRFGain( UINT8_T rfCH, UINT8_T gainVal )
{
	return HMC472Lib_SetRFGain( rfCH, gainVal );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472Task_ReadGen( UINT8_T genCH )
{
	return HMC472Lib_ReadGen( genCH );
}


///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T  HMC472Task_SetGen( UINT8_T genCH, UINT16_T genVal )
{
	return HMC472Lib_SetGen( genCH, genVal );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472Task_ReadHMC472Gen( UINT8_T genCH )
{
	return HMC472Lib_ReadHMC472Gen( genCH );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472Task_ReadHMC472RF( UINT8_T rfCH )
{
	return HMC472Lib_ReadHMC472RF( rfCH );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472Task_ReadRFGen( UINT8_T genCH, UINT8_T rfCH )
{
	return HMC472Lib_ReadRFGen( genCH, rfCH );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_SetRFGen( UINT8_T genCH, UINT8_T rfCH, UINT16_T rfGenPower )
{
	return HMC472Lib_SetRFGen( genCH, rfCH, rfGenPower );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_GetVersion( UINT8_T *pVersion,UINT8_T length )
{
	return HMC472Lib_GetVersion( pVersion,length );
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_USART_MenuTask( USART_HandlerType*USARTx )
{
	UINT8_T _return = OK_0;
	UINT16_T gainVal = 0;
	//---判断主命令
	if (USARTx->msgRxdHandler.pMsgVal[USARTx->msgCmdIndex]==CMD_HMC472)
	{
		USARTTask_FillMode_Init( USARTx,1 );
		USARTTask_FillMode_AddByte(USARTx,CMD_HMC472);
		//---解析命令
		switch (USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex])
		{
			case CMD_HMC472_READ_CHA		:
				gainVal = HMC472Task_ReadGen( HMC472_GEN_CHA );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte(USARTx, (UINT8_T)(gainVal>>8));
				USARTTask_FillMode_AddByte(USARTx, (UINT8_T)(gainVal));
				break;
			case CMD_HMC472_WRITE_CHA		:
				gainVal = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex ];
				gainVal = ( gainVal << 8 ) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
				_return = HMC472Task_SetGen( HMC472_GEN_CHA, gainVal );
				USARTTask_FillMode_AddByte( USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex] );
				USARTTask_FillMode_AddByte( USARTx, _return );
				//---保存参数
				HMC472Task_EepromWrite(GEN_CHA_DBM_ADDR_X1,HMC472Task_ReadGen(HMC472_GEN_CHA));
				break;
			case CMD_HMC472_READ_CHB		:
				gainVal = HMC472Task_ReadGen( HMC472_GEN_CHB );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_CHB		:
				gainVal = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex ];
				gainVal = ( gainVal << 8 ) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
				_return = HMC472Task_SetGen( HMC472_GEN_CHB, gainVal );
				USARTTask_FillMode_AddByte( USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex] );
				USARTTask_FillMode_AddByte( USARTx, _return );
				//---保存参数
				HMC472Task_EepromWrite(GEN_CHB_DBM_ADDR_X1,HMC472Task_ReadGen(HMC472_GEN_CHB));
				break;
			case CMD_HMC472_READ_CHC		:
				gainVal = HMC472Task_ReadGen( HMC472_GEN_CHC );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal ) );
				break;								
			case CMD_HMC472_WRITE_CHC		:
				gainVal = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex ];
				gainVal = ( gainVal << 8 ) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
				_return = HMC472Task_SetGen( HMC472_GEN_CHC, gainVal );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(GEN_CHC_DBM_ADDR_X1,HMC472Task_ReadGen(HMC472_GEN_CHC));
				break;
			case CMD_HMC472_READ_CHD		:
				gainVal = HMC472Task_ReadGen( HMC472_GEN_CHD );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_CHD		:
				gainVal = USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex ];
				gainVal = ( gainVal << 8 ) + USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex + 1];
				_return = HMC472Task_SetGen( HMC472_GEN_CHD,gainVal);
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(GEN_CHD_DBM_ADDR_X1,HMC472Task_ReadGen(HMC472_GEN_CHD));
				break;
			case CMD_HMC472_READ_GEN_CHA	:
				gainVal = HMC472Task_ReadHMC472Gen( HMC472_GEN_CHA );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_GEN_CHA	:
				_return = HMC472_SetGenGain( HMC472_RF_CHA, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte( USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex] );
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_GEN_CHA_DBM_ADDR_X1,HMC472Task_ReadHMC472Gen(HMC472_GEN_CHA));
				break;
			case CMD_HMC472_READ_GEN_CHB	:
				gainVal = HMC472Task_ReadHMC472Gen( HMC472_GEN_CHB );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_GEN_CHB	:
				_return = HMC472Task_SetGenGain( HMC472_RF_CHB, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_GEN_CHB_DBM_ADDR_X1,HMC472Task_ReadHMC472Gen(HMC472_GEN_CHB));
				break;
			case CMD_HMC472_READ_GEN_CHC	:
				gainVal = HMC472Task_ReadHMC472Gen( HMC472_GEN_CHC );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_GEN_CHC	:
				_return = HMC472Task_SetGenGain( HMC472_RF_CHC, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );				
				USARTTask_FillMode_AddByte( USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex] );
				USARTTask_FillMode_AddByte( USARTx, _return );
				//---保存参数
				HMC472Task_EepromWrite(HMC472_GEN_CHC_DBM_ADDR_X1,HMC472Task_ReadHMC472Gen(HMC472_GEN_CHC));
				break;
			case CMD_HMC472_READ_GEN_CHD	:
				gainVal = HMC472Task_ReadHMC472Gen( HMC472_GEN_CHD );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_GEN_CHD	:
				_return = HMC472Task_SetGenGain( HMC472_RF_CHD, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte( USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex] );
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_GEN_CHD_DBM_ADDR_X1,HMC472Task_ReadHMC472Gen(HMC472_GEN_CHD));
				break;
			case CMD_HMC472_READ_RF_CHA		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHA );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHA	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHA, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHA_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHA));
				break;
			case CMD_HMC472_READ_RF_CHB		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHB );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHB	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHB, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHB_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHB));
				break;
			case CMD_HMC472_READ_RF_CHC		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHC );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHC	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHC, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHC_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHC));
				break;
			case CMD_HMC472_READ_RF_CHD		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHD );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;								
			case CMD_HMC472_WRITE_RF_CHD	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHD, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHD_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHD));
				break;
			case CMD_HMC472_READ_RF_CHE		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHE );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHE	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHE, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHE_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHE));
				break;
			case CMD_HMC472_READ_RF_CHF		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHF );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHF	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHF, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHF_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHF));
				break;
			case CMD_HMC472_READ_RF_CHG		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHG );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHG	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHG, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHG_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHG));
				break;
			case CMD_HMC472_READ_RF_CHH		:
				gainVal = HMC472Task_ReadHMC472RF( HMC472_RF_CHH );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx,(UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_RF_CHH	:
				_return = HMC472Task_SetRFGain( HMC472_RF_CHH, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				//---保存参数
				HMC472Task_EepromWrite(HMC472_RF_CHH_DBM_ADDR_X1,HMC472Task_ReadHMC472RF(HMC472_RF_CHH));
				break;
			case CMD_HMC472_READ_GEN_RF		:
				gainVal = HMC472Task_ReadRFGen(USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex],USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex+1]  );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, 0);
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal >> 8 ) );
				USARTTask_FillMode_AddByte( USARTx, (UINT8_T)( gainVal ) );
				break;
			case CMD_HMC472_WRITE_GEN_RF	:
				_return = HMC472Task_SetRFGain( CMD_HMC472_WRITE_GEN_RF, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataTwoIndex] );
				USARTTask_FillMode_AddByte(USARTx, USARTx->msgRxdHandler.pMsgVal[USARTx->msgDataOneIndex]);
				USARTTask_FillMode_AddByte(USARTx, _return);
				break;
			default:
				_return = ERROR_2;
				break;
		}

		if (_return==OK_0)
		{
			//---启动发送
			USARTTask_FillMode_WriteSTART(USARTx, 1);
		}
	}
	else
	{
		_return = EnCodeTask_USART_MenuTask(USARTx);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 从eeprom中读取配置数据
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_EepromInit(void)
{
	UINT8_T _return = 0;
	//---读取第一次的参数
	UINT8_T tempAddrX1[2]={0x00,0x00};
	//---读取第二次的参数
	UINT8_T tempAddrX2[2] = { 0x00,0x00 };
	//---合成结果
	UINT16_T tempVal = 0;

	//---信号源CHA的默认功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHA_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHA_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHA_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHA_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHA_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		HMC472Task_SetGen( HMC472_GEN_CHA,tempVal);
	}

	//---信号源CHB的默认功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHB_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHB_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, GEN_CHB_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHB_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHB_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		HMC472Task_SetGen( HMC472_GEN_CHB,tempVal);
	}

	//---信号源CHC的默认功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHC_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHC_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, GEN_CHC_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHC_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHC_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		HMC472Task_SetGen( HMC472_GEN_CHC,tempVal);
	}

	//---信号源CHD的默认功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHD_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,GEN_CHD_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, GEN_CHD_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHD_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,GEN_CHD_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		HMC472Task_SetGen( HMC472_GEN_CHD,tempVal);
	}

	//---信号源CHA的第一级衰减功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHA_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHA_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_GEN_CHA_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHA_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHA_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetGenGain( HMC472_GEN_CHA,(UINT8_T)tempVal);
	}

	//---信号源CHB的第一级衰减功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHB_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHB_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_GEN_CHB_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHB_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHB_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetGenGain( HMC472_GEN_CHB,(UINT8_T)tempVal);
	}

	//---信号源CHC的第一级衰减功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHC_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHC_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_GEN_CHC_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHC_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHC_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetGenGain( HMC472_GEN_CHC,(UINT8_T)tempVal);
	}

	//---信号源CHD的第一级衰减功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHD_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_GEN_CHD_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_GEN_CHD_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHD_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_GEN_CHD_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetGenGain( HMC472_GEN_CHD,(UINT8_T)tempVal);
	}

	//---信号源的CHA通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHA_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHA_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHA_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHA_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHA_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHA,(UINT8_T)tempVal);
	}

	//---信号源的CHB通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHB_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHB_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHB_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHB_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHB_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHB,(UINT8_T)tempVal);
	}

	//---信号源的CHC通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHC_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHC_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHC_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHC_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHC_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHC,(UINT8_T)tempVal);
	}

	//---信号源的CHD通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHD_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHD_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHD_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHD_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHD_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHD,(UINT8_T)tempVal);
	}

	//---信号源的CHE通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHE_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHE_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHE_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHE_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHE_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHE,(UINT8_T)tempVal);
	}

	//---信号源的CHF通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHF_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHF_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHF_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHF_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHF_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHF,(UINT8_T)tempVal);
	}

	//---信号源的CHG通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHG_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHG_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHG_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHG_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHG_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHG,(UINT8_T)tempVal);
	}

	//---信号源的CHH通道衰减的功率
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHH_DBM_ADDR_X1,tempAddrX1,2);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,HMC472_RF_CHH_DBM_ADDR_X2,tempAddrX2,2);
	//---数据大小的对比
	if (CompareByteToByte(tempAddrX1,tempAddrX2,2)!=0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, HMC472_RF_CHH_DBM_ADDR_X3,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHH_DBM_ADDR_X1,tempAddrX1,2);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,HMC472_RF_CHH_DBM_ADDR_X2,tempAddrX1,2);
	}
	//---判断数据是否合法
	if (tempAddrX1[0]!=0xFF)
	{
		tempVal=tempAddrX1[0];
		tempVal=(tempVal<<8)+tempAddrX1[1];
		tempVal /= 5;
		HMC472Task_SetRFGain( HMC472_RF_CHH,(UINT8_T)tempVal);
	}

	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 向eeprom中写入数据
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_EepromWrite(UINT16_T addr,UINT16_T val)
{
	UINT8_T temp[2] = { 0x00,0x00 };
	UINT8_T _return = 0;
	temp[0] = (UINT8_T)(val >> 8);
	temp[1] = (UINT8_T)(val & 0xFF);
	//---第一次保存的参数的位置
	_return=AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,addr, temp, 2);
	if (_return!=0)
	{
		_return += 1;
	}
	//---第二次保存参数的位置
	_return=AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,addr+0x20, temp, 2);
	if (_return != 0)
	{
		_return += 1;
	}
	//---第三次保存参数的位置
	_return=AT24CXXTask_I2C_WriteData(pAT24cxxDevice0,addr+0x40, temp, 2);
	if (_return != 0)
	{
		_return += 1;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能：HMC472数控信号源的USART通信逻辑
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472Task_USART_RFGenTask(USART_HandlerType*USARTx)
{
	UINT8_T _return = OK_0;
	if (USARTx!=NULL)
	{
		//---判断接收是否完成
		if (USARTTask_GetReadState(USARTx) == 1)
		{
			//---CRC的校验
			if (( USARTTask_CRCTask_Read( USARTx ) == OK_0 ) && ( USARTTask_DeviceID( USARTx ) == OK_0 ))
			{
				//---HMC472的任务处理函数
				_return=HMC472Task_USART_MenuTask(USARTx);
			}
			else
			{
				//---发生CRC校验错误
				USART_Printf(USARTx, "=>>串口%d:发生CRC校验错误<<=\r\n", (USARTx->msgIndex - 1));
			}
			_return= USARTTask_Read_Init(USARTx);
			goto GotoExit;
		}
		_return=USARTTask_TimeOVFTask(USARTx);
	}
	else
	{
		_return = ERROR_2;
	}
GotoExit:
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472Task_Key_RFGenTask(void)
{
	//---设置输出功率
	UINT8_T rfPower=0;
	//---读取按键扫描结果
	UINT8_T _return = 0;
	_return= KeyTask_GenSelScan();
	switch (_return)
	{
		case 0:
			//---315/433/230M---80dBm
			rfPower = g_RFGendBmD;
			//---207M---85dBm
			//gen207Power+=40;
			break;
		case 1:
			//---315/433/230M---100dBm
			rfPower = g_RFGendBmC;
			//---207M---95dBm
			//gen207Power+=20;
			break;
		case 2:
			//---315/433/230M---90dBm
			rfPower = g_RFGendBmB;
			//---207M---90dBm
			//gen207Power+=30;
			break;
		case 3:
			//---315/433/230M---95dBm
			rfPower = g_RFGendBmA;
			//---207M---80dBm
			//gen207Power+=0;
			break;
		default:
			//gen207Power=g_207MGenTemp;
			_return = 0x04;
			break;
	}
	if ((_return!=0x04)&&(rfPower!=0))
	{
		HMC472Task_SetRFGain( HMC472_RF_CHA,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHB,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHC,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHD,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHE,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHF,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHG,rfPower);
		HMC472Task_SetRFGain( HMC472_RF_CHH,rfPower);
	}
}