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

    template <class T>
    bool vectorContains(std::vector<T> v, T key) {
        if (std::count(v.begin(), v.end(), key)) {
            return true;
        }
        return false;
    }

    static unsigned long x = 123456789, y = 362436069, z = 521288629;

    unsigned long xorshf96(void) {          //period 2^96-1
        unsigned long t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }

    void addCube(std::vector <Vertex>* vertices, std::vector <Index>* indices,int vertex_index, glm::vec3 center, glm::vec3 size, glm::vec3 color) {
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

        Index idx;
        for (int i = 0; i < 36; i++) cube_vertexes[i] += vertex_index;
        memcpy(idx.indices, cube_vertexes, sizeof(cube_vertexes));
        indices->push_back(idx);
    }

    /* --Reference
    int to1D(int x, int y, int z, int xMax, int yMax) {
        return (z * xMax * yMax) + (y * xMax) + x;
    }

    int* to3D(int idx, int xMax, int yMax) {
        int z = idx / (xMax * yMax);
        idx -= (z * xMax * yMax);
        int y = idx / xMax;
        int x = idx % xMax;
        return new int[3] { x, y, z };
    }
    */

    unsigned int CA3D::getAdjacentCount(unsigned int i) {
        int count = 0;

        int zi = i / (this->size_x * this->size_y);
        i -= (zi * this->size_x * this->size_y);
        int yi = i / this->size_x;
        int xi = i % this->size_x;

        if (this->neighborhood_mode == CA3D_NEIGHBORHOOD_NEUMANN) {
        }

        if (this->neighborhood_mode == CA3D_NEIGHBORHOOD_MOORE) {
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    for (int z = -1; z <= 1; z++) {
                        if ((xi + x) < 0 || (xi + x) >= this->size_x) continue;
                        if ((yi + y) < 0 || (yi + y) >= this->size_y) continue;
                        if ((zi + z) < 0 || (zi + z) >= this->size_z) continue;

                        if (this->map[(xi + x) + this->size_x * ((yi + y) + this->size_y * (zi + z))] > 0) count += 1;
                    }
                }
            }
        }

        return count;
    }

    /* ----  Init  -----*/
	CA3D::CA3D(unsigned int size_x, const unsigned int size_y, const unsigned int size_z) {
        this->size_x = size_x;
        this->size_y = size_y;
        this->size_z = size_z;
        this->size_i = size_x * size_y * size_z;

        this->map = new int [size_i];
        for (unsigned int i = 0; i < size_i; ++i) {
            this->map[i] = 0;
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
            for (unsigned int i = 0; i < size_i; ++i) this->map[i] = 0;
        }

        if (mode == CA3D_INIT_MODE_FULL) {
            for (unsigned int i = 0; i < size_i; ++i) this->map[i] = this->stateCount - 1;
        }

        if (mode == CA3D_INIT_MODE_RANDOM) {
            for (unsigned int i = 0; i < size_i; ++i) this->map[i] = (xorshf96() % 2) > 0 ? this->stateCount - 1 : 0;
        }

        if (mode == CA3D_INIT_MODE_RANDOM_STATE) {
            for (unsigned int i = 0; i < size_i; ++i) this->map[i] = xorshf96() % this->stateCount;
        }

        if (mode == CA3D_INIT_MODE_CENTER_BLOCK) {
            this->resetMap(CA3D_INIT_MODE_BLANK);
            unsigned int size = 1;
            unsigned int xm = this->size_x / 2;
            unsigned int ym = this->size_y / 2;
            unsigned int zm = this->size_z / 2;
            for (unsigned int x = xm - size; x < xm + size; ++x)
                for (unsigned int y = ym - size; y < ym + size; ++y)
                    for (unsigned int z = zm - size; z < zm + size; ++z)
                        this->map[x + this->size_x * (y + this->size_y * z)] = this->stateCount - 1;
        }
    }

    void CA3D::magic() {
        for (unsigned int i = 0; i < size_i; i++) {
            unsigned int adj = getAdjacentCount(i);
            //if(adj>0) std::cout << adj << std::endl
            bool survive = adj == 4;  //vectorContains<int>(this->aliveIF, adj);
            bool birth = adj == 4; //vectorContains<int>(this->bornIF, adj);

            if (this->map[i] > 0 && !survive) {
                this->map[i] -= 1;
                continue;
            }

            if (this->map[i] == 0 && birth) {
                this->map[i] = this->stateCount - 1;
                continue;
            }

        }
    }


    /* ----  Graphics  -----*/
    void CA3D::initMesh() {
        this->vertices = new std::vector <Vertex>();
        this->indices = new std::vector <Index>();
        this->indicesOriginal = new std::vector <Index>();

        unsigned int cubes_vertexes[] = {
            0, 1, 3,   0, 2, 3, //Top     face
            4, 6, 7,   4, 5, 7, //Bottom  face
            0, 1, 5,   0, 4, 5, //Rear    face
            2, 3, 7,   2, 6, 7, //Front   face
            1, 5, 7,   1, 3, 7, //Left    face
            0, 2, 6,   0, 4, 6, //Right   face
        };

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

                    Index idx;
                    Index idx2;
                    memcpy(idx.indices, cubes_vertexes, sizeof(cubes_vertexes));
                    memcpy(idx2.indices, cubes_vertexes, sizeof(cubes_vertexes));
                    for (int i = 0; i < 36; i++) cubes_vertexes[i] += 8;
                    this->indices->push_back(idx);
                    this->indicesOriginal->push_back(idx2);

                }
            }
        }

        float thickness = 0.2f;
        float offset_from_main_cubes = 0.75f;
        glm::vec3 colorBorder(0.0f, 0.0f, 1.0f);


        glm::vec3 center(-0.5f, -0.5f, -0.5f);
        glm::vec3 size(this->size_x + offset_from_main_cubes, this->size_y + offset_from_main_cubes, this->size_z + offset_from_main_cubes);


        unsigned int vertex_index = cubes_vertexes[0];
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
        for (unsigned int i = 0; i < this->size_i; ++i) {
            if (this->map[i] == 0) {
                memset(this->indices->at(i).indices, 0, 144);
            } else {
                memcpy(this->indices->at(i).indices, this->indicesOriginal->at(i).indices, 144);
            }
        }

        mesh->updateIndexes();
        mesh->draw(shader, camera, glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
}
