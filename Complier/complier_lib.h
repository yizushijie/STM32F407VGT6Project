#ifndef COMPLIER_LIB_H_
#define COMPLIER_LIB_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	#include "complier_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////
	
	//===º¯Êý¶¨Òå
	UINT8_T  HexToBcd(UINT8_T hexVal);
	UINT8_T  HexToASCII(UINT8_T hexVal);
	UINT8_T  ASCIIToHex(UINT8_T hexVal);
	UINT8_T  BcdToDec(UINT8_T bcdVal);
	UINT8_T  DecToBcd(UINT8_T decVal);
	UINT8_T  AscSortByte(UINT8_T *pVal, UINT16_T length);
	UINT8_T  AscSortWord(UINT16_T *pVal, UINT16_T length);
	UINT8_T  AscSortDoubleWord(UINT32_T* pVal, UINT16_T length);
	UINT8_T  DescSortByte(UINT8_T *pVal, UINT16_T length);
	UINT8_T  DescSortWord(UINT16_T *pVal, UINT16_T length);
	UINT16_T CalcAvgWordFromIndex(UINT16_T *pVal, UINT16_T length, UINT16_T index);
	UINT16_T CalcAvgWord(UINT16_T *pVal, UINT16_T length);
	UINT8_T  CalcAvgByteFromIndex(UINT8_T *pVal, UINT16_T length, UINT16_T index);
	UINT16_T CalcAvgByte(UINT8_T *pVal, UINT16_T length);
	UINT32_T CalcAvgDoubleWordFromIndex(UINT32_T* pVal, UINT16_T length, UINT16_T index);
	UINT8_T  CompareByteToByte(UINT8_T *p1, UINT8_T *p2, UINT16_T length);
	UINT8_T  CompareByteToByteFromIndex(UINT8_T *p1, UINT8_T *p2, UINT16_T index, UINT16_T length);
	UINT8_T  CompareWordToWord(UINT16_T *p1, UINT16_T *p2, UINT16_T length);
	UINT8_T  CompareWordToWordFromIndex(UINT16_T *p1, UINT16_T *p2, UINT16_T index, UINT16_T length);
	UINT8_T	 CompareByte(UINT8_T* p1, UINT8_T val, UINT16_T length);
	UINT8_T  CompareWord(UINT16_T* p1, UINT16_T val, UINT16_T length);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*COMPLIER_LIB_H */