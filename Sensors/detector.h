#ifndef CDETECTOR
#define CDETECTOR

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
       CString                  m_label;
};

#endif // CDETECTOR
