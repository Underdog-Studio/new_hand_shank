#include "exti_hal.h"



static void (*EXTI0_Callback)(void *str)=0;//外部中断0回调函数指针
static void *str0;
static void (*EXTI1_Callback)(void *str)=0;//外部中断1回调函数指针
static void *str1;
static void (*EXTI2_Callback)(void *str)=0;//外部中断2回调函数指针
static void *str2;
static void (*EXTI3_Callback)(void *str)=0;//外部中断3回调函数指针
static void *str3;
static void (*EXTI4_Callback)(void *str)=0;//外部中断4回调函数指针
static void *str4;
static void (*EXTI5_Callback)(void *str)=0;//外部中断5回调函数指针
static void *str5;
static void (*EXTI6_Callback)(void *str)=0;//外部中断6回调函数指针
static void *str6;
static void (*EXTI7_Callback)(void *str)=0;//外部中断7回调函数指针
static void *str7;
static void (*EXTI8_Callback)(void *str)=0;//外部中断8回调函数指针
static void *str8;
static void (*EXTI9_Callback)(void *str)=0;//外部中断9回调函数指针
static void *str9;
static void (*EXTI10_Callback)(void *str)=0;//外部中断10回调函数指针
static void *str10;
static void (*EXTI11_Callback)(void *str)=0;//外部中断11回调函数指针
static void *str11;
static void (*EXTI12_Callback)(void *str)=0;//外部中断12回调函数指针
static void *str12;
static void (*EXTI13_Callback)(void *str)=0;//外部中断13回调函数指针
static void *str13;
static void (*EXTI14_Callback)(void *str)=0;//外部中断14回调函数指针
static void *str14;
static void (*EXTI15_Callback)(void *str)=0;//外部中断15回调函数指针
static void *str15;




