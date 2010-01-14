/*! \file StateMachine.h
	\author Rafael Eduardo Gonchor
	\brief State Machine
*/


#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include "Defines.h"
#include "Config.h"
#include "Log.h"
#include "interface/IEvent.h"
#include <vector>


namespace Seed {


class STMEvent : public IEvent
{
	public:
		STMEvent()
			: IEvent(IEvent::STM, 0)
		{
		}

		STMEvent(u32 iId)
			: IEvent(IEvent::STM, iId)
		{
		}

		inline void Initialize(u32 iId)
		{
			this->iId = iId;
		}

		virtual ~STMEvent()
		{
		}

		virtual const char *GetObjectName() const
		{
			return "STMEvent";
		}

		virtual int GetObjectType() const
		{
			return Seed::ObjectEvent;
		}
};

class STMState
{
	public:
		virtual ~STMState() {}
		virtual void OnStart(IObject *) {};
		virtual void OnUpdate(IObject *) {};
		virtual void OnStop(IObject *) {};
};

//Class to define a TRANSITION
class STMTransition
{
	private:
		STMState *from;
		STMState *to;
		STMEvent *event;

	public:
		void Initialize(STMState *from, STMEvent *event, STMState *to)
		{
			this->from = from;
			this->to = to;
			this->event = event;
		};

		inline STMState *GetFromState()
		{
			return from;
		};

		inline STMState *GetToState()
		{
			return to;
		};

		inline STMEvent *GetEvent()
		{
			return event;
		};
};

#ifdef ERROR
#undef ERROR
#endif // ERROR

//template<unsigned int TRANSITIONNUMBER>
class StateMachine
{
	public:
		enum eReturnCode
		{
			OK				= 0x00,
			STATE_NOT_FOUND	= 0x01,
			EVENT_NOT_FOUND	= 0x02,
			ERROR			= 0xFF
		};

	private:
		STMState *currentState;
		std::vector<STMTransition *> transitions;

		SEED_DISABLE_COPY(StateMachine);
		
		
	public:
		StateMachine() :  currentState(NULL), transitions() {}
		~StateMachine() {}

		eReturnCode Initialize(STMState *state, IObject *pUserData)
		{
			if (!state)
				return STATE_NOT_FOUND; //invalid state

			currentState = NULL;
			unsigned int i=0;

			for (; i<transitions.size(); i++)
				if (transitions[i]->GetFromState() == state)
					break;

			if (i>=transitions.size())
				return STATE_NOT_FOUND; //invalid state

			currentState = state;

			currentState->OnStart(pUserData);

			return OK;
		};

		inline void RegisterTransition(STMTransition *transition)
		{
			//Check if the transition is has a valid configuration
			ASSERT(transition->GetFromState() && transition->GetToState() && transition->GetEvent());
			//transitions.Add(transition);
			transitions.push_back(transition);
		};

		inline void ClearTransitions()
		{
			transitions.clear();
		}

		inline STMState *GetCurrentState()
		{
			return currentState;
		}

		inline eReturnCode OnSTMEvent(STMEvent *evt, IObject *pUserData)
		{
			unsigned int i = 0;

			for (; i<transitions.size(); i++)
			{
				if (transitions[i]->GetEvent() == evt)
				{
					if (transitions[i]->GetFromState() == currentState)
					{
						//Old State Exit
						transitions[i]->GetFromState()->OnStop(pUserData);

						currentState = transitions[i]->GetToState();

						//New State Entry callback
						currentState->OnStart(pUserData);

						break;
					}
				}
			}

			if (i >= transitions.size()) //no registred transition helding this event
			{
				Log("StateMachine: Event not found.");
				return EVENT_NOT_FOUND; //invalid event
			}

			return OK;
		};

		inline eReturnCode Update(IObject *pUserData)
		{
			if (!currentState) //not initialized
			{
				Log("StateMachine: State not found.");
				return STATE_NOT_FOUND;
			}

			//State Looping callback
			currentState->OnUpdate(pUserData);

			return OK;
		}
};


} // namespace


#endif // __STATE_MACHINE_H__

