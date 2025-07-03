// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RASFadeWidget.h"
#include "Animation/WidgetAnimation.h" 

URASFadeWidget::URASFadeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URASFadeWidget::FadeIn()
{
	PlayAnimation(FadeInAnim);
    float Duration = FadeInAnim->GetEndTime();

    FTimerHandle TempHandle; 

    GetWorld()->GetTimerManager().SetTimer(
        TempHandle,
        FTimerDelegate::CreateWeakLambda(this, [this]()
            {
                this->RemoveFromParent();
            }),
        Duration,
        false
    );
}

void URASFadeWidget::FadeOut()
{
    PlayAnimation(FadeOutAnim);
    float Duration = FadeOutAnim->GetEndTime();

    FTimerHandle TempHandle;

    GetWorld()->GetTimerManager().SetTimer(
        TempHandle,
        FTimerDelegate::CreateWeakLambda(this, [this]()
            {
                this->RemoveFromParent();
            }),
        Duration,
        false
    );
}
