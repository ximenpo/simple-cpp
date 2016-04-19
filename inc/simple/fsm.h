#ifndef FSM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2
#define FSM_H_66BC65DB_AFF6_43C8_8654_D1A2801635E2

#include	"_third/StateMachineCompact/StateMachine.h"

#if     defined(END_STATE_MAP)
#   undef   END_STATE_MAP
#   define  END_STATE_MAP \
   { StateFunc(0) }\
   }; \
   return &StateMap[0]; }

#endif

#endif
