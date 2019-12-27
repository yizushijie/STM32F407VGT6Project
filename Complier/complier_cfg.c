#include "complier_cfg.h"

//===获取编译的日期和时间
#ifdef USE_COMPLIER_VERSION
	#if defined(USE_MCU_STM32)
		const UINT8_T  g_VersionDate[VERSION_DATE_SIZE] = __DATE__;
		const UINT8_T  g_VersionTime[VERSION_TIME_SIZE] = __TIME__;
	#elif defined(USE_MCU_AVR)
		PRO_FLASH_UINT8_T  g_VersionDate[VERSION_DATE_SIZE] = __DATE__;
		PRO_FLASH_UINT8_T  g_VersionTime[VERSION_TIME_SIZE] = __TIME__;
	#else 
		#error "不支持型号的MCU."
	#endif
#endif

