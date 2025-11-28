# ASS: A Simple Scheduler

This is a modified version of FreeRTOS tailored for Cortex-M0/-M0+ systems with limited RAM.

Compared to the original FreeRTOS, the most significant differences are:

- All tasks share a single stack pool (rather than each task having pre-determined, dedicated stack)
- Does not support dynamic kernel object allocation (`configSUPPORT_DYNAMIC_ALLOCATION`)
- Does not support task deletion (`INCLUDE_vTaskDelete`)

See [FreeRTOS/Source/include/ass_config.h](FreeRTOS/Source/include/ass_config.h) for details.
