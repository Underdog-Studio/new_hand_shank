/**
  ******************************************************************************
  * @file      startup_stm32f303xc.s
  * @author    MCD Application Team
  * @version   V1.2.0
  * @date      19-June-2015
  * @brief     STM32F303xB/STM32F303xC devices vector table for Atollic 
  *            TrueSTUDIO toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address,
  *                - Configure the clock system  
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M4 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

  .syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script >>在链接文件中定义*/
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

.equ  BootRAM,        0xF1E0F85F
/**
 * @brief 这是处理器在发生重置事件后首次开始执行时调用的代码。仅执行绝对必要的集合，之后调用应用程序提供的 main（） 例程。
 * @param  None
 * @retval : None
*/
    .section	.text.Reset_Handler          /*设置新的代码段*/
	.weak	Reset_Handler                    /*隐形声明*/
	.type	Reset_Handler, %function         /*将Reset_Handler指定为函数 */
Reset_Handler:
  ldr   sp, =_estack    /* Atollic update: set stack pointer */

/* Copy the data segment initializers from flash to SRAM 把.data段从FLASH搬到RAM中 */
  movs	r1, #0
  b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss
/* Zero fill the bss segment.零填充 bss 段。 */
FillZerobss:
	movs	r3, #0
	str	r3, [r2], #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

/* Call the clock system intitialization function调用时钟系统初始化函数.*/
    bl  SystemInit
/* Call static constructors调用静态构造函数 */
    bl __libc_init_array
/* Call the application's entry point调用应用程序的入口点.*/
	bl	main

LoopForever:
    b LoopForever
    
.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  这是处理器收到意外中断时调用的代码。 这只会进入无限循环，保留系统状态以供调试器检查。
 *
 * @param  None
 * @retval : None
*/
    .section	.text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler
/******************************************************************************
*
*Cortex-M4 的中断向量表。 请注意，必须在此上面放置正确的构造，以确保它最终位于物理地址 0x0000.0000。
*
******************************************************************************/
 	.section	.isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	NMI_Handler
	.word	HardFault_Handler
	.word	MemManage_Handler
	.word	BusFault_Handler
	.word	UsageFault_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	SVC_Handler
	.word	DebugMon_Handler
	.word	0
	.word	PendSV_Handler
	.word	SysTick_Handler
	.word	WWDG_IRQHandler
	.word	LVD_IRQHandler
	.word	TAMPER_IRQHandler
	.word	RTC_IRQHandler
	.word	FMC_IRQHandler
	.word	RCU_CTC_IRQHandler
	.word	EXTI0_IRQHandler
	.word	EXTI1_IRQHandler
	.word	EXTI2_IRQHandler
	.word	EXTI3_IRQHandler
	.word	EXTI4_IRQHandler
	.word	DMA0_Channel0_IRQHandler
	.word	DMA0_Channel1_IRQHandler
	.word	DMA0_Channel2_IRQHandler
	.word	DMA0_Channel3_IRQHandler
	.word	DMA0_Channel4_IRQHandler
	.word	DMA0_Channel5_IRQHandler
	.word	DMA0_Channel6_IRQHandler
	.word	ADC0_1_IRQHandler
	.word	USBD_HP_CAN0_TX_IRQHandler
	.word	USBD_LP_CAN0_RX0_IRQHandler
	.word	CAN0_RX1_IRQHandler
	.word	CAN0_EWMC_IRQHandler
	.word	EXTI5_9_IRQHandler
	.word	TIMER0_BRK_IRQHandler
	.word	TIMER0_UP_IRQHandler
	.word	TIMER0_TRG_CMT_IRQHandler
	.word	TIMER0_Channel_IRQHandler
	.word	TIMER1_IRQHandler
	.word	TIMER2_IRQHandler
	.word	TIMER3_IRQHandler
	.word	I2C0_EV_IRQHandler
	.word	I2C0_ER_IRQHandler
	.word	I2C1_EV_IRQHandler
	.word	I2C1_ER_IRQHandler
	.word	SPI0_IRQHandler
	.word	SPI1_IRQHandler
	.word	USART0_IRQHandler
	.word	USART1_IRQHandler
	.word	USART2_IRQHandler
	.word	EXTI10_15_IRQHandler
	.word	RTC_Alarm_IRQHandler
	.word	USBD_WKUP_IRQHandler
	.word	TIMER7_BRK_IRQHandler
	.word	TIMER7_UP_IRQHandler
	.word	TIMER7_TRG_CMT_IRQHandler
	.word	TIMER7_Channel_IRQHandler
	.word	ADC2_IRQHandler
	.word	EXMC_IRQHandler
	.word	SDIO_IRQHandler
	.word	TIMER4_IRQHandler
	.word	SPI2_IRQHandler
	.word	UART3_IRQHandler
	.word	UART4_IRQHandler
	.word	TIMER5_IRQHandler
	.word	TIMER6_IRQHandler
	.word	DMA1_Channel0_IRQHandler
	.word	DMA1_Channel1_IRQHandler
	.word	DMA1_Channel2_IRQHandler
	.word	DMA1_Channel3_4_IRQHandler

