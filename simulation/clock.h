static const int JOINT_1_LENGTH = 200;
static const int JOINT_2_LENGTH = 150;
static const int JOINT_1_OFFSET = 0;
static const int JOINT_2_OFFSET = 0;
static const int CLOCK_RADIUS = 300;

struct IkRes {
    float angle1;
    float angle2;
};

struct vec2 {
    float x;
    float y;
};

struct IkRes clock_ik(float x, float y);
struct vec2 number_position(int i);
struct vec2 hand_target(float time);

