//
// Created by igorn on 13/06/2018.
//

#ifndef FEMPROJECT_ELEMENTISOPAR_Q4_HPP
#define FEMPROJECT_ELEMENTISOPAR_Q4_HPP

#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace Eigen;
using namespace std;

/**
 * Q4 Isoparametric Element Class.
 * @tparam numericType numeric type to be used, either float or double.
 */
template <class numericType>
class elementClass_Q4 {
public:
    /// Default Constructor
    elementClass_Q4();

    /// General Constructor
    elementClass_Q4(const std::vector<unsigned int>  &nodalConnectivity, const Eigen::Matrix<numericType,4,2> &nodalPosCoord);

    // METHODS
    /**
     * Method: Get interpolation functions of the isoparametric element. It returns the uncompressed format.
     * @param positionVec Position Vector of a point inside the element (to interpolate).
     * @return Returns N (uncompressed version): Represents the nodal interpolation functions of the isoparametric element. N(2,4);
     */
    Eigen::SparseMatrix<numericType>  sprMatN(const Matrix<numericType, 2, 1> &positionVec) const;

    /**
     * Method: Get interpolation function derivatives of the isoparametric element. It returns the uncompressed format.
     * @param positionVec Position Vector of a point inside the element (to interpolate).
     * @return Returns B (uncompressed version): A sparse matrix with the derivatives of the isoparametric element. B(3,4);
     */
    Eigen::SparseMatrix<numericType>  sprMatB(const Matrix<numericType, 2, 1> &positionVec) const;

    /**
     * Method: Evaluate the local stiffness matrix of the element.
     * @param youngModulus Young's Modulus of the element.
     * @param poissonRatio Poisson Ratio of the element.
     * @return local stiffness matrix of the element.
     */
    Eigen::SparseMatrix<numericType> sprMatK(numericType youngModulus,numericType poissonRatio) const;

    /**
     * Evaluate the area of the element.
     * @return area of the element.
     */
    numericType evalArea() const;

    // GETTER & SETTER
    /**
     * Getter Method: Get the identificators of all nodes of this element.
     * @return the identificators of all nodes of this element.
     */
    const std::vector<unsigned int> &getM_nodalConnectivity() const;

    /**
     * Returns the displacement vector.
     * @return displacement vector.
     */
    const Matrix<numericType, 8, 1> &get_uVec() const {
        return m_uVec;
    }

    /**
     * Returns the strain vector.
     * @return displacement vector.
     */
    const Matrix<numericType, 3, 1> &get_eVec() const {
        return m_eVec;
    }

    /**
     * Returns the stress vector.
     * @return displacement vector.
     */
    const Matrix<numericType, 3, 1> &get_sigVec() const {
        return m_sigVec;
    }

    /**
     * Evaluates stress strain and saves it.
     * @param uVec displacement vector.
     */
    void evaluateStressStrain(const Matrix<numericType, 8, 1> &uVec, numericType youngModulus,numericType posissonRatio);


private:
    // MEMBERS
    /**
     * Member: It stores a matrix os coefficients. Each column corresponds to the coefficient's polynomial vector.
     */
    Eigen::Matrix<numericType, 4, 4> m_interpFunCoeff;

    /**
     * Member: It stores identificators of all nodes of this element. It must be stored in counter-clockwise order.
     */
    std::vector<unsigned int> m_nodalConnectivity;

    /**
     * Member: It stores coordinates of all nodes of this element.
     */
    Eigen::Matrix<numericType,4,2> m_nodalPosCoord;

    /**
     * Memeber: Stores the displacement vector.
     */
    Eigen::Matrix<numericType,8,1> m_uVec;

    /**
     * Memeber: Stores the strain vector.
     */
    Eigen::Matrix<numericType,3,1> m_eVec;

    /**
     * Memeber: Stores the stress vector.
     */
    Eigen::Matrix<numericType,3,1> m_sigVec;

private:

    // METHODS
    /**
     * Method: Get interpolation functions of the isoparametric element at a point. It returns the compressed format.
     * @param positionVec Position Vector of a point inside the element (to interpolate).
     * @return N(eps,eta), compressed in a vector.
     */
    Eigen::Matrix<numericType, 4, 1>  evalInterpFunc(const Matrix<numericType, 2, 1> &positionVec) const;

