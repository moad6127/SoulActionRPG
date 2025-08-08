// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameplayTags.h"

//FSoulGameplayTags FSoulGameplayTags::GameplayTags;
//
//void FSoulGameplayTags::InitializeNativeGameplayTags()
//{
//	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces dagame taken, improves block chance"));
//}


namespace SoulGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attributes_Secondary_Armor, "Attributes.Secondary.Armor", "Reduces dagame taken, improves block chance");
}
