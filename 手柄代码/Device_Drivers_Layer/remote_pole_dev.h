#ifndef remote_pole_dev_H
#define remote_pole_dev_H

#include "adc_hal.h"
#include "gpio_hal.h"
#include "button_drv.h"

#define sampling_Len 50

class remote_pole_dev
{

private:
    float remote_pole_max,remote_pole_min,remote_pole_resolution;
    bool Calibration_state=true;
    adc_hal X1,Y1,X2,Y2;

public:
    typedef struct
    {
        float X;
        float Y;
    }remote_pole_data;
    remote_pole_dev()=default;


    void remote_pole_dev_init(void);    

    //设置遥杆分辨率
    void Set_remote_pole_resolution(float resolution);
    //遥杆中点校准
    void remote_pole_midpoint_calibration(void);
    //遥杆行程校准
    void remote_pole_itinerary_calibration(void);
    void remote_pole_set_Calibration_state(bool state);
    //获取遥感数据
    remote_pole_data Get_left_remote_pole(void);
    remote_pole_data Get_right_remote_pole(void);

    //左遥杆行程
    remote_pole_data left_remote_pole_resolution_max={.X=3300,.Y=3300};
    remote_pole_data left_remote_pole_resolution_min={.X=600,.Y=600};
    //右遥杆行程
    remote_pole_data right_remote_pole_resolution_max={.X=3300,.Y=3300};
    remote_pole_data right_remote_pole_resolution_min={.X=600,.Y=600};
    remote_pole_data left_remote_pole_midpoint={.X=2200,.Y=2200};
    remote_pole_data right_remote_pole_midpoint={.X=2200,.Y=2200};
};




#endif