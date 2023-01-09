#include "remote_pole_dev.h"

void remote_pole_dev::remote_pole_dev_init(void)
{
    this->X1.adc_init(ADC0,ADC_CHANNEL_11);
    this->Y1.adc_init(ADC0,ADC_CHANNEL_10);
    this->X2.adc_init(ADC0,ADC_CHANNEL_15);
    this->Y2.adc_init(ADC0,ADC_CHANNEL_14);
}

/*
*@ 设置遥杆分辨率
*@ resolution 半行程分辨率
*/
void remote_pole_dev::Set_remote_pole_resolution(float resolution)
{
    this->remote_pole_resolution = resolution;

}

/*
*@ 获取遥杆数据
*/
remote_pole_dev::remote_pole_data remote_pole_dev::Get_left_remote_pole(void)
{
    remote_pole_data data;
    if(X1.adc_get_channel_value() > this->left_remote_pole_midpoint.X)
        data.X = (X1.adc_get_channel_value() - this->left_remote_pole_midpoint.X) / (float)(this->left_remote_pole_resolution_max.X-this->left_remote_pole_midpoint.X) * this->remote_pole_resolution;
    else if(X1.adc_get_channel_value() <= this->left_remote_pole_midpoint.X)
        data.X = - (X1.adc_get_channel_value() - this->left_remote_pole_midpoint.X) / (float)(this->left_remote_pole_resolution_min.X-this->left_remote_pole_midpoint.X) * this->remote_pole_resolution;

    if(Y1.adc_get_channel_value() > this->left_remote_pole_midpoint.Y)
        data.Y = (Y1.adc_get_channel_value() - this->left_remote_pole_midpoint.Y) / (float)(this->left_remote_pole_resolution_max.Y-this->left_remote_pole_midpoint.Y) * this->remote_pole_resolution;
    else if(Y1.adc_get_channel_value() <= this->left_remote_pole_midpoint.Y)
        data.Y = - (Y1.adc_get_channel_value() - this->left_remote_pole_midpoint.Y) / (float)(this->left_remote_pole_resolution_min.Y-this->left_remote_pole_midpoint.Y) * this->remote_pole_resolution;

    return data;
}
remote_pole_dev::remote_pole_data remote_pole_dev::Get_right_remote_pole(void)
{
    remote_pole_data data;
    if(X2.adc_get_channel_value() > this->right_remote_pole_midpoint.X)
        data.X = (X2.adc_get_channel_value() - this->right_remote_pole_midpoint.X) / (float)(this->right_remote_pole_resolution_max.X-this->right_remote_pole_midpoint.X) * this->remote_pole_resolution;
    else if(X2.adc_get_channel_value() <= this->right_remote_pole_midpoint.X)
        data.X = - (X2.adc_get_channel_value() - this->right_remote_pole_midpoint.X) / (float)(this->right_remote_pole_resolution_min.X-this->right_remote_pole_midpoint.X) * this->remote_pole_resolution;

    if(Y2.adc_get_channel_value() > this->right_remote_pole_midpoint.Y)
        data.Y = (Y2.adc_get_channel_value() - this->right_remote_pole_midpoint.Y) / (float)(this->left_remote_pole_resolution_max.Y-this->right_remote_pole_midpoint.Y) * this->remote_pole_resolution;
    else if(Y2.adc_get_channel_value() <= this->right_remote_pole_midpoint.Y)
        data.Y = - (Y2.adc_get_channel_value() - this->right_remote_pole_midpoint.Y) / (float)(this->right_remote_pole_resolution_min.Y-this->right_remote_pole_midpoint.Y) * this->remote_pole_resolution;

    return data;
}


/*
*@ 遥杆中点校准
*/
void remote_pole_dev::remote_pole_midpoint_calibration(void)
{
    this->left_remote_pole_midpoint.X = 0;
    this->left_remote_pole_midpoint.Y = 0;
    this->right_remote_pole_midpoint.X = 0;
    this->right_remote_pole_midpoint.Y = 0;
    for (uint8_t i = 0; i < sampling_Len; i++)
    {
        this->left_remote_pole_midpoint.X += this->X1.adc_get_channel_value()/(float)sampling_Len;
        this->left_remote_pole_midpoint.Y += this->Y1.adc_get_channel_value()/(float)sampling_Len;
        this->right_remote_pole_midpoint.X += this->X2.adc_get_channel_value()/(float)sampling_Len;
        this->right_remote_pole_midpoint.Y += this->Y2.adc_get_channel_value()/(float)sampling_Len;
    }
    
    
}

/*
*@ 遥杆行程校准
*/
void remote_pole_dev::remote_pole_itinerary_calibration(void)
{
    remote_pole_data temp_data;
    if(Calibration_state == true)
    {
        Calibration_state = false;
        left_remote_pole_resolution_max.X = 0;
        left_remote_pole_resolution_max.Y = 0;
        left_remote_pole_resolution_min.X = 4096;
        left_remote_pole_resolution_min.Y = 4096;
        right_remote_pole_resolution_max.X = 0;
        right_remote_pole_resolution_max.Y = 0;
        right_remote_pole_resolution_min.X = 4096;
        right_remote_pole_resolution_min.Y = 4096;
        this->remote_pole_midpoint_calibration();
    }
        //左遥感校准数据采集
        temp_data.X = 0;
        temp_data.Y = 0;
        for (uint8_t i = 0; i < sampling_Len; i++)
        {
            temp_data.X += this->X1.adc_get_channel_value()/(float)sampling_Len;
            temp_data.Y += this->Y1.adc_get_channel_value()/(float)sampling_Len;
        }
        if(temp_data.X > this->left_remote_pole_resolution_max.X)
            left_remote_pole_resolution_max.X = temp_data.X;
        if(temp_data.Y > this->left_remote_pole_resolution_max.Y)
            left_remote_pole_resolution_max.Y = temp_data.Y;
        if(temp_data.X < this->left_remote_pole_resolution_min.X)
            left_remote_pole_resolution_min.X = temp_data.X;
        if(temp_data.Y < this->left_remote_pole_resolution_min.Y)
            left_remote_pole_resolution_min.Y = temp_data.Y;

        //右遥感校准数据采集
        temp_data.X = 0;
        temp_data.Y = 0;
        for (uint8_t i = 0; i < sampling_Len; i++)
        {
            temp_data.X += this->X2.adc_get_channel_value()/(float)sampling_Len;
            temp_data.Y += this->Y2.adc_get_channel_value()/(float)sampling_Len;
        }
        if(temp_data.X > this->right_remote_pole_resolution_max.X)
            right_remote_pole_resolution_max.X = temp_data.X;
        if(temp_data.Y > this->right_remote_pole_resolution_max.Y)
            right_remote_pole_resolution_max.Y = temp_data.Y;
        if(temp_data.X < this->right_remote_pole_resolution_min.X)
            right_remote_pole_resolution_min.X = temp_data.X;
        if(temp_data.Y < this->right_remote_pole_resolution_min.Y)
            right_remote_pole_resolution_min.Y = temp_data.Y;
}

void remote_pole_dev::remote_pole_set_Calibration_state(bool state)
{
    this->Calibration_state = state;
}





