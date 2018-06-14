//
// Created by igorn on 13/06/2018.
//

#ifndef FEMPROJECT_ELEMENTISOPAR_Q4_HPP
#define FEMPROJECT_ELEMENTISOPAR_Q4_HPP

#include <Eigen/Dense>

using Eigen::MatrixXd;

class elementIsopar_Q4 {
public:
    // Constructor
    elementIsopar_Q4();

    double getInterpFunc(int idFunc, double eps, double eta);
    double getInterpFuncDiffEps(int idFunc, double eps, double eta);
    double getInterpFuncDiffEta(int idFunc, double eps, double eta);
    Eigen::Matrix<double, 3, 8> getInterpFuncDiff(double eps, double eta);


protected:
    // MEMBERS
    Eigen::Matrix<double, 4, 4> m_interp_coeffVector;

};

/* ---------------------------------------- /\ definition | declaration \/ ---------------------------------------- */

elementIsopar_Q4::elementIsopar_Q4()
{
    m_interp_coeffVector << .25, .25, .25, .25,
                            .25, -.25, .25, -.25,
                            .25, -.25, -.25, .25,
                            .25, .25, -.25, -.25;
}

double elementIsopar_Q4::getInterpFunc(int idFunc, double eps, double eta){
    Eigen::Matrix<double, 4, 1> tempMatrixOfVariables;
    tempMatrixOfVariables << 1., eps, eta, eps*eta;
    return tempMatrixOfVariables.dot(m_interp_coeffVector.row(idFunc));
}

double elementIsopar_Q4::getInterpFuncDiffEps(int idFunc, double eps, double eta){
    Eigen::Matrix<double, 4, 1> tempMatrixOfVariablesDiffEps;
    tempMatrixOfVariablesDiffEps << 0, 1, 0, eta;
    return tempMatrixOfVariablesDiffEps.dot(m_interp_coeffVector.row(idFunc));
}

double elementIsopar_Q4::getInterpFuncDiffEta(int idFunc, double eps, double eta){
    Eigen::Matrix<double, 4, 1> tempMatrixOfVariablesDiffEta;
    tempMatrixOfVariablesDiffEta << 0, 0, 1, eps;
    return tempMatrixOfVariablesDiffEta.dot(m_interp_coeffVector.row(idFunc));
}

Eigen::Matrix<double, 3, 8> elementIsopar_Q4::getInterpFuncDiff(double eps, double eta){
    Eigen::Matrix<double, 4, 1> tempMatrixOfVariablesDiffEps;
    tempMatrixOfVariablesDiffEps << 0, 1, 0, eta;
    Eigen::Matrix<double, 4, 1> tempMatrixOfVariablesDiffEta;
    tempMatrixOfVariablesDiffEta << 0, 0, 1, eps;
    Eigen::Matrix<double, 3, 8> B = MatrixXd::Zero(3, 8);

    Eigen::Matrix<double, 1, 4> interp_coeffVector_curRow;

    for(unsigned int curId = 0; curId < 4; ++curId){
        interp_coeffVector_curRow = m_interp_coeffVector.row(curId);

        B(0,0+curId*2) = tempMatrixOfVariablesDiffEta.dot(interp_coeffVector_curRow);
        B(1,1+curId*2) = tempMatrixOfVariablesDiffEta.dot(interp_coeffVector_curRow);
        B(2,0+curId*2) = B(0,0+curId*2);
        B(2,1+curId*2) = B(1,1+curId*2);
    }
    return B;
}


#endif //FEMPROJECT_ELEMENTISOPAR_Q4_HPP
