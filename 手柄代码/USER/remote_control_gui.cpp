#include "remote_control_gui.h"




extern lv_indev_t *indev_keypad;
void remote_control_gui::gui_init(remote_pole_dev *remote_pole,st7789_drv *Screen_deive,si24r1_master_drv *RF_master,battery_testing_drv *battery_testing)
{
    lv_init();//LVGL初始化
    lv_port_disp_init();//LVGL显示初始化
    lv_port_indev_init();//LVGL输入初始化

    this->Home_key.button_init(GPIOB,GPIO_PIN_11,button_drv::trigger_high);

    this->remote_pole = remote_pole;
    this->Screen_deive = Screen_deive;
    this->RF_master = RF_master;
    this->battery_testing = battery_testing;

    this->Remote_Screen = lv_obj_create(lv_scr_act());
    lv_obj_set_pos(this->Remote_Screen,0,0);
    lv_obj_set_size(this->Remote_Screen,280,240);
    lv_obj_clear_flag(this->Remote_Screen, LV_OBJ_FLAG_SCROLLABLE);


    Status_bar = lv_obj_create(this->Remote_Screen);//创建状态栏
    lv_obj_set_size(Status_bar,220,20);
    lv_obj_set_pos(Status_bar,15,-15);
    lv_obj_clear_flag(Status_bar, LV_OBJ_FLAG_SCROLLABLE);

    bar_batter = lv_bar_create(Status_bar);//电量显示
    lv_obj_set_size(bar_batter,30,10);
    lv_obj_align(bar_batter,LV_ALIGN_CENTER,0,0);
    lv_bar_set_range(bar_batter,0,100); 
    lv_obj_set_pos(bar_batter,90,0);
    lv_bar_set_value(bar_batter,50,LV_ANIM_OFF);


    img_Connect_State = lv_label_create(Status_bar);
    lv_obj_align(img_Connect_State,LV_ALIGN_CENTER,0,0);
    lv_label_set_text(img_Connect_State,LV_SYMBOL_GPS);
    lv_label_set_recolor(img_Connect_State,true);
    
    

    

    Remote_interface_group = lv_group_create();
    this->Remote_interface = lv_obj_create(Remote_Screen);
    lv_obj_set_size(this->Remote_interface,280,220);
    lv_obj_set_pos(this->Remote_interface,-15,3);
    this->Main_interface_show();
    this->gui_interface_anim(gui_anim_t::GUI_ANIM_FADE_ON,gui_anim_path_t::GUI_PATH_BOUNCE,2000);

}

/*
*@ GUI调度
*/
uint8_t get_chnanel_load(void);
void remote_control_gui::gui_tick(void)
{
    //UI状态栏数据跟新
    if(RF_master->si24r1_connect_state)
    {
        lv_label_set_text_fmt(img_Connect_State,"#00FF00 %s#",LV_SYMBOL_GPS);
    }else
    {
        lv_label_set_text_fmt(img_Connect_State,"#FF0000 %s#",LV_SYMBOL_GPS);
    }
    lv_bar_set_value(bar_batter,this->battery_testing->get_batter_level(),LV_ANIM_OFF);
    

    //UI界面栏数据跟新
    switch (Remote_interface_state)
    {
    case gui_state::Main_interface :
        this->Main_interface_updata();
        if(this->Home_key.Get_Button_Double_Click())
        {
            this->Config_interface_show();
            this->gui_interface_anim(gui_anim_t::GUI_ANIM_MOVE_TOP,gui_anim_path_t::GUI_PATH_BOUNCE,700);
        }
        break;
    case gui_state::Config_interface :
        this->button_mock_detect();
        break;
    case gui_state::Calibration_interface:
        Calibration_interface_updata();
        this->remote_pole->remote_pole_itinerary_calibration();
        if(this->Home_key.Get_Button_Double_Click())
        {
            this->remote_pole->remote_pole_set_Calibration_state(true);
            this->Config_interface_show();
            this->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_RIGHT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
        }
        break;
    case gui_state::Light_interface:
        this->button_mock_detect();
        if(this->Home_key.Get_Button_Double_Click())
        {
            this->Config_interface_show();
            this->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_RIGHT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
        }
        break;
    case gui_state::Frequency_interface:
        this->Frequency_mock_detect();
        this->button_mock_detect();
        if(this->Home_key.Get_Button_Double_Click())
        {
            if(this->RF_master->si24r1_toggle_channel(get_chnanel_load()))
            {
                this->Config_interface_show();
                this->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_RIGHT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
            }
            
        }
        break;
    case gui_state::Auto_connect_interface:
        if(this->RF_master->si24r1_auto_matching())
        {
            this->Config_interface_show();
            this->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_RIGHT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
        }
        break;
    case gui_state::None_interface:
        this->button_mock_detect();
    default:
        break;
    }
    vTaskDelay(20);
    lv_task_handler();
}

