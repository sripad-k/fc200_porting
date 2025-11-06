#include "math_util.h"

float wrap_pif(float angle_rad)
{
    float angle_wrap = angle_rad;

    for (int i = 0; i < 5; i++)
    {
        if (angle_wrap > LD_PI)
        {
            angle_wrap = angle_wrap - LD_2PI;
        }
        else if (angle_wrap < -LD_PI)
        {
            angle_wrap = angle_wrap + LD_2PI;
        }
        else
        {
            break;
        }
    }

    return angle_wrap;
}

double wrap_pi(double angle_rad)
{
    double angle_wrap = angle_rad;

    for (int i = 0; i < 5; i++)
    {
        if (angle_wrap > LD_PI)
        {
            angle_wrap = angle_wrap - LD_2PI;
        }
        else if (angle_wrap < -LD_PI)
        {
            angle_wrap = angle_wrap + LD_2PI;
        }
        else
        {
            break;
        }
    }

    return angle_wrap;
}