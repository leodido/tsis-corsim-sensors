#ifndef CNETWORK
#define CNETWORK

#include <string>

class CDetector;
class CLink;
class CNode;

class CNetwork : public CObject
{
    public:
        enum OutputProcessor { ALL = 0, DETECTORS = 1, NODES = 2, LINKS = 3, LANES = 4, NO = 5 };

        CNetwork();
        explicit CNetwork(const CString& input_traf_file_name);
        virtual ~CNetwork();

        inline CString                  getInputFile(void) const { return m_traf_input_file; }
        inline void                     setInputFile(const CString& name) { m_traf_input_file = name; }
        void                            readInputFile(void);

        void                            getNodes(FILE* file);
        CNode*                          findNode(int id);
        void                            printNodes(void);

        void                            getLinks(FILE* file);
        CLink*                          findLink(int up, int dn);
        int                             getLinkCorsimId(int unode, int dnnode);
        void                            printLinks(void);

        void                            createLanes(FILE* file);
        void                            printLanes(void);

        void                            getDetectors(FILE* file);
        void                            setDetectorCorsimId(CDetector* detector);
        void                            processDetectors(void);
        void                            printDetectorsCount(void);
        void                            printDetectorsTransitions(void);

        void                            setupOutputProcessor(OutputProcessor type);
        void                            writeOutput(void);

    private:
        // private methods
        int                             readTRFLine(FILE* file, char* line);
        std::string                     writeDetectorsOutput();    

        // private members
        CString                         m_network_name;
        CString                         m_traf_input_file;
        CString                         m_sensors_output_file;
        OutputProcessor                 m_out_type;
        CTypedPtrList<CPtrList, CLink*> m_link_list;
        CTypedPtrList<CPtrList, CNode*> m_node_list;
};

#endif // CNETWORK