/*
*@ 显示主界面
*/
void remote_control_gui::Main_interface_show(void)
{
    Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);

    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_move_foreground(this->Status_bar);
    lv_obj_clear_flag(Remote_interface_load, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    bitmeta_logo = lv_img_create(Remote_interface_load);//logo
    lv_img_set_src(bitmeta_logo,"0:/image/bitmeta.bin");

    lv_obj_set_pos(bitmeta_logo,0,-10);


    car_img = lv_img_create(Remote_interface_load);//logo  
    lv_obj_set_size(car_img,128,128);
    lv_obj_set_pos(car_img,61,70);
    lv_img_set_src(car_img,"0:/image/car.bin");

    bar_LX = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_LX,100,10);
    lv_slider_set_mode(bar_LX,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_LX,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_LX,-1000,1000); 
    lv_obj_set_pos(bar_LX,-60,95);
    lv_slider_set_value(bar_LX,0,LV_ANIM_OFF);


    bar_LY = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_LY,10,100);
    lv_slider_set_mode(bar_LY,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_LY,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_LY,-1000,1000); 
    lv_obj_set_pos(bar_LY,-125,30);
    lv_slider_set_value(bar_LY,0,LV_ANIM_OFF);


    bar_RX = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_RX,100,10);
    lv_slider_set_mode(bar_RX,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_RX,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_RX,-1000,1000); 
    lv_obj_set_pos(bar_RX,60,95);
    lv_slider_set_value(bar_RX,0,LV_ANIM_OFF);


    bar_RY = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_RY,10,100);
    lv_slider_set_mode(bar_RY,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_RY,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_RY,-1000,1000); 
    lv_obj_set_pos(bar_RY,125,30);
    lv_slider_set_value(bar_RY,0,LV_ANIM_OFF);

    Remote_interface_state = gui_state::Main_interface;
}

/*
*@ 主界面数据跟新
*/
void remote_control_gui::Main_interface_updata(void)
{
    //遥感数据跟新
    if(lv_obj_is_valid(bar_LX))
        lv_slider_set_value(bar_LX,-remote_pole->Get_left_remote_pole().X,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_LY))
        lv_slider_set_value(bar_LY,-remote_pole->Get_left_remote_pole().Y,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_RX))
        lv_slider_set_value(bar_RX,-remote_pole->Get_right_remote_pole().X,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_RY))
        lv_slider_set_value(bar_RY,-remote_pole->Get_right_remote_pole().Y,LV_ANIM_OFF);
}


