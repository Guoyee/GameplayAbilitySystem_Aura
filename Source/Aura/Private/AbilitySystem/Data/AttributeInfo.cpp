// Copyright Yerik Guo


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Misc/TextFilterExpressionEvaluator.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	// 查找 Tag
	for (const FAuraAttributeInfo& Info : AttributeInfoList)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
    
	// 未找到，记录日志
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttributeInfo not found for Tag [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
    
	return FAuraAttributeInfo();
}
