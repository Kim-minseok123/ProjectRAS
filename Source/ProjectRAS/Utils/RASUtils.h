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
    template<typename T>
    FORCEINLINE static T& RandomArrayItemFromStream(TArray<T>& Array, FRandomStream& Stream)
    {
        check(Array.Num() > 0);
        const int32 Index = Stream.RandRange(0, Array.Num() - 1);
        return Array[Index];
    }
    template<typename T>
    static void ShuffleTArray(TArray<T>& Array, FRandomStream& Stream)
    {

        const int32 NumElements = Array.Num();
        for (int32 i = NumElements - 1; i > 0; --i)
        {
            int32 RandomIndex = Stream.RandRange(0, i);

            if (i != RandomIndex)
            {
                Array.Swap(i, RandomIndex);
            }
        }
    }

    static const float GetCurrentPlatformTime()
    {
		static const double StartTime = FPlatformTime::Seconds();
		return static_cast<float>(FPlatformTime::Seconds() - StartTime);
    }
protected:
};