/*
*@ 显示设置界面
*/
void Auto_Connect_btn_event(lv_event_t * e);
void Frequency_btn_event(lv_event_t * e);
void Calibration_btn_event(lv_event_t * e);
void Light_btn_event(lv_event_t * e);
void Return_btn_event(lv_event_t * e);
static uint8_t Config_user_data[10]={1,2,3,4,5,6,7,8,9,10};
void remote_control_gui::Config_interface_show(void)
{
    Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);

    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_move_foreground(this->Status_bar);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    lv_obj_t * list1 = lv_list_create(Remote_interface_load);
    lv_obj_set_size(list1, 280, 224);
    lv_obj_align(list1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * list_btn;

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_SHUFFLE, "Auto Connect");
    lv_obj_add_event_cb(list_btn,Auto_Connect_btn_event,LV_EVENT_CLICKED,this);
    lv_group_add_obj(Remote_interface_group,list_btn);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_UPLOAD, "Frequency");
    lv_obj_add_event_cb(list_btn,Frequency_btn_event,LV_EVENT_CLICKED,this);
    lv_group_add_obj(Remote_interface_group,list_btn);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_REFRESH, "Calibration");
    lv_obj_add_event_cb(list_btn,Calibration_btn_event,LV_EVENT_CLICKED,this);
    lv_group_add_obj(Remote_interface_group,list_btn);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_EYE_OPEN, "Light");
    lv_obj_add_event_cb(list_btn,Light_btn_event,LV_EVENT_CLICKED,this);
    lv_group_add_obj(Remote_interface_group,list_btn);

    list_btn = lv_list_add_btn(list1, LV_SYMBOL_NEW_LINE, "Return");
    lv_obj_add_event_cb(list_btn,Return_btn_event,LV_EVENT_CLICKED,this);
    lv_group_add_obj(Remote_interface_group,list_btn);
    lv_indev_set_group(indev_keypad,Remote_interface_group);

    Remote_interface_state = gui_state::Config_interface;
}

/*
*@ 自动连接(对频)按钮事件
*/
void Auto_Connect_btn_event(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->Auto_Connect_interface_show();
    ((remote_control_gui*)(e->user_data))->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_LEFT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
}
/*
*@ 频道切换按钮事件
*/
void Frequency_btn_event(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->Frequency_interface_show();
    ((remote_control_gui*)(e->user_data))->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_LEFT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
}
/*
*@ 校准按钮事件
*/
void Calibration_btn_event(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->Calibration_interface_show();
    ((remote_control_gui*)(e->user_data))->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_LEFT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
}
/*
*@ 亮度调节按钮事件
*/
void Light_btn_event(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->Light_interface_show();
    ((remote_control_gui*)(e->user_data))->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_LEFT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
}
/*
*@ 返回按钮事件
*/
void Return_btn_event(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->Main_interface_show();
    ((remote_control_gui*)(e->user_data))->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_BOTTOM,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);
}


/*
*@ 校准界面显示
*/
void remote_control_gui::Calibration_interface_show(void)
{
    Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);

    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_move_foreground(this->Status_bar);
    lv_obj_clear_flag(Remote_interface_load, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    bar_LX = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_LX,100,10);
    lv_slider_set_mode(bar_LX,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_LX,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_LX,-1000,1000); 
    lv_obj_set_pos(bar_LX,-60,0);
    lv_slider_set_value(bar_LX,0,LV_ANIM_OFF);

    bar_LY = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_LY,10,100);
    lv_slider_set_mode(bar_LY,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_LY,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_LY,-1000,1000); 
    lv_obj_set_pos(bar_LY,-60,0);
    lv_slider_set_value(bar_LY,0,LV_ANIM_OFF);

    bar_RX = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_RX,100,10);
    lv_slider_set_mode(bar_RX,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_RX,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_RX,-1000,1000); 
    lv_obj_set_pos(bar_RX,60,0);
    lv_slider_set_value(bar_RX,0,LV_ANIM_OFF);

    bar_RY = lv_slider_create(Remote_interface_load);
    lv_obj_set_size(bar_RY,10,100);
    lv_slider_set_mode(bar_RY,LV_SLIDER_MODE_SYMMETRICAL);
    lv_obj_align(bar_RY,LV_ALIGN_CENTER,0,0);
    lv_slider_set_range(bar_RY,-1000,1000); 
    lv_obj_set_pos(bar_RY,60,0);
    lv_slider_set_value(bar_RY,0,LV_ANIM_OFF);
    Remote_interface_state = gui_state::Calibration_interface;
}
/*
*@ 校准界面数据跟新
*/
void remote_control_gui::Calibration_interface_updata(void)
{
    //遥感数据跟新
    if(lv_obj_is_valid(bar_LX))
        lv_slider_set_value(bar_LX,-remote_pole->Get_left_remote_pole().X,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_LY))
        lv_slider_set_value(bar_LY,-remote_pole->Get_left_remote_pole().Y,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_RX))
        lv_slider_set_value(bar_RX,-remote_pole->Get_right_remote_pole().X,LV_ANIM_OFF);
    if(lv_obj_is_valid(bar_RY))
        lv_slider_set_value(bar_RY,-remote_pole->Get_right_remote_pole().Y,LV_ANIM_OFF);
}


