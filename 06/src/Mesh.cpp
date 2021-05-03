#include "Mesh.h"
#include "Sphere.h"

Mesh::Mesh() {
  vert_num = tri_num = 0;
  vao = vbo = nbo = ibo = 0;
  modelMat = mat4(1.0f);
}

Mesh::~Mesh() {
  delete[] vertices;
  delete[] triangles;
  delete[] fnormals;
  delete[] vnormals;
}

void Mesh::computeNormals() {
  fnormals = new vec3[tri_num];
  vnormals = new vec3[vert_num];

  vec3 a, b, n;

  // Compute face normals
  for (unsigned int i = 0; i < tri_num; i++) {

    a = vertices[triangles[i][1]] - vertices[triangles[i][0]];
    b = vertices[triangles[i][2]] - vertices[triangles[i][0]];

    n = cross(a, b);
    fnormals[i] = normalize(n);
  }

  // Compute vertex normals
  for (unsigned int i = 0; i < vert_num; i++) {
    vnormals[i] = vec3(0.0f);
  }

  for (unsigned int i = 0; i < tri_num; i++) {
    for (unsigned int j = 0; j < 3; j++) {
      vnormals[triangles[i][j]] += fnormals[i];
    }
  }

  for (unsigned int i = 0; i < vert_num; i++) {
    vnormals[i] = normalize(vnormals[i]);
  }
}

void Mesh::prepareVBOandShaders(const char *v_shader_file,
                                const char *f_shader_file) {
  vShader.create(v_shader_file, GL_VERTEX_SHADER);
  fShader.create(f_shader_file, GL_FRAGMENT_SHADER);
  shaderProg.create();
  shaderProg.link(vShader);
  shaderProg.link(fShader);

  vShader.destroy();
  fShader.destroy();

  // create vbo
  // generate a new VBO and get the associated ID
  glGenBuffers(1, &vbo);

  // bind VBO in order to use
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // upload data to VBO - data went to GPU
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num, vertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
  // delete[] vertices; // commented out, since it's handled by the destructor

  // repeat for normals and indices

  glGenBuffers(1, &nbo);
  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num, vnormals,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
  // delete[] vnormals; // commented out, since it's handled by the destructor

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * tri_num, triangles,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // clean up
  // delete[] triangles; // commented out, since it's handled by the destructor

  // so far, we transferred the position and index data to GPU. There will be no
  // data transfer calls at drawing time

  // Define VAO
  // There are 2 buffer objects (vertices and normals), which should be input
  // "attribute" variables to the vertex shader. We set up the layout of both of
  // these with vertex array object. The actual vertices and normal vectors have
  // already been kept on the GPU memory by VBOs. The purpose of VAO is to have
  // vertices and normal vectors as separate attributes in the vertex shader. So
  // VAO's attrobites point to these data on the GPU, rather than referring back
  // to any CPU data.

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                        NULL); // 0 - the layout id in vertex shader
  glBindBuffer(GL_ARRAY_BUFFER, nbo);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
                        NULL); // 1 - the layout id in fragment shader

  // Ettributes are disabled by default in OpenGL 4.
  // We have to explicitly enable each one.
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void add_triangle(vector<uvec3> &triangles, uint a, uint b, uint c)
{
  triangles.emplace_back(uvec3(a, b, c));
}

void add_quad(vector<uvec3> &triangles, uint a, uint b, uint c, uint d)
{
  triangles.emplace_back(uvec3(a, b, c));
  triangles.emplace_back(uvec3(a, c, d));
}

