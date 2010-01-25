/*! \file IGameApp.h
	\author	Danny Angelo Carminati Grein
	\brief The real main for a game application
*/


#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "IUpdatable.h"
#include "IModule.h"


namespace Seed {


class IGameApp : public IUpdatable, public IModule
{
	public:
		IGameApp();
		virtual ~IGameApp();

		virtual void Setup(int argc, char **argv);
		virtual void WriteOut(const char *msg);
		virtual void WriteErr(const char *msg);
		virtual void WriteDbg(const char *msg);

		virtual BOOL HasError() const;

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(IGameApp);
};


} // namespace


#endif // __GAME_APP_H__