/*
（@ 亮度调节界面
*/
static void Light_event_cb(lv_event_t * e);
lv_obj_t *light_text,*light_slider;
void remote_control_gui::Light_interface_show(void)
{
    this->Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);
    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_clear_flag(Remote_interface_load, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    light_slider = lv_slider_create(Remote_interface_load);
    lv_slider_set_range(light_slider,1,100);
    lv_obj_set_size(light_slider,200,20);
    lv_obj_align(light_slider,LV_ALIGN_CENTER,0,0);
    lv_slider_set_value(light_slider,(uint32_t)this->current_light,LV_ANIM_OFF);
    light_text = lv_label_create(light_slider);
    lv_obj_align(light_text,LV_ALIGN_CENTER,0,0);
    lv_label_set_recolor(light_text,true);
    lv_label_set_text_fmt(light_text,"#000000 %d",this->current_light);
    lv_obj_add_event_cb(light_slider,Light_event_cb,LV_EVENT_VALUE_CHANGED,this);
    this->Screen_deive->Set_light(this->current_light);
    lv_group_add_obj(Remote_interface_group,light_slider);

    lv_indev_set_group(indev_keypad,Remote_interface_group);
    this->Remote_interface_state = gui_state::Light_interface;
}

static void Light_event_cb(lv_event_t * e)
{
    ((remote_control_gui*)(e->user_data))->current_light = (uint8_t)lv_slider_get_value(light_slider);
    lv_label_set_text_fmt(light_text,"#000000 %d",((remote_control_gui*)(e->user_data))->current_light);
    ((remote_control_gui*)(e->user_data))->Screen_deive->Set_light((int8_t)((remote_control_gui*)(e->user_data))->current_light);
}


/*
*@ 频道切换界面显示
*/
lv_obj_t *chnanel_spinbox;
void remote_control_gui::Frequency_interface_show(void)
{
    this->Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);
    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_clear_flag(Remote_interface_load, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    chnanel_spinbox = lv_spinbox_create(Remote_interface_load);
    lv_obj_set_width(chnanel_spinbox,50);
    lv_obj_align(chnanel_spinbox,LV_ALIGN_CENTER,0,0);
    lv_spinbox_set_value(chnanel_spinbox,this->RF_master->channel);

    lv_spinbox_set_range(chnanel_spinbox,0,125);
    lv_spinbox_set_digit_format(chnanel_spinbox, 3, 3);

    lv_group_add_obj(Remote_interface_group,chnanel_spinbox);
    lv_indev_set_group(indev_keypad,Remote_interface_group);
    this->Remote_interface_state = gui_state::Frequency_interface;
}
/*
*@ 频道切换 摇杆模拟按钮
*/
void remote_control_gui::Frequency_mock_detect(void)
{
    remote_pole_dev::remote_pole_data temp;
    temp = this->remote_pole->Get_right_remote_pole();
    if(temp.Y > 300 && Get_deta_T(Frequency_TX) > 0.2f)
    {
        Frequency_TX = Get_System_T();
        lv_spinbox_decrement(chnanel_spinbox);
    }

    if(temp.Y < -300 && Get_deta_T(Frequency_TY) > 0.2f)
    {
        Frequency_TY = Get_System_T();
        lv_spinbox_increment(chnanel_spinbox);
    }
}
uint8_t get_chnanel_load(void)
{
    return (uint8_t)lv_spinbox_get_value(chnanel_spinbox);
}

