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

typedef enum GameState_t
{
   GameState_Overworld = 0,
   GameState_Overworld_Waiting,
   GameState_Overworld_MainMenu,
   GameState_Overworld_SpellMenu,
   GameState_Overworld_ItemMenu,
   GameState_Overworld_ScrollingDialog,

   GameState_Count
}
GameState_t;

typedef enum Animation_t
{
   Animation_Overworld_Pause = 0,
   Animation_TileMap_FadeOut,
   Animation_TileMap_FadeIn,
   Animation_RainbowBridge_Trippy,
   Animation_RainbowBridge_Whiteout,
   Animation_RainbowBridge_FadeIn,
   Animation_RainbowBridge_Pause,

   Animation_Count
}
Animation_t;

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

typedef enum MenuId_t
{
   MenuId_Overworld = 0,
   MenuId_OverworldSpell,
   MenuId_OverworldItem,

   MenuId_Count
}
MenuId_t;

typedef enum MenuCommand_t
{
   MenuCommand_OverworldMain_Talk = 0,
   MenuCommand_OverworldMain_Status,
   MenuCommand_OverworldMain_Search,
   MenuCommand_OverworldMain_Spell,
   MenuCommand_OverworldMain_Item,
   MenuCommand_OverworldMain_Door,

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

   MenuCommand_Count
}
MenuCommand_t;

typedef enum ScrollingDialogType_t
{
   ScrollingDialogType_Overworld = 0,

   ScrollingDialogType_Count
}
ScrollingDialogType_t;

typedef enum DialogMessageId_t
{
   DialogMessageId_Talk_NobodyThere = 0,
   DialogMessageId_Search_NothingFound,
   DialogMessageId_Search_FoundItem,
   DialogMessageId_Search_FoundHiddenStairs,
   DialogMessageId_Spell_None,
   DialogMessageId_Spell_OverworldCantCast,
   DialogMessageId_Spell_NotEnoughMp,
   DialogMessageId_Item_None,
   DialogMessageId_Door_None,
   DialogMessageId_Door_NoKeys,

   DialogMessageId_Use_Herb,
   DialogMessageId_Use_Wing,
   DialogMessageId_Use_FairyWater,
   DialogMessageId_Use_TorchCantUse,
   DialogMessageId_Use_Torch,
   DialogMessageId_Use_FairyFlute,
   DialogMessageId_Use_SilverHarp,
   DialogMessageId_Use_GwaelynsLoveCantUse,
   DialogMessageId_Use_GwaelynsLove,
   DialogMessageId_Use_RainbowDropCantUse,
   DialogMessageId_Use_RainbowDrop,
   DialogMessageId_Use_CursedBelt,

   DialogMessageId_Chest_ItemCollected,
   DialogMessageId_Chest_ItemNoSpace,
   DialogMessageId_Chest_GoldCollected,
   DialogMessageId_Chest_GoldNoSpace,
   DialogMessageId_Chest_Tablet,
   DialogMessageId_Chest_DeathNecklace,

   DialogMessageId_Count
}
DialogMessageId_t;

#endif // ENUMS_H
