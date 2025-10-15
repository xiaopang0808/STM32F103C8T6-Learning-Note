/******************************************************************************
* 文件名：General_Timer_IC.h
* 作　者：Xiaopang
* 创建日期：2025-08-21
* 最后修改：2025-08-21
* 版　本：v1.0
* 功能描述：

*
* 修改记录：
*  版本   日期       修改人     修改内容
*  v1.0  2025-08-19  Xiaopang   初始版本创建
*
* 注意事项：
******************************************************************************/
#ifndef __GENERAL_TIMER_IC_H
#define __GENERAL_TIMER_IC_H


#include "sys.h"

extern uint8_t g_timxchy_cap_sta;
extern uint16_t g_timxchy_cap_val;


/******************** 外部接口函数声明 ********************/

void gtim_timx_cap_chy_ic_init(uint16_t psc,uint16_t arr);



#endif /* __GENERAL_TIMER_IC_H */
