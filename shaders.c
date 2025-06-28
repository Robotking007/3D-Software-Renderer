#include "shaders.h"
#include <string.h>

// Framebuffer and depth buffer
unsigned char framebuffer[WIDTH * HEIGHT * 4];
float depth_buffer[WIDTH * HEIGHT];

void clear_buffers() {
    memset(framebuffer, 0, sizeof(framebuffer));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        depth_buffer[i] = 1.0f;
    }
}

void draw_pixel(int x, int y, float z, Color color) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    
    int idx = (y * WIDTH + x) * 4;
    if (z < depth_buffer[y * WIDTH + x]) {
        depth_buffer[y * WIDTH + x] = z;
        framebuffer[idx] = color.r;
        framebuffer[idx+1] = color.g;
        framebuffer[idx+2] = color.b;
        framebuffer[idx+3] = 255; // Alpha
    }
}

void draw_line(int x0, int y0, int x1, int y1, Color color) {
    // Bresenham's line algorithm
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    
    while (1) {
        draw_pixel(x0, y0, 0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_wireframe(Vec3 v0, Vec3 v1, Vec3 v2) {
    // Convert to screen coordinates
    v0.x = (v0.x + 1.0f) * 0.5f * WIDTH;
    v0.y = (1.0f - (v0.y + 1.0f) * 0.5f) * HEIGHT;
    v1.x = (v1.x + 1.0f) * 0.5f * WIDTH;
    v1.y = (1.0f - (v1.y + 1.0f) * 0.5f) * HEIGHT;
    v2.x = (v2.x + 1.0f) * 0.5f * WIDTH;
    v2.y = (1.0f - (v2.y + 1.0f) * 0.5f) * HEIGHT;

    draw_line((int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, (Color){0, 255, 0});
    draw_line((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, (Color){0, 255, 0});
    draw_line((int)v2.x, (int)v2.y, (int)v0.x, (int)v0.y, (Color){0, 255, 0});
}

Color shade_flat(Vec3 bary) {
    return (Color){255, 100, 100}; // Simple red tint
}

Color shade_normals(Vec3 bary) {
    // Gradient based on barycentric coordinates
    return (Color){
        (unsigned char)(bary.x * 255),
        (unsigned char)(bary.y * 255),
        (unsigned char)(bary.z * 255)
    };
}

Color shade_phong(Vec3 bary, Vec3 normal) {
    // Simple directional light
    Vec3 light_dir = {0, 0, -1};
    float intensity = fmaxf(0, vec3_dot(normal, light_dir));
    
    return (Color){
        (unsigned char)(intensity * 255),
        (unsigned char)(intensity * 200),
        (unsigned char)(intensity * 100)
    };
}