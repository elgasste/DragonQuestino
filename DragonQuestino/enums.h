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
   GameState_Overworld_Washing,
   GameState_Overworld_MainMenu,
   GameState_Overworld_ItemMenu,
   GameState_Overworld_ScrollingDialog,
   GameState_Overworld_RainbowBridgeTrippyAnimation,
   GameState_Overworld_RainbowBridgeWhiteoutAnimation,
   GameState_Overworld_RainbowBridgePauseAnimation,
   GameState_TileMapTransition,

   GameState_Count
}
GameState_t;

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

   MenuCommand_OverworldItem_Herb,
   MenuCommand_OverworldItem_Wing,
   MenuCommand_OverworldItem_FairyWater,
   MenuCommand_OverworldItem_Torch,
   MenuCommand_OverworldItem_FairyFlute,
   MenuCommand_OverworldItem_SilverHarp,
   MenuCommand_OverworldItem_GwaelynsLove,
   MenuCommand_OverworldItem_RainbowDrop,
   MenuCommand_OverworldItem_CursedBelt,

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
   DialogMessageId_Spell_None,
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
