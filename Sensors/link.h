/*******************************************************************************
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
    Class that represents and encapsulates the link.
********************************************************************************/

#ifndef CLINK
#define CLINK

#pragma once

#include "node.h"

// define a constant for the maximum number of netsim lanes
const int net_max_lanes = 7;

// forward declarations
class CLane;
class CDetector;
class CBinarySequence;

class CLink : public CObject
{
public:
    CLink();
    virtual ~CLink();

    void                                  processDetectors(void);
    CLane                                *findLane(int lane_id);
    float                                 computeTravelTime(void);

    inline int                            getCorsimId(void)
    {
        return m_corsim_id;
    }

    inline int                            getUpNode(void)
    {
        return m_up_node->getId();
    }
    inline int                            getDnNode(void)
    {
        return m_dn_node->getId();
    }
    inline int                            getThruNode(void)
    {
        return m_thru_node->getId();
    }
    inline int                            getLeftNode(void)
    {
        return m_left_node->getId();
    }
    inline int                            getRightNode(void)
    {
        return m_right_node->getId();
    }

    void                                  printDetectorsCount(void);
    void                                  resetDetectorsCount(void);
    void                                  printDetectorsTransitions(void);

    inline void                           addLane(CLane *lane)
    {
        m_lane_list.AddTail(lane);
    }

    inline void                           addDetector(CDetector *detector)
    {
        m_detector_list.AddTail(detector);
    }
    inline int                            getNumDetectors() const
    {
        return m_detector_list.GetCount();
    }

    inline int                            getOpposingNodeId() const
    {
        return m_opposing_node_id;
    }
    inline void                           setOpposingNodeId(int node_id)
    {
        m_opposing_node_id = node_id;
    }

    inline CLink                         *getOpposingLink() const
    {
        return m_opposing_link;
    }
    inline void                           setOpposingLink(CLink *link)
    {
        m_opposing_link = link;
    }

    void                                  updateTimePeriodsDetectorsCount(void);

    void                                  print(void);

private:
    friend class                          CNetwork; // make the CNetwork class a friend so the CNetwork object can get and set link data without using interface methods
    int                                   m_corsim_id;
    CNode                                *m_up_node;
    CNode                                *m_dn_node;
    CNode                                *m_thru_node;
    CNode                                *m_left_node;
    CNode                                *m_right_node;
    int                                   m_length;
    int                                   m_full_lanes_num;
    int                                   m_left_turn_bays_num;
    int                                   m_right_turn_bays_num;
    int                                   m_left_bays_len;
    int                                   m_right_bays_len;
    int                                   m_free_flow_speed;
    float                                 m_travel_time;
    CString                               m_str_channel_code[net_max_lanes];
    int                                   m_left_movement_pc;
    int                                   m_thru_movement_pc;
    int                                   m_right_movement_pc;
    int                                   m_control_code[max_intervals];
    int                                   m_offset;
    bool                                  m_preempt;
    int                 m_current_tp;

    int                                   m_opposing_node_id; // id of the upstream node of the link that opposes this link
    CLink                                *m_opposing_link; // pointer to the upstream node of the link that opposes this link
    CTypedPtrList<CPtrList, CLane *>      m_lane_list; // list of lanes on this link
    CTypedPtrList<CPtrList, CDetector *>  m_detector_list; // list of detectors associated with this link
};

#endif // CLINK
