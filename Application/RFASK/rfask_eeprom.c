#include"rfask_eeprom.h"

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 加载eeprom中的配置数据
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void RFASK_EEPROMInit(RFASK_HandlerType* rfask, AT24CXX_HandlerType* AT24CXXx)
{
	UINT8_T _return = OK_0;
	UINT8_T tempAddrX1[16] = { 0 };
	UINT8_T tempAddrX2[16] = { 0 };

	//---读取配置信息
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_TYPE_ADDR_X1, tempAddrX1, 5);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_TYPE_ADDR_X2, tempAddrX2, 5);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 5) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,  RFASK_TYPE_ADDR_X3, tempAddrX1, 5);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_TYPE_ADDR_X1, tempAddrX1, 5);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_TYPE_ADDR_X2, tempAddrX1, 5);
		_return = ERROR_1;
	}
	if (tempAddrX1[0]!=0xFF)
	{
		rfask->msgDeviceType = tempAddrX1[0];

		rfask->msgSampleX100Res = tempAddrX1[1];
		rfask->msgSampleX100Res =(rfask->msgSampleX100Res<<8)+ tempAddrX1[2];

		rfask->msgAmpTimes= tempAddrX1[3];
		rfask->msgAmpTimes = (rfask->msgAmpTimes << 8) + tempAddrX1[4];
	}
	//---设备的类型不能使0
	if (tempAddrX1[0] == 0x00)
	{
		_return = ERROR_1;
	}
	//---清零操作
	memset(tempAddrX1, 0, 16);
	memset(tempAddrX2, 0, 16);

	//---读取默认频率点
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_FREQ_YSEL1_ADDR_X1, tempAddrX1, 16);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_FREQ_YSEL1_ADDR_X2, tempAddrX2, 16);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 16) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_FREQ_YSEL1_ADDR_X3,  tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_FREQ_YSEL1_ADDR_X1, tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_FREQ_YSEL1_ADDR_X1, tempAddrX1, 16);
		_return = ERROR_2;
	}
	if (tempAddrX1[0] != 0xFF)
	{
		rfask->msgFreqX100MHzYSel1 = tempAddrX1[0];
		rfask->msgFreqX100MHzYSel1 = (rfask->msgFreqX100MHzYSel1 << 8) + tempAddrX1[1];
		rfask->msgFreqX100MHzYSel1 = (rfask->msgFreqX100MHzYSel1 << 8) + tempAddrX1[2];
		rfask->msgFreqX100MHzYSel1 = (rfask->msgFreqX100MHzYSel1 << 8) + tempAddrX1[3];

		rfask->msgFreqX100MHzYSel2 = tempAddrX1[4];
		rfask->msgFreqX100MHzYSel2 = (rfask->msgFreqX100MHzYSel2 << 8) + tempAddrX1[5];
		rfask->msgFreqX100MHzYSel2 = (rfask->msgFreqX100MHzYSel2 << 8) + tempAddrX1[6];
		rfask->msgFreqX100MHzYSel2 = (rfask->msgFreqX100MHzYSel2 << 8) + tempAddrX1[7];

		rfask->msgFreqX100MHzYSel3 = tempAddrX1[8];
		rfask->msgFreqX100MHzYSel3 = (rfask->msgFreqX100MHzYSel3 << 8) + tempAddrX1[9];
		rfask->msgFreqX100MHzYSel3 = (rfask->msgFreqX100MHzYSel3 << 8) + tempAddrX1[10];
		rfask->msgFreqX100MHzYSel3 = (rfask->msgFreqX100MHzYSel3 << 8) + tempAddrX1[11];

		rfask->msgFreqX100MHzYSel4 = tempAddrX1[12];
		rfask->msgFreqX100MHzYSel4 = (rfask->msgFreqX100MHzYSel4 << 8) + tempAddrX1[13];
		rfask->msgFreqX100MHzYSel4 = (rfask->msgFreqX100MHzYSel4 << 8) + tempAddrX1[14];
		rfask->msgFreqX100MHzYSel4 = (rfask->msgFreqX100MHzYSel4 << 8) + tempAddrX1[15];
	}

	//---清零操作
	memset(tempAddrX1, 0, 16);
	memset(tempAddrX2, 0, 16);

	//---读取第一个频率扫描的频率配置参数
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X1, tempAddrX1, 16);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X2, tempAddrX2, 16);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 16) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X3, tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X1, tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTONE_START_MAX_CURRENT_ADDR_X2, tempAddrX1, 16);
		_return = ERROR_3;
	}
	if (tempAddrX1[0] != 0xFF)
	{
		rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA = tempAddrX1[0];
		rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStartMaxCurrentX100mA << 8) + tempAddrX1[1];
		
		rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA = tempAddrX1[2];
		rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStartMinCurrentX100mA << 8) + tempAddrX1[3];

		rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA = tempAddrX1[4];
		rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStopMaxCurrentX100mA << 8) + tempAddrX1[5];

		rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA = tempAddrX1[6];
		rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA = (rfask->msgFreqCurrentPointOne.msgStopMinCurrentX100mA << 8) + tempAddrX1[7];

		rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = tempAddrX1[8];
		rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + tempAddrX1[9];
		rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + tempAddrX1[10];
		rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz << 8) + tempAddrX1[11];

		rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz = tempAddrX1[12];
		rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz = (rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz << 8) + tempAddrX1[13];

		rfask->msgFreqCurrentPointOne.msgFreqPointNum = tempAddrX1[14];
		rfask->msgFreqCurrentPointOne.msgFreqPointNum = (rfask->msgFreqCurrentPointOne.msgFreqPointNum << 8) + tempAddrX1[15];

		//---计算截止频率
		rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz = rfask->msgFreqCurrentPointOne.msgFreqPointNum;
		rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz *= rfask->msgFreqCurrentPointOne.msgStepFreqX100MHz;
		rfask->msgFreqCurrentPointOne.msgStopFreqX100MHz += rfask->msgFreqCurrentPointOne.msgStartFreqX100MHz;

	}

	//---清零操作
	memset(tempAddrX1, 0, 16);
	memset(tempAddrX2, 0, 16);

	//---读取第一个频率扫描的ADC配置参数
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_FREQ_CURRENT_POWER_ADDR_X1, tempAddrX1, 8);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_FREQ_CURRENT_POWER_ADDR_X2, tempAddrX2, 8);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 8) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTONE_FREQ_CURRENT_POWER_ADDR_X3, tempAddrX1,  8);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTONE_FREQ_CURRENT_POWER_ADDR_X1, tempAddrX1, 8);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTONE_FREQ_CURRENT_POWER_ADDR_X2, tempAddrX1, 8);
		_return = ERROR_4;
	}
	if (tempAddrX1[0] != 0xFF)
	{
		rfask->msgFreqCurrentPointOne.msgADCPointNum = tempAddrX1[2];
		rfask->msgFreqCurrentPointOne.msgADCPointNum = (rfask->msgFreqCurrentPointOne.msgADCPointNum << 8) + tempAddrX1[3];

		rfask->msgFreqCurrentPointOne.msgADCPassMax = tempAddrX1[4];
		rfask->msgFreqCurrentPointOne.msgADCPassMax = (rfask->msgFreqCurrentPointOne.msgADCPassMax << 8) + tempAddrX1[5];

		rfask->msgFreqCurrentPointOne.msgADCPassMin = tempAddrX1[6];
		rfask->msgFreqCurrentPointOne.msgADCPassMin = (rfask->msgFreqCurrentPointOne.msgADCPassMin << 8) + tempAddrX1[7];
	}

	//---清零操作
	memset(tempAddrX1, 0, 16);
	memset(tempAddrX2, 0, 16);

	//---读取第二个频率扫描的频率配置参数
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X1, tempAddrX1, 16);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X2, tempAddrX2, 16);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 16) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,  RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X3, tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X1, tempAddrX1, 16);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTTWO_START_MAX_CURRENT_ADDR_X2, tempAddrX1, 16);
		_return = ERROR_5;
	}
	if (tempAddrX1[0] != 0xFF)
	{
		rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA = tempAddrX1[0];
		rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStartMaxCurrentX100mA << 8) + tempAddrX1[1];

		rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA = tempAddrX1[2];
		rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStartMinCurrentX100mA << 8) + tempAddrX1[3];

		rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA = tempAddrX1[4];
		rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStopMaxCurrentX100mA << 8) + tempAddrX1[5];

		rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA = tempAddrX1[6];
		rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA = (rfask->msgFreqCurrentPointTwo.msgStopMinCurrentX100mA << 8) + tempAddrX1[7];

		rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = tempAddrX1[8];
		rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + tempAddrX1[9];
		rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + tempAddrX1[10];
		rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz << 8) + tempAddrX1[11];

		rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz = tempAddrX1[12];
		rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz = (rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz << 8) + tempAddrX1[13];

		rfask->msgFreqCurrentPointTwo.msgFreqPointNum = tempAddrX1[14];
		rfask->msgFreqCurrentPointTwo.msgFreqPointNum = (rfask->msgFreqCurrentPointTwo.msgFreqPointNum << 8) + tempAddrX1[15];

		rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz = rfask->msgFreqCurrentPointTwo.msgFreqPointNum;
		rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz *= rfask->msgFreqCurrentPointTwo.msgStepFreqX100MHz;
		rfask->msgFreqCurrentPointTwo.msgStopFreqX100MHz += rfask->msgFreqCurrentPointTwo.msgStartFreqX100MHz;
	}

	//---清零操作
	memset(tempAddrX1, 0, 16);
	memset(tempAddrX2, 0, 16);

	//---读取第二个频率扫描的ADC配置参数
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTTWO_FREQ_CURRENT_POWER_ADDR_X1, tempAddrX1, 8);
	AT24CXXTask_I2C_ReadData(pAT24cxxDevice0, RFASK_POINTTWO_FREQ_CURRENT_POWER_ADDR_X2, tempAddrX2, 8);
	//---数据匹配
	if (CompareByteToByte(tempAddrX1, tempAddrX2, 8) != 0)
	{
		AT24CXXTask_I2C_ReadData(pAT24cxxDevice0,  RFASK_POINTTWO_FREQ_CURRENT_POWER_ADDR_X3, tempAddrX1, 8);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTTWO_FREQ_CURRENT_POWER_ADDR_X1, tempAddrX1, 8);
		AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, RFASK_POINTTWO_FREQ_CURRENT_POWER_ADDR_X2, tempAddrX1, 8);
		_return = ERROR_6;
	}
	if (tempAddrX1[0] != 0xFF)
	{
		rfask->msgFreqCurrentPointTwo.msgADCPointNum = tempAddrX1[2];
		rfask->msgFreqCurrentPointTwo.msgADCPointNum = (rfask->msgFreqCurrentPointTwo.msgADCPointNum << 8) + tempAddrX1[3];

		rfask->msgFreqCurrentPointTwo.msgADCPassMax = tempAddrX1[4];
		rfask->msgFreqCurrentPointTwo.msgADCPassMax = (rfask->msgFreqCurrentPointTwo.msgADCPassMax << 8) + tempAddrX1[5];

		rfask->msgFreqCurrentPointTwo.msgADCPassMin = tempAddrX1[6];
		rfask->msgFreqCurrentPointTwo.msgADCPassMin = (rfask->msgFreqCurrentPointTwo.msgADCPassMin << 8) + tempAddrX1[7];
	}

	//---如果eeprom中的数据错误，重新加载默认配置信息
	if (_return!=OK_0)
	{
		RFASK_StructInit(rfask);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 将数据写入eeprom中并进行三重地址保护验证
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T RFASK_EEPROMWrite(AT24CXX_HandlerType* AT24CXXx, UINT16_T addr, UINT32_T cfgVal, UINT8_T cout)
{
	UINT8_T _return = OK_0;
	UINT8_T temp[4] = { 0 };
	temp[0] = (UINT8_T)(cfgVal >> 24);
	temp[1] = (UINT8_T)(cfgVal >> 16);
	temp[2] = (UINT8_T)(cfgVal >> 8);
	temp[3] = (UINT8_T)(cfgVal);
	
	//---校验写入的数据是否合法
	if ((cout==0)||(cout>4)||(AT24CXXx==NULL))
	{
		_return = ERROR_1;
	}
	else
	{
		//---第一次保存的参数的位置
		_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, addr, &temp[4 - cout], cout);
		if (_return != 0)
		{
			_return += 2;
		}
		//---第二次保存参数的位置
		_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, addr + 0x50, &temp[4 - cout], cout);
		if (_return != 0)
		{
			_return += 3;
		}
		//---第三次保存参数的位置
		_return = AT24CXXTask_I2C_WriteData(pAT24cxxDevice0, addr + 0xA0, &temp[4 - cout], cout);
		if (_return != 0)
		{
			_return += 4;
		}
	}
	return _return;
}