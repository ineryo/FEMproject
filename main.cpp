#include <iostream>
#include <Eigen/Dense>

#include "elementClass_Q4.hpp"

using namespace Eigen;
using namespace std;

int main() {

    MatrixXd m(2,2);
    m << 1, 2, 3, 4;
    std::cout << m << std::endl;
    std::cout << m*m << std::endl;

    elementClass_Q4<double> vai;

    Matrix<double, 2, 1> pos1;
    pos1 << 1., 1.;
    Matrix<double, 2, 1> pos2;
    pos2 << -1.,1.;
    Matrix<double, 2, 1> pos3;
    pos3 << -1.,-1.;
    Matrix<double, 2, 1> pos4;
    pos4 << 1.,-1.;

    std::cout << std::endl << std::endl << "1st Point / compressed N:" << std::endl <<  vai.sprMatN(pos1) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point / compressed N:" << std::endl <<  vai.sprMatN(pos2) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point / compressed N:" << std::endl <<  vai.sprMatN(pos3) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point / compressed N:" << std::endl <<  vai.sprMatN(pos4) << std::endl;

    std::cout << std::endl << std::endl << "1st Point / compressed B:" << std::endl <<  vai.sprMatB(pos1) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point / compressed B:" << std::endl <<  vai.sprMatB(pos2) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point / compressed B:" << std::endl <<  vai.sprMatB(pos3) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point / compressed B:" << std::endl <<  vai.sprMatB(pos4) << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}