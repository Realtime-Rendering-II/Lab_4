#ifndef LAB_4_C3DOBJECT_H
#define LAB_4_C3DOBJECT_H

#include "mesh.h"

class C3DObject : Mesh{
public:
    C3DObject(const std::string &obj_path, const std::string &texture_path);

    void set_model_matrix(glm::mat4 model_matrix);

    void update_distance_to_camera(glm::vec3 camera_position, glm::mat4 camera_view_matrix);

    void draw_bounding_box(GLuint shader);

    void draw(GLuint shader) override;

    bool operator < (C3DObject const& object) const{
        return distance_to_camera < object.distance_to_camera;
    };

    float distance_to_camera = -1;
private:
    Mesh* bounding_box;

    std::vector<Vertex> bounding_box_vertices;

    void compute_bounding_box();
};
#endif //LAB_4_C3DOBJECT_H
