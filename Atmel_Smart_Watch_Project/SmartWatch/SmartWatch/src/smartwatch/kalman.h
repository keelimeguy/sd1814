#ifndef KALMAN_H
#define KALMAN_H

#define Q00 25
#define Q11 400
#define Q22 0.0036
#define Q33 2500
#define Q44 4e-006
#define Q55 4e-006

void kalman_CGM(float z, float R, unsigned char sensorNum, float x_result[6]);
void kalman_setT(float val);

#endif