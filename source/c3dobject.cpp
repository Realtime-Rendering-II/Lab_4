#include "../include/c3dobject.hpp"
#include "../include/obj_loader.h"
#include "../include/texture.h"

C3DObject::C3DObject(const std::string &obj_path, const std::string &texture_path)
        : Mesh(ObjLoader::load_obj(obj_path)) {

    compute_bounding_box();
    Texture *texture = new Texture{"../models_and_textures/textures/earthmap1k.jpg"};
    add_texture(texture->get_texture());
}


void C3DObject::set_model_matrix(glm::mat4 model_matrix) {
    Mesh::set_model_matrix(model_matrix);
    bounding_box->set_model_matrix(model_matrix);
}

void C3DObject::compute_bounding_box() {
    float object_max_x = -1e10f, object_max_y = -1e10f, object_max_z = -1e10f;
    float object_min_x = +1e10f, object_min_y = +1e10f, object_min_z = +1e10f;

    for(auto vertex : vertices){
        object_max_x = std::fmax(object_max_x, vertex.position.x);
        object_max_y = std::fmax(object_max_y, vertex.position.y);
        object_max_z = std::fmax(object_max_z, vertex.position.z);

        object_min_x = std::fmin(object_min_x, vertex.position.x);
        object_min_y = std::fmin(object_min_y, vertex.position.y);
        object_min_z = std::fmin(object_min_z, vertex.position.z);
    }

    std::vector<Vertex> vertices;

    vertices.push_back(Vertex{glm::vec3{object_max_x, object_max_y, object_min_z}});
    vertices.push_back(Vertex{glm::vec3{object_max_x, object_min_y, object_min_z}});
    vertices.push_back(Vertex{glm::vec3{object_max_x, object_max_y, object_max_z}});
    vertices.push_back(Vertex{glm::vec3{object_max_x, object_min_y, object_max_z}});
    vertices.push_back(Vertex{glm::vec3{object_min_x, object_max_y, object_min_z}});
    vertices.push_back(Vertex{glm::vec3{object_min_x, object_min_y, object_min_z}});
    vertices.push_back(Vertex{glm::vec3{object_min_x, object_max_y, object_max_z}});
    vertices.push_back(Vertex{glm::vec3{object_min_x, object_min_y, object_max_z}});

    bounding_box_vertices = vertices;

    std::vector<Vertex> bounding_box_vertices{
        vertices[4], vertices[2], vertices[0],
        vertices[2], vertices[7], vertices[3],
        vertices[6], vertices[5], vertices[7],
        vertices[1], vertices[7], vertices[5],
        vertices[0], vertices[3], vertices[1],
        vertices[4], vertices[1], vertices[5],
        vertices[4], vertices[6], vertices[2],
        vertices[2], vertices[6], vertices[7],
        vertices[6], vertices[4], vertices[5],
        vertices[1], vertices[3], vertices[7],
        vertices[0], vertices[2], vertices[3],
        vertices[4], vertices[0], vertices[1]};

    bounding_box = new Mesh{bounding_box_vertices};
}

void C3DObject::draw_bounding_box(GLuint shader) {
    bounding_box->draw(shader);
}

void C3DObject::draw(GLuint shader) {
    Mesh::draw(shader);
}

void C3DObject::update_distance_to_camera(glm::vec3 camera_position, glm::mat4 camera_view_matrix) {
    // TODO 3: compute and update the distance_to_camera
    // Note: you can use the vertices defined in bounding_box_vertices for this
}