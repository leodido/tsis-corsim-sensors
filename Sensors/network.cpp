#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <iomanip>

#include "corsim.h"
#include "netsim.h"
#include "corwin.h"

#include "stdafx.h"
#include "network.h"
#include "node.h"
#include "link.h"
#include "lane.h"
#include "detector.h"
#include "commons.h"

CNetwork::CNetwork() : CObject()
                     , m_network_name("")
                     , m_traf_input_file("")
                     , m_sensors_output_file("")
                     , m_out_type(NO)
                     , m_link_list()
                     , m_node_list()
{
   // default ctor
}

CNetwork::CNetwork(const CString& input_traf_file_name)
                  : CObject()
                  , m_network_name("")
                  , m_traf_input_file(input_traf_file_name)
                  , m_sensors_output_file("")
                  , m_out_type(NO)
                  , m_link_list()
                  , m_node_list()
{
    char buff[DTTMSZ];
    m_sensors_output_file  = m_traf_input_file;
    m_sensors_output_file += _T(getDateTime(buff));
}

CNetwork::~CNetwork()
{
    POSITION pos = NULL;
    CNode* node = NULL;
    CLink* link = NULL;
    // delete the link list
    pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {
        link = m_link_list.GetNext(pos);
        delete link;
    }
    m_link_list.RemoveAll();
    // delete the node list
    pos = m_node_list.GetHeadPosition();
    while (pos != NULL)
    {
        node = m_node_list.GetNext(pos);
        delete node;
    }
    m_node_list.RemoveAll();
}

void CNetwork::setupOutputProcessor(OutputProcessor type)
{
    switch(type)
    {
        case DETECTORS:
            m_sensors_output_file += _T("_sensors");
            break;
        case LINKS:
            m_sensors_output_file += _T("_links");
            break;
        case NODES:
            m_sensors_output_file += _T("_nodes");
            break;
        case LANES:
            m_sensors_output_file += _T("_lanes");
            break;
        case ALL:
            m_sensors_output_file += _T("_complete_report");
            break;
        default:
            break;
    }
    if (type != NO)
        m_sensors_output_file += _T(".csv");
    m_out_type = type;

    sprintf(out_buf, "Output file: \"%s\".\n", m_sensors_output_file);
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
}

void CNetwork::writeOutput(void)
{
    if (m_out_type != NULL)
    {
        std::ofstream output_stream;
        output_stream.open((LPCTSTR) m_sensors_output_file);
        switch(m_out_type)
        {
            case DETECTORS:
                output_stream << writeDetectorsOutput() << std::endl;
                break;
        }
        output_stream.close();
    }
}

std::string CNetwork::writeDetectorsOutput(void)
{
    std::vector<std::string> rows;
    int row = 0;
    int num_det = 0;
    // loop through the links
    CLink* link = NULL;
    POSITION pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {
        link = m_link_list.GetNext(pos);
        // loop through the detectors
        POSITION det_pos = link->m_detector_list.GetHeadPosition();
        while (det_pos != NULL)
        {
            CDetector* detector = link->m_detector_list.GetNext(det_pos);
            // create the header
            std::stringstream header;
            if (num_det == 0)
            {
                header << "time" << ", " << detector->getLabel();
                rows.push_back(header.str());
            }
            else
            {
                header << rows.at(0) << ", " << detector->getLabel();
                rows[0] = header.str();
            }
            // loop through the detector transitions
            std::vector<std::string> transitions = detector->getTransitions();
            for (unsigned i = 0; i < transitions.size(); i++) {
                std::stringstream t_row;
                if (num_det == 0)
                {
                    std::stringstream time;
                    time << setprecision(4) << setw(4) << ((double) (i + 1) / 10);
                    t_row << time.str() << ", " << transitions.at(i);
                    rows.push_back(t_row.str());
                }
                else if (num_det > 0)
                {
                    t_row << rows.at(i + 1) << ", " << transitions.at(i);
                    rows[i + 1] = t_row.str();
                }                
            }
            num_det += 1;
        }
        row += 1;
    }
    std::ostringstream out;
    std::copy(rows.begin(), rows.end(), std::ostream_iterator<std::string>(out, "\n"));

    return out.str();
}

