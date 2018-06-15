//
// Created by igorn on 14/06/2018.
//

#ifndef FEMPROJECT_BASEELEMCLASS_HPP
#define FEMPROJECT_BASEELEMCLASS_HPP

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

template <unsigned int nNodePerElem, class elemIsoparClass>
class baseElemClass {
public:
    // CONSTRUCTOR
    baseElemClass(const vector<unsigned int> &m_nodeListOfElem, elemIsoparClass *anElemIsopar);

    // GETTER & SETTER
    const vector<unsigned int> &getM_nodeListOfElem() const;

protected:
    // MEMBERS
    std::vector<unsigned int> m_nodeListOfElem;
    elemIsoparClass* elemIsopar;

};

/* ---------------------------------------- /\ definition | declaration \/ ---------------------------------------- */

template <unsigned int nNodePerElem, class elemIsoparClass>
baseElemClass<nNodePerElem, elemIsoparClass>
        ::baseElemClass(const vector<unsigned int> &m_nodeListOfElem, elemIsoparClass *anElemIsopar):
        m_nodeListOfElem(m_nodeListOfElem), elemIsopar(anElemIsopar) {
}

template <unsigned int nNodePerElem, class elemIsoparClass>
const vector<unsigned int> &baseElemClass<nNodePerElem, elemIsoparClass>
        ::getM_nodeListOfElem() const {
    return m_nodeListOfElem;
}


#endif //FEMPROJECT_BASEELEMCLASS_HPP
