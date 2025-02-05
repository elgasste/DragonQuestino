namespace DragonQuestinoEditor
{
   public static class Constants
   {
      public const string ShieldBackgroundFilePath = "..\\..\\..\\..\\..\\Assets\\shield_background.png";
      public const string TileTexturesFilePath = "..\\..\\..\\..\\..\\Assets\\tile_textures.png";
      public const string EditorSaveDataFilePath = "..\\..\\..\\..\\..\\Assets\\editor_save.json";
      public const string PlayerSpriteFilePath = "..\\..\\..\\..\\..\\Assets\\player_sprite.png";
      public const string StaticSpriteSheetFilePath = "..\\..\\..\\..\\..\\Assets\\static_sprites.png";
      public const string TextTileSetFilePath = "..\\..\\..\\..\\..\\Assets\\text_tileset.png";

      public const string GameDataSourceFilePath = "..\\..\\..\\..\\..\\DragonQuestino\\game_data.c";
      public const string ShieldBackgroundSourceFilePath = "..\\..\\..\\..\\..\\DragonQuestino\\giga_shield_background_data.c";

      public const int GigaShieldWidth = 480;
      public const int GigaShieldHeight = 800;

      public const int PaletteSize = 256;

      public const int TileSize = 16;
      public const int TilePixels = 256;
      public const int TileTextureCount = 32;
      public const int TileMapMinTilesX = 20;
      public const int TileMapMinTilesY = 15;
      public const int TileMapMaxTilesX = 140;
      public const int TileMapMaxTilesY = 135;
      public const int TileMapTileCount = TileMapMaxTilesX * TileMapMaxTilesY;
      public const int TileTextureDefaultIndex = 8;  // black square

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
      public const int TextTileCount = 83;  // keep this in parity with TEXT_TILE_COUNT

      // bits 7-8
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

      // bits 10-11
      public static readonly UInt32[] TileSetIndexEncounterRates = [
         0,       // grass
         0x200,   // trees
         0x300,   // hills
         0x200,   // desert
         0x400,   // swamp
         0,       // mountains
         0,       // stone wall
         0,       // brick path
         0,       // void
         0,       // barrier
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

      // bits 12-13
      public static readonly UInt32[] TileSetIndexDamageRates = [
         0x0,     // grass
         0x0,     // trees
         0x0,     // hills
         0x0,     // desert
         0x800,   // swamp
         0,       // mountains
         0,       // stone wall
         0,       // brick path
         0,       // void
         0x1000,  // barrier
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