void CNetwork::readInputFile()
{
    // open the TRAF file
    FILE* file_trf = NULL;
    if (file_trf = fopen(m_traf_input_file, "r"))
    {
        sprintf(out_buf, "Opened file \"%s\".\n", m_traf_input_file);
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
        // create the node list
        getNodes(file_trf);
        rewind(file_trf);
        // create the link list
        getLinks(file_trf);
        rewind(file_trf);
        // find the opposing link for each link, if one exists
        int up = 0;
        int down = 0;
        CLink* link = NULL;
        POSITION pos = m_link_list.GetHeadPosition();
        while (pos != NULL)
        {
            link = m_link_list.GetNext(pos);
            up = link->getOpposingNodeId();
            down = link->m_dn_node->getId();
            CLink* opposing = NULL;
            opposing = findLink(up, down);
            link->setOpposingLink(opposing);
        }
        // create the lanes on each link
        createLanes(file_trf);
        rewind(file_trf);
        // NOTE: create here the signal timings for the nodes to be controlled by the algorithm, if you want
        // create the detectors that exist in the TRAF file
        getDetectors(file_trf);
        // close the stream
        fclose(file_trf);
    }
}

int CNetwork::readTRFLine(FILE* file, char* line)
{
    // NOTE: to use this function properly, the caller must ensure that line is allocated with at least 81 character
    int card_type = -1; // card type -1 = failed to read line
    char last_char; // used to store last character of the line
    char str_discard[81]; // used to discard characters beyond column 80
    // clear the line (fill with nulls)
    // this is necessary for the card type extraction to work properly for lines that are shorter than 80 characters
    memset(line, 0, 81);
    // read an entire line (up to 80 characters) until a newline character is encountered
    // if fgets encounters a newline character, it places the newline character into the buffer
    if (fgets(line, 81, file) != NULL)
    {
        // if a newline character was not encountered, continue to read and discard bytes until a newline is encountered
        last_char = line[strlen(line) - 1];
        while (last_char != '\n')
        {
            if (fgets(str_discard, 81, file) != NULL)
                last_char = str_discard[strlen(str_discard) - 1];
            else
                break;
        }
        // make sure the line is null terminated
        line[80] = '\0';
        // extract the card type from the line and return
        sscanf_s(line + 77, "%d", &card_type);
        // ensure the card type is in the proper range, if not, return an error code
        if (card_type < 0 || 210 < card_type)
            card_type = -1;
    }
    else
    {
        // error reading the file
        if (ferror(file))
        {
            line[0] = '\0';
            card_type = -1;
        }
    }

    return card_type;
}

