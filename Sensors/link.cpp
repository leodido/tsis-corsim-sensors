#include "stdafx.h"

#include <math.h>
#include <vector>
#include <numeric>

#include "corsim.h"
#include "netsim.h"
#include "corwin.h"
#include "commons.h"

#include "sensors.h"
#include "detector.h"
#include "link.h"
#include "lane.h"
#include "integer.h"
#include "binary_sequence.h"

CLink::CLink() : CObject()
    ,   m_corsim_id(0)
    ,   m_up_node(NULL)
    ,   m_dn_node(NULL)
    ,   m_thru_node(NULL)
    ,   m_left_node(NULL)
    ,   m_right_node(NULL)
    ,   m_length(0)
    ,   m_full_lanes_num(0)
    ,   m_left_turn_bays_num(0)
    ,   m_right_turn_bays_num(0)
    ,   m_left_bays_len(0)
    ,   m_right_bays_len(0)
    ,   m_free_flow_speed(0)
    ,   m_travel_time(0.0f)
    ,   m_left_movement_pc(0)
    ,   m_thru_movement_pc(0)
    ,   m_right_movement_pc(0)
    ,   m_offset(0)
    ,   m_opposing_node_id(0)
    ,   m_opposing_link(NULL)
    ,   m_lane_list()
    ,   m_detector_list()
    ,   m_preempt(false)
    ,   m_current_tp(1)
{
    for (int lane = 0; lane < net_max_lanes; lane++)
    {
        m_str_channel_code[lane].Empty();
    }

    for (int interval = 0; interval < max_intervals; interval++)
    {
        m_control_code[interval] = -1;
    }
}

CLink::~CLink()
{
    POSITION pos = NULL;
    // delete the lanes on this link
    CLane *lane = NULL;
    pos = m_lane_list.GetHeadPosition();
    while (pos != NULL)
    {
        lane = m_lane_list.GetNext(pos);
        delete lane;
    }
    m_lane_list.RemoveAll();
    // delete the detectors on this link
    CDetector *detector = NULL;
    pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        detector = m_detector_list.GetNext(pos);
        delete detector;
    }
    m_detector_list.RemoveAll();
}

void CLink::processDetectors(void)
{
    // this method processes the information from the detectors on this link
    int index = 0;
    int det = 0;
    int det_info = 0;
    int type = 0; // TODO: reduce scope
    unsigned int speed = 0; // TODO: reduce scope
    unsigned int num = 0;
    int new_state = 0;
    int old_state = 0;
    POSITION pos = NULL;
    CDetector *detector = NULL;
    POSITION pos_i = NULL;
    CInteger *pi = NULL;
    CBinarySequence *sequence = NULL;

    // compute the current time
    int time = net_clock + end_of_init;

    // loop through the detectors
    pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        detector = m_detector_list.GetNext(pos);

        if (is_log_active && log_level == 2)
        {
            int det_id = detector->getId();
            sprintf(out_buf, "# detector { id: %d, lane: %d }", det_id, detector->getLane()->getId());
            OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
        }

        int det_num = detector->getCorsimId();
        det_info = net_det_mod[det_num];

        // the first 3 bits contain the type of detector, so (AND) det_info with 0x7 = 111.
        type = det_info & 0x7;

        // the next 7 bits contain the vehicles speed (if type==1), so (AND) det_info with 0x10F447 = 1111111.
        speed = (det_info >> 3) & 0x10F447;

        // next 13 bits contain the vehicle count since start of simulation.
        num = det_info >> 10;

        // bit array representing the activation/deactivation of the detector
        det = net_det_on[det_num];

        // NOTE: it allocates memory for sequence
        sequence = CBinarySequence::convert(det);

        // loop through the binary sequence starting at the end because when the sequence is created it is in reverse order
        pos_i = sequence->sequence.GetTailPosition();
        old_state = detector->getState() ? 1 : 0;
        for (index = 0; index < 10; index++)
        {
            pi = sequence->sequence.GetPrev(pos_i);
            new_state = pi->data;
            if ((old_state == 0) && (new_state == 1))
            {
                // activation
                detector->setActivationTime(0.1f);
                detector->setDeactivationTime(0.0f);
                detector->setState(true, !is_init); // set state and record it if is not the initialization phase
            }
            if ((old_state == 1) && (new_state == 1))
            {
                detector->setActivationTime(detector->getActivationTime() + 0.1f);
                detector->setDeactivationTime(0.0f);
                detector->setState(true, !is_init); // set state and record it if is not the initialization phase
            }
            if ((old_state == 1) && (new_state == 0))
            {
                // deactivation
                detector->setActivationTime(0.0f);
                detector->setDeactivationTime(0.1f);
                detector->setState(false, !is_init); // set state and record it if is not the initialization phase
            }
            if ((old_state == 0) && (new_state == 0))
            {
                detector->setActivationTime(0.0f);
                detector->setDeactivationTime(detector->getDeactivationTime() + 0.1f);
                detector->setState(false, !is_init); // set state and record it if is not the initialization phase
            }
            old_state = new_state;
        }
        // clean up
        delete sequence;
        // update count
        detector->setCount((int) num);
    }
}

