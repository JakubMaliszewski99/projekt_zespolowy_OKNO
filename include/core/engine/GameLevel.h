#pragma once
#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

enum MapLumpTypes {
  eThings = 0,
  eLineDefs,
  eSideDefs,
  eVertexes,
  eSegs,
  eSSectors,
  eNodes,
  eSectors,
  eReject,
  eBlockMap
};

#pragma pack(push, 1)
struct color_t {
  uint8_t r, g, b;
};

struct name_t {
  int8_t value[8];
};

struct patchheader_t {
  uint16_t width;
  uint16_t height;
  int16_t leftoffset;
  int16_t topoffset;
  uint32_t columnofs;
};

/// @brief Structs holding textures lump
/// @reference: https://doomwiki.org/wiki/TEXTURE1_and_TEXTURE2
struct mappatch_t {
  int16_t originx;
  int16_t originy;
  int16_t patch;
  int16_t stepdir;
  int16_t colormap;
};

struct maptexture_t {
  int8_t name[8];
  int32_t masked;
  int16_t width;
  int16_t height;
  int32_t columndirectory;
  int16_t patchcount;
  mappatch_t patches;
};

struct maptexturelump_t {
  int32_t numtextures;
  int32_t offset;
};

struct pnames_t {
  int32_t nummappatches;
  name_t name_p_start;
};

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

struct GameLevelPatch {
  int8_t *name;
};

struct GameLevelTexture {
  size_t width, height;
  int8_t name[8];
  std::vector<std::vector<color_t>> image;
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
  std::vector<color_t *> pallets;
  std::vector<maptexture_t> textures;
  std::vector<GameLevelPatch> patches;
  std::unordered_map<std::string, std::unique_ptr<GameLevelTexture>>
      spriteImages;
  std::unordered_map<std::string, std::unique_ptr<GameLevelTexture>>
      textureImages;
};
