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

bool findPath(Room actual, TArray<Room>& path, TArray<TArray<bool>>& walls, TArray<TArray<bool>>& visited, FVector2D finish) {
	path.Add(actual);

	visited[actual.x][actual.y] = true;
	if (actual.x == finish.X && actual.y == finish.Y) {
		return true;
	}

	int XLimit = finish.X + 1;
	int YLimit = finish.Y + 1;

	Room child;
	if (( // It must be possible to go left
		(actual.x + 1) < XLimit && walls[actual.x + 1][actual.y] == false && visited[actual.x + 1][actual.y] == false
		//) && ( // The left-left field must be a wall or unexplored [otw we could find a shorter path by skipping actual field]
		//	(actual.x + 2) >= XLimit || walls[actual.x + 2][actual.y] == true || visited[actual.x + 2][actual.y] == false
		//	) && ( // The left-bottom field must be a wall or unexplored
		//		(actual.y - 1) < 0 || walls[actual.x + 1][actual.y - 1] == true || visited[actual.x + 1][actual.y - 1] == false
		//		) && ( // The left-top field must be a wall or unexplored
		//			(actual.y + 1) >= YLimit || walls[actual.x + 1][actual.y + 1] == true || visited[actual.x + 1][actual.y + 1] == false
					)) {
		// Go left
		child.x = actual.x + 1;
		child.y = actual.y;
		if (findPath(child, path, walls, visited, finish)) {
			return true;
		}
	}
	if ((
		(actual.y + 1) < YLimit && walls[actual.x][actual.y + 1] == false && visited[actual.x][actual.y + 1] == false
		//) && ( // The top-top field must be a wall or unexplored [otw we could find a shorter path by skipping actual field]
		//	(actual.y + 2) >= YLimit || walls[actual.x][actual.y + 2] == true || visited[actual.x][actual.y + 2] == false
		//	) && ( // The right-top field must be a wall or unexplored
		//		(actual.x - 1) < 0 || walls[actual.x - 1][actual.y + 1] == true || visited[actual.x - 1][actual.y + 1] == false
		//		) && ( // The left-top field must be a wall or unexplored
		//			(actual.x + 1) >= XLimit || walls[actual.x + 1][actual.y + 1] == true || visited[actual.x + 1][actual.y + 1] == false
					)) {
		// Go up
		child.x = actual.x;
		child.y = actual.y + 1;
		if (findPath(child, path, walls, visited, finish)) {
			return true;
		}
	}
	if ((
		actual.x > 0 && walls[actual.x - 1][actual.y] == false && visited[actual.x - 1][actual.y] == false
		//) && ( // The right-right field must be a wall or unexplored [otw we could find a shorter path by skipping actual field]
		//	(actual.x - 2) < 0 || walls[actual.x - 2][actual.y] == true || visited[actual.x - 2][actual.y] == false
		//	) && ( // The right-bottom field must be a wall or unexplored
		//		(actual.y - 1) < 0 || walls[actual.x - 1][actual.y - 1] == true || visited[actual.x - 1][actual.y - 1] == false
		//		) && ( // The right-top field must be a wall or unexplored
		//			(actual.y + 1) >= YLimit || walls[actual.x - 1][actual.y + 1] == true || visited[actual.x - 1][actual.y + 1] == false
					)) {
		// Go right
		child.x = actual.x - 1;
		child.y = actual.y;
		if (findPath(child, path, walls, visited, finish)) {
			return true;
		}
	}
	if ((
		actual.y > 0 && walls[actual.x][actual.y - 1] == false && visited[actual.x][actual.y - 1] == false
		//) && ( // The bottom-bottom field must be a wall or unexplored [otw we could find a shorter path by skipping actual field]
		//	(actual.y - 2) < 0 || walls[actual.x][actual.y - 2] == true || visited[actual.x][actual.y - 2] == false
		//	) && ( // The right-bottom field must be a wall or unexplored
		//		(actual.x - 1) < 0 || walls[actual.x - 1][actual.y - 1] == true || visited[actual.x - 1][actual.y - 1] == false
		//		) && ( // The left-bottom field must be a wall or unexplored
		//			(actual.x + 1) >= XLimit || walls[actual.x + 1][actual.y - 1] == true || visited[actual.x + 1][actual.y - 1] == false
					)) {
		// Go down
		child.x = actual.x;
		child.y = actual.y - 1;
		if (findPath(child, path, walls, visited, finish)) {
			return true;
		}
	}

	//visited[actual.x][actual.y] = false;
	path.Pop();
	return false;
}

