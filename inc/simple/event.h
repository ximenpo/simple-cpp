#ifndef EVENT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define EVENT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<deque>

#include	"delegate.h"

template<typename	DelegateType>
class event {
public:
    typedef	DelegateType					delegate_type;
    typedef	std::pair<DelegateType,bool>	item_type;			// delegate & deleted
    typedef	std::deque<item_type>			container_type;

private:
    typedef	typename delegate_type::return_type		RET;
    typedef	typename delegate_type::param1_type		P1;
    typedef	typename delegate_type::param2_type		P2;
    typedef	typename delegate_type::param3_type		P3;
    typedef	typename delegate_type::param4_type		P4;
    typedef	typename delegate_type::param5_type		P5;
    typedef	typename delegate_type::param6_type		P6;
    typedef	typename delegate_type::param7_type		P7;
    typedef	typename delegate_type::param8_type		P8;

public:
    //	清空Observer
    void	clear() {
        observers_.clear();
    }

    //	关联Observer
    void	attach(DelegateType	observer) {
        if(!observer) {
            return;
        }

        typename container_type::iterator it,end;
        for( it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(it->first == observer && !it->second) {
                return;
            }
        }

        observers_.push_back(std::make_pair(observer, false));
    }

    //	分离Observer
    void	detach(DelegateType	observer) {
        if(!observer) {
            return;
        }

        typename container_type::iterator it,end;
        for( it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(it->first == observer) {
                it->second	= true;
                something_detached_	= true;
                return;
            }
        }
    }

    //
    //	通知更新，所有对象都会执行 subject/observer
    //
    void	notify( ) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first();
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        if(something_detached_)pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
        if(something_detached_)pack();
    }

    //
    //	功能执行，执行时碰到返回true就停止
    //
    void	execute( ) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first())break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7))break;
        }
        if(something_detached_)pack();
    }

    void	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        typename container_type::iterator it,end;
        for(it = observers_.begin(), end = observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8))break;
        }
        if(something_detached_)pack();
    }

public:
    //
    //	操作符定义，通知模式
    //
    event&		operator +=(delegate_type observer) {
        Attach(observer);
        return	*this;
    }

    event&		operator -=(delegate_type observer) {
        Detach(observer);
        return	*this;
    }

    event&		operator ()( ) {
        notify();
        return *this;
    }
    event&		operator ()( P1 arg1	) {
        notify(arg1);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2	) {
        notify(arg1, arg2);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3	) {
        notify(arg1, arg2, arg3);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        notify(arg1, arg2, arg3, arg4);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        notify(arg1, arg2, arg3, arg4, arg5);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        return *this;
    }
    event&		operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        return *this;
    }

private:
    void	pack() {
        for(typename container_type::iterator  it = observers_.begin(); it != observers_.end(); ) {
            if(it->second)		it	= observers_.erase(it);
            else				++it;
        }
        something_detached_	= false;
    }

protected:
    container_type	observers_;

private:
    bool			something_detached_;
};

#endif
