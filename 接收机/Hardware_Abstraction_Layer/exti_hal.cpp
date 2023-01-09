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
                    exti_mode_enum mode,
                    exti_trig_type_enum trig_type,
                    uint32_t pull_up_down
                    )
{
    this->linex = linex;
    if(linex<=BIT(15) && gpio_periph !=0)
    {
        gpio_mode_set(gpio_periph,GPIO_MODE_INPUT,pull_up_down,linex);
        syscfg_exti_line_config(get_exti_source(gpio_periph),get_exti_gpiopin(linex));
    }
	exti_init(linex,mode,trig_type);

    if(linex<=BIT(1))
    {
        nvic_irq_enable(EXTI0_1_IRQn,nvic_irq_priority);
        
    }else if(linex<=BIT(3))
    {
        nvic_irq_enable(EXTI2_3_IRQn,nvic_irq_priority);
    }else
    {
        nvic_irq_enable(EXTI4_15_IRQn,nvic_irq_priority);
    }
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
    case EXTI_TRIG_RISING://上升沿
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN &= ~(uint32_t)linex;
        break;
    case EXTI_TRIG_FALLING://下降沿
        EXTI_RTEN &= ~(uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
        break;
    case EXTI_TRIG_BOTH:
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
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
			return EXTI_SOURCE_GPIOA;
		case GPIOB:
			return EXTI_SOURCE_GPIOB;
		case GPIOC:
			return EXTI_SOURCE_GPIOC;
		case GPIOF:
		    return EXTI_SOURCE_GPIOF;
	}
	return 0;
}

uint8_t exti_hal::get_exti_gpiopin(exti_line_enum linex)
{
    switch(linex)
    {
        case EXTI_0:
            return EXTI_SOURCE_PIN0;
        case EXTI_1:
            return EXTI_SOURCE_PIN1;
        case EXTI_2:
            return EXTI_SOURCE_PIN2;
        case EXTI_3:
            return EXTI_SOURCE_PIN3;
        case EXTI_4:
            return EXTI_SOURCE_PIN4;
        case EXTI_5:
            return EXTI_SOURCE_PIN5;
        case EXTI_6:
            return EXTI_SOURCE_PIN6;
        case EXTI_7:
            return EXTI_SOURCE_PIN7;
        case EXTI_8:
            return EXTI_SOURCE_PIN8;
        case EXTI_9:
            return EXTI_SOURCE_PIN9;
        case EXTI_10:
            return EXTI_SOURCE_PIN10;
        case EXTI_11:
            return EXTI_SOURCE_PIN11;
        case EXTI_12:
            return EXTI_SOURCE_PIN12;
        case EXTI_13:
            return EXTI_SOURCE_PIN13;
        case EXTI_14:
            return EXTI_SOURCE_PIN14;
        case EXTI_15:
            return EXTI_SOURCE_PIN15;
        default:
            return 0;

    }
}