void Mesh::create_sphere( Sphere mesh,
                          const char *v_shader_file,
                          const char *f_shader_file) {
  cout << "creating sphere mesh\n";
  vector<vec3> ori_vertices;
  vector<uvec3> ori_triangles;

  pos = mesh.pos;
  size = vec3(mesh.radius, mesh.radius, mesh.radius);
  color = mesh.color;

  // todo
  // pos, radius, color
  // pos, size, color using shader translate
  // todo store and use in draw()

  int n_stacks = 10;
  int n_slices = 10;
  // add top vertex
  uint v0 = 0;
  ori_vertices.emplace_back(vec3(0, 1, 0));
  // generate vertices per stack / slice
  for (int i = 0; i < n_stacks - 1; i++)
  {
    float phi = M_PI * float(i + 1) / float(n_stacks);
    for (int j = 0; j < n_slices; j++)
    {
      float theta = 2.0f * M_PI * float(j) / float(n_slices);
      float x = std::sin(phi) * std::cos(theta);
      float y = std::cos(phi);
      float z = std::sin(phi) * std::sin(theta);
      ori_vertices.emplace_back(vec3(x, y, z));
    }
  }
  // add bottom vertex
  uint v1 = ori_vertices.size() - 1;
  ori_vertices.emplace_back(vec3(0, -1, 0));
  // add top / bottom triangles
  for (int i = 0; i < n_slices; ++i)
  {
    uint i0 = i + 1;
    uint i1 = (i + 1) % n_slices + 1;
    add_triangle(ori_triangles, v0, i1, i0);
    i0 = i + n_slices * (n_stacks - 2) + 1;
    i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
    add_triangle(ori_triangles, v1, i0, i1);
  }
  // add quads per stack / slice
  for (int j = 0; j < n_stacks - 2; j++)
  {
    uint j0 = j * n_slices + 1;
    uint j1 = (j + 1) * n_slices + 1;
    for (int i = 0; i < n_slices; i++)
    {
      uint i0 = j0 + i;
      uint i1 = j0 + (i + 1) % n_slices;
      uint i2 = j1 + (i + 1) % n_slices;
      uint i3 = j1 + i;
      add_quad(ori_triangles, i0, i1, i2, i3);
    }
  }

  // convert to final primitive
  this->vert_num = ori_vertices.size();
  this->tri_num = ori_triangles.size();
  this->vertices = new vec3[vert_num];
  this->triangles = new uvec3[tri_num];

  cout << "  vert_num: " << this->vert_num << '\n';
  cout << "  tri_num:  " << this->tri_num  << '\n';

  // Use arrays to store vertices and triangles, instead of using c++ vectors.
  // This is because we have to use arrays when sending data to GPUs.
  for (uint i = 0; i < vert_num; i++) {
    this->vertices[i] = ori_vertices[i];
  }
  for (uint i = 0; i < tri_num; i++) {
    this->triangles[i] = ori_triangles[i];
  }

  computeNormals();
  cout << "  normals created.\n";
  prepareVBOandShaders(v_shader_file, f_shader_file);
  cout << "  shaders created.\n";
}

void Mesh::draw(mat4 viewMat, mat4 projMat, vec3 lightPos, vec3 viewPos, bool wireframe=false) {

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  if (vert_num <= 0 && tri_num <= 0) {
    cout << "!! vert & tri empty" << '\n';
    return;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT, GL_FILL);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glUseProgram(shaderProg.id);
  mat4 m = translate(mat4(1.0), pos);
  modelMat = scale(m, size);
  shaderProg.setMatrix4fv("model", 1, value_ptr(modelMat));
  shaderProg.setMatrix4fv("view", 1, value_ptr(viewMat));
  shaderProg.setMatrix4fv("projection", 1, value_ptr(projMat));
  shaderProg.setFloat3V("objectColor", 1, value_ptr(color));
  shaderProg.setFloat3V("lightColor", 1, value_ptr(vec3(1, 1, 1)));
  shaderProg.setFloat3V("lightPos", 1, value_ptr(lightPos));
  // shaderProg.setFloat3V("viewPos", 1, value_ptr(viewPos));

  // cout << glm::to_string(modelMat) << endl;
  // cout << glm::to_string(viewMat) << endl;
  // cout << glm::to_string(projMat) << endl;

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_TRIANGLES, tri_num * 3, GL_UNSIGNED_INT, NULL);

  glPopMatrix();
  glDisable(GL_POLYGON_OFFSET_FILL);

  glPopMatrix();
}
