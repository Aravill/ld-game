// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateContentSceneComponent.h"

#include "ContentGenerator.h"
#include "MapDataTable.h"
#include "Containers/UnrealString.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/Array.h"

struct Room {
	int x;
	int y;
	int width;
	int height;
};

UDataTable* UGenerateContentSceneComponent::GenerateData(const int DimensionSize, const int MinSpace) {
	FString data = "ID,X,Y,Width,Height,Type";
	int id = 0;
	FString type = "wall";
	TArray<TArray<bool>> walls;
	for (int x = 0; x < DimensionSize; ++x) {
		TArray<bool> wall;
		wall.Init(false, DimensionSize);
		walls.Add(wall);
	}

	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, -1, -1, DimensionSize+1, 1, *type);
    	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, DimensionSize, -1, 1, DimensionSize+1, *type);
    	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, -1, 0, 1, DimensionSize+1, *type);
    	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, 0, DimensionSize, DimensionSize+1, 1, *type);

	Room room;
	room.x = 0;
	room.y = 0;
	room.width = DimensionSize;
	room.height = DimensionSize;
	TArray<Room> stack;
	stack.Add(room);

	while (stack.Num() > 0) {
		Room actual = stack.Pop();
		Room a;
		Room b;
		int counter = 0;

		int horizontal = actual.height <= (2 * MinSpace) ? 0 : actual.width <= (2 * MinSpace) ? 1 : FMath::RandRange(0, 1);
		if (horizontal) {
			int y = FMath::RandRange(MinSpace, actual.height - MinSpace - 1);
			while ((actual.x > 0) && (walls[actual.x - 1][actual.y + y] == false) &&
				((actual.x + actual.width) < DimensionSize) && (walls[actual.x + actual.width][actual.y + y] == false)
			) {
				y = FMath::RandRange(MinSpace, actual.height - MinSpace - 1);

				UE_LOG(LogTemp, Warning, TEXT("%d"), counter);
				if (++counter == 10) {
					break;
				}
			}
			if (counter == 10) continue;
			int x = (actual.x > 0) && (walls[actual.x - 1][actual.y + y] == false)
				? 0
				: ((actual.x + actual.width) < DimensionSize) && (walls[actual.x + actual.width][actual.y + y] == false)
				? (actual.width - MinSpace)
				: FMath::RandRange(0, actual.width - MinSpace);

			a.x = actual.x;
			a.y = actual.y;
			a.height = y;
			a.width = actual.width;

			b.x = actual.x;
			b.y = actual.y + y + 1;
			b.height = actual.height - y - 1;
			b.width = actual.width;

			if (x != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x, actual.y + y, x, 1, *type);
				for (int i = 0; i < x; ++i) {
					walls[actual.x + i][actual.y + y] = true;
				}
			}
			if ((actual.width - x - MinSpace) != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x + x + MinSpace, actual.y + y, actual.width - x - MinSpace, 1, *type);
				for (int i = 0; i < actual.width - x - MinSpace; ++i) {
					walls[actual.x + x + MinSpace + i][actual.y + y] = true;
				}
			}
		}
		else {
			int x = FMath::RandRange(MinSpace, actual.width - MinSpace - 1);
			while (
				((actual.y + actual.height) < DimensionSize) && (walls[actual.x + x][actual.y + actual.height] == false)
				&& (actual.y > 0) && (walls[actual.x + x][actual.y - 1] == false)
			) {
				x = FMath::RandRange(MinSpace, actual.width - MinSpace - 1);

				UE_LOG(LogTemp, Warning, TEXT("%d"), counter);
				if (++counter == 10) {
					break;
				}
			}
			if (counter == 10) continue;
			int y = (actual.y > 0) && (walls[actual.x + x][actual.y - 1] == false)
				? 0
				: ((actual.y + actual.height) < DimensionSize) && (walls[actual.x + x][actual.y + actual.height] == false)
				? (actual.height - MinSpace)
				: FMath::RandRange(0, actual.height - MinSpace);

			a.x = actual.x;
			a.y = actual.y;
			a.height = actual.height;
			a.width = x;

			b.x = actual.x + x + 1;
			b.y = actual.y;
			b.height = actual.height;
			b.width = actual.width - x - 1;

			if (y != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x + x, actual.y, 1, y, *type);
				for (int i = 0; i < y; ++i) {
					walls[actual.x + x][actual.y + i] = true;
				}
			}
			if ((actual.height - y - MinSpace) != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x + x, actual.y + y + MinSpace, 1, actual.height - y - MinSpace, *type);
				for (int i = 0; i < actual.height - y - MinSpace; ++i) {
					walls[actual.x + x][actual.y + y + MinSpace + i] = true;
				}
			}
		}

		if (((a.width > (MinSpace * 2)) && (a.height >= MinSpace)) || ((a.height > (MinSpace * 2)) && (a.width >= MinSpace))) {
			stack.Add(a);
		}
		if (((b.width > (MinSpace * 2)) && (b.height >= MinSpace)) || ((b.height > (MinSpace * 2)) && (b.width >= MinSpace))) {
			stack.Add(b);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *data);

	datatable->CreateTableFromCSVString(data);

	return datatable;
}

// Sets default values for this component's properties
UGenerateContentSceneComponent::UGenerateContentSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> mydatatable_BP(TEXT("DataTable'/Game/TopDownBP/Blueprints/data/template.template'"));

	if (mydatatable_BP.Object)
	{
		UE_LOG(LogTemp, Warning, TEXT("get datatable"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("get datatable")));
		datatable = mydatatable_BP.Object;
	}
}


// Called when the game starts
void UGenerateContentSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGenerateContentSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

