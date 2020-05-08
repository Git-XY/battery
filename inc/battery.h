#ifndef __BATTERY_H__
#define __BATTERY_H__

#include <rtthread.h>
#include <rtdevice.h>

#if defined(PKG_USING_BATTERY) ||defined(RT_DEBUG_BATTERY)

int battery_level(rt_uint8_t *current_battery);

#endif
#endif

