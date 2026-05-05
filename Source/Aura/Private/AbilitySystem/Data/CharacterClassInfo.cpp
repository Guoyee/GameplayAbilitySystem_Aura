// Copyright Yerik Guo


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass characterClass)
{
    return CharacterClassInformation.FindChecked(characterClass);
}
