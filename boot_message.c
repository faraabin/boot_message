/**
******************************************************************************
* @file           : boot_message.c
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

/* Includes ------------------------------------------------------------------*/
#include "boot_message.h"
#include "boot_message_config.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
  
  const char *BootName;
  
  uint8_t BootResult;
  
  uint32_t ExeTimeUs;
  
  uint32_t TimeStamp;

}sBootMessage;

FARAABIN_FUNCTION_GROUP_TYPE_DEF_(sFaraabinFobjectEventGroup);

/* Private variables ---------------------------------------------------------*/
static sBootMessage BootMessages[BOOT_MESSAGE_MAX_QTY];
static uint32_t LastMsgIndex;
static bool SingleInitFlag = false;

FARAABIN_EVENT_GROUP_DEF_STATIC_(BootMessageEventGroup);
FARAABIN_DICT_GROUP_DEF_STATIC_(BootMessageDictGroup);

/* Private function prototypes -----------------------------------------------*/
/* Variables -----------------------------------------------------------------*/

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                          ##### Exported Functions #####                          ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t fBootMessage_Init(void) {
  
  if(SingleInitFlag) {
    return 1;
  }

  BootMessageEventGroup.Enable = true;
  FARAABIN_EventGroup_Init_WithPath_(&BootMessageEventGroup, "System");
  FARAABIN_DictGroup_Init_(&BootMessageDictGroup);
  FARAABIN_FunctionGroupType_Init_(&BootMessageFn);

  LastMsgIndex = 0;
  SingleInitFlag = true;

  return 0;
}

/**
 * @brief 
 * 
 * @param message 
 * @param result 
 * @param exeTimeUs 
 * @return uint8_t 
 */
uint8_t fBootMessage_AddMessage(const char *message, uint8_t result, uint32_t exeTimeUs) {

  if(LastMsgIndex < BOOT_MESSAGE_MAX_QTY) {

    BootMessages[LastMsgIndex].TimeStamp = fChrono_GetTick();
    BootMessages[LastMsgIndex].ExeTimeUs = exeTimeUs;
    BootMessages[LastMsgIndex].BootName = message;
    BootMessages[LastMsgIndex].BootResult = result;

    LastMsgIndex++;

    return 0;
  }

  return 1;
}

/**
 * @brief 
 * 
 * @param src 
 * @param dst 
 */
void fBootMessage_Private_BootAssignResult(uint8_t *src, uint8_t *dst) {
  
  if(dst == NULL) {
    return;
  }
  
  if(src == NULL) {
    return;
  }
  
  *dst = *src;
}

/*
╔══════════════════════════════════════════════════════════════════════════════════╗
║                            ##### Private Functions #####                         ║
╚══════════════════════════════════════════════════════════════════════════════════╝
*/

/**
 * @brief 
 * 
 */
FARAABIN_DICT_GROUP_FUNC_(BootMessageDictGroup) {

  FARAABIN_FUNCTION_GROUP_OBJECT_DICT_WP_(BootMessageFn, BootMessageEventGroup, "System");
  
  FARAABIN_DICT_GROUP_FUNC_END_;
}

/**
 * @brief 
 * 
 */
FARAABIN_FUNCTION_(BootMessageFn, SEND_BOOT_MESSAGES, "help") {

  for(int i = 0; i < LastMsgIndex; i++) {

    if(BootMessages[i].BootResult != 0) {

      FARAABIN_PrintfErrorTo_(me, "[Timestamp (us): %010d] %s FAILED [Execution (us): %d].", BootMessages[i].TimeStamp, BootMessages[i].BootName, BootMessages[i].ExeTimeUs);

    } else {

      FARAABIN_PrintfTo_(me, "[Timestamp (us): %010d] %s SUCCESSFUL [Execution (us): %d].", BootMessages[i].TimeStamp, BootMessages[i].BootName, BootMessages[i].ExeTimeUs);

    }

  }
    
  FARAABIN_FUNCTION_END_();
}

/**
 * @brief 
 * 
 */
FARAABIN_FUNCTION_GROUP_(BootMessageFn, "help") {
  
  FARAABIN_FUNCTION_GROUP_ADD_(BootMessageFn, SEND_BOOT_MESSAGES);

}

/************************ © COPYRIGHT FaraabinCo *****END OF FILE****/
