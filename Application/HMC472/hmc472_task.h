#ifndef HMC472_TASK_H_
#define HMC472_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "hmc472_lib.h"
	#include "usart_task.h"
	#include "at24cxx_task.h"
	#include "encode_task.h"
	#include "key_task.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===参数配置的地址，第三页开始
	#define GEN_CHA_DBM_ADDR_X1					0x10					
	#define GEN_CHB_DBM_ADDR_X1					( GEN_CHA_DBM_ADDR_X1+0x02 )
	#define GEN_CHC_DBM_ADDR_X1					( GEN_CHA_DBM_ADDR_X1+0x04 )
	#define GEN_CHD_DBM_ADDR_X1					( GEN_CHA_DBM_ADDR_X1+0x06 )

	//===第四页开始								  
	#define HMC472_GEN_CHA_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x08 )
	#define HMC472_GEN_CHB_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x0A )
	#define HMC472_GEN_CHC_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x0C )
	#define HMC472_GEN_CHD_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x0E )
			
	//===第五页开始	
	#define HMC472_RF_CHA_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x10 )
	#define HMC472_RF_CHB_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x12 )
	#define HMC472_RF_CHC_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x14 )
	#define HMC472_RF_CHD_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x16 )

	//===第六页开始	
	#define HMC472_RF_CHE_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x18 )
	#define HMC472_RF_CHF_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x1A )
	#define HMC472_RF_CHG_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x1C )
	#define HMC472_RF_CHH_DBM_ADDR_X1			( GEN_CHA_DBM_ADDR_X1+0x1E )
	
	//===保存数据的第二个地址，第七页开始	
	#define	GEN_CHA_DBM_ADDR_X2					( GEN_CHA_DBM_ADDR_X1+0x20 )//0x30
	#define GEN_CHB_DBM_ADDR_X2					( GEN_CHA_DBM_ADDR_X2+0x02 )
	#define GEN_CHC_DBM_ADDR_X2					( GEN_CHA_DBM_ADDR_X2+0x04 )
	#define GEN_CHD_DBM_ADDR_X2					( GEN_CHA_DBM_ADDR_X2+0x06 )
	
	//===第八页开始	
	#define HMC472_GEN_CHA_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x08 )
	#define HMC472_GEN_CHB_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x0A )
	#define HMC472_GEN_CHC_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x0C )
	#define HMC472_GEN_CHD_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x0E )
		
	//===第九页开始		
	#define HMC472_RF_CHA_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x10 )
	#define HMC472_RF_CHB_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x12 )
	#define HMC472_RF_CHC_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x14 )
	#define HMC472_RF_CHD_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x16 )

	//===第十页开始		
	#define HMC472_RF_CHE_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x18 )
	#define HMC472_RF_CHF_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x1A )
	#define HMC472_RF_CHG_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x1C )
	#define HMC472_RF_CHH_DBM_ADDR_X2			( GEN_CHA_DBM_ADDR_X2+0x1E )
	
	//===保存数据的第三个地址，第十一页开始		
	#define	GEN_CHA_DBM_ADDR_X3					( GEN_CHA_DBM_ADDR_X1+0x40 )//0x50
	#define GEN_CHB_DBM_ADDR_X3					( GEN_CHA_DBM_ADDR_X3+0x02 )
	#define GEN_CHC_DBM_ADDR_X3					( GEN_CHA_DBM_ADDR_X3+0x04 )
	#define GEN_CHD_DBM_ADDR_X3					( GEN_CHA_DBM_ADDR_X3+0x06 )
		
	//===第十二页开始		
	#define HMC472_GEN_CHA_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x08 )
	#define HMC472_GEN_CHB_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x0A )
	#define HMC472_GEN_CHC_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x0C )
	#define HMC472_GEN_CHD_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x0E )
		
	//===第十三页开始		
	#define HMC472_RF_CHA_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x10 )
	#define HMC472_RF_CHB_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x12 )
	#define HMC472_RF_CHC_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x14 )
	#define HMC472_RF_CHD_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x16 )

	//===第十四页开始		
	#define HMC472_RF_CHE_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x18 )
	#define HMC472_RF_CHF_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x1A )
	#define HMC472_RF_CHG_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x1C )
	#define HMC472_RF_CHH_DBM_ADDR_X3			( GEN_CHA_DBM_ADDR_X3+0x1E )

	/////////////////////////////////////////////////////////////////////////////////////

	//===函数定义
	void HMC472Task_Init( void );
	void HMC472Task_SetGain( UINT8_T val );
	UINT8_T HMC472Task_SetGenGain( UINT8_T genCh, UINT8_T gainVal );
	UINT8_T HMC472Task_SetRFGain( UINT8_T rfCH, UINT8_T gainVal );
	UINT16_T HMC472Task_ReadGen( UINT8_T genCH );
	UINT16_T HMC472Task_ReadHMC472Gen( UINT8_T genCH );
	UINT16_T HMC472Task_ReadHMC472RF( UINT8_T rfCH );
	UINT16_T HMC472Task_ReadRFGen( UINT8_T genCH, UINT8_T rfCH );
	UINT8_T HMC472Task_SetRFGen( UINT8_T genCH, UINT8_T rfCH, UINT16_T rfGenPower );
	UINT8_T HMC472Task_GetVersion( UINT8_T *pVersion,UINT8_T length );
	UINT8_T HMC472Task_EepromInit(void);
	UINT8_T HMC472Task_EepromWrite(UINT16_T addr, UINT16_T val);
	UINT8_T HMC472Task_USART_RFGenTask(USART_HandlerType*USARTx);
	void HMC472Task_Key_RFGenTask(void);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*HMC472_TASK_H_ */