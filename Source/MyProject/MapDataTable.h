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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		int Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
		FString Type;
};


/**
 *
 */
UCLASS()
class MYPROJECT_API UMapDataTable : public UDataTable
{
	GENERATED_BODY()

};