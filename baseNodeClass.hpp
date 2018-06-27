//
// Created by igorn on 14/06/2018.
//

#ifndef FEMPROJECT_NODECLASS_HPP
#define FEMPROJECT_NODECLASS_HPP

#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

template <unsigned int nSpatialDimension, class numericType>
class baseNodeClass {
public:
    // CONSTRUCTOR
    // Default Constructor
    baseNodeClass() = default;
    // General Constructor
    explicit baseNodeClass(Eigen::Matrix<numericType,nSpatialDimension,1> position);

    // GETTER & SETTER
    const Eigen::Matrix<numericType, nSpatialDimension, 1> &getM_position() const;

    bool set_eVec(const Eigen::Matrix<numericType,Eigen::Dynamic, Eigen::Dynamic> &eVec){
        m_eVec = eVec;
    }

    bool set_sigVec(const Eigen::Matrix<numericType,Eigen::Dynamic, Eigen::Dynamic> &sigVec){
        m_sigVec = sigVec;
    }

protected:
    // MEMBERS
    Eigen::Matrix<numericType, nSpatialDimension, 1> m_position;

    /**
     * Memeber: Stores the strain vector.
     */
    Eigen::Matrix<numericType,Eigen::Dynamic, Eigen::Dynamic> m_eVec;

    /**
     * Memeber: Stores the stress vector.
     */
    Eigen::Matrix<numericType,Eigen::Dynamic, Eigen::Dynamic> m_sigVec;



};

/* ---------------------------------------- /\ definition | declaration \/ ---------------------------------------- */

template <unsigned int nSpatialDimension, class numericType>
baseNodeClass<nSpatialDimension, numericType>
        ::baseNodeClass(Eigen::Matrix<numericType, nSpatialDimension, 1> position):
        m_position(position)
{

}

template<unsigned int nSpatialDimension, class numericType>
const Eigen::Matrix<numericType, nSpatialDimension, 1> &
baseNodeClass<nSpatialDimension, numericType>
        ::getM_position() const
{
    return m_position;
}


#endif //FEMPROJECT_NODECLASS_HPP