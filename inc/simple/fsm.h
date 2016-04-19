#ifndef FSM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define FSM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

//
//  fsm event params
//
struct  fsm_event {
    virtual ~fsm_event() {};
};

struct  fsm_state_entry;

//
//  fsm impls.
//
class   fsm {
public:
    fsm(unsigned char states_sum):
        state_sum_(states_sum),
        current_state_(0),
        event_generated_(false),
        event_(0) {

    }
    virtual ~fsm() {}

public:
    enum {
        EVENT_IGNORED = 0xFE,
        CANNOT_HAPPEN,
    };

    unsigned char   current_state() {
        return  current_state_;
    }

    void            fire_event(fsm_event* evt) {
        fire_event(current_state(), evt);
    }

protected:
    void            fire_event(unsigned char new_state, fsm_event* evt = 0) {
        //if (pData == 0)
        //    pData = new fsm_event();

        event_              = evt;
        event_generated_    = true;
        current_state_      = new_state;
    }

protected:
    //  internal funcs.
    void            internal_fire_transition_event(unsigned char new_state, fsm_event* evt = 0) {
        // if we are supposed to ignore this event
        if (new_state == EVENT_IGNORED) {
            // just delete the event data, if any
            if (0 != evt)
                delete evt;
        } else {
            // TODO - capture software lock here for thread-safety if necessary

            // generate the event and execute the state engine
            fire_event(new_state, evt);
            run_state_engine();

            // TODO - release software lock here
        }
    }

    virtual const fsm_state_entry*      internal_get_state_entry_map() = 0;

private:
    unsigned char       current_state_;
    const unsigned char state_sum_;
    bool                event_generated_;
    fsm_event*          event_;

private:
    inline  void        run_state_engine(void);
};

//  state entry func
typedef void (fsm::*fsm_state_func)(fsm_event *);
struct fsm_state_entry {
    fsm_state_func  state_func;
};

//
//  state define helpers.
//
#define     FSM_STATE_MAP_BEGIN() \
public:\
const fsm_state_entry*  internal_get_state_entry_map() {\
    static const fsm_state_entry StateMap[] = {

#define     FSM_STATE_ENTRY(state_func)\
    { reinterpret_cast<fsm_state_func>(state_func) },

#define     FSM_STATE_MAP_END() \
   { fsm_state_func(0) }\
   }; \
   return &StateMap[0]; }

//
//  transition define helpers.
//
#define     FSM_TRANSITION_MAP_BEGIN() \
    static const unsigned char TRANSITIONS[] = {\

#define     FSM_TRANSITION_ENTRY(entry)\
    entry,

#define     FSM_TRANSITION_MAP_END(data) \
    0 };\
    internal_fire_transition_event(TRANSITIONS[current_state()], data);

////////////////////////////////////////////////////////
//
//  internal impls.
//
inline  void fsm::run_state_engine(void) {
    fsm_event* tmp_evt = 0;

    // while events are being generated keep executing states
    while (event_generated_) {
        tmp_evt = event_;           // copy of event data pointer
        event_  = 0;                // event data used up, reset ptr
        event_generated_ = false;   // event used up, reset flag

        assert(current_state_ < state_sum_);

        // get state map
        const fsm_state_entry* state_map = internal_get_state_entry_map();

        // execute the state passing in event data, if any
        (this->*state_map[current_state_].state_func)(tmp_evt);

        // if event data was used, then delete it
        if (tmp_evt) {
            delete tmp_evt;
            tmp_evt = 0;
        }
    }
}

#endif
