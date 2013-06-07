#include "stdafx.h"
#include "lane.h"
#include "link.h"
#include "detector.h"

CLane::CLane() : CObject()
    ,   m_id(0)
    ,   m_link(NULL)
    ,   m_str_type("Full")
    ,   m_detector_list()
    ,   m_left_movement_pc(0)
    ,   m_thru_movement_pc(0)
    ,   m_right_movement_pc(0)
{
}

CLane::CLane(int id, CLink *link, const CString &str_type)
    :   CObject()
    ,   m_id(id)
    ,   m_link(link)
    ,   m_str_type(str_type)
    ,   m_detector_list()
    ,   m_left_movement_pc(0)
    ,   m_thru_movement_pc(0)
    ,   m_right_movement_pc(0)
{
}

CLane::~CLane()
{
    // nullify the pointer to the link that owns this object
    m_link = NULL;
    m_detector_list.RemoveAll();
}
