#include "dma_hal.h"
//DMA0
static void (*DMA0_Channel0_Callback)(void *str)=0;//通道0中断回调函数指针
static void *DMA0_Channel0_str;
static void (*DMA0_Channel1_Callback)(void *str)=0;//通道1中断回调函数指针
static void *DMA0_Channel1_str;
static void (*DMA0_Channel2_Callback)(void *str)=0;//通道2中断回调函数指针
static void *DMA0_Channel2_str;
static void (*DMA0_Channel3_Callback)(void *str)=0;//通道3中断回调函数指针
static void *DMA0_Channel3_str;
static void (*DMA0_Channel4_Callback)(void *str)=0;//通道4中断回调函数指针
static void *DMA0_Channel4_str;
static void (*DMA0_Channel5_Callback)(void *str)=0;//通道5中断回调函数指针
static void *DMA0_Channel5_str;
static void (*DMA0_Channel6_Callback)(void *str)=0;//通道6中断回调函数指针
static void *DMA0_Channel6_str;
//DMA1
static void (*DMA1_Channel0_Callback)(void *str)=0;//通道0中断回调函数指针
static void *DMA1_Channel0_str;
static void (*DMA1_Channel1_Callback)(void *str)=0;//通道1中断回调函数指针
static void *DMA1_Channel1_str;
static void (*DMA1_Channel2_Callback)(void *str)=0;//通道2中断回调函数指针
static void *DMA1_Channel2_str;
static void (*DMA1_Channel3_Callback)(void *str)=0;//通道3中断回调函数指针
static void *DMA1_Channel3_str;
static void (*DMA1_Channel4_Callback)(void *str)=0;//通道4中断回调函数指针
static void *DMA1_Channel4_str;
bool dma_hal::dma_init( uint32_t dma_periph,        /*!< dma端口 */
                    dma_channel_enum dma_channel,   /*!< dma通道 */
                    uint32_t periph_addr,       /*!< 外设基址 */
                    uint32_t periph_width,      /*!< 外设的传输数据大小 */
                    uint32_t memory_addr,       /*!< 内存地址 */
                    uint32_t memory_width,      /*!< 传输数据的内存大小 */
                    uint32_t number,            /*!< 通道数据传输数量 */
                    uint32_t priority,          /*!< 通道优先级 */
                    uint8_t periph_inc,         /*!< 外设增加模式 */
                    uint8_t memory_inc,         /*!< 内存增加模式 */
                    uint8_t direction,           /*!< 通道数据传输方向 */
                    uint8_t nvic_irq_priority//中断优先级
                    )
{
    uint32_t ctl;
    this->dma_periph = dma_periph;
    this->dma_channel = dma_channel;
    if(DMA1 == this->dma_periph){//判断通道是否有效
        if(this->dma_channel > DMA_CH4){
            return false;
        }
    }
    switch (this->dma_periph)
    {
    case DMA0:
        rcu_periph_clock_enable(RCU_DMA0);
        break;
    case DMA1:
        rcu_periph_clock_enable(RCU_DMA1);
        break;
    default:
        break;
    }
    
    this->reset();
        /* configure peripheral base address */
    DMA_CHPADDR(this->dma_periph, this->dma_channel) = periph_addr;

    /* configure memory base address */
    DMA_CHMADDR(this->dma_periph, this->dma_channel) = memory_addr;
    
    /* configure the number of remaining data to be transferred */
    DMA_CHCNT(this->dma_periph, this->dma_channel) = (number & DMA_CHANNEL_CNT_MASK);

    /* configure peripheral transfer width,memory transfer width and priority */
    ctl = DMA_CHCTL(this->dma_periph, this->dma_channel);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO);
    ctl |= (periph_width | memory_width | priority);
    DMA_CHCTL(this->dma_periph, this->dma_channel) = ctl;

    /* configure peripheral increasing mode */
    if(DMA_PERIPH_INCREASE_ENABLE == periph_inc){
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_PNAGA;
    }else{
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_PNAGA;
    }

    /* configure memory increasing mode */
    if(DMA_MEMORY_INCREASE_ENABLE == memory_inc){
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_MNAGA;
    }else{
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_MNAGA;
    }

    /* configure the direction of data transfer */
    if(DMA_PERIPHERAL_TO_MEMORY == direction){
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_DIR;
    }else{
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_DIR;
    }
    
    //中断使能
    if(nvic_irq_priority!=255)
        switch (this->dma_periph)
        {
        case DMA0:
            switch (this->dma_channel)
            {
            case DMA_CH0:
                nvic_irq_enable(DMA0_Channel0_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH1:
                nvic_irq_enable(DMA0_Channel1_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH2:
                nvic_irq_enable(DMA0_Channel2_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH3:
                nvic_irq_enable(DMA0_Channel3_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH4:
                nvic_irq_enable(DMA0_Channel4_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH5:
                nvic_irq_enable(DMA0_Channel5_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH6:
                nvic_irq_enable(DMA0_Channel6_IRQn,nvic_irq_priority,0);
                break;
            default:
                break;
            }
            break;
        case DMA1:
            switch (this->dma_channel)
            {
            case DMA_CH0:
                nvic_irq_enable(DMA1_Channel0_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH1:
                nvic_irq_enable(DMA1_Channel1_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH2:
                nvic_irq_enable(DMA1_Channel2_IRQn,nvic_irq_priority,0);
                break;
            case DMA_CH3:
            case DMA_CH4:
                nvic_irq_enable(DMA1_Channel3_Channel4_IRQn,nvic_irq_priority,0);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

    return true;

}

void dma_hal::reset(void)
{
    /* disable DMA a channel */
    DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_CHEN;
    /* reset DMA channel registers */
    DMA_CHCTL(this->dma_periph, this->dma_channel) = DMA_CHCTL_RESET_VALUE;
    DMA_CHCNT(this->dma_periph, this->dma_channel) = DMA_CHCNT_RESET_VALUE;
    DMA_CHPADDR(this->dma_periph, this->dma_channel) = DMA_CHPADDR_RESET_VALUE;
    DMA_CHMADDR(this->dma_periph, this->dma_channel) = DMA_CHMADDR_RESET_VALUE;
    DMA_INTC(this->dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, this->dma_channel);
}

void dma_hal::circulation_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_CMEN;
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_CMEN;
        break;
    default:
        break;
    }
}

void dma_hal::channel_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_CHEN;
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_CHEN;
        break;
    default:
        break;
    } 
}

void dma_hal::memory_increase_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_MNAGA;
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_MNAGA;
        break;
    default:
        break;
    } 
}

void dma_hal::periph_increase_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_PNAGA;
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_PNAGA;
        break;
    default:
        break;
    } 
}

void dma_hal::periph_address_config(uint32_t address)
{
    DMA_CHPADDR(this->dma_periph, this->dma_channel) = address;
}

void dma_hal::memory_address_config(uint32_t address)
{
    DMA_CHMADDR(this->dma_periph, this->dma_channel) = address;
}

void dma_hal::transfer_number_config(uint32_t number)
{
    DMA_CHCNT(this->dma_periph, this->dma_channel) = (number & DMA_CHANNEL_CNT_MASK);
}

void dma_hal::priority_config(uint32_t priority)
{
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(this->dma_periph, this->dma_channel);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PRIO;
    ctl |= priority;
    DMA_CHCTL(this->dma_periph, this->dma_channel) = ctl;
}

void dma_hal::memory_width_config(uint32_t mwidth)
{
    uint32_t ctl;
        /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(this->dma_periph, this->dma_channel);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MWIDTH;
    ctl |= mwidth;
    DMA_CHCTL(this->dma_periph, this->dma_channel) = ctl;
}

void dma_hal::periph_width_config(uint32_t pwidth)
{
    uint32_t ctl;
        /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(this->dma_periph, this->dma_channel);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PWIDTH;
    ctl |= pwidth;
    DMA_CHCTL(this->dma_periph, this->dma_channel) = ctl;
}

void dma_hal::transfer_direction_config(uint32_t direction)
{
    if(DMA_PERIPHERAL_TO_MEMORY == direction){
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_DIR;
    } else {
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_DIR;
    }
}

FlagStatus dma_hal::flag_get(uint32_t flag)
{
    FlagStatus reval;
    if(RESET != (DMA_INTF(this->dma_periph) & DMA_FLAG_ADD(flag, this->dma_channel))){
        reval = SET;
    }else{
        reval = RESET;
    }
    return reval;
}

void dma_hal::flag_clear(uint32_t flag)
{
    DMA_INTC(this->dma_periph) |= DMA_FLAG_ADD(flag, this->dma_channel);
}

FlagStatus dma_hal::interrupt_flag_get(uint32_t flag)
{
    uint32_t interrupt_enable = 0U, interrupt_flag = 0U;

    switch(flag){
        case DMA_INT_FLAG_FTF:
            interrupt_flag = DMA_INTF(this->dma_periph) & DMA_FLAG_ADD(flag, this->dma_channel);
            interrupt_enable = DMA_CHCTL(this->dma_periph, this->dma_channel) & DMA_CHXCTL_FTFIE;
            break;
        case DMA_INT_FLAG_HTF:
            interrupt_flag = DMA_INTF(this->dma_periph) & DMA_FLAG_ADD(flag, this->dma_channel);
            interrupt_enable = DMA_CHCTL(this->dma_periph, this->dma_channel) & DMA_CHXCTL_HTFIE;
            break;
        case DMA_INT_FLAG_ERR:
            interrupt_flag = DMA_INTF(this->dma_periph) & DMA_FLAG_ADD(flag, this->dma_channel);
            interrupt_enable = DMA_CHCTL(this->dma_periph, this->dma_channel) & DMA_CHXCTL_ERRIE;
            break;
        default:
            for(;;);
        }

    if(interrupt_flag && interrupt_enable){
        return SET;
    }else{
        return RESET;
    }
}

void dma_hal::interrupt_flag_clear(uint32_t flag)
{
    DMA_INTC(this->dma_periph) |= DMA_FLAG_ADD(flag, this->dma_channel);
}
/*!
    \brief      DMA 中断开关
    \param[in]  source: 指定中断源
                可以选择一个或多个参数，如下所示
      \arg        DMA_INT_FTF: 通道完全传输完成中断
      \arg        DMA_INT_HTF: 通道半传输完成中断
      \arg        DMA_INT_ERR: 通道错误中断
    \param[out] none
    \retval     none
*/
void dma_hal::interrupt_switch(uint32_t source,ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= source;
        
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~source;
        break;
    default:
        break;
    }
    
}


