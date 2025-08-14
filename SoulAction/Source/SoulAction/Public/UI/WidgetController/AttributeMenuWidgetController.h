// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/SoulWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
struct FSoulAttributeInfo;
class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FSoulAttributeInfo&, Info);


UCLASS(BlueprintType, Blueprintable)
class SOULACTION_API UAttributeMenuWidgetController : public USoulWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category ="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfomation;
};
