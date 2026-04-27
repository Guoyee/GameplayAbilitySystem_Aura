# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Aura is an Action/RPG top-down game built with **Unreal Engine 5.3**. It uses the **Gameplay Ability System (GAS)** for combat/attributes, **Enhanced Input** for controls, and click-to-move navigation. Single C++ module named `Aura`.

## Build & Development

```bash
# Build the editor target (most common during development)
<UE5-Engine>\Engine\Build\BatchFiles\Build.bat AuraEditor Win64 Development -Project="D:\UE5\Aura\Aura.uproject" -WaitMutex

# Build the game target
<UE5-Engine>\Engine\Build\BatchFiles\Build.bat Aura Win64 Development -Project="D:\UE5\Aura\Aura.uproject" -WaitMutex

# Generate Visual Studio project files
<UE5-Engine>\Engine\Build\BatchFiles\GenerateProjectFiles.bat -Project="D:\UE5\Aura\Aura.uproject" -Game -Engine
```

Replace `<UE5-Engine>` with the actual UE 5.3 engine installation path.

## Architecture

### Ability System

The project uses a **dual ASC ownership** model:

- **Player**: `AAuraPlayerState` owns the `UAuraAbilitySystemComponent` and `UAuraAttributeSet` (created as subobjects). Replication mode: `Mixed`. This persists ASC across character death/respawn.
- **Enemy**: `AAuraEnemy` owns its own ASC and AttributeSet as subobjects. Replication mode: `Minimal`.

`AAuraCharacterBase` implements `IAbilitySystemInterface` and holds `TObjectPtr<>` pointers to ASC and AS. These are assigned during `InitAbilityActorInfo()`:
- `AAuraCharacter::PossessedBy()` triggers it on the server
- `AAuraCharacter::OnRep_PlayerState()` triggers it on the client (for the player)
- The method stores the ASC/AS pointers, calls `AbilityActorInfoSet()` (which broadcasts a delegate used by widget controllers), applies default attributes via `InitializeDefaultAttributes()`, and creates the HUD overlay

`AddCharacterAbilities()` is called only with `HasAuthority()` — grants abilities whose `StartupInputTag` is added to `DynamicAbilityTags` on the ASC.

### Attribute Hierarchy

Three layers of attributes, all `FGameplayAttributeData` with full replication:

1. **Primary** (4): Strength, Intelligence, Resilience, Vigor — base stats
2. **Secondary** (10): Armor, MaxHealth, MaxMana, etc. — derived from primaries via MMCs
3. **Vital** (4): Health, Mana, MaxHealth, MaxMana — dynamic resources

MMCs use `ICombatInterface::GetPlayerLevel()` for level scaling (e.g., `MaxHealth = 80 + 2.5*Vigor + 10*PlayerLevel`).

Tags are defined in `FAuraGameplayTags` (singleton struct, initialized in `UAuraAssetManager::StartInitialLoading()`). `TagsToAttributeMap` in `UAuraAttributeSet` maps tags to attributes for UI lookups.

### Input & Ability Binding

Enhanced Input with a tag-driven binding system:

1. `UAuraInputConfig` (DataAsset) maps `UInputAction` → `FGameplayTag` (e.g., `IA_LMB` → `InputTag.LMB`)
2. `UAuraInputComponent::BindAbilityActions()` iterates the config and binds each action to press/release/held handlers on `AAuraPlayerController`
3. The controller's handlers check for RMB (click-to-move/pathfind) vs. ability inputs (delegated to `UAuraAbilitySystemComponent::AbilityInputTagHeld/Released()`)
4. The ASC searches `ActivatableAbilities` for matching `DynamicAbilityTags`

Abilities declare their input via `UAuraGameplayAbility::StartupInputTag`.

### Widget Controllers

`AAuraHUD` lazily creates and caches widget controllers (`UOverlayWidgetController`, `UAttributeMenuWidgetController`). Bound via `FWidgetControllerParams` (bundles PC, PS, ASC, AS). Each controller:
1. `SetWidgetControllerParams()` — stores references
2. `BindCallbacksToDependencies()` — binds lambdas to ASC attribute change delegates
3. `BroadcastInitialValues()` — pushes current values to UI

`UAuraAbilitySystemLibrary` provides static `GetOverlayWidgetController()` / `GetAttributeMenuWidgetController()` for Blueprint access.

### Effect Actor Pattern

`AAuraEffectActor` handles Instant, Duration, and Infinite gameplay effects with configurable application/removal policies per effect. Tracks active infinite effect handles in a `TMap<UAbilitySystemComponent*, TArray<FActiveGameplayEffectHandle>>` for cleanup.

### Projectile Spawning

`UAuraProjectileSpell` uses UE5's deferred spawn pattern: animation notifies call `BeginProjectileSpawn()` (gets socket location, spawns deferred) then `FinishProjectileSpawn()` (finalizes actor).

### Interfaces

- `ICombatInterface` — `GetPlayerLevel()`, `GetCombatSocketLocation()` (used by MMCs and projectile spells)
- `IEnemyInterface` — `HighlightActor()` / `UnHighlightActor()` (uses custom depth stencil `CUSTOM_DEPTH_RED = 250`)

### Click-to-Move

Short-press: `UNavigationSystemV1::FindPathToLocationSynchronously` + `USplineComponent` for auto-running. Held: direct movement following cursor. Targeting mode: holding LMB/RMB triggers abilities on release via `TargetDataUnderMouse`.

## Coding Conventions

- **Copyright header**: `// Copyright Yerik Guo` at the top of every source file
- **Includes**: `#pragma once` in headers; includes are module-relative from `Source/Aura/` root (e.g., `#include "AbilitySystem/AuraAttributeSet.h"`)
- **Naming**: `AAura` prefix for Actors, `UAura` for UObjects, `I` for interfaces, `F` for structs. Member variables PascalCase. Booleans use `b` prefix (`bTargeting`, `bAutoRunning`).
- **Pointers**: `TObjectPtr<>` for all UObject pointers (UE5.1+ convention)
- **Constructor pattern**: Set `PrimaryActorTick.bCanEverTick = false` unless ticking is needed
- **UPROPERTY categories**: Grouped logically (e.g., `"Attributes|Primary"`, `"Combat"`, `"Input"`)
- **Access modifiers**: `public:`, `protected:`, `private:` in that order
- **C++17 features**: Structured bindings used (e.g., iterating `TMap` pairs)

## Module Dependencies

- **Public**: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `GameplayAbilities`
- **Private**: `GameplayTags`, `GameplayTasks`, `NavigationSystem`

## Asset Naming

| Prefix | Type |
|--------|------|
| `BP_` | Blueprint |
| `WBP_` | Widget Blueprint |
| `GA_` | Gameplay Ability |
| `GE_` | Gameplay Effect |
| `DA_` | Data Asset |
| `DT_` | Data Table |
| `IA_` | Input Action |
| `IMC_` | Input Mapping Context |
| `ABP_` | Animation Blueprint |
| `NS_` | Niagara System |
| `MI_` / `M_` | Material Instance / Material |
| `SM_` / `SK_` | Static Mesh / Skeletal Mesh |
| `T_` | Texture |
