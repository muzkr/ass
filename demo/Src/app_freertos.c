/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "FreeRTOS.h"
#include "task.h"

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer );
void vApplicationGetTaskMemory( StackType_t **ppxTaskStackBuffer, uint32_t *pulTaskStackSize );

#define TASK_STACK_SIZE 1024 // In words (not bytes)

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xTaskStack[TASK_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
}

void vApplicationGetTaskMemory( StackType_t **ppxTaskStackBuffer, uint32_t *pulTaskStackSize )
{
  *ppxTaskStackBuffer = xTaskStack;
  *pulTaskStackSize = TASK_STACK_SIZE;
}
