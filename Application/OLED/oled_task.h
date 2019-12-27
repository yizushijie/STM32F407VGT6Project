#ifndef OLED_TASK_H_
#define OLED_TASK_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "oled_lib.h"
	//////////////////////////////////////////////////////////////////////////////////////
	//===º¯Êý¶¨Òå
	UINT8_T OLEDTask_I2C_Init(OLED_IIC_HandlerType *OLEDx, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C);
	UINT8_T OLEDTask_I2C_WriteCmd(OLED_IIC_HandlerType *OLEDx, UINT8_T cmd);
	UINT8_T OLEDTask_I2C_WriteData(OLED_IIC_HandlerType *OLEDx, UINT8_T cmd);
	void OLEDTask_I2C_DisplayON(OLED_IIC_HandlerType *OLEDx);
	void OLEDTask_I2C_DisplayOFF(OLED_IIC_HandlerType *OLEDx);
	void OLEDTask_I2C_Fill(OLED_IIC_HandlerType *OLEDx, UINT8_T dat);
	void OLEDTask_I2C_Clear(OLED_IIC_HandlerType *OLEDx);
	void OLEDTask_I2C_SetPos(OLED_IIC_HandlerType *OLEDx, UINT8_T xPos, UINT8_T yPos);
	void OLEDTask_I2C_DrawBMP(OLED_IIC_HandlerType *OLEDx, UINT8_T x0Pos, UINT8_T y0Pos, UINT8_T x1Pos, UINT8_T y1Pos, UINT8_T BMP[]);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*OLED_TASK_H_ */