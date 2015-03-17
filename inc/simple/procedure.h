#ifndef PROCEDURE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define PROCEDURE_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	<cassert>
#include	<deque>

//
//	定时器定时判断
//
struct	procedure_timing {
    double	start_timestamp;
    double	current_timestamp;

    bool	check_expired(double delta)const {
        return	current_timestamp >= start_timestamp + delta;
    }
};

//
//	label & goto support
//
struct	procedure_label {
    int		label_pos__;
};

//
//	local data support
//
struct	procedure_data {
    virtual	~procedure_data() {}
};

//
//	procedure running context.
//
struct	procedure_context {
public:
    procedure_context():procedure_state__(0), procedure_data__(0) {
    }
    ~procedure_context() {
    }

public:
    void	reset() {
        procedure_state__ = 0;
        delete procedure_data__;
        procedure_data__ = 0;
    }
    bool	is_running() {
        return (procedure_state__ > 0);
    }
    bool	is_finished() {
        return (procedure_state__ < 0);
    }

    void	save_label(procedure_label* label) {
        if(0 != label) {
            label->label_pos__	= procedure_state__;
        }
    }

    void	goto_label(procedure_label* label) {
        if(0 != label) {
            procedure_state__ = label->label_pos__;
        }
    }

public:
    int					procedure_state__;
    procedure_data*		procedure_data__;
};

//
//	procedure internal macros.
//
namespace {
#define		PROCEDURE_I_PREPARE(PC)					procedure_context* procedure_context___ = PC;
#define		PROCEDURE_I_STATE()						procedure_context___->procedure_state__
#define		PROCEDURE_I_DATA_DECLARE(DATA,NAME)		DATA** procedure_data___ = (DATA**)(&procedure_context___->procedure_data__);DATA*& NAME = *procedure_data___;
#define		PROCEDURE_I_DATA_CREATE(DATA)			assert(0 == procedure_context___->procedure_data__); procedure_context___->procedure_data__ = (new DATA);
#define		PROCEDURE_I_CLEAR()						delete procedure_context___->procedure_data__; procedure_context___->procedure_data__ = 0; procedure_context___->procedure_state__ = -1;
}

#if		!defined(PROCEDURE_HANDLE_ERROR)
#	define		PROCEDURE_HANDLE_ERROR()			for (;;)
#endif

//
//	procedure operation macros.
//
#define		PROCEDURE_BEGIN(PC)						PROCEDURE_I_PREPARE(PC);switch(PROCEDURE_I_STATE()){ case -1: break; case 0:
#define		PROCEDURE_BEGIN_(PC,DATA,NAME)			PROCEDURE_I_PREPARE(PC);PROCEDURE_I_DATA_DECLARE(DATA,NAME);switch(PROCEDURE_I_STATE()){ case -1: break; case 0: {PROCEDURE_I_DATA_CREATE(DATA);}
#define		PROCEDURE_BEGIN_RUNLOOP(PC)				PROCEDURE_I_PREPARE(PC);switch(PROCEDURE_I_STATE()){ case -1: case 0:
#define		PROCEDURE_BEGIN_RUNLOOP_(PC,DATA,NAME)	PROCEDURE_I_PREPARE(PC);PROCEDURE_I_DATA_DECLARE(DATA,NAME);switch(PROCEDURE_I_STATE()){ case -1: case 0: {PROCEDURE_I_DATA_CREATE(DATA);}
#define		PROCEDURE_YIELD()						{ PROCEDURE_I_STATE() = __LINE__; return;		case __LINE__: ; }
#define		PROCEDURE_YIELD_(R)						{ PROCEDURE_I_STATE() = __LINE__; return (R);	case __LINE__: ; }
#define		PROCEDURE_WAIT(PT, S)					{(PT)->start_timestamp = (PT)->current_timestamp; while(!(PT)->check_expired(S)){PROCEDURE_YIELD();}}
#define		PROCEDURE_WAIT_(PT, S, R)				{(PT)->start_timestamp = (PT)->current_timestamp; while(!(PT)->check_expired(S)){PROCEDURE_YIELD_(R);}}
#define		PROCEDURE_STOP()						{ PROCEDURE_I_CLEAR(); return;		case __LINE__: ; }
#define		PROCEDURE_STOP_(R)						{ PROCEDURE_I_CLEAR(); return (R);	case __LINE__: ; }
#define		PROCEDURE_END()							{ break; default: PROCEDURE_HANDLE_ERROR() ; } } {PROCEDURE_I_CLEAR();} return;
#define		PROCEDURE_END_(R)						{ break; default: PROCEDURE_HANDLE_ERROR() ; } } {PROCEDURE_I_CLEAR();} return (R);

