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

/*! \file SceneNode.h
	\author	Danny Angelo Carminati Grein
	\brief Scene Node
*/

#ifndef __SCENE_NODE_H__
#define __SCENE_NODE_H__

#include "Array.h"
#include "interface/ISceneNode.h"

namespace Seed {

template <int NODES> class SceneNode : public ISceneNode
{
	public:
		SceneNode()
		{
			arNodes.Truncate();
		}

		virtual ~SceneNode()
		{
			arNodes.Truncate();
		}

		virtual void Add(ISceneObject *obj)
		{
			if (obj)
			{
				for (u32 i = 0; i < arNodes.Size(); i++)
				{
					if (arNodes[i] == obj)
						return;
				}

				obj->SetParent(this);
				arNodes.Add(obj);
			}
		}

		virtual void Remove(ISceneObject *obj)
		{
			if (obj)
			{
				arNodes.Remove(obj);
			}
		}

		virtual void Update(f32 delta)
		{
			for (u32 i = 0; i < arNodes.Size(); i++)
			{
				arNodes[i]->Update(delta);
			}
		}

		virtual void Render()
		{
			for (u32 i = 0; i < arNodes.Size(); i++)
			{
				arNodes[i]->Render();
			}
		}

		virtual void Reset()
		{
			arNodes.Truncate();
		}

		virtual u32 Size() const
		{
			return arNodes.Size();
		}

		virtual ISceneObject *GetChildAt(u32 i)
		{
			return arNodes[i];
		}

	private:
		SEED_DISABLE_COPY(SceneNode);

	private:
		Array<ISceneObject *, NODES> arNodes;
};

} // namespace

#endif // __SCENE_NODE_H__
