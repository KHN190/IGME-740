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

# Requirements

* 2 lighting sources
* switch between per-vertex & wireframe render
* move light sources with keyboard
* 2 shading for teapots: per-vertex + per-frag
