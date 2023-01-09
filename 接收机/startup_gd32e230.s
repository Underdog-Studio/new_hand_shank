/**
  ******************************************************************************
  * @file      startup_stm32f10x_cl.s
  * @author    MCD Application Team
  * @version   V3.5.1
  * @date      08-September-2021
  * @brief     STM32F10x Connectivity line Devices vector table for RIDE7 toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR 
  *                  address.
  *                - Configure the clock system    
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2011 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m23
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss

.equ  BootRAM, 0xF1E0F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs  r1, #0
  b     LoopCopyDataInit

CopyDataInit:
  ldr   r3, =_sidata
  ldr   r3, [r3, r1]
  str   r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr   r0, =_sdata
  ldr   r3, =_edata
  adds  r2, r0, r1
  cmp   r2, r3
  bcc   CopyDataInit
  ldr   r2, =_sbss
  b     LoopFillZerobss

/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str  r3, [r2]
  adds r2, r2, #4
    
LoopFillZerobss:
  ldr   r3, = _ebss
  cmp   r2, r3
  bcc   FillZerobss
/* Call the clock system intitialization function.*/
  bl  SystemInit  
/* Call the application's entry point.*/
  bl    main
  bx    lr
.size   Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt. This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None
 * @retval None
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler

/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
  .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
    
g_pfnVectors:
  .word _estack
  .word Reset_Handler

  .word NMI_Handler
  .word HardFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word 0
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler


  .word WWDG_IRQHandler
  .word LVD_IRQHandler  
  .word RTC_IRQHandler
  .word FMC_IRQHandler
  .word RCU_IRQHandler
  .word EXTI0_1_IRQHandler
  .word EXTI2_3_IRQHandler
  .word EXTI4_15_IRQHandler
  .word 0  
  .word DMA_Channel0_IRQHandler
  .word DMA_Channel1_2_IRQHandler
  .word DMA_Channel3_4_IRQHandler
  .word ADC_CMP_IRQHandler 
  .word TIMER0_BRK_UP_TRG_COM_IRQHandler
  .word TIMER0_Channel_IRQHandler
  .word 0
  .word TIMER2_IRQHandler
  .word TIMER5_IRQHandler   
  .word 0  
  .word TIMER13_IRQHandler
  .word TIMER14_IRQHandler
  .word TIMER15_IRQHandler
  .word TIMER16_IRQHandler
  .word I2C0_EV_IRQHandler
  .word I2C1_EV_IRQHandler 
  .word SPI0_IRQHandler
  .word SPI1_IRQHandler 
  .word USART0_IRQHandler
  .word USART1_IRQHandler
  .word 0
  .word 0
  .word 0
  .word I2C0_ER_IRQHandler
  .word 0
  .word I2C1_ER_IRQHandler
  .word BootRAM    /* @0x1E0. This is for boot in RAM mode for 
                         STM32F10x Connectivity line Devices. */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
*
*******************************************************************************/
  .weak NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak WWDG_IRQHandler
  .thumb_set WWDG_IRQHandler,Default_Handler
  
  .weak LVD_IRQHandler
  .thumb_set LVD_IRQHandler,Default_Handler
 
  .weak RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler
  
  .weak FMC_IRQHandler
  .thumb_set FMC_IRQHandler,Default_Handler
  
  .weak RCU_IRQHandler
  .thumb_set RCU_IRQHandler,Default_Handler
  
  .weak EXTI0_1_IRQHandler
  .thumb_set EXTI0_1_IRQHandler,Default_Handler
  
  .weak EXTI2_3_IRQHandler
  .thumb_set EXTI2_3_IRQHandler,Default_Handler
  
  .weak EXTI4_15_IRQHandler
  .thumb_set EXTI4_15_IRQHandler,Default_Handler
  
  .weak DMA_Channel0_IRQHandler
  .thumb_set DMA_Channel0_IRQHandler,Default_Handler
  
  .weak DMA_Channel1_2_IRQHandler
  .thumb_set DMA_Channel1_2_IRQHandler,Default_Handler
  
  .weak DMA_Channel3_4_IRQHandler
  .thumb_set DMA_Channel3_4_IRQHandler,Default_Handler
  
  .weak ADC_CMP_IRQHandler
  .thumb_set ADC_CMP_IRQHandler,Default_Handler
   
  .weak TIMER0_BRK_UP_TRG_COM_IRQHandler
  .thumb_set TIMER0_BRK_UP_TRG_COM_IRQHandler,Default_Handler
  
  .weak TIMER0_Channel_IRQHandler
  .thumb_set TIMER0_Channel_IRQHandler,Default_Handler
  
  .weak TIMER2_IRQHandler
  .thumb_set TIMER2_IRQHandler,Default_Handler
    
  .weak TIMER5_IRQHandler
  .thumb_set TIMER5_IRQHandler,Default_Handler
    
  .weak TIMER13_IRQHandler
  .thumb_set TIMER13_IRQHandler,Default_Handler
    
  .weak TIMER14_IRQHandler
  .thumb_set TIMER14_IRQHandler,Default_Handler
  
  .weak TIMER15_IRQHandler
  .thumb_set TIMER15_IRQHandler,Default_Handler
  
  .weak TIMER16_IRQHandler
  .thumb_set TIMER16_IRQHandler,Default_Handler
   
  .weak I2C0_EV_IRQHandler
  .thumb_set I2C0_EV_IRQHandler,Default_Handler
    
  .weak I2C1_EV_IRQHandler
  .thumb_set I2C1_EV_IRQHandler,Default_Handler
    
  .weak SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler,Default_Handler
    
  .weak SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler
      
  .weak USART0_IRQHandler
  .thumb_set USART0_IRQHandler,Default_Handler
      
  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler,Default_Handler
      
  .weak I2C0_ER_IRQHandler
  .thumb_set I2C0_ER_IRQHandler,Default_Handler
      
  .weak I2C1_ER_IRQHandler
  .thumb_set I2C1_ER_IRQHandler,Default_Handler
 
