// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.

int32 ICombatInterface::GetPlayerLevel()
{
	return int32();
}

void ICombatInterface::Equip(ABaseWeapon* Weapon)
{
}

bool ICombatInterface::bInitWeaponGet() const
{
	return false;
}



