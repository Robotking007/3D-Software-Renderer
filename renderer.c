#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>
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

// Global state
static unsigned char framebuffer[WIDTH * HEIGHT * 4];
static float depth_buffer[WIDTH * HEIGHT];
static Mesh current_mesh;
static Transform camera;
static Mat4 projection_matrix;
static RenderState render_state = {
    .mode = RENDER_MODE_SOLID,
    .shader = SHADER_FLAT
};

EMSCRIPTEN_KEEPALIVE
void init_renderer() {
    camera = (Transform){
        .position = {0, 0, -5},
        .rotation = {0, 0, 0},
        .scale = 1.0f
    };
    projection_matrix = mat4_perspective(FOV, (float)WIDTH/HEIGHT, 0.1f, 100.0f);
    memset(framebuffer, 0, sizeof(framebuffer));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        depth_buffer[i] = 1.0f;
    }
    load_cube(&current_mesh);
}

EMSCRIPTEN_KEEPALIVE
unsigned char* get_framebuffer() {
    return framebuffer;
}

EMSCRIPTEN_KEEPALIVE
void set_render_mode(int mode) {
    render_state.mode = mode;
}

EMSCRIPTEN_KEEPALIVE
void set_shader(int shader) {
    render_state.shader = shader;
}

void render_triangle(Vec3 v0, Vec3 v1, Vec3 v2) {
    // Convert to screen coordinates
    v0.x = (v0.x + 1.0f) * 0.5f * WIDTH;
    v0.y = (1.0f - (v0.y + 1.0f) * 0.5f) * HEIGHT;
    v1.x = (v1.x + 1.0f) * 0.5f * WIDTH;
    v1.y = (1.0f - (v1.y + 1.0f) * 0.5f) * HEIGHT;
    v2.x = (v2.x + 1.0f) * 0.5f * WIDTH;
    v2.y = (1.0f - (v2.y + 1.0f) * 0.5f) * HEIGHT;

    // Bounding box
    int min_x = fmax(0, floorf(fminf(v0.x, fminf(v1.x, v2.x))));
    int max_x = fmin(WIDTH-1, ceilf(fmaxf(v0.x, fmaxf(v1.x, v2.x))));
    int min_y = fmax(0, floorf(fminf(v0.y, fminf(v1.y, v2.y))));
    int max_y = fmin(HEIGHT-1, ceilf(fmaxf(v0.y, fmaxf(v1.y, v2.y))));

    // Rasterize
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            Vec3 p = {x, y, 0};
            Vec3 bary = barycentric(v0, v1, v2, p);
            
            if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
                float z = v0.z * bary.x + v1.z * bary.y + v2.z * bary.z;
                if (z < depth_buffer[y * WIDTH + x]) {
                    depth_buffer[y * WIDTH + x] = z;
                    
                    Color color = {255, 0, 0};
                    if (render_state.shader == SHADER_FLAT) {
                        color = shade_flat(bary);
                    } else if (render_state.shader == SHADER_NORMALS) {
                        color = shade_normals(bary);
                    }
                    
                    draw_pixel(x, y, z, color);
                }
            }
        }
    }
}

EMSCRIPTEN_KEEPALIVE
void render_frame(float delta_time) {
    // Clear buffers
    memset(framebuffer, 0, sizeof(framebuffer));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        depth_buffer[i] = 1.0f;
    }

    // Update rotation
    static float rotation = 0.0f;
    rotation += delta_time * 0.5f;
    
    // Calculate matrices
    Mat4 view = mat4_look_at(
        camera.position,
        (Vec3){0, 0, 0},
        (Vec3){0, 1, 0}
    );
    
    Mat4 model = mat4_identity();
    model = mat4_rotate_y(model, rotation);
    
    Mat4 mvp = mat4_mul(mat4_mul(projection_matrix, view), model);
    
    // Render all triangles
    for (int i = 0; i < current_mesh.index_count; i += 3) {
        Vec3 v0 = current_mesh.vertices[current_mesh.indices[i]];
        Vec3 v1 = current_mesh.vertices[current_mesh.indices[i+1]];
        Vec3 v2 = current_mesh.vertices[current_mesh.indices[i+2]];
        
        v0 = mat4_mul_vec3(mvp, v0);
        v1 = mat4_mul_vec3(mvp, v1);
        v2 = mat4_mul_vec3(mvp, v2);
        
        if (render_state.mode == RENDER_MODE_WIREFRAME) {
            draw_wireframe(v0, v1, v2);
        } else {
            render_triangle(v0, v1, v2);
        }
    }
}