//
// Created by igorn on 21/06/2018.
//

#ifndef FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP
#define FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <string>
#include <fstream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

// #include "inutility.h"

using namespace Eigen;
using namespace std;

/**
 * Simple structure to stores a linear system solution by Eigen lib.
 */
class eigenLinearSolverInfo
{
public:
    /**
     * Default constructor
     */
    eigenLinearSolverInfo() {}

    /**
     * Setter method
     * @param iter Number of iterations.
     * @param er Estimated error.
     * @param inform Information about the solution process.
     */
    void setValues(unsigned int iter, double er, Eigen::ComputationInfo inform) {ite=iter;error=er;info=inform;}

    unsigned int ite;
    double error;
    Eigen::ComputationInfo info;
};

/**
 * Structural Analysis superclass to store and compute the analysis.
 * @tparam numericType numeric type to be used, either float or double.
 * @tparam elemClass Class to be used to store and compute the elements.
 * @tparam nodeClass Class to be used to store and compute the nodes.
 */
template <class numericType,class elemClass,class nodeClass>
class baseStructuralAnalysisClass
{
public:
    /**
     * Default constructor.
     */
    baseStructuralAnalysisClass();

    /**
     * Read-file constructor.
     * @param fileName Name of the file to be read.
     */
    baseStructuralAnalysisClass(std::string fileName);

    /**
     * Default Destructor.
     */
    ~baseStructuralAnalysisClass() {}

    // VIRTUAL METHODS
    virtual unsigned int doStructuralAnalysis();

//protected:
    // VIRTUAL METHODS
    virtual bool readFormatedFile(std::string fileName);
    virtual bool readFromFile_ElemList(std::ifstream* openedFile);
    virtual bool readFromFile_NodeList(std::ifstream* opennedFile);

    virtual Eigen::Matrix<unsigned int,Eigen::Dynamic,1>computeDegreesOfFreedom();
    virtual Eigen::SparseMatrix<numericType>    evaluateGlobalStiffMat();
    virtual Eigen::Matrix<numericType,Eigen::Dynamic,1>evaluateGlobalnodeForceVec();
    virtual Eigen::Matrix<numericType,Eigen::Dynamic,1>evaluateGlobalDisplacVec();
    virtual bool updateLocalNodeInfo();
    virtual bool updateLocalElemInfo();

    // PURE VIRTUAL METHODS
    virtual Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> rotationMatrix( Eigen::Matrix<numericType,Eigen::Dynamic,1>& dirElemVec ) const =0;

    // MEMBERS - scalar, list
    // Spacial dimension
    unsigned int m_nDim;
    // Number of possible degrees of freedom in each node
    unsigned int m_maxNodeDegFree;
    // Number of nodes per element
    unsigned int m_nNodePerElem;
    // Number of properties per element
    unsigned int m_nPropPerElem;
    // Number of degrees of freedom
    unsigned int m_nTotalDoF;
    // Penalty value
    numericType m_penalty;
    const numericType m_penalty_multiplier = 1e20;
    // List of elements
    std::vector<elemClass> m_elemList;
    // List of nodes
    std::vector<nodeClass> m_nodeList;
    // Name of the file input
    std::string m_fileInputName;
    // Report status of the linear solver computation
    eigenLinearSolverInfo m_linSolverInfo;


    // MEMBERS - vector, matrix
    // Degrees of freedom vector
    Eigen::Matrix<unsigned int,Eigen::Dynamic,1> m_degFreeVec;
    // Global stiffness matrix
    Eigen::SparseMatrix<numericType> m_glbStiffMat;
    // Global force vector
    Eigen::Matrix<numericType,Eigen::Dynamic,1> m_glbnodeForceVec;
    // Global displacement vector
    Eigen::Matrix<numericType,Eigen::Dynamic,1> m_glbDisplacVec;

