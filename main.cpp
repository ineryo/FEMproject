
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


//
    cout<<  "\n\nELEMENTO Q4 871 ELEMENTOS\n\n";

    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_15.inp");

    bora.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 231 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora2("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_30.inp");
//
//    bora2.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 88 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora3("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_45.inp");
//
//    bora3.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 51 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora4("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_40.inp");
//
//    bora4.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 39 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora5("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_75.inp");
//
//    bora5.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 22 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora6("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_90.inp");
//
//    bora6.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 20 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora7("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_105.inp");
//
//    bora7.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO Q4 16 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_Q4<double>,baseNodeClass<2,double>> bora8("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//Q4_120.inp");
//
//    bora8.doStructuralAnalysis();
//
//
//    cout<<  "\n\nELEMENTO T6 348 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T6("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_35.inp");
//
//    bora_T6.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO T6 160 ELEMENTOS\n\n";
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T62("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_50.inp");
//
//    bora_T62.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO T6 78 ELEMENTOS\n\n";
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T63("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_75.inp");
//
//    bora_T63.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO T6 44 ELEMENTOS\n\n";
//
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T64("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_90.inp");
//
//    bora_T64.doStructuralAnalysis();
//
//    cout<<  "\n\nELEMENTO T6 40 ELEMENTOS\n\n";
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T65("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_105.inp");
//
//    bora_T65.doStructuralAnalysis();

//    cout<<  "\n\nELEMENTO T6 32 ELEMENTOS\n\n";
//    baseStructuralAnalysisClass<double,elementClass_T6<double>,baseNodeClass<2,double>> bora_T66("..//Exemplos e Arquivos Abaqus//Exemplos Final//inp//T6_120.inp");
//
//    bora_T66.doStructuralAnalysis();
//
//
//    std::cout << "Hello, World!" << std::endl;




    return 0;
}