FString generateOils(TArray<TArray<bool>>& walls, TArray<Room>& path, int* id, FIntPoint oilInterval) {
	FString oil = "oil";
	FString data = "";

	int counter = 0;
	for (int index = 0; index < path.Num(); ++index) {
		counter++;
		if (counter < oilInterval.X) {
			continue;
		}
		Room actual = path[index];
		if (counter % oilInterval.Y == 0) {
			data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), (*id)++, actual.x, actual.y, 1, 1, *oil);
			counter = 0;
			continue;
		}
		int count = 0;
		if (actual.x > 0 && walls[actual.x - 1][actual.y] == false && (
				(actual.y == 0 || walls[actual.x - 1][actual.y - 1] == true) &&
			    ((actual.y == walls[actual.x - 1].Num() - 1) || walls[actual.x - 1][actual.y + 1] == true)
			)
		) {
			count++;
		}
		if (actual.x < walls.Num() - 1 && walls[actual.x + 1][actual.y] == false && (
				(actual.y == 0 || walls[actual.x + 1][actual.y - 1] == true) &&
				((actual.y == walls[actual.x + 1].Num() - 1) || walls[actual.x + 1][actual.y + 1] == true)
			)
		) {
			count++;
		}
		if (actual.y > 0 && walls[actual.x][actual.y - 1] == false && (
				(actual.x == 0 || walls[actual.x - 1][actual.y - 1] == true) &&
				((actual.x == walls.Num() - 1) || walls[actual.x + 1][actual.y - 1] == true)
			)
		) {
			count++;
		}
		if (actual.y < walls[actual.x].Num() - 1 && walls[actual.y][actual.y + 1] == false && (
				(actual.x == 0 || walls[actual.x - 1][actual.y + 1] == true) &&
				((actual.x == walls.Num() - 1) || walls[actual.x + 1][actual.y + 1] == true)
			)
		) {
			count++;
		}
		if (count > 2) {
			data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), (*id)++, actual.x, actual.y, 1, 1, *oil);
			counter = 0;
		}
	}
	return data;
}

