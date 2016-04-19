#include "simple/igloo.h"
using namespace igloo;

#include	<iostream>
using	std::cout;
using	std::endl;

#include	"simple/fsm.h"

struct MotorFSMData : public fsm_event {
    int speed;

    MotorFSMData(int speed) {
        this->speed	= speed;
    }
};

// the MotorFSM state machine class
class MotorFSM : public fsm {
public:
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_IDLE = 0,
        ST_STOP,
        ST_START,
        ST_CHANGE_SPEED,
        ST_MAX_STATES
    };

public:
    MotorFSM() : fsm(ST_MAX_STATES) {}

    // external events taken by this state machine
    void Halt();
    void SetSpeed(MotorFSMData*);

private:
    // state machine state functions
    void ST_Idle(EventData*);
    void ST_Stop(EventData*);
    void ST_Start(MotorFSMData*);
    void ST_ChangeSpeed(MotorFSMData*);

    // state map to define state function order
    BEGIN_STATE_MAP
    STATE_MAP_ENTRY(&MotorFSM::ST_Idle)
    STATE_MAP_ENTRY(&MotorFSM::ST_Stop)
    STATE_MAP_ENTRY(&MotorFSM::ST_Start)
    STATE_MAP_ENTRY(&MotorFSM::ST_ChangeSpeed)
    END_STATE_MAP
};

// halt MotorFSM external event
void MotorFSM::Halt(void) {
    // given the Halt event, transition to a new state based upon
    // the current state of the state machine
    BEGIN_TRANSITION_MAP                      // - Current State -
    TRANSITION_MAP_ENTRY (EVENT_IGNORED)  // ST_Idle
    TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)  // ST_Stop
    TRANSITION_MAP_ENTRY (ST_STOP)        // ST_Start
    TRANSITION_MAP_ENTRY (ST_STOP)        // ST_ChangeSpeed
    END_TRANSITION_MAP(NULL)
}

// set MotorFSM speed external event
void MotorFSM::SetSpeed(MotorFSMData* pData) {
    BEGIN_TRANSITION_MAP                      // - Current State -
    TRANSITION_MAP_ENTRY (ST_START)       // ST_Idle
    TRANSITION_MAP_ENTRY (CANNOT_HAPPEN)  // ST_Stop
    TRANSITION_MAP_ENTRY (ST_CHANGE_SPEED)// ST_Start
    TRANSITION_MAP_ENTRY (ST_CHANGE_SPEED)// ST_ChangeSpeed
    END_TRANSITION_MAP(pData)
}

// state machine sits here when MotorFSM is not running
void MotorFSM::ST_Idle(EventData* pData) {
    //cout << "MotorFSM::ST_Idle" << endl;
}

// stop the MotorFSM
void MotorFSM::ST_Stop(EventData* pData) {
    //cout << "MotorFSM::ST_Stop" << endl;

    // perform the stop MotorFSM processing here
    // transition to ST_Idle via an internal event
    InternalEvent(ST_IDLE);
}

// start the MotorFSM going
void MotorFSM::ST_Start(MotorFSMData* pData) {
    //cout << "MotorFSM::ST_Start" << endl;
    // set initial MotorFSM speed processing here
}

// changes the MotorFSM speed once the MotorFSM is moving
void MotorFSM::ST_ChangeSpeed(MotorFSMData* pData) {
    //cout << "MotorFSM::ST_ChangeSpeed" << endl;
    // perform the change MotorFSM speed to pData->speed here
}


Context(fsm_usage) {
    Spec(motor_fsm_usage) {
        MotorFSM	fsm;

        AssertThat(fsm.current_state(), Equals(MotorFSM::ST_IDLE));
        fsm.SetSpeed(new MotorFSMData(10));
        AssertThat(fsm.current_state(), Equals(MotorFSM::ST_START));
        fsm.SetSpeed(new MotorFSMData(10));
        AssertThat(fsm.current_state(), Equals(MotorFSM::ST_CHANGE_SPEED));
        fsm.Halt();
        AssertThat(fsm.current_state(), Equals(MotorFSM::ST_IDLE));
    }
};
