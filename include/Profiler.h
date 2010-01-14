#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "defines.h"
#include <map>
#include <stack>
#include <time.h>

#define SEED_FUNCTION_PROFILER					ProfileContext _ctx_func(__FUNCTION__);
#define SEED_BEGIN_REGION_PROFILER(name, str)	ProfileContext _c##name(str, Profiler::regionProfilerInstance);
#define SEED_END_REGION_PROFILER(name)			_c##name.Terminate();

class ProfileContext;

struct ProfilerEntry
{
	int time;
	int maxtime;
	int calls;

	ProfilerEntry()
		: time(0)
		, maxtime(0)
		, calls(0)
	{}
};

typedef std::map<const char *, ProfilerEntry *> FuncTimeMap;
typedef std::pair<const char *, ProfilerEntry *> FuncTimePair;
typedef FuncTimeMap::iterator FuncTimeMapIt;
typedef std::stack<ProfileContext *> ContextStack;

class Profiler
{
	public:
		Profiler(const char *name);
		~Profiler();

		void AddSlice(const char *func, int time);
		void AddTotal(const char *func, int time);
		void Dump();
		void Reset();

		static Profiler *funcProfilerInstance;
		static Profiler *regionProfilerInstance;

	private:
		const char *pName;
		FuncTimeMap mapSubjectSlice;
		FuncTimeMap mapSubjectTotal;

		Profiler(const Profiler &);
		Profiler &operator=(const Profiler &);
};

class ProfileContext
{
	public:
		ProfileContext(const char *f, Profiler *prof = Profiler::funcProfilerInstance);
		~ProfileContext();

		void Terminate();
		void StopPrevious();
		void RestorePrevious();
		void StopAndCommit();
		void StartOrContinue();
		void Push();
		void Pop();

		static ContextStack stack;

	private:
		const char *func;
		int beg;
		int begTotal;
		bool bTerminated;

		Profiler *pProf;

		ProfileContext(const ProfileContext &);
		ProfileContext &operator=(const ProfileContext &);
};

#endif