    /**
     * Method: Get interpolation function derivatives of the isoparametric element at a point. It returns the compressed format.
     * @param positionVec Position Vector of a point inside the element (to interpolate).
     * @return B(eps,eta), compressed in a dense matrix.
     */
    Eigen::Matrix<numericType, 2, 4> evalInterpFuncDiff(const Matrix<numericType, 2, 1> &positionVec) const;

    /**
     * Method: Evaluate the determinant of the Jacobian of the isoparametric element at a point.
     * @param positionVec Position Vector of a point inside the element (to interpolate).
     * @return |J(eps,eta)|, determinant of the matrix.
     */
    numericType evalJacobDet(const Matrix<numericType, 2, 1> &positionVec) const;

    /**
     * Method: Evaluate the constitutive matrix of the element.
     * @param youngModulus Young's Modulus of the element.
     * @param poissonRatio Poisson Ratio of the element.
     * @return C(E,v).
     */
    Eigen::SparseMatrix<numericType>  evalConstitutiveMatrix(numericType youngModulus,numericType poissonRatio) const;

};

/* ---------------------------------------- /\ definition | declaration \/ ---------------------------------------- */

template <class numericType>
elementClass_Q4<numericType>::
elementClass_Q4()
{
    this->m_interpFunCoeff <<   .25, .25, .25, .25,
            .25, -.25, .25, -.25,
            .25, -.25, -.25, .25,
            .25, .25, -.25, -.25;
}

template <class numericType>
elementClass_Q4<numericType>::
elementClass_Q4(const std::vector<unsigned int>  &nodalConnectivity, const Eigen::Matrix<numericType,4,2> &nodalPosCoord):
        m_nodalConnectivity(nodalConnectivity), m_nodalPosCoord(nodalPosCoord)
{
    this->m_interpFunCoeff <<   .25, .25, .25, .25,
            .25, -.25, .25, -.25,
            .25, -.25, -.25, .25,
            .25, .25, -.25, -.25;
}

template <class numericType>
Eigen::SparseMatrix<numericType>  elementClass_Q4<numericType>::
sprMatN(const Matrix<numericType, 2, 1> &positionVec) const {

    Eigen::SparseMatrix<numericType> N_uncompressed(2,8); // Sparse matrix to be returned (must be filled)

    // Auxiliar objects to help fill a sparse matrix
    typedef Eigen::Triplet<numericType> TripNumericType;                // Triplet typedef for a sparse matrix
    std::vector<TripNumericType> tripletList;                           // Triplet list for indexation of sparse matrix
    tripletList.reserve(8);                                             // Allocating memory for the triplet list

    Eigen::Matrix<numericType, 4, 1>  N_compressed;                     // Stores the compressed N vector
    N_compressed = this->evalInterpFunc(positionVec);                   // Evaluating the compressed N vector

    unsigned int curNodeFun = 0;    // current node's interpolation function
    unsigned int curRow = 0;        // current row

    for(curNodeFun = 0; curNodeFun < 4; ++curNodeFun){
        for(curRow = 0; curRow < 2; ++curRow) {
            tripletList.push_back(TripNumericType(curRow, curRow+curNodeFun*2, N_compressed(curNodeFun,0)));    // [curRow,curRow+curNodeFun*spaceDimension]: interpFunVectorComp(curNodeFun,0)
        }
    }

    N_uncompressed.setFromTriplets(tripletList.begin(), tripletList.end());
    return N_uncompressed;
}

