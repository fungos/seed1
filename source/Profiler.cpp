#include "Timer.h"
#include "Profiler.h"
#include "Log.h"

Profiler *Profiler::funcProfilerInstance = new Profiler("Function");
Profiler *Profiler::regionProfilerInstance = new Profiler("Region");

ContextStack ProfileContext::stack;

Profiler::Profiler(const char *name)
	: pName(name)
	, mapSubjectSlice()
	, mapSubjectTotal()
{
}

Profiler::~Profiler()
{
	Reset();
}

void Profiler::AddSlice(const char *func, int time)
{
	FuncTimeMapIt it = mapSubjectSlice.find(func);
	if (it != mapSubjectSlice.end())
	{
		ProfilerEntry *p = (*it).second;
		p->calls++;
		p->time += time;

		if(time > p->maxtime)
			p->maxtime = time;
	}
	else
	{
		ProfilerEntry *p = new ProfilerEntry();
		p->calls = 0;
		p->time = time;
		p->maxtime = time;

		mapSubjectSlice.insert(FuncTimePair(func, p));
	}
}

void Profiler::AddTotal(const char *func, int time)
{
	FuncTimeMapIt it = mapSubjectTotal.find(func);
	if (it != mapSubjectTotal.end())
	{
		ProfilerEntry *p = (*it).second;
		p->calls++;
		p->time += time;
	}
	else
	{
		ProfilerEntry *p = new ProfilerEntry();
		p->calls = 1;
		p->time = time;

		mapSubjectTotal.insert(FuncTimePair(func, p));
	}
}

void Profiler::Dump()
{
	fprintf(stdout, "Dumping %s Totals: %d", pName, (int)mapSubjectTotal.size());

	FuncTimeMapIt it = mapSubjectTotal.begin();
	FuncTimeMapIt end = mapSubjectTotal.end();
	for(; it != end; ++it)
	{
		Log("%s: %s [%d (Max %d) called %d times]", pName, (*it).first, (*it).second->time, (*it).second->maxtime, (*it).second->calls);
	}

	fprintf(stdout, "\nDumping %s Slices: %d", pName, (int)mapSubjectSlice.size());

	it = mapSubjectSlice.begin();
	end = mapSubjectSlice.end();
	for(; it != end; ++it)
	{
		Log("%s: %s [%d (Max %d) interrupted %d times]", pName, (*it).first, (*it).second->time, (*it).second->maxtime, (*it).second->calls);
	}
}

void Profiler::Reset()
{
	FuncTimeMapIt it = mapSubjectTotal.begin();
	FuncTimeMapIt end = mapSubjectTotal.end();
	for(; it != end; ++it)
	{
		delete (*it).second;
	}
	mapSubjectTotal.clear();

	it = mapSubjectSlice.begin();
	end = mapSubjectSlice.end();
	for(; it != end; ++it)
	{
		delete (*it).second;
	}
	mapSubjectSlice.clear();
}

ProfileContext::ProfileContext(const char *f, Profiler *prof)
	: func(f)
	, beg(0)
	, begTotal(0)
	, bTerminated(false)
	, pProf(prof)
{
	begTotal = clock();
	this->Push();
	this->StartOrContinue();
}

ProfileContext::~ProfileContext()
{
	this->Terminate();
}

void ProfileContext::Terminate()
{
	if (bTerminated)
		return;

	int end = clock();

	int diff = 0;
	diff = (end - begTotal);// / CLOCKS_PER_SEC;

	pProf->AddTotal(func, diff);

	this->StopAndCommit();
	this->Pop();
	bTerminated = true;
}

void ProfileContext::StopPrevious()
{
	if (!stack.empty())
	{
		ProfileContext *parent = stack.top();
		parent->StopAndCommit();
	}
}

void ProfileContext::RestorePrevious()
{
	if (!stack.empty())
	{
		ProfileContext *parent = stack.top();
		parent->StartOrContinue();
	}
}

void ProfileContext::StopAndCommit()
{
	int end = pTimer->GetMilliseconds();

	int diff = 0;
	diff = (end - beg);// / CLOCKS_PER_SEC;

	pProf->AddSlice(func, diff);
	beg = 0;
}

void ProfileContext::StartOrContinue()
{
	beg = pTimer->GetMilliseconds();
}

void ProfileContext::Push()
{
	this->StopPrevious();
	stack.push(this);
}

void ProfileContext::Pop()
{
	if (!stack.empty())
		stack.pop();
	this->RestorePrevious();
}
