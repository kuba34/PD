#include "PD.hpp"

int PD(float error, float Kp, float Kd)
{
    float d = (blad - pop_blad) * Kd;
		float p = Kp * blad;
    return p + d;
}
