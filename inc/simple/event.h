#ifndef EVENT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define EVENT_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<deque>

#include	"delegate.h"

template<typename   DelegateType>
class event_handler {
public:
    //  相关类型
    typedef	DelegateType					delegate_type;
    typedef	std::pair<DelegateType,bool>	item_type;			// delegate & deleted
    typedef	std::deque<item_type>			container_type;

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

public:
    //
    //	操作符定义，添加／删除
    //
    void	operator +=(delegate_type observer) {
        attach(observer);
    }

    void	operator -=(delegate_type observer) {
        detach(observer);
    }

protected:
    void do_pack() {
        for(typename container_type::iterator  it = observers_.begin(); it != observers_.end(); ) {
            if(it->second)		it	= observers_.erase(it);
            else				++it;
        }
        something_detached_	= false;
    }

protected:
    container_type	observers_;
    bool			something_detached_;
};

template<typename	DelegateType>
class event : private event_handler<DelegateType> {
public:
    typedef event_handler<DelegateType>             handler_type;
    typedef	typename handler_type::delegate_type	delegate_type;

private:
    //  参数类型
    typedef	typename handler_type::container_type	container_type;
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
    //
    //  事件处理器
    //
    handler_type&  handler() {
        return  *this;
    }

public:
    //
    //	通知更新，所有对象都会执行 subject/observer
    //
    void	notify( ) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first();
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        if(this->something_detached_)this->do_pack();
    }

    void	notify( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second)it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
        if(this->something_detached_)this->do_pack();
    }

    //
    //	功能执行，执行时碰到返回true就停止
    //
    bool	execute( ) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first()) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

    bool	execute( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        bool    handled = false;
        typename container_type::iterator it,end;
        for(it = this->observers_.begin(), end = this->observers_.end(); it != end; ++it ) {
            if(!it->second && it->first(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)) {
                handled = true;
                break;
            }
        }
        if(this->something_detached_)this->do_pack();
        return handled;
    }

public:
    //
    //	操作符定义，通知模式
    //
    void	operator ()( ) {
        notify();
    }
    void	operator ()( P1 arg1	) {
        notify(arg1);
    }
    void	operator ()( P1 arg1, P2 arg2	) {
        notify(arg1, arg2);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3	) {
        notify(arg1, arg2, arg3);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4	) {
        notify(arg1, arg2, arg3, arg4);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5	) {
        notify(arg1, arg2, arg3, arg4, arg5);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
    void	operator ()( P1 arg1, P2 arg2, P3 arg3, P4 arg4, P5 arg5, P6 arg6, P7 arg7, P8 arg8	) {
        notify(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }
};

#endif
