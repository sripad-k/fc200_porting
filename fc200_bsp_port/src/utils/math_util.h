#ifndef H_MATH_UTIL
#define H_MATH_UTIL

#define LD_PI_F 3.14159265358979f   // pi
#define LD_2PI_F 6.28318530717959f  // 2 x pi
#define LD_D2R_F 0.01745329251994f  // degrees to radians
#define LD_R2D_F 57.29577951308232f // radians to degrees

#define LD_PI 3.14159265358979   // pi
#define LD_2PI 6.28318530717959  // 2 x pi
#define LD_D2R 0.01745329251994  // degrees to radians
#define LD_R2D 57.29577951308232 // radians to degrees

/*
Wraps the input angle angle_rad in the range -pi to pi.
Tries for maximum 5 iterations.
float version.
*/
float wrap_pif(float angle_rad);

/*
Wraps the input angle angle_rad in the range -pi to pi.
Tries for maximum 5 iterations.
double version.
*/
double wrap_pi(double angle_rad);

#endif /* H_MATH_UTIL_IO */