void dma_hal::memory_to_memory_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) |= DMA_CHXCTL_M2M;
        break;
    case DISABLE:
        DMA_CHCTL(this->dma_periph, this->dma_channel) &= ~DMA_CHXCTL_M2M;
        break;
    
    default:
        break;
    }
}

void dma_hal::Callback_Register(void *str,void (*app)(void*))
{
    switch (this->dma_periph)
    {
    case DMA0:
        switch (this->dma_channel)
        {
        case DMA_CH0:
            DMA0_Channel0_Callback = app;
            DMA0_Channel0_str = str;
            break;
        case DMA_CH1:
            DMA0_Channel1_Callback = app;
            DMA0_Channel1_str = str;
            break;
        case DMA_CH2:
            DMA0_Channel2_Callback = app;
            DMA0_Channel2_str = str;
            break;
        case DMA_CH3:
            DMA0_Channel3_Callback = app;
            DMA0_Channel3_str = str;
            break;
        case DMA_CH4:
            DMA0_Channel4_Callback = app;
            DMA0_Channel4_str = str;
            break;
        case DMA_CH5:
            DMA0_Channel5_Callback = app;
            DMA0_Channel5_str = str;
            break;
        case DMA_CH6:
            DMA0_Channel6_Callback = app;
            DMA0_Channel6_str = str;
            break;
        default:
            break;
        }
        break;
    case DMA1:
        switch (this->dma_channel)
        {
        case DMA_CH0:
            DMA1_Channel0_Callback = app;
            DMA1_Channel0_str = str;
            break;
        case DMA_CH1:
            DMA1_Channel1_Callback = app;
            DMA1_Channel1_str = str;
            break;
        case DMA_CH2:
            DMA1_Channel2_Callback = app;
            DMA1_Channel2_str = str;
            break;
        case DMA_CH3:
            DMA1_Channel3_Callback = app;
            DMA1_Channel3_str = str;
            break;
        case DMA_CH4:
            DMA1_Channel4_Callback = app;
            DMA1_Channel4_str = str;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

extern "C"
{
void DMA0_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH0,DMA_INT_FTF) && DMA0_Channel0_Callback !=0)
    {
        DMA0_Channel0_Callback(DMA0_Channel0_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH0,DMA_INT_FTF);
    }
}
void DMA0_Channel1_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH1,DMA_INT_FTF) && DMA0_Channel1_Callback != 0)
    {
        DMA0_Channel1_Callback(DMA0_Channel1_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH1,DMA_INT_FTF);
    }
}
void DMA0_Channel2_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH2,DMA_INT_FTF) && DMA0_Channel2_Callback != 0)
    {
        DMA0_Channel2_Callback(DMA0_Channel2_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH2,DMA_INT_FTF);
    }
}
void DMA0_Channel3_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH3,DMA_INT_FTF) && DMA0_Channel3_Callback != 0)
    {
        DMA0_Channel3_Callback(DMA0_Channel3_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH3,DMA_INT_FTF);
    }
}
void DMA0_Channel4_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH4,DMA_INT_FTF) && DMA0_Channel4_Callback != 0)
    {
        DMA0_Channel4_Callback(DMA0_Channel4_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH4,DMA_INT_FTF);
    }
}

