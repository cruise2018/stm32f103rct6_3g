/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <los_sem.h>
#include <osport.h>

#define CN_RCV_RING_BUFLEN  128

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USART1 init function */
UART_HandleTypeDef huart1;
static tagRingBuf  gs_ringbuf_uart1_rcv;
static  u8_t       gs_ringmem_uart1_rcv[CN_RCV_RING_BUFLEN];
static u32_t       gs_uart1_rcv_sync;
void uart1_irq_handler(void)
{
    unsigned char value;
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
    {
        value = (uint8_t)(huart1.Instance->DR & 0x00FF);
        ring_write(&gs_ringbuf_uart1_rcv,&value,1);
    }
    else if (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        (void)LOS_SemPost(gs_uart1_rcv_sync);
    }
}
void Debug_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    LOS_HwiCreate(USART1_IRQn, 0, 0, uart1_irq_handler, 0);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
    
    //create the receive buffer and receive sync
    LOS_SemCreate(0,&gs_uart1_rcv_sync);
    ring_init(&gs_ringbuf_uart1_rcv,gs_ringmem_uart1_rcv,CN_RCV_RING_BUFLEN,0,0);   

}


/* USART2 init function */
UART_HandleTypeDef huart2;
static tagRingBuf  gs_ringbuf_uart2_rcv;
static  u8_t       gs_ringmem_uart2_rcv[CN_RCV_RING_BUFLEN];
static u32_t       gs_uart2_rcv_sync;
void uart2_irq_handler(void)
{
    unsigned char value;
    if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
    {
        value = (uint8_t)(huart2.Instance->DR & 0x00FF);
        ring_write(&gs_ringbuf_uart2_rcv,&value,1);
    }
    else if (__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        (void)LOS_SemPost(gs_uart2_rcv_sync);
    }
}
void Debug_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    LOS_HwiCreate(USART2_IRQn, 0, 0, uart2_irq_handler, 0);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    
    //create the receive buffer and receive sync
    LOS_SemCreate(0,&gs_uart2_rcv_sync);
    ring_init(&gs_ringbuf_uart2_rcv,gs_ringmem_uart2_rcv,CN_RCV_RING_BUFLEN,0,0);   

}

/* USART3 init function */
UART_HandleTypeDef huart3;
static tagRingBuf  gs_ringbuf_uart3_rcv;
static  u8_t       gs_ringmem_uart3_rcv[CN_RCV_RING_BUFLEN];
static u32_t       gs_uart3_rcv_sync;
void uart3_irq_handler(void)
{
    unsigned char value;
    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE) != RESET)
    {
        value = (uint8_t)(huart3.Instance->DR & 0x00FF);
        ring_write(&gs_ringbuf_uart3_rcv,&value,1);
    }
    else if (__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);
        (void)LOS_SemPost(gs_uart3_rcv_sync);
    }
}
void Debug_USART3_UART_Init(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    LOS_HwiCreate(USART3_IRQn, 0, 0, uart3_irq_handler, 0);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
    
    //create the receive buffer and receive sync
    LOS_SemCreate(0,&gs_uart3_rcv_sync);
    ring_init(&gs_ringbuf_uart3_rcv,gs_ringmem_uart3_rcv,CN_RCV_RING_BUFLEN,0,0);   

}



void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(huart->Instance==USART1)
    {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
    }
    else if(huart->Instance==USART2)
    {
        /* config USART2 clock */
        //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_AHB1Periph_GPIOA, ENABLE);
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART2_CLK_ENABLE();

        /* USART2 GPIO config */
        /* Configure USART2 Tx (PA.02) as alternate function push-pull */        
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        
    }
    else if(huart->Instance==USART3)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_USART3_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        
        __HAL_AFIO_REMAP_USART3_DISABLE();
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if(huart->Instance==USART1)
    {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
    }
    else if(huart->Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);    
    }
    else if(huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);    
    }  
}


/* define fputc */
#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
int fgetc(FILE *f){
    int ret;
    unsigned char  value;
    do{
        ret = ring_read(&gs_ringbuf_uart3_rcv,&value,1);
        if(ret > 0){
            break;
        }
        LOS_SemPend(gs_uart3_rcv_sync,LOS_WAIT_FOREVER);
    }while(ret == 0);
    ret = value;
    return ret;
}
#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */
__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
    return len;
}
#endif

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/