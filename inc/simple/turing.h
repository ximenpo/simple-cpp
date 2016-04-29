#ifndef TURING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TURING_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include    "delegate.h"

//
//  turing_machine
//
class	turing_machine {
public:
    turing_machine();

public:
    //  executor delegates
    delegate<bool(turing_machine*, int instruction_address)>    instruction_executor;// true if no error

public:
    // start/stop machine
    bool	start();
    void	stop();
    bool    is_started();

    //  pause status
    void	set_paused(bool paused = true);
    bool	is_paused();

    //  instruction address
    void    set_instruction_address(int address);
    int     get_instruction_address();

    //  run loop, return false if there's something wrong.
    bool	run(bool step_run);

private:
    int		pos_;
    bool    started_;
    bool	paused_;
};

#endif
