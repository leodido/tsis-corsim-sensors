#include "stdafx.h"

#include <string>
#include <vector>

#include "corsim.h"
#include "netsim.h"
#include "corwin.h"
#include "commons.h"

#include "detector.h"
#include "lane.h"
#include "link.h"

CDetector::CDetector() : CObject()
                       , m_id(0)
                       , m_corsim_id(0)
                       , m_link(NULL)
                       , m_lane(NULL)
                       , m_type(presence)
                       , m_length(0)
                       , m_dist_from_dw_node(0)
                       , m_state(false)
                       , m_count(0)
                       , m_activation_time(0.0f)
                       , m_deactivation_time(0.0f)
                       , m_transition_list()
                       , m_label("")
{
}

CDetector::CDetector(int id, CLink* link, CLane* lane, DetectorType type, int length, int distance)
                    : CObject()
                    , m_id(id)
                    , m_corsim_id(0)
                    , m_link(link)
                    , m_lane(lane)
                    , m_type(type)
                    , m_length(length)
                    , m_dist_from_dw_node(distance)
                    , m_state(false)
                    , m_count(0)
                    , m_activation_time(0.0f)
                    , m_deactivation_time(0.0f)
                    , m_transition_list()
                    , m_label("")
{
    char res[10] = { '\0' };
    sprintf(res, "D%d", m_id);
    m_label = _T(res);
}

CDetector::~CDetector()
{
    // nullify pointers to associated link and lane objects
    m_link = NULL;
    m_lane = NULL;
}

void CDetector::printTransitionList(void)
{
    // process (show) the items in the list
    int counter = 1;
    for (POSITION pos = m_transition_list.GetHeadPosition(); pos != NULL;)
    {
        int value = m_transition_list.GetNext(pos);
        double dec_sec = (double) counter / 10;
        sprintf(out_buf, "time: %.2f    %d", dec_sec, value);
        OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
        counter += 1;
    }
}

void CDetector::setState(bool active, bool record)
{
    m_state = active;
    if (record)
    {
        if (m_state)
            m_transition_list.AddTail(1);
        else
            m_transition_list.AddTail(0);
    }
}

std::vector<std::string> CDetector::getTransitions(void)
{
    std::vector<std::string> transitions;
    POSITION pos = NULL;
    pos = m_transition_list.GetHeadPosition();
    while (pos != NULL)
    {
        int value = m_transition_list.GetNext(pos);
        char res[1];
        sprintf(res, "%d", value);
        transitions.push_back(res);
    }
    return transitions;
}