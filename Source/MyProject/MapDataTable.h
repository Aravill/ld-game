// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MapDataTable.generated.h"


/** Example Data */
USTRUCT(BlueprintType)
struct FCustomMapData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	/** Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		FString name;

	/** Extra HitPoints gained at this level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		FString age;
};


/**
 *
 */
UCLASS()
class MYPROJECT_API UMapDataTable : public UDataTable
{
	GENERATED_BODY()

};