/*******************************************************************************
* 为每个异常处理程序提供弱别名到Default_Handler，因为它们是弱别名，因此任何具有相同名称的函数都将覆盖此定义。
*******************************************************************************/

  .weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

  .weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

  .weak	MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

  .weak	BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak	UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	WWDG_IRQHandler
	.thumb_set WWDG_IRQHandler,Default_Handler

	.weak	LVD_IRQHandler
	.thumb_set LVD_IRQHandler,Default_Handler

	.weak	TAMPER_IRQHandler
	.thumb_set TAMPER_IRQHandler,Default_Handler

	.weak	RTC_IRQHandler
	.thumb_set RTC_IRQHandler,Default_Handler

	.weak	FMC_IRQHandler
	.thumb_set FMC_IRQHandler,Default_Handler

	.weak	RCU_CTC_IRQHandler
	.thumb_set RCU_CTC_IRQHandler,Default_Handler

	.weak	EXTI0_IRQHandler
	.thumb_set EXTI0_IRQHandler,Default_Handler

	.weak	EXTI1_IRQHandler
	.thumb_set EXTI1_IRQHandler,Default_Handler

	.weak	EXTI2_IRQHandler
	.thumb_set EXTI2_IRQHandler,Default_Handler

	.weak	EXTI3_IRQHandler
	.thumb_set EXTI3_IRQHandler,Default_Handler

	.weak	EXTI4_IRQHandler
	.thumb_set EXTI4_IRQHandler,Default_Handler

	.weak	DMA0_Channel0_IRQHandler
	.thumb_set DMA1_Channel1_IRQHandler,Default_Handler

	.weak	DMA0_Channel1_IRQHandler
	.thumb_set DMA0_Channel1_IRQHandler,Default_Handler

	.weak	DMA0_Channel2_IRQHandler
	.thumb_set DMA0_Channel2_IRQHandler,Default_Handler

	.weak	DMA0_Channel3_IRQHandler
	.thumb_set DMA0_Channel3_IRQHandler,Default_Handler

	.weak	DMA0_Channel4_IRQHandler
	.thumb_set DMA0_Channel4_IRQHandler,Default_Handler

	.weak	DMA0_Channel5_IRQHandler
	.thumb_set DMA0_Channel5_IRQHandler,Default_Handler

	.weak	DMA0_Channel6_IRQHandler
	.thumb_set DMA0_Channel6_IRQHandler,Default_Handler

	.weak	ADC0_1_IRQHandler
	.thumb_set ADC0_1_IRQHandler,Default_Handler

	.weak	USBD_HP_CAN0_TX_IRQHandler
	.thumb_set USBD_HP_CAN0_TX_IRQHandler,Default_Handler

	.weak	USBD_LP_CAN0_RX0_IRQHandler
	.thumb_set USBD_LP_CAN0_RX0_IRQHandler,Default_Handler

	.weak	CAN0_RX1_IRQHandler
	.thumb_set CAN0_RX1_IRQHandler,Default_Handler

	.weak	CAN0_EWMC_IRQHandler
	.thumb_set CAN0_EWMC_IRQHandler,Default_Handler

	.weak	EXTI5_9_IRQHandler
	.thumb_set EXTI5_9_IRQHandler,Default_Handler

	.weak	TIMER0_BRK_IRQHandler
	.thumb_set TIMER0_BRK_IRQHandler,Default_Handler

	.weak	TIMER0_UP_IRQHandler
	.thumb_set TIMER0_UP_IRQHandler,Default_Handler

	.weak	TIMER0_TRG_CMT_IRQHandler
	.thumb_set TIMER0_TRG_CMT_IRQHandler,Default_Handler

	.weak	TIMER0_Channel_IRQHandler
	.thumb_set TIMER0_Channel_IRQHandler,Default_Handler

	.weak	TIMER1_IRQHandler
	.thumb_set TIMER1_IRQHandler,Default_Handler

	.weak	TIMER2_IRQHandler
	.thumb_set TIMER2_IRQHandler,Default_Handler

	.weak	TIMER3_IRQHandler
	.thumb_set TIMER3_IRQHandler,Default_Handler

	.weak	I2C0_EV_IRQHandler
	.thumb_set I2C0_EV_IRQHandler,Default_Handler

	.weak	I2C0_ER_IRQHandler
	.thumb_set I2C0_ER_IRQHandler,Default_Handler

	.weak	I2C1_EV_IRQHandler
	.thumb_set I2C1_EV_IRQHandler,Default_Handler

	.weak	I2C1_ER_IRQHandler
	.thumb_set I2C1_ER_IRQHandler,Default_Handler

	.weak	SPI0_IRQHandler
	.thumb_set SPI0_IRQHandler,Default_Handler

	.weak	SPI1_IRQHandler
	.thumb_set SPI1_IRQHandler,Default_Handler

	.weak	USART0_IRQHandler
	.thumb_set USART0_IRQHandler,Default_Handler

	.weak	USART1_IRQHandler
	.thumb_set USART1_IRQHandler,Default_Handler

	.weak	USART2_IRQHandler
	.thumb_set USART2_IRQHandler,Default_Handler

	.weak	EXTI10_15_IRQHandler
	.thumb_set EXTI10_15_IRQHandler,Default_Handler

	.weak	RTC_Alarm_IRQHandler
	.thumb_set RTC_Alarm_IRQHandler,Default_Handler

	.weak	USBD_WKUP_IRQHandler
	.thumb_set USBD_WKUP_IRQHandler,Default_Handler

	.weak	TIMER7_BRK_IRQHandler
	.thumb_set TIMER7_BRK_IRQHandler,Default_Handler

	.weak	TIMER7_UP_IRQHandler
	.thumb_set TIMER7_UP_IRQHandler,Default_Handler

	.weak	TIMER7_TRG_CMT_IRQHandler
	.thumb_set TIMER7_TRG_CMT_IRQHandler,Default_Handler

	.weak	TIMER7_Channel_IRQHandler
	.thumb_set TIMER7_Channel_IRQHandler,Default_Handler

	.weak	ADC2_IRQHandler
	.thumb_set ADC2_IRQHandler,Default_Handler

	.weak	EXMC_IRQHandler
	.thumb_set EXMC_IRQHandler,Default_Handler

	.weak	SDIO_IRQHandler
	.thumb_set SDIO_IRQHandler,Default_Handler

	.weak	TIMER4_IRQHandler
	.thumb_set TIMER4_IRQHandler,Default_Handler

	.weak	SPI2_IRQHandler
	.thumb_set SPI2_IRQHandler,Default_Handler

	.weak	UART3_IRQHandler
	.thumb_set UART3_IRQHandler,Default_Handler

	.weak	UART4_IRQHandler
	.thumb_set UART4_IRQHandler,Default_Handler

	.weak	TIMER5_IRQHandler
	.thumb_set TIMER5_IRQHandler,Default_Handler

	.weak	TIMER6_IRQHandler
	.thumb_set TIMER6_IRQHandler,Default_Handler

	.weak	DMA1_Channel0_IRQHandler
	.thumb_set DMA1_Channel0_IRQHandler,Default_Handler

	.weak	DMA1_Channel1_IRQHandler
	.thumb_set DMA1_Channel1_IRQHandler,Default_Handler

	.weak	DMA1_Channel2_IRQHandler
	.thumb_set DMA1_Channel2_IRQHandler,Default_Handler	
	
	.weak	DMA1_Channel3_4_IRQHandler
	.thumb_set DMA1_Channel3_4_IRQHandler,Default_Handler
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/