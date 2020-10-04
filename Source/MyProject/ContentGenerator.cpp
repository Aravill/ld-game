// Fill out your copyright notice in the Description page of Project Settings.


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

FString UContentGenerator::GenerateData(const int DimensionSize, const int MinSpace) {
	FString data = "ID;x;y;width;height;type";
	int id = 0;
	FString type = "wall";

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

		int horizontal = FMath::RandRange(0, 1);
		if (horizontal) {
			int y = FMath::RandRange(1, actual.height - 2);
			int x = FMath::RandRange(0, actual.width - MinSpace);

			a.x = actual.x;
			a.y = actual.y;
			a.height = y;
			a.width = actual.width;

			b.x = actual.x;
			b.y = actual.y + y + 1;
			b.height = actual.height - y - 1;
			b.width = actual.width;

			if(x != 0) {
				data += FString::Printf(TEXT("\n%d;%d;%d;%d;%d;%s"), id++, actual.x, actual.y + y, x, 1, *type);
			}
			if((actual.width - x - MinSpace) != 0) {
				data += FString::Printf(TEXT("\n%d;%d;%d;%d;%d;%s"), id++, actual.x + x + MinSpace, actual.y + y, actual.width - x - MinSpace, 1, *type);
			}
		}
		else {
			int x = FMath::RandRange(1, actual.width - 2);
			int y = FMath::RandRange(0, actual.height - MinSpace);

			a.x = actual.x;
			a.y = actual.y;
			a.height = actual.height;
			a.width = x;

			b.x = actual.x + x + 1;
			b.y = actual.y;
			b.height = actual.height;
			b.width = actual.width - x - 1;

			if(y != 0) {
				data += FString::Printf(TEXT("\n%d;%d;%d;%d;%d;%s"), id++, actual.x + x, actual.y, 1, y, *type);
			}
			if((actual.height - y - MinSpace) != 0) {
				data += FString::Printf(TEXT("\n%d;%d;%d;%d;%d;%s"), id++, actual.x + x, actual.y + y + MinSpace, 1, actual.height - y - MinSpace, *type);
			}
		}

		if (a.width > MinSpace && a.height > MinSpace) {
			stack.Add(a);
		}
		if (b.width > MinSpace && b.height > MinSpace) {
			stack.Add(b);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *data);

	return data;
}
//
//UDataTable *UContentGenerator::GenerateMap() {
//	UDataTable datatable; // FIXME: Another approach, maybe shared or unique pointers
//	FString data = generateData();
//	datatable.CreateTableFromCSVString(data);
//	return &datatable;
//}