void CNetwork::getNodes(FILE* file)
{
    sprintf(out_buf, "Parsing %s ...", "nodes");
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    // get the node information from the TRAF file and create the node list
    char line[81] = { '\0' };
    int card_type = 0;
    CNode* node = NULL;
    POSITION pos = NULL;
    int  node_id = 0;
    char node_ids[5] = { '\0' };
    char control_type[2] = { '\0' };
    char x_pos[7] = { '\0' };
    char y_pos[7] = { '\0' };
    // read data records from the TRAF file
    while (!feof(file))
    {
        // read a line of the file
        card_type = readTRFLine(file, line);
        // parse the line based on its card type
        if (card_type == 36 || card_type == 43)
        {
            // first, extract the node id
            strncpy_s(node_ids, line, 4);
            node_ids[4] = '\0';
            node_id = atoi(node_ids);
            // second, determine if this node is under external control
            strncpy_s(control_type, line + 76, 1);
            control_type[1] = '\0';
            if (node_id < 9000)
            {
                // create a node object
                if (node_id > 8000)
                {
                    node = new CNode(node_id, "source"); // source node
                }
                else
                {
                    if ((control_type[0] == '2') || (control_type[0] == '3'))
                    {
                        // node is controlled by algorithm external to corsim
                        node = new CNode(node_id, "external");
                    }
                    else
                    {
                        // node is controlled by corsim
                        node = new CNode(node_id, "corsim");
                    }
                }
                m_node_list.AddTail(node);
            }
        }
        else if (card_type == 195)
        {
            // first, get the node id
            strncpy_s(node_ids, line, 4);
            node_ids[4] = '\0';
            node_id = atoi(node_ids);
            // second, extract the node coordinates
            strncpy_s(x_pos, line + 6, 6);
            x_pos[6] = '\0';
            strncpy_s(y_pos, line + 14, 6);
            y_pos[6] = '\0';
            // search the node list for the corresponding node object and set its node coordinates
            pos = m_node_list.GetHeadPosition();
            while (pos != NULL)
            {
                node = m_node_list.GetNext(pos);
                if (node->getId() == node_id)
                {
                    node->setLocation(atoi(x_pos), atoi(y_pos));
                }
            }
        }
    }
    // assign the corsim internal id to each node
    int inode_id = 0;
    bool found;
    pos = m_node_list.GetHeadPosition();
    while (pos != NULL)
    {
      inode_id = 0;
      found = false;
      node = m_node_list.GetNext(pos);
      while ((inode_id < IMXNOD) && (!found))
      {
         if (node->getId() == net_node_num[inode_id])
         {
            found = true;
            node->setCorsimId(inode_id);
         }
         inode_id++;
      }
   }
}

void CNetwork::printNodes(void)
{
    CNode* node = NULL;
    POSITION pos = m_node_list.GetHeadPosition();
    while (pos != NULL)
    {      
	    node = m_node_list.GetNext(pos);
        node->print();
    } 
}

// FIXME: lancia eccezione
void CNetwork::printLinks(void)
{
    CLink* link = NULL;
    POSITION pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {      
	    link = m_link_list.GetNext(pos);
        link->print();
    } 
}

void CNetwork::getLinks(FILE* file)
{
    // get the information about the links in the TRAF file and create the link list
    char line[81] = { '\0' };
    int card_type = 0;
    int index = 0;
    CLink* link = NULL;
    // link data
    char up[5] = { '\0' };
    char dn[5] = { '\0' };
    char th[5] = { '\0' };
    char le[5] = { '\0' };
    char ri[5] = { '\0' };
    char op[5] = { '\0' };
    char length[5] = { '\0' };
    char speed[5] = { '\0' };
    char left_bays_len[5] = { '\0' };
    char right_bays_len[5] = { '\0' };
    char full_lanes_num[5] = { '\0' };
    char left_bays_num[5] = { '\0' };
    char right_bays_num[5] = { '\0' };
    char ch[net_max_lanes][5] = { '\0' };
    // search the TRAF file for link data records
    while (!feof(file))
    {
        // read a line of the file
        card_type = readTRFLine(file, line);
        // parse the type 11 (link data) cards
        if (card_type == 11)
        {
            // initialize all the character data strings
            for (index = 0; index < 5; index++)
            {
                up[index] = '\0';
                dn[index] = '\0';
                th[index] = '\0';
                le[index] = '\0';
                ri[index] = '\0';
                op[index] = '\0';
                length[index] = '\0';
                speed[index] = '\0';
                left_bays_len[index] = '\0';
                right_bays_len[index] = '\0';
                full_lanes_num[index] = '\0';
                left_bays_num[index] = '\0';
                right_bays_num[index] = '\0';
                // iterate ch with net_max_lanes
                for (int li = 0; li < net_max_lanes; li++)
                {
                    ch[li][index] = '\0';
                }
            }
            // parse the line, which contains: the upstream node number, the downstream node number, the link length, etc. - see card 11
            for (index = 0; index < 4; index++)
            {
                up[index] = line[index];
                dn[index] = line[index + 4];
                length[index] = line[index + 8];
                left_bays_len[index] = line[index + 12];
                right_bays_len[index] = line[index + 16];
                th[index] = line[index + 40];
                le[index] = line[index + 36];
                ri[index] = line[index + 44];
                op[index] = line[index + 52];
                speed[index] = line[index + 64];
            }
            // read the channelization codes for each lane
            for (index = 0; index < net_max_lanes; index++)
            {
                ch[index][1] = line[index + 29];
            }
            full_lanes_num[0] = line[21];
            left_bays_num[0] = line[23];
            right_bays_num[0] = line[25];
            if ((atoi(dn) < 8000) && (atoi(up) < 8000))
            {
                // link is not a source link so create it
                link = new CLink();
                link->m_full_lanes_num = atoi(full_lanes_num);
                link->m_left_turn_bays_num = atoi(left_bays_num);
                link->m_left_bays_len = atoi(left_bays_len);
                link->m_right_turn_bays_num = atoi(right_bays_num);
                link->m_right_bays_len = atoi(right_bays_len);
                link->m_length = atoi(length);
                link->m_free_flow_speed = atoi(speed);
                if (link->m_free_flow_speed == 0)
                   link->m_free_flow_speed = 44;
                link->m_up_node = findNode(atoi(up));
                link->m_dn_node = findNode(atoi(dn));
                link->m_thru_node = findNode(atoi(th));
                link->m_left_node = findNode(atoi(le));
                link->m_right_node = findNode(atoi(ri));
                link->m_corsim_id = getLinkCorsimId(link->m_up_node->getId(), link->m_dn_node->getId());
                link->setOpposingNodeId(atoi(op));
                if ((atoi(up) < 8000) && (atoi(dn) < 8000))
                    link->m_travel_time = link->computeTravelTime();
                for (index = 0; index < net_max_lanes; index++)
                {
                    link->m_str_channel_code[index] = _T(ch[index][1]);
                }
                m_link_list.AddTail(link);
         }
      }
      else if (card_type > 11)
      {
         // because corsim expects the cards to be in ascending order by card type, we can jump out of the loop as soon as we encounter a card type greater than 11
         break;
      }
   }
}

