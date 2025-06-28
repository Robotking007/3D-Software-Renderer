# 3D Software Renderer

![Demo Screenshot](screenshot.png)  
*A WebAssembly-powered 3D renderer written in C*

## Features

- 🖌️ Pure software rendering (no GPU required)
- ⚡ WebAssembly compilation for browser deployment
- 🧊 Supports multiple 3D models (Cube, Sphere, Monkey)
- 🎨 Multiple rendering modes:
  - Wireframe
  - Solid shading
  - Texture mapping (WIP)
- 💡 Lighting models:
  - Flat shading
  - Normal visualization
  - Phong shading (specular highlights)

## Prerequisites

- Windows/Linux/macOS
- [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
- Python 3.x (for local server)
- Modern browser (Chrome/Firefox/Edge)

## Quick Start

```bash
# Clone the repository
git clone https://github.com/your-username/3d-software-renderer.git
cd 3d-software-renderer

# Install dependencies (one-time setup)
./setup.sh

# Build the project
./build.sh

# Launch development server
python -m http.server 8000