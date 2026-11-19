// Copyright Dark State Studio.All rights reserved.

#include "DssCore.h"

#include "Debug/DssDebug.h"
#include "HAL/IConsoleManager.h"

#define LOCTEXT_NAMESPACE "FDssCoreModule"

void FDssCoreModule::StartupModule()
{
	FDssDebug::Initialize();
}

void FDssCoreModule::ShutdownModule()
{
	FDssDebug::Shutdown();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDssCoreModule, DssCore)
