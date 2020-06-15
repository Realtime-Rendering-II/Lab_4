#ifndef LAB_4_MESH_H
#define LAB_4_MESH_H

#include <vector>
#include "vertex.h"
#include <GL/glew.h>

class Mesh{
public:
    Mesh(std::vector<Vertex> vertices);
    virtual void draw(GLuint shader);
    void add_texture(GLuint texture);
    glm::mat4 get_model_matrix();

    virtual void set_model_matrix(glm::mat4 new_model);
    std::vector<Vertex> get_vertices();
protected:
    GLuint VAO, VBO;
    glm::mat4 model_matrix;
    void setup_mesh();
    std::vector<Vertex> vertices;
    std::vector<GLuint> textures;
};
#endif //LAB_4_MESH_H