    // MEMBERS - boolean
    // Check if initialization info of the elements is saved already
    bool m_isElemListSaved;
    // Check if initialization info of the nodes is saved already
    bool m_isNodeListSaved;
    // Check if initialization info is saved already
    bool m_isInitialStructureSaved;
    // Check if all local rotation matrix are saved already (one for each element)
    bool m_isLocalRotationMatrixComputed;
    // Check if initialization info is saved already
    bool m_isDegreeOfFreedomVecComputed;
    // Check if global stiffness matrix is computed already
    bool m_isGlbStiffMatComputed;
    // Check if global force vector is computed already
    bool m_isGlbnodeForceVecComputed;
    // Check if global displacement vector is computed already
    bool m_isGlbDisplacVecComputed;
    // Check if local element information (force and displacement) is updated already
    bool m_isLocalElemInfoUpdated;
    // Check if local nodal information (force and displacement) is updated already
    bool m_isLocalNodeInfoUpdated;
};

template<class numericType, class elemClass, class nodeClass>
baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::baseStructuralAnalysisClass():
        m_isElemListSaved(false),m_isNodeListSaved(false),m_isInitialStructureSaved(false),m_isLocalRotationMatrixComputed(false),m_isDegreeOfFreedomVecComputed(false), m_isGlbStiffMatComputed(false), m_isGlbnodeForceVecComputed(false), m_isGlbDisplacVecComputed(false), m_isLocalElemInfoUpdated(false), m_isLocalNodeInfoUpdated(false)
{
    nodeClass aNode;
    m_maxNodeDegFree = aNode.maxNodeDegFree();
    m_nDim = aNode.nDimNode();

    elemClass anElem;
    m_nNodePerElem = anElem.connectionsPerElem();
    m_nPropPerElem = anElem.propertiesPerElem();
}

template<class numericType, class elemClass, class nodeClass>
baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::baseStructuralAnalysisClass(string fileName):
        baseStructuralAnalysisClass<numericType,elemClass,nodeClass>()
{
    m_fileInputName = fileName;
}

