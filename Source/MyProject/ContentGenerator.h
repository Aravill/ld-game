// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Containers/UnrealString.h"
#include "Engine/DataTable.h"
#include "ContentGenerator.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UContentGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Generation")
	static FString GenerateData(const int DimensionSize, const int MinSpace);
};