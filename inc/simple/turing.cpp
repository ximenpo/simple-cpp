
#include    "turing.h"


turing_machine::turing_machine():pos_(-1),started_(false),paused_(false) {
}

// start/stop machine
bool	turing_machine::start() {
    pos_        = 0;
    paused_     = false;
    started_    = true;
    return  true;
}
void	turing_machine::stop() {
    pos_        = -1;
    paused_     = false;
    started_    = false;
}
bool    turing_machine::is_started() {
    return  started_;
}

//  pause status
void	turing_machine::set_paused(bool paused) {
    paused_ = paused;
}
bool	turing_machine::is_paused() {
    return  paused_;
}

//  instruction address
void    turing_machine::set_instruction_address(int address) {
    pos_    = address;
}
int     turing_machine::get_instruction_address() {
    return  pos_;
}

//  run loop, return false if there's something wrong.
bool	turing_machine::run(bool step_run) {
    bool    result = false;

    while(  started_
            &&  !paused_
            &&  pos_ >= 0
            &&  instruction_executor
         ) {
        result  = instruction_executor(this, pos_++);
        if(step_run || !result) {
            break;
        }
    }

    return  result;
}
