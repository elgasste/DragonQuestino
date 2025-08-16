#if !defined( TABLES_H )
#define TABLES_H

#include "common.h"

#define STAT_TABLE_SIZE                      30
#define SPELL_TABLE_SIZE                     10
#define WEAPON_TABLE_SIZE                    8
#define ARMOR_TABLE_SIZE                     8
#define SHIELD_TABLE_SIZE                    4
#define INNPRICE_TABLE_SIZE                  5

global uint16_t g_experienceTable[STAT_TABLE_SIZE] = {
   0,
   7,
   23,
   47,
   110,
   220,
   450,
   800,
   1300,
   2000,
   2900,
   4000,
   5500,
   7500,
   10000,
   13000,
   16000,
   19000,
   22000,
   26000,
   30000,
   34000,
   38000,
   42000,
   46000,
   50000,
   54000,
   58000,
   62000,
   UINT16_MAX
};

global uint8_t g_strengthTable[STAT_TABLE_SIZE] = {
   4,
   5,
   7,
   7,
   12,
   16,
   18,
   22,
   30,
   35,
   40,
   48,
   52,
   60,
   68,
   72,
   72,
   85,
   87,
   92,
   95,
   97,
   99,
   103,
   113,
   117,
   125,
   130,
   135,
   140
};

global uint8_t g_agilityTable[STAT_TABLE_SIZE] = {
   4,
   4,
   6,
   8,
   10,
   10,
   17,
   20,
   22,
   31,
   35,
   40,
   48,
   55,
   64,
   70,
   78,
   84,
   86,
   88,
   90,
   90,
   94,
   98,
   100,
   105,
   107,
   115,
   120,
   130
};

global uint8_t g_hitPointsTable[STAT_TABLE_SIZE] = {
   15,
   22,
   24,
   31,
   35,
   38,
   40,
   46,
   50,
   54,
   62,
   63,
   70,
   78,
   86,
   92,
   100,
   115,
   130,
   138,
   149,
   158,
   165,
   170,
   174,
   180,
   189,
   195,
   200,
   210
};

global uint8_t g_magicPointsTable[STAT_TABLE_SIZE] = {
   0,
   0,
   5,
   16,
   20,
   24,
   26,
   29,
   36,
   40,
   50,
   58,
   64,
   70,
   72,
   95,
   100,
   108,
   115,
   128,
   135,
   146,
   153,
   161,
   161,
   168,
   175,
   180,
   190,
   200
};

global uint8_t g_spellsLevelTable[SPELL_TABLE_SIZE] = {
   2,
   3,
   6,
   8,
   9,
   11,
   12,
   14,
   16,
   18
};

global uint8_t g_weaponEffectTable[WEAPON_TABLE_SIZE] = {
   0,
   2,
   4,
   10,
   15,
   20,
   28,
   40
};

global uint16_t g_weaponCostTable[WEAPON_TABLE_SIZE] = {
   0,
   10,
   60,
   180,
   560,
   1500,
   9800,
   0
};

global uint8_t g_armorEffectTable[ARMOR_TABLE_SIZE] = {
   0,
   2,
   4,
   10,
   16,
   24,
   24,
   28
};

global uint16_t g_armorCostTable[ARMOR_TABLE_SIZE] = {
   0,
   10,
   70,
   300,
   1000,
   3000,
   7700,
   0
};

global uint8_t g_shieldEffectTable[SHIELD_TABLE_SIZE] = {
   0,
   4,
   10,
   20
};

global uint16_t g_shieldCostTable[SHIELD_TABLE_SIZE] = {
   0,
   90,
   800,
   14800
};

global uint16_t g_innPriceTable[INNPRICE_TABLE_SIZE] = {
   6,
   25,
   20,
   100,
   55
};

global uint32_t g_passwordChecksumTable[32] = {
   11,
   29,
   7,
   5,
   16,
   4,
   9,
   28,
   19,
   15,
   13,
   12,
   8,
   6,
   0,
   3,
   24,
   10,
   23,
   25,
   31,
   30,
   22,
   1,
   27,
   21,
   18,
   26,
   14,
   20,
   17,
   2
};

#endif // TABLES_H