void CLink::updateTimePeriodsDetectorsCount()
{
    int det_info = 0;
    int num = 0;
    // loop through the detectors
    POSITION pos = NULL;
    CDetector *detector = NULL;
    pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        detector = m_detector_list.GetNext(pos);
        det_info = net_det_mod[detector->getCorsimId()];
        num = det_info >> 10; // total count until now for current detector
        int count = num;
        int current_tp = tp_counter - 1;
        if (current_tp > 1)
        {
            std::vector<int> vols = detector->getVolumes();
            count -= std::accumulate(vols.begin(), vols.end(), 0);
        }
        detector->setTimePeriodVolume(count);
    }
}

void CLink::resetDetectorsCount(void)
{
    // loop through the detectors
    POSITION pos = NULL;
    CDetector *detector = NULL;
    pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        detector = m_detector_list.GetNext(pos);
        detector->setCount(0);
    }
}

void CLink::printDetectorsTransitions(void)
{
    // loop through the detectors
    POSITION pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        CDetector *detector = m_detector_list.GetNext(pos);
        detector->printTransitionList();
    }
}

CLane *CLink::findLane(int lane_id)
{
    // returns a pointer to the lane object, given its lane id
    bool found = false;
    CLane *lane = NULL;
    POSITION pos = m_lane_list.GetHeadPosition();
    while (pos != NULL && !found)
    {
        lane = m_lane_list.GetNext(pos);
        found = lane->getId() == lane_id;
    }
    if (!found) lane = NULL;

    return lane;
}

float CLink::computeTravelTime(void)
{
    // computes the travel time to traverse a link
    float travel_time = 0.0f;
    travel_time = (float) m_length / (float) m_free_flow_speed;

    return travel_time;
}

void CLink::printDetectorsCount(void)
{
    // get the detectors count
    POSITION pos = NULL;
    CDetector *detector = NULL;
    // loop through the detectors
    pos = m_detector_list.GetHeadPosition();
    while (pos != NULL)
    {
        detector = m_detector_list.GetNext(pos);
        sprintf_s(out_buf, "Detector: %d - Count: %d", detector->getId(), detector->getCount());
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
    }
}

void CLink::print(void)
{
    int out_len;
    out_len  = sprintf(out_buf, "link { corsim_id: %d, ", getCorsimId());
    out_len += sprintf(out_buf + out_len, "up_node_id: %d, ", getUpNode());
    out_len += sprintf(out_buf + out_len, "dn_node_id: %d, ", getDnNode());
    out_len += sprintf(out_buf + out_len, "th_node_id: %d, ", getThruNode());
    out_len += sprintf(out_buf + out_len, "lf_node_id: %d, ", getLeftNode());
    out_len += sprintf(out_buf + out_len, "rg_node_id: %d, ", getRightNode());
    out_len += sprintf(out_buf + out_len, "length: %d, ", m_length);
    out_len += sprintf(out_buf + out_len, "full_lanes_num: %d, ", m_full_lanes_num);
    out_len += sprintf(out_buf + out_len, "free_flow_speed: %d, ", m_free_flow_speed);
    out_len += sprintf(out_buf + out_len, "travel_time: %f, ", m_travel_time);
    out_len += sprintf(out_buf + out_len, "free_flow_speed: %d }", m_free_flow_speed);
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
}
