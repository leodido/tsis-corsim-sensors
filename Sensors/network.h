/*****************************************************************************************
    Copyright 2013 Leonardo Di Donato

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and 
    limitations under the License.

    Application: tsis-corsim-sensors

    Description:
    Class that represents and encapsulates the network.
*****************************************************************************************/

#ifndef CNETWORK
#define CNETWORK

#pragma once

#include <string>

class CDetector;
class CLink;
class CNode;

class CNetwork : public CObject
{
    public:
        enum OutputProcessor {
			ALL			= 0,
			DETECTORS	= 1,
			NODES		= 2,
			LINKS		= 3,
			LANES		= 4,
			NO			= 5
		};

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