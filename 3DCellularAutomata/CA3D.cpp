#include "CA3D.h"
#include <iostream>

namespace CA3D {
    /* ----  Utilitaries  -----*/
    std::vector<std::string> split(const std::string& s, char seperator) {
        std::vector<std::string> output;
        std::string::size_type prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::string::npos) {
            std::string substring(s.substr(prev_pos, pos - prev_pos));
            output.push_back(substring);
            prev_pos = ++pos;
        }

        output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
        return output;
    }

    std::vector<int> CA3D::getIntVectorFromRuleParameter(std::string param)
    {
        std::vector<int> output = std::vector<int>();
        std::vector<std::string> sub_params = split(param, ',');

        for (const std::string& sub_param : sub_params) {
            if (sub_param.find("-") != std::string::npos) {
                //Sub param is a range
                std::vector<std::string> range = split(sub_param, '-');
                if (range.size() != 2) continue;
                int a = std::stoi(range.at(0));
                int b = std::stoi(range.at(1));
                for (; a <= b; a++) output.push_back(a);

            }
            else {
                //Sub param is a number
                output.push_back(std::stoi(sub_param));
            }
        }

        return output;
    }

    void addCube(std::vector <Vertex>* vertices, std::vector <GLuint>* indices,int vertex_index, glm::vec3 center, glm::vec3 size, glm::vec3 color) {
        float px, nx, py, ny, pz, nz;

        unsigned int cube_vertexes[] = {
            0, 1, 3,   0, 2, 3, //Top     face
            4, 6, 7,   4, 5, 7, //Bottom  face
            0, 1, 5,   0, 4, 5, //Rear    face
            2, 3, 7,   2, 6, 7, //Front   face
            1, 5, 7,   1, 3, 7, //Left    face
            0, 2, 6,   0, 4, 6, //Right   face
        };

        px = center.x + size.x / 2;
        nx = center.x - size.x / 2;
        py = center.y + size.y / 2;
        ny = center.y - size.y / 2;
        pz = center.z + size.z / 2;
        nz = center.z - size.z / 2;

        vertices->push_back(Vertex{ glm::vec3(nx,  py, nz), color });
        vertices->push_back(Vertex{ glm::vec3(px,  py, nz), color });
        vertices->push_back(Vertex{ glm::vec3(nx,  py, pz), color });
        vertices->push_back(Vertex{ glm::vec3(px,  py, pz), color });

        vertices->push_back(Vertex{ glm::vec3(nx, ny, nz), color });
        vertices->push_back(Vertex{ glm::vec3(px, ny, nz), color });
        vertices->push_back(Vertex{ glm::vec3(nx, ny, pz), color });
        vertices->push_back(Vertex{ glm::vec3(px, ny, pz), color });

        for (const int i : cube_vertexes) {
            indices->push_back(i + vertex_index);
        }
    }

    /* ----  Init  -----*/
	CA3D::CA3D(unsigned int size_x, const unsigned int size_y, const unsigned int size_z) {
        this->size_x = size_x;
        this->size_y = size_y;
        this->size_z = size_z;

        this->map = new int** [size_x];
        for (unsigned int x = 0; x < size_x; ++x) {
            this->map[x] = new int* [size_y];
            for (unsigned int y = 0; y < size_y; ++y) {
                this->map[x][y] = new int[size_z];
                for (unsigned int z = 0; z < size_z; ++z) {
                    this->map[x][y][z] = 0;
                }
            }
        }

        this->initMesh();
	}

    /* ----  Cellular Automata  -----*/
    bool CA3D::initRule(std::string rule) {
        std::cout << "Loading rule: " << rule << std::endl;

        std::vector<std::string> rule_split = split(rule, '/');
        if (rule_split.size() != 4) {
            std::cerr << "CA Rule is invalid (Size not 4)" << std::endl;
            return false;
        }

        this->aliveIF = getIntVectorFromRuleParameter(rule_split.at(0));
        std::cout << "A cells is alive if: ";  for (const int& i : this->aliveIF) std::cout << i << " ";  std::cout << "cells are alive" << std::endl;

        this->bornIF = getIntVectorFromRuleParameter(rule_split.at(1));
        std::cout << "A cells is born if: ";  for (const int& i : this->bornIF) std::cout << i << " ";  std::cout << "cells are alive" << std::endl;

        this->stateCount = std::stoi(rule_split.at(2));
        if (this->stateCount < 1) this->stateCount = 1;
        std::cout << "A cells has " << this->stateCount << " states" << std::endl;

        this->neighborhood_mode = rule_split.at(3).c_str()[0] == CA3D_NEIGHBORHOOD_MOORE ? CA3D_NEIGHBORHOOD_MOORE : CA3D_NEIGHBORHOOD_NEUMANN;
        std::cout << "Cell neighborhood mode: " << (this->neighborhood_mode == CA3D_NEIGHBORHOOD_MOORE ? "Moore" : "Neumann") << std::endl;

        return true;
    }

    void CA3D::resetMap(int mode) {
        if (mode == CA3D_INIT_MODE_BLANK) {
            for (unsigned int x = 0; x < this->size_x; ++x)
                for (unsigned int y = 0; y < this->size_y; ++y)
                    for (unsigned int z = 0; z < this->size_z; ++z)
                        this->map[x][y][z] = 0;
        }

        if (mode == CA3D_INIT_MODE_FULL) {
            for (unsigned int x = 0; x < this->size_x; ++x)
                for (unsigned int y = 0; y < this->size_y; ++y)
                    for (unsigned int z = 0; z < this->size_z; ++z)
                        this->map[x][y][z] = this->stateCount - 1;
        }

        if (mode == CA3D_INIT_MODE_RANDOM) {
            for (unsigned int x = 0; x < this->size_x; ++x)
                for (unsigned int y = 0; y < this->size_y; ++y)
                    for (unsigned int z = 0; z < this->size_z; ++z)
                        this->map[x][y][z] = (rand() % 100) > 50 ? this->stateCount - 1 : 0;
        }

        if (mode == CA3D_INIT_MODE_RANDOM_STATE) {
            for (unsigned int x = 0; x < this->size_x; ++x)
                for (unsigned int y = 0; y < this->size_y; ++y)
                    for (unsigned int z = 0; z < this->size_z; ++z)
                        this->map[x][y][z] = rand() % this->stateCount;
        }

        if (mode == CA3D_INIT_MODE_CENTER_BLOCK) {
            this->resetMap(CA3D_INIT_MODE_BLANK);
            unsigned int size = 3;
            unsigned int xm = this->size_x / 2;
            unsigned int ym = this->size_y / 2;
            unsigned int zm = this->size_z / 2;
            for (unsigned int x = xm - size; x < xm + size; ++x)
                for (unsigned int y = ym - size; y < ym + size; ++y)
                    for (unsigned int z = zm - size; z < zm + size; ++z)
                        this->map[x][y][z] = this->stateCount - 1;
        }
    }

    void CA3D::magic()
    {
    }


    /* ----  Graphics  -----*/
    void CA3D::initMesh() {
        this->vertices = new std::vector <Vertex>();
        this->indices = new std::vector <GLuint>();

        unsigned int cube_vertexes[] = {
            0, 1, 3,   0, 2, 3, //Top     face
            4, 6, 7,   4, 5, 7, //Bottom  face
            0, 1, 5,   0, 4, 5, //Rear    face
            2, 3, 7,   2, 6, 7, //Front   face
            1, 5, 7,   1, 3, 7, //Left    face
            0, 2, 6,   0, 4, 6, //Right   face
        };

        unsigned int vertex_index = 0;
        glm::vec3 colorCube(0.5f, 0.5f, 0.5f);

        for (unsigned int x = 0; x < size_x; ++x) {
            for (unsigned int y = 0; y < size_y; ++y) {
                for (unsigned int z = 0; z < size_z; ++z) {

                    float cx = (float)x - (float)this->size_x / 2.0f;
                    float cy = (float)y - (float)this->size_y / 2.0f;
                    float cz = (float)z - (float)this->size_z / 2.0f;

                    float px = cx + 0.8f / 2;
                    float nx = cx - 0.8f / 2;
                    float py = cy + 0.8f / 2;
                    float ny = cy - 0.8f / 2;
                    float pz = cz + 0.8f / 2;
                    float nz = cz - 0.8f / 2;

                    this->vertices->push_back(Vertex{ glm::vec3(nx,  py, nz), glm::vec3(1.0f, 0.0f, 0.0f) });
                    this->vertices->push_back(Vertex{ glm::vec3(px,  py, nz), glm::vec3(0.0f, 1.0f, 0.0f) });
                    this->vertices->push_back(Vertex{ glm::vec3(nx,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f) });
                    this->vertices->push_back(Vertex{ glm::vec3(px,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f) });

                    this->vertices->push_back(Vertex{ glm::vec3(nx, ny, nz), glm::vec3(1.0f, 1.0f, 0.0f) });
                    this->vertices->push_back(Vertex{ glm::vec3(px, ny, nz), glm::vec3(1.0f, 0.0f, 1.0f) });
                    this->vertices->push_back(Vertex{ glm::vec3(nx, ny, pz), glm::vec3(0.5f, 0.5f, 0.5f) });
                    this->vertices->push_back(Vertex{ glm::vec3(px, ny, pz), glm::vec3(1.0f, 1.0f, 1.0f) });

                    for (const int i : cube_vertexes) {
                        this->indices->push_back(i + vertex_index);
                    }
                    
                    //addCube(this->vertices, this->indices, vertex_index, glm::vec3(cx,cy,cz), glm::vec3(0.8f, 0.8f, 0.8f), colorCube); vertex_index += 8;


                }
            }
        }

        float thickness = 0.1f;
        float offset_from_main_cubes = 0.5f;
        glm::vec3 colorBorder(0.0f, 0.0f, 1.0f);



        glm::vec3 center(-0.5f, -0.5f, -0.5f);
        glm::vec3 size(this->size_x + offset_from_main_cubes, this->size_y + offset_from_main_cubes, this->size_z + offset_from_main_cubes);

        addCube(this->vertices, this->indices, vertex_index, glm::vec3(              center.x,  size.y / 2 + center.y, -size.z / 2 + center.z), glm::vec3(size.x, thickness, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(              center.x,  size.y / 2 + center.y,  size.z / 2 + center.z), glm::vec3(size.x, thickness, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(-size.x / 2 + center.x,  size.y / 2 + center.y,               center.z), glm::vec3(thickness, thickness, size.z), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3( size.x / 2 + center.x,  size.y / 2 + center.y,               center.z), glm::vec3(thickness, thickness, size.z), colorBorder); vertex_index += 8;

        addCube(this->vertices, this->indices, vertex_index, glm::vec3(              center.x, -size.y / 2 + center.y, -size.z / 2 + center.z), glm::vec3(size.x, thickness, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(              center.x, -size.y / 2 + center.y,  size.z / 2 + center.z), glm::vec3(size.x, thickness, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(-size.x / 2 + center.x, -size.y / 2 + center.y,               center.z), glm::vec3(thickness, thickness, size.z), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3( size.x / 2 + center.x, -size.y / 2 + center.y,               center.z), glm::vec3(thickness, thickness, size.z), colorBorder); vertex_index += 8;

        addCube(this->vertices, this->indices, vertex_index, glm::vec3( size.x / 2 + center.x,               center.y,  size.z / 2 + center.z), glm::vec3(thickness, size.y, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(-size.x / 2 + center.x,               center.y, -size.z / 2 + center.z), glm::vec3(thickness, size.y, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3 (size.x / 2 + center.x,               center.y, -size.z / 2 + center.z), glm::vec3(thickness, size.y, thickness), colorBorder); vertex_index += 8;
        addCube(this->vertices, this->indices, vertex_index, glm::vec3(-size.x / 2 + center.x,               center.y,  size.z / 2 + center.z), glm::vec3(thickness, size.y, thickness), colorBorder); vertex_index += 8;
        //addCube(this->vertices, this->indices, vertex_index, glm::vec3(-(size.x / 2), size.y / 2, 0.0f), glm::vec3(size.x, 0.0f, 0.0f)); vertex_index += 8;


        this->mesh = new Mesh(this->vertices, this->indices);
    }
    void CA3D::draw(Shader& shader, Camera& camera) {
        unsigned int vertex_index = 0;
        unsigned int indices_index = 0;

        unsigned int cube_vertexes[] = {
            0, 1, 3,   0, 2, 3, //Top     face
            4, 6, 7,   4, 5, 7, //Bottom  face
            0, 1, 5,   0, 4, 5, //Rear    face
            2, 3, 7,   2, 6, 7, //Front   face
            1, 5, 7,   1, 3, 7, //Left    face
            0, 2, 6,   0, 4, 6, //Right   face
        };

        for (unsigned int x = 0; x < size_x; ++x) {
            for (unsigned int y = 0; y < size_y; ++y) {
                for (unsigned int z = 0; z < size_z; ++z) {

                    if (this->map[x][y][z] == 0) {
                        for (unsigned int i = 0; i < 36; i++) {
                            this->indices->at(indices_index + i) = -1;
                        }
                    }
                    else {
                        for (unsigned int i=0; i < 36; i++) {
                            this->indices->at(indices_index + i) = vertex_index + cube_vertexes[i];
                        }
                    }

                    vertex_index += 8;
                    indices_index += 36;
                }
            }
        }

        mesh->update();
        mesh->draw(shader, camera, glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
}
