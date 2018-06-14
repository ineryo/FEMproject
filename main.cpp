#include <iostream>
#include <Eigen/Dense>

#include "elementIsopar_Q4.hpp"

using Eigen::MatrixXd;

int main() {

    MatrixXd m(2,2);
    m << 1, 2, 3, 4;
    std::cout << m << std::endl;
    std::cout << m*m << std::endl;

    elementIsopar_Q4 vai;

    std::cout << vai.getInterpFunc(2,1,1) << std::endl;
    std::cout << vai.getInterpFunc(2,-1,1) << std::endl;
    std::cout << vai.getInterpFunc(2,-1,-1) << std::endl;
    std::cout << vai.getInterpFunc(2,1,-1) << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}