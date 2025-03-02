#if !defined( TABLES_H )
#define TABLES_H

#include "common.h"

#define EXPERIENCE_TABLE_SIZE  29

global uint16_t g_experienceTable[EXPERIENCE_TABLE_SIZE] = {
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

#endif // TABLES_H
