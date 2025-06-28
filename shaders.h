#ifndef SHADERS_H
#define SHADERS_H

#include "math_utils.h"
#include <stdlib.h>

// Configuration
#ifndef WIDTH
#define WIDTH 800
#endif

#ifndef HEIGHT
#define HEIGHT 600
#endif

// Color type (only defined here)
typedef struct {
    unsigned char r, g, b;
} Color;

// Render modes
typedef enum {
    RENDER_MODE_WIREFRAME,
    RENDER_MODE_SOLID,
    RENDER_MODE_TEXTURED
} RenderMode;

// Shader types
typedef enum {
    SHADER_FLAT,
    SHADER_NORMALS,
    SHADER_PHONG
} ShaderType;

// Function declarations
void draw_pixel(int x, int y, float z, Color color);
void draw_line(int x0, int y0, int x1, int y1, Color color);
void draw_wireframe(Vec3 v0, Vec3 v1, Vec3 v2);

// Shading functions
Color shade_flat(Vec3 bary);
Color shade_normals(Vec3 bary);
Color shade_phong(Vec3 bary, Vec3 normal);

// Buffer management
void clear_buffers();

#endif // SHADERS_H