void exti_hal::init(exti_line_enum linex,
                    uint32_t gpio_periph,
                    uint8_t nvic_irq_priority,
                    uint8_t nvic_irq_sub_priority,
                    exti_mode_enum mode,
                    exti_trig_type_enum trig_type,
                    uint32_t pull_up_down
                    )
{
    this->linex = linex;

    if(linex<=BIT(15))
    {
        switch (pull_up_down)
        {
        case GPIO_PUPD_NONE:
            this->gpio__init(gpio_periph,(uint32_t)linex,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ);
            break;
        case GPIO_PUPD_PULLUP:
            this->gpio__init(gpio_periph,(uint32_t)linex,GPIO_MODE_IPU,GPIO_OSPEED_50MHZ);
            break;
        case GPIO_PUPD_PULLDOWN:
            this->gpio__init(gpio_periph,(uint32_t)linex,GPIO_MODE_IPD,GPIO_OSPEED_50MHZ);
            break;       
        default:
            break;
        }
        if(linex<BIT(5))
        {
            switch (linex)
            {
                case EXTI_0:
                    nvic_irq_enable(EXTI0_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
                    break;
                case EXTI_1:
                    nvic_irq_enable(EXTI1_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
                    break;
                case EXTI_2:
                    nvic_irq_enable(EXTI2_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
                    break;
                case EXTI_3:
                    nvic_irq_enable(EXTI3_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
                    break;
                case EXTI_4:
                    nvic_irq_enable(EXTI4_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
                    break;
                default:
                    break;
            }
        }else if(linex<BIT(10))
        {
            nvic_irq_enable(EXTI5_9_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
        }else
        {
            nvic_irq_enable(EXTI10_15_IRQn,nvic_irq_priority,nvic_irq_sub_priority);
        }
        gpio_exti_source_select(this->get_exti_source(gpio_periph),this->get_exti_gpiopin(linex));
    }
	exti_init(linex,mode,trig_type);
    exti_interrupt_flag_clear(linex);
    
}

void exti_hal::Callback_Register(void *str,void (*app)(void *str))
{
    switch (this->linex)
    {
    case EXTI_0:
        EXTI0_Callback = app;
        str0 = str;
        break;
    case EXTI_1:
        EXTI1_Callback = app;
        str1 = str;
        break;
    case EXTI_2:
        EXTI2_Callback = app;
        str2 = str;
        break;
    case EXTI_3:
        EXTI3_Callback = app;
        str3 = str;
        break;
    case EXTI_4:
        EXTI4_Callback = app;
        str4 = str;
        break;
    case EXTI_5:
        EXTI5_Callback = app;
        str5 = str;
        break;
    case EXTI_6:
        EXTI6_Callback = app;
        str6 = str;
        break;
    case EXTI_7:
        EXTI7_Callback = app;
        str7 = str;
        break;
    case EXTI_8:
        EXTI8_Callback = app;
        str8 = str;
        break;
    case EXTI_9:
        EXTI9_Callback = app;
        str9 = str;
        break;
    case EXTI_10:
        EXTI10_Callback = app;
        str10 = str;
        break;
    case EXTI_11:
        EXTI11_Callback = app;
        str11 = str;
        break;
    case EXTI_12:
        EXTI12_Callback = app;
        str12 = str;
        break;
    case EXTI_13:
        EXTI13_Callback = app;
        str13 = str;
        break;
    case EXTI_14:
        EXTI14_Callback = app;
        str14 = str;
        break;
    case EXTI_15:
        EXTI15_Callback = app;
        str15 = str;
        break;
    default:
        break;
    }
}


void exti_hal::set_exti_trig(exti_trig_type_enum trig_type)
{
    switch(trig_type){
    case EXTI_TRIG_RISING:
        EXTI_RTEN |= (uint32_t)this->linex;
        EXTI_FTEN &= ~(uint32_t)this->linex;
        break;
    case EXTI_TRIG_FALLING:
        EXTI_RTEN &= ~(uint32_t)this->linex;
        EXTI_FTEN |= (uint32_t)this->linex;
        break;
    case EXTI_TRIG_BOTH:
        EXTI_RTEN |= (uint32_t)this->linex;
        EXTI_FTEN |= (uint32_t)this->linex;
        break;
    default:
        break;
    }
}





uint8_t exti_hal::get_exti_source(uint32_t gpio_periph)
{
    switch(gpio_periph)
	{
		case GPIOA:
			return GPIO_PORT_SOURCE_GPIOA;
		case GPIOB:
			return GPIO_PORT_SOURCE_GPIOB;
		case GPIOC:
			return GPIO_PORT_SOURCE_GPIOC;
		case GPIOD:
		    return GPIO_PORT_SOURCE_GPIOD;
		case GPIOE:
			return GPIO_PORT_SOURCE_GPIOE;
		case GPIOF:
			return GPIO_PORT_SOURCE_GPIOF;
		case GPIOG:
		    return GPIO_PORT_SOURCE_GPIOG;
	}
	return 0;
}

uint8_t exti_hal::get_exti_gpiopin(exti_line_enum linex)
{
    switch(linex)
    {
        case EXTI_0:
            return GPIO_PIN_SOURCE_0;
        case EXTI_1:
            return GPIO_PIN_SOURCE_1;
        case EXTI_2:
            return GPIO_PIN_SOURCE_2;
        case EXTI_3:
            return GPIO_PIN_SOURCE_3;
        case EXTI_4:
            return GPIO_PIN_SOURCE_4;
        case EXTI_5:
            return GPIO_PIN_SOURCE_5;
        case EXTI_6:
            return GPIO_PIN_SOURCE_6;
        case EXTI_7:
            return GPIO_PIN_SOURCE_7;
        case EXTI_8:
            return GPIO_PIN_SOURCE_8;
        case EXTI_9:
            return GPIO_PIN_SOURCE_9;
        case EXTI_10:
            return GPIO_PIN_SOURCE_10;
        case EXTI_11:
            return GPIO_PIN_SOURCE_11;
        case EXTI_12:
            return GPIO_PIN_SOURCE_12;
        case EXTI_13:
            return GPIO_PIN_SOURCE_13;
        case EXTI_14:
            return GPIO_PIN_SOURCE_14;
        case EXTI_15:
            return GPIO_PIN_SOURCE_15;
        default:
            return 0;

    }
}

extern "C"
{
    /**
    *@Description :外部中断服务函数
    **/
    void EXTI0_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_0))&&EXTI0_Callback!=0)
        {
            EXTI0_Callback(str0);
            exti_interrupt_flag_clear(EXTI_0);
        }
    }
    void EXTI1_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_1))&&EXTI1_Callback!=0)
        {
            EXTI1_Callback(str1);
            exti_interrupt_flag_clear(EXTI_1);
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_2))&&EXTI2_Callback!=0)
        {
            EXTI2_Callback(str2);
            exti_interrupt_flag_clear(EXTI_2);
        }    
    }
    void EXTI3_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_3))&&EXTI3_Callback!=0)
        {
            EXTI3_Callback(str3);
            exti_interrupt_flag_clear(EXTI_3);
        }       
    }
    void EXTI4_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_4))&&EXTI4_Callback!=0)
        {
            EXTI4_Callback(str4);
            exti_interrupt_flag_clear(EXTI_4);
        }       
    }
    void EXTI5_9_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_5))&&EXTI5_Callback!=0)
        {
            EXTI5_Callback(str5);
            exti_interrupt_flag_clear(EXTI_5);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_6))&&EXTI6_Callback!=0)
        {
            EXTI6_Callback(str6);
            exti_interrupt_flag_clear(EXTI_6);
        } 
        if((RESET != exti_interrupt_flag_get(EXTI_7))&&EXTI7_Callback!=0)
        {
            EXTI7_Callback(str7);
            exti_interrupt_flag_clear(EXTI_7);
        }   
        if((RESET != exti_interrupt_flag_get(EXTI_8))&&EXTI8_Callback!=0)
        {
            EXTI8_Callback(str8);
            exti_interrupt_flag_clear(EXTI_8);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_9))&&EXTI9_Callback!=0)
        {
            EXTI9_Callback(str9);
            exti_interrupt_flag_clear(EXTI_9);
        }
    }
    void EXTI10_15_IRQHandler(void)
    {
        if((RESET != exti_interrupt_flag_get(EXTI_10))&&EXTI10_Callback!=0)
        {
            EXTI10_Callback(str10);
            exti_interrupt_flag_clear(EXTI_10);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_11))&&EXTI11_Callback!=0)
        {
            EXTI11_Callback(str11);
            exti_interrupt_flag_clear(EXTI_11);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_12))&&EXTI12_Callback!=0)
        {
            EXTI12_Callback(str12);
            exti_interrupt_flag_clear(EXTI_12);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_13))&&EXTI13_Callback!=0)
        {
            EXTI13_Callback(str13);
            exti_interrupt_flag_clear(EXTI_13);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_14))&&EXTI14_Callback!=0)
        {
            EXTI14_Callback(str14);
            exti_interrupt_flag_clear(EXTI_14);
        }
        if((RESET != exti_interrupt_flag_get(EXTI_15))&&EXTI15_Callback!=0)
        {
            EXTI15_Callback(str15);
            exti_interrupt_flag_clear(EXTI_15);
        }
    }

}

