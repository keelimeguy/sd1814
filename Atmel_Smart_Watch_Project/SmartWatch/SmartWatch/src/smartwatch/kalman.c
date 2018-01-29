/* UConn Senior Design Team 1814, January 2018
*/

#include "kalman.h"

static float P_result[36] = {225, 1, 0, 0, 0, 0, 1, 225, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 0, 0, 0.01};
static float T = 0.08333;

static void setPointer(float* pointer, char sel, uint8_t sensorNum, float x_result[6]) {
    float F2 [36] = {exp(-1*x_result[2]*T), (1-exp(-1*x_result[2]*T)), ((x_result[1]-x_result[0])/x_result[2])*(1-exp(-1*x_result[2]*T)), (-1+x_result[2]*T+exp(-1*x_result[2]*T))/x_result[2], 0, 0, 0, 1, 0, T, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
    float H2 [6] = {x_result[4], 0, 0, 0, x_result[0], 1};
    float Hc [6] = {0, 1, 0, 0, 0, 0};
	float *temp_point;

    switch(sel) {
        case 'F':
            temp_point = &F2[0];
            memcpy(pointer,temp_point,144);
            break;

        case 'H':
            if(sensorNum == 1)
                temp_point = &H2[0];
            else
                temp_point = &Hc[0];

            memcpy(pointer,temp_point,24);
            break;

        case 'x':
            temp_point = &x_result[0];
            memcpy(pointer,temp_point,24);
            break;
    }
}

void kalman_setT(float val) {
    T = val;
}

void kalman_CGM(float z, float R, unsigned char sensorNum, float x_result[6]) {
    //  Kalman Filter for the following system:
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //          plant equation:      x(k) = F(k-1)*x(k-1) + v(k-1)                 %
    //          measurement equation: z(k) = H(k)*x(k)     + w(k)                  %
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    uint8_t i0;
    uint8_t i1;
    uint8_t k;
    float *F = (float *) malloc(144);
    float *xk_1k_1 = (float *) malloc(24);
    setPointer(F,'F',sensorNum, x_result);
    setPointer(xk_1k_1,'x',sensorNum, x_result);

     for (i0 = 0; i0 < 6; i0++) {
        x_result[i0] = 0.0F;
        for (i1 = 0; i1 < 6; i1++) {
            x_result[i0] += F[i0 * 6 + i1] * xk_1k_1[i1];
        }
    }

    free(xk_1k_1);
    float *b_F = (float *) malloc(144);

    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            b_F[i0 * 6 + i1] = 0.0F;
            for (k = 0; k < 6; k++) {
                b_F[i0 * 6 + i1] += F[i0 * 6 + k] *  P_result[k+6*i1];
            }
        }
    }

    float *y = (float *) malloc(4);
    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            *y = 0.0F;
            for (k = 0; k < 6; k++) {
                *y += b_F[i0 * 6 + k] * F[i1 * 6 + k];
            }
            P_result[i0*6+i1] = *y;
        }
    }
    free(F);
    free(b_F);

    // Try manually adding Q diagonals
    P_result[0] += Q00;
    P_result[7] += Q11;
    P_result[14] += Q22;
    P_result[21] += Q33;
    P_result[28] += Q44;
    P_result[35] += Q55;

    float *nu = (float *)malloc(4);
    if (sensorNum == 1) {
        *nu = z - (x_result[0] * (x_result[4]) + x_result[5]);
    } else {
        *nu = z - x_result[1];
    }

    *y = 0.0F;
    float *b_H = (float *) malloc(24);
    float *H = (float *) malloc(24);
    setPointer(H, 'H', sensorNum, x_result);
    for (i0 = 0; i0 < 6; i0++) {
        b_H[i0] = 0.0F;
        for (i1 = 0; i1 < 6; i1++) {
            b_H[i0] += H[i1] * P_result[i1+6*i0];
        }

        *y += b_H[i0] * H[i0];
    }
    free(b_H);

    *y += R;
    float *W = (float *)malloc(24);
    float *temp = (float *)malloc(4);
    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
         *temp += P_result[i0 + 6 * i1] * H[i1];
         }

        W[i0] = *temp / *y;
        *temp = 0.0F;
        x_result[i0] += W[i0] * *nu;


    }
    free(temp);
    free(y);
    free(nu);

    char *I= (char *)malloc(36);
    for (i0 = 0; i0 < 36; i0++) {
        I[i0] = 0;
    }

    for (k = 0; k < 6; k++) {
        I[k + 6 * k] = 1;
    }

    float *b_FF = (float *) malloc(144);
    for (k = 0; k < 6; k++) {

        for (i1 = 0; i1 < 6; i1++) {
            b_FF[i1*6+k] = I[i1*6+k] - W[k]*H[i1];
        }
    }

    free(I);
    float *c_I = (float *)malloc(144);

    for (k = 0; k < 6; k++) {
        for (i0 = 0; i0 < 6; i0++) {
            c_I[k + 6 * i0] = 0.0F;
            for (i1 = 0; i1 < 6; i1++) {
                c_I[k + 6 * i0] += b_FF[k + 6 * i1] * P_result[i1+6*i0];
            }
        }
    }

    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            if ((i1+6*i0)%7 == 0)
                b_FF[i0 + 6 * i1] = 1 - W[i1] * H[i0];
            else
                b_FF[i0 + 6 * i1] = 0 - W[i1] * H[i0];
        }
    }
    free(H);

    float *d_I = (float *)malloc(144);
    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            d_I[i0 + 6 * i1] = 0.0F;
            for (k = 0; k < 6; k++) {
                d_I[i0 + 6 * i1] += c_I[i0 + 6 * k] * b_FF[k + 6 * i1];
            }
        }
    }
    free(c_I);

    float *b_W = (float *)malloc(144);
    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            b_W[i0 + 6 * i1] = W[i0] * (R) * W[i1];
        }
    }

    free(W);

    for (k = 0; k < 6; k++) {
        for (i1 = 0; i1 < 6; i1++) {
            d_I[(k*6)+i1] += b_W[(k*6)+i1];
        }
    }
    free(b_W);

    for (i0 = 0; i0 < 6; i0++) {
        for (i1 = 0; i1 < 6; i1++) {
            P_result[i1+6*i0] = (d_I[i1 + 6 * i0] + d_I[i0 + 6 * i1])/ 2.0F;
        }
    }
    free(d_I);
    free(b_FF);
}
