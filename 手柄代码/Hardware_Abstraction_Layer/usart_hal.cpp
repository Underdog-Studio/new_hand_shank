#include "usart_hal.h"


void usart_hal::usart_init(uint32_t usart_periph,
                       uint32_t baudrate,			//波特率
					   usart_gpio_group gpio_group, //gpio组
                       uint32_t wlen,				//数据位 8 or 16
                       uint32_t stblen,             //停止位
                       uint32_t paritycfg,			//奇偶校验
                       uint32_t rtsconfig,			//rts控制
                       uint32_t ctsconfig,			//cts控制
                       uint32_t txconfig,			//发送控制
                       uint32_t rxconfig,			//接收控制
                       usart_interrupt_enum inttype,//使能中断 发送 or 接收 or 。。。
                       uint8_t nvic_irq_priority	//中断优先级
                       )
{
    this->usart_num = usart_periph;
	switch (this->usart_num)
	{
	case USART0:
		rcu_periph_clock_enable(RCU_USART0);
		switch (gpio_group)
		{
			case usart_gpio_group::group0:
				gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);
				gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
				break;
			case usart_gpio_group::group1:
				gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
				gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_7);
				break;
			default:
				break;
		}
		nvic_irq_enable(USART0_IRQn, nvic_irq_priority,0);//中断优先级
		break;
	case USART1:
		rcu_periph_clock_enable(RCU_USART1);
		switch (gpio_group)
		{
			case usart_gpio_group::group0:
				gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);
				gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);
				break;
			case usart_gpio_group::group1:
				gpio_init(GPIOD,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);
				gpio_init(GPIOD,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_6);
				break;
			default:
				break;
		}
		nvic_irq_enable(USART1_IRQn, nvic_irq_priority,0);//中断优先级
		break;
	case USART2:
		rcu_periph_clock_enable(RCU_USART2);
		switch (gpio_group)
		{
			case usart_gpio_group::group0:
				gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
				gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
				break;
			case usart_gpio_group::group1:
				gpio_init(GPIOC,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
				gpio_init(GPIOC,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
				break;
			case usart_gpio_group::group2:
				gpio_init(GPIOD,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_8);
				gpio_init(GPIOD,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_9);
				break;
			default:
				break;
		}
		nvic_irq_enable(USART2_IRQn, nvic_irq_priority,0);//中断优先级
		break;
	default:
		break;
	}
	//初始化串口
	usart_deinit(this->usart_num);
	usart_baudrate_set(this->usart_num, baudrate);//波特率115200
	usart_word_length_set(this->usart_num, USART_WL_8BIT);//默认8个数据位
	usart_stop_bit_set(this->usart_num, stblen);//默认一个停止位
	usart_parity_config(this->usart_num, paritycfg);//默认无奇偶校验
	usart_hardware_flow_rts_config(this->usart_num, rtsconfig);//RTS流控关闭
	usart_hardware_flow_cts_config(this->usart_num, ctsconfig);//CTS流控关闭
	usart_transmit_config(this->usart_num, txconfig);//使能串口发送
	usart_receive_config(this->usart_num, rxconfig);//使能串口接受

	usart_interrupt_flag_clear(this->usart_num,USART_INT_FLAG_RBNE);//清除接收中断
	usart_interrupt_enable(this->usart_num, inttype);//使能接收中断
	
	
	usart_enable(this->usart_num);

}


void usart_hal::write_packet(uint8_t *data,uint16_t num)
{
    uint16_t i;
    for(i=0;i<num;i++)
    {
        while(usart_flag_get(this->usart_num, USART_FLAG_TBE) == RESET){}//等待串口发送成功
        usart_data_transmit(this->usart_num,data[i]);//串口发送数据
    }
}

extern "C"
{

static void (*USART0_Callback)(uint8_t,void*);//串口0中断回调函数指针
void *usart0_str;
void USART0_IRQHandler(void)
{
	if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE)!=RESET && USART0_Callback!=0)
	{
		USART0_Callback((uint8_t)usart_data_receive(USART0),usart0_str);
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
	}else
	{
		if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE_ORERR);
		if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE_ORERR);

	}
  
}
static void (*USART1_Callback)(uint8_t,void*);//串口1中断回调函数指针
void *usart1_str;
void USART1_IRQHandler(void)
{
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE)!=RESET && USART1_Callback!=0)
	{
		USART1_Callback((uint8_t)usart_data_receive(USART1),usart1_str);
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
	}else
	{
		if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE_ORERR);
		if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE_ORERR);
	}
}

static void (*USART2_Callback)(uint8_t,void*);//串口2中断回调函数指针
void *usart2_str;
void USART2_IRQHandler(void)
{
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE)!=RESET && USART2_Callback!=0)
	{
		USART2_Callback((uint8_t)usart_data_receive(USART2),usart2_str);
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);
	}else
	{
		if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE_ORERR);
		if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE_ORERR)!=RESET)
			usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE_ORERR);
	}
}

}

void usart_hal::Callback_Register(void *str,void (*app)(uint8_t,void*))
{
	switch (this->usart_num)
	{
	case USART0:
		USART0_Callback = app;
		usart0_str = str;
		break;
	case USART1:
		USART1_Callback = app;
		usart1_str = str;
		break;
	case USART2:
		USART2_Callback = app;
		usart2_str = str;
		break;
	default:
		break;
	}
}












