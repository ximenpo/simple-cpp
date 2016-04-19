#include "SelfTest.h"
#include <iostream>

using namespace std;

SelfTest::SelfTest(INT maxStates) :
	StateMachine(maxStates)
{
}
	
void SelfTest::Cancel()
{
	// State machine base classes can't use a transition map, only the 
	// most-derived state machine class within the hierarchy can. So external 
	// events like this use the current state and call ExternalEvent()
	// to invoke the state machine transition. 
	if (GetCurrentState() != ST_IDLE)
		ExternalEvent(ST_FAILED);
}

STATE_DEFINE(SelfTest, Idle, NoEventData)
{
	cout << "SelfTest::ST_Idle" << endl;
}

ENTRY_DEFINE(SelfTest, EntryIdle, NoEventData)
{
	cout << "SelfTest::EN_EntryIdle" << endl;
}

STATE_DEFINE(SelfTest, Completed, NoEventData)
{
	cout << "SelfTest::ST_Completed" << endl;
	InternalEvent(ST_IDLE);
}

STATE_DEFINE(SelfTest, Failed, NoEventData)
{
	cout << "SelfTest::ST_Failed" << endl;
	InternalEvent(ST_IDLE);
}

