#if !defined( ENUMS_H )
#define ENUMS_H

#include "common.h"

typedef enum Button_t
{
   Button_Left = 0,
   Button_Up,
   Button_Right,
   Button_Down,
   Button_A,
   Button_B,

   Button_Count
}
Button_t;

typedef enum Direction_t
{
   Direction_Left = 0,
   Direction_Up,
   Direction_Right,
   Direction_Down,

   Direction_Count
}
Direction_t;

typedef enum MainState_t
{
   MainState_Overworld = 0,
   MainState_Battle,

   MainState_Count
}
MainState_t;

typedef enum SubState_t
{
   SubState_None = 0,
   SubState_Waiting,
   SubState_Menu,
   SubState_Dialog,

   SubState_Count
}
SubState_t;

typedef enum AnimationId_t
{
   AnimationId_Overworld_Pause = 0,
   AnimationId_TileMap_FadeOut,
   AnimationId_TileMap_FadePause,
   AnimationId_TileMap_FadeIn,
   AnimationId_TileMap_WhiteOut,
   AnimationId_TileMap_WhitePause,
   AnimationId_TileMap_WhiteIn,
   AnimationId_CastSpell,
   AnimationId_RainbowBridge_Trippy,
   AnimationId_RainbowBridge_WhiteOut,
   AnimationId_RainbowBridge_FadeIn,
   AnimationId_RainbowBridge_Pause,
   AnimationId_Battle_Checkerboard,
   AnimationId_Battle_EnemyFadeIn,
   AnimationId_Battle_EnemyFadeOut,
   AnimationId_Battle_EnemyFadeInPause,
   AnimationId_Battle_EnemyDamage,
   AnimationId_Battle_EnemyDodge,
   AnimationId_Battle_VictoryPause,

   AnimationId_Count
}
AnimationId_t;

typedef enum Item_t
{
   Item_Key = 0,
   Item_Herb,
   Item_Wing,
   Item_FairyWater,
   Item_Torch,
   Item_FairyFlute,
   Item_SilverHarp,
   Item_GwaelynsLove,
   Item_StoneOfSunlight,
   Item_StaffOfRain,
   Item_Token,
   Item_RainbowDrop,
   Item_SphereOfLight,
   Item_DragonScale,
   Item_CursedBelt,

   Item_Count
}
Item_t;

typedef enum SpecialEnemy_t
{
   SpecialEnemy_None = 0,
   SpecialEnemy_GreenDragon,
   SpecialEnemy_AxeKnight,
   SpecialEnemy_Golem,
   SpecialEnemy_Dragonlord,

   SpecialEnemy_Count
}
SpecialEnemy_t;

typedef enum MenuId_t
{
   MenuId_Overworld = 0,
   MenuId_OverworldSpell,
   MenuId_OverworldItem,
   MenuId_Zoom,

   MenuId_Battle,
   MenuId_BattleSpell,

   MenuId_Count
}
MenuId_t;

typedef enum MenuCommand_t
{
   MenuCommand_Overworld_Talk = 0,
   MenuCommand_Overworld_Status,
   MenuCommand_Overworld_Search,
   MenuCommand_Overworld_Spell,
   MenuCommand_Overworld_Item,
   MenuCommand_Overworld_Door,

   MenuCommand_Spell_Heal,
   MenuCommand_Spell_Sizz,
   MenuCommand_Spell_Sleep,
   MenuCommand_Spell_Glow,
   MenuCommand_Spell_Fizzle,
   MenuCommand_Spell_Evac,
   MenuCommand_Spell_Zoom,
   MenuCommand_Spell_Repel,
   MenuCommand_Spell_Midheal,
   MenuCommand_Spell_Sizzle,

   MenuCommand_Item_Herb,
   MenuCommand_Item_Wing,
   MenuCommand_Item_FairyWater,
   MenuCommand_Item_Torch,
   MenuCommand_Item_FairyFlute,
   MenuCommand_Item_SilverHarp,
   MenuCommand_Item_GwaelynsLove,
   MenuCommand_Item_RainbowDrop,
   MenuCommand_Item_CursedBelt,

   MenuCommand_Zoom_Tantegel,
   MenuCommand_Zoom_Brecconary,
   MenuCommand_Zoom_Garinham,
   MenuCommand_Zoom_Kol,
   MenuCommand_Zoom_Cantlin,
   MenuCommand_Zoom_Rimuldar,

   MenuCommand_Battle_Attack,
   MenuCommand_Battle_Flee,
   MenuCommand_Battle_Spell,
   MenuCommand_Battle_Item,

   MenuCommand_Count
}
MenuCommand_t;

typedef enum DialogId_t
{
   DialogId_Spell_None = 0,
   DialogId_Spell_OverworldCantCast,
   DialogId_Spell_CastHeal1,
   DialogId_Spell_CastHeal2,
   DialogId_Spell_CastMidheal1,
   DialogId_Spell_CastMidheal2,
   DialogId_Spell_OverworldCastGlowCursed,
   DialogId_Spell_OverworldCastGlow,
   DialogId_Spell_CastRepelCursed,
   DialogId_Spell_CastRepel,
   DialogId_Spell_CastEvacCursed,
   DialogId_Spell_CastEvac,
   DialogId_Spell_CastZoom,
   DialogId_Spell_NotEnoughMp,
   DialogId_Spell_Blocked,
   DialogId_Item_None,

   DialogId_FullyHealed,
   DialogId_HolyProtection_Off,

   DialogId_Use_Herb1,
   DialogId_Use_Herb2,
   DialogId_Use_WingCantUse,
   DialogId_Use_Wing,
   DialogId_Use_FairyWaterCursed,
   DialogId_Use_FairyWater,
   DialogId_Use_TorchCantUse,
   DialogId_Use_TorchAlreadyUsed,
   DialogId_Use_Torch,
   DialogId_Use_FairyFlute,
   DialogId_Use_SilverHarp,
   DialogId_Use_GwaelynsLoveCantUse,
   DialogId_Use_GwaelynsLove,
   DialogId_Use_RainbowDropCantUse,
   DialogId_Use_RainbowDrop,
   DialogId_Use_CursedBelt,

   DialogId_Chest_DeathNecklace,

   DialogId_Battle_EnemyApproaches,
   DialogId_Battle_AttackAttemptSucceeded,
   DialogId_Battle_AttackAttemptFailed,
   DialogId_Battle_FleeAttemptSucceeded,
   DialogId_Battle_FleeAttemptFailed,
   DialogId_Battle_Victory,
   DialogId_Battle_VictoryWithSpoils,
   DialogId_Battle_VictoryWithLevelUp,
   DialogId_Battle_Spell_None,
   DialogId_Battle_Spell_CantCast,
   DialogId_Battle_Spell_NotEnoughMp,
   DialogId_Battle_Spell_Blocked,
   DialogId_Battle_Spell_Fizzled,
   DialogId_Battle_FullyHealed,
   DialogId_Battle_Spell_NoEffect,
   DialogId_Battle_Spell_AttackSucceeded,
   DialogId_Battle_Spell_Sizz,
   DialogId_Battle_Spell_Sizzle,

   DialogId_Count
}
DialogId_t;

#endif // ENUMS_H
