
#include <algorithm>
#include "timer.h"
#include "procedure.h"

timer::timer()
    :	action_last_id_(0)
    ,	timestamp_(0.0f)
    ,	action_erased_(false) {
}

timer::~timer() {
    clear();
}

bool timer::has(int action_id) {
    ACTION_LIST::iterator it		= action_list_.begin();
    ACTION_LIST::iterator it_end	= action_list_.end();
    for(; it != it_end; ++it) {
        if(it->first->timer_id == action_id) {
            return true;
        }
    }
    return false;
}

bool timer::erase(int action_id) {
    ACTION_LIST::iterator it		= action_list_.begin();
    ACTION_LIST::iterator it_end	= action_list_.end();
    for(; it != it_end; ++it) {
        if(it->first->timer_id == action_id) {
            it->second = true;
            action_erased_	= true;
            return true;
        }
    }
    return false;
}

size_t timer::size() {
    return	action_list_.size();
}

void timer::update(double dt) {
    timestamp_	+= dt;

    ACTION_LIST::iterator	it		= action_list_.begin();
    ACTION_LIST::iterator	it_end	= action_list_.end();
    for(; it != it_end; ++it) {
        if(it->second) {
            // had been erased.
            continue;
        }
        timer_action* pAction	= it->first;
        if(pAction->fire_timestamp <= timestamp_ && pAction->execute(timestamp_)) {
            action_erased_	= true;
            it->second	= true;
        }
    }

    if(action_erased_) {
        this->pack();
    }
}

void timer::clear() {
    ACTION_LIST::iterator it		= action_list_.begin();
    ACTION_LIST::iterator it_end	= action_list_.end();
    for(; it != it_end; ++it) {
        delete	it->first;
    }

    action_list_.clear();
}

void timer::pack() {
    for(ACTION_LIST::iterator  it = action_list_.begin(); it != action_list_.end(); ) {
        if(!it->second) {
            ++it;
        } else {
            delete	it->first;
            it	= action_list_.erase(it);
        }
    }
    action_erased_	= false;
}

unsigned int timer::execute(delegate<void()> action, float delay) {
    struct	real_action : public timer_action {
        delegate<void()>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                handler();
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= (void*)0;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<void(void* param)> action, float delay, void* param) {
    struct	real_action : public timer_action {
        delegate<void(void* param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                handler((void*)fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<void(any_ptr param)> action, float delay, any_ptr param) {
    struct	real_action : public timer_action {
        delegate<void(any_ptr param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                handler(fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool()> action, float delay) {
    struct	real_action : public timer_action {
        delegate<bool()>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                return	handler();
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= (void*)0;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(void* param)> action, float delay, void* param) {
    struct	real_action : public timer_action {
        delegate<bool(void* param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                return	handler((void*)fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(any_ptr param)> action, float delay, any_ptr param) {
    struct	real_action : public timer_action {
        delegate<bool(any_ptr param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                return	handler(fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context* ctx, procedure_timing* timing)> action, float delay) {
    struct	real_action : public timer_action {
        procedure_context	ctx;
        procedure_timing	timing;
        delegate<bool(procedure_context* ctx, procedure_timing* timing)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= (void*)0;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context* ctx, procedure_timing* timing, void* param)> action, float delay, void* param) {
    struct	real_action : public timer_action {
        procedure_context	ctx;
        procedure_timing	timing;
        delegate<bool(procedure_context* ctx, procedure_timing* timing, void* param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing, (void*)fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context* ctx, procedure_timing* timing, any_ptr param)> action, float delay, any_ptr param) {
    struct	real_action : public timer_action {
        procedure_context	ctx;
        procedure_timing	timing;
        delegate<bool(procedure_context* ctx, procedure_timing* timing, any_ptr param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing, fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing)> action, float delay) {
    struct	real_action : public timer_action {
        procedure_context_ex	ctx;
        procedure_timing		timing;
        delegate<bool(procedure_context_ex* ctx, procedure_timing* timing)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= (void*)0;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, void* param)> action, float delay, void* param) {
    struct	real_action : public timer_action {
        procedure_context_ex	ctx;
        procedure_timing		timing;
        delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, void* param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing, (void*)fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}

unsigned int timer::execute(delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, any_ptr param)> action, float delay, any_ptr param) {
    struct	real_action : public timer_action {
        procedure_context_ex	ctx;
        procedure_timing		timing;
        delegate<bool(procedure_context_ex* ctx, procedure_timing* timing, any_ptr param)>	handler;
    protected:
        virtual	bool	do_execute(double timestamp) {
            if(handler) {
                timing.current_timestamp	= timestamp;
                return	handler(&ctx, &timing, fire_param);
            }
            return	true;	// always stop.
        }
    };
    do {
        ++action_last_id_;
    } while(action_last_id_ == INVALID_ID);
    real_action*	paction	= new real_action;
    paction->timer_id		= action_last_id_;
    paction->fire_param		= param;
    paction->fire_timestamp	= timestamp_ + delay;
    paction->handler		= action;
    paction->ctx.reset();
    paction->timing.start_timestamp	= paction->timing.current_timestamp	= timestamp_;
    action_list_.push_back(std::make_pair(static_cast<timer_action*>(paction), false));

    return paction->timer_id;
}
