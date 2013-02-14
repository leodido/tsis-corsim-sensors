#include "stdafx.h"

#include "corsim.h"
#include "netsim.h"
#include "corwin.h"

#include "commons.h"

#include "node.h"
#include "link.h"

CNode::CNode() : CObject()
               , m_id(0)
               , m_str_control_type()
               , m_corsim_internal_id(0)
               , m_x_pos(0)
               , m_y_pos(0)
{
   for (int approach = 0; approach < max_approaches; approach++)
   {
      m_approaches[approach] = NULL;
   }

   for (int interval = 0; interval < max_intervals; interval++)
   {
      m_duration[interval] = 0;
   }
}

CNode::CNode(int id, const CString& str_control_type)
            : CObject()
            , m_id(id)
            , m_str_control_type(str_control_type)
            , m_corsim_internal_id(0)
            , m_x_pos(0)
            , m_y_pos(0)
{
    for (int approach = 0; approach < max_approaches; approach++)
    {
        m_approaches[approach] = NULL;
    }
    for (int interval=0; interval < max_intervals; interval++)
    {
        m_duration[interval] = 0;
    }
}

CNode::~CNode()
{
}

CLink* CNode::getApproach(int approach) const
{
    CLink* approach_link = NULL;
    if (1 <= approach && approach <= max_approaches)
    {
        approach_link = m_approaches[approach - 1];
    }

    return approach_link;
}

int CNode::setApproach(int approach, CLink* approach_link)
{
    int valid = -1;
    if (1 <= approach && approach <= max_approaches)
    {
        valid = 0;
        m_approaches[approach - 1] = approach_link;
    }

    return valid;
}

int CNode::getDuration(int interval) const
{
    int duration = 0;
    if (1 <= interval && interval <= max_intervals)
    {
        duration = m_duration[interval - 1];
    }

    return duration;
}

int CNode::setDuration(int interval, int duration)
{
    int valid = -1;
    if (1 <= interval && interval <= max_intervals)
    {
        valid = 0;
        m_duration[interval - 1] = duration;
    }

    return valid;
}

void CNode::setLocation(int x_pos, int y_pos)
{
    m_x_pos = x_pos;
    m_y_pos = y_pos;
}

void CNode::print(void)
{
    int out_len;
    out_len  = sprintf(out_buf, "# node { id: %d, ", getId());
    out_len += sprintf(out_buf + out_len, "corsim_id: %d, ", getCorsimId());
    out_len += sprintf(out_buf + out_len, "control_type: %s, ", getControlType());
    out_len += sprintf(out_buf + out_len, "x_pos: %d, ", getXLocation());
    out_len += sprintf(out_buf + out_len, "y_pos: %d, ", getYLocation());
    out_len += sprintf(out_buf + out_len, "durations: %s ", "{");
    for (int interval = 0; interval < max_intervals; interval++)
    {
        if (interval == (max_intervals - 1))
            out_len += sprintf(out_buf + out_len, "%d }", m_duration[interval]);
        else
            out_len += sprintf(out_buf + out_len, "%d, ", m_duration[interval]);
    }
    out_len += sprintf(out_buf + out_len, " %s", "}");
    OutputString(out_buf, strlen(out_buf), SIM_COLOR_RGB, RTE_MESSAGE_RGB);
}