#ifndef HVPP_CFG_H_
#define HVPP_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include"complier_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===0---编程忙和1---等待新命令
	#define HVPP_RDY_BSY_PORT								GPIOA
	#define HVPP_RDY_BSY_BIT								LL_GPIO_PIN_0
	#define HVPP_RDY_BSY_STATE								GPIO_GET_STATE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_RDY_BSY_WRITE								GPIO_SET_WRITE(DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_RDY_BSY_READ								GPIO_SET_READ( DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_RDY_BSY_OUT_0								GPIO_OUT_0(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_RDY_BSY_OUT_1								GPIO_OUT_1(    DECA_CH_PORT,DECA_CH_BIT)
	#define HVPP_RDY_BSY_OUT_C								GPIO_OUT_C(    DECA_CH_PORT,DECA_CH_BIT)


	
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*HVPP_CFG_H_ */