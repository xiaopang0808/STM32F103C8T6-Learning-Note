#ifndef __ULTRASOUND_H
#define __ULTRASOUND_H

#define MIN_DISTANCE  30    // 触发避障的最小距离(cm)

void UltraSound_Init(void);
float UltraSound_Measure(void);
void ObstacleAvoidance(void);

#endif

