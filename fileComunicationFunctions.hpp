//
// Created by lucas_omena on 21/06/2018.
//

#ifndef FEMPROJECT_FILECOMUNICATIONFUNCTIONS_HPP
#define FEMPROJECT_FILECOMUNICATIONFUNCTIONS_HPP

#endif //FEMPROJECT_FILECOMUNICATIONFUNCTIONS_HPP


#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Eigen/Dense>

#include "baseNodeClass.hpp"
using namespace Eigen;
using namespace std;
/**
 * Read input and write output files library.
 * @fileName name of the input files.
 * @return return the type of data requested.
 */


/// Node Reader
void readNodes(std::ifstream &myReadFile, std::vector<baseNodeClass<2,double>>& nodeVector)
{
    std::string line;
    std::string token;
    double x,y;
    Eigen::Matrix<double,2,1> position;

    while (myReadFile.is_open())
    {
        std::getline(myReadFile,line);
        if (line == "*Node")
        {
            while (1) {
                std::getline(myReadFile, line);
                std::istringstream ss(line);
                std::getline(ss, token, ',');
                if (token == "*Element")
                    break;
                std::getline(ss, token, ',');
                x = stof(token);
                std::getline(ss, token, ',');
                y = stof(token);
                cout << x << " " << y << endl;
                position << x,y;
                nodeVector.push_back(baseNodeClass<2,double>(position));
            }
            break;
        }
    }


}
