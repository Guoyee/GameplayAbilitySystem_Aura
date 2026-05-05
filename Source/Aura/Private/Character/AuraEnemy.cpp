// Copyright Yerik Guo


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widgets/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

    HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
    HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);
    Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
    return Level;
}

// [Claude] 死亡时隐藏头顶血条，防止 Mesh ragdoll 脱离 Capsule 后血条悬空漂移
void AAuraEnemy::OnDeath()
{
    HealthBar->SetHiddenInGame(true);
}

void AAuraEnemy::Die()
{
    SetLifeSpan(LifeSpan);
    Super::Die();
}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

    //通过ECharacterClass查找并设置default Attributes
    //该部分仅在服务端执行（属性会被复制到客户端，且客户端没有 Game Mode 信息）
    if (HasAuthority())
    {
        TObjectPtr<UCharacterClassInfo> CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(this);
        if (CharacterClassInfo)
        {
            DefaultPrimaryAttributes = CharacterClassInfo->GetClassDefaultInfo(CharacterClass).PrimaryAttributes;
            DefaultSecondaryAttributes = CharacterClassInfo->SecondaryAttributes;
            DefaultVitalAttributes = CharacterClassInfo->VitalAttributes;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No character class info found!"));
        }
    }

    //更新完Default Attributes后 再初始化！！！
    InitAbilityActorInfo();

    //设置controller用于触发委托事件
    if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
    {
        AuraUserWidget->SetWidgetController(this);
    }

    if (UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
            {
                OnHealthChanged.Broadcast(Data.NewValue);
            }
        );

        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
        [this](const FOnAttributeChangeData& Data)
            {
                OnMaxHealthChanged.Broadcast(Data.NewValue);
            }
        );
        // [Claude] 注册 HitReact Tag 监听，控制受击时减速/停止移动
        AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
            this,
            &AAuraEnemy::HitReactTagChanged
        );

        //更新初始值
        OnHealthChanged.Broadcast(AuraAS->GetHealth());
        OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
    }

}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    bHitReacting = NewCount > 0;
    GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::InitAbilityActorInfo()
{
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

    InitializeDefaultAttributes();
    // [Claude] 赋予敌人 StartupAbilities 中的 GA（如受击反馈 GA_HitReact）
    AddCharacterAbilities();
}