CLink* CNetwork::findLink(int up, int dn)
{
    CLink* link = NULL;
    // finds the link between nodes (up, dn)
    bool found = false;
    POSITION pos_link = m_link_list.GetHeadPosition();
    while ((pos_link != NULL) && (!found))
    {
        link = m_link_list.GetNext(pos_link);
        found = (link->m_up_node->getId() == up) && (link->m_dn_node->getId() == dn);
    }
    if (found)
        return link;
    else
        return NULL;
}

CNode* CNetwork::findNode(int id)
{
    // find the node id in the node list
    bool found = false;
    CNode* node = NULL;
    POSITION pos = m_node_list.GetHeadPosition();
    while ((pos != NULL) && (!found))
    {
        node = m_node_list.GetNext(pos);
        found = node->getId() == id;
    }
    if (found)
        return node;
    else
        return NULL;
}

int CNetwork::getLinkCorsimId(int upnode, int dnnode)
{
    // find the corsim link id for the link (upnode, dnnode)
    int id = 0;
    int index = 0;
    int dnode = 0;
    int unode = 0;
    // search through all the links in corsim
    for (index = 0; index < ttlnk; index++)
    {
        // for the ith link, get the downstream node and upstream node as represented in corsim
        dnode = net_lnk_dn[index];
        unode = net_lnk_up[index];
        // for non-source nodes (< 7000), use the net_node_num array to map the node number in corsim back to the user defined node number in the TRAF file
        // use an offset of -1 (i.e., dnode - 1), because C arrays start at 0 while FORTRAN arrays start at 1
        if (dnode < 7000) dnode = net_node_num[dnode - 1];
        if (unode < 7000) unode = net_node_num[unode - 1];
        if ((dnode == dnnode) && (unode == upnode)) id = index;
    }

    return id;
}

