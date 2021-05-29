#include "CA3D.h"
#include <iostream>

namespace CA3D {
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



	CA3D::CA3D(unsigned int size_x, unsigned int size_y) {
        this->size_x;
        this->size_y;
	}

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
        std::cout << "A cells has " << this->stateCount << " states" << std::endl;

        this->neighborhood_mode = rule_split.at(3).c_str()[0] == CA3D_NEIGHBORHOOD_MOORE ? CA3D_NEIGHBORHOOD_MOORE : CA3D_NEIGHBORHOOD_NEUMANN;
        std::cout << "Cell neighborhood mode: " << (this->neighborhood_mode == CA3D_NEIGHBORHOOD_MOORE ? "Moore" : "Neumann") << std::endl;

        return true;
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
                for(; a<=b; a++) output.push_back(a);

            } else {
                //Sub param is a number
                output.push_back(std::stoi(sub_param));
            }
        }


        return output;
    }


}
