using System.CodeDom;

namespace DragonQuestinoEditor
{
   public static class Constants
   {
      public const string AssetsBasePath = "..\\..\\..\\..\\..\\Assets\\";
      public const string SourceBasePath = "..\\..\\..\\..\\..\\DragonQuestino\\";

      public const string ShieldBackgroundFilePath = AssetsBasePath + "shield_background.png";
      public const string TileTexturesFilePath = AssetsBasePath + "tile_textures.png";
      public const string EditorSaveDataFilePath = AssetsBasePath + "editor_save.json";
      public const string StaticSpriteSheetFilePath = AssetsBasePath + "static_sprites.png";
      public const string TextTileSetFilePath = AssetsBasePath + "text_tileset.png";

      public static readonly string[] ActiveSpriteSheetPaths = [
         AssetsBasePath + "player_sprite.png",
         AssetsBasePath + "king_sprite.png",
         AssetsBasePath + "princess_sprite.png",
         AssetsBasePath + "soldier_sprite.png",
         AssetsBasePath + "blue_hair_man_sprite.png"
      ];

      public const string GameDataSourceFilePath = SourceBasePath + "game_data.c";
      public const string ShieldBackgroundSourceFilePath = SourceBasePath + "giga_shield_background_data.c";
      public const string GeneratedDefinesSourceFilePath = SourceBasePath + "generated_defines.h";

      public const int GigaShieldWidth = 480;
      public const int GigaShieldHeight = 800;

      public const int PaletteMaxSize = 256;

      public const int MapTileSize = 16;
      public const int MapTilePixels = 256;
      public const int MapTileTextureCount = 32;
      public const int TileMapMinTilesX = 20;
      public const int TileMapMinTilesY = 15;
      public const int TileMapMaxTilesX = 140;
      public const int TileMapMaxTilesY = 135;
      public const int TileMapTileCount = TileMapMaxTilesX * TileMapMaxTilesY;
      public const int MapTileTextureDefaultIndex = 8;  // black square

      public const int TileMapListViewItemSize = 32;
      public const int TileMapTextureListViewMaxWidth = 1120;
      public const int TileMapTextureListViewMaxHeight = 760;
      public const int TileMapPortalListViewMaxWidth = 560;
      public const int TileMapPortalListViewMaxHeight = 760;

      public const int SpriteFrameSize = 16;
      public const int SpriteFramePixels = 256;
      public const int ActiveSpriteFrameCount = 2;
      public const int ActiveSpritePositionCount = 4;

      public const int TextTileSize = 8;
      public const int TextTileCount = 85;  // keep this in parity with TEXT_TILE_COUNT

      public const int EnemyTileSize = 8;
      public const int EnemyTileStride = 10;
      public const int EnemyTileCount = 120;   // keep this in parity with ENEMY_TILE_COUNT

      public const int PlayerActiveSpriteIndex = 0;

      // bits 6-7
      public static readonly UInt32[] TileSetIndexWalkSpeeds = [
         0,       // grass
         0x40,    // trees
         0x80,    // hills
         0x40,    // desert
         0xC0,    // swamp
         0,       // mountains
         0,       // stone wall
         0,       // brick path
         0,       // void
         0xC0,    // barrier
         0,       // counter
         0,       // metal wall
         0,       // water, no shore
         0,       // bridge
         0,       // water, left shore
         0,       // water, top shore
         0,       // water, right shore
         0,       // water, bottom shore
         0,       // water, upper-left shore
         0,       // water, upper-right shore
         0,       // water, lower-right shore
         0,       // water, lower-left shore
         0,       // water, upper-stop
         0,       // water, right-stop
         0,       // water, bottom-stop
         0,       // water, left-stop
         0,       // water, full-stop
         0,       // water, horizontal river
         0        // water, vertical river
      ];

      // bits 8-9
      public static readonly UInt32[] TileSetIndexEncounterRates = [
         0,       // grass
         0x100,   // trees
         0x200,   // hills
         0x200,   // desert
         0x100,   // swamp
         0,       // mountains
         0,       // stone wall
         0x100,   // brick path
         0,       // void
         0x100,   // barrier
         0,       // counter
         0,       // metal wall
         0,       // water, no shore
         0,       // bridge
         0,       // water, left shore
         0,       // water, top shore
         0,       // water, right shore
         0,       // water, bottom shore
         0,       // water, upper-left shore
         0,       // water, upper-right shore
         0,       // water, lower-right shore
         0,       // water, lower-left shore
         0,       // water, upper-stop
         0,       // water, right-stop
         0,       // water, bottom-stop
         0,       // water, left-stop
         0,       // water, full-stop
         0,       // water, horizontal river
         0        // water, vertical river
      ];

      // bits 10-11
      public static readonly UInt32[] TileSetIndexDamageRates = [
         0x0,     // grass
         0x0,     // trees
         0x0,     // hills
         0x0,     // desert
         0x400,   // swamp
         0,       // mountains
         0,       // stone wall
         0,       // brick path
         0,       // void
         0x800,   // barrier
         0,       // counter
         0,       // metal wall
         0,       // water, no shore
         0,       // bridge
         0,       // water, left shore
         0,       // water, top shore
         0,       // water, right shore
         0,       // water, bottom shore
         0,       // water, upper-left shore
         0,       // water, upper-right shore
         0,       // water, lower-right shore
         0,       // water, lower-left shore
         0,       // water, upper-stop
         0,       // water, right-stop
         0,       // water, bottom-stop
         0,       // water, left-stop
         0,       // water, full-stop
         0,       // water, horizontal river
         0        // water, vertical river
      ];
   }
}
