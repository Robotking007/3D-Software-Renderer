async function loadWASMRenderer() {
    const importObject = {
        env: {
            memory: new WebAssembly.Memory({ initial: 256, maximum: 2048 }),
            table: new WebAssembly.Table({ initial: 8, element: 'anyfunc' }),
            abort: (msg) => console.error(`WASM abort: ${msg}`),
            Math_random: () => Math.random(),
            sinf: Math.sin,
            cosf: Math.cos,
            tanf: Math.tan,
            sqrtf: Math.sqrt,
            expf: Math.exp,
            logf: Math.log,
            powf: Math.pow,
            fmodf: (x, y) => x % y
        },
        wasi_snapshot_preview1: {
            fd_write: () => {},
            proc_exit: () => {}
        }
    };

    try {
        // Fetch and instantiate WASM module
        const response = await fetch('build/renderer.wasm');
        if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
        
        const bytes = await response.arrayBuffer();
        const { instance } = await WebAssembly.instantiate(bytes, importObject);
        
        // Create wrapper for WASM exports
        const wrapper = {
            memory: instance.exports.memory,
            
            init: (width, height) => {
                instance.exports.init_renderer(width, height);
            },
            
            render_frame: (delta_time) => {
                instance.exports.render_frame(delta_time);
            },
            
            get_framebuffer: () => {
                return instance.exports.get_framebuffer();
            },
            
            load_cube: () => {
                instance.exports.load_cube();
            },
            
            set_render_mode: (mode) => {
                instance.exports.set_render_mode(mode);
            },
            
            set_shader: (shader) => {
                instance.exports.set_shader(shader);
            },
            
            // Add more exported functions as needed
        };
        
        return wrapper;
    } catch (error) {
        console.error('Failed to load WASM module:', error);
        throw error;
    }
}

// Export for Node.js testing (if needed)
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { loadWASMRenderer };
}