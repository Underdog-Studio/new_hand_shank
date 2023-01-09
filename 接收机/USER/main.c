#include "gd32e23x.h"
#include "System_time.h"
#include "gd32e23x_gpio.h"
#include "gd32e23x_rcu.h"
#include "main.h"

int main(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOF);
	__enable_irq();
	System_time_init();
	Main();
}