UDataTable* UGenerateContentSceneComponent::GenerateData(const int Seed, FIntPoint OilInterval, const int DimensionSize, const int MinSpace) {
	FString data = "ID,X,Y,Width,Height,Type";
	int id = 0;
	FString type = "wall";
	FString start = "start";
	FString finish = "finish";
	FString treasure = "treasure";
	FMath::RandInit(Seed);

	if (OilInterval.X == 0) {
		OilInterval.X = 16;
	}

	if (OilInterval.Y == 0) {
		OilInterval.Y = 32;
	}

	// PREPARE MAP

	TArray<TArray<bool>> walls;
	for (int x = 0; x < DimensionSize; ++x) {
		TArray<bool> wall;
		wall.Init(false, DimensionSize);
		walls.Add(wall);
	}

    // INIT START, FINISH AND WALLS

	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, 0, 0, 1, 1, *start);
	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, DimensionSize - 1, DimensionSize - 1, 1, 1, *finish);
	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, -1, -1, DimensionSize + 1, 1, *type);
	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, DimensionSize, -1, 1, DimensionSize + 1, *type);
	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, -1, 0, 1, DimensionSize + 1, *type);
	data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, 0, DimensionSize, DimensionSize + 1, 1, *type);

	// DEFINE THE ROOM

	Room room;
	room.x = 0;
	room.y = 0;
	room.width = DimensionSize;
	room.height = DimensionSize;
	TArray<Room> stack;
	stack.Add(room);

	// GENERATE MAZE

	while (stack.Num() > 0) {
		// Current room to split - it should be doable
		Room actual = stack.Pop();
		Room a;
		Room b;
		int counter = 0;

		// If height or width is too small, split in the other orientation, otw random
		int horizontal = actual.height <= (2 * MinSpace) ? 0 : actual.width <= (2 * MinSpace) ? 1 : FMath::RandRange(0, 1);
		if (horizontal) {
			// Find the splitting coordinate
			int y = FMath::RandRange(MinSpace, actual.height - MinSpace - 1);

			// If there is an entrance to the room in the way of the wall on both sides, try to find another coordinate
			while ((actual.x > 0) && (walls[actual.x - 1][actual.y + y] == false) &&
				((actual.x + actual.width) < DimensionSize) && (walls[actual.x + actual.width][actual.y + y] == false)
			) {
				y = FMath::RandRange(MinSpace, actual.height - MinSpace - 1);

				UE_LOG(LogTemp, Warning, TEXT("%d"), counter);
				if (++counter == 10) {
					break;
				}
			}
			// If we tried 10 times and could not find suitable coordinate, give up and leave slightly bigger room
			if (counter == 10) continue;

			// Find a coordinate for doorway in the generated wall so that it does not block any entrances
			int x = (actual.x > 0) && (walls[actual.x - 1][actual.y + y] == false)
				? 0
				: ((actual.x + actual.width) < DimensionSize) && (walls[actual.x + actual.width][actual.y + y] == false)
				? (actual.width - MinSpace)
				: FMath::RandRange(0, actual.width - MinSpace);

			// Create two new rooms split by the generated wall

			a.x = actual.x;
			a.y = actual.y;
			a.height = y;
			a.width = actual.width;

			b.x = actual.x;
			b.y = actual.y + y + 1;
			b.height = actual.height - y - 1;
			b.width = actual.width;

			// Output those walls
			if (x != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x, actual.y + y, x, 1, *type);
				// Save to map for later
				for (int i = 0; i < x; ++i) {
					walls[actual.x + i][actual.y + y] = true;
				}
			}
			if ((actual.width - x - MinSpace) != 0) {
				data += FString::Printf(TEXT("\n%d,%d,%d,%d,%d,%s"), id++, actual.x + x + MinSpace, actual.y + y, actual.width - x - MinSpace, 1, *type);
				// Save to map for later
				for (int i = 0; i < actual.width - x - MinSpace; ++i) {
					walls[actual.x + x + MinSpace + i][actual.y + y] = true;
				}
			}
		}
		// Mirror for vertical
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

		// If it makes sense to split the room further, add it to stack
		if (((a.width > (MinSpace * 2)) && (a.height >= MinSpace)) || ((a.height > (MinSpace * 2)) && (a.width >= MinSpace))) {
			stack.Add(a);
		}
		if (((b.width > (MinSpace * 2)) && (b.height >= MinSpace)) || ((b.height > (MinSpace * 2)) && (b.width >= MinSpace))) {
			stack.Add(b);
		}
	}

	// GENERATE OIL

	// Use DFS to go through the maze and spawn oil with certain probability that increases depending on the distance from last spawned oil

	Room tile;
	tile.x = 0;
	tile.y = 0;
	tile.width = 0;
	stack.Add(tile);

	TArray<TArray<bool>> visited;
	for (int x = 0; x < DimensionSize; ++x) {
		TArray<bool> line;
		line.Init(false, DimensionSize);
		visited.Add(line);
	}

	bool result = findPath(tile, stack, walls, visited, FVector2D(DimensionSize - 1, DimensionSize - 1));
	if (!result) {
		FString error = "Could not find the exit.";
		UE_LOG(LogTemp, Error, TEXT("%s"), *error);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *data);
	}
	else {
		data += generateOils(walls, stack, &id, OilInterval);
	}

	// Log generated maze
	UE_LOG(LogTemp, Warning, TEXT("%s"), *data);

	// Load the data
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

