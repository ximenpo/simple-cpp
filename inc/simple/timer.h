#ifndef TIMER_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define TIMER_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include <deque>

#include	"delegate.h"
#include	"ptr.h"

struct	procedure_timing;
struct	procedure_context;
struct	procedure_context_ex;

//
//	定时器动作[基类]
//
struct	timer_action {
    unsigned int	timer_id;
    any_ptr			fire_param;
    double			fire_timestamp;

public:
    virtual		~timer_action() {}

    bool		execute(double timestamp) {
        return	do_execute(timestamp);
    }

protected:
    //	return	ture means action is completed.
    virtual	bool	do_execute(double timestamp)=0;
};

//
//	定时行为执行器
//
class timer {
public:
    timer();
    ~timer();

public:
    enum			{ INVALID_ID = 0 };

public:
    bool			has(int action_id);
    bool			erase(int action_id);
    size_t			size();
    void			clear();

    ///	void()
    unsigned int	execute(delegate<void()> action, float delay = 0.0f);
    ///	void(void* param)
    unsigned int	execute(delegate<void(void* param)> action, float delay = 0.0f, void* param = 0);
    ///	void(any_ptr param)
    unsigned int	execute(delegate<void(any_ptr param)> action, float delay = 0.0f, any_ptr param = (void*)0);

    ///	bool()
    unsigned int	execute(delegate<bool()> action, float delay = 0.0f);
    ///	bool(void* param)
    unsigned int	execute(delegate<bool(void* param)> action, float delay = 0.0f, void* param = 0);
    ///	bool(any_ptr param)
    unsigned int	execute(delegate<bool(any_ptr param)> action, float delay = 0.0f, any_ptr param = (void*)0);

    ///	bool(procedure_context* ctx, procedure_timing* timing)
    unsigned int	execute(delegate<bool(procedure_context* ctx, procedure_timing* timing)> action, float delay = 0.0f);
    ///	bool(procedure_context* ctx, procedure_timing* timing, void* param)
    unsigned int	execute(delegate<bool(procedure_context* ctx, procedure_timing* timing, void* param)> action, float delay = 0.0f, void* param = 0);
    ///	bool(procedure_context* ctx, procedure_timing* timing, any_ptr param)
    unsigned int	execute(delegate<bool(procedure_context* ctx, procedure_timing* timing, any_ptr param)> action, float delay = 0.0f, any_ptr param = (void*)0);

    ///	bool(procedure_context_ex* ctx, procedure_timing* timing)
    unsigned int	execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing)> action, float delay = 0.0f);
    ///	bool(procedure_context_ex* ctx, procedure_timing* timing, void* param)
    unsigned int	execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, void* param)> action, float delay = 0.0f, void* param = 0);
    ///	bool(procedure_context_ex* ctx, procedure_timing* timing, any_ptr param)
    unsigned int	execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, any_ptr param)> action, float delay = 0.0f, any_ptr param = (void*)0);

public:
    void			update(double dt);

protected:
    typedef	std::deque<std::pair<timer_action*, bool> >		ACTION_LIST;

private:
    void			pack();

protected:
    volatile double	timestamp_;
    unsigned int	action_last_id_;
    ACTION_LIST		action_list_;

private:
    bool			action_erased_;
};

#endif
