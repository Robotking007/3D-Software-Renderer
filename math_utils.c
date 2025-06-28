#include "math_utils.h"
#include <math.h>

Vec3 vec3_add(Vec3 a, Vec3 b) { 
    return (Vec3){a.x+b.x, a.y+b.y, a.z+b.z}; 
}

Vec3 vec3_sub(Vec3 a, Vec3 b) { 
    return (Vec3){a.x-b.x, a.y-b.y, a.z-b.z}; 
}

Vec3 vec3_mul(Vec3 v, float s) { 
    return (Vec3){v.x*s, v.y*s, v.z*s}; 
}

float vec3_dot(Vec3 a, Vec3 b) { 
    return a.x*b.x + a.y*b.y + a.z*b.z; 
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

Vec3 vec3_normalize(Vec3 v) {
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    return (Vec3){v.x/len, v.y/len, v.z/len};
}

Mat4 mat4_identity() {
    Mat4 m = {0};
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
    return m;
}

Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Mat4 m = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                m.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return m;
}

Vec3 mat4_mul_vec3(Mat4 m, Vec3 v) {
    Vec3 result;
    float w = m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3];
    result.x = (m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]) / w;
    result.y = (m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]) / w;
    result.z = (m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]) / w;
    return result;
}

Mat4 mat4_perspective(float fov, float aspect, float near, float far) {
    Mat4 m = {0};
    float tan_half_fov = tanf(fov / 2.0f);
    
    m.m[0][0] = 1.0f / (aspect * tan_half_fov);
    m.m[1][1] = 1.0f / tan_half_fov;
    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -1.0f;
    m.m[3][2] = -(2.0f * far * near) / (far - near);
    
    return m;
}

Mat4 mat4_look_at(Vec3 eye, Vec3 target, Vec3 up) {
    Vec3 z = vec3_normalize(vec3_sub(eye, target));
    Vec3 x = vec3_normalize(vec3_cross(up, z));
    Vec3 y = vec3_cross(z, x);
    
    Mat4 m = {{
        {x.x, x.y, x.z, -vec3_dot(x, eye)},
        {y.x, y.y, y.z, -vec3_dot(y, eye)},
        {z.x, z.y, z.z, -vec3_dot(z, eye)},
        {0,   0,   0,   1}
    }};
    
    return m;
}

Mat4 mat4_rotate_y(Mat4 m, float angle) {
    Mat4 rot = mat4_identity();
    rot.m[0][0] = cosf(angle);
    rot.m[0][2] = sinf(angle);
    rot.m[2][0] = -sinf(angle);
    rot.m[2][2] = cosf(angle);
    return mat4_mul(m, rot);
}

Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p) {
    Vec3 v0 = vec3_sub(b, a);
    Vec3 v1 = vec3_sub(c, a);
    Vec3 v2 = vec3_sub(p, a);
    
    float d00 = vec3_dot(v0, v0);
    float d01 = vec3_dot(v0, v1);
    float d11 = vec3_dot(v1, v1);
    float d20 = vec3_dot(v2, v0);
    float d21 = vec3_dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    
    Vec3 result;
    result.y = (d11 * d20 - d01 * d21) / denom;
    result.z = (d00 * d21 - d01 * d20) / denom;
    result.x = 1.0f - result.y - result.z;
    
    return result;
}