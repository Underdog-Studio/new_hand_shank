#include "adc_hal.h"

uint8_t ADC0_channel_buff[14]={0};


uint16_t* get_adc_data_addr(uint32_t ADCX);
static FlagStatus ADC0_init_Flag = RESET;//ADC初始化标志
static uint8_t ADC0_channel_Len=0;//ADC通道数
static uint16_t adc0_hal_data[14];//ADC数据缓冲区
static uint8_t adc0_channel_count=0;//ADC通道计数

static FlagStatus ADC1_init_Flag = RESET;

static uint16_t adc1_hal_data[14];
static uint8_t adc1_channel_count=0;

static FlagStatus ADC2_init_Flag = RESET;
static uint8_t ADC2_channel_Len=0;//ADC通道数
static uint16_t adc2_hal_data[14];
static uint8_t adc2_channel_count=0;






void adc_hal::adc_init(uint32_t adcx,uint8_t adc_channel)
{
    this->adc_periph = adcx;
    this->adc_channel = adc_channel;
    switch (adcx)
    {
    case ADC0:
        rcu_periph_clock_enable(RCU_ADC0);
        this->adc_rank = ADC0_channel_Len;//通道编号
        ADC0_channel_Len+=1;//通道数
        ADC0_channel_buff[this->adc_rank] = adc_channel;
        this->dma_init( DMA0,
                    DMA_CH0,
                    (uint32_t)(&ADC_RDATA(this->adc_periph)),
                    DMA_PERIPHERAL_WIDTH_16BIT,
                    (uint32_t)get_adc_data_addr(this->adc_periph),
                    DMA_MEMORY_WIDTH_16BIT,
                    ADC0_channel_Len,
                    DMA_PRIORITY_LOW,
                    DMA_PERIPH_INCREASE_DISABLE,
                    DMA_MEMORY_INCREASE_ENABLE,
                    DMA_PERIPHERAL_TO_MEMORY
                    );
        this->circulation_switch(ENABLE);
        this->memory_to_memory_switch(DISABLE);
        this->channel_switch(ENABLE);
       // if(ADC0_init_Flag==RESET)
        //{
            ADC0_init_Flag = SET;
            rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV12); //9M
            this->adc_reset();
            this->adc_mode_config(ADC_MODE_FREE);
            this->adc_resolution_config(ADC_RESOLUTION_12B);
            ////连续转换
            this->adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
            this->adc_special_function_config(ADC_SCAN_MODE,ENABLE);
            
            this->adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
            
            this->adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
            this->adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
       // }

        this->adc_channel_length_config(ADC_REGULAR_CHANNEL,ADC0_channel_Len);
        
        this->adc_channel_init(adc_channel);
        this->adc_switch(ENABLE);
        break;
    case ADC1:
        rcu_periph_clock_enable(RCU_ADC1);

        rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV12); //9M
        this->adc_reset();
        this->adc_resolution_config(ADC_RESOLUTION_12B);
        this->adc_data_alignment_config(ADC_DATAALIGN_RIGHT);//数据右对齐
        this->adc_channel_length_config(ADC_REGULAR_CHANNEL,1);
        this->adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);//外部触发使能
        this->adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);//触发源选择软件触发

        
        
        this->adc_switch(ENABLE);
        break;
    case ADC2:
        rcu_periph_clock_enable(RCU_ADC2);
        this->dma_init( DMA1,
                    DMA_CH4,
                    (uint32_t)(&ADC_RDATA(this->adc_periph)),
                    DMA_PERIPHERAL_WIDTH_16BIT,
                    (uint32_t)get_adc_data_addr(this->adc_periph),
                    DMA_MEMORY_WIDTH_16BIT,
                    ADC2_channel_Len,
                    DMA_PRIORITY_LOW,
                    DMA_PERIPH_INCREASE_DISABLE,
                    DMA_MEMORY_INCREASE_ENABLE,
                    DMA_PERIPHERAL_TO_MEMORY
                    );
        this->circulation_switch(ENABLE);
        this->memory_to_memory_switch(DISABLE);
        this->channel_switch(ENABLE);

        rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV12); //9M
        this->adc_reset();
        this->adc_mode_config(ADC_MODE_FREE);
        this->adc_resolution_config(ADC_RESOLUTION_12B);
        ////连续转换
        this->adc_special_function_config(ADC_CONTINUOUS_MODE,ENABLE);
        this->adc_special_function_config(ADC_SCAN_MODE,ENABLE);
        
        this->adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
        this->adc_channel_length_config(ADC_REGULAR_CHANNEL,ADC2_channel_Len);

        this->adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);
        this->adc_external_trigger_source_config(ADC_REGULAR_CHANNEL,ADC0_1_2_EXTTRIG_REGULAR_NONE);
        break;
    default:
        break;
    }

}

