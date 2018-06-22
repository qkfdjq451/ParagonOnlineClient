// Fill out your copyright notice in the Description page of Project Settings.

#include "DeveloperUI.h"
#include "Engine.h"
#include "Navigation/RecastHelpers.h"
#include "NavigationMetaData.h"
#include "Navigation/PImplRecastNavMesh.h"

void UDeveloperUI::ExportNavimesh()
{
	FWorldContext Context = GEngine->GetWorldContexts().Last();
	UWorld *World = Context.World();

	UNavigationSystem* NavSys = World->GetNavigationSystem();
	ANavigationData* NavData = NavSys->GetMainNavData(FNavigationSystem::ECreateIfEmpty::DontCreate);
	
	ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	NavMesh->GetGenerator()->ExportNavigationData(FString(TEXT("C:\\Users\\user\\Downloads\\recastnavigation-1.5.1\\RecastDemo\\Bin\\Meshes\\Test")));
	
}