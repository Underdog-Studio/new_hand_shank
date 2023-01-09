#include "main.h"
#include "gpio_hal.h"
#include "System_time.h"
#include "st7789_drv.h"
#include "remote_pole_dev.h"
#include "button_drv.h"
#include "si24r1/si24r1_master_drv.h"
#include "battery_testing_drv.h"
#include "remote_control_gui.h"


#include "lv_obj.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

#include "FreeRTOS.h"
#include "task.h"
#include "src\draw\lv_img_buf.h"

#include "ff.h"
#include "diskio.h"

#include "Flash/W25Q.h"

//遥杆任务
#define remote_pole_task_STK_SIZE	512
TaskHandle_t remote_pole_task_Handler;
#define remote_pole_task_PRIO	5
void remote_pole_task(void *pvParameters);
//主界面任务
#define UI_task_STK_SIZE	512
TaskHandle_t UI_task_Handler;
#define UI_task_PRIO	11
void UI_task(void *pvParameters);
//设置任务
#define Config_task_STK_SIZE	512
TaskHandle_t Config_task_Handler;
#define Config_task_PRIO	10
void Config_task(void *pvParameters);

W25Q SPI_FLASH;
void SPI_FLASH_Write(uint8_t *write_buffer,uint32_t write_addr,uint16_t write_len)
{
    SPI_FLASH.W25QXX_Write(write_buffer,write_addr,write_len);
}
void SPI_FLASH_Read(uint8_t *read_buffer,uint32_t read_addr,uint16_t read_len)
{
    SPI_FLASH.W25QXX_Read(read_buffer,read_addr,read_len);
}

lv_obj_t *Remote_Screen;
lv_obj_t *Status_bar;//状态栏
lv_obj_t *Remote_interface;//遥控器界面栏
lv_group_t *Remote_interface_group;//遥控界面组 用于输入设备控制

FATFS fs;

pwm_hal buzzer_test;

