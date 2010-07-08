/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

#if defined(SEED_ENABLE_PROFILER)

#include "Timer.h"
#include "Profiler.h"
#include "Log.h"

#define TAG "[Profiler] "

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
	this->Reset();
}

void Profiler::AddSlice(const char *func, u64 time)
{
	FuncTimeMapIt it = mapSubjectSlice.find(func);
	if (it != mapSubjectSlice.end())
	{
		ProfilerEntry *p = (*it).second;
		p->calls++;
		p->time += time;

		if (time > p->maxtime)
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

void Profiler::AddTotal(const char *func, u64 time)
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
	Log(TAG "Dumping %s Totals: %d", pName, (int)mapSubjectTotal.size());

	FuncTimeMapIt it = mapSubjectTotal.begin();
	FuncTimeMapIt end = mapSubjectTotal.end();
	for (; it != end; ++it)
	{
		ProfilerEntry *entry = (*it).second;
		int calls = entry->calls;
		float average = static_cast<float>((float)entry->time / (float)calls);
		Log(TAG "%s: %s [average time %f, called %d times]", pName, (*it).first, average, calls);
	}

	Log(TAG "Dumping %s Slices: %d", pName, (int)mapSubjectSlice.size());

	it = mapSubjectSlice.begin();
	end = mapSubjectSlice.end();
	for (; it != end; ++it)
	{
		ProfilerEntry *entry = (*it).second;
		int calls = entry->calls;
		float average = static_cast<float>((float)entry->time / (float)calls);
		Log(TAG "%s: %s [average time %f, interrupted %d times]", pName, (*it).first, average, calls);
	}
}

void Profiler::Reset()
{
	FuncTimeMapIt it = mapSubjectTotal.begin();
	FuncTimeMapIt end = mapSubjectTotal.end();
	for (; it != end; ++it)
	{
		delete (*it).second;
	}
	mapSubjectTotal.clear();

	it = mapSubjectSlice.begin();
	end = mapSubjectSlice.end();
	for (; it != end; ++it)
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
	begTotal = pTimer->GetMilliseconds(); //clock();
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

	u64 end = pTimer->GetMilliseconds(); //clock();

	u64 diff = 0;
	diff = static_cast<u64>(end - begTotal);// / CLOCKS_PER_SEC;

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
	u64 end = pTimer->GetMilliseconds();

	u64 diff = 0;
	diff = static_cast<u64>(end - beg);// / CLOCKS_PER_SEC;

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

#endif // SEED_ENABLE_PROFILER
