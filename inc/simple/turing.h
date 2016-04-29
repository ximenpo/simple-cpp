#ifndef TURING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TURING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include    "delegate.h"

//
//  turing_machine
//
class	turing_machine {
public:
    turing_machine():pos_(-1),started_(false),paused_(false) {
    }

public:
    //  executor delegates
    delegate<bool(turing_machine&, int address)>        instruction_executor;// true if succeed
public:
    // start/stop machine
    bool	start() {
        pos_        = 0;
        paused_     = false;
        started_    = true;
        return  true;
    }
    void	stop() {
        pos_        = -1;
        paused_     = false;
        started_    = false;
    }
    bool    is_started() {
        return  started_;
    }

    //  pause status
    void	set_paused(bool paused = true) {
        paused_ = paused;
    }
    bool	is_paused() {
        return  paused_;
    }

    //  instruction address
    void    set_instruction_address(int address) {
        pos_    = address;
    }
    int     get_instruction_address() {
        return  pos_;
    }

    //  run loop, return false if there's something wrong.
    bool	run(bool step_run) {
        bool    result = false;

        while(  started_
                &&  !paused_
                &&  pos_ >= 0
                &&  instruction_executor
             ) {
            result  = instruction_executor(*this, pos_++);
            if(step_run) {
                break;
            }
        }

        return  result;
    }

private:
    int		pos_;
    bool    started_;
    bool	paused_;
};

#endif