void DMA0_Channel5_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH5,DMA_INT_FTF) && DMA0_Channel5_Callback != 0)
    {
        DMA0_Channel5_Callback(DMA0_Channel5_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH5,DMA_INT_FTF);
    }
}

void DMA0_Channel6_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0,DMA_CH6,DMA_INT_FTF) && DMA0_Channel6_Callback != 0)
    {
        DMA0_Channel6_Callback(DMA0_Channel6_str);
        dma_interrupt_flag_clear(DMA0,DMA_CH6,DMA_INT_FTF);
    }
}

void DMA1_Channel0_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH0,DMA_INT_FTF) && DMA1_Channel0_Callback != 0)
    {
        DMA1_Channel0_Callback(DMA1_Channel0_str);
        dma_interrupt_flag_clear(DMA1,DMA_CH0,DMA_INT_FTF);
    }
}
void DMA1_Channel1_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH1,DMA_INT_FTF) && DMA1_Channel1_Callback != 0)
    {
        DMA1_Channel1_Callback(DMA1_Channel1_str);
        dma_interrupt_flag_clear(DMA1,DMA_CH1,DMA_INT_FTF);
    }
}
void DMA1_Channel2_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH2,DMA_INT_FTF) && DMA1_Channel2_Callback != 0)
    {
        DMA1_Channel2_Callback(DMA1_Channel2_str);
        dma_interrupt_flag_clear(DMA1,DMA_CH2,DMA_INT_FTF);
    }
}

void DMA1_Channel3_4_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1,DMA_CH3,DMA_INT_FTF) && DMA1_Channel3_Callback != 0)
    {
        DMA1_Channel3_Callback(DMA1_Channel3_str);
        dma_interrupt_flag_clear(DMA1,DMA_CH3,DMA_INT_FTF);
    }
    if(dma_interrupt_flag_get(DMA1,DMA_CH4,DMA_INT_FTF) && DMA1_Channel4_Callback != 0)
    {
        DMA1_Channel4_Callback(DMA1_Channel4_str);
        dma_interrupt_flag_clear(DMA1,DMA_CH4,DMA_INT_FTF);
    }
}

}