template<class numericType, class elemClass, class nodeClass>
unsigned int baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::doStructuralAnalysis()
{
    bool flag = true;
    unsigned int numOperations = 0;

    // Read file!
    flag = readFormatedFile(m_fileInputName);
    if (flag)
    {
        ++numOperations;
        // evaluate all Local Rotation Matrix!
        //flag = evaluateLocalRotationMat();
        if (flag)
        {
            ++numOperations;
            // compute Degrees of Freedom!
            computeDegreesOfFreedom();
            flag = m_isDegreeOfFreedomVecComputed;
            if (flag)
            {
                ++numOperations;
                // evaluate Global Force Vector!
                evaluateGlobalnodeForceVec();
                flag = m_isGlbnodeForceVecComputed;
                if (flag)
                {
                    ++numOperations;
                    // evaluate Global Stiffness Matrix!
                    evaluateGlobalStiffMat();
                    flag = m_isGlbStiffMatComputed;
                    if (flag)
                    {
                        ++numOperations;
                        // evaluate Global Displacement Vector (solve problem)!
                        evaluateGlobalDisplacVec();
                        flag = m_isGlbDisplacVecComputed;
                        if (flag)
                        {
                            ++numOperations;
                            // update Local Nodal Information!
                            flag = updateLocalNodeInfo();
                            if (flag)
                            {
                                ++numOperations;
                                // evaluate Local Element Information!
                                flag = updateLocalElemInfo();
                                if (flag)   {   ++numOperations;    }
                            }
                        }
                    }
                }
            }
        }
    }
    return numOperations;
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::readFormatedFile(string fileName)
{
    bool flagOpen = false;
    bool flagElem = false;
    bool flagNode = false;

    if( !m_isInitialStructureSaved )
    {
        std::string aLine;
        std::string buff;

        unsigned int numNode = 0;
        unsigned int numElem = 0;

        ifstream inFile;
        inFile.open(fileName);

        if (inFile.is_open())
        {
            flagOpen = true;

            while ( getline(inFile,aLine) )
            {
                // Check if its a comment (it shouldnt be read, go to next line)
                if(aLine[0] != '*') continue;

                // Check if it list Elements
                if ( aLine.find("*Element")!=std::string::npos )
                {
                    flagElem = readFromFile_ElemList( &inFile);
                }

                // Check if it list Nodes
                if ( aLine.find("*Node")!=std::string::npos )
                {
                    flagNode = readFromFile_NodeList( &inFile);
                }
            }
            inFile.close();
        }
    }

    m_isInitialStructureSaved = (m_isInitialStructureSaved) || (flagOpen && flagElem && flagNode);
    return m_isInitialStructureSaved;
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::readFromFile_ElemList(ifstream *openedFile)
{
    bool flagOpen = false;

    if( (!m_isInitialStructureSaved) && (!m_isElemListSaved) && (m_isNodeListSaved) )
    {
        std::string aLine = "0000";
        std::string pieceLine;

        if (openedFile->is_open())
        {
            flagOpen = true;
            numericType aux_numericType;
            unsigned int aux_unsigned_int;

            unsigned int curElem;
            unsigned int curAux;

            while(aLine[0] != '*') {
                if( getline(*openedFile,aLine) )
                {
                    std::stringstream thisLine (aLine);
                    // Reading id
                    std::getline(thisLine, pieceLine, ',');
                    // aux_unsigned_int= std::stoul(pieceLine);

                    // Reading nodalConnectivity[4]
                    std::vector<unsigned int> nodalConnectivity;
                    for(curAux = 0; curAux< m_nNodePerElem ; ++curAux)
                    {
                        std::getline(thisLine, pieceLine, ',');
                        aux_unsigned_int= std::stoul(pieceLine);
                        nodalConnectivity.push_back(aux_unsigned_int);
                    }

                    // Evaluating nodalPosCoord[4,2]
                    Eigen::Matrix<numericType,m_nNodePerElem,m_nSpatialDimension> nodalPosCoord;
                    for(curAux = 0; curAux<m_nNodePerElem; ++curAux)
                    {
                        nodalPosCoord.row(curAux) = this->m_nodeList[nodalConnectivity[curAux]].getM_position().transpose();
                    }

                    // elementClass_Q4(const std::vector<unsigned int>  &nodalConnectivity, const Eigen::Matrix<numericType,4,2> &nodalPosCoord);
                    m_elemList.push_back( elemClass(nodalConnectivity, nodalPosCoord) );
                }
            }
        }
    }


    // Updating flag
    m_isElemListSaved = (m_isInitialStructureSaved) || ((flagOpen)&&((m_isNodeListSaved)));
    return m_isElemListSaved;
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::readFromFile_NodeList(ifstream *openedFile)
{
    bool flagOpen = false;

    if( (!m_isInitialStructureSaved) && (!m_isNodeListSaved) )
    {
        std::string aLine = "0000";
        std::string pieceLine;

        if (openedFile->is_open())
        {
            flagOpen = true;
            unsigned int aux_unsigned_int;
            numericType aux_numericType;

            unsigned int curNode;
            unsigned int curAux;

            while(aLine[0] != '*') {
                if( getline(*openedFile,aLine) )
                {
                    std::stringstream thisLine (aLine);
                    // Reading id
                    std::getline(thisLine, pieceLine, ',');
                    // aux_unsigned_int= std::stoul(pieceLine);

                    // Reading nodalConnectivity[4]
                    std::vector<unsigned int> nodalConnectivity;
                    for(curAux = 0; curAux< m_nNodePerElem ; ++curAux)
                    {
                        std::getline(thisLine, pieceLine, ',');
                        aux_unsigned_int= std::stoul(pieceLine);
                        nodalConnectivity.push_back(aux_unsigned_int);
                    }

                    // Evaluating nodalPosCoord[4,2]
                    Eigen::Matrix<numericType,m_nSpatialDimension,1> position;
                    for(curAux = 0; curAux<m_nSpatialDimension; ++curAux)
                    {
                        std::getline(thisLine, pieceLine, ',');
                        position(curAux) = std::stod(pieceLine);
                    }

                    // baseNodeClass(Eigen::Matrix<numericType,nSpatialDimension,1> position);
                    m_nodeList.push_back( nodeClass( position) );
                }
            }
        }
    }
    m_isNodeListSaved = (m_isInitialStructureSaved) || (flagOpen);
    return m_isNodeListSaved;
}

template<class numericType, class elemClass, class nodeClass>
Eigen::Matrix<unsigned int, Eigen::Dynamic, 1> baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::computeDegreesOfFreedom()
{
    unsigned int numNode = m_nodeList.size();
    unsigned int numNodeDegFree = m_maxNodeDegFree;
    unsigned int numDof = numNode*numNodeDegFree;

    if( m_isInitialStructureSaved && (!m_isDegreeOfFreedomVecComputed) )
    {
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> degreesFreedomVec = Eigen::Matrix<unsigned int, Eigen::Dynamic, 1>::Zero(numDof);
        Eigen::Matrix<numericType,Eigen::Dynamic,1> curNode_isFixedVec;
        unsigned int actualDegreeFreedom = 0;
        unsigned int curNode;
        unsigned int curDegAtNode;

        // Searching all nodes
        for(curNode = 0; curNode<numNode; ++curNode)
        {
            // Saving "DoF" of each node
            curNode_isFixedVec = m_nodeList[curNode].isItFixed();

            // Searching all "DoF" at current node
            for(curDegAtNode = 0; curDegAtNode<numNodeDegFree; ++curDegAtNode)
            {
                // Verifying each "DoF" at current node
                if( curNode_isFixedVec(curDegAtNode) == 0 )
                {
                    // If it is an actual free degree, update the number of actual DoF and save it
                    degreesFreedomVec(curNode*numNodeDegFree+curDegAtNode) = ++actualDegreeFreedom;
                }
            }
        }
        m_nTotalDoF = actualDegreeFreedom;
        m_degFreeVec = degreesFreedomVec;
    }

    // Updating flag
    m_isDegreeOfFreedomVecComputed = (m_isInitialStructureSaved);

    // Returning suitable value
    if(m_isDegreeOfFreedomVecComputed)  {   return m_degFreeVec;    }
    else                                    return Eigen::Matrix<unsigned int, Eigen::Dynamic, 1>::Zero(numDof);
}

template<class numericType, class elemClass, class nodeClass>
Eigen::SparseMatrix<numericType> baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::evaluateGlobalStiffMat()
{
    // Getting constantes and some auxiliar variables
    unsigned int numNodePerElem = m_nNodePerElem;   // Number of nodes per element [USING =2]!!!!!!!!!!!!!
    unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
    unsigned int numElemDegFree = numNodePerElem*numNodeDegFree;// Number of possible degrees of freedom in each elem
    unsigned int numElem = m_elemList.size();       // Number of elements
    unsigned int numNode = m_nodeList.size();       // Number of nodes
    unsigned int idCurElem;                         // Index of an element
    unsigned int iniNode_CurElem;                   // Initial node of an element
    unsigned int endNode_CurElem;                   // End node of an element

    // Global Stiffness Matrix
    Eigen::SparseMatrix<numericType> glbStiffMatrix(numNode,numNode);

    if( (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed && m_isLocalRotationMatrixComputed) && (!m_isGlbStiffMatComputed) )
    {
        // Getting some small vectors and matrix
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemDofPosition_curElem(numElemDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> iniNodeDofPosition_curElem(numNodeDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> endNodeDofPosition_curElem(numNodeDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemConnection_curElem(numNodePerElem);       // ConnectionVector of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> localStifffMat_CurElem(numElemDegFree,numElemDegFree);  // localStiffMatrix of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> rotationMat_CurElem(numElemDegFree,numElemDegFree);     // rotationMatrix of an element
        typedef Eigen::Triplet<numericType> TripNumericType;                // Triplet typedef for a sparse matrix
        std::vector<TripNumericType> tripletList;                           // Triplet list for indexzation of sparse matrix
        tripletList.reserve(numElemDegFree*numElemDegFree*numElem);         // Allocating memory for the triplet list
        unsigned int curRow_LclStfMat =0;                                   // Index of a row at localStiffMatrix
        unsigned int curCol_LclStfMat =0;                                   // Index of a col at localStiffMatrix
        //    unsigned int curRowDof_LclStfMat =0;                              // DoF of the IndexRow @ localStiffMatrix
        //    unsigned int curColDof_LclStfMat =0;                              // DoF of the IndexCol @ localStiffMatrix
        unsigned int glbIdRow = 0;                                          // Index of the row @ globalStiffMatrix
        unsigned int glbIdCol = 0;                                          // Index of the col @ globalStiffMatrix
        numericType stiff;                                                  // An element of the localStiffMatrix
        numericType maxValue = 0;                                           // Variable to compute the max value of localStiffMatrix

        // Trick...
        // Walking inside the elementList: elem by elem
        for( idCurElem=0; idCurElem<numElem; ++idCurElem)
        {
            // Saving the connection (id nodes) of an element
            elemConnection_curElem = m_elemList[idCurElem].getNodeConnection();
            iniNode_CurElem = elemConnection_curElem(0);
            endNode_CurElem = elemConnection_curElem(1);

            //        // Getting the DoFs of the nodes that connects an element
            //        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemDoF_curElem(numRowLclStiff);
            //        elemDoF_curElem << m_degFreeVec.segment(iniNode_CurElem*numNodeDegFree,numNodeDegFree);
            //        elemDoF_curElem << m_degFreeVec.segment(endNode_CurElem*numNodeDegFree,numNodeDegFree);

            // Getting the 'position' of the DoF of the nodes that connects an element
            iniNodeDofPosition_curElem = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,iniNode_CurElem*numNodeDegFree,(iniNode_CurElem+1u)*numNodeDegFree - 1u);
            endNodeDofPosition_curElem = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,endNode_CurElem*numNodeDegFree,(endNode_CurElem+1u)*numNodeDegFree - 1u);
            elemDofPosition_curElem << iniNodeDofPosition_curElem,endNodeDofPosition_curElem;

            // Computing the local stiffness matrix of an element and its rotation matrix
            localStifffMat_CurElem = m_elemList[idCurElem].localStiffMatrix();
            rotationMat_CurElem =  m_elemList[idCurElem].getRotMat();

            // Applying rotation to get it in global coordinates
            localStifffMat_CurElem = rotationMat_CurElem.transpose() * localStifffMat_CurElem * rotationMat_CurElem;

            // Walking inside the localStiffMatrix: row by row
            for( curRow_LclStfMat = 0; curRow_LclStfMat<numElemDegFree ; ++curRow_LclStfMat)
            {
                glbIdRow = elemDofPosition_curElem(curRow_LclStfMat);
                //            // Get the DoF of this specific row (corresponding DoF of the node)
                //            curRowDof_LclStfMat = elemDoF_curElem(curRow_LclStfMat);
                //            if( curRowDof_LclStfMat = 0 )  {   continue;   }    // Neglet this row if DoF=0 (its not free!)

                // Walking inside the localStiffMatrix: col by col
                for( curCol_LclStfMat = 0; curCol_LclStfMat<numElemDegFree ; ++curCol_LclStfMat)
                {
                    glbIdCol = elemDofPosition_curElem(curCol_LclStfMat);
                    //                // Get the DoF of this specific col (corresponding DoF of the node)
                    //                curColDof_LclStfMat = elemDoF_curElem(curCol_LclStfMat);
                    //                if( curColDof_LclStfMat = 0 )  {   continue;   }    // Neglet this col if DoF=0 (its not free!)

                    // Getting the stiffness of localStiffMatrix roxXcol and building the triplet
                    stiff = localStifffMat_CurElem(curRow_LclStfMat, curCol_LclStfMat);
                    tripletList.push_back( TripNumericType( glbIdRow , glbIdCol , stiff ) );

                    // Computing the maxValue of diag(globalStiffMatrix)
                    if( (glbIdRow == glbIdCol) && (maxValue < stiff) )      maxValue = stiff;
                }
            }
        }

        // Saving global stiffness matrix as sparse (from triplets(i,j,v_ij))
        glbStiffMatrix.setFromTriplets(tripletList.begin(), tripletList.end());
        m_glbStiffMat = glbStiffMatrix;

        // Saving penalty value at class member m_penalty
        m_penalty = maxValue * m_penalty_multiplier;
    }

    // Updating flag
    m_isGlbStiffMatComputed = (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed && m_isLocalRotationMatrixComputed);

    // Returning suitable value
    if(m_isGlbStiffMatComputed)   { return m_glbStiffMat;   }
    else                            return glbStiffMatrix;
}

template<class numericType, class elemClass, class nodeClass>
Eigen::Matrix<numericType, Eigen::Dynamic, 1> baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::evaluateGlobalnodeForceVec()
{
    // Getting constant and some auxiliar variables
    unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
    unsigned int numNode = m_nodeList.size();       // Number of nodes
    unsigned int numDof = numNode*numNodeDegFree;   // Number of DoF
    unsigned int idCurNode;                         // Index of an node

    if( (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed  && m_isLocalRotationMatrixComputed) && (!m_isGlbnodeForceVecComputed) )
    {
        // Global Force Vector
        Eigen::Matrix<numericType,Eigen::Dynamic,1> glbnodeForceVector = Eigen::Matrix<numericType,Eigen::Dynamic,1>::Zero(numDof);

        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> dofPosition_curNode(numNodeDegFree);
        Eigen::Matrix<numericType,Eigen::Dynamic,1> localnodeForceVec(numNodeDegFree);         // localnodeForceVector of an node

        // Getting some small vectors and matrix
        unsigned int idCurnodeForceVec =0;                                   // Index at localnodeForceVector
        //    unsigned int curRowDof_LclStfMat =0;                           // DoF of the IndexRow @ localStiffMatrix
        unsigned int glbId = 0;                                          // Index at globalnodeForceVector

        // Trick...
        // Walking inside the elementList: node by node
        for( idCurNode=0; idCurNode<numNode; ++idCurNode)
        {

            // Getting the 'position' of the DoF of the nodes that connects an element
            dofPosition_curNode = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,idCurNode*numNodeDegFree,(idCurNode+1u)*numNodeDegFree - 1u);

            // Computing the localnodeForceVector of an element
            localnodeForceVec = m_nodeList[idCurNode].nodeForceVec();

            // Walking inside the localStiffMatrix: row by row
            for( idCurnodeForceVec = 0; idCurnodeForceVec<numNodeDegFree ; ++idCurnodeForceVec)
            {
                glbId = dofPosition_curNode(idCurnodeForceVec);

                // Saving global force vector
                glbnodeForceVector(glbId) = localnodeForceVec(idCurnodeForceVec);
            }
        }
        m_glbnodeForceVec = glbnodeForceVector;
    }

    // Updating flag
    m_isGlbnodeForceVecComputed = (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed && m_isLocalRotationMatrixComputed);

    // Returning suitable value
    if(m_isGlbDisplacVecComputed)   {   return m_glbnodeForceVec;   }
    else                                return Eigen::Matrix<numericType,Eigen::Dynamic,1>::Zero(numDof);
}

template<class numericType, class elemClass, class nodeClass>
Eigen::Matrix<numericType, Eigen::Dynamic, 1> baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::evaluateGlobalDisplacVec()
{
    bool isSolverSuccess = false;

    // Getting constant and some auxiliar variables
    unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
    unsigned int numNode = m_nodeList.size();       // Number of nodes
    unsigned int numDof = numNode*numNodeDegFree;   // Number of DoF
    unsigned int curDof_CurNode;                     // Index of an DoF
    unsigned int idCurNode;                         // Index of an DoF
    unsigned int idCurDof;

    if( m_isGlbStiffMatComputed && m_isGlbnodeForceVecComputed)
    {
        // Global Displacement Vector
        Eigen::Matrix<numericType,Eigen::Dynamic,1> displacVector = Eigen::Matrix<numericType,Eigen::Dynamic,1>::Zero(numDof);

        // Getting global force vector and stiffness matrix
        Eigen::SparseMatrix<numericType> penalizedStiffMatrix = m_glbStiffMat;
        Eigen::Matrix<numericType,Eigen::Dynamic,1> penalizednodeForceVector = m_glbnodeForceVec;
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> dofPosition_curNode(numNodeDegFree);
        Eigen::Matrix<numericType,Eigen::Dynamic,1> dispVec_curNode(numNodeDegFree);


        for ( idCurNode=0; idCurNode<numNode; ++idCurNode)
        {
            // Getting the 'position' of the DoF of the node
            dofPosition_curNode = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,idCurNode*numNodeDegFree,(idCurNode+1u)*numNodeDegFree - 1u);

            // Getting the displacement vector od the node
            dispVec_curNode = m_nodeList[idCurNode].displacVec();

            for ( curDof_CurNode=0; curDof_CurNode<numNodeDegFree; ++curDof_CurNode)
            {
                idCurDof = dofPosition_curNode(curDof_CurNode);

                // Check if this "DoF" is fixed (not an actually DoF)
                if( m_degFreeVec(idCurDof) == 0 )
                {
                    // If it is fixed:
                    penalizedStiffMatrix.coeffRef(idCurDof,idCurDof) = m_penalty;
                    penalizednodeForceVector(idCurDof) = m_penalty * dispVec_curNode(curDof_CurNode);
                }
            }
        }

        // Solving by conjugate gradiente method
        ConjugateGradient<SparseMatrix<numericType>, Lower|Upper> cgSolver;
//        cgSolver.setTolerance(1e-8);
        cgSolver.compute(penalizedStiffMatrix);
        displacVector = cgSolver.solve(penalizednodeForceVector);
        m_linSolverInfo.setValues (cgSolver.iterations(),cgSolver.error(),cgSolver.info());

        if(m_linSolverInfo.info == Eigen::Success)
        {
            isSolverSuccess = true;
            m_glbnodeForceVec = m_glbStiffMat * displacVector;      // Updating global force vector!
            m_glbDisplacVec = displacVector;
        }
    }

    // Updating flag
    m_isGlbDisplacVecComputed = (( m_isGlbStiffMatComputed && m_isGlbnodeForceVecComputed) && isSolverSuccess);

    // Returning suitable value
    if(m_isGlbDisplacVecComputed)   {   return m_glbDisplacVec; }
    else                                return Eigen::Matrix<numericType,Eigen::Dynamic,1>::Zero(numDof);
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::updateLocalNodeInfo()
{
    // Getting constant and some auxiliar variables
    unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
    unsigned int numNode = m_nodeList.size();       // Number of nodes
    unsigned int idCurNode;                         // Index of an node

    if( (m_isGlbDisplacVecComputed && m_isGlbnodeForceVecComputed && m_isGlbStiffMatComputed) && !m_isLocalNodeInfoUpdated)
    {
        // Walking inside the nodeList: node by node
        for( idCurNode=0; idCurNode<numNode; ++idCurNode)
        {
            // Getting current node force vector
            Eigen::Matrix<numericType,Eigen::Dynamic,1> nodenodeForceVec = m_glbnodeForceVec.segment(idCurNode*numNodeDegFree,numNodeDegFree);
            m_nodeList[idCurNode].setnodeForceVec( nodenodeForceVec );

            // Getting current node displacement vector
            Eigen::Matrix<numericType,Eigen::Dynamic,1> nodeDispVec = m_glbDisplacVec.segment(idCurNode*numNodeDegFree,numNodeDegFree);
            m_nodeList[idCurNode].setDisplacVec( nodeDispVec );
        }
    }

    m_isLocalNodeInfoUpdated = (m_isGlbDisplacVecComputed && m_isGlbnodeForceVecComputed && m_isGlbStiffMatComputed);
    return m_isLocalNodeInfoUpdated;
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::updateLocalElemInfo()
{
    if(m_isLocalNodeInfoUpdated && !m_isLocalElemInfoUpdated)
    {
        // Getting constantes and some auxiliar variables
        unsigned int numNodePerElem = m_nNodePerElem;   // Number of nodes per element [USING =2]!!!!!!!!!!!!!
        unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
        unsigned int numElemDegFree = numNodePerElem*numNodeDegFree;// Number of possible degrees of freedom in each elem
        unsigned int numElem = m_elemList.size();       // Number of elements
        unsigned int idCurElem;                         // Index of an element
        unsigned int iniNode_curElem;                   // Initial node of an element
        unsigned int endNode_curElem;                   // End node of an element

        // Getting some small vectors and matrix
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemConnection_curElem;       // ConnectionVector of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> rotationMat_CurElem;     // RotationMatrix of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    nodeForceVec_curElem(numElemDegFree);           // nodeForceVector of the iniNode
//        Eigen::Matrix<numericType,Eigen::Dynamic,1>    internForceVec_curElem(numNodeDegFree);           // nodeForceVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    posVec_iniNode_curElem;     // PositionVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    posVec_endNode_curElem;     // PositionVector of the endNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    dispVec_iniNode_curElem;    // DisplacementVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    dispVec_endNode_curElem;    // DisplacementVector of the endNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    tempVec;                    // Temporary vector
        numericType iniLength;

        // Walking inside the elementList: elem by elem
        for( idCurElem=0; idCurElem<numElem; ++idCurElem)
        {
            // Saving the connection (id nodes) of an element
            elemConnection_curElem = m_elemList[idCurElem].getNodeConnection();
            iniNode_curElem = elemConnection_curElem(0);
            endNode_curElem = elemConnection_curElem(1);

            // Saving the rotation matrix of an element
            rotationMat_CurElem =  m_elemList[idCurElem].getRotMat();

            // Getting position vector of both iniNode and endNode
            posVec_iniNode_curElem = m_nodeList[iniNode_curElem].positionVec();
            posVec_endNode_curElem = m_nodeList[endNode_curElem].positionVec();

            // Getting displacement vector of both iniNode and endNode
            dispVec_iniNode_curElem = m_nodeList[iniNode_curElem].displacVec();
            dispVec_endNode_curElem = m_nodeList[endNode_curElem].displacVec();

            // Evaluating engineering strain vector eps=deltaL/L and saving it
            tempVec = posVec_endNode_curElem - posVec_iniNode_curElem;
            iniLength = tempVec.norm();
            tempVec = (dispVec_endNode_curElem - dispVec_iniNode_curElem) / iniLength;
            m_elemList[idCurElem].setEngStrainVec(tempVec);

            // Getting force vector of both iniNode and endNode (and transforming from global to local coordinates)
            nodeForceVec_curElem << m_nodeList[iniNode_curElem].nodeForceVec() , m_nodeList[endNode_curElem].nodeForceVec();
            nodeForceVec_curElem = rotationMat_CurElem * nodeForceVec_curElem;
            m_elemList[idCurElem].setnodeForceVec( nodeForceVec_curElem );

//            // Getting internal force vector
//            internForceVec_curElem = nodeForceVec_curElem.tail(numNodeDegFree) - nodeForceVec_curElem.head(numNodeDegFree);
//            m_elemList[idCurElem].setInternForceVec( internForceVec_curElem );
        }
    }

    m_isLocalElemInfoUpdated = m_isLocalNodeInfoUpdated;
    return m_isLocalElemInfoUpdated;
}

#endif //FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP
