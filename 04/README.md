# Compile

```bash
mkdir build && cd build && cmake .. && make
```

# Run

```bash
# in build
ln -s ../shaders .
ln -s ../Mesh .
# actual run
./run
```

# Use

See [shaders](./shaders) for per vertex & per fragment implementation. Press `s` to select between lights. The selected light will be rendered with wireframe, though it's not very obvious. I used script to move the lights, which also changes the lighting, and kept the demonstration that they can be selected using keyboard.

Lighting colors, number of lights and objects are hardcoded.

# Requirements

* 2 lighting sources
* switch between per-vertex & wireframe render
* move light sources with keyboard
* 2 shading for teapots: per-vertex + per-frag