void test(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const void* color);
st7789_drv display_device;//屏幕驱动
remote_pole_dev remote_pole;//遥杆驱动
button_drv LA,LB,RA,RB,LL,RR;
si24r1_master_drv RF_master;
battery_testing_drv battery_testing;
lv_obj_t *bar_batter;//电池电量显示
volatile uint8_t data_tremm[4096];
void Main(void *pvParameters)
{
    static uint8_t recive_data[100],test_data[31]={
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        '\n'
    };
    float temp_;
    gpio_hal GPIO_test1;
    GPIO_test1.gpio__init(GPIOB,GPIO_PIN_10,GPIO_MODE_OUT_PP);
    GPIO_test1.low();
    RF_master.si24r1_master_init();//si24r1射频初始化
    battery_testing.battery_testing_init();
    display_device.st7789_init();//初始化屏幕
    lvgl_display_Callback_Register(test);//显示注册
    //按键初始化
    LA.button_init(GPIOC,GPIO_PIN_3,button_drv::trigger_high);
    LB.button_init(GPIOC,GPIO_PIN_2,button_drv::trigger_high);
    //RA.button_init(GPIOA,GPIO_PIN_11,button_drv::trigger_high);
    RB.button_init(GPIOA,GPIO_PIN_8,button_drv::trigger_high);
    LL.button_init(GPIOB,GPIO_PIN_9,button_drv::trigger_high);
    //RR.button_init(GPIOA,GPIO_PIN_12,button_drv::trigger_high);

    SPI_FLASH.W25Q_init();

    lv_init();//LVGL初始化
    lv_port_disp_init();//LVGL显示初始化
    lv_port_indev_init();//LVGL输入初始化

    if(!f_mount(&fs,"0:",1))
    {
        lv_port_fs_init();//LVGL文件系统初始化
    }
    
    Remote_Screen = lv_obj_create(lv_scr_act());
    lv_obj_set_pos(Remote_Screen,0,0);
    lv_obj_set_size(Remote_Screen,280,240);
    lv_obj_clear_flag(Remote_Screen, LV_OBJ_FLAG_SCROLLABLE);//取消滚动条


    Status_bar = lv_obj_create(Remote_Screen);//创建状态栏
    lv_obj_set_size(Status_bar,220,20);
    lv_obj_set_pos(Status_bar,15,-15);//放在最顶上
    lv_obj_clear_flag(Status_bar, LV_OBJ_FLAG_SCROLLABLE);//取消滚动条

    bar_batter = lv_bar_create(Status_bar);//电量显示
    lv_obj_set_size(bar_batter,30,10);
    lv_obj_align(bar_batter,LV_ALIGN_CENTER,0,0);
    lv_bar_set_range(bar_batter,0,100); 
    lv_obj_set_pos(bar_batter,90,0);
    lv_bar_set_value(bar_batter,50,LV_ANIM_OFF);
    
    remote_pole.remote_pole_dev_init();//遥杆初始化
    remote_pole.Set_remote_pole_resolution(1000);//设置遥杆半分辨率
    
    
    buzzer_test.pwm_init(TIMER2,444,1000,timer_ch3);
    buzzer_test.set_channel4_duty(0);


    //遥杆任务创建
	// xTaskCreate((TaskFunction_t		)remote_pole_task,
	// 						(const char*			)"remote_pole_task",
	// 						(uint16_t					)remote_pole_task_STK_SIZE,
	// 						(void*						)NULL,
	// 						(UBaseType_t			)remote_pole_task_PRIO,
	// 						(TaskHandle_t*		)&remote_pole_task_Handler);
    //UI任务创建
	xTaskCreate((TaskFunction_t		)UI_task,
							(const char*			)"UI_task",
							(uint16_t					)UI_task_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)UI_task_PRIO,
							(TaskHandle_t*		)&UI_task_Handler);
    // //设置任务创建
	// xTaskCreate((TaskFunction_t		)Config_task,
	// 						(const char*			)"Config_task",
	// 						(uint16_t					)Config_task_STK_SIZE,
	// 						(void*						)NULL,
	// 						(UBaseType_t			)Config_task_PRIO,
	// 						(TaskHandle_t*		)&Config_task_Handler);

    int16_t TEMP_data;
    while(1)
    {
        if(LL.Get_Button_Long_Press())
        {
            GPIO_test1.high();
        }
        vTaskDelay(15);
        battery_testing.battery_testing_tick();


        test_data[0] = 0xAB;
        test_data[1] = 0xBA;
        test_data[10] = 0;
        test_data[11] = 0;
        TEMP_data =(int16_t)remote_pole.Get_left_remote_pole().Y;
        test_data[2] = (uint8_t)(TEMP_data);
        test_data[10]+=test_data[2];
        test_data[11]+=test_data[10];

        test_data[3] = (uint8_t)(TEMP_data>>8);
        test_data[10]+=test_data[3];
        test_data[11]+=test_data[10];

        TEMP_data = (int16_t)remote_pole.Get_left_remote_pole().X;
        test_data[4] = (uint8_t)(TEMP_data);
        test_data[10]+=test_data[4];
        test_data[11]+=test_data[10];

        test_data[5] = (uint8_t)(TEMP_data>>8);
        test_data[10]+=test_data[5];
        test_data[11]+=test_data[10];

        TEMP_data = (int16_t)remote_pole.Get_right_remote_pole().X;
        test_data[6] = (uint8_t)(TEMP_data);
        test_data[10]+=test_data[6];
        test_data[11]+=test_data[10];

        test_data[7] = (uint8_t)(TEMP_data>>8);
        test_data[10]+=test_data[7];
        test_data[11]+=test_data[10];

        TEMP_data = (int16_t)remote_pole.Get_right_remote_pole().Y;
        test_data[8] = (uint8_t)(TEMP_data);
        test_data[10]+=test_data[8];
        test_data[11]+=test_data[10];

        test_data[9] = (uint8_t)(TEMP_data>>8);
        test_data[10]+=test_data[9];
        test_data[11]+=test_data[10];


        RF_master.send_buffer.write_packet(test_data,12);//往发送缓冲区写入数据
        RF_master.si24r1_master_tick();
    }
 
}





/*
*@ 遥杆检测任务
*/
void Main_interface_updata(void);
uint8_t UI_task_flag = 4;
void Main_interface_init(lv_scr_load_anim_t anim_type,uint32_t time);
void Config_interface_init(lv_scr_load_anim_t anim_type);
void remote_pole_task(void *pvParameters)
{
    while(1)
    {    
        vTaskDelay(80);
    }
}
extern "C"
{
    void vApplicationTickHook()
    {
        // 告诉lvgl已经过去了1毫秒
        lv_tick_inc(1);
    }
}
remote_control_gui UI;
void UI_task(void *pvParameters)
{
    
    UI.gui_init(&remote_pole,&display_device,&RF_master,&battery_testing);
    while (1)
    {
        UI.gui_tick();
    }
    
}






/*
*@ 显示刷新重映射
*/
void test(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const void* color)
{
    display_device.Show(xsta,ysta,xend,yend,color);
}
/*
*@ 按键获取重映射
*/
uint32_t keypad_get_key(void)
{
    if(UI.bt_down)
        return 1;
    if(UI.bt_up)
        return 2;
    if(UI.bt_light)
        return 3;
    if(UI.bt_right)
        return 4;
    if(UI.Home_key.Get_Button_Click())
        return 5;
    return 0;
}

