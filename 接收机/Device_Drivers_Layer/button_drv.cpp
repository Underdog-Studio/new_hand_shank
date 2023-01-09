#include "button_drv.h"

static void button_callback(void *str);

void button_drv::button_init(	uint32_t gpio_periph,
              					uint32_t pin,
								trigger_levels trigger_level)
{
	this->trigger_level = trigger_level;//触发电平
	if(this->trigger_level == trigger_levels::trigger_low)
		this->init((exti_line_enum)pin,gpio_periph,3,EXTI_INTERRUPT,EXTI_TRIG_FALLING,GPIO_PUPD_PULLUP);//上拉下降沿中断
	else
		this->init((exti_line_enum)pin,gpio_periph,3,EXTI_INTERRUPT,EXTI_TRIG_RISING,GPIO_PUPD_PULLDOWN);//下拉上升沿中断
	this->Callback_Register(this,button_callback);//按键回调注册
	this->Button_T = Get_System_T();//按键时间跟新
}



/*
*@ 获取按键单击
*@ return false未产生 true产生
*/
bool button_drv::Get_Button_Click(void)
{
	if(Get_deta_T(this->Button_T)>0.2f)
	{
		bool temp = this->Button_Click;
		this->Button_Click = false;
		return temp;
	}
	return false;
}
/*
*@ 获取按键双击
*@ return false未产生 true产生
*/
bool button_drv::Get_Button_Double_Click(void)
{
	if(Get_deta_T(this->Button_T)<0.2f)
	{
		bool temp = this->Button_Double_Click;
		this->Button_Double_Click = false;
		return temp;
	}
	return false;
}
/*
*@ 获取按键长按
*@ return false未产生 true产生
*/
bool button_drv::Get_Button_Long_Press(void)
{
	bool temp = this->Button_Long_Press;
	this->Button_Long_Press = false;
	return temp;
}


/*
*@ 按键中断回调函数
*/
static void button_callback(void *str)
{
	float Deta_T=Get_deta_T(((button_drv*)str)->Button_T);//获取时间间隔
	if(((button_drv*)str)->Button_State==false)//按下触发
	{
		if(Deta_T<0.3f)
			((button_drv*)str)->Button_Count++;
		
		else
			((button_drv*)str)->Button_Count = 0;
		((button_drv*)str)->Button_T = Get_System_T();
		((button_drv*)str)->Button_State=true;
		if(((button_drv*)str)->trigger_level == button_drv::trigger_levels::trigger_low)
			((button_drv*)str)->set_exti_trig(EXTI_TRIG_RISING);//上升沿
		else
			((button_drv*)str)->set_exti_trig(EXTI_TRIG_FALLING);//下降沿
	}else//松开触发
	{
		if(Deta_T<0.3f)
		{
			((button_drv*)str)->Button_Count++;
			if(((button_drv*)str)->Button_Count == 1)//单击
			{
				((button_drv*)str)->Button_Click = true;
				((button_drv*)str)->Button_Count = 0;
			}else if(((button_drv*)str)->Button_Count == 2 && ((button_drv*)str)->Button_Click == true)//双击
			{
				((button_drv*)str)->Button_Click = false;
				((button_drv*)str)->Button_Double_Click = true;
				((button_drv*)str)->Button_Count = 0;
			}
		}else
			((button_drv*)str)->Button_Count = 0;
		if(Deta_T>0.5f)//长按
		{
			((button_drv*)str)->Button_Long_Press=true;
			((button_drv*)str)->Button_Count = 0;
		}
		((button_drv*)str)->Button_T = Get_System_T();
		((button_drv*)str)->Button_State = false;

		if(((button_drv*)str)->trigger_level == button_drv::trigger_levels::trigger_low)
			((button_drv*)str)->set_exti_trig(EXTI_TRIG_FALLING);//下降沿
		else
			((button_drv*)str)->set_exti_trig(EXTI_TRIG_RISING);//上升沿	
	}
}



