#ifndef CLANE
#define CLANE

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
    Class that represents and encapsulates the lane.
*****************************************************************************************/

class CLink;
class CDetector;

class CLane : public CObject
{
   public:
      CLane();
      CLane(int id, CLink* link, const CString& str_type);
      virtual ~CLane();

      inline int                            getId() const { return m_id; }

      inline void                           addDetector(CDetector* detector) { m_detector_list.AddTail(detector); }
      inline int                            getNumDetectors() const { return m_detector_list.GetCount(); }

      inline int                            getLeftMovePercent() const { return m_left_movement_pc; }
      inline void                           setLeftMovePercent(int percent) { m_left_movement_pc = percent; }
      inline int                            getThruMovePercent() const { return m_thru_movement_pc; }
      inline void                           setThruMovePercent(int percent) { m_thru_movement_pc = percent; }
      inline int                            getRightMovePercent() const { return m_right_movement_pc; }
      inline void                           setRightMovePercent(int percent) { m_right_movement_pc = percent; }

   private:
      int                                   m_id; // lane id 1, 2, .., 7 - see card type 11
      CLink*                                m_link; // pointer to link on which this lane is located
      CString                               m_str_type; // bay or full
      CTypedPtrList<CPtrList, CDetector*>   m_detector_list;
      int                                   m_left_movement_pc;
      int                                   m_thru_movement_pc;
      int                                   m_right_movement_pc;
};

#endif // CLANE
