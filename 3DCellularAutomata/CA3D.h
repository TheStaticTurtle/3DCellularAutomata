#ifndef CA3D_MAIN_CLASS_H
#define CA3D_MAIN_CLASS_H

#include <string>
#include <vector>

#include "Cube.h"

#define CA3D_NEIGHBORHOOD_MOORE   'M'
#define CA3D_NEIGHBORHOOD_NEUMANN 'N' 

#define CA3D_INIT_MODE_BLANK        0
#define CA3D_INIT_MODE_FULL         1
#define CA3D_INIT_MODE_RANDOM       2
#define CA3D_INIT_MODE_RANDOM_STATE 3
#define CA3D_INIT_MODE_CENTER_BLOCK 4

namespace CA3D {

	std::vector<std::string> split(const std::string& s, char seperator);

	class CA3D {
		public:
			CA3D(unsigned int size_x, unsigned int size_y, unsigned int size_z);
			bool initRule(std::string rule);
			void resetMap(int mode);

			void draw(Shader& shader, Camera& camera);
			void magic();

		private:
			std::vector <int> getIntVectorFromRuleParameter(std::string param);

			char neighborhood_mode = CA3D_NEIGHBORHOOD_MOORE;
			unsigned int size_x = 20;
			unsigned int size_y = 20;
			unsigned int size_z = 20;

			std::vector<int> aliveIF{ 4 };
			std::vector<int> bornIF{ 4 };
			int stateCount = 5;

			int*** map;

			void initMesh();
			std::vector <Vertex>* vertices;
			std::vector <GLuint>* indices;
			Mesh* mesh;
	};
}

#endif
