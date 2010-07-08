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

/*! \file ISceneNode.h
	\author	Danny Angelo Carminati Grein
	\brief Scene Node Interface
*/

#ifndef __ISCENE_NODE_H__
#define __ISCENE_NODE_H__

#include "interface/ISceneObject.h"

namespace Seed {

class SEED_CORE_API ISceneNode : public ISceneObject
{
	public:
		ISceneNode();
		virtual ~ISceneNode();

		virtual BOOL IsNode() const;

		// IRenderable
		virtual void Update(f32 dt);
		virtual void Render();

		virtual void Reset() = 0;
		virtual void Add(ISceneObject *obj) = 0;
		virtual void Remove(ISceneObject *obj) = 0;
		virtual u32 Size() const = 0;
		virtual ISceneObject *GetChildAt(u32 i) = 0;

	private:
		SEED_DISABLE_COPY(ISceneNode);
};

} // namespace

#endif // __ISCENE_NODE_H__
