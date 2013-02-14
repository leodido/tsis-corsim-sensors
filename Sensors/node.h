#ifndef CNODE
#define CNODE

// define constant for the maximum number of approaches
const int max_approaches = 5;

// define a constant for fixed-time signal intervals
const int max_intervals = 12;

class CLink;

class CNode : public CObject
{
    public:
        CNode();
        CNode(int id, const CString& str_control_type);
        virtual ~CNode();

        inline int              getId() const { return m_id; }
        inline const CString&   getControlType() const { return m_str_control_type; }

        inline int              getCorsimId() const { return m_corsim_internal_id; }
        inline void             setCorsimId(int id) { m_corsim_internal_id = id; }

        CLink*                  getApproach(int approach) const;
        int                     setApproach(int approach, CLink* approach_link);

        int                     getDuration(int interval) const;
        int                     setDuration(int interval, int duration);

        void                    setLocation(int x_pos, int y_pos);
        inline int              getXLocation(void) const { return m_x_pos; }
        inline int              getYLocation(void) const { return m_y_pos; }

        void                    print(void);

    private:
        int                     m_id;
        CString                 m_str_control_type;
        int                     m_corsim_internal_id;
        CLink*                  m_approaches[max_approaches];
        int                     m_duration[max_intervals];
        int                     m_x_pos;
        int                     m_y_pos;
};

#endif // CNODE
