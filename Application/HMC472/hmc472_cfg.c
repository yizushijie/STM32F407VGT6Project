#include "hmc472_cfg.h"

//===信号源默认功率
UINT16_T g_GenCHAdBm = HMC472_GEN_CHA_DBM;
UINT16_T g_GenCHBdBm = HMC472_GEN_CHB_DBM;
UINT16_T g_GenCHCdBm = HMC472_GEN_CHC_DBM;
UINT16_T g_GenCHDdBm = HMC472_GEN_CHD_DBM;

//===信号源的数控衰减器功率
UINT16_T g_HMC472GenCHAdBm = 0;
UINT8_T  g_HMC472GenCHAPower = 0;
UINT16_T g_HMC472GenCHBdBm = 0;
UINT8_T  g_HMC472GenCHBPower = 0;
UINT16_T g_HMC472GenCHCdBm = 0;
UINT8_T  g_HMC472GenCHCPower = 0;
UINT16_T g_HMC472GenCHDdBm = 0;
UINT8_T  g_HMC472GenCHDPower = 0;

//===RF数控衰减器8通道输出的功率
UINT16_T g_HMC472RFCHAdBm = 0;
UINT8_T	 g_HMC472RFCHAPower = 0;
UINT16_T g_HMC472RFCHBdBm = 0;
UINT8_T	 g_HMC472RFCHBPower = 0;
UINT16_T g_HMC472RFCHCdBm = 0;
UINT8_T	 g_HMC472RFCHCPower = 0;
UINT16_T g_HMC472RFCHDdBm = 0;
UINT8_T	 g_HMC472RFCHDPower = 0;

UINT16_T g_HMC472RFCHEdBm = 0;
UINT8_T	 g_HMC472RFCHEPower = 0;
UINT16_T g_HMC472RFCHFdBm = 0;
UINT8_T	 g_HMC472RFCHFPower = 0;
UINT16_T g_HMC472RFCHGdBm = 0;
UINT8_T	 g_HMC472RFCHGPower = 0;
UINT16_T g_HMC472RFCHHdBm = 0;
UINT8_T	 g_HMC472RFCHHPower = 0;

