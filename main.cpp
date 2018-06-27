
#include <iostream>
#include <Eigen/Dense>
#include <set>

#include "elementClass_Q4.hpp"
#include "elementClass_T6.hpp"
#include "baseNodeClass.hpp"
#include "baseStructuralAnalysisClass.hpp"

using namespace Eigen;
using namespace std;

int main() {

    MatrixXd m(2,2);
    m << 1, 2, 3, 4;
    std::cout << m << std::endl;
    std::cout << m*m << std::endl;

    m << 4 , 8 , 12, 16;
    std::cout << m << std::endl;
    std::cout << m*m << std::endl;



    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_15.inp");

    bora.doStructuralAnalysis();

    std::cout << "Hello, World!" << std::endl;




    return 0;
}