#define		PROCEDURE_LABEL_(LABEL)					{ (LABEL)->label_pos__ = __LINE__; case __LINE__: ; }


//
//	procedure_context_ex，可以传递给procedure内部调用到的函数
//
struct	procedure_context_ex {
public:
    procedure_context_ex() : level_(0) {}

public:
    void	reset() {
        level_ = 0;
        contexts_.clear();
    }
    bool	is_running() {
        return (!contexts_.empty() && contexts_[0].is_running());
    }
    bool	is_finished() {
        return (contexts_.empty() || contexts_[0].is_finished());
    }

public:
    procedure_context*	begin_context() {
        level_++;
        return get_context();
    }
    procedure_context*	get_context() {
        if(contexts_.size() < level_)contexts_.push_back(procedure_context());
        return contexts_.empty()?0:&contexts_[level_-1];
    }
    void				end_context() {
        level_--;
    }

private:
    size_t							level_;
    std::deque<procedure_context>	contexts_;
};


//
//	internal macros.
//
namespace {
#define		PROCEDURE_EI_PREPARE(PC)				procedure_context_ex* continuation_ex_context___ = (PC);PROCEDURE_I_PREPARE(continuation_ex_context___->begin_context())
#define		PROCEDURE_EI_STATE()					PROCEDURE_I_STATE()
#define		PROCEDURE_EI_DATA_DECLARE(DATA,NAME)	PROCEDURE_I_DATA_DECLARE(DATA,NAME)
#define		PROCEDURE_EI_DATA_CREATE(DATA)			PROCEDURE_I_DATA_CREATE(DATA)
#define		PROCEDURE_EI_POP_LEVEL()				continuation_ex_context___->end_context()
#define		PROCEDURE_EI_CLEAR()					PROCEDURE_I_CLEAR();PROCEDURE_EI_POP_LEVEL()
}

#if		!defined(PROCEDURE_EX_HANDLE_ERROR)
#define		PROCEDURE_EX_HANDLE_ERROR()			PROCEDURE_HANDLE_ERROR()
#endif

//
//	procedure_context_ex operation macros.
//
#define		PROCEDUREEX_BEGIN(PC)						PROCEDURE_EI_PREPARE(PC);switch(PROCEDURE_EI_STATE()){ case -1: break; case 0:
#define		PROCEDUREEX_BEGIN_(PC,DATA,NAME)			PROCEDURE_EI_PREPARE(PC);PROCEDURE_EI_DATA_DECLARE(DATA,NAME);switch(CONITNUATION_EI_STATE()){ case -1: break; case 0: {PROCEDURE_EI_DATA_CREATE(DATA);}
#define		PROCEDUREEX_BEGIN_RUNLOOP(PC)				PROCEDURE_EI_PREPARE(PC);switch(PROCEDURE_EI_STATE()){ case -1: case 0:
#define		PROCEDUREEX_BEGIN_RUNLOOP_(PC,DATA,NAME)	PROCEDURE_EI_PREPARE(PC);PROCEDURE_EI_DATA_DECLARE(DATA,NAME);switch(CONITNUATION_EI_STATE()){ case -1: case 0: {PROCEDURE_EI_DATA_CREATE(DATA);}
#define		PROCEDUREEX_YIELD()							{ PROCEDURE_EI_POP_LEVEL();PROCEDURE_EI_STATE() = __LINE__; return false;		case __LINE__: ; }
#define		PROCEDUREEX_WAIT(PT, S)						{(PT)->start_timestamp = (PT)->current_timestamp; while(!(PT)->check_expired(S)){PROCEDUREEX_YIELD();}}
#define		PROCEDUREEX_STOP()							{ PROCEDURE_EI_CLEAR(); return true;			case __LINE__: ; }
#define		PROCEDUREEX_END()							{ break; default: PROCEDURE_EX_HANDLE_ERROR() ; } } {PROCEDURE_EI_CLEAR();} return true;

#endif
