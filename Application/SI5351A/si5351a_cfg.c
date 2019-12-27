#include "si5351a_cfg.h"

//===变量定义
SI5351A_HandlerType			g_Si5351aDevice0={0};
pSI5351A_HandlerType		pSi5351aDevice0=&g_Si5351aDevice0;

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：参数初始化
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
void SI5351A_I2C_ParamsInit(SI5351A_HandlerType* SI5351Ax)
{
	//---初始化参数
	memset(SI5351Ax->msgClockCTRL,0x4F, SI5351A_CLKOUT_CHANNEL_SIZE);
	//---配置参数
	SI5351Ax->msgIsResetPll=0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_Device0_Init(SI5351A_HandlerType* SI5351Ax)
{
	SI5351Ax->msgI2C.msgI2Cx = NULL;
	SI5351Ax->msgI2C.msgSCL.msgGPIOPort = GPIOB;
	SI5351Ax->msgI2C.msgSCL.msgGPIOBit = LL_GPIO_PIN_8;
	SI5351Ax->msgI2C.msgSDA.msgGPIOPort = GPIOB;
	SI5351Ax->msgI2C.msgSDA.msgGPIOBit = LL_GPIO_PIN_9;
	SI5351Ax->msgI2C.msgModelIsHW = 0;
	SI5351Ax->msgI2C.msgPluseWidth = 1;
	SI5351Ax->msgI2C.msgDelayus = NULL;
	SI5351Ax->msgI2C.msgAddr = SI5351A_WADDR;
	SI5351Ax->msgI2C.msgClockSpeed = 0;
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_Device1_Init(SI5351A_HandlerType* SI5351Ax)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_Device2_Init(SI5351A_HandlerType* SI5351Ax)
{
	return OK_0;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 写单独模式
//////输入参数:
//////输出参数:
//////说		明： 软件模拟I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SWI2C_WriteSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T val)
{
	UINT8_T _return = OK_0;
	//---启动并发送写操作
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}
	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), addr);
	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
	//---判断应答
	if (_return)
	{
		_return = ERROR_3;
		goto GoToExit;
	}
	//---发送寄存器数据
	I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), val);
	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
	//---判断应答
	if (_return)
	{
		_return = ERROR_3 + 1;
		goto GoToExit;
	}	
GoToExit:
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： 硬件I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_HWI2C_WriteSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T val)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_WriteSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T val)
{
	if (SI5351Ax->msgI2C.msgModelIsHW == 0)
	{
		//---软件模拟I2C
		return SI5351A_SWI2C_WriteSingle(SI5351Ax, addr, val);
	}
	else
	{
		//---硬件I2C
		return SI5351A_HWI2C_WriteSingle(SI5351Ax, addr, val);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 读单独模式
//////输入参数:
//////输出参数:
//////说		明： 软件模拟I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SWI2C_ReadSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal)
{
	UINT8_T _return = OK_0;
	//---启动写数据
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}
	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), addr);
	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
	if (_return != OK_0)
	{
		//---发送数据失败
		_return = ERROR_3;
		goto GoToExit;
	}
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	//---启动读取数据
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 0);
	if (_return != OK_0)
	{
		//---启动读数据失败
		_return = ERROR_4;
		goto GoToExit;
	}
	//---读取数据
	_return = I2CTask_MSW_ReadByte(&(SI5351Ax->msgI2C));
	//---发送应答信号
	I2CTask_MSW_SendACK(&(SI5351Ax->msgI2C), 1);
	if (pVal!=NULL)
	{
		*pVal=_return;
	}
	_return = OK_0;
	//---退出入口函数
GoToExit:
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_HWI2C_ReadSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明： I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_ReadSingle(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal)
{
	if (SI5351Ax->msgI2C.msgModelIsHW == 0)
	{
		//---软件模拟I2C
		return SI5351A_SWI2C_ReadSingle(SI5351Ax, addr, pVal);
	}
	else
	{
		//---硬件I2C
		return SI5351A_HWI2C_ReadSingle(SI5351Ax, addr, pVal);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 写Bulk模式
//////输入参数:
//////输出参数:
//////说		明： 软件模拟I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SWI2C_WriteBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	UINT8_T _return = OK_0;
	UINT8_T i = 0;
	//---启动并发送写操作
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}
	//---发送地址
	I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), addr);
	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
	//---判断应答
	if (_return)
	{
		_return = ERROR_3;
		goto GoToExit;
	}
	//---发送数据
	for (i = 0; i < length; i++)
	{
		//---发送数据
		I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), pVal[i]);
		//---读取ACK
		_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
		//---判断应答
		if (_return)
		{
			_return = ERROR_4 + i;
			goto GoToExit;
		}
	}
	//---退出入口函数
