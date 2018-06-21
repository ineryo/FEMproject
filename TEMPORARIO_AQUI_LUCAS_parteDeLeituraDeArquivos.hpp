template <class numericType,class elemClass,class nodeClass>
class inBaseStructuralAnalysis
{
public:

    // VIRTUAL METHODS
    virtual bool readFormatedFile(std::string fileName);
    virtual bool readFromFile_ElemList(std::ifstream* opennedFile, unsigned int numElem);
    virtual bool readFromFile_NodeList(std::ifstream* opennedFile, unsigned int numNode);
}




template<class numericType, class elemClass, class nodeClass>
bool inBaseStructuralAnalysis<numericType,elemClass,nodeClass>
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
                if(aLine[0] == '%') continue;

                // Check if it list Elements
                if ( aLine.find("#Element:")!=std::string::npos )
                {
                    std::stringstream thisLine (aLine);
                    thisLine >> buff >> numElem;
                    m_elemList.reserve(numElem);
                    flagElem = readFromFile_ElemList( &inFile, numElem);
                }

                // Check if it list Nodes
                if ( aLine.find("#Node:")!=std::string::npos )
                {
                    std::stringstream thisLine (aLine);
                    thisLine >> buff >> numNode;
                    m_nodeList.reserve(numNode);
                    flagNode = readFromFile_NodeList( &inFile, numNode);
                }
            }
            inFile.close();
        }
    }

    m_isInitialStructureSaved = (m_isInitialStructureSaved) || (flagOpen && flagElem && flagNode);
    return m_isInitialStructureSaved;
}






template<class numericType, class elemClass, class nodeClass>
bool inBaseStructuralAnalysis<numericType,elemClass,nodeClass>
::readFromFile_ElemList(ifstream *opennedFile, unsigned int numElem)
{
    bool flagOpen = false;

    if( (!m_isInitialStructureSaved) && (!m_isElemListSaved) )
    {
        std::string aLine;
        unsigned int numNodePerElem = m_nNodePerElem;
        unsigned int numPropPerElem = m_nPropPerElem;

        if (opennedFile->is_open())
        {
            flagOpen = true;
            bool aux_bool;
            numericType aux_numericType;
            unsigned int aux_unsigned_int;

            unsigned int curElem;
            unsigned int curAux;
            for(curElem = 0; curElem<numElem; ++curElem )
            {
                if( getline(*opennedFile,aLine) )
                {
                    std::stringstream thisLine (aLine);
                    // Reading id
                    thisLine >> aux_unsigned_int;

                    // Reading connection_vector[numNodePerElem]= node_id
                    Eigen::Matrix<unsigned int,Eigen::Dynamic,1> aConnectionVector(numNodePerElem);
                    for(curAux = 0; curAux<numNodePerElem ; ++curAux)
                    {
                        thisLine >> aux_unsigned_int;
                        aConnectionVector[curAux] = aux_unsigned_int;
                    }

                    // Reading isLattice_vector[numNodePerElem]= isLattice?
                    Eigen::Matrix<bool,Eigen::Dynamic,1> aLatticeVector(numNodePerElem);
                    for(curAux = 0; curAux<numNodePerElem ; ++curAux)
                    {
                        thisLine >> aux_bool;
                        aLatticeVector[curAux] = aux_bool;
                    }

                    // Reading property_vector[nProperties]= property
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> aPropertyVector(numPropPerElem);
                    for(curAux = 0; curAux<numPropPerElem ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        aPropertyVector[curAux] = aux_numericType;
                    }
                    m_elemList.push_back( elemClass( aConnectionVector, aLatticeVector, aPropertyVector) );
                }
            }
        }
    }
    m_isElemListSaved = (m_isInitialStructureSaved) || (flagOpen);
    return m_isElemListSaved;
}







template<class numericType, class elemClass, class nodeClass>
bool inBaseStructuralAnalysis<numericType,elemClass,nodeClass>
::readFromFile_NodeList(ifstream *opennedFile, unsigned int numNode)
{
    bool flagOpen = false;

    if( (!m_isInitialStructureSaved) && (!m_isNodeListSaved) )
    {
        std::string aLine;
        unsigned int numDim = m_nDim;
        unsigned int numMaxDegFree = m_maxNodeDegFree;

        if (opennedFile->is_open())
        {
            flagOpen = true;
            unsigned int aux_unsigned_int;
            numericType aux_numericType;

            unsigned int curNode;
            unsigned int curAux;
            for(curNode = 0; curNode<numNode; ++curNode )
            {
                if( getline(*opennedFile,aLine) )
                {
                    std::stringstream thisLine (aLine);
                    // Reading id
                    thisLine >> aux_unsigned_int;

                    // Reading position_vector[numDim]= Xi
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> aPositionVector(numDim);
                    for(curAux = 0; curAux<numDim ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        aPositionVector[curAux] = aux_numericType;
                    }

                    // Reading isFixed_vector[numMaxDegFree]= isFixed?
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> isFixedVector(numMaxDegFree);  //isFixed is not a bool, it can be [0,1] to get %
                    for(curAux = 0; curAux<numMaxDegFree ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        isFixedVector[curAux] = aux_numericType;
                    }

                    // Reading displacement_vector[numMaxDegFree]= displacement
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> aDisplacementVector(numMaxDegFree);
                    for(curAux = 0; curAux<numMaxDegFree ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        aDisplacementVector[curAux] = aux_numericType;
                    }

                    // Reading force_vector[numMaxDegFree]= force
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> anodeForceVector(numMaxDegFree);
                    for(curAux = 0; curAux<numMaxDegFree ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        anodeForceVector[curAux] = aux_numericType;
                    }

                    // Reading spring_vector[numMaxDegFree]= springStiff
                    Eigen::Matrix<numericType,Eigen::Dynamic,1> aSpringVector(numMaxDegFree);
                    for(curAux = 0; curAux<numMaxDegFree ; ++curAux)
                    {
                        thisLine >> aux_numericType;
                        aSpringVector[curAux] = aux_numericType;
                    }
                    m_nodeList.push_back( nodeClass( aPositionVector, aDisplacementVector, isFixedVector, aSpringVector, anodeForceVector) );
                }
            }
        }
    }
    m_isNodeListSaved = (m_isInitialStructureSaved) || (flagOpen);
    return m_isNodeListSaved;
}