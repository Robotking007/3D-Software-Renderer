#!/bin/bash

# 3D Renderer Build Script
set -e

BUILD_DIR="build"
SRC_DIR="src"
OUTPUT="renderer"

# Create build directory
mkdir -p $BUILD_DIR

# Build WebAssembly version
echo "Building WebAssembly renderer..."
emcc \
    $SRC_DIR/math_utils.c \
    $SRC_DIR/shaders.c \
    $SRC_DIR/mesh_loader.c \
    $SRC_DIR/renderer.c \
    -I$SRC_DIR \
    -O3 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s EXPORTED_FUNCTIONS='["_init_renderer","_render_frame","_get_framebuffer","_set_render_mode","_set_shader"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
    -D__EMSCRIPTEN__ \
    -o $BUILD_DIR/$OUTPUT.js

# Copy assets
echo "Copying assets..."
cp -r assets $BUILD_DIR/

echo -e "\n\033[32mBuild successful!\033[0m"
echo "Files generated in $BUILD_DIR/:"
ls -lh $BUILD_DIR/$OUTPUT.*