GoToExit:
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_HWI2C_WriteBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_WriteBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	if (SI5351Ax->msgI2C.msgModelIsHW == 0)
	{
		//---软件模拟I2C
		return SI5351A_SWI2C_WriteBulk(SI5351Ax, addr, pVal, length);
	}
	else
	{
		//---硬件I2C
		return SI5351A_HWI2C_WriteBulk(SI5351Ax, addr, pVal,length);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能： 读Bulk模式
//////输入参数:
//////输出参数:
//////说		明： 软件模拟I2C传输命令
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SWI2C_ReadBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	UINT8_T _return = OK_0;
	UINT16_T i = 0;
	//---启动写数据
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 1);
	if (_return != OK_0)
	{
		//---启动写数据失败
		_return = ERROR_2;
		goto GoToExit;
	}
	//---发送寄存器地址
	I2CTask_MSW_SendByte(&(SI5351Ax->msgI2C), addr);
	//---读取ACK
	_return = I2CTask_MSW_ReadACK(&(SI5351Ax->msgI2C));
	if (_return != OK_0)
	{
		//---发送数据失败
		_return = ERROR_3;
		goto GoToExit;
	}
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	//---启动读取数据
	_return = I2CTask_MSW_START(&(SI5351Ax->msgI2C), 0);
	if (_return != OK_0)
	{
		//---启动读数据失败
		_return = ERROR_4;
		goto GoToExit;
	}
	for (i = 0; i < length; i++)
	{
		//---读取数据
		pVal[i] = I2CTask_MSW_ReadByte(&(SI5351Ax->msgI2C));
		if (i == (length - 1))
		{
			_return = 1;
		}
		//---发送应答信号
		I2CTask_MSW_SendACK(&(SI5351Ax->msgI2C), _return);
	}
	_return = OK_0;
	//---退出入口函数
GoToExit:
	//---发送停止信号
	I2CTask_MSW_STOP(&(SI5351Ax->msgI2C));
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_HWI2C_ReadBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	return ERROR_1;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_ReadBulk(SI5351A_HandlerType* SI5351Ax, UINT8_T addr, UINT8_T* pVal, UINT8_T length)
{
	if (SI5351Ax->msgI2C.msgModelIsHW == 0)
	{
		//---软件模拟I2C
		return SI5351A_SWI2C_ReadBulk(SI5351Ax, addr, pVal, length);
	}
	else
	{
		//---硬件I2C
		return SI5351A_HWI2C_ReadBulk(SI5351Ax, addr, pVal, length);
	}
}