void CNetwork::createLanes(FILE* file)
{
    // create the lanes
    char line[81] = { '\0' };
    int card_type = 0;
    int index = 0;
    int last_id = 0;
    bool found = false;
    POSITION pos = NULL;
    CLink* link = NULL;
    CLane* lane = NULL;
    CString channel_code;
    // lane data
    char up[5] = { '\0' };
    char dn[5] = { '\0' };
    char left[5] = { '\0' };
    char thru[5] = { '\0' };
    char right[5] = { '\0' };
    // search the TRAF file for link turn movement data records
    while (!feof(file))
    {
        // read a line of the file
        card_type = readTRFLine(file, line);
        // parse the type 21 (link turn movement data) cards
        if (card_type == 21)
        {
            // initialize the character data strings
            for (index = 0; index < 5; index++)
            {
                up[index] = '\0';
                dn[index] = '\0';
                left[index] = '\0';
                thru[index] = '\0';
                right[index] = '\0';
            }
            for (index = 0; index < 4; index++)
            {
                up[index] = line[index];
                dn[index] = line[index + 4];
                left[index] = line[index + 8];
                thru[index] = line[index + 12];
                right[index] = line[index + 16];
            }
            // search for the link between nodes up and down // FIXME: use findLink(atoi(up), atoi(dn)) ?
            found = false;
            pos = m_link_list.GetHeadPosition();
            while ((pos != NULL) && (!found))
            {
                link = m_link_list.GetNext(pos);
                found = (link->m_up_node->getId() == atoi(up)) && (link->m_dn_node->getId() == atoi(dn));
            }
            if (found && link != NULL)
            {
                // assign the turning percentages for the link
                link->m_left_movement_pc = atoi(left);
                link->m_thru_movement_pc = atoi(thru);
                link->m_right_movement_pc = atoi(right);
            }
        }
        else if (card_type > 21)
        {
            // because corsim expects the cards to be in ascending order by card type, we can jump out of the loop as soon as we encounter a card type greater than 21
            break;
        }
    }
    // for each link create the lanes for the link
    float f_left = 0.0f;
    float f_thru = 0.0f;
    float f_right = 0.0f;
    pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {
        link = m_link_list.GetNext(pos);
        for (index = 0; index < link->m_full_lanes_num; index++)
        {
            lane = new CLane(index + 1, link, "Full");
            channel_code = link->m_str_channel_code[index];
            if (channel_code == CString("T"))
            {
                // lane is thru only
                lane->setLeftMovePercent(0);
                lane->setRightMovePercent(0);
                lane->setThruMovePercent(100);
            }
            if (channel_code == CString("1"))
            {
                // lane is left turn only
                lane->setLeftMovePercent(100);
                lane->setRightMovePercent(0);
                lane->setThruMovePercent(0);
            }
            if (channel_code == CString("4"))
            {
                // lane is right turn only
                lane->setLeftMovePercent(0);
                lane->setRightMovePercent(100);
                lane->setThruMovePercent(0);
            }
            if (channel_code == CString("7"))
            {
                // lane is right and thru
                f_right = (float) link->m_right_movement_pc;
                f_thru = (float) link->m_thru_movement_pc;
                lane->setLeftMovePercent(0);
                lane->setRightMovePercent((int) (100 * (f_right / (f_right + f_thru))));
                lane->setThruMovePercent((int) (100 * (f_thru / (f_thru + f_right))));
            }
            if (channel_code == CString("8"))
            {
                // lane is left and thru
                f_thru = (float) link->m_thru_movement_pc;
                f_left = (float) link->m_left_movement_pc;
                lane->setLeftMovePercent((int) (100 * (f_left / (f_left + f_thru))));
                lane->setThruMovePercent((int) (100 * (f_thru / (f_left + f_thru))));
                lane->setRightMovePercent(0);
            }
            if (channel_code == CString("9"))
            {
                // lane is left, thru, and right
                f_left = (float) link->m_left_movement_pc;
                f_thru = (float) link->m_thru_movement_pc;
                f_right = (float) link->m_right_movement_pc;
                lane->setLeftMovePercent((int) (100 * (f_left / (f_left + f_right + f_thru))));
                lane->setRightMovePercent((int) (100 * (f_right / (f_left + f_right + f_thru))));
                lane->setThruMovePercent((int) (100 * (f_thru / (f_left + f_right + f_thru))));
            }
            link->addLane(lane);
        }
        // compute lane number for the first right turn bay - see card type 11
        last_id = net_max_lanes - link->m_right_turn_bays_num - link->m_left_turn_bays_num + 1;
        for (index = 0; index < link->m_right_turn_bays_num; index++)
        {
            lane = new CLane(last_id + index, link, "Bay");
            lane->setLeftMovePercent(0);
            lane->setRightMovePercent(100);
            lane->setThruMovePercent(0);
            link->addLane(lane);
        }
        // compute the lane number for the first left turn bay - see card type 11
        last_id = net_max_lanes - link->m_left_turn_bays_num + 1;
        for (index = 0; index < link->m_left_turn_bays_num; index++)
        {
            lane = new CLane(last_id + index, link, "Bay");
            lane->setLeftMovePercent(100);
            lane->setRightMovePercent(0);
            lane->setThruMovePercent(0);
            link->addLane(lane);
        }
    }
}

