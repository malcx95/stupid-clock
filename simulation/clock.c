#include <math.h>
#include "clock.h"

float law_of_cosines(float a, float b, float c) {
    return acos((a*a + b*b - c*c) / (2*a*b));
}

float distance(float x, float y) {
    return sqrt(x*x + y*y);
}

struct IkRes clock_ik(float x, float y) {
    float dist = distance(x, y);
    float d1 = atan2(y, x);
    float d2 = law_of_cosines(dist, JOINT_1_LENGTH, JOINT_2_LENGTH);

    struct IkRes res;
    res.angle1 = d1 + d2 + JOINT_1_OFFSET;
    res.angle2 = law_of_cosines(JOINT_1_LENGTH, JOINT_2_LENGTH, dist) - M_PI + JOINT_2_OFFSET;
    return res;
}

struct vec2 number_position(int i) {
    if (i % 2 == 1) {
        i += 6;
    }
    float i_angle = -(float)(i - 3) / 6.0f * M_PI;

    return (struct vec2) {
        CLOCK_RADIUS * cos(i_angle),
        CLOCK_RADIUS * sin(i_angle)
    };
}

struct vec2 hand_target(float time) {
    int i_time = (int)time;
    float f_time = time - i_time;

    struct vec2 pos1 = number_position(i_time);
    struct vec2 pos2 = number_position(i_time + 1);

    return (struct vec2) {
        pos2.x * f_time + pos1.x * (1.0f - f_time),
        pos2.y * f_time + pos1.y * (1.0f - f_time)
    };
}
