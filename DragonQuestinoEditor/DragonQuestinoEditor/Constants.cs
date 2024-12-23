namespace DragonQuestinoEditor
{
   internal static class Constants
   {
      public const string TileTexturesFilePath = "..\\..\\..\\..\\..\\Assets\\tile_textures.png";
      public const string EditorSaveDataFilePath = "..\\..\\..\\..\\..\\Assets\\editor_save.json";
      public const string PlayerSpriteFilePath = "..\\..\\..\\..\\..\\Assets\\player_sprite.png";
      public const string DataSourceFilePath = "..\\..\\..\\..\\..\\DragonQuestino\\game_data.c";

      public const int PaletteSize = 256;

      public const int TileSize = 16;
      public const int TilePixels = 256;
      public const int TileTextureCount = 32;
      public const int TileMapMinTilesX = 20;
      public const int TileMapMinTilesY = 15;
      public const int TileMapMaxTilesX = 140;
      public const int TileMapMaxTilesY = 135;
      public const int TileMapTileCount = TileMapMaxTilesX * TileMapMaxTilesY;

      public const int SpriteFrameSize = 16;
      public const int SpriteFramePixels = 256;
      public const int SpriteFrameCount = 2;
      public const int SpritePositionCount = 4;

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
