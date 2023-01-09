#include "System_time.h"
#include "main.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ff.h"

#define Main_Task_STK_SIZE	128
TaskHandle_t Main_Task_Handler;
#define Main_Task_PRIO	1

#include "usbd_msc_core.h"
#include "usbd_hw.h"

usb_dev usb_msc;

int main(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOF);
	rcu_periph_clock_enable(RCU_GPIOG);
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
	System_time_init();


	xTaskCreate((TaskFunction_t		)Main,
							(const char*			)"Main_Task",
							(uint16_t					)Main_Task_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)Main_Task_PRIO,
							(TaskHandle_t*		)&Main_Task_Handler);//
	
	vTaskStartScheduler();
	
	while (1);
	
}