void adc_hal::adc_channel_init(uint8_t adc_channel)
{
    uint8_t i;
    switch (this->adc_periph)
    {
    case ADC0:
        switch (adc_channel)
        {
            case ADC_CHANNEL_0:
                this->gpio__init(GPIOA,GPIO_PIN_0,GPIO_MODE_AIN);  
                break;
            case ADC_CHANNEL_1:
                this->gpio__init(GPIOA,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_2:
                this->gpio__init(GPIOA,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_3:
                this->gpio__init(GPIOA,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_4:
                this->gpio__init(GPIOA,GPIO_PIN_4,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_5:
                this->gpio__init(GPIOA,GPIO_PIN_5,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_6:
                this->gpio__init(GPIOA,GPIO_PIN_6,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_7:
                this->gpio__init(GPIOA,GPIO_PIN_7,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_8:
                this->gpio__init(GPIOB,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_9:
                this->gpio__init(GPIOB,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_10:
                this->gpio__init(GPIOC,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_11:
                this->gpio__init(GPIOC,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_12:
                this->gpio__init(GPIOC,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_13:
                this->gpio__init(GPIOC,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_14:
                this->gpio__init(GPIOC,GPIO_PIN_4,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_15:
                this->gpio__init(GPIOC,GPIO_PIN_5,GPIO_MODE_AIN);
                break;
            default:
                break;
        }
        for(i=0;i<ADC0_channel_Len;i++)
            adc_regular_channel_config(i,ADC0_channel_buff[i], ADC_SAMPLETIME_239POINT5);
        break;
    case ADC1:  
        switch (adc_channel)
        {
            case ADC_CHANNEL_0:
                this->gpio__init(GPIOA,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_1:
                this->gpio__init(GPIOA,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_2:
                this->gpio__init(GPIOA,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_3:
                this->gpio__init(GPIOA,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_4:
                this->gpio__init(GPIOA,GPIO_PIN_4,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_5:
                this->gpio__init(GPIOA,GPIO_PIN_5,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_6:
                this->gpio__init(GPIOA,GPIO_PIN_6,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_7:
                this->gpio__init(GPIOA,GPIO_PIN_7,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_8:
                this->gpio__init(GPIOB,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_9:
                this->gpio__init(GPIOB,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_10:
                this->gpio__init(GPIOC,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_11:
                this->gpio__init(GPIOC,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_12:
                this->gpio__init(GPIOC,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_13:
                this->gpio__init(GPIOC,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_14:
                this->gpio__init(GPIOC,GPIO_PIN_4,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_15:
                this->gpio__init(GPIOC,GPIO_PIN_5,GPIO_MODE_AIN);
                break;
            default:
                break;
        }   
        adc_regular_channel_config(this->adc_rank,adc_channel , ADC_SAMPLETIME_55POINT5);
        break;
    case ADC2:
        switch (adc_channel)
        {
            case ADC_CHANNEL_0:
                this->gpio__init(GPIOA,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_1:
                this->gpio__init(GPIOA,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_2:
                this->gpio__init(GPIOA,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_3:
                this->gpio__init(GPIOA,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_10:
                this->gpio__init(GPIOC,GPIO_PIN_0,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_11:
                this->gpio__init(GPIOC,GPIO_PIN_1,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_12:
                this->gpio__init(GPIOC,GPIO_PIN_2,GPIO_MODE_AIN);
                break;
            case ADC_CHANNEL_13:
                this->gpio__init(GPIOC,GPIO_PIN_3,GPIO_MODE_AIN);
                break;
            default:
                break;
        }
        adc_regular_channel_config(this->adc_rank,adc_channel , ADC_SAMPLETIME_239POINT5);
        break;
    default:
        break;
    }  
    
}

void adc_hal::adc_reset(void)
{
    switch(this->adc_periph){
    case ADC0:
        rcu_periph_reset_enable(RCU_ADC0RST);
        rcu_periph_reset_disable(RCU_ADC0RST);
        break;
    case ADC1:
        rcu_periph_reset_enable(RCU_ADC1RST);
        rcu_periph_reset_disable(RCU_ADC1RST);
        break;
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))    
    case ADC2:
        rcu_periph_reset_enable(RCU_ADC2RST);
        rcu_periph_reset_disable(RCU_ADC2RST);
        break;
#endif    
    default:
        break;      
    }
}

/*!
    \brief      打开或关闭ADC接口
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_hal::adc_switch(ControlStatus newvalue)
{
    switch (newvalue)
    {
    case ENABLE:
        if(RESET == (ADC_CTL1(this->adc_periph) & ADC_CTL1_ADCON)){
        ADC_CTL1(this->adc_periph) |= (uint32_t)ADC_CTL1_ADCON;
        } 
        this->adc_calibration();//校准ADC
        this->adc_dma_mode_switch(ENABLE);//默认打开dma
        this->adc_software_trigger_enable(ADC_REGULAR_CHANNEL);//软件触发使能
        break;
    case DISABLE:
        ADC_CTL1(this->adc_periph) &= ~((uint32_t)ADC_CTL1_ADCON);
        break;
    default:
        break;
    }
}

/*!
    \brief 配置adc同步模式
    \param adc模式
        这些参数中只能选择一个 带-的不能使用
        ADC_MODE_FREE 所有adc独立工作
        -ADC_DAUL_REGULAL_PARALLEL_INSERTED_PARALLEL  ADC0和ADC1在组合的常规并联+插入并联模式下工作
        -ADC_DAUL_REGULAL_PARALLEL_INSERTED_ROTATION  ADC0和ADC1在组合的常规并行+触发器旋转模式下工作
        -ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_FAST  ADC0和ADC1在组合插入并联+后续快速模式下工作
        -ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_SLOW  ADC0和ADC1在组合插入并联+后续慢速模式下工作
        -ADC_DAUL_INSERTED_PARALLEL  ADC0和ADC1仅在插入式并行模式下工作
        ADC_DAUL_REGULAL_PARALLEL  ADC0和ADC1仅在常规并行模式下工作
        ADC_DAUL_REGULAL_FOLLOWUP_FAST  ADC0和ADC1在常规快速交叉模式下工作
        ADC_DAUL_REGULAL_FOLLOWUP_SLOW  ADC0和ADC1在常规慢速交叉模式下工作
        -ADC_DAUL_INSERTED_TRRIGGER_ROTATION  ADC0和ADC1仅在触发旋转模式下工作
*/
void adc_hal::adc_mode_config(uint32_t mode)
{
    ADC_CTL0(this->adc_periph) &= ~(ADC_CTL0_SYNCM);
    ADC_CTL0(this->adc_periph) |= mode;
}

/*!
    \brief      启用或禁用 ADC 特殊功能
    \param[in]  function: 要配置的功能
                可以在下面选择一个或多个参数
      \arg        ADC_SCAN_MODE: 扫描模式选择
      \arg        ADC_INSERTED_CHANNEL_AUTO: 插入的通道组自动转换
      \arg        ADC_CONTINUOUS_MODE: 连续模式选择
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_hal::adc_special_function_config(uint32_t function , ControlStatus newvalue)
{
    if(newvalue){
        if(0U != (function & ADC_SCAN_MODE)){
            ADC_CTL0(this->adc_periph) |= ADC_SCAN_MODE;
        }
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)){
            ADC_CTL0(this->adc_periph) |= ADC_INSERTED_CHANNEL_AUTO;
        } 
        if(0U != (function & ADC_CONTINUOUS_MODE)){
            ADC_CTL1(this->adc_periph) |= ADC_CONTINUOUS_MODE;
        }        
    }else{
        if(0U != (function & ADC_SCAN_MODE)){
            ADC_CTL0(this->adc_periph) &= ~ADC_SCAN_MODE;
        }
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)){
            ADC_CTL0(this->adc_periph) &= ~ADC_INSERTED_CHANNEL_AUTO;
        } 
        if(0U != (function & ADC_CONTINUOUS_MODE)){
            ADC_CTL1(this->adc_periph) &= ~ADC_CONTINUOUS_MODE;
        }       
    }
}

/*!
    \brief      配置模数转换器数据对齐
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[in]  data_alignment: 数据对齐选择
                只能选择一个参数
      \arg        ADC_DATAALIGN_RIGHT: LSB对齐(向右)
      \arg        ADC_DATAALIGN_LEFT: MSB对齐(向左)
    \param[out] none
    \retval     none
*/
void adc_hal::adc_data_alignment_config(uint32_t data_alignment)
{
    if(ADC_DATAALIGN_RIGHT != data_alignment){
        ADC_CTL1(this->adc_periph) |= ADC_CTL1_DAL;
    }else{
        ADC_CTL1(this->adc_periph) &= ~((uint32_t)ADC_CTL1_DAL);
    }
}

/*!
    \brief      配置常规通道组或插入的通道组的长度
    \param[in]  adc_channel_group: 选择通道组
                只能选择一个参数
      \arg        ADC_REGULAR_CHANNEL: 常规通道组
      \arg        ADC_INSERTED_CHANNEL: 插入通道组
    \param[in]  length: 通道的长度
                        常规的通道组 1-16
                        插入的通道组 1-4
    \param[out] none
    \retval     none
*/
void adc_hal::adc_channel_length_config(uint8_t adc_channel_group, uint32_t length)
{
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_RSQ0(this->adc_periph) &= ~((uint32_t)ADC_RSQ0_RL);
        ADC_RSQ0(this->adc_periph) |= RSQ0_RL((uint32_t)(length-1U));

        break;
    case ADC_INSERTED_CHANNEL:
        ADC_ISQ(this->adc_periph) &= ~((uint32_t)ADC_ISQ_IL);
        ADC_ISQ(this->adc_periph) |= ISQ_IL((uint32_t)(length-1U));
    
        break;
    default:
        break;
    }
}

/*!
    \brief      配置ADC常规通道
    \param[in]  rank: 常规组序列号，此参数必须介于 0 到 15 之间
    \param[in]  adc_channel: 选定的ADC通道
                这些参数中只能选择一个
      \arg        ADC_CHANNEL_x(x=0..17)(x=16 and x=17 are only for ADC0): ADC通道
    \param[in]  sample_time: 采样时间值
                只能选择一个参数
      \arg        ADC_SAMPLETIME_1POINT5: 1.5 周期
      \arg        ADC_SAMPLETIME_7POINT5: 7.5 cycles
      \arg        ADC_SAMPLETIME_13POINT5: 13.5 cycles
      \arg        ADC_SAMPLETIME_28POINT5: 28.5 cycles
      \arg        ADC_SAMPLETIME_41POINT5: 41.5 cycles
      \arg        ADC_SAMPLETIME_55POINT5: 55.5 cycles
      \arg        ADC_SAMPLETIME_71POINT5: 71.5 cycles
      \arg        ADC_SAMPLETIME_239POINT5: 239.5 cycles
    \param[out] none
    \retval     none
*/
void adc_hal::adc_regular_channel_config(uint8_t rank , uint8_t adc_channel , uint32_t sample_time)
{
    uint32_t rsq,sampt;
    
    /* ADC 常规序列配置 */
    if(rank < 6U){
        rsq = ADC_RSQ2(this->adc_periph);
        rsq &=  ~((uint32_t)(ADC_RSQX_RSQN << (5U*rank)));
        rsq |= ((uint32_t)adc_channel << (5U*rank));
        ADC_RSQ2(this->adc_periph) = rsq;
    }else if(rank < 12U){
        rsq = ADC_RSQ1(this->adc_periph);
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-6U))));
        rsq |= ((uint32_t)adc_channel << (5U*(rank-6U)));
        ADC_RSQ1(this->adc_periph) = rsq;
    }else if(rank < 16U){
        rsq = ADC_RSQ0(this->adc_periph);
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-12U))));
        rsq |= ((uint32_t)adc_channel << (5U*(rank-12U)));
        ADC_RSQ0(this->adc_periph) = rsq;
    }else{
    }
    
    /* ADC采样时间配置 */
    if(adc_channel < 10U){
        sampt = ADC_SAMPT1(this->adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*adc_channel)));
        sampt |= (uint32_t)(sample_time << (3U*adc_channel));
        ADC_SAMPT1(this->adc_periph) = sampt;
    }else if(adc_channel < 18U){
        sampt = ADC_SAMPT0(this->adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*(adc_channel-10U))));
        sampt |= (uint32_t)(sample_time << (3U*(adc_channel-10U)));
        ADC_SAMPT0(this->adc_periph) = sampt;
    }else{
    }
}

/*!
    \brief      使能 ADC 外部触发
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[in]  adc_channel_group: 选择通道组
                可以选择一个或多个参数
      \arg        ADC_REGULAR_CHANNEL: 常规通道组
      \arg        ADC_INSERTED_CHANNEL: 插入通道组
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_hal::adc_external_trigger_config(uint8_t adc_channel_group, ControlStatus newvalue)
{
    if(newvalue){
        if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
            ADC_CTL1(this->adc_periph) |= ADC_CTL1_ETERC;
        }
        if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
            ADC_CTL1(this->adc_periph) |= ADC_CTL1_ETEIC;
        }        
    }else{
        if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
            ADC_CTL1(this->adc_periph) &= ~ADC_CTL1_ETERC;
        }
        if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
            ADC_CTL1(this->adc_periph) &= ~ADC_CTL1_ETEIC;
        }      
    }
}

/*!
    \brief      配置 ADC 外部触发源
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[in]  adc_channel_group: 选择通道组
                只能选择一个参数
      \arg        ADC_REGULAR_CHANNEL: 常规通道组
      \arg        ADC_INSERTED_CHANNEL: 插入通道组
    \param[in]  external_trigger_source: 常规或插入的组触发源
                只能选择一个参数
                常规通道:
      \arg        ADC0_1_EXTTRIG_REGULAR_T0_CH0: timer 0 CC0 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T0_CH1: timer 0 CC1 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T0_CH2: timer 0 CC2 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T1_CH1: timer 1 CC1 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T2_TRGO: timer 2 TRGO 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T3_CH3: timer 3 CC3 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_T7_TRGO: timer 7 TRGO 事件选择
      \arg        ADC0_1_EXTTRIG_REGULAR_EXTI_11 : 外部中断线 11
      \arg        ADC2_EXTTRIG_REGULAR_T2_CH0: timer 2 CC0 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T1_CH2: timer 1 CC2 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T0_CH2: timer 0 CC2 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T7_CH0: timer 7 CC0 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T7_TRGO: timer 7 TRGO 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T4_CH0: timer 4 CC0 事件选择
      \arg        ADC2_EXTTRIG_REGULAR_T4_CH2: timer 4 CC2 事件选择
      \arg        ADC0_1_2_EXTTRIG_REGULAR_NONE: 软件触发  
                插入通道:
      \arg        ADC0_1_EXTTRIG_INSERTED_T0_TRGO: timer 0 TRGO 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_T0_CH3: timer 0 CC3 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_T1_TRGO: timer 1 TRGO 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_T1_CH0: timer 1 CC0 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_T2_CH3: timer 2 CC3 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_T3_TRGO: timer 3 TRGO 事件选择
      \arg        ADC0_1_EXTTRIG_INSERTED_EXTI_15: 外部中断线 15
      \arg        ADC0_1_EXTTRIG_INSERTED_T7_CH3: timer 7 CC3 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T0_TRGO: timer 0 TRGO 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T0_CH3: timer 0 CC3 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T3_CH2: timer 3 CC2 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T7_CH1: timer 7 CC1 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T7_CH3: timer 7 CC3 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T4_TRGO: timer 4 TRGO 事件选择
      \arg        ADC2_EXTTRIG_INSERTED_T4_CH3: timer 4 CC3 事件选择
      \arg        ADC0_1_2_EXTTRIG_INSERTED_NONE: 软件触发  
    \param[out] none
    \retval     none
*/
void adc_hal::adc_external_trigger_source_config(uint8_t adc_channel_group, uint32_t external_trigger_source)
{
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL1(this->adc_periph) &= ~((uint32_t)ADC_CTL1_ETSRC);
        ADC_CTL1(this->adc_periph) |= (uint32_t)external_trigger_source;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL1(this->adc_periph) &= ~((uint32_t)ADC_CTL1_ETSIC);
        ADC_CTL1(this->adc_periph) |= (uint32_t)external_trigger_source;
        break;
    default:
        break;
    }
}
/*!
    \brief      ADC校准和复位校准
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[out] none
    \retval     none
*/
void adc_hal::adc_calibration(void)
{
    /* 复位选定的ADC校准寄存器 */
    ADC_CTL1(this->adc_periph) |= (uint32_t) ADC_CTL1_RSTCLB;
    /* check the RSTCLB bit state */
    while((ADC_CTL1(this->adc_periph) & ADC_CTL1_RSTCLB)){
    }
    /* enable ADC calibration process */
    ADC_CTL1(this->adc_periph) |= ADC_CTL1_CLB;
    /* check the CLB bit state */
    while((ADC_CTL1(this->adc_periph) & ADC_CTL1_CLB)){
    }
}

/*!
    \brief      打开或关闭DMA
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void adc_hal::adc_dma_mode_switch(ControlStatus newvalue)
{
    switch (newvalue)
    {
    case ENABLE:
        ADC_CTL1(this->adc_periph) |= (uint32_t)(ADC_CTL1_DMA);
        break;
    case DISABLE:
        ADC_CTL1(this->adc_periph) &= ~((uint32_t)ADC_CTL1_DMA);
        break;
    default:
        break;
    }
}

/*!
    \brief     打开adc软件触发
    \param[in]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \param[in]  adc_channel_group: select the channel group
                可以选择一个或多个参数
      \arg        ADC_REGULAR_CHANNEL: 常规通道
      \arg        ADC_INSERTED_CHANNEL: 插入通道
    \param[out] none
    \retval     none
*/
void adc_hal::adc_software_trigger_enable(uint8_t adc_channel_group)
{
    if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
        ADC_CTL1(this->adc_periph) |= ADC_CTL1_SWRCST;
    }
    if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
        ADC_CTL1(this->adc_periph) |= ADC_CTL1_SWICST;
    }
}

uint16_t* get_adc_data_addr(uint32_t ADCX)
{
    switch (ADCX)
    {
    case ADC0:
        return adc0_hal_data;
        break;
    case ADC1:
        return adc1_hal_data;
        break;
    case ADC2:
        return adc2_hal_data;
        break;
    default:
        break;
    }
    return (uint16_t*)0;
}

/*!
    \brief      配置ADC分辨率 
    \param[in]  resolution: ADC分辨率
                only one among these parameters can be selected
      \arg        ADC_RESOLUTION_12B: 12-bit ADC 分辨率
      \arg        ADC_RESOLUTION_10B: 10-bit ADC 分辨率
      \arg        ADC_RESOLUTION_8B: 8-bit ADC 分辨率
      \arg        ADC_RESOLUTION_6B: 6-bit ADC 分辨率
    \param[out] none
    \retval     none
*/
void adc_hal::adc_resolution_config(uint32_t resolution)
{
    ADC_OVSAMPCTL(this->adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_DRES);
    ADC_OVSAMPCTL(this->adc_periph) |= (uint32_t)resolution;
}

uint16_t adc_hal::adc_get_channel_value(void)
{
    switch (this->adc_periph)
    {
        case ADC0:
            return adc0_hal_data[this->adc_rank];
            break;
        case ADC1:
            return adc1_hal_data[this->adc_rank];
            break;
        case ADC2:
            this->adc_regular_channel_config(0,this->adc_channel,ADC_SAMPLETIME_55POINT5);
            while(!adc_flag_get(this->adc_periph,ADC_FLAG_EOC));
            adc_flag_clear(this->adc_periph,ADC_FLAG_EOC);
            return (adc_regular_data_read(this->adc_periph)&0x0fff);
            break;
        default:
        return 0;
    }

}

