// Fill out your copyright notice in the Description page of Project Settings.

#include "RollerGame.h"
#include "RollerGameGameMode.h"
#include "PlayerSphere.h"



ARollerGameGameMode::ARollerGameGameMode()
{
	DefaultPawnClass = APlayerSphere::StaticClass();
}