void CNetwork::getDetectors(FILE* file)
{
    // read the detectors information from the TRAF input file and create the detector list
    char line[81] = { '\0' };
    int card_type = 0;
    int index = 0;
    bool found = false;
    CLink* clink = NULL;
    CLane* clane = NULL;
    POSITION pos_link = NULL;
    CDetector* detector = NULL;
    // detector data
    char up[5] = { '\0' };
    char dn[5] = { '\0' };
    char lane[2] = { '\0' };
    char distance[6] = { '\0' };
    char type[2] = { '\0' };
    char id[5] = { '\0' };
    char length[5] = { '\0' };
    // search the TRAF file for detector data records
    while (!feof(file))
    {
        // read a line of the file
        card_type = readTRFLine(file, line);
        // parse the type 42 (detector data) cards
        if (card_type == 42)
        {
            // initialize the character data strings
            for (index = 0; index < 5; index++)
            {
                up[index] = '\0';
                dn[index] = '\0';
                id[index] = '\0';
                length[index] = '\0';
            }
            for (index = 0; index < 6; index++)
            {
                distance[index] = '\0';
            }
            for (index = 0; index < 2; index++)
            {
                lane[index] = '\0';
                type[index] = '\0';
            }
            // parse the line
            for (index = 0 ; index < 4; index++)
            {
                up[index] = line[index];
                dn[index] = line[index + 4];
                id[index] = line[index + 22];
                length[index] = line[index + 28];
            }
            lane[0] = line[11];
            type[0] = line[34];
            for (index = 0; index < 5; index++)
            {
                distance[index] = line[index + 15];
            }
            // search for the link the detector is on
            found = false;
            pos_link = m_link_list.GetHeadPosition();
            while ((pos_link != NULL) && (!found))
            {
                clink = m_link_list.GetNext(pos_link);
                found = (clink->m_up_node->getId() == atoi(up)) && (clink->m_dn_node->getId() == atoi(dn));
            }
            // create the detector only if the link is valid
            if (clink != NULL)
            {
                // find the lane object associated with the detector
                // a code of 8 or 9 indicates the detector spans multiple lanes, always including lane 1
                // if the code is 8 or 9, set the lane id to 1 so the lookup does not fail
                int lane_id = atoi(lane);
                if (lane_id > net_max_lanes)
                    lane_id = 1;
                clane = clink->findLane(lane_id);
                // create the detector
                int n_distance = (int) ((float) atoi(distance) / 10.0);
                detector = new CDetector(atoi(id), clink, clane, (CDetector::DetectorType) atoi(type), atoi(length), n_distance);
                // find and set the detector's internal corsim id
                setDetectorCorsimId(detector);
                // add the detector to the detector lists in the link and lane objects
                clink->addDetector(detector);
                if (clane != NULL)
                    clane->addDetector(detector);
            }
        }
        else if(card_type > 42)
        {
            // because corsim expects the cards to be in ascending order by card type, we can jump out of the loop as soon as we encounter a card type greater than 42
            break;
        }
    }
}

