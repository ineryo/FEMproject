//
// Created by igorn on 21/06/2018.
//

#ifndef FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP
#define FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP

#include <iostream>
#include <iomanip>
#include <type_traits>
#include <string>
#include <set>
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
    eigenLinearSolverInfo()=default;

    /**
     * Setter method
     * @param iter Number of iterations.
     * @param er Estimated error.
     * @param inform Information about the solution process.
     */
    void setValues(std::ptrdiff_t iter, double er, Eigen::ComputationInfo inform) {ite=iter;error=er;info=inform;};

    std::ptrdiff_t ite;
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
    explicit baseStructuralAnalysisClass(std::string fileName);

    /**
     * Default Destructor.
     */
    ~baseStructuralAnalysisClass() {}

    // VIRTUAL METHODS
    virtual unsigned int doStructuralAnalysis();

protected:
    /**
     * Method: reads a formated file to store all initial information about the structure.
     * @param fileName file name (and optional directory).
     * @return flag.
     */
    virtual bool readFormatedFile(std::string fileName);

    /**
     * Method: reads PART of a formated file to store the list of elements.
     * @param fileName opened file (and already positioned).
     * @return flag.
     */
    virtual bool readFromFile_ElemList(std::ifstream* openedFile);


    /**
     * Method: reads PART of a formated file to store the list of nodes.
     * @param fileName opened file (and already positioned).
     * @return flag.
     */
    virtual bool readFromFile_NodeList(std::ifstream* openedFile);

    /**
     * Method to run all the set of nodes and create the DoF order.
     * @return vector with
     */
    virtual Eigen::Matrix<unsigned int,Eigen::Dynamic,1>computeDegreesOfFreedom();

    /**
     * Method to evaluate the global stiffness matrix.
     * @return Global stiffness matrix.
     */
    virtual Eigen::SparseMatrix<numericType>    evaluateGlobalStiffMat();

    /**
     * Method to evaluate the global force vector.
     * @return Global force vector.
     */
    virtual Eigen::SparseMatrix<numericType> evaluateGlobalForceVec();

    /**
     * Method to evaluate the global displacement vector (solves the linear system).
     * @return
     */
    virtual Eigen::Matrix<numericType,Eigen::Dynamic,1>evaluateGlobalDisplacVec();

    /**
     * Method to update the local information of the element with its displacement.
     * @return flag.
     */
    bool updateLocalInfo();

    // MEMBERS - scalar, list
    // Spacial dimension
    unsigned int m_nSpatialDimension;
    // Number of possible degrees of freedom in each node
    unsigned int m_maxNodeDegFree;
    // Number of nodes per element
    unsigned int m_nNodePerElem;
    // Number of degrees of freedom
    unsigned int m_nTotalDoF;
    // Penalty value
    numericType m_penalty;
    const numericType m_penalty_multiplier = 1e20;
    // List of elements
    std::vector<elemClass> m_elemList;
    // List of elements with pressure force applied!
    std::vector<unsigned int> m_elemListWithPressureApplied;
    // List of nodes
    std::vector<nodeClass> m_nodeList;
    // List of fixed nodes
    std::set<unsigned int> m_fixedNodes;
    // Name of the file input
    std::string m_fileInputName;
    // Report status of the linear solver computation
    eigenLinearSolverInfo m_linSolverInfo;
    numericType m_youngModulus;
    numericType m_poissonRatio;
    numericType m_pressure;

    // MEMBERS - vector, matrix
    // Degrees of freedom vector
    Eigen::Matrix<unsigned int,Eigen::Dynamic,1> m_degFreeVec;
    // Global stiffness matrix
    Eigen::SparseMatrix<numericType> m_glbStiffMat;
    // Global force vector
    Eigen::SparseMatrix<numericType> m_glbForceVec;
    // Global displacement vector
    Eigen::SparseMatrix<numericType> m_glbDisplacVec;

    // MEMBERS - boolean
    // Check if initialization info of the elements is saved already
    bool m_isElemListSaved;
    // Check if initialization info of the nodes is saved already
    bool m_isNodeListSaved;
    // Check if initialization info is saved already
    bool m_isInitialStructureSaved;
    // Check if initialization info is saved already
    bool m_isDegreeOfFreedomVecComputed;
    // Check if global stiffness matrix is computed already
    bool m_isGlbStiffMatComputed;
    // Check if global force vector is computed already
    bool m_isGlbForceVecComputed;
    // Check if global displacement vector is computed already
    bool m_isGlbDisplacVecComputed;
    // Check if local element information (force and displacement) is updated already
    bool m_isLocalInfoUpdated;
    // Check if local nodal information (force and displacement) is updated already
    bool m_isLocalNodeInfoUpdated;
};

