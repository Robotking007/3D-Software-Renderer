class RendererApp {
    constructor() {
        this.canvas = document.getElementById('renderCanvas');
        this.ctx = this.canvas.getContext('2d');
        this.fpsElement = document.getElementById('fpsCounter');
        this.triCountElement = document.getElementById('triCount');
        this.consoleLog = document.getElementById('consoleLog');
        
        this.renderer = null;
        this.isRendering = false;
        this.lastTime = 0;
        this.fps = 0;
        this.frameCount = 0;
        this.lastFpsUpdate = 0;
        
        this.init();
    }
    
    log(message) {
        const entry = document.createElement('div');
        entry.className = 'log-entry';
        entry.textContent = `> ${message}`;
        this.consoleLog.appendChild(entry);
        this.consoleLog.scrollTop = this.consoleLog.scrollHeight;
    }
    
    async init() {
        this.log("Initializing application...");
        this.resizeCanvas();
        window.addEventListener('resize', this.resizeCanvas.bind(this));
        
        this.setupControls();
        
        try {
            this.renderer = await loadWASMRenderer();
            this.log("WASM renderer loaded successfully");
            
            this.renderer.init(this.canvas.width, this.canvas.height);
            this.renderer.load_cube();
            this.triCountElement.textContent = "12";
            
            this.startRendering();
        } catch (error) {
            this.log(`Error loading WASM: ${error.message}`);
            console.error(error);
        }
    }
    
    resizeCanvas() {
        const container = document.querySelector('.render-view');
        this.canvas.width = container.clientWidth;
        this.canvas.height = container.clientHeight;
        this.log(`Canvas resized to ${this.canvas.width}x${this.canvas.height}`);
        
        if (this.renderer) {
            this.renderer.init(this.canvas.width, this.canvas.height);
        }
    }
    
    setupControls() {
        // Model selection
        document.getElementById('modelSelect').addEventListener('change', (e) => {
            const model = e.target.value;
            this.log(`Loading model: ${model}`);
            
            switch(model) {
                case 'cube':
                    this.renderer.load_cube();
                    this.triCountElement.textContent = "12";
                    break;
                case 'sphere':
                    // Implement sphere loading
                    this.triCountElement.textContent = "960";
                    break;
                case 'monkey':
                    // Implement monkey loading
                    this.triCountElement.textContent = "500";
                    break;
            }
        });
        
        // Render mode buttons
        document.querySelectorAll('.button-group button').forEach(btn => {
            if (btn.id.includes('Btn')) {
                btn.addEventListener('click', () => {
                    document.querySelectorAll('.button-group button').forEach(b => {
                        b.classList.remove('active');
                    });
                    btn.classList.add('active');
                    
                    if (btn.id === 'wireframeBtn') {
                        this.renderer.set_render_mode(0);
                        this.log("Render mode: WIREFRAME");
                    } else if (btn.id === 'solidBtn') {
                        this.renderer.set_render_mode(1);
                        this.log("Render mode: SOLID");
                    } else if (btn.id === 'texturedBtn') {
                        this.renderer.set_render_mode(2);
                        this.log("Render mode: TEXTURED");
                    } else if (btn.id === 'flatBtn') {
                        this.renderer.set_shader(0);
                        this.log("Shader: FLAT");
                    } else if (btn.id === 'normalsBtn') {
                        this.renderer.set_shader(1);
                        this.log("Shader: NORMALS");
                    } else if (btn.id === 'phongBtn') {
                        this.renderer.set_shader(2);
                        this.log("Shader: PHONG");
                    } else if (btn.id === 'rotateLeftBtn') {
                        this.log("Camera: Rotating left");
                    } else if (btn.id === 'rotateRightBtn') {
                        this.log("Camera: Rotating right");
                    } else if (btn.id === 'resetBtn') {
                        this.log("Camera: View reset");
                    }
                });
            }
        });
    }
    
    startRendering() {
        if (this.isRendering) return;
        this.isRendering = true;
        this.log("Starting render loop...");
        
        const renderFrame = (time) => {
            if (!this.lastTime) this.lastTime = time;
            const deltaTime = (time - this.lastTime) / 1000;
            this.lastTime = time;
            
            // Update FPS counter
            this.frameCount++;
            if (time - this.lastFpsUpdate >= 1000) {
                this.fps = Math.round((this.frameCount * 1000) / (time - this.lastFpsUpdate));
                this.fpsElement.textContent = this.fps;
                this.frameCount = 0;
                this.lastFpsUpdate = time;
            }
            
            // Render frame
            if (this.renderer) {
                this.renderer.render_frame(deltaTime);
                
                // Get the rendered frame from WASM memory
                const buffer = new Uint8ClampedArray(
                    this.renderer.memory.buffer,
                    this.renderer.get_framebuffer(),
                    this.canvas.width * this.canvas.height * 4
                );
                
                // Draw to canvas
                const imageData = new ImageData(buffer, this.canvas.width, this.canvas.height);
                this.ctx.putImageData(imageData, 0, 0);
            }
            
            requestAnimationFrame(renderFrame);
        };
        
        requestAnimationFrame(renderFrame);
    }
}

// Start the application when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    new RendererApp();
});