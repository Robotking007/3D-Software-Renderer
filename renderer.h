#ifndef RENDERER_H
#define RENDERER_H
#define NATIVE_BUILD 1
#include "math_utils.h"
#include "mesh_loader.h"
#include "shaders.h"

#define WIDTH 800
#define HEIGHT 600
#define FOV (M_PI / 3.0)

typedef struct {
    Vec3 position;
    Vec3 rotation;
    float scale;
} Transform;

typedef struct {
    int mode;
    int shader;
} RenderState;

// Core functions
void init_renderer();
void render_frame(float delta_time);
unsigned char* get_framebuffer();

// Control functions
void set_render_mode(int mode);
void set_shader(int shader);

#endif // RENDERER_H