#pragma once
#include <stdint.h>
#include <vector>
#include <unordered_map>

enum MapLumpTypes { eThings = 0, eLineDefs, eSideDefs, eVertexes, eSegs, eSSectors, eNodes, eSectors, eReject, eBlockMap };

#pragma pack(push, 1)
/// @brief Struct holding THINGS lump
/// @name mapthings_t
/// @reference: https://doomwiki.org/wiki/Thing
struct mapthings_t {
  int16_t x;
  int16_t y;
  int16_t angle;
  int16_t type;
  int16_t flags;
};

/// @brief Struct holding LINEDEFS lump
/// @name maplinedefs_t
/// @reference: https://doomwiki.org/wiki/Linedef
struct maplinedefs_t {
  int16_t startVertex;
  int16_t endVertex;
  int16_t flags;
  int16_t specialType;
  int16_t sector;
  int16_t frontSidedef;
  int16_t backSidedef;
};

/// @brief Struct holding SIDEDEFS lump
/// @name mapsidedefs_t
/// @reference: https://doomwiki.org/wiki/Sidedef
struct mapsidedefs_t {
  int16_t xOffset;
  int16_t yOffset;
  int8_t upperTextureName[8];
  int8_t lowerTextureName[8];
  int8_t middleTextureName[8];
  int16_t sectorNumber;
};

struct mapvertexes_t {
  int16_t x;
  int16_t y;
};

/// @brief Struct holding SEGS lump
/// @name mapsegs_t
/// @reference: https://doomwiki.org/wiki/Seg
struct mapsegs_t {
  int16_t startingVectorNumber;
  int16_t endingVectorNumber;
  int16_t angle;
  int16_t linedefNumber;
  int16_t direction;
  int16_t offset;
};


/// @brief Struct holding SSECTOR (subsector) lump
/// @name mapsegs_t
/// @reference: https://doomwiki.org/wiki/Subsector
struct mapssectors_t {
  int16_t segCount;
  int16_t firstSectorNumber;
};

struct mapnodes_t {
  int16_t xStart;
  int16_t yStart;
  int16_t xEndOffset;
  int16_t yEndOffset;
  int16_t rightBoundingBox[4];
  int16_t leftBoundingBox[4];
  int16_t rightChild;
  int16_t leftChild;
};

struct mapsectors_t {
  int16_t floorHeight;
  int16_t ceilingHeight;
  int8_t floorTextureName[8];
  int8_t ceilingTextureName[8];
  int16_t lightLevel;
  int16_t specialType;
  int16_t tagNumber;
};

/// Note: Probably should be created as a hashmap
struct mapreject_t {
  int8_t *data;
};

struct mapblockmapheader_t {
  int16_t x;
  int16_t y;
  int16_t columns;
  int16_t rows;
};

struct mapblockmap_t {
  std::vector<std::vector<maplinedefs_t *>> linedefs;
};
#pragma pack(pop)

// TODO: Convert all the structures to friendly equivalent
struct GameLevelSegment {
  int16_t startingVectorNumber;
  int16_t endingVectorNumber;
  float angle;
  int16_t linedefNumber;
  int16_t direction;
  int16_t offset;
  int16_t frontSector;
  int16_t backSector;
};

struct GameLevel {
  std::vector<mapthings_t> things;
  std::vector<maplinedefs_t> linedefs;
  std::vector<mapsidedefs_t> sidedefs;
  std::vector<mapvertexes_t> vertexes;
  std::vector<GameLevelSegment> segments;
  std::vector<mapssectors_t> subsuctors;
  std::vector<mapnodes_t> nodes;
  std::vector<mapsectors_t> sectors;
  std::unordered_map<int, int> rejects;
  std::vector<std::vector<maplinedefs_t *>> blockmap;
};