//===RF输出功率分档，默认输出都是-75dBm
VLTUINT8_T g_RFGendBmA = 40;  //---95===75+20
VLTUINT8_T g_RFGendBmB = 30;  //---90===75+15
VLTUINT8_T g_RFGendBmC = 50;  //---100===75+25
VLTUINT8_T g_RFGendBmD = 20;  //---85===75+10 20190318 增加将最小值为-85dBm

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 初始化GPIO的端口
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472_GPIO_Init( void )
{
	//---使能GPIO的时钟

	GPIOTask_Clock( HMC472_V1_PORT, 1 );
	GPIOTask_Clock( HMC472_V2_PORT, 1 );
	GPIOTask_Clock( HMC472_V3_PORT, 1 );
	GPIOTask_Clock( HMC472_V4_PORT, 1 );
	GPIOTask_Clock( HMC472_V5_PORT, 1 );
	GPIOTask_Clock( HMC472_V6_PORT, 1 );

	GPIOTask_Clock( HMC472_GENA_PORT, 1 );
	GPIOTask_Clock( HMC472_GENB_PORT, 1 );
	GPIOTask_Clock( HMC472_GENC_PORT, 1 );
	GPIOTask_Clock( HMC472_GEND_PORT, 1 );

	GPIOTask_Clock( HMC472_RFA_PORT, 1 );
	GPIOTask_Clock( HMC472_RFB_PORT, 1 );
	GPIOTask_Clock( HMC472_RFC_PORT, 1 );
	GPIOTask_Clock( HMC472_RFD_PORT, 1 );

	GPIOTask_Clock( HMC472_RFE_PORT, 1 );
	GPIOTask_Clock( HMC472_RFF_PORT, 1 );
	GPIOTask_Clock( HMC472_RFG_PORT, 1 );
	GPIOTask_Clock( HMC472_RFH_PORT, 1 );

	//---GPIO的结构体
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;						//---配置状态为输出模式
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;				//---GPIO的速度---高速设备
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;			//---输出模式---开漏输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;							//---上拉
#ifndef USE_MCU_STM32F1
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;						//---端口复用模式
#endif
	
	//---初始化HMC472的数控总线V1
	GPIO_InitStruct.Pin = HMC472_V1_BIT;
	LL_GPIO_Init( HMC472_V1_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V1_PORT, HMC472_V1_BIT );

	//---初始化HMC472的数控总线V2
	GPIO_InitStruct.Pin = HMC472_V2_BIT;
	LL_GPIO_Init( HMC472_V2_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V2_PORT, HMC472_V2_BIT );

	//---初始化HMC472的数控总线V3
	GPIO_InitStruct.Pin = HMC472_V3_BIT;
	LL_GPIO_Init( HMC472_V3_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V3_PORT, HMC472_V3_BIT );

	//---初始化HMC472的数控总线V4
	GPIO_InitStruct.Pin = HMC472_V4_BIT;
	LL_GPIO_Init( HMC472_V4_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V4_PORT, HMC472_V4_BIT );

	//---初始化HMC472的数控总线V5
	GPIO_InitStruct.Pin = HMC472_V5_BIT;
	LL_GPIO_Init( HMC472_V5_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V5_PORT, HMC472_V5_BIT );

	//---初始化HMC472的数控总线V6
	GPIO_InitStruct.Pin = HMC472_V6_BIT;
	LL_GPIO_Init( HMC472_V6_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_V6_PORT, HMC472_V6_BIT );
	
	//---74HC573D使能端口配置
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;			//---输出模式---推挽输出
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;						//---下拉

	GPIO_InitStruct.Pin = HMC472_GENA_BIT;
	LL_GPIO_Init( HMC472_GENA_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_GENA_PORT, HMC472_GENA_BIT );

	GPIO_InitStruct.Pin = HMC472_GENB_BIT;
	LL_GPIO_Init( HMC472_GENB_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_GENB_PORT, HMC472_GENB_BIT );
    
	GPIO_InitStruct.Pin = HMC472_GENC_BIT;
	LL_GPIO_Init( HMC472_GENC_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_GENC_PORT, HMC472_GENC_BIT );

	GPIO_InitStruct.Pin = HMC472_GEND_BIT;
	LL_GPIO_Init( HMC472_GEND_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_GEND_PORT, HMC472_GEND_BIT );

	GPIO_InitStruct.Pin = HMC472_RFA_BIT;
	LL_GPIO_Init( HMC472_RFA_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFA_PORT, HMC472_RFA_BIT );

	GPIO_InitStruct.Pin = HMC472_RFB_BIT;
	LL_GPIO_Init( HMC472_RFB_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFB_PORT, HMC472_RFB_BIT );

	GPIO_InitStruct.Pin = HMC472_RFC_BIT;
	LL_GPIO_Init( HMC472_RFC_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFC_PORT, HMC472_RFC_BIT );

	GPIO_InitStruct.Pin = HMC472_RFD_BIT;
	LL_GPIO_Init( HMC472_RFD_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFD_PORT, HMC472_RFD_BIT );

	GPIO_InitStruct.Pin = HMC472_RFE_BIT;
	LL_GPIO_Init( HMC472_RFE_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFE_PORT, HMC472_RFE_BIT );

	GPIO_InitStruct.Pin = HMC472_RFF_BIT;
	LL_GPIO_Init( HMC472_RFF_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFF_PORT, HMC472_RFF_BIT );

	GPIO_InitStruct.Pin = HMC472_RFG_BIT;
	LL_GPIO_Init( HMC472_RFG_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFG_PORT, HMC472_RFG_BIT );

	GPIO_InitStruct.Pin = HMC472_RFH_BIT;
	LL_GPIO_Init( HMC472_RFH_PORT, &GPIO_InitStruct );
	GPIO_OUT_1( HMC472_RFH_PORT, HMC472_RFH_BIT );
	
	//---等待操作完成
	DelayTask_us( 10 );

	//---所存数据
	H74C573_LATCH_DISABLE;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472_Init( void )
{
	HMC472_GPIO_Init();
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能：设置HMC472的数据总线控制，用于控制衰减的步进 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472_DataBus( UINT8_T val )
{
	//---D0
	if (val & 0x01)
	{
		HMC472_V6_OUT_1;
	}
	else
	{
		HMC472_V6_OUT_0;
	}

	//---D1
	if (val & 0x02)
	{
		HMC472_V5_OUT_1;
	}
	else
	{
		HMC472_V5_OUT_0;
	}

	//---D2
	if (val & 0x04)
	{
		HMC472_V4_OUT_1;
	}
	else
	{
		HMC472_V4_OUT_0;
	}

	//---D3
	if (val & 0x08)
	{
		HMC472_V3_OUT_1;
	}
	else
	{
		HMC472_V3_OUT_0;
	}

	//---D4
	if (val & 0x10)
	{
		HMC472_V2_OUT_1;
	}
	else
	{
		HMC472_V2_OUT_0;
	}

	//---D5
	if (val & 0x20)
	{
		HMC472_V1_OUT_1;
	}
	else
	{
		HMC472_V1_OUT_0;
	}
}
///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 设置HMC的步进增益
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
void HMC472_SetGain( UINT8_T val )
{
	switch (val)
	{
		case HMC472_NONE_DBM:
			HMC472_BUS_OUT( 0x3F );
			break;
		case HMC472_05_DBM:
			HMC472_BUS_OUT( 0x3E );
			break;
		case HMC472_10_DBM:
			HMC472_BUS_OUT( 0x3D );
			break;
		case HMC472_15_DBM:
			HMC472_BUS_OUT( 0x3C );
			break;
		case HMC472_20_DBM:
			HMC472_BUS_OUT( 0x3B );
			break;
		case HMC472_25_DBM:
			HMC472_BUS_OUT( 0x3A );
			break;
		case HMC472_30_DBM:
			HMC472_BUS_OUT( 0x39 );
			break;
		case HMC472_35_DBM:
			HMC472_BUS_OUT( 0x38 );
			break;
		case HMC472_40_DBM:
			HMC472_BUS_OUT( 0x37 );
			break;
		case HMC472_45_DBM:
			HMC472_BUS_OUT( 0x36 );
			break;
		case HMC472_50_DBM:
			HMC472_BUS_OUT( 0x35 );
			break;
		case HMC472_55_DBM:
			HMC472_BUS_OUT( 0x34 );
			break;
		case HMC472_60_DBM:
			HMC472_BUS_OUT( 0x33 );
			break;
		case HMC472_65_DBM:
			HMC472_BUS_OUT( 0x32 );
			break;
		case HMC472_70_DBM:
			HMC472_BUS_OUT( 0x31 );
			break;
		case HMC472_75_DBM:
			HMC472_BUS_OUT( 0x30 );
			break;
		case HMC472_80_DBM:
			HMC472_BUS_OUT( 0x2F );
			break;
		case HMC472_85_DBM:
			HMC472_BUS_OUT( 0x2E );
			break;
		case HMC472_90_DBM:
			HMC472_BUS_OUT( 0x2D );
			break;
		case HMC472_95_DBM:
			HMC472_BUS_OUT( 0x2C );
			break;
		case HMC472_100_DBM:
			HMC472_BUS_OUT( 0x2B );
			break;
		case HMC472_105_DBM:
			HMC472_BUS_OUT( 0x2A );
			break;
		case HMC472_110_DBM:
			HMC472_BUS_OUT( 0x29 );
			break;
		case HMC472_115_DBM:
			HMC472_BUS_OUT( 0x28 );
			break;
		case HMC472_120_DBM:
			HMC472_BUS_OUT( 0x27 );
			break;
		case HMC472_125_DBM:
			HMC472_BUS_OUT( 0x26 );
			break;
		case HMC472_130_DBM:
			HMC472_BUS_OUT( 0x25 );
			break;
		case HMC472_135_DBM:
			HMC472_BUS_OUT( 0x24 );
			break;
		case HMC472_140_DBM:
			HMC472_BUS_OUT( 0x23 );
			break;
		case HMC472_145_DBM:
			HMC472_BUS_OUT( 0x22 );
			break;
		case HMC472_150_DBM:
			HMC472_BUS_OUT( 0x21 );
			break;
		case HMC472_155_DBM:
			HMC472_BUS_OUT( 0x20 );
			break;
		case HMC472_160_DBM:
			HMC472_BUS_OUT( 0x1F );
			break;
		case HMC472_165_DBM:
			HMC472_BUS_OUT( 0x1E );
			break;
		case HMC472_170_DBM:
			HMC472_BUS_OUT( 0x1D );
			break;
		case HMC472_175_DBM:
			HMC472_BUS_OUT( 0x1C );
			break;
		case HMC472_180_DBM:
			HMC472_BUS_OUT( 0x1B );
			break;
		case HMC472_185_DBM:
			HMC472_BUS_OUT( 0x1A );
			break;
		case HMC472_190_DBM:
			HMC472_BUS_OUT( 0x19 );
			break;
		case HMC472_195_DBM:
			HMC472_BUS_OUT( 0x18 );
			break;
		case HMC472_200_DBM:
			HMC472_BUS_OUT( 0x17 );
			break;
		case HMC472_205_DBM:
			HMC472_BUS_OUT( 0x16 );
			break;
		case HMC472_210_DBM:
			HMC472_BUS_OUT( 0x15 );
			break;
		case HMC472_215_DBM:
			HMC472_BUS_OUT( 0x14 );
			break;
		case HMC472_220_DBM:
			HMC472_BUS_OUT( 0x13 );
			break;
		case HMC472_225_DBM:
			HMC472_BUS_OUT( 0x12 );
			break;
		case HMC472_230_DBM:
			HMC472_BUS_OUT( 0x11 );
			break;
		case HMC472_235_DBM:
			HMC472_BUS_OUT( 0x10 );
			break;
		case HMC472_240_DBM:
			HMC472_BUS_OUT( 0x0F );
			break;
		case HMC472_245_DBM:
			HMC472_BUS_OUT( 0x0E );
			break;
		case HMC472_250_DBM:
			HMC472_BUS_OUT( 0x0D );
			break;
		case HMC472_255_DBM:
			HMC472_BUS_OUT( 0x0C );
			break;
		case HMC472_260_DBM:
			HMC472_BUS_OUT( 0x0B );
			break;
		case HMC472_265_DBM:
			HMC472_BUS_OUT( 0x0A );
			break;
		case HMC472_270_DBM:
			HMC472_BUS_OUT( 0x09 );
			break;
		case HMC472_275_DBM:
			HMC472_BUS_OUT( 0x08 );
			break;
		case HMC472_280_DBM:
			HMC472_BUS_OUT( 0x07 );
			break;
		case HMC472_285_DBM:
			HMC472_BUS_OUT( 0x06 );
			break;
		case HMC472_290_DBM:
			HMC472_BUS_OUT( 0x05 );
			break;
		case HMC472_295_DBM:
			HMC472_BUS_OUT( 0x04 );
			break;
		case HMC472_300_DBM:
			HMC472_BUS_OUT( 0x03 );
			break;
		case HMC472_305_DBM:
			HMC472_BUS_OUT( 0x02 );
			break;
		case HMC472_310_DBM:
			HMC472_BUS_OUT( 0x01 );
			break;
		case HMC472_315_DBM:
			HMC472_BUS_OUT( 0x00 );
			break;
		default:
			HMC472_BUS_OUT( 0x3F );
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472_SetGenGain( UINT8_T genCh, UINT8_T gainVal )
{
	UINT8_T _return=0;
	//---计算增益的等级
	UINT16_T genGain = gainVal;
	genGain*=5;
	//---设置衰减器的衰减值
	HMC472_SetGain( gainVal );
	//---选择RF信号源
	switch (genCh)
	{
		case HMC472_GEN_NONE:
			_return = 1;
			break;
		case HMC472_GEN_CHA:
			H74C573_GENA_LATCH_ENABLE;
			g_HMC472GenCHAdBm = genGain;
			g_HMC472GenCHAPower = gainVal;
			break;
		case HMC472_GEN_CHB:
			H74C573_GENB_LATCH_ENABLE;
			g_HMC472GenCHBdBm = genGain;
			g_HMC472GenCHBPower = gainVal;
			break;
		case HMC472_GEN_CHC:
			g_HMC472GenCHCdBm = genGain;
			g_HMC472GenCHCPower = gainVal;
			H74C573_GENC_LATCH_ENABLE;
			break;
		case HMC472_GEN_CHD:
			H74C573_GEND_LATCH_ENABLE;
			g_HMC472GenCHDdBm = genGain;
			g_HMC472GenCHDPower = gainVal;
			break;
		default:
			_return = 1;
			break;
	}
	//---等待数据加载成功
	DelayTask_us(10);
	//---锁定数据
	H74C573_GEN_LATCH_DISABLE;
	
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472_SetRFGain( UINT8_T rfCH, UINT8_T gainVal )
{
	UINT8_T _return = 0;	
	//---计算增益的等级
	UINT16_T rfGain = gainVal;
	//---计算信号源功率
	rfGain *= 5;
	//---设置衰减器的衰减值
	HMC472_SetGain( gainVal );
	//---选择RF输出信号
	switch (rfCH)
	{
		case HMC472_RF_NONE:
			_return = 1;
			break;
		case HMC472_RF_CHA:
			H74C573_RFA_LATCH_ENABLE;
			g_HMC472RFCHAdBm = rfGain;
			g_HMC472RFCHAPower = gainVal;
			break;
		case HMC472_RF_CHB:
			H74C573_RFB_LATCH_ENABLE;
			g_HMC472RFCHBdBm = rfGain;
			g_HMC472RFCHBPower = gainVal;
			break;
		case HMC472_RF_CHC:
			H74C573_RFC_LATCH_ENABLE;
			g_HMC472RFCHCdBm = rfGain;
			g_HMC472RFCHCPower = gainVal;
			break;
		case HMC472_RF_CHD:
			H74C573_RFD_LATCH_ENABLE;
			g_HMC472RFCHDdBm = rfGain;
			g_HMC472RFCHDPower = gainVal;
			break;
		case HMC472_RF_CHE:
			H74C573_RFE_LATCH_ENABLE;
			g_HMC472RFCHEdBm = rfGain;
			g_HMC472RFCHEPower = gainVal;
			break;
		case HMC472_RF_CHF:
			H74C573_RFF_LATCH_ENABLE;
			g_HMC472RFCHFdBm = rfGain;
			g_HMC472RFCHFPower = gainVal;
			break;
		case HMC472_RF_CHG:
			H74C573_RFG_LATCH_ENABLE;
			g_HMC472RFCHGdBm = rfGain;
			g_HMC472RFCHGPower = gainVal;
			break;
		case HMC472_RF_CHH:
			H74C573_RFH_LATCH_ENABLE;
			g_HMC472RFCHHdBm = rfGain;
			g_HMC472RFCHHPower = gainVal;
			break;
		default:
			_return = 1;
			break;
	}
	//---等待数据加载成功
	DelayTask_us(10);
	//---锁定数据
	H74C573_RF_LATCH_DISABLE;
	
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 获取信号源的功率
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472_ReadGen( UINT8_T genCH )
{
	UINT16_T _return = 0;

	//---计算信号源的功率
	switch (genCH)
	{
		case HMC472_GEN_NONE:
			_return = 0;
			break;
		case HMC472_GEN_CHA:
			_return = g_GenCHAdBm;
			break;
		case HMC472_GEN_CHB:
			_return = g_GenCHBdBm;
			break;
		case HMC472_GEN_CHC:
			_return = g_GenCHCdBm;
			break;
		case HMC472_GEN_CHD:
			_return = g_GenCHDdBm;
			break;
		default:
			_return = 0;
			break;
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
UINT8_T  HMC472_SetGen(UINT8_T genCH, UINT16_T genVal )
{
	UINT8_T _return = 0;

	//---计算信号源的功率
	switch (genCH)
	{
		case HMC472_GEN_NONE:
			_return = 1;
			break;
		case HMC472_GEN_CHA:
			 g_GenCHAdBm=genVal;
			break;
		case HMC472_GEN_CHB:
			g_GenCHBdBm=genVal;
			break;
		case HMC472_GEN_CHC:
			g_GenCHCdBm=genVal;
			break;
		case HMC472_GEN_CHD:
			g_GenCHDdBm=genVal;
			break;
		default:
			_return = 1;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 获取第一级4通道的衰减功率
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472_ReadHMC472Gen( UINT8_T genCH )
{
	UINT16_T _return = 0;

	//---计算信号源的功率
	switch (genCH)
	{
		case HMC472_GEN_NONE:
			_return = 0;
			break;
		case HMC472_GEN_CHA:
			_return = g_HMC472GenCHAdBm;
			break;
		case HMC472_GEN_CHB:
			_return = g_HMC472GenCHBdBm;
			break;
		case HMC472_GEN_CHC:
			_return = g_HMC472GenCHCdBm;
			break;
		case HMC472_GEN_CHD:
			_return = g_HMC472GenCHDdBm;
			break;
		default:
			_return = 0;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 获取第二级8通道的衰减功率
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472_ReadHMC472RF(UINT8_T rfCH )
{
	UINT16_T _return = 0;
	//---选择RF输出信号
	switch (rfCH)
	{
		case HMC472_RF_NONE:
			_return = 0;
			break;
		case HMC472_RF_CHA:
			_return = g_HMC472RFCHAdBm;
			break;
		case HMC472_RF_CHB:
			_return = g_HMC472RFCHBdBm;
			break;
		case HMC472_RF_CHC:
			_return = g_HMC472RFCHCdBm;
			break;
		case HMC472_RF_CHD:
			_return = g_HMC472RFCHDdBm;
			break;
		case HMC472_RF_CHE:
			_return = g_HMC472RFCHEdBm;
			break;
		case HMC472_RF_CHF:
			_return = g_HMC472RFCHFdBm;
			break;
		case HMC472_RF_CHG:
			_return = g_HMC472RFCHGdBm;
			break;
		case HMC472_RF_CHH:
			_return = g_HMC472RFCHHdBm;
			break;
		default:
			_return = 0;
			break;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 获取信号源输出端的功率
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT16_T HMC472_ReadRFGen( UINT8_T genCH, UINT8_T rfCH )
{
	UINT16_T _return = 0;

	//---计算信号源的功率
	switch (genCH)
	{
		case HMC472_GEN_NONE:
			_return = 0;
			break;
		case HMC472_GEN_CHA:
			_return = g_GenCHAdBm;
			_return += g_HMC472GenCHAdBm;
			break;
		case HMC472_GEN_CHB:
			_return = g_GenCHBdBm;
			_return += g_HMC472GenCHBdBm;
			break;
		case HMC472_GEN_CHC:
			_return = g_GenCHCdBm;
			_return += g_HMC472GenCHCdBm;
			break;
		case HMC472_GEN_CHD:
			_return = g_GenCHDdBm;
			_return += g_HMC472GenCHDdBm;
			break;
		default:
			_return = 0;
			break;
	}

	//---如果信号源功率错误，则直接返回
	if (_return!=0)
	{
		//---选择RF输出信号
		switch (rfCH)
		{
			case HMC472_RF_NONE:
				_return = 0;
				break;
			case HMC472_RF_CHA:
				_return+=g_HMC472RFCHAdBm ;
				break;
			case HMC472_RF_CHB:
				_return+=g_HMC472RFCHBdBm ;
				break;
			case HMC472_RF_CHC:
				_return+=g_HMC472RFCHCdBm ;
				break;
			case HMC472_RF_CHD:
				_return+=g_HMC472RFCHDdBm ;
				break;
			case HMC472_RF_CHE:
				_return+=g_HMC472RFCHEdBm ;
				break;
			case HMC472_RF_CHF:
				_return+=g_HMC472RFCHFdBm ;
				break;
			case HMC472_RF_CHG:
				_return+=g_HMC472RFCHGdBm ;
				break;
			case HMC472_RF_CHH:
				_return+=g_HMC472RFCHHdBm ;
				break;
			default:
				_return = 0;
				break;
		}
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 设置信号源最后输出的功率
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472_SetRFGen( UINT8_T genCH,UINT8_T rfCH, UINT16_T rfGenPower )
{
	UINT8_T _return = OK_0;
	UINT8_T gainVal = 0;
	//---输出信号的功率
	UINT16_T rfPower = 0;
	//---计算信号源的功率
	switch (genCH)
	{
		case HMC472_GEN_NONE:
			rfPower = 0;
			break;
		case HMC472_GEN_CHA:
			rfPower = g_GenCHAdBm;
			rfPower += g_HMC472GenCHAdBm;
			break;
		case HMC472_GEN_CHB:
			rfPower = g_GenCHBdBm;
			rfPower += g_HMC472GenCHBdBm;
			break;
		case HMC472_GEN_CHC:
			rfPower = g_GenCHCdBm;
			rfPower += g_HMC472GenCHCdBm;
			break;
		case HMC472_GEN_CHD:
			rfPower = g_GenCHDdBm;
			rfPower += g_HMC472GenCHDdBm;
			break;
		default:
			rfPower = 0;
			break;
	}
	if (rfPower!=0)
	{
		//---计算第二级衰减器的衰减值
		if (rfGenPower>rfPower)
		{
			gainVal = (UINT8_T)( ( rfGenPower - rfPower ) / 5 );
		}
		else
		{
			gainVal = 0;
		}

		//---设置第二级的衰减
		HMC472_SetRFGain( rfCH, gainVal );
	}
	else
	{
		_return = ERROR_2;
	}
	return _return;
}

///////////////////////////////////////////////////////////////////////////////
//////函		数： 
//////功		能： 版本管理
//////输入参数: 
//////输出参数: 
//////说		明： 
//////////////////////////////////////////////////////////////////////////////
UINT8_T HMC472_GetVersion( UINT8_T *pVersion,UINT8_T length )
{
	UINT8_T _return = 0;
	if (length>=HMC472_GEN_VERSION_SIZE)
	{
		sprintf( (char *)pVersion, HMC472_GEN_VERSION );
		_return=HMC472_GEN_VERSION_SIZE;
	}
	return _return;
}