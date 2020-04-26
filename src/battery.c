#include "battery.h"

/*
 * �����嵥�� ADC �豸ʹ������
 * ���̵����� adc_sample ��������ն�
 * ������ø�ʽ��adc_sample
 * �����ܣ�ͨ�� ADC �豸������ѹֵ��ת��Ϊ��ֵ��
 *           ʾ������ο���ѹΪ3.3V,ת��λ��Ϊ12λ��
*/

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>
#define ADC_BATTERY
#define ADC_DEV_NAME        "adc1"      /* ADC �豸���� */
#define ADC_DEV_CHANNEL      1          /* ADC ͨ�� */
#define REFER_VOLTAGE        3300      /* �ο���ѹ 3.3V,���ݾ��ȳ���1000����3λС��*/
#define CONVERT_BITS        (1 << 12)   /* ת��λ��Ϊ12λ */
#define ADC_HW_PRE_SCALING_COMPENSATION 1
#define ADC_PRE_SCALING_COMPENSATION    1
static const float VoltageTable[] =
        {
                4142.2,4011.2,3912.9,3831.0,3774.9,3734.4,3688.4,3605.3
        };
static const uint8_t PercentageTable[] =
        {
                100,86,72,58,43,29,15,1
        };

uint8_t cal_percentage(uint16_t volatage)
{
    uint8_t length = 8;
    uint8_t i      = 0;

    //find the first value which is < volatage
    for(i = 0; i< length ; ++i) {

        if(volatage >= VoltageTable[i]) {
            break;
        }
    }

    if( i>= length) 
        return 0;
    if(i == 0) 
        return 100;
    return (volatage - VoltageTable[i])/((VoltageTable[i-1] - VoltageTable[i])/(PercentageTable[i-1] - PercentageTable[i])) + PercentageTable[i];

}

static int adc_sample(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;
	
    /* �����豸 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);

    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }
    /* ʹ���豸 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);

    /* ��ȡ����ֵ */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
    rt_kprintf("the value is :%d \n", value);

    /* ת��Ϊ��Ӧ��ѹֵ */
    vol = value * (REFER_VOLTAGE * ADC_HW_PRE_SCALING_COMPENSATION*ADC_PRE_SCALING_COMPENSATION)/ CONVERT_BITS;
	printf("vol is %d\r\n",vol);
	printf("num is %d\r\n",(REFER_VOLTAGE * ADC_HW_PRE_SCALING_COMPENSATION*ADC_PRE_SCALING_COMPENSATION));
	printf("num1 is %d\r\n",(REFER_VOLTAGE * ADC_HW_PRE_SCALING_COMPENSATION*ADC_PRE_SCALING_COMPENSATION)/ CONVERT_BITS);
    rt_kprintf("the voltage is :%d.%02d \n", vol / 1000, vol % 1000);

	rt_kprintf("the cal_percentage is %d%\r\n",cal_percentage(vol));
    /* �ر�ͨ�� */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

    return ret;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(adc_sample, adc voltage convert sample);