template <class numericType>
Eigen::SparseMatrix<numericType>  elementClass_Q4<numericType>::
sprMatB(const Matrix<numericType, 2, 1> &positionVec) const {

    // Auxiliar objects to help fill a sparse matrix
    typedef Eigen::Triplet<numericType> TripNumericType;                // Triplet typedef for a sparse matrix
    std::vector<TripNumericType> tripletList;                           // Triplet list for indexation of sparse matrix

    Eigen::Matrix<numericType,2,4> B_compressed;                        // Stores the compressed dense matrix B
    B_compressed = this->evalInterpFuncDiff(positionVec);                // Evaluating the dense matrix B


    unsigned int curNodeFun = 0;                                        // current node's interpolation function
    unsigned int curRow = 0;                                            // current row

    tripletList.reserve(4*4);                                           // Allocating memory for the triplet list
    Eigen::SparseMatrix<numericType> B_uncompressed(3,8);               // Sparse matrix to be returned (must be filled)

    // Feeding triplet to fill the sparse matrix
    for(curNodeFun = 0; curNodeFun < 4; ++curNodeFun){

        tripletList.push_back(TripNumericType(0, 0+2*curNodeFun, B_compressed(0,curNodeFun)));
        tripletList.push_back(TripNumericType(1, 1+2*curNodeFun, B_compressed(1,curNodeFun)));

        tripletList.push_back(TripNumericType(2, 0+2*curNodeFun, B_compressed(1,curNodeFun)));
        tripletList.push_back(TripNumericType(2, 1+2*curNodeFun, B_compressed(0,curNodeFun)));
    }

    B_uncompressed.setFromTriplets(tripletList.begin(), tripletList.end()); // Filling the sparse matrix
    return B_uncompressed;                                                  // Returning the answer (for this case)
}

template <class numericType>
Eigen::SparseMatrix<numericType>  elementClass_Q4<numericType>::
sprMatK(numericType youngModulus,numericType poissonRatio) const {

    Eigen::Matrix<numericType,2,1> pointVec;
    Eigen::SparseMatrix<numericType> B;
    Eigen::SparseMatrix<numericType> C;
    C = evalConstitutiveMatrix(youngModulus,poissonRatio);
    numericType jacobDet = 0;

    Eigen::SparseMatrix<numericType> Kl;

    // Point ( +1/sqrt(3) , +1/sqrt(3))
    Eigen::SparseMatrix<numericType> k1;
    pointVec << +1./std::sqrt(3), +1./std::sqrt(3);
    B = this->sprMatB(pointVec);
    jacobDet = this->evalJacobDet(pointVec);
    k1 = SparseMatrix<numericType>(B.transpose()) * C * B * jacobDet;

    // Point ( +1/sqrt(3) , -1/sqrt(3))
    Eigen::SparseMatrix<numericType> k2;
    pointVec << +1./std::sqrt(3), -1./std::sqrt(3);
    B = this->sprMatB(pointVec);
    jacobDet = this->evalJacobDet(pointVec);
    k2 = SparseMatrix<numericType>(B.transpose()) * C * B * jacobDet;

    // Point ( +1/sqrt(3) , +1/sqrt(3))
    Eigen::SparseMatrix<numericType> k3;
    pointVec << -1./std::sqrt(3), +1./std::sqrt(3);
    B = this->sprMatB(pointVec);
    jacobDet = this->evalJacobDet(pointVec);
    k3 = SparseMatrix<numericType>(B.transpose()) * C * B * jacobDet;

    // Point ( +1/sqrt(3) , +1/sqrt(3))
    Eigen::SparseMatrix<numericType> k4;
    pointVec << -1./std::sqrt(3), -1./std::sqrt(3);
    B = this->sprMatB(pointVec);
    jacobDet = this->evalJacobDet(pointVec);
    k4 = SparseMatrix<numericType>(B.transpose()) * C * B * jacobDet;

    Kl = k1+k2+k3+k4;

    return Kl;
}

template <class numericType>
void  elementClass_Q4<numericType>::
evaluateStressStrain(const Matrix<numericType, 8, 1> &uVec, numericType youngModulus,numericType poissonRatio) {

    Matrix<numericType, 2, 1> pointVec;
    pointVec << 0., 0.;

    Eigen::SparseMatrix<numericType> B;
    Eigen::SparseMatrix<numericType> C;
    C = evalConstitutiveMatrix(youngModulus,poissonRatio);
    B = this->sprMatB(pointVec);

    m_uVec = uVec;
    m_eVec = B*uVec;
    m_sigVec = C*m_eVec;
};

template <class numericType>
numericType elementClass_Q4<numericType>::
evalArea() const{
    Eigen::Matrix<numericType,2,2> mat1 = m_nodalPosCoord.block(0,0,2,2);
    Eigen::Matrix<numericType,2,2> mat2 = m_nodalPosCoord.block(2,0,2,2);

    return 0.5*(abs(mat1.determinant())+abs(mat2.determinant()));
}


