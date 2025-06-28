#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>

typedef struct { float x, y, z; } Vec3;
typedef struct { float m[4][4]; } Mat4;

// Vector operations
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 v, float s);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
Vec3 vec3_normalize(Vec3 v);

// Matrix operations
Mat4 mat4_identity();
Mat4 mat4_mul(Mat4 a, Mat4 b);
Vec3 mat4_mul_vec3(Mat4 m, Vec3 v);
Mat4 mat4_perspective(float fov, float aspect, float near, float far);
Mat4 mat4_look_at(Vec3 eye, Vec3 target, Vec3 up);
Mat4 mat4_rotate_y(Mat4 m, float angle);

// Utility functions
Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p);

#endif // MATH_UTILS_H