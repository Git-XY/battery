#include "battery.h"
#if defined(PKG_USING_BATTERY) ||defined(RT_DEBUG_BATTERY)

#define ADC_DEV_NAME        "adc1"        /* ADC 设备名称 */
#define ADC_DEV_CHANNEL      1            /* ADC 通道 */
#define REFER_VOLTAGE        3300         /* 参考电压 3.3V,数据精度乘以1000保留3位小数*/
#define CONVERT_BITS        (1 << 12)     /* 转换位数为12位 */

#define ADC_HW_PRE_SCALING_COMPENSATION 2 
#define ADC_PRE_SCALING_COMPENSATION    1

static const float VoltageTable[] = {
  4142.2,4011.2,3912.9,3831.0,3774.9,3734.4,3688.4,3605.3};
static const uint8_t PercentageTable[] ={
  100,86,72,58,43,29,15,1};

static uint8_t cal_percentage(uint16_t volatage)
{
	uint8_t i;
    for(i = 0; i< 8 ; ++i) {
        if(volatage >= VoltageTable[i]) {
            break;
        }
    }
    if(i >= 8) 
        return 0;
    if(i == 0) 
        return 100;
    return (volatage - VoltageTable[i])/((VoltageTable[i-1] - VoltageTable[i])/(PercentageTable[i-1] - PercentageTable[i])) + PercentageTable[i];
}

int battery_level(rt_uint8_t *current_battery)
{
    rt_adc_device_t adc_dev;
    rt_uint32_t adc_average = 0, vol;
    rt_err_t ret = RT_EOK;
	
    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);

    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }
    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* 读取8次ADC采样值 */
	for(rt_uint8_t i = 0; i < 8; i++)
	{
		 adc_average += rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
	}
	 /* 算术平均化 */
	adc_average >>= 3;
		
    /* 转换为对应电压值 */
    vol = adc_average * (REFER_VOLTAGE * ADC_HW_PRE_SCALING_COMPENSATION*ADC_PRE_SCALING_COMPENSATION)/ CONVERT_BITS;
    *current_battery = cal_percentage(vol);
//  rt_kprintf("the voltage is :%d.%02d \n", vol / 1000, vol % 1000);
//	rt_kprintf("the cal_percentage is %d%\r\n",cal_percentage(vol));
	
    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}

//MSH_CMD_EXPORT(battery_level, adc voltage convert sample);

#endif
