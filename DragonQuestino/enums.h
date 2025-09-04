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
   Button_Start,

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
   MainState_Startup = 0,
   MainState_EnterName,
   MainState_EnterPassword,
   MainState_Overworld,
   MainState_Battle,
   MainState_Ending_1,
   MainState_Ending_2,

   MainState_Count
}
MainState_t;

typedef enum SubState_t
{
   SubState_None = 0,
   SubState_Menu,
   SubState_Dialog,
   SubState_Status,
   SubState_NonUseableItems,
   SubState_BinaryChoice,
   SubState_ShopMenu,

   SubState_Count
}
SubState_t;

typedef enum AnimationId_t
{
   AnimationId_Pause = 0,
   AnimationId_ActivePause,
   AnimationId_FadeOut,
   AnimationId_MidFadeOut,
   AnimationId_SlowFadeOut,
   AnimationId_FadeIn,
   AnimationId_MidFadeIn,
   AnimationId_ActiveMidFadeIn,
   AnimationId_WhiteOut,
   AnimationId_WhiteIn,
   AnimationId_CastSpell,
   AnimationId_TileDeath,
   AnimationId_RainbowBridge_Trippy,
   AnimationId_RainbowBridge_WhiteOut,
   AnimationId_RainbowBridge_FadeIn,
   AnimationId_Battle_Checkerboard,
   AnimationId_Battle_EnemyFadeIn,
   AnimationId_Battle_EnemySlowFadeIn,
   AnimationId_Battle_EnemyFadeOut,
   AnimationId_Battle_EnemySlowFadeOut,
   AnimationId_Battle_EnemyFadeInPause,
   AnimationId_Battle_EnemyDamage,
   AnimationId_Battle_EnemyDodge,
   AnimationId_Battle_PlayerDamage,
   AnimationId_Battle_PlayerDeath,
   AnimationId_Ending_WalkFade,

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

typedef enum Spell_t
{
   Spell_Heal = 0,
   Spell_Sizz,
   Spell_Sleep,
   Spell_Glow,
   Spell_Fizzle,
   Spell_Evac,
   Spell_Zoom,
   Spell_Repel,
   Spell_Midheal,
   Spell_Sizzle,

   Spell_Count
}
Spell_t;

typedef enum SpecialEnemy_t
{
   SpecialEnemy_None = 0,
   SpecialEnemy_GreenDragon,
   SpecialEnemy_AxeKnight,
   SpecialEnemy_Golem,
   SpecialEnemy_DragonlordWizard,
   SpecialEnemy_DragonlordDragon,

   SpecialEnemy_Count
}
SpecialEnemy_t;

typedef enum MenuId_t
{
   MenuId_Startup = 0,
   MenuId_Overworld,
   MenuId_OverworldSpell,
   MenuId_OverworldItem,
   MenuId_Zoom,

   MenuId_Battle,
   MenuId_BattleSpell,
   MenuId_BattleItem,

   MenuId_Count
}
MenuId_t;

typedef enum MenuCommand_t
{
   MenuCommand_Startup_NewGame = 0,
   MenuCommand_Startup_EnterPassword,

   MenuCommand_Overworld_Talk,
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

typedef enum BattleTurn_t
{
   BattleTurn_Player = 0,
   BattleTurn_Enemy,

   BattleTurn_Count
}
BattleTurn_t;

typedef enum AccessoryType_t
{
   AccessoryType_Weapon = 0,
   AccessoryType_Armor,
   AccessoryType_Shield,

   AccessoryType_Count
}
AccessoryType_t;

typedef enum ShopType_t
{
   ShopType_Weapon,
   ShopType_Item,

   ShopType_Count
}
ShopType_t;

typedef enum TileTextureType_t
{
   TileTextureType_Title = 0,
   TileTextureType_Map,
   TileTextureType_TheEnd,

   TileTextureType_Count
}
TileTextureType_t;

#endif // ENUMS_H
