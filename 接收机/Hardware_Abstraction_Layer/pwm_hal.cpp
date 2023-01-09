#include "pwm_hal.h"
/**定时器通道分组对应GPIO
timer0
    ch0 -- PA8
    ch1 -- PA9
    ch2 -- PA10
    ch3 -- PA11
timer2
    group0
    ch0 -- PA6
    ch1 -- PA7
    ch2 -- PB0
    CH3 -- PB1
    group1
    ch0 -- PB4
    ch1 -- PB5
    ch2 -- PB0
    CH3 -- PB1
timer13
    group0
    ch0 -- PA4
    group1
    ch0 -- PA7
    group2
    ch0 -- PB1
timer14
    group0
    ch0 -- PA2
    ch1 -- PA3
    group1
    ch0 -- PB14
    ch1 -- PB15
timer15
    group0
    ch0 -- PA6
    group1
    ch1 -- PB8
timer16
    group0
    ch0 -- PA7
    group1
    ch0 -- PB9
**/


void pwm_hal::pwm_init(uint32_t timer_periph,
                        uint16_t cycle,  
                        uint32_t resolution,
                        uint8_t timer_chx,
                        pwm_gpio_group gpio_group,
                        uint16_t ocnpolarity,
                        uint16_t ocnidlestate
                        )
{
    this->timer_num = timer_periph;
    this->resolution = resolution;
    this->init(timer_periph,cycle,resolution);
    switch (timer_periph)
    {
        case TIMER0:
        {
            if(timer_chx&timer_ch0)
            {
                //CH0--A
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_8);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
                gpio_af_set(GPIOA,GPIO_AF_2,GPIO_PIN_8);
            }
            if(timer_chx&timer_ch1)
            {
                //CH1--B
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_9);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
                gpio_af_set(GPIOA,GPIO_AF_2,GPIO_PIN_9);
            }
            if(timer_chx&timer_ch2)
            {
                //CH2--C
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_10);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
                gpio_af_set(GPIOA,GPIO_AF_2,GPIO_PIN_10);
            }
            if(timer_chx&timer_ch3)
            {
                //CH3--D
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_11);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
                gpio_af_set(GPIOA,GPIO_AF_2,GPIO_PIN_11);
            }
            break;
        }
        case TIMER2:
        {
            if(timer_chx&timer_ch2)
            {
                //CH2--C
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);
                gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
                gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_0);
            }
            if(timer_chx&timer_ch3)
            {
                //CH3--D
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);
                gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
                gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_1);
            }
            switch (gpio_group)
            {
                case pwm_gpio_group::group0:
                    if(timer_chx&timer_ch0)
                    {
                        //CH0--A
                        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_6);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
                        gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_6);
                    }
                    if(timer_chx&timer_ch1)
                    {
                        //CH1--B
                        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_7);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
                        gpio_af_set(GPIOA,GPIO_AF_1,GPIO_PIN_7);
                    }
                    break;
                case pwm_gpio_group::group1:
                    if(timer_chx&timer_ch0)
                    {
                        //CH0--A
                        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_4);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
                        gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_4);
                    }
                    if(timer_chx&timer_ch1)
                    {
                        //CH1--B
                        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
                        gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_5);
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case TIMER13:
        {
            if(timer_chx&timer_ch0)
            {
                switch (gpio_group)
                {
                case pwm_gpio_group::group0:
                    //CH0--A
                    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_4);
                    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
                    gpio_af_set(GPIOA,GPIO_AF_4,GPIO_PIN_4);
                    break;
                case pwm_gpio_group::group1:
                    //CH0--A
                    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_7);
                    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
                    gpio_af_set(GPIOA,GPIO_AF_4,GPIO_PIN_7);
                    break;
                case pwm_gpio_group::group2: 
                    //CH0--A
                    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);
                    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
                    gpio_af_set(GPIOB,GPIO_AF_0,GPIO_PIN_1);
                    break;
                default:
                    break;
                }



            }
            break;
        }
        case TIMER14:
        {
            switch (gpio_group)
            {
            case pwm_gpio_group::group0:
                if(timer_chx&timer_ch0)
                {
                    //CH0--A
                    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_2);
                    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
                    gpio_af_set(GPIOA,GPIO_AF_0,GPIO_PIN_2);
                }
                if(timer_chx&timer_ch1)
                {
                    //CH1--B
                    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_3);
                    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
                    gpio_af_set(GPIOA,GPIO_AF_0,GPIO_PIN_3);
                }
                break;
            case pwm_gpio_group::group1:
                if(timer_chx&timer_ch0)
                {
                    //CH0--A
                    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_14);
                    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
                    gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_14);
                }
                if(timer_chx&timer_ch1)
                {
                    //CH1--B
                    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_15);
                    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
                    gpio_af_set(GPIOB,GPIO_AF_1,GPIO_PIN_15);
                }
                break;
            default:
                break;
            }
            break;
        }
        case TIMER15:
        {
            if(timer_chx&timer_ch0)
            {
                switch (gpio_group)
                {
                    case pwm_gpio_group::group0:
                        //CH0--A
                        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_6);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
                        gpio_af_set(GPIOA,GPIO_AF_5,GPIO_PIN_6);
                        break;
                    case pwm_gpio_group::group1:
                        //CH0--A
                        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_8);
                        gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
                        gpio_af_set(GPIOB,GPIO_AF_2,GPIO_PIN_8);
                        break;
                    default:
                        break;
                }


            }
            break;
        }
        case TIMER16:
        {
            if(timer_chx&timer_ch0)
            {
                switch (gpio_group)
                {
                    case pwm_gpio_group::group0:
                        //CH0--A
                        gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_7);
                        gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
                        gpio_af_set(GPIOA,GPIO_AF_5,GPIO_PIN_7);
                        break;
                    case pwm_gpio_group::group1:
                        //CH0--A
                        gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO_PIN_9);
                        gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
                        gpio_af_set(GPIOB,GPIO_AF_2,GPIO_PIN_9);
                        break;
                    default:
                        break;
                }

            }
            break;
        }
        default:
            break;
    } 
        
	timer_oc_parameter_struct timer_ocinitparam;
	timer_ocinitparam.outputstate = TIMER_CCX_ENABLE;          //使能比较输出
	timer_ocinitparam.ocpolarity = ocnpolarity;      //输出极性低
	timer_ocinitparam.ocnidlestate = ocnidlestate;   //空闲状态电平
	
    if(timer_chx&timer_ch0)
    {
	    timer_channel_output_config(timer_periph,TIMER_CH_0,&timer_ocinitparam);
        timer_channel_output_pulse_value_config(timer_periph,TIMER_CH_0,0);//输出占空比0/1600
        timer_channel_output_mode_config(timer_periph,TIMER_CH_0,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(timer_periph,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
    }
    if(timer_chx&timer_ch1)
    {
	    timer_channel_output_config(timer_periph,TIMER_CH_1,&timer_ocinitparam);
        timer_channel_output_pulse_value_config(timer_periph,TIMER_CH_1,0);//输出占空比0/1600
        timer_channel_output_mode_config(timer_periph,TIMER_CH_1,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(timer_periph,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);
    }
    if(timer_chx&timer_ch2)
    {
	    timer_channel_output_config(timer_periph,TIMER_CH_2,&timer_ocinitparam);
        timer_channel_output_pulse_value_config(timer_periph,TIMER_CH_2,0);//输出占空比0/1600
        timer_channel_output_mode_config(timer_periph,TIMER_CH_2,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(timer_periph,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);
    }
    if(timer_chx&timer_ch3)
    {
	    timer_channel_output_config(timer_periph,TIMER_CH_3,&timer_ocinitparam);
        timer_channel_output_pulse_value_config(timer_periph,TIMER_CH_3,0);//输出占空比0/1600
        timer_channel_output_mode_config(timer_periph,TIMER_CH_3,TIMER_OC_MODE_PWM0);
        timer_channel_output_shadow_config(timer_periph,TIMER_CH_3,TIMER_OC_SHADOW_ENABLE);
    }
	timer_primary_output_config(timer_periph,ENABLE);


    
}


void pwm_hal::set_channel1_duty(float duty)
{
    timer_channel_output_pulse_value_config(this->timer_num,TIMER_CH_0,(uint32_t)(resolution*(duty/100)));
}
void pwm_hal::set_channel2_duty(float duty)
{
    timer_channel_output_pulse_value_config(this->timer_num,TIMER_CH_1,(uint32_t)(resolution*(duty/100)));
}
void pwm_hal::set_channel3_duty(float duty)
{
    timer_channel_output_pulse_value_config(this->timer_num,TIMER_CH_2,(uint32_t)(resolution*(duty/100)));
}
void pwm_hal::set_channel4_duty(float duty)
{
    timer_channel_output_pulse_value_config(this->timer_num,TIMER_CH_3,(uint32_t)(resolution*(duty/100)));
}















