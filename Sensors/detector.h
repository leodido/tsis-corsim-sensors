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
    Class that represents and encapsulates the detector/sensor.
*****************************************************************************************/

#ifndef CDETECTOR
#define CDETECTOR

#pragma once

#include <string>
#include <vector>

class CLane;
class CLink;

class CDetector : public CObject
{
    public:
      enum DetectorType { presence = 0, pulse = 1 };

      CDetector();
      CDetector(int id, CLink* link, CLane* lane, DetectorType type, int length, int distance);
      virtual ~CDetector();

      inline int                getId() const { return m_id; }
      inline int                getCorsimId() const { return m_corsim_id; }
      inline void               setCorsimId(int corsim_id) { m_corsim_id = corsim_id; }

      inline CLink*             getLink() const { return m_link; }
      inline CLane*             getLane() const { return m_lane; }

      // constant attributes (type, lenght, distance)
      inline DetectorType       getType() const { return m_type; }
      inline int                getLength() const { return m_length; }
      inline int                getDistance() const  { return m_dist_from_dw_node; }

      // state
      inline bool               getState() const { return m_state; }
      void                      setState(bool active, bool record = false);

      // volume count
      inline int                getCount() const { return m_count; }
      inline void               setCount(int count) { m_count = count; }
      inline void               incCount() { m_count += 1; }
      inline void               decCount() { m_count -= 1; }

      // activation/deactivation
      inline float              getActivationTime() const { return m_activation_time; }
      inline void               setActivationTime(float time) { m_activation_time = time; }
      inline float              getDeactivationTime() const { return m_deactivation_time; }
      inline void               setDeactivationTime(float time) { m_deactivation_time = time; }

      void                      printTransitionList(void);
      std::vector<std::string>  getTransitions(void);

      inline CString            getLabel(void) const { return m_label; }

    private:
        int                     m_id;
        int                     m_corsim_id;
        CLink*                  m_link;
        CLane*                  m_lane;
        DetectorType            m_type;
        int                     m_length;
        int                     m_dist_from_dw_node;
        bool                    m_state; // activated = true, deactivated = false
        int                     m_count;
        float                   m_activation_time;
        float                   m_deactivation_time;
        CList<int, int>         m_transition_list;
        CString                 m_label;
};

#endif // CDETECTOR
