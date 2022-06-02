// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTasks/AT_ListenForAttributeChange.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

UAT_ListenForAttributeChange* UAT_ListenForAttributeChange::ListenForAttributeChange(
	UAbilitySystemComponent* InAbilitySystemComponent, FGameplayAttribute InAttribute)
{
	UAT_ListenForAttributeChange* Object = NewObject<UAT_ListenForAttributeChange>();
	Object->AbilitySystemComponent = InAbilitySystemComponent;
	Object->GameplayAttribute = InAttribute;

	// Verify our ASC and Attribute is valid
	if(!InAbilitySystemComponent || !InAttribute.IsValid())
	{
		Object->RemoveFromRoot();
		return nullptr;
	}

	Object->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Object->GameplayAttribute).AddUObject(Object, &UAT_ListenForAttributeChange::OnAttributeChanged);
	return Object;
}

UAT_ListenForAttributeChange* UAT_ListenForAttributeChange::ListenForAttributesChanged(
	UAbilitySystemComponent* InAbilitySystemComponent, TArray<FGameplayAttribute> InAttributes)
{
	UAT_ListenForAttributeChange* Object = NewObject<UAT_ListenForAttributeChange>();
	Object->AbilitySystemComponent = InAbilitySystemComponent;
	Object->GameplayAttributes = InAttributes;

	if(!InAbilitySystemComponent || InAttributes.Num() <= 0)
	{
		Object->RemoveFromRoot();
		return nullptr;
	}

	for(const FGameplayAttribute& Attribute : Object->GameplayAttributes)
	{
		Object->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(Object, &UAT_ListenForAttributeChange::OnAttributeChanged);
	}

	return Object;
}

void UAT_ListenForAttributeChange::OnAttributeChanged(const FOnAttributeChangeData& Data) const
{
	const FGameplayAttribute Attribute = Data.Attribute;
	const float NewValue = Data.NewValue;
	const float OldValue = Data.OldValue;

	if(NewValue == OldValue)
	{
		return;
	}

	GameplayAttributeChanged.Broadcast(Attribute, NewValue, OldValue);
}

void UAT_ListenForAttributeChange::EndTask()
{
	// Remove all our bindings to our Ability System Component if they are still valid
	if(IsValid(AbilitySystemComponent))
	{
		if(GameplayAttribute.IsValid())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GameplayAttribute).RemoveAll(this);
		}

		if(GameplayAttributes.Num() > 0)
		{
			for(const FGameplayAttribute& Attribute : GameplayAttributes)
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
			}
		}
	}

	// Set ready to be destroyed and mark for garbage collection
	SetReadyToDestroy();
	MarkAsGarbage();
}
