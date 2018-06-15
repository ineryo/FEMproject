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
    baseNodeClass();
    // General Constructor
    explicit baseNodeClass(Eigen::Matrix<numericType,nSpatialDimension,1> position);

    // GETTER & SETTER
    const Eigen::Matrix<numericType, nSpatialDimension, 1> &getM_position() const;

protected:
    // MEMBERS
    Eigen::Matrix<numericType, nSpatialDimension, 1> m_position;

};

/* ---------------------------------------- /\ definition | declaration \/ ---------------------------------------- */

template <unsigned int nSpatialDimension, class numericType>
baseNodeClass<nSpatialDimension, numericType>
        ::baseNodeClass()
{

}

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