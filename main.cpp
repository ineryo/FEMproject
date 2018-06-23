
#include <iostream>
#include <Eigen/Dense>
#include <set>

#include "elementClass_Q4.hpp"
#include "elementClass_T6.hpp"

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

    elementClass_Q4<double> vai;

    Matrix<double, 2, 1> pos1;
    pos1 << 1., 1.;
    Matrix<double, 2, 1> pos2;
    pos2 << -1.,1.;
    Matrix<double, 2, 1> pos3;
    pos3 << -1.,-1.;
    Matrix<double, 2, 1> pos4;
    pos4 << 1.,-1.;

//    std::cout << std::endl << std::endl << "1st Point / compressed N:" << std::endl <<  vai.sprMatN(pos1) << std::endl;
//    std::cout << std::endl << std::endl << "2nd Point / compressed N:" << std::endl <<  vai.sprMatN(pos2) << std::endl;
//    std::cout << std::endl << std::endl << "3rd Point / compressed N:" << std::endl <<  vai.sprMatN(pos3) << std::endl;
//    std::cout << std::endl << std::endl << "4rd Point / compressed N:" << std::endl <<  vai.sprMatN(pos4) << std::endl;

    std::cout << std::endl << std::endl << "1st Point / compressed B:" << std::endl <<  vai.sprMatB(pos1) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point / compressed B:" << std::endl <<  vai.sprMatB(pos2) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point / compressed B:" << std::endl <<  vai.sprMatB(pos3) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point / compressed B:" << std::endl <<  vai.sprMatB(pos4) << std::endl;

    // elementClass_Q4(numericType youngModulus,numericType poissonRatio, const std::vector<unsigned int>  &nodalConnectivity, const Eigen::Matrix<numericType,4,2> &nodalPosCoord);
    double youngModulus = 1.e6;
    double poissonRatio = .5;
    std::vector<unsigned int>  nodalConnectivity;   nodalConnectivity.push_back(0); nodalConnectivity.push_back(1); nodalConnectivity.push_back(2); nodalConnectivity.push_back(3);
    Eigen::Matrix<double,4,2> nodalPosCoord;        nodalPosCoord << 1., 1., -1, 1, -1, -1, 1, -1;  nodalPosCoord *= 5;
    elementClass_Q4<double> foi( nodalConnectivity, nodalPosCoord);



    std::cout << std::endl << std::endl << "1st Point / uncompressed N:" << std::endl <<  foi.sprMatN(pos1) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point / uncompressed N:" << std::endl <<  foi.sprMatN(pos2) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point / uncompressed N:" << std::endl <<  foi.sprMatN(pos3) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point / uncompressed N:" << std::endl <<  foi.sprMatN(pos4) << std::endl;

    std::cout << std::endl << std::endl << "1st Point / uncompressed B:" << std::endl <<  foi.sprMatB(pos1) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point / uncompressed B:" << std::endl <<  foi.sprMatB(pos2) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point / uncompressed B:" << std::endl <<  foi.sprMatB(pos3) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point / uncompressed B:" << std::endl <<  foi.sprMatB(pos4) << std::endl;

    std::cout << std::endl << std::endl << "1st Point / uncompressed B:" << std::endl <<  foi.sprMatK(youngModulus,poissonRatio) << std::endl;




    elementClass_T6<double> vai_T6;

    Matrix<double, 2, 1> pos1_T6;
    pos1_T6 << 2., 0.;
    Matrix<double, 2, 1> pos2_T6;
    pos2_T6 << 1.,0.;
    Matrix<double, 2, 1> pos3_T6;
    pos3_T6 << 0.,0.;
    Matrix<double, 2, 1> pos4_T6;
    pos4_T6 << 0.,1.;
    Matrix<double, 2, 1> pos5_T6;
    pos5_T6 << 0,2.;
    Matrix<double, 2, 1> pos6_T6;
    pos6_T6 << 1.,1.;

    std::cout << std::endl << std::endl << "1st Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos1_T6) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos2_T6) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos3_T6) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos4_T6) << std::endl;
    std::cout << std::endl << std::endl << "5rd Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos5_T6) << std::endl;
    std::cout << std::endl << std::endl << "6rd Point T6/ compressed N:" << std::endl <<  vai_T6.sprMatN(pos6_T6) << std::endl;

    std::cout << std::endl << std::endl << "1st Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos1_T6) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos2_T6) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos3_T6) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos4_T6) << std::endl;
    std::cout << std::endl << std::endl << "5rd Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos5_T6) << std::endl;
    std::cout << std::endl << std::endl << "6rd Point T6/ compressed B:" << std::endl <<  vai_T6.sprMatB(pos6_T6) << std::endl;


    std::vector<unsigned int>  nodalConnectivity_T6;   nodalConnectivity_T6.push_back(0);
    nodalConnectivity_T6.push_back(1); nodalConnectivity_T6.push_back(2); nodalConnectivity_T6.push_back(3);
    nodalConnectivity_T6.push_back(4);nodalConnectivity_T6.push_back(5);

    Eigen::Matrix<double,6,2> nodalPosCoord_T6;        nodalPosCoord_T6 << 2., 0., 1., 0, 0, 0, 0, 1., 0, 2., 1., 1.;
    elementClass_T6<double> foi_T6(youngModulus,poissonRatio, nodalConnectivity_T6, nodalPosCoord_T6);


    std::cout << std::endl << std::endl << "1st Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos1_T6) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos2_T6) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos3_T6) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos4_T6) << std::endl;
    std::cout << std::endl << std::endl << "5rd Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos5_T6) << std::endl;
    std::cout << std::endl << std::endl << "6rd Point T6/ compressed N:" << std::endl <<  foi_T6.sprMatN(pos6_T6) << std::endl;

    std::cout << std::endl << std::endl << "1st Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos1_T6) << std::endl;
    std::cout << std::endl << std::endl << "2nd Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos2_T6) << std::endl;
    std::cout << std::endl << std::endl << "3rd Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos3_T6) << std::endl;
    std::cout << std::endl << std::endl << "4rd Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos4_T6) << std::endl;
    std::cout << std::endl << std::endl << "5rd Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos5_T6) << std::endl;
    std::cout << std::endl << std::endl << "6rd Point T6/ compressed B:" << std::endl <<  foi_T6.sprMatB(pos6_T6) << std::endl;

    std::cout << std::endl << std::endl << "K:" << std::endl <<  foi_T6.sprMatK() << std::endl;

    std::cout << "Hello, World!" << std::endl;


    std::set<unsigned int> lista;
    lista.insert(1);
    lista.insert(3);
    lista.insert(5);
    lista.insert(10);
    lista.insert(2);
    lista.insert(0);

    for (auto it = lista.begin(); it != lista.end(); ++it)
        std::cout << *it << "\n";


    return 0;
}