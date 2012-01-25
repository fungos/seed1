//
//  Created by Danny Grein on 10/13/08.
//  Copyright TechFront Studios 2008. All rights reserved.

#include "Defines.h"
#include "interface/IGameApp.h"

const FilePath *iphGetRootPath();
const FilePath *iphGetHomePath();

namespace Seed {
	void SetGameApp(IGameApp *app);
} // namespace