template<class numericType, class elemClass, class nodeClass>
baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::baseStructuralAnalysisClass():
        m_isElemListSaved(false),m_isNodeListSaved(false),m_isInitialStructureSaved(false),m_isDegreeOfFreedomVecComputed(false), m_isGlbStiffMatComputed(false), m_isGlbForceVecComputed(false), m_isGlbDisplacVecComputed(false), m_isLocalInfoUpdated(false), m_isLocalNodeInfoUpdated(false)
{
    //nodeClass aNode;
    m_maxNodeDegFree = 2;           // MUST BE EDITED TO BE MORE GENERAL
    m_nSpatialDimension = 2;           // MUST BE EDITED TO BE MORE GENERAL
    elemClass anElem;
    //elemClass anElem;
    m_nNodePerElem = anElem.getnumberofNodes();           // MUST BE EDITED TO BE MORE GENERAL
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
    bool flag = m_isInitialStructureSaved;
    unsigned int numOperations = 0;

    // Read file!
    if(!flag) {
        flag = readFormatedFile(m_fileInputName);

    }

    if (flag) {

        ++numOperations;
        // evaluate all Local Rotation Matrix!
        //flag = evaluateLocalRotationMat();
        if (flag) {

            ++numOperations;
            // compute Degrees of Freedom!
            computeDegreesOfFreedom();
            flag = m_isDegreeOfFreedomVecComputed;
            if (flag) {
                ++numOperations;
                // evaluate Global Force Vector!
                evaluateGlobalForceVec();
                flag = m_isGlbForceVecComputed;
                if (flag) {
                    ++numOperations;
                    // evaluate Global Stiffness Matrix!
                    evaluateGlobalStiffMat();
                    flag = m_isGlbStiffMatComputed;
                    if (flag) {
                        ++numOperations;
                        // evaluate Global Displacement Vector (solve problem)!
                        evaluateGlobalDisplacVec();
                        flag = m_isGlbDisplacVecComputed;
                        if (flag) {

                            ++numOperations;
                            // evaluate Local Element Information!
                            flag = updateLocalInfo();
                            if (flag) { ++numOperations; }
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
    bool flagPressure = false;
    bool flagMaterial = false;
    bool flagForceElem = false;
    bool flagFixedNode = false;

    if( !m_isInitialStructureSaved )
    {
        std::string aLine;
        std::string buff;
        std::string laterLine="00000";

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
                if ( (aLine == "*Element, type=CPS4")||(aLine == "*Element, type=CPS6M") )
                {

                    flagElem = readFromFile_ElemList( &inFile);
                }

                // Check if it list Nodes
                if ( aLine == "*Node" )
                {
                    flagNode = readFromFile_NodeList( &inFile);

                }

                // Check if it list Material Data
                if ( aLine == "*Elastic" )
                {
                    getline(inFile,aLine);
                    std::string pieceLine;
                    std::stringstream thisLine (aLine);
                    std::getline(thisLine, pieceLine, ',');
                    m_youngModulus = std::stod(pieceLine);
                    std::getline(thisLine, pieceLine, ',');
                    m_poissonRatio = std::stod(pieceLine);
                    flagMaterial = true;
                }

                // Check if it list Elements with pressure applied in it
                if ( (aLine == "*Elset, elset=_Set_Load_S3, internal, instance=Part_Geometry-1")||
                        (aLine == "*Elset, elset=_Set_Load_S1, internal, instance=Part_Geometry-1"))

                {
                    getline(inFile,aLine);
                    std::string pieceLine;
                    std::stringstream thisLine (aLine);
                    unsigned int curAux=0;
                    while (std::getline(thisLine, pieceLine, ',')){
                        m_elemListWithPressureApplied.push_back(std::stoul(pieceLine)-1u);
                    }
                    flagForceElem = true;
                }

                // Check if it list Elements with pressure applied in it
                if ( aLine == "*Elset, elset=_Set_Load_S3, internal, instance=Part_Geometry-1, generate" )
                {
                    getline(inFile,aLine);
                    std::string pieceLine;
                    std::stringstream thisLine (aLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxBegin = std::stoul(pieceLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxEnd = std::stoul(pieceLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxStep = std::stoul(pieceLine);

                    unsigned int curValue;
                    for(curValue=(auxBegin-1); curValue<auxEnd; curValue+=auxStep) {
                        m_elemListWithPressureApplied.push_back((curValue)-1u);
                    }
                    flagForceElem = true;
                }

                // Check if it list Elements with pressure applied in it
                if ( aLine == "*Dsload" )
                {
                    getline(inFile,aLine);
                    std::string pieceLine;
                    std::stringstream thisLine(aLine);
                    std::getline(thisLine, pieceLine, ',');
                    std::getline(thisLine, pieceLine, ',');
                    std::getline(thisLine, pieceLine, ',');
                    m_pressure = std::stod(pieceLine);

                    flagPressure = true;
                }

                // Check if it list fixed nodes
                if ( aLine == "*Nset, nset=Set_Encastre, instance=Part_Geometry-1, generate" )
                {
                    getline(inFile,aLine);
                    std::string pieceLine;
                    std::stringstream thisLine (aLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxBegin = std::stoul(pieceLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxEnd = std::stoul(pieceLine);

                    std::getline(thisLine, pieceLine, ',');
                    unsigned int auxStep = std::stoul(pieceLine);

                    unsigned int curValue;
                    for(curValue=(auxBegin-1); curValue<auxEnd; curValue+=auxStep) {
                        m_fixedNodes.insert(curValue);
                    }
                    flagFixedNode = true;
                }

                if ( aLine == "*Nset, nset=Set_Encastre, instance=Part_Geometry-1" )
                {
                    getline(inFile,aLine);
                    std::stringstream thisLine (aLine);
                    std::string pieceLine;
                    unsigned int auxNodeId;
                    // Reading id
                    while (std::getline(thisLine, pieceLine, ','))
                    {
                        auxNodeId = std::stoul(pieceLine)-1;
                        m_fixedNodes.insert(auxNodeId);
                    }

                    flagFixedNode = true;
                }
            }
            inFile.close();
        }
    }

    m_isInitialStructureSaved = (m_isInitialStructureSaved) || (flagOpen && flagElem && flagNode && flagPressure && flagMaterial && flagForceElem && flagFixedNode);
    return m_isInitialStructureSaved;
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::readFromFile_ElemList(ifstream *openedFile)
{
    bool flagOpen = false;
    Eigen::Matrix<numericType,2,1> curPos;

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

            getline(*openedFile,aLine);
            while(aLine[0] != '*') {
                std::stringstream thisLine (aLine);
                // Reading id
                std::getline(thisLine, pieceLine, ',');
                // aux_unsigned_int= std::stoul(pieceLine);

                // Reading nodalConnectivity[4]
                std::vector<unsigned int> nodalConnectivity;
                for(curAux = 0; curAux< m_nNodePerElem ; ++curAux)
                {
                    std::getline(thisLine, pieceLine, ',');
                    aux_unsigned_int= (std::stoul(pieceLine)-1u);
                    nodalConnectivity.push_back(aux_unsigned_int);

                }

                // Evaluating nodalPosCoord[4,2]
                Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> nodalPosCoord(m_nNodePerElem,m_nSpatialDimension);
                for(curAux = 0; curAux<m_nNodePerElem; ++curAux)
                {
                    curPos = (m_nodeList[nodalConnectivity[curAux]].getM_position());
                    nodalPosCoord.row(curAux) = curPos.transpose();
                }

                // elementClass_Q4(const std::vector<unsigned int>  &nodalConnectivity, const Eigen::Matrix<numericType,4,2> &nodalPosCoord);
                m_elemList.push_back( elemClass(nodalConnectivity, nodalPosCoord) );

                getline(*openedFile,aLine);
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

            getline(*openedFile,aLine);
            while(aLine[0] != '*') {
                std::stringstream thisLine (aLine);
                // Reading id
                std::getline(thisLine, pieceLine, ',');
                // aux_unsigned_int= std::stoul(pieceLine);

                Eigen::Matrix<numericType,Eigen::Dynamic,1> position(m_nSpatialDimension);
                for(curAux = 0; curAux<m_nSpatialDimension; ++curAux)
                {
                    std::getline(thisLine, pieceLine, ',');
                    position(curAux) = std::stod(pieceLine);
                }

                // baseNodeClass(Eigen::Matrix<numericType,nSpatialDimension,1> position);
                m_nodeList.push_back( nodeClass( position) );

                // Next line
                getline(*openedFile,aLine);
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
        unsigned int firstNonFixed = 0;
        unsigned int nextFixed = 0;
        unsigned int curDegAtNode;

        // Searching all nodes
        for (auto it = m_fixedNodes.begin(); it != m_fixedNodes.end(); ++it) {
            nextFixed = *it;
            for (curNode = firstNonFixed; curNode < nextFixed; ++curNode) {
                // Searching all "DoF" at current node
                for (curDegAtNode = 0; curDegAtNode < numNodeDegFree; ++curDegAtNode) {
                    // If it is an actual free degree, update the number of actual DoF and save it
                    degreesFreedomVec(curDegAtNode + curNode * numNodeDegFree) = ++actualDegreeFreedom;
                }
            }
            m_nTotalDoF = actualDegreeFreedom;
            m_degFreeVec = degreesFreedomVec;

            firstNonFixed = nextFixed+1;
        }
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
    unsigned int idNode_curElem;                    // Initial node of an element

    // Global Stiffness Matrix
    Eigen::SparseMatrix<numericType> glbStiffMatrix(numNode*numNodeDegFree,numNode*numNodeDegFree);

    if( (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed ) && (!m_isGlbStiffMatComputed) )
    {
        // Getting some small vectors and matrix
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemDofPosition_curElem(numElemDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> iniNodeDofPosition_curElem(numNodeDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> endNodeDofPosition_curElem(numNodeDegFree);
        std::vector<unsigned int> elemConnection_curElem(numNodePerElem);       // ConnectionVector of an element
        Eigen::SparseMatrix<numericType> localStifffMat_CurElem(numElemDegFree,numElemDegFree);  // localStiffMatrix of an element
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
            elemConnection_curElem = m_elemList[idCurElem].getM_nodalConnectivity();

            // Getting the 'position' of the DoF of the nodes that connects an element
            for(idNode_curElem = 0; idNode_curElem < numNodePerElem; ++idNode_curElem){
                elemDofPosition_curElem.block(idNode_curElem*numNodeDegFree,0,numNodeDegFree,1) = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,elemConnection_curElem[idNode_curElem]*numNodeDegFree,(elemConnection_curElem[idNode_curElem]+1u)*numNodeDegFree - 1u);
            }

            // Computing the local stiffness matrix of an element and its rotation matrix
            localStifffMat_CurElem = m_elemList[idCurElem].sprMatK(m_youngModulus, m_poissonRatio);

            // Walking inside the localStiffMatrix: row by row
            for( curRow_LclStfMat = 0; curRow_LclStfMat<numElemDegFree ; ++curRow_LclStfMat)
            {
                glbIdRow = elemDofPosition_curElem(curRow_LclStfMat);

                // Walking inside the localStiffMatrix: col by col
                for( curCol_LclStfMat = 0; curCol_LclStfMat<numElemDegFree ; ++curCol_LclStfMat)
                {
                    glbIdCol = elemDofPosition_curElem(curCol_LclStfMat);

                    // Getting the stiffness of localStiffMatrix roxXcol and building the triplet
                    stiff = localStifffMat_CurElem.coeffRef(curRow_LclStfMat, curCol_LclStfMat);
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
    m_isGlbStiffMatComputed = (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed );

    // Returning suitable value
    if(m_isGlbStiffMatComputed)   { return m_glbStiffMat;   }
    else                            return glbStiffMatrix;
}

template<class numericType, class elemClass, class nodeClass>
Eigen::SparseMatrix<numericType> baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::evaluateGlobalForceVec(){

    // Getting constantes and some auxiliar variables
    unsigned int numNodePerElem = m_nNodePerElem;   // Number of nodes per element [USING =2]!!!!!!!!!!!!!
    unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
    unsigned int numElemDegFree = numNodePerElem*numNodeDegFree;// Number of possible degrees of freedom in each elem
    unsigned int numElemWithPressure = m_elemListWithPressureApplied.size();       // Number of elements
    unsigned int numNode = m_nodeList.size();       // Number of nodes
    unsigned int idCurElemWithPressure;                         // Index of an element
    unsigned int idNode_curElem;                    // Initial node of an element

    // Global Stiffness Matrix
    Eigen::SparseMatrix<numericType> forceVec(numNode*numNodeDegFree,1);

    if( (m_isInitialStructureSaved && m_isDegreeOfFreedomVecComputed ) && (!m_isGlbForceVecComputed) )
    {
        // Getting some small vectors and matrix
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemDofPosition_curElem(numElemDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> iniNodeDofPosition_curElem(numNodeDegFree);
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> endNodeDofPosition_curElem(numNodeDegFree);
        std::vector<unsigned int> elemConnection_curElem(numNodePerElem);       // ConnectionVector of an element
        Eigen::SparseMatrix<numericType> localStifffMat_CurElem(numElemDegFree,numElemDegFree);  // localStiffMatrix of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> rotationMat_CurElem(numElemDegFree,numElemDegFree);     // rotationMatrix of an element
        typedef Eigen::Triplet<numericType> TripNumericType;                // Triplet typedef for a sparse matrix
        std::vector<TripNumericType> tripletList;                           // Triplet list for indexzation of sparse matrix
        tripletList.reserve(numNodePerElem*numElemWithPressure);         // Allocating memory for the triplet list
        unsigned int curRow_LclForcVec =0;                                   // Index of a row at localStiffMatrix
        //    unsigned int curRowDof_LclStfMat =0;                              // DoF of the IndexRow @ localStiffMatrix
        //    unsigned int curColDof_LclStfMat =0;                              // DoF of the IndexCol @ localStiffMatrix
        unsigned int glbIdRow = 0;                                          // Index of the row @ globalStiffMatrix
        unsigned int glbIdCol = 0;                                          // Index of the col @ globalStiffMatrix
        Eigen::Matrix<numericType, 2, 1>  curEdge;
        numericType edge_size;
        numericType force;                                                  // An element of the localStiffMatrix
        numericType maxValue = 0;                                           // Variable to compute the max value of localStiffMatrix


        // Trick...
        // Walking inside the elementList: elem by elem
        for( idCurElemWithPressure=0; idCurElemWithPressure<numElemWithPressure; ++idCurElemWithPressure)
        {
            // Saving the connection (id nodes) of an element
            unsigned int teste1 = idCurElemWithPressure;
            unsigned int teste2 = m_elemListWithPressureApplied[idCurElemWithPressure];
            elemConnection_curElem = m_elemList[m_elemListWithPressureApplied[idCurElemWithPressure]].getM_nodalConnectivity();
            //get Element Edge
            curEdge = m_nodeList[elemConnection_curElem[1]].getM_position();
            curEdge -=m_nodeList[elemConnection_curElem[2]].getM_position();

            edge_size = curEdge.norm();

            // Walking inside the localStiffMatrix: row by row
            for( idNode_curElem = int(numNodePerElem/2); idNode_curElem<numNodePerElem ; ++idNode_curElem)
            {
                    tripletList.push_back( TripNumericType( elemConnection_curElem[idNode_curElem]*numNodeDegFree+1 , 0 , -m_pressure* edge_size/2) );
            }
        }

        // Saving global stiffness matrix as sparse (from triplets(i,j,v_ij))
        forceVec.setFromTriplets(tripletList.begin(), tripletList.end());
        m_glbForceVec = forceVec;

        m_isGlbForceVecComputed = true;
    }

    return forceVec;
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
    unsigned int idCurFixNode;                         // Index of an DoF
    bool isFixed;

    if( m_isGlbStiffMatComputed && m_isGlbForceVecComputed)
    {
        // Global Displacement Vector
        Eigen::SparseMatrix<numericType> displacVector(numDof,1);

        // Getting global force vector and stiffness matrix
        Eigen::SparseMatrix<numericType> penalizedStiffMatrix = m_glbStiffMat;
        Eigen::SparseMatrix<numericType> penalizedForceVector = m_glbForceVec;
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> dofPosition_curNode(numNodeDegFree);

        for (std::set<unsigned int>::iterator it=m_fixedNodes.begin(); it!=m_fixedNodes.end(); ++it){
            idCurFixNode = *it;

            penalizedStiffMatrix.coeffRef(idCurFixNode*numNodeDegFree,idCurFixNode*numNodeDegFree) = m_penalty;
            penalizedStiffMatrix.coeffRef(idCurFixNode*numNodeDegFree+1,idCurFixNode*numNodeDegFree+1) = m_penalty;

            penalizedForceVector.coeffRef(idCurFixNode*numNodeDegFree,0) = 0;
            penalizedForceVector.coeffRef(idCurFixNode*numNodeDegFree+1,0) = 0;
        }

//        std::cout << "GLOBAL FORCE VECTOR" << std::endl << m_glbForceVec.transpose() << std::endl << std::endl;
//        std::cout << "GLOBAL STIFF MATRIX" << std::endl << m_glbStiffMat << std::endl << std::endl;
//        std::cout << "reduced force vector" << std::endl << penalizedForceVector.transpose() << std::endl << std::endl;
//        std::cout << "reduced stiff matrix" << std::endl << penalizedStiffMatrix << std::endl << std::endl;

        // Solving by conjugate gradiente method
        Eigen::SparseLU<SparseMatrix <numericType>> cgSolver;
//        cgSolver.setTolerance(1e-3);
//        cgSolver.setMaxIterations(100000);
        cgSolver.compute(penalizedStiffMatrix);
        displacVector = cgSolver.solve(penalizedForceVector);
//        m_linSolverInfo.setValues (cgSolver.iterations(),cgSolver.error(),cgSolver.info());
//        std::cout << " Displacements Nodes Vector" << endl << displacVector << endl << endl;


        if(m_linSolverInfo.info == Eigen::Success)
        {

            isSolverSuccess = true;
            m_glbForceVec = m_glbStiffMat * displacVector;      // Updating global force vector!
            m_glbDisplacVec = displacVector;
        }
    }

    std::cout << " Displacements Nodes Vector" << endl << m_glbDisplacVec << endl << endl;
    // Updating flag
    m_isGlbDisplacVecComputed = (( m_isGlbStiffMatComputed && m_isGlbForceVecComputed) && isSolverSuccess);

    // Returning suitable value
    if(m_isGlbDisplacVecComputed)   {   return m_glbDisplacVec; }
    else                                return Eigen::Matrix<numericType,Eigen::Dynamic,1>::Zero(numDof);
}

template<class numericType, class elemClass, class nodeClass>
bool baseStructuralAnalysisClass<numericType,elemClass,nodeClass>
::updateLocalInfo() {

    if((m_isGlbDisplacVecComputed && m_isGlbForceVecComputed && m_isGlbStiffMatComputed) && !m_isLocalInfoUpdated)
    {
        // Getting constantes and some auxiliar variables
        unsigned int numNodePerElem = m_nNodePerElem;   // Number of nodes per element [USING =2]!!!!!!!!!!!!!
        unsigned int numNodeDegFree = m_maxNodeDegFree; // Number of possible degrees of freedom in each node
        unsigned int numElemDegFree = numNodePerElem*numNodeDegFree;// Number of possible degrees of freedom in each elem
        unsigned int numElem = m_elemList.size();       // Number of elements
        unsigned int numNode = m_nodeList.size();       // Number of elements
        unsigned int idCurElem;                         // Index of an element
        unsigned int idNode;
        unsigned int idNode_curElem;                    // Initial node of an element
        unsigned int curIdDof;


        // Getting some small vectors and matrix
        Eigen::Matrix<unsigned int,Eigen::Dynamic,1> elemDofPosition_curElem(numElemDegFree);
        std::vector<unsigned int> elemConnection_curElem;       // ConnectionVector of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> rotationMat_CurElem;     // RotationMatrix of an element
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    nodeForceVec_curElem(numElemDegFree);           // nodeForceVector of the iniNode
//        Eigen::Matrix<numericType,Eigen::Dynamic,1>    internForceVec_curElem(numNodeDegFree);           // nodeForceVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    posVec_iniNode_curElem;     // PositionVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    posVec_endNode_curElem;     // PositionVector of the endNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    dispVec_iniNode_curElem;    // DisplacementVector of the iniNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    dispVec_endNode_curElem;    // DisplacementVector of the endNode
        Eigen::Matrix<numericType,Eigen::Dynamic,1>    tempVec;                    // Temporary vector
        numericType iniLength;

        Eigen::SparseMatrix<numericType> glbDispVec = m_glbDisplacVec;
        Eigen::SparseMatrix<numericType> lclDispVec(numElemDegFree,1);
        Eigen::Matrix<numericType,3,1> lclStress;
        Eigen::Matrix<numericType,3,1> lclStrain;

        Eigen::Matrix<unsigned int,Eigen::Dynamic,Eigen::Dynamic> counterNodes = Eigen::Matrix<unsigned int,Eigen::Dynamic,Eigen::Dynamic>::Zero(numNode,1);
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> lclStressNodes = Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic>::Zero(3,numNode);
        Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic> lclStrainNodes = Eigen::Matrix<numericType,Eigen::Dynamic,Eigen::Dynamic>::Zero(3,numNode);


        // Walking inside the elementList: elem by elem
        for( idCurElem=0; idCurElem<numElem; ++idCurElem)
        {
            // Saving the connection (id nodes) of an element
            elemConnection_curElem = m_elemList[idCurElem].getM_nodalConnectivity();

            // Getting the 'position' of the DoF of the nodes that connects an element
            for(idNode_curElem = 0; idNode_curElem < numNodePerElem; ++idNode_curElem){
                elemDofPosition_curElem.block(idNode_curElem*numNodeDegFree,0,numNodeDegFree,1) = Matrix<unsigned int,Eigen::Dynamic,1>::LinSpaced(numNodeDegFree,elemConnection_curElem[idNode_curElem]*numNodeDegFree,(elemConnection_curElem[idNode_curElem]+1u)*numNodeDegFree - 1u);
            }

            // Getting the local displacement vector of the curElem
            for(curIdDof = 0; curIdDof < numElemDegFree; ++curIdDof){
                lclDispVec.coeffRef(curIdDof,0) = glbDispVec.coeffRef(elemDofPosition_curElem(curIdDof,0),0);
            }

            // Evaluating the curElem displacement (SHOULD BE THE REDUCED GAUSSIAN INTEGRATION POINT. FOR THIS PROJECT U MAY USE (0,0))
            m_elemList[idCurElem].evaluateStressStrain(lclDispVec, m_youngModulus, m_poissonRatio);

            // Getting the local strain vector
            lclStrain = m_elemList[idCurElem].get_eVec();

            // Getting the local stress vector
            lclStress = m_elemList[idCurElem].get_sigVec();

            // Walking inside the nodes of the element
            for(idNode_curElem = 0; idNode_curElem < numNodePerElem; ++idNode_curElem){
                idNode = elemConnection_curElem[idNode_curElem];
                lclStressNodes.col(idNode)  += lclStress;
                lclStrainNodes.col(idNode)  += lclStrain;
                counterNodes(idNode,0)      += 1;
            }
        }

        lclStressNodes.transposeInPlace();
        lclStrainNodes.transposeInPlace();

        // Walking inside the nodeList: node by node
        for( idNode=0; idNode<numNode; ++idNode) {
            lclStressNodes.row(idNode) /= counterNodes(idNode,0);
            lclStrainNodes.row(idNode) /= counterNodes(idNode,0);

            m_nodeList[idNode].set_eVec(lclStressNodes.row(idNode));
            m_nodeList[idNode].set_sigVec(lclStrainNodes.row(idNode));
        }

        std::cout << " Stress Nodes Matrix" << endl << lclStressNodes << endl << endl;
        std::cout << " Strain Nodes Matrix" << endl << lclStrainNodes << endl << endl;
    }


    m_isLocalInfoUpdated = m_isLocalNodeInfoUpdated;
    return m_isLocalInfoUpdated;
};

#endif //FEMPROJECT_BASESTRUCTURALANALYSISCLASS_HPP
