#ifndef HMC472_LIB_H_
#define HMC472_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "hmc472_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===º¯Êý¶¨Òå
	void HMC472Lib_Init( void );
	void HMC472Lib_SetGain( UINT8_T val );
	UINT8_T HMC472Lib_SetGenGain( UINT8_T genCh, UINT8_T gainVal );
	UINT8_T HMC472Lib_SetRFGain( UINT8_T rfCH, UINT8_T gainVal );
	UINT16_T HMC472Lib_ReadGen( UINT8_T genCH );
	UINT8_T  HMC472Lib_SetGen(UINT8_T genCH, UINT16_T genVal );
	UINT16_T HMC472Lib_ReadHMC472Gen( UINT8_T genCH );
	UINT16_T HMC472Lib_ReadHMC472RF( UINT8_T rfCH );
	UINT16_T HMC472Lib_ReadRFGen( UINT8_T genCH, UINT8_T rfCH );
	UINT8_T HMC472Lib_SetRFGen( UINT8_T genCH, UINT8_T rfCH, UINT16_T rfGenPower );
	UINT8_T HMC472Lib_GetVersion( UINT8_T *pVersion,UINT8_T length );

	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*HMC472_LIB_H_ */