// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformTime.h"

/**
 * 
 */
class PROJECTRAS_API RASUtils
{
public:
    RASUtils();
	~RASUtils();
    template <typename T>
    static T* FindComponentByName(class USceneComponent* Root, const FName& Name)
    {
        if (!Root) return nullptr;

        for (USceneComponent* Child : Root->GetAttachChildren())
        {
            if (Child && Child->GetName() == Name.ToString())
            {
                return Cast<T>(Child);
            }
        }
        return nullptr;
    }

    static const float GetCurrentPlatformTime()
    {
		static const double StartTime = FPlatformTime::Seconds();
		return static_cast<float>(FPlatformTime::Seconds() - StartTime);
    }
protected:
};
