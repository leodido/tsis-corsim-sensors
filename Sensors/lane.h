#ifndef CLANE
#define CLANE

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