template <class numericType>
Eigen::Matrix<numericType, 4, 1> elementClass_Q4<numericType>::
evalInterpFunc(const Matrix<numericType, 2, 1> &positionVec) const{

    numericType eps = positionVec(0);                                       // Storing the first spatial coordinate
    numericType eta = positionVec(1);                                       // Storing the second spatial coordinate

    Eigen::Matrix<numericType, 4, 1> N_compressed;                          // Allocating memory for the return value

    Eigen::Matrix<numericType, 4, 1> tempMatrixOfVariables;                 // Allocating memory for the polynomial terms
    tempMatrixOfVariables << 1., eps, eta, eps*eta;                         // Evaluating the polynomial terms

    N_compressed.noalias() = this->m_interpFunCoeff*tempMatrixOfVariables;  // Evaluating the return value

    return N_compressed;                                                    // Returning the answer
}

template <class numericType>
Eigen::Matrix<numericType, 2,4> elementClass_Q4<numericType>::
evalInterpFuncDiff(const Matrix<numericType, 2, 1> &positionVec) const{

    numericType eps = positionVec(0);                                       // Storing the first spatial coordinate
    numericType eta = positionVec(1);                                       // Storing the second spatial coordinate

    Eigen::Matrix<numericType, 2, 4> B_compressed;                          // Allocating memory for the return value

    Eigen::Matrix<numericType, 4, 1> tempMatrixOfVariablesDiffEps;          // Allocating memory for the polynomial terms (dN/dEps)
    tempMatrixOfVariablesDiffEps << 0, 1, 0, eta;                           // Evaluating the polynomial terms (dN/dEps)

    Eigen::Matrix<numericType, 4, 1> tempMatrixOfVariablesDiffEta;          // Allocating memory for the polynomial terms (dN/dEta)
    tempMatrixOfVariablesDiffEta << 0, 0, 1, eps;                           // Evaluating the polynomial terms (dN/dEta)

    // Evaluating the return value
    B_compressed.row(0) = (this->m_interpFunCoeff*tempMatrixOfVariablesDiffEps).transpose();
    B_compressed.row(1) = (this->m_interpFunCoeff*tempMatrixOfVariablesDiffEta).transpose();


    return B_compressed;    // Returning the answer
}

template <class numericType>
numericType elementClass_Q4<numericType>::
evalJacobDet(const Matrix<numericType, 2, 1> &positionVec) const{
    return ((this->evalInterpFuncDiff(positionVec)) * (this->m_nodalPosCoord)).determinant();
}

template<class numericType>
const std::vector<unsigned int> &elementClass_Q4<numericType>::getM_nodalConnectivity() const {
    return m_nodalConnectivity;
}

template <class numericType>
Eigen::SparseMatrix<numericType> elementClass_Q4<numericType>::
evalConstitutiveMatrix(numericType youngModulus,numericType poissonRatio) const{

    // Auxiliar objects to help fill a sparse matrix
    typedef Eigen::Triplet<numericType> TripNumericType;                // Triplet typedef for a sparse matrix
    std::vector<TripNumericType> tripletList;                           // Triplet list for indexation of sparse matrix
    tripletList.reserve(5);                                             // Allocating memory for the triplet list

    Eigen::SparseMatrix<numericType> C(3,3);                            // Sparse matrix to be returned (must be filled)

    // Plane Stress:
    numericType auxValue1 = youngModulus/(1.-poissonRatio*poissonRatio);  // auxValue1 = E/(1-v^2)
    numericType auxValue2 = poissonRatio*auxValue1;                          // auxValue2 = v*E/(1-v^2)


    // Feeding triplet to fill the sparse matrix
    tripletList.push_back(TripNumericType(0, 0, auxValue1));
    tripletList.push_back(TripNumericType(0, 1, auxValue2));
    tripletList.push_back(TripNumericType(1, 0, auxValue2));
    tripletList.push_back(TripNumericType(1, 1, auxValue1));
    tripletList.push_back(TripNumericType(2, 2, auxValue1*(1.-poissonRatio)*.5));

    C.setFromTriplets(tripletList.begin(), tripletList.end());          // Filling the sparse matrix
    return C;                                                           // Returning the answer
}

#endif //FEMPROJECT_ELEMENTISOPAR_Q4_HPP
