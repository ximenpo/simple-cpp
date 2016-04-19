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
    void ST_Idle(fsm_event*);
    void ST_Stop(fsm_event*);
    void ST_Start(MotorFSMData*);
    void ST_ChangeSpeed(MotorFSMData*);

    // state map to define state function order
    FSM_STATE_MAP_BEGIN()
    FSM_STATE_ENTRY(&MotorFSM::ST_Idle)
    FSM_STATE_ENTRY(&MotorFSM::ST_Stop)
    FSM_STATE_ENTRY(&MotorFSM::ST_Start)
    FSM_STATE_ENTRY(&MotorFSM::ST_ChangeSpeed)
    FSM_STATE_MAP_END();
};

// halt MotorFSM external event
void MotorFSM::Halt(void) {
    // given the Halt event, transition to a new state based upon
    // the current state of the state machine
    FSM_TRANSITION_MAP_BEGIN()                // - Current State -
    FSM_TRANSITION_ENTRY (EVENT_IGNORED)  // ST_Idle
    FSM_TRANSITION_ENTRY (CANNOT_HAPPEN)  // ST_Stop
    FSM_TRANSITION_ENTRY (ST_STOP)        // ST_Start
    FSM_TRANSITION_ENTRY (ST_STOP)        // ST_ChangeSpeed
    FSM_TRANSITION_MAP_END(NULL)
}

// set MotorFSM speed external event
void MotorFSM::SetSpeed(MotorFSMData* pData) {
    FSM_TRANSITION_MAP_BEGIN()                // - Current State -
    FSM_TRANSITION_ENTRY (ST_START)       // ST_Idle
    FSM_TRANSITION_ENTRY (CANNOT_HAPPEN)  // ST_Stop
    FSM_TRANSITION_ENTRY (ST_CHANGE_SPEED)// ST_Start
    FSM_TRANSITION_ENTRY (ST_CHANGE_SPEED)// ST_ChangeSpeed
    FSM_TRANSITION_MAP_END(pData)
}

// state machine sits here when MotorFSM is not running
void MotorFSM::ST_Idle(fsm_event* pData) {
    //cout << "MotorFSM::ST_Idle" << endl;
}

// stop the MotorFSM
void MotorFSM::ST_Stop(fsm_event* pData) {
    //cout << "MotorFSM::ST_Stop" << endl;

    // perform the stop MotorFSM processing here
    // transition to ST_Idle via an internal event
    fire_event(ST_IDLE);
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
