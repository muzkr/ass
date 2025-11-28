
#ifndef INC_FREERTOS_H
#error "include FreeRTOS.h" must appear in source files before "include semphr.h"
#endif

#include <assert.h>
#include "portmacro.h"

#if !defined(configSUPPORT_STATIC_ALLOCATION)
#define configSUPPORT_STATIC_ALLOCATION 1
#else
static_assert(1 == configSUPPORT_STATIC_ALLOCATION, "configSUPPORT_STATIC_ALLOCATION must be defined as 1");
#endif

#if !defined(configSUPPORT_DYNAMIC_ALLOCATION)
#define configSUPPORT_DYNAMIC_ALLOCATION 0
#else
static_assert(0 == configSUPPORT_DYNAMIC_ALLOCATION, "configSUPPORT_DYNAMIC_ALLOCATION must be defined as 0");
#endif

#if INCLUDE_vTaskDelete
#error INCLUDE_vTaskDelete cannot be defined as true
#endif

#if !defined(configMINIMAL_STACK_SIZE)
#define configMINIMAL_STACK_SIZE 32
#else
static_assert(configMINIMAL_STACK_SIZE > 0, "configMINIMAL_STACK_SIZE must be defined as positive");
static_assert(0 == (configMINIMAL_STACK_SIZE * sizeof(portSTACK_TYPE)) % portBYTE_ALIGNMENT, "configMINIMAL_STACK_SIZE must be defined so that when converted to bytes it is a multiple of portBYTE_ALIGNMENT");
#endif

// TODO: This may change in the future
#if configUSE_TIMERS
#error configUSE_TIMERS cannot be defined as true
#endif

// TODO: This may change in the future
#if configUSE_TRACE_FACILITY
#error configUSE_TRACE_FACILITY cannot be defined as true
#endif

// TODO: This may change in the future
#if INCLUDE_uxTaskGetStackHighWaterMark
#error INCLUDE_uxTaskGetStackHighWaterMark cannot be defined as true
#endif

// TODO: This may change in the future
#if INCLUDE_uxTaskGetStackHighWaterMark2
#error INCLUDE_uxTaskGetStackHighWaterMark2 cannot be defined as true
#endif
