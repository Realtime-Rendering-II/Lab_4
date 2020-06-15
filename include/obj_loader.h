#ifndef LAB_4_OBJ_LOADER_H
#define LAB_4_OBJ_LOADER_H

#include <fstream>
#include <string>
#include <sstream>
#include "mesh.h"

namespace ObjLoader{
    std::vector<Vertex> load_obj(std::string const& obj_path);
};

#endif //LAB_4_OBJ_LOADER_H
