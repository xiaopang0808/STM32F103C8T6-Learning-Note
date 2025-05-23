#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetLeftSpeed(int8_t Speed);
void Motor_SetRightSpeed(int8_t Speed);
void MoveForward(void);
void MoveBackward(void);
void Turn_Left(void);
void Turn_Right(void);
void Car_Stop(void);
void Self_Right(void);
void Self_Left(void);
void Set_Car_Speed(int8_t speed);



#endif
