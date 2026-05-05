// Copyright Yerik Guo


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
    //创建 FGameplayEffectAttributeCaptureDefinition类型的对象 ANameDef
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

    AuraDamageStatics()
    {
        //设置ANameDef的值
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
    }
};

static const AuraDamageStatics& DamageStatics()
{
    /*    
     *    Meyers Singleton（迈耶斯单例）：
     *    1.C++11标准保证：函数内的 static 局部变量具有静态存储期，仅初始化一次，初始化过程是线程安全的
     *    2.static 局部对象的析构顺序与构造顺序相反，在 main 返回或 exit 之后（程序完全退出后）执行
    */
    static AuraDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    //在这里提前Capture需要的属性 ， 如果没有添加对应的属性 AttemptCalculateCapturedAttributeMagnitude 会返回 false，拿不到对应的值
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    
    AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTags;
    EvaluateParameters.TargetTags = TargetTags;
    
    /*
     *根据捕获的值计算Damage
    */
    float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
    
    //Capture BlockCance on Target, and determine if there was a successful Block
    //计算格挡，被格挡伤害减半
    
    float TargetBlockChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
    TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);
    bool bBlocked = FMath::RandRange(0, 99) < TargetBlockChance;
    Damage = bBlocked ? Damage/2.f : Damage;
    
    
    //计算护甲/护甲穿透
    float TargetArmor = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters, TargetArmor);
    TargetArmor = FMath::Max(TargetArmor, 0.f);
    
    float SourceArmorPenetration = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters, SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);
    const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration)/100.f;
    
    Damage *= 100.f / (EffectiveArmor + 100.f);
    
    // [Claude] 计算暴击：暴击几率 - 目标暴击抗性 = 有效暴击几率，暴击伤害系数 = 1.5 + CritDamage/100
    float SourceCriticalHitChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
    SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.f);

    float SourceCriticalHitDamage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
    SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.f);

    float TargetCriticalHitResistance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
    TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.f);

    const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance;
    bool bCriticalHit = FMath::RandRange(0, 99) < EffectiveCriticalHitChance;
    Damage = bCriticalHit ? Damage * (1.5f + SourceCriticalHitDamage / 100.f) : Damage;

    
    
    //应用计算结果
    const FGameplayModifierEvaluatedData EvaluationData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluationData);
}