///////////////////////////////////////////////////////////////////////////////
//////fVCO is the PLL output, and must be between 600..900MHz,
//////fVCO = fXTAL * (a + (b / c))
//////fXTAL = the crystal input frequency
//////a = an integer between 15 and 90
//////b = the fractional numerator(0..1, 048, 575)
//////c = the fractional denominator(1..1, 048, 575)
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置pll时钟
//////输入参数:	
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_ConfigMSNBReg(SI5351A_HandlerType* SI5351Ax, UINT8_T regMSNAddr, UINT32_T a, UINT32_T b, UINT32_T c)
{
	UINT32_T p1 = 0;
	UINT32_T p2 = 0;
	UINT32_T p3 = 0;

	if (b == 0)
	{
		p1 = (UINT32_T)(128 * a - 512);
		p2 = 0;
		p3 = c;
	}
	else
	{
		p1 = (UINT32_T)(128 * ((float)b / (float)c));
		p2 = (UINT32_T)(128 * b - c * p1);
		p1 = (UINT32_T)(128 * a + p1 - 512);
		p3 = c;
	}

	UINT8_T params[10];
	UINT8_T i = 0;
	UINT8_T temp;

	// Registers 26-27 for PLLA
	temp = ((p3 >> 8) & 0xFF);
	params[i++] = temp;
	temp = (UINT8_T)(p3 & 0xFF);
	params[i++] = temp;

	// Register 28 for PLLA
	temp = (UINT8_T)((p1 >> 16) & 0x03);
	params[i++] = temp;

	// Registers 29-30 for PLLA
	temp = (UINT8_T)((p1 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (UINT8_T)(p1 & 0xFF);
	params[i++] = temp;

	// Register 31 for PLLA
	temp = (UINT8_T)((p3 >> 12) & 0xF0);
	temp += (UINT8_T)((p2 >> 16) & 0x0F);
	params[i++] = temp;

	// Registers 32-33 for PLLA
	temp = (UINT8_T)((p2 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (UINT8_T)(p2 & 0xFF);
	params[i++] = temp;
	//---写入命令
	return SI5351A_I2C_WriteBulk(SI5351Ax, regMSNAddr, params, i);
}

//////////////////////////////////////////////////////////////////////////////
//////fOUT = fVCO / MSx
//////a + b / c
//////a = The integer value, which must be 4, 6 or 8 in integer mode(MSx_INT = 1)
//////or 8..900 in fractional mode(MSx_INT = 0).
//////b = The fractional numerator(0..1, 048, 575)
//////c = The fractional denominator(1..1, 048, 575)
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_ConfigMSREG(SI5351A_HandlerType* SI5351Ax, UINT8_T regMSAddr, UINT32_T pllDIV, UINT32_T clkoutDIV)
{
	UINT32_T p1 = 0;
	UINT32_T p2 = 0;
	UINT32_T p3 = 0;
	//---a=pllDIV
	//---b=0
	//---c=1
	p1 = (UINT32_T)(128 * pllDIV - 512);
	p2 = 0;
	p3 = 1;
	UINT8_T params[10];
	UINT8_T i = 0;
	UINT8_T temp;

	// Registers 42-43 for CLK0
	temp = (UINT8_T)((p3 >> 8) & 0xFF);
	params[i++] = temp;
	temp = (UINT8_T)(p3 & 0xFF);
	params[i++] = temp;

	// Register 44 for CLK0
	temp = ((UINT8_T)((p1 >> 16) & 0x03) | clkoutDIV);
	params[i++] = temp;

	// Registers 45-46 for CLK0
	temp = (UINT8_T)((p1 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (UINT8_T)(p1 & 0xFF);
	params[i++] = temp;

	// Register 47 for CLK0
	temp = ((UINT8_T)((p3 >> 12) & 0xF0)) | ((UINT8_T)((p2 >> 16) & 0x0F));
	params[i++] = temp;

	// Registers 48-49 for CLK0
	temp = (UINT8_T)((p2 >> 8) & 0xFF);
	params[i++] = temp;

	temp = (UINT8_T)(p2 & 0xFF);
	params[i++] = temp;

	//---写入命令
	return SI5351A_I2C_WriteBulk(SI5351Ax, regMSAddr, params, i);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：计算配置参数
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_CalcConfig(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, UINT64_T freq)
{
	UINT64_T pllFreq;
	UINT8_T  clkoutDIV = 0;
	UINT32_T a = 0;
	UINT32_T b = 0;
	UINT32_T c = 0;
	UINT32_T pllDIV = 0;
	//---输出时钟---8 KHz
	if (freq < 8000ULL)
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_128;
		freq <<= 7;
	}
	//---输出时钟---8-16 KHz
	else if ((freq >= 8000ULL) && (freq < 16000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_64;
		freq <<= 6;
	}
	//---输出时钟---16-32 KHz
	else if ((freq >= 16000ULL) && (freq < 32000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_32;
		freq <<= 5;
	}
	//---输出时钟---32-64 KHz
	else if ((freq >= 32000ULL) && (freq < 64000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_16;
		freq <<= 4;
	}
	//---输出时钟---64-128 KHz
	else if ((freq >= 64000ULL) && (freq < 128000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_8;
		freq <<= 3;
	}
	//---输出时钟---128-256 KHz
	else if ((freq >= 128000ULL) && (freq < 256000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_4;
		freq <<= 2;
	}
	//---输出时钟---256-512 KHz
	else if ((freq >= 256000ULL) && (freq < 512000ULL))
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_2;
		freq <<= 1;
	}
	else
	{
		clkoutDIV = SI5351A_CLKOUT_DIV_1;
	}
	//---校验输出频率下限
	if (freq < SI5351A_CLKOUT_MIN_HZ)
	{
		freq = SI5351A_CLKOUT_MIN_HZ;
	}
	//---校验输出频率上限
	if (freq > SI5351A_CLKOUT_MAX_HZ)
	{
		freq = SI5351A_CLKOUT_MAX_HZ;
	}
	//---锁相环频率:900 mhz，计算锁相环的分频数Fvco;
	pllDIV = (UINT32_T)(900000000 / freq);
	//---校验界限
	if (pllDIV > SI5351A_MS_A_MAX)
	{
		pllDIV = SI5351A_MS_A_MAX;
	}
	if (pllDIV < SI5351A_MS_A_MIN)
	{
		pllDIV = SI5351A_MS_A_MIN;
	}
	//---保证分频数偶数
	if ((pllDIV & 0x01) != 0)
	{
		pllDIV--;
	}
	//---重新计算PLL的频率
	pllFreq = pllDIV * freq;
	a = (UINT32_T)(pllFreq / SI5351A_CLK_HZ);
	b = (UINT32_T)(((pllFreq % SI5351A_CLK_HZ) * 1048575) / SI5351A_CLK_HZ);
	c = 1048575;
	//---校验界限
	if (b > SI5351A_PLL_B_MAX)
	{
		b -= 1048575;
		a++;
	}
	//---校验界限
	if (a > SI5351A_PLL_A_MAX)
	{
		a = SI5351A_PLL_A_MAX;
	}
	if (a < SI5351A_PLL_A_MIN)
	{
		a = SI5351A_PLL_A_MIN;
	}
	clkChannel=(clkChannel > (SI5351A_CLKOUT_CHANNEL_SIZE - 1)) ? (SI5351A_CLKOUT_CHANNEL_SIZE - 1) : clkChannel;
	//---配置PLLA
	SI5351A_I2C_ConfigMSNBReg(SI5351Ax, SI5351A_REG_MSNA_ADDR, a, b, c);
	//---配置MS，通道0的输出值
	clkoutDIV=SI5351A_I2C_ConfigMSREG(SI5351Ax, SI5351A_REG_MS0_ADDR + clkChannel * 18, pllDIV, clkoutDIV);
	//---校验是否复位PLL
	if (SI5351Ax->msgIsResetPll!=0)
	{
		//---复位PLL
		clkoutDIV= SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_PLL_RST_ADDR, SI5351A_PLL_RST_A);
	}
	//---使能通道CLK0输出
	// SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_CLK0_ADDR + clkChannel, SI5351Ax->msgClockCTRL[clkChannel]);
	return clkoutDIV;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_Init(SI5351A_HandlerType* SI5351Ax, void(*pFuncDelayus)(UINT32_T delay), UINT8_T isHWI2C)
{
	UINT8_T _return = OK_0;
	//---参数初始化
	SI5351A_I2C_ParamsInit(SI5351Ax);
	//---选择设备参数
	if ((SI5351Ax != NULL) && (SI5351Ax == SI5351A_TASK_ONE))
	{
		SI5351A_I2C_Device0_Init(SI5351Ax);
	}
	else if ((SI5351Ax != NULL) && (SI5351Ax == SI5351A_TASK_TWO))
	{
		SI5351A_I2C_Device1_Init(SI5351Ax);
	}
	else if ((SI5351Ax != NULL) && (SI5351Ax == SI5351A_TASK_THREE))
	{
		SI5351A_I2C_Device2_Init(SI5351Ax);
	}
	else
	{
		return ERROR_1;
	}

	//---判断是硬件I2C还是软件I2C
	if (isHWI2C)
	{
		//_return= I2CTask_MHW_Init(&(WM8510x->msgI2C), pFuncDelay);
		SI5351Ax->msgI2C.msgModelIsHW = 1;
	}
	else
	{
		_return = I2CTask_MSW_Init(&(SI5351Ax->msgI2C), pFuncDelayus);
		SI5351Ax->msgI2C.msgModelIsHW = 0;
	}
	_return = SI5351A_I2C_START(SI5351Ax);
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_DeInit(SI5351A_HandlerType* SI5351Ax)
{
	//---注销I2C设备
	if (SI5351Ax->msgI2C.msgModelIsHW == 1)
	{
		return ERROR_1;
	}
	else
	{
		return I2CTask_MSW_DeInit(&(SI5351Ax->msgI2C));
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_I2C_START(SI5351A_HandlerType* SI5351Ax)
{
	//---读取ID信息
	UINT8_T _return = SI5351A_ReadID(SI5351Ax);
	if (_return==OK_0)
	{
		UINT8_T temp[8]={0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00};
		//---复位PLL
		SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_PLL_RST_ADDR, SI5351A_PLL_RST_B | SI5351A_PLL_RST_A);
		//---各通道进入PWR模式
		SI5351A_I2C_WriteBulk(SI5351Ax, SI5351A_REG_CLK0_ADDR,temp,3);
		//---设置驱动能力
		SI5351A_SetClockChannelIDRV(SI5351Ax,0, SI5351A_CLK_IDRV_2mA);
		//---变量归零处理
		memset(temp, 0x00, 8);
		SI5351A_I2C_WriteBulk(SI5351Ax, SI5351A_REG_SSC_EN_ADDR,temp,8);
		SI5351A_I2C_WriteBulk(SI5351Ax, 157, temp, 8);
		//---配置寄存器
		SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_FANOUT_ENABLE_ADDR, 0xC0);
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：复位PLL
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_PLLRST(SI5351A_HandlerType* SI5351Ax, UINT8_T pllIndex)
{
	if (pllIndex == SI5351A_PLL_RST_B)
	{
		return SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_PLL_RST_ADDR, SI5351A_PLL_RST_B);
	}
	else
	{
		return SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_PLL_RST_ADDR, SI5351A_PLL_RST_A);
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：读取ID信息,使用时钟内部负载电容的配置作为ID表示，默认上电之后有个初值0B11XXXXXX
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_ReadID(SI5351A_HandlerType* SI5351Ax)
{
	UINT8_T temp=0;
	UINT8_T  _return= SI5351A_I2C_ReadSingle(SI5351Ax, SI5351A_REG_CRYSTAL_LOAD_ADDR, &temp);
	if (_return==OK_0)
	{
		temp>>=6;
		if ((temp&0x03)==0x03)
		{
			_return=OK_0;
		}
		else
		{
			_return=ERROR_2;
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
UINT8_T SI5351A_SetFreqHz(SI5351A_HandlerType* SI5351Ax,UINT8_T clkChannel, UINT64_T freq)
{
	return SI5351A_CalcConfig(SI5351Ax, clkChannel,freq);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SetFreqKHz(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, float freqKHz)
{
	UINT64_T freq= (UINT64_T)freqKHz*1000;
	return SI5351A_CalcConfig(SI5351Ax, clkChannel, freq);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SetFreqMHz(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, float freqMHz)
{
	UINT64_T freq = (UINT64_T)freqMHz * 1000000;
	return SI5351A_CalcConfig(SI5351Ax, clkChannel, freq);
}

///////////////////////////////////////////////////////////////////////////////
//////函		数：
//////功		能：设置时钟输出通道的驱动能力
//////输入参数:
//////输出参数:
//////说		明：
//////////////////////////////////////////////////////////////////////////////
UINT8_T SI5351A_SetClockChannelIDRV(SI5351A_HandlerType* SI5351Ax, UINT8_T clkChannel, UINT8_T idrv)
{
	clkChannel=((clkChannel>(SI5351A_CLKOUT_CHANNEL_SIZE-1))?(SI5351A_CLKOUT_CHANNEL_SIZE-1):clkChannel);
	SI5351Ax->msgClockCTRL[clkChannel]&=0xFC;
	SI5351Ax->msgClockCTRL[clkChannel]|=idrv;
	return SI5351A_I2C_WriteSingle(SI5351Ax, SI5351A_REG_CLK0_ADDR+clkChannel, SI5351Ax->msgClockCTRL[clkChannel]);
}