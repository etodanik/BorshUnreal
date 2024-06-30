// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#include "borsh/concepts.h"
#include "Borsh/ThirdParty/BorshUnrealConcepts.h"
#include "borsh/utils.h"
#include "borsh/converters.h"
#include "Borsh/ThirdParty/BorshUnrealConverters.h"
#include "borsh/serializer.h"
#include "borsh/templates.h"
#include "Borsh/ThirdParty/BorshSerialize.h"

class FBorshModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
