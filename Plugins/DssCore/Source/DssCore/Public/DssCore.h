// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FDssCoreModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
