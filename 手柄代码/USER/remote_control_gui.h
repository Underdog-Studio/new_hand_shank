#ifndef _remote_control_gui_H
#define _remote_control_gui_H

#include "lv_obj.h"
#include "lv_disp.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "FreeRTOS.h"
#include "task.h"

#include "button_drv.h"
#include "st7789_drv.h"
#include "remote_pole_dev.h"
#include "si24r1/si24r1_master_drv.h"
#include "System_time.h"
#include "battery_testing_drv.h"

class remote_control_gui
{


private:  
    remote_pole_dev *remote_pole;//摇杆
    


    lv_obj_t *Remote_Screen;//遥控屏幕

    lv_obj_t *Status_bar;//状态栏
    lv_obj_t *Remote_interface;//遥控器界面栏
    lv_obj_t *Remote_interface_load;//待加载界面
    
    lv_group_t *Remote_interface_group;//遥控器界面组

    lv_obj_t *bar_batter;//电池电量显示
    lv_obj_t *img_Connect_State;//连接状态

    //遥控器界面栏
    lv_obj_t *bitmeta_logo,*car_img;
    lv_obj_t *bar_LX,*bar_LY,*bar_RX,*bar_RY;//遥感显示条


    /* data */
public:
    typedef enum 
    {
      GUI_LOAD_ANIM_NONE,
      GUI_ANIM_MOVE_LEFT,
      GUI_ANIM_MOVE_RIGHT,
      GUI_ANIM_MOVE_TOP,
      GUI_ANIM_MOVE_BOTTOM,
      GUI_ANIM_FADE_ON,
    }gui_anim_t;
    typedef enum 
    {
      GUI_PATH_LINEAR,//线
      GUI_PATH_BOUNCE//回弹
    }gui_anim_path_t;
    typedef enum
    {
        None_interface,
        Main_interface,
        Config_interface,
        Calibration_interface,
        Light_interface,
        Frequency_interface,
        Auto_connect_interface,
    }gui_state;
    remote_control_gui()=default;
    gui_state Remote_interface_state = gui_state::None_interface;//界面状态机
    button_drv Home_key;//home按键
    st7789_drv *Screen_deive;//屏幕驱动
    si24r1_master_drv *RF_master;//射频驱动
    battery_testing_drv *battery_testing;//电池电量检测驱动
    bool bt_up,bt_down,bt_light,bt_right;
    void gui_init(remote_pole_dev *remote_pole,st7789_drv *Screen_deive,si24r1_master_drv *RF_master,battery_testing_drv *battery_testing);

    void button_mock_detect(void);

    void Main_interface_show(void);
    void Main_interface_updata(void);
    void Config_interface_show(void);
    void Config_btn_event(lv_event_t * e);

    void Calibration_interface_show(void);
    void Calibration_interface_updata(void);

    void Light_interface_show(void);
    uint8_t current_light=20;

    void Auto_Connect_interface_show(void);

    void Frequency_interface_show(void);
    void Frequency_mock_detect(void);
    Time Frequency_TX,Frequency_TY;
    void gui_tick(void); 
    void gui_interface_anim(gui_anim_t anim_type,gui_anim_path_t anim_path,uint32_t time);


};



#endif
