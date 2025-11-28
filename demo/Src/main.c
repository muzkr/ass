/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

TaskHandle_t defaultTaskHandle;
StaticTask_t defaultTask;

TaskHandle_t serialTask1Handle;
StaticTask_t serialTask1;

TaskHandle_t serialTask2Handle;
StaticTask_t serialTask2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *arg);
void SerialTask(void *arg);
static void SerialSend(const uint8_t *buf, uint32_t size);

static inline void SerialSendStr(const char *str)
{
    SerialSend((const uint8_t *)str, strlen(str));
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
     */
    LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD1_STROBE | LL_SYSCFG_UCPD2_STROBE);

    /* PendSV_IRQn interrupt configuration */
    NVIC_SetPriority(PendSV_IRQn, 3);
    NVIC_SetPriority(SysTick_IRQn, 3);

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    defaultTaskHandle = xTaskCreateStatic( //
        StartDefaultTask,                  //
        "defaultTask",                     //
        NULL,                              //
        1,                                 //
        &defaultTask                       //
    );

    serialTask1Handle = xTaskCreateStatic( //
        SerialTask,                        //
        "serial1",                         //
        "hello world!\n",                  //
        1,                                 //
        &serialTask1                       //
    );

    serialTask2Handle = xTaskCreateStatic( //
        SerialTask,                        //
        "serial2",                         //
        "2718281828459045\n",              //
        1,                                 //
        &serialTask2                       //
    );

    /* Start scheduler */
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */

    while (1)
    {
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
    {
    }

    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1)
    {
    }

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 9, LL_RCC_PLLR_DIV_3);
    LL_RCC_PLL_Enable();
    LL_RCC_PLL_EnableDomain_SYS();
    while (LL_RCC_PLL_IsReady() != 1)
    {
    }

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    /* Sysclk activation on the main PLL */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }

    /* Set APB1 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    // LL_Init1msTick(48000000);
    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(48000000);
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    /**USART2 GPIO Configuration
    PA2   ------> USART2_TX
    PA3   ------> USART2_RX
    */
    GPIO_InitStruct.Pin = USART2_TX_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(USART2_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART2_RX_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(USART2_RX_GPIO_Port, &GPIO_InitStruct);

    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART2, &USART_InitStruct);
    LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_DisableFIFO(USART2);
    LL_USART_ConfigAsyncMode(USART2);

    LL_USART_Enable(USART2);

    /* Polling USART2 initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
    {
    }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    /**/
    LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

    /**/
    GPIO_InitStruct.Pin = LED_GREEN_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);
}

void SerialTask(void *arg)
{
    const char *msg = (char *)arg;

    for (;;)
    {
        SerialSendStr(msg);
        // portYIELD();
        vTaskDelay(pdMS_TO_TICKS(200 * strlen(msg)));
    }
}

static void SerialSend(const uint8_t *buf, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART2))
        {
        }
        LL_USART_TransmitData8(USART2, buf[i]);
    }
}

/**
 * @brief  Function implementing the defaultTask thread.
 * @param  arg: Not used
 * @retval None
 */
void StartDefaultTask(void *arg)
{
    /* Infinite loop */
    for (;;)
    {
        LL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