void CNetwork::setDetectorCorsimId(CDetector* detector)
{
    // finds the corsim detector id for the specified detector and sets it in the detector object
    int distance = detector->getDistance();
    CLink* link = detector->getLink();
    CLane* lane = detector->getLane();
    int lane_id = lane->getId();
    int det_id = detector->getId();
    // get the id of the first detector on the same link as detector
    int id = net_lnk_lnk[link->m_corsim_id];
    // compare the position and lane for the first detector with that of detector
    bool found = false;
    int this_lane = 0;
    int this_distance = 0;
    int this_id = 0;
    int tmp = 0;
    this_distance = net_det_pos[id - 1] / 10;
    this_id = net_det_id[id - 1];
    if ((distance == this_distance) && (det_id == this_id))
    {
	    while (!found && tmp < 2)
	    {
		    this_lane = net_det_lane[2 * id - 2 + tmp];
		    found = false;
		    if (lane_id == this_lane)
			    found = true;
		    else if (this_lane == 9)
			    found = true;
		    else if (this_lane == 8)
		    {
			    int left = link->m_left_turn_bays_num;
			    int right = link->m_right_turn_bays_num;
			    int nfull = 7 - left - right;
			    if (lane_id > 0 && lane_id <= nfull)
				    found = true;
			    else if (lane_id > 7)
				    found = true;
		    }
		    else if (this_lane == 10 && lane_id == 8)
			    found = true;
		    else if (this_lane == 11 && lane_id == 9)
			    found = true;
		    tmp++;
	    }
    }
    tmp = 0;
    while (!found && id > 0)
    {
        // not found so get the next detector on this link and compare again
        id = net_det_link[id-1];
	    if (id > 0)
	    {
		    this_distance = net_det_pos[id - 1] / 10;
		    this_id = net_det_id[id - 1];
		    if ((distance == this_distance) && (det_id == this_id))
		    {
			    while (!found && tmp < 2)
			    {
				    this_lane = net_det_lane[2 * id - 2 + tmp];
				    found = false;
				    if (lane_id == this_lane)
					    found = true;
				    else if (this_lane == 9)
					    found = true;
				    else if(this_lane == 8)
				    {
					    int left = link->m_left_turn_bays_num;
					    int right = link->m_right_turn_bays_num;
					    int nfull = 7 - left - right;
					    if (lane_id > 0 && lane_id <= nfull)
						    found = true;
					    else if (lane_id > 7)
						    found = true;
				    }
				    else if (this_lane == 10 && lane_id == 8)
					    found = true;
				    else if (this_lane == 11 && lane_id == 9)
					    found = true;
				    tmp++;
			    }
		    }
	    }
    }
    if (found)
    {
        detector->setCorsimId(id - 1);
    }
    else
    {
	    sprintf_s(out_buf, "Detector: %d was not found", detector->getId());
	    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);  
    }
}

void CNetwork::processDetectors(void)
{
    if (is_log_active) {
        sprintf(out_buf, "Processing %s ...", "detectors");
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }

    // process any detector information
    CLink* link = NULL;
    POSITION pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {
        link = m_link_list.GetNext(pos);

        if (is_log_active) {
            int out_len;
            out_len  = sprintf(out_buf, "Processing detectors on link (%d, %d) ...", link->getUpNode(), link->getDnNode());
            OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
        }

        link->processDetectors();
    }
}

void CNetwork::printDetectorsTransitions(void)
{
    // process any detector information
    CLink* link = NULL;
    POSITION pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {
        link = m_link_list.GetNext(pos);
        link->printDetectorsTransitions();
    }
}

void CNetwork::printDetectorsCount(void)
{
    // get the count from each detector and save it in a *.txt
    CLink* link = NULL;
    POSITION pos = m_link_list.GetHeadPosition();
    while (pos != NULL)
    {      
	    link = m_link_list.GetNext(pos);
        link->printDetectorsCount();
    } 
}