/*
*@ 自动对频界面显示
*/
 lv_obj_t* mbox1;
 static void Auto_Connect_msgbox_callback(lv_event_t * event);
void remote_control_gui::Auto_Connect_interface_show(void)
{
    this->Remote_interface_state = gui_state::None_interface;
    indev_keypad->group = NULL;
    lv_group_remove_all_objs(Remote_interface_group);
    Remote_interface_load = lv_obj_create(Remote_Screen);
    lv_obj_clear_flag(Remote_interface_load, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(Remote_interface_load,280,220);
    lv_obj_set_pos(Remote_interface_load,-15,3);

    lv_obj_t *Auto_Connect_spinner = lv_spinner_create(Remote_interface_load,1000,80);
    lv_obj_set_size(Auto_Connect_spinner,80,80);
    lv_obj_align(Auto_Connect_spinner,LV_ALIGN_CENTER,0,20);
    
    lv_obj_t *Auto_Connect_label = lv_label_create(Remote_interface_load);
    lv_obj_align(Auto_Connect_label,LV_ALIGN_CENTER,0,-50);
    lv_label_set_text_fmt(Auto_Connect_label,"Scanning devices.");

    static const char* btn_str[] = { "OK", "Cancel", "" };
    mbox1 = lv_msgbox_create(Remote_interface_load,NULL,"#5FB878 Set the receiver to auto-linking and make sure#",btn_str,false);
    
    lv_obj_add_event_cb(lv_msgbox_get_btns(mbox1),Auto_Connect_msgbox_callback,LV_EVENT_CLICKED,this);
    lv_obj_set_width(mbox1,200);
    lv_obj_align(mbox1, LV_ALIGN_CENTER, 0, 0);

    lv_group_add_obj(Remote_interface_group,lv_msgbox_get_btns(mbox1));
    lv_indev_set_group(indev_keypad,Remote_interface_group);
    
}

static void Auto_Connect_msgbox_callback(lv_event_t * event)
{
    char btn_ok[]={"OK"},btn_close[]={"Cancel"};
    lv_obj_t * obj = lv_event_get_target(event);
    uint16_t btn_id = lv_btnmatrix_get_selected_btn(obj);
    const char * txt = lv_btnmatrix_get_btn_text(obj, btn_id);
    if(strcmp(btn_ok,txt)==0)
    {
        lv_obj_del(mbox1);
        ((remote_control_gui*)event->user_data)->Remote_interface_state = remote_control_gui::gui_state::Auto_connect_interface;
    }else if (strcmp(btn_close,txt)==0)
    {
        ((remote_control_gui*)event->user_data)->Config_interface_show();
        ((remote_control_gui*)event->user_data)->gui_interface_anim(remote_control_gui::gui_anim_t::GUI_ANIM_MOVE_RIGHT,remote_control_gui::gui_anim_path_t::GUI_PATH_BOUNCE,700);//向右滑出

    }
    
}


/*
*@ GUI界面切换动画
*/
static void GUI_set_X_anim(void * obj, int32_t v)
{
    lv_obj_set_x((lv_obj_t*)obj,v);
}
static void GUI_set_Y_anim(void * obj, int32_t v)
{
    lv_obj_set_y((lv_obj_t*)obj,v);
}
static void GUI_scale_anim(void * obj, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t*)obj, v, 0);
}

static void gui_interface_anim_ready(lv_anim_t * a)
{
    lv_obj_del((lv_obj_t*)(a->var));
    
}
void remote_control_gui::gui_interface_anim(gui_anim_t anim_type,gui_anim_path_t anim_path,uint32_t time)
{
    lv_anim_t gui_last_anim,gui_now_anim;
    lv_anim_del(&gui_last_anim, NULL);
    lv_anim_del(&gui_now_anim, NULL);

    lv_anim_init(&gui_last_anim);
    lv_anim_init(&gui_now_anim);

    gui_last_anim.var = this->Remote_interface;
    gui_last_anim.time = time;
    gui_last_anim.act_time = 0;
    
    gui_last_anim.ready_cb = gui_interface_anim_ready;


    gui_now_anim.var = this->Remote_interface_load;
    gui_now_anim.time = time;
    gui_now_anim.act_time = 0;
    


    switch (anim_path)
    {
    case gui_anim_path_t::GUI_PATH_LINEAR :
        gui_last_anim.path_cb = lv_anim_path_linear;
        gui_now_anim.path_cb = lv_anim_path_linear;
        break;
    case gui_anim_path_t::GUI_PATH_BOUNCE :
        gui_last_anim.path_cb = lv_anim_path_bounce;
        gui_now_anim.path_cb = lv_anim_path_bounce;
        break;
    default:
        break;
    }


    switch (anim_type)
    {
    case gui_anim_t::GUI_ANIM_MOVE_BOTTOM :
        gui_last_anim.exec_cb = GUI_set_Y_anim;
        gui_last_anim.start_value = 3;
        gui_last_anim.current_value = 223;
        gui_last_anim.end_value = 223;

        gui_now_anim.exec_cb = GUI_set_Y_anim;
        gui_now_anim.start_value = -223;
        gui_now_anim.current_value = 3; 
        gui_now_anim.end_value = 3;
        break;
    case gui_anim_t::GUI_ANIM_MOVE_TOP :
        gui_last_anim.exec_cb = GUI_set_Y_anim;
        gui_last_anim.start_value = 3;
        gui_last_anim.current_value = -223; 
        gui_last_anim.end_value = -223;

        gui_now_anim.exec_cb = GUI_set_Y_anim;
        gui_now_anim.start_value = 223;
        gui_now_anim.current_value = 3; 
        gui_now_anim.end_value = 3;
        break;
    case gui_anim_t::GUI_ANIM_MOVE_LEFT :
        gui_last_anim.exec_cb = GUI_set_X_anim;
        gui_last_anim.start_value = -15;
        gui_last_anim.current_value = -295; 
        gui_last_anim.end_value = -295;

        gui_now_anim.exec_cb = GUI_set_X_anim;
        gui_now_anim.start_value = 265;
        gui_now_anim.current_value = -15;
        gui_now_anim.end_value = -15;
        break;
    case gui_anim_t::GUI_ANIM_MOVE_RIGHT :
        gui_last_anim.exec_cb = GUI_set_X_anim;
        gui_last_anim.start_value = -15;
        gui_last_anim.current_value = 265; 
        gui_last_anim.end_value = 265;

        gui_now_anim.exec_cb = GUI_set_X_anim;
        gui_now_anim.start_value = -295;
        gui_now_anim.current_value = -15; 
        gui_now_anim.end_value = -15;
        break;
    case gui_anim_t::GUI_ANIM_FADE_ON :
        gui_now_anim.exec_cb = GUI_scale_anim;
        gui_now_anim.start_value = 0;
        gui_now_anim.current_value = 0;
        gui_now_anim.end_value = 255;
        break;
    default:
        break;
    }
    
    //lv_scr_load_anim
    this->Remote_interface = this->Remote_interface_load;
    lv_anim_start(&gui_last_anim);
    lv_anim_start(&gui_now_anim);
}




void remote_control_gui::button_mock_detect(void)
{
    if(this->remote_pole->Get_right_remote_pole().X<-300)
    {
        bt_right = true;
    }else
    {
        bt_right = false;
    }
    if(this->remote_pole->Get_right_remote_pole().X>300)
    {
        bt_light = true;
    }else
    {
        bt_light = false;
    }
    if(this->remote_pole->Get_right_remote_pole().Y<-300)
    {
        bt_up = true;
    }else
    {
        bt_up = false;
    }
    if(this->remote_pole->Get_right_remote_pole().Y>300)
    {
        bt_down = true;
    }else
    {
        bt_down = false;
    }
}


