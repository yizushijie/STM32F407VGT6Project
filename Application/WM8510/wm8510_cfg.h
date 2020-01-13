#ifndef WM8510_CFG_H_
#define WM8510_CFG_H_
//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////////////////
	//===包含的头文件
	#include "complier_lib.h"
	#include "timer_task.h"
	#include "i2c_task.h"
	#include "hw_cfg.h"
	//////////////////////////////////////////////////////////////////////////////////////

	//===每100KHz需要补偿15Hz
	//===每20KHz需要补偿3Hz
	//=== 12M时钟下最小输出频率是156KHz----单位为HZ
	#define WM8510_MCLK_HZ				12000000UL
	#define WM8510_MCLK_KHZ				( WM8510_MCLK_HZ/1000UL )
	#define WM8510_MCLK_MHZ				( WM8510_MCLK_HZ/1000000UL )
	
	//---PLL计算时能取值的最小值
	#define WM8510_PLLR_MIN				5
	
	//---PLL计算时能取值的最大值
	#define WM8510_PLLR_MAX				13
	
	//---PLL计算时能取值的最小值限制值
	#define WM8510_PLLR_MIN_LIMIT		6
	
	//---频率输出----在12MHz时钟条件下，设定最大输出38MHz，最小输出20KHz
	#define PDIV0_MDIV1_BDIV1_MAX		( WM8510_MCLK_HZ * WM8510_PLLR_MAX / 4					)																					//39000000
	
	#define PDIV0_MDIV1_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4				)																					//18000000
	#define PDIV0_MDIV1D5_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/4			)																					//12000000
	#define PDIV0_MDIV2_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/4			)																					//9000000
	#define PDIV0_MDIV3_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/4			)																					//6000000
	#define PDIV0_MDIV4_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/4			)																					//4500000
	#define PDIV0_MDIV6_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/4			)																					//3000000
	#define PDIV0_MDIV8_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/4			)																					//2250000
	#define PDIV0_MDIV12_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/4			)																					//1500000
	
	#define PDIV0_MDIV1_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/4			)																					//9000000
	#define PDIV0_MDIV1D5_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/4		)																					//6000000
	#define PDIV0_MDIV2_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/2/4			)																					//4500000
	#define PDIV0_MDIV3_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/2/4			)																					//3000000
	#define PDIV0_MDIV4_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/2/4			)																					//2250000
	#define PDIV0_MDIV6_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/2/4			)																					//1500000
	#define PDIV0_MDIV8_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/2/4			)																					//1125000
	#define PDIV0_MDIV12_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/2/4		)																					//0750000
	
	#define PDIV0_MDIV1_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/4			)																					//4500000
	#define PDIV0_MDIV1D5_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/4/4		)																					//3000000
	#define PDIV0_MDIV2_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/4/4			)																					//2250000
	#define PDIV0_MDIV3_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/4/4			)																					//1500000
	#define PDIV0_MDIV4_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/4/4			)																					//1125000
	#define PDIV0_MDIV6_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/4/4			)																					//0750000
	#define PDIV0_MDIV8_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/4/4			)																					//0562500
	#define PDIV0_MDIV12_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/4/4		)																					//0375000
	
	#define PDIV0_MDIV1_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/4			)																					//2250000
	#define PDIV0_MDIV1D5_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/8/4		)																					//1500000
	#define PDIV0_MDIV2_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/8/4			)																					//1125000
	#define PDIV0_MDIV3_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/8/4			)																					//0750000
	#define PDIV0_MDIV4_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/8/4			)																					//0562500
	#define PDIV0_MDIV6_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/8/4			)																					//0375000
	#define PDIV0_MDIV8_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/8/4			)																					//0281250
	#define PDIV0_MDIV12_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/8/4		)																					//0187500
	
	#define PDIV0_MDIV1_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /16/4			)																					//1125000
	#define PDIV0_MDIV1D5_BDIV16_MIN	( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/16/4		)																					//0750000
	#define PDIV0_MDIV2_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/16/4		)																					//0562500
	#define PDIV0_MDIV3_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/16/4		)																					//0375000
	#define PDIV0_MDIV4_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/16/4		)																					//0281250
	#define PDIV0_MDIV6_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/16/4		)																					//0187500
	#define PDIV0_MDIV8_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/16/4		)																					//0140625
	#define PDIV0_MDIV12_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/16/4		)																					//0093750
	
	#define PDIV0_MDIV1_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /32/4			)																					//0562500
	#define PDIV0_MDIV1D5_BDIV32_MIN	( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/32/4		)																					//0375000
	#define PDIV0_MDIV2_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /2/32/4		)																					//0281250
	#define PDIV0_MDIV3_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /3/32/4		)																					//0187500
	#define PDIV0_MDIV4_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /4/32/4		)																					//0140625
	#define PDIV0_MDIV6_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /6/32/4		)																					//0093750
	#define PDIV0_MDIV8_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /8/32/4		)																					//0070313
	#define PDIV0_MDIV12_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT /12/32/4		)																					//0046875
	
	#define PDIV1_MDIV1_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4			)																					//9000000
	#define PDIV1_MDIV1D5_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/4		)																					//6000000
	#define PDIV1_MDIV2_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/4			)																					//4500000
	#define PDIV1_MDIV3_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/4			)																					//3000000
	#define PDIV1_MDIV4_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/4			)																					//2250000
	#define PDIV1_MDIV6_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/4			)																					//1500000
	#define PDIV1_MDIV8_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/4			)																					//1125000
	#define PDIV1_MDIV12_BDIV1_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/4		)																					//0750000
	
	#define PDIV1_MDIV1_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/4			)																					//4500000
	#define PDIV1_MDIV1D5_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/2/4		)																					//3000000
	#define PDIV1_MDIV2_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/2/4		)																					//2250000
	#define PDIV1_MDIV3_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/2/4		)																					//1500000
	#define PDIV1_MDIV4_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/2/4		)																					//1125000
	#define PDIV1_MDIV6_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/2/4		)																					//0750000
	#define PDIV1_MDIV8_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/2/4		)																					//0562500
	#define PDIV1_MDIV12_BDIV2_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/2/4		)																					//0375000
	
	#define PDIV1_MDIV1_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/4			)																					//2250000
	#define PDIV1_MDIV1D5_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/4/4		)																					//1500000
	#define PDIV1_MDIV2_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/4/4		)																					//1125000
	#define PDIV1_MDIV3_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/4/4		)																					//0750000
	#define PDIV1_MDIV4_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/4/4		)																					//0562500
	#define PDIV1_MDIV6_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/4/4		)																					//0375000
	#define PDIV1_MDIV8_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/4/4		)																					//0281250
	#define PDIV1_MDIV12_BDIV4_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/4/4		)																					//0187500
	
	#define PDIV1_MDIV1_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/4			)																					//1125000
	#define PDIV1_MDIV1D5_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/8/4		)																					//0750000
	#define PDIV1_MDIV2_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/8/4		)																					//0562500
	#define PDIV1_MDIV3_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/8/4		)																					//0375000
	#define PDIV1_MDIV4_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/8/4		)																					//0281250
	#define PDIV1_MDIV6_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/8/4		)																					//0187500
	#define PDIV1_MDIV8_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/8/4		)																					//0140625
	#define PDIV1_MDIV12_BDIV8_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/8/4		)																					//0093750
	
	#define PDIV1_MDIV1_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /16/4		)																					//0562500
	#define PDIV1_MDIV1D5_BDIV16_MIN	( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/16/4		)																					//0375000
	#define PDIV1_MDIV2_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/16/4		)																					//0281250
	#define PDIV1_MDIV3_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/16/4		)																					//0187500
	#define PDIV1_MDIV4_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/16/4		)																					//0140625
	#define PDIV1_MDIV6_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/16/4		)																					//0093750
	#define PDIV1_MDIV8_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/16/4		)																					//0070313
	#define PDIV1_MDIV12_BDIV16_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/16/4		)																					//0046875
	
	#define PDIV1_MDIV1_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /32/4		)																					//0281250
	#define PDIV1_MDIV1D5_BDIV32_MIN	( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT*2/3/2/32/4		)																					//0187500
	#define PDIV1_MDIV2_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /2/32/4		)																					//0140625
	#define PDIV1_MDIV3_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /3/32/4		)																					//0093750
	#define PDIV1_MDIV4_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /4/32/4		)																					//0070313
	#define PDIV1_MDIV6_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /6/32/4		)																					//0046875
	#define PDIV1_MDIV8_BDIV32_MIN		( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /8/32/4		)																					//0035156
	#define PDIV1_MDIV12_BDIV32_MIN_1	( WM8510_MCLK_HZ * WM8510_PLLR_MIN_LIMIT/2 /12/32/4		)																					//0023438
	#define PDIV1_MDIV12_BDIV32_MIN_2	( WM8510_MCLK_HZ * WM8510_PLLR_MIN/2 /12/32/4			)																					//0019531
	#define PDIV1_MDIV12_BDIV32_MIN		PDIV1_MDIV12_BDIV32_MIN_2
	
	#define WM8510_OUT_MAX_FREQ			( (WM8510_MCLK_MHZ*WM8510_PLLR_MAX /4 -1)*1000000UL )																						//38000000
	#define WM8510_OUT_MAX_FREQ_ADD		( (WM8510_MCLK_MHZ*WM8510_PLLR_MAX /4 -1)*150 )
	#define WM8510_OUT_MIN_FREQ			( ((PDIV1_MDIV12_BDIV32_MIN/1000)+2)*1000UL )																								//20000
	#define WM8510_OUT_MIN_FREQ_ADD		( (UINT8_T)((WM8510_OUT_MIN_FREQ*15+50000)/100000UL) )
	
	#define MCLK_MDIV1_BDIV1_MIN		( WM8510_MCLK_HZ										)																					//12000000
	#define MCLK_MDIV1D5_BDIV1_MIN		( WM8510_MCLK_HZ*2/3									)																					//08000000
	#define MCLK_MDIV2_BDIV1_MIN		( WM8510_MCLK_HZ/2										)																					//06000000
	#define MCLK_MDIV3_BDIV1_MIN		( WM8510_MCLK_HZ/3										)																					//04000000
	#define MCLK_MDIV4_BDIV1_MIN		( WM8510_MCLK_HZ/4										)																					//03000000
	#define MCLK_MDIV6_BDIV1_MIN		( WM8510_MCLK_HZ/6										)																					//02000000
	#define MCLK_MDIV8_BDIV1_MIN		( WM8510_MCLK_HZ/8										)																					//01500000
	#define MCLK_MDIV12_BDIV1_MIN		( WM8510_MCLK_HZ/12										)																					//01000000
	
	#define MCLK_MDIV1_BDIV2_MIN		( WM8510_MCLK_HZ/2										)																					//6000000
	#define MCLK_MDIV1D5_BDIV2_MIN		( WM8510_MCLK_HZ*2/3/2									)																					//4000000
	#define MCLK_MDIV2_BDIV2_MIN		( WM8510_MCLK_HZ/2/2									)																					//3000000
	#define MCLK_MDIV3_BDIV2_MIN		( WM8510_MCLK_HZ/3/2									)																					//2000000
	#define MCLK_MDIV4_BDIV2_MIN		( WM8510_MCLK_HZ/4/2									)																					//1500000
	#define MCLK_MDIV6_BDIV2_MIN		( WM8510_MCLK_HZ/6/2									)																					//1000000
	#define MCLK_MDIV8_BDIV2_MIN		( WM8510_MCLK_HZ/8/2									)																					//0750000
	#define MCLK_MDIV12_BDIV2_MIN		( WM8510_MCLK_HZ/12/2									)																					//0500000
	
	#define MCLK_MDIV1_BDIV4_MIN		( WM8510_MCLK_HZ/4										)																					//3000000
	#define MCLK_MDIV1D5_BDIV4_MIN		( WM8510_MCLK_HZ*2/3/4									)																					//2000000
	#define MCLK_MDIV2_BDIV4_MIN		( WM8510_MCLK_HZ/2/4									)																					//1500000
	#define MCLK_MDIV3_BDIV4_MIN		( WM8510_MCLK_HZ/3/4									)																					//1000000
	#define MCLK_MDIV4_BDIV4_MIN		( WM8510_MCLK_HZ/4/4									)																					//0750000
	#define MCLK_MDIV6_BDIV4_MIN		( WM8510_MCLK_HZ/6/4									)																					//0500000
	#define MCLK_MDIV8_BDIV4_MIN		( WM8510_MCLK_HZ/8/4									)																					//0375000
	#define MCLK_MDIV12_BDIV4_MIN		( WM8510_MCLK_HZ/12/4									)																					//0250000
	
	#define MCLK_MDIV1_BDIV8_MIN		( WM8510_MCLK_HZ/8										)																					//1500000
	#define MCLK_MDIV1D5_BDIV8_MIN		( WM8510_MCLK_HZ*2/3/8									)																					//1000000
	#define MCLK_MDIV2_BDIV8_MIN		( WM8510_MCLK_HZ/2/8									)																					//0750000
	#define MCLK_MDIV3_BDIV8_MIN		( WM8510_MCLK_HZ/3/8									)																					//0500000
	#define MCLK_MDIV4_BDIV8_MIN		( WM8510_MCLK_HZ/4/8									)																					//0375000
	#define MCLK_MDIV6_BDIV8_MIN		( WM8510_MCLK_HZ/6/8									)																					//0250000
	#define MCLK_MDIV8_BDIV8_MIN		( WM8510_MCLK_HZ/8/8									)																					//0187500
	#define MCLK_MDIV12_BDIV8_MIN		( WM8510_MCLK_HZ/12/8									)																					//0125000
	
	#define MCLK_MDIV1_BDIV16_MIN		( WM8510_MCLK_HZ/16										)																					//0750000
	#define MCLK_MDIV1D5_BDIV16_MIN		( WM8510_MCLK_HZ*2/3									)																					//0500000
	#define MCLK_MDIV2_BDIV16_MIN		( WM8510_MCLK_HZ/2/16									)																					//0375000
	#define MCLK_MDIV3_BDIV16_MIN		( WM8510_MCLK_HZ/3/16									)																					//0250000
	#define MCLK_MDIV4_BDIV16_MIN		( WM8510_MCLK_HZ/4/16									)																					//0187500
	#define MCLK_MDIV6_BDIV16_MIN		( WM8510_MCLK_HZ/6/16									)																					//0125000
	#define MCLK_MDIV8_BDIV16_MIN		( WM8510_MCLK_HZ/8/16									)																					//0093750
	#define MCLK_MDIV12_BDIV16_MIN		( WM8510_MCLK_HZ/12/16									)																					//0062500
	
	#define MCLK_MDIV1_BDIV32_MIN		( WM8510_MCLK_HZ/32										)																					//0375000
	#define MCLK_MDIV1D5_BDIV32_MIN		( WM8510_MCLK_HZ*2/3/32									)																					//0250000
	#define MCLK_MDIV2_BDIV32_MIN		( WM8510_MCLK_HZ/2/32									)																					//0187500
	#define MCLK_MDIV3_BDIV32_MIN		( WM8510_MCLK_HZ/3/32									)																					//0125000
	#define MCLK_MDIV4_BDIV32_MIN		( WM8510_MCLK_HZ/4/32									)																					//0093750
	#define MCLK_MDIV6_BDIV32_MIN		( WM8510_MCLK_HZ/6/32									)																					//0062500
	#define MCLK_MDIV8_BDIV32_MIN		( WM8510_MCLK_HZ/8/32									)																					//0046875
	#define MCLK_MDIV12_BDIV32_MIN		( WM8510_MCLK_HZ/12/32									)																					//0031250
	
	//===MCLK分频数
	enum WM8510_MCLKDIV_ENUM
	{
		MCLK_DIV_1 = 0,
		MCLK_DIV_1D5 = 1,
		MCLK_DIV_2 = 2,
		MCLK_DIV_3 = 3,
		MCLK_DIV_4 = 4,
		MCLK_DIV_6 = 5,
		MCLK_DIV_8 = 6,
		MCLK_DIV_12 = 7,
	};

	//===BCLK分频数
	enum WM8510_BCLKDIV_ENUM
	{
		BCLK_DIV_1 = 0,
		BCLK_DIV_2 = 1,
		BCLK_DIV_4 = 2,
		BCLK_DIV_8 = 3,
		BCLK_DIV_16 = 4,
		BCLK_DIV_32 = 5,
	};

	//===W8510寄存器地址
	#define WM8510_R0_ADDR					(0<<1)
	#define WM8510_R1_ADDR					(1<<1)
	#define WM8510_R2_ADDR					(2<<1)
	#define WM8510_R3_ADDR					(3<<1)
	#define WM8510_R4_ADDR					(4<<1)
	#define WM8510_R5_ADDR					(5<<1)
	#define WM8510_R6_ADDR					(6<<1)
	#define WM8510_R7_ADDR					(7<<1)
	#define WM8510_R8_ADDR					(8<<1)
	#define WM8510_R10_ADDR					(10<<1)
	#define WM8510_R11_ADDR					(11<<1)
	#define WM8510_R14_ADDR					(14<<1)
	#define WM8510_R15_ADDR					(15<<1)
	#define WM8510_R24_ADDR					(24<<1)
	#define WM8510_R25_ADDR					(25<<1)
	#define WM8510_R27_ADDR					(27<<1)
	#define WM8510_R28_ADDR					(28<<1)
	#define WM8510_R29_ADDR					(29<<1)
	#define WM8510_R30_ADDR					(30<<1)
	#define WM8510_R32_ADDR					(32<<1)
	#define WM8510_R33_ADDR					(33<<1)
	#define WM8510_R34_ADDR					(34<<1)
	#define WM8510_R35_ADDR					(35<<1)
	#define WM8510_R36_ADDR					(36<<1)
	#define WM8510_R37_ADDR					(37<<1)
	#define WM8510_R38_ADDR					(38<<1)
	#define WM8510_R39_ADDR					(39<<1)
	#define WM8510_R40_ADDR					(40<<1)
	#define WM8510_R44_ADDR					(44<<1)
	#define WM8510_R45_ADDR					(45<<1)
	#define WM8510_R47_ADDR					(47<<1)
	#define WM8510_R49_ADDR					(49<<1)
	#define WM8510_R50_ADDR					(50<<1)
	#define WM8510_R54_ADDR					(54<<1)
	#define	WM8510_R56_ADDR					(56<<1)
	
	//===写命令
	#define WM8510_WADDR					0x34																																	// 1A-34
	//===读命令
	#define WM8510_RADDR					0x35																																	// 1B-35	
	//WM8510寄存器功能位
	#define	R0_Reset						0x00 																																	//复位
	
	//===R1
	#define	WM8510_R1H_BUFDCOPEN			0x01
	#define	WM8510_R1L_MIC2EN				0x40
	#define	WM8510_R1L_PLLEN				0x20
	#define	WM8510_R1L_MICBEN				0x10
	#define WM8510_R1L_BIASEN				0x08
	#define	WM8510_R1L_BUFIOEN				0x04
	#define	WM8510_R1L_VMIDSEL_5K			0x03
	#define	WM8510_R1L_VMIDSEL_500K			0x02
	#define	WM8510_R1L_VMIDSEL_50K			0x01
	#define	WM8510_R1L_VMIDSEL_OFF			0x00
	
	//===R2
	#define	WM8510_R2L_BOOSTEN				0x10
	#define	WM8510_R2L_INPPGAEN				0x04
	#define	WM8510_R2L_ADCEN				0x01
	
	//===R3
	#define	WM8510_R3L_MONOEN				0x80
	#define	WM8510_R3L_SPKNEN				0x40
	#define	WM8510_R3L_SPKPEN				0x20
	#define	WM8510_R3L_MONOMIXEN			0x10
	#define WM8510_R3L_SPKMIXEN				0x04
	#define	WM8510_R3L_DACEN				0x01
	
	//===R4
	#define	WM8510_R4H_BCP					0x01
	#define	WM8510_R4L_FRAMEP				0x80
	#define	WM8510_R4L_WL_16bit				0x00
	#define	WM8510_R4L_WL_20bit				0x20
	#define	WM8510_R4L_WL_24bit				0x40
	#define	WM8510_R4L_WL_32bit				0x60
	#define	WM8510_R4L_FMT_Right			0x00
	#define	WM8510_R4L_FMT_Left				0x08
	#define WM8510_R4L_FMT_I2S				0x10
	#define	WM8510_R4L_FMT_PCM				0x18
	#define	WM8510_R4L_DACLRSWAP			0x04
	#define	WM8510_R4L_ADCRSWAP				0x02
	
	//===R5
	#define	WM8510_R5L_DAC_COMP_OFF			0x00
	#define	WM8510_R5L_DAC_COMP_uLAW 		0x10
	#define	WM8510_R5L_DAC_COMP_ALAW		0x18
	#define	WM8510_R5L_ADC_COMP_OFF			0x00
	#define	WM8510_R5L_ADC_COMP_uLAW 		0x04
	#define	WM8510_R5L_ADC_COMP_ALAW		0x06
	#define	WM8510_R5L_LOOPBACK				0x01
	
	//===R6
	#define WM8510_R6H_CLKSEL				0x01
	#define WM8510_R6L_MCLKDIV_1			(0x00<<5)
	#define WM8510_R6L_MCLKDIV_15			(0x01<<5)		//1.5分频
	#define WM8510_R6L_MCLKDIV_2			(0x02<<5)
	#define WM8510_R6L_MCLKDIV_3			(0x03<<5)
	#define WM8510_R6L_MCLKDIV_4			(0x04<<5)
	#define WM8510_R6L_MCLKDIV_6			(0x05<<5)
	#define WM8510_R6L_MCLKDIV_8			(0x06<<5)
	#define WM8510_R6L_MCLKDIV_12			(0x07<<5)
	#define WM8510_R6L_BCLKDIV_1			(0x00<<2)
	#define WM8510_R6L_BCLKDIV_2			(0x01<<2)
	#define WM8510_R6L_BCLKDIV_4			(0x02<<2)
	#define WM8510_R6L_BCLKDIV_8			(0x03<<2)
	#define WM8510_R6L_BCLKDIV_16			(0x04<<2)
	#define WM8510_R6L_BCLKDIV_32			(0x05<<2)
	#define WM8510_R6L_MS					 0x01
	
	//===BCLK的分频
	#define WM8510_BCLKDIV_1				(0 << 2)
	#define WM8510_BCLKDIV_2				(1 << 2)
	#define WM8510_BCLKDIV_4				(2 << 2)
	#define WM8510_BCLKDIV_8				(3 << 2)
	#define WM8510_BCLKDIV_16				(4 << 2)
	#define WM8510_BCLKDIV_32				(5 << 2)
	
	//===MCLK的分频
	#define WM8510_MCLKDIV_1				(0 << 5)
	#define WM8510_MCLKDIV_1_5				(1 << 5)
	#define WM8510_MCLKDIV_2				(2 << 5)
	#define WM8510_MCLKDIV_3				(3 << 5)
	#define WM8510_MCLKDIV_4				(4 << 5)
	#define WM8510_MCLKDIV_6				(5 << 5)
	#define WM8510_MCLKDIV_8				(6 << 5)
	#define WM8510_MCLKDIV_12				(7 << 5)
	
	//===R7
	#define WM8510_R7L_SR_48kHz				(0x00<<1)
	#define WM8510_R7L_SR_32kHz				(0x01<<1)
	#define WM8510_R7L_SR_24kHz				(0x02<<1)
	#define WM8510_R7L_SR_16kHz				(0x03<<1)
	#define WM8510_R7L_SR_12kHz				(0x04<<1)
	#define WM8510_R7L_SR_8kHz				(0x05<<1)
	#define WM8510_R7L_SLOWCLKEN			 0x01
	
	//===R8
	#define WM8510_R8L_OPCLKDIV_1			(0x00<<4)
	#define WM8510_R8L_OPCLKDIV_2			(0x01<<4)
	#define WM8510_R8L_OPCLKDIV_3			(0x02<<4)
	#define WM8510_R8L_OPCLKDIV_4			(0x03<<4)
	#define WM8510_R8L_GPIOPOL				(0x01<<3)
	#define WM8510_R8L_GPIOSEL_CSB			0x00
	#define WM8510_R8L_GPIOSEL_Jack			0x01
	#define WM8510_R8L_GPIOSEL_Temp			0x02
	#define WM8510_R8L_GPIOSEL_Amute		0x03
	#define WM8510_R8L_GPIOSEL_PLLLCK		0x04
	#define WM8510_R8L_GPIOSEL_Lock			0x05
	
	//===PLL输出分频
	#define WM8510_OPCLKDIV_1				(0 << 4)
	#define WM8510_OPCLKDIV_2				(1 << 4)
	#define WM8510_OPCLKDIV_3				(2 << 4)
	#define WM8510_OPCLKDIV_4				(3 << 4)
	
	//===R10
	#define WM8510_R10L_DACMU
	#define WM8510_R10L_DEEMPH_32kHz		(0x01<<4)
	#define WM8510_R10L_DEEMPH_44kHz		(0x02<<4)
	#define WM8510_R10L_DEEMPH_48kHz		(0x03<<4)
	#define WM8510_R10L_DACOSR128			(0x01<<3)
	#define	WM8510_R10L_AMUTE				(0x01<<2)
	#define	WM8510_R10L_DACPOL				0x01
	
	//===R40
	#define	WM8510_R40L_MONOATTN			0x04
	#define	WM8510_R40L_SPKATTN				0x02
	
	//===R44
	#define WM8510_R44H_MBVSEL				0x01
	#define	WM8510_R44L_MIC2MODE			0x08
	#define WM8510_R44L_MIC2_2INPPGA		0x04
	#define WM8510_R44L_MICN2INPPGA			0x02
	#define WM8510_R44L_MICP2INPPGA			0x01
	
	//===R49
	#define	WM8510_R49L_MONOBOOST			0x08
	#define WM8510_R49L_SPKBOOST			0x04
	#define WM8510_R49L_TSDEN				0x02
	#define	WM8510_R49L_VROI				0x01
	
	//===R50
	#define WM8510_R50L_MIC2_2SPK			0x20
	#define	WM8510_R50L_BYP2SPK				0x02
	#define WM8510_R50L_DAC2SPK				0x01
	
	//===R56
	#define WM8510_R56L_MONOOUT				0x40
	#define WM8510_R56L_MIC2_2MONO			0x04
	#define WM8510_R56L_BYP2MONO			0x02
	#define WM8510_R56L_DAC2MONO			0x01
	
	//===定义WM8510的输出频率变化多少次之后执行复位操作
	//#define USE_WM8510_RESET
	#define WM8510_FREQ_CHANGE_MAX			3000
	//===结构体定义
	typedef struct _WM8510_HandlerType			WM8510_HandlerType;
	//===指针结构体定义
	typedef struct _WM8510_HandlerType			*pWM8510_HandlerType;
	//===定义结构
	struct _WM8510_HandlerType
	{
		UINT8_T				msgMclkDIV;																																				//---mclk的分频数
		UINT8_T				msgBclkDIV;																																				//---bclk的分频数
		UINT8_T				msgPreDIV;																																				//---预分频数
		UINT8_T				msgPllN;																																				//---pll的N值
		UINT8_T				msgR0[2];																																				//---寄存器R0的配置值
		UINT8_T				msgR1[2];																																				//---寄存器R1的配置值
		UINT8_T				msgNowR6[2];																																			//---寄存器R6的配置值
		UINT8_T				msgLastR6[2];																																			//---寄存器R6的配置值
		UINT8_T				msgNowR36[2];																																			//---寄存器R36的配置值
		UINT8_T				msgLastR36[2];																																			//---寄存器R36的配置值
		UINT8_T				msgNowR37[2];																																			//---寄存器R37的配置值
		UINT8_T				msgLastR37[2];																																			//---寄存器R37的配置值
		UINT8_T				msgNowR38[2];																																			//---寄存器R38的配置值
		UINT8_T				msgLastR38[2];																																			//---寄存器R38的配置值
		UINT8_T				msgNowR39[2];																																			//---寄存器R39的配置值
		UINT8_T				msgLastR39[2];																																			//---寄存器R39的配置值
	#ifdef USE_WM8510_RESET
		UINT16_T			msgFreqChangeCount;																																		//---记录当前频率变化的次数
	#endif
		UINT32_T			mssgFreqAdd;																																			//---输出频率的补偿值，每100KHz补偿15Hz
		UINT32_T			msgPllK;																																				//---pll的K值
		UINT32_T			magPllFreq;																																				//---pll频率
		UINT32_T			msgRefOSC;																																				//---参考时钟
		UINT32_T			msgFreqHz;																																				//---输出的频率Hz
		UINT32_T			msgFreqKHz;																																				//---输出的频率KHz
		float				msgPllRate;																																				//---pll的比值
	#ifdef WM8510_USE_lEVEL_SHIFT
		GPIO_HandlerType	msgOE;																																					//---OE使用的端口，用于控制电平装换的开关
	#endif
		I2C_HandlerType msgI2C;																																						//---使用的I2C设备
	};

	//===定义的任务函数
	#define WM8510_TASK_ONE				pWm8510Device0
	#define WM8510_TASK_TWO				0
	#define WM8510_TASK_THREE			0

	//===外部调用接口
	extern WM8510_HandlerType			g_Wm8510Device0;
	extern pWM8510_HandlerType			pWm8510Device0;

	//===函数定义
	void    WM8510_I2C_ParamsInit(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_Device0_Init(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_Device1_Init(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_Device2_Init(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_Init(WM8510_HandlerType *WM8510x, void(*pFuncDelayus)(UINT32_T delay),UINT32_T(*pFuncTimerTick)(void), UINT8_T isHWI2C);
	UINT8_T WM8510_I2C_START(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_DeInit(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_SWI2C_WriteReg(WM8510_HandlerType *WM8510x, UINT8_T *pVal, UINT8_T length);
	UINT8_T WM8510_HWI2C_WriteReg(WM8510_HandlerType *WM8510x, UINT8_T *pVal, UINT8_T length);
	UINT8_T WM8510_I2C_SendCMD(WM8510_HandlerType *WM8510x, UINT8_T *pVal);
	UINT8_T WM8510_I2C_SetFreqHz(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510_I2C_SetFreqKHz(WM8510_HandlerType *WM8510x, float freq);
	UINT8_T WM8510_I2C_SetFreqMHz(WM8510_HandlerType *WM8510x, float freq);
	void    WM8510_I2C_Reset(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_SetFreqHzWithAllFreqReg(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510_CalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x);
	UINT8_T WM8510_I2C_SetFreqHzWithAllFreqRegAndCalibrateFreqKHzOutPut(WM8510_HandlerType *WM8510x, UINT32_T freq);
	UINT8_T WM8510_I2C_CalibrateClock(WM8510_HandlerType *WM8510x);
	//////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
//////////////////////////////////////////////////////////////////////////////////////
#endif /*WM8510_CFG_H_ */