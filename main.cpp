
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





    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_75.inp");

    bora.doStructuralAnalysis();


//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T6("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_120.inp");
//
//    bora_T6.doStructuralAnalysis();


    std::cout << "Hello, World!" << std::endl;




    return 0;
}