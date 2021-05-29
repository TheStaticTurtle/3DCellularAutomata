#ifndef CA3D_MAIN_CLASS_H
#define CA3D_MAIN_CLASS_H

#include <string>
#include <vector>

#define CA3D_NEIGHBORHOOD_MOORE   'M'
#define CA3D_NEIGHBORHOOD_NEUMANN 'N' 

namespace CA3D {

class CA3D
{
public:
	CA3D(unsigned int size_x, unsigned int size_y);
	bool initRule(std::string rule);

private:
	std::vector <int> getIntVectorFromRuleParameter(std::string param);

	char neighborhood_mode = CA3D_NEIGHBORHOOD_MOORE;
	unsigned int size_x = 20;
	unsigned int size_y = 20;

	std::vector<int> aliveIF{ 4 };
	std::vector<int> bornIF{ 4 };
	int stateCount = 5;

};

}

#endif
