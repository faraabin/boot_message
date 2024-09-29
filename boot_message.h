/**
******************************************************************************
* @file           : boot_message.h
* @brief          :
* @note           :
* @copyright      : COPYRIGHT© 2024 FaraabinCo
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright© 2024 FaraabinCo.
* All rights reserved.</center></h2>
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
* @verbatim
* @endverbatim
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __boot_message_H
#define __boot_message_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "faraabin.h"

#include <stdint.h>

/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define fBootStart_() uint8_t __boot_result__ = 0

#define fBootRetryResult_(func_, retry_, delayMs_, result_) \
	do{ \
    uint8_t __res__ = 0;\
    uint8_t __cnt__ = 0; \
    while(__cnt__ < (retry_)) { \
      tic_();\
      __res__ = (func_);\
			timeUs_t __elapsed__ = tocUs_();\
			fBootMessage_Private_BootAssignResult(&__res__, result_);\
			if(__res__ != 0) {\
				__boot_result__ = 1;\
			}\
      fBootMessage_AddMessage(#func_, __res__, __elapsed__);\
      __cnt__++;\
      fChrono_DelayMs(delayMs_);\
    } \
  }while(0)

#define fBoot_(func_) \
	fBootRetryResult_(func_, 1, 0, NULL)

#define fBootRetry_(func_, retry_, delayMs_) \
	fBootRetryResult_(func_, retry_, delayMs_, NULL)

#define fBootWithResult_(func_, result_) \
	fBootRetryResult_(func_, 1, 0, result_) 

#define BOOT_RESULT_ __boot_result__

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
uint8_t fBootMessage_Init(void);

uint8_t fBootMessage_AddMessage(const char *message, uint8_t result, uint32_t exeTimeUs);

void fBootMessage_Private_BootAssignResult(uint8_t *src, uint8_t *dst);

FARAABIN_FUNCTION_GROUP_PROTOTYPE_(bootMessageFuncGroup);

/* Exported variables --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_MESSAGE_H */

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