extern "C"
{

/**
*@Description :外部中断服务函数
**/
#if EXTI_Lin0_hal|EXTI_Lin1_hal
	void EXTI0_1_IRQHandler(void)
	{
		#if EXTI_Lin0_hal
		if(RESET != exti_interrupt_flag_get(EXTI_0))
		{
            if(EXTI0_Callback!=0)
			    EXTI0_Callback(str0); 
            exti_interrupt_flag_clear(EXTI_0);
		}

		#endif
		#if EXTI_Lin1_hal
		if(RESET != exti_interrupt_flag_get(EXTI_1))
		{
            if(EXTI1_Callback!=0)
			    EXTI1_Callback(str1); 
            exti_interrupt_flag_clear(EXTI_1);
		}
		#endif
	}
#endif
#if EXTI_Lin2_hal|EXTI_Lin3_hal
	void EXTI2_3_IRQHandler(void)
	{
		#if EXTI_Lin2_hal
		if((RESET != exti_interrupt_flag_get(EXTI_2))&&EXTI2_Callback!=0)
		{
			EXTI2_Callback(str2);
            exti_interrupt_flag_clear(EXTI_2);
		}
	  #endif
		#if EXTI_Lin3_hal
		if((RESET != exti_interrupt_flag_get(EXTI_3))&&EXTI3_Callback!=0)
		{
			EXTI3_Callback(str3);
            exti_interrupt_flag_clear(EXTI_3);
		}
	  #endif
	}
#endif
#if (EXTI_Lin4_hal|EXTI_Lin5_hal|EXTI_Lin6_hal|EXTI_Lin7_hal|\
     EXTI_Lin8_hal|EXTI_Lin9_hal|EXTI_Lin10_hal|EXTI_Lin11_hal|\
     EXTI_Lin12_hal|EXTI_Lin13_hal|EXTI_Lin14_hal|EXTI_Lin15_hal)
	void EXTI4_15_IRQHandler(void)
	{
		#if EXTI_Lin4_hal
		if((RESET != exti_interrupt_flag_get(EXTI_4))&&EXTI4_Callback!=0)
		{
			EXTI4_Callback(str4);
            exti_interrupt_flag_clear(EXTI_4);
		}
		#endif
		#if EXTI_Lin5_hal
		if((RESET != exti_interrupt_flag_get(EXTI_5))&&EXTI5_Callback!=0)
		{
			EXTI5_Callback(str5);
            exti_interrupt_flag_clear(EXTI_5);
		}
		#endif
		#if EXTI_Lin6_hal
		if((RESET != exti_interrupt_flag_get(EXTI_6))&&EXTI6_Callback!=0)
		{
			EXTI6_Callback(str6);
            exti_interrupt_flag_clear(EXTI_6);
		}
		#endif
		#if EXTI_Lin7_hal
		if((RESET != exti_interrupt_flag_get(EXTI_7))&&EXTI7_Callback!=0)
		{
			EXTI7_Callback(str7);
            exti_interrupt_flag_clear(EXTI_7);
		}
		#endif
		#if EXTI_Lin8_hal
		if((RESET != exti_interrupt_flag_get(EXTI_8))&&EXTI8_Callback!=0)
		{
			EXTI8_Callback(str8);
			exti_interrupt_flag_clear(EXTI_8);
		}
		
		#endif
		#if EXTI_Lin9_hal
		if((RESET != exti_interrupt_flag_get(EXTI_9))&&EXTI9_Callback!=0)
		{
			EXTI9_Callback(str9);
            exti_interrupt_flag_clear(EXTI_9);
		}
		#endif
		#if EXTI_Lin10_hal
		if((RESET != exti_interrupt_flag_get(EXTI_10))&&EXTI10_Callback!=0)
		{
			EXTI10_Callback(str10);
            exti_interrupt_flag_clear(EXTI_10);
		}
		#endif
		#if EXTI_Lin11_hal
		if((RESET != exti_interrupt_flag_get(EXTI_11))&&EXTI11_Callback!=0)
		{
			EXTI11_Callback(str11);
            exti_interrupt_flag_clear(EXTI_11);
		}
		#endif
		#if EXTI_Lin12_hal
		if((RESET != exti_interrupt_flag_get(EXTI_12))&&EXTI12_Callback!=0)
		{
			EXTI12_Callback(str12);
            exti_interrupt_flag_clear(EXTI_12);
		}
		#endif
		#if EXTI_Lin13_hal
		if((RESET != exti_interrupt_flag_get(EXTI_13))&&EXTI13_Callback!=0)
		{
			EXTI13_Callback(str13);
            exti_interrupt_flag_clear(EXTI_13);
		}
		#endif
		#if EXTI_Lin14_hal
		if((RESET != exti_interrupt_flag_get(EXTI_14))&&EXTI14_Callback!=0)
		{
			EXTI14_Callback(str14);
            exti_interrupt_flag_clear(EXTI_14);
		}
		#endif
		#if EXTI_Lin15_hal
		if((RESET != exti_interrupt_flag_get(EXTI_15))&&EXTI15_Callback!=0)
		{
			EXTI15_Callback(str15);
            exti_interrupt_flag_clear(EXTI_15);
		}
		#endif
	}


#endif
}


