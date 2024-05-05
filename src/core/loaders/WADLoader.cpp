#include "../include/core/loaders/WADLoader.h"

WADLoader::WADLoader() {}

// TODO: Probably to delete? Will create collision based on BSP
//std::vector<std::vector<maplinedefs_t *>>
//WADLoader::loadBlockmapFromLump(unsigned char *defsData, size_t defsSize,
//                                maplinedefs_t *definedLines) {
//  mapblockmapheader_t *blockmapheader = (mapblockmapheader_t *)defsData;
//  std::vector<std::vector<maplinedefs_t *>> linedefs;
//
//  linedefs.reserve(blockmapheader->rows);
//  for (int i = 0; i < blockmapheader->rows; i++) {
//    std::vector<maplinedefs_t *> vectorOfLinedefs;
//    vectorOfLinedefs.reserve(blockmapheader->columns);
//    linedefs.push_back(vectorOfLinedefs);
//  }
//
//  int16_t *listOfOffset = (int16_t *)(defsData + 8);
//  for (int i = 0; i < blockmapheader->rows * blockmapheader->columns; i++) {
//    int16_t currentOffset = listOfOffset[i];
//    int16_t *blocklist = (int16_t *)(defsData + currentOffset);
//    while (*blocklist != -1) {
//      // linedefs[i][j] = definedLines + *blocklist;
//      blocklist++;
//    }
//  }
//
//  return linedefs;
//}
//
//// TODO: Probably to delete? No replacement, f it.
//std::unordered_map<int, int> WADLoader::loadRejects(unsigned char *defsData,
//                                                    size_t defsSize) {
//  std::unordered_map<int, int> rejects;
//  return rejects;
//}

int WADLoader::getLumpTypeFromName(unsigned char lumpname[8]) {
  if (strncmp((const char *)lumpname, "THINGS", 8) == 0) {
    return MapLumpTypes::eThings;
  } else if (strncmp((const char *)lumpname, "LINEDEFS", 8) == 0) {
    return MapLumpTypes::eLineDefs;
  } else if (strncmp((const char *)lumpname, "SIDEDEFS", 8) == 0) {
    return MapLumpTypes::eSideDefs;
  } else if (strncmp((const char *)lumpname, "VERTEXES", 8) == 0) {
    return MapLumpTypes::eVertexes;
  } else if (strncmp((const char *)lumpname, "SEGS", 8) == 0) {
    return MapLumpTypes::eSegs;
  } else if (strncmp((const char *)lumpname, "SSECTORS", 8) == 0) {
    return MapLumpTypes::eSSectors;
  } else if (strncmp((const char *)lumpname, "NODES", 8) == 0) {
    return MapLumpTypes::eNodes;
  } else if (strncmp((const char *)lumpname, "SECTORS", 8) == 0) {
    return MapLumpTypes::eSectors;
  } else if (strncmp((const char *)lumpname, "REJECT", 8) == 0) {
    return MapLumpTypes::eReject;
  } else if (strncmp((const char *)lumpname, "BLOCKMAP", 8) == 0) {
    return MapLumpTypes::eBlockMap;
  }
  return -1;
}

GameLevel *WADLoader::loadFromFile(std::string filename, std::string mapName) {
  std::ifstream file(filename.c_str(), std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "[ERROR] WADLoader: Could not load the file." << std::endl;
    return nullptr;
  }

  std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});

  wadfile_t *wadFile = (wadfile_t *)buffer.data();
  if (strncmp("IWAD", (const char *)wadFile->info.identification, 4) != 0 &&
      strncmp("PWAD", (const char *)wadFile->info.identification, 4) != 0) {
    std::cerr << "[ERROR] WADLoader: Invalid WAD header." << std::endl;
    return nullptr;
  }

  size_t selectedMapLumpIndex = -1;
  filelump_t *lumpDirectory =
      (filelump_t *)(buffer.data() + wadFile->info.infotableofs);
  for (size_t i = 0; i < wadFile->info.numlumps; i++) {
    if (strncmp(mapName.c_str(), (const char *)lumpDirectory[i].name, 8) == 0) {
      selectedMapLumpIndex = i;
      break;
    }
  }

  if (selectedMapLumpIndex < 0) {
    std::cerr << "[ERROR] WADLoader: Map " << mapName << " doesn't exist."
              << std::endl;
    return nullptr;
  }

  if (selectedMapLumpIndex + 11 > wadFile->info.numlumps) {
    std::cerr << "[ERROR] WADLoader: Invalid map structure." << std::endl;
    return nullptr;
  }

  selectedMapLumpIndex++;

  GameLevel *gameLevel = new GameLevel();
  for (size_t i = 0; i < 10; i++) {
    filelump_t currentLump = lumpDirectory[selectedMapLumpIndex + i];
    int lumpType = getLumpTypeFromName(currentLump.name);

    if (lumpType == MapLumpTypes::eThings) {
      gameLevel->things = loadDefinitionsFromLump<mapthings_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eLineDefs) {
      gameLevel->linedefs = loadDefinitionsFromLump<maplinedefs_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eSideDefs) {
      gameLevel->sidedefs = loadDefinitionsFromLump<mapsidedefs_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eVertexes) {
      gameLevel->vertexes = loadDefinitionsFromLump<mapvertexes_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eSegs) {
      auto segments = loadDefinitionsFromLump<mapsegs_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
      std::vector<GameLevelSegment> glSegments;
      for (auto seg : segments) {
        auto linedef = gameLevel->linedefs[seg.linedefNumber];
        auto frontsideId = linedef.frontSidedef;
        auto backsideId = linedef.backSidedef;

        if (seg.direction) {
          std::swap(frontsideId, backsideId);
        }

        auto frontside = gameLevel->sidedefs[frontsideId];
        int16_t backsideSector = -1;
        // TODO: Create enums for linedef flags
        if (linedef.flags & 4 /*TWOSIDED*/) {
          mapsidedefs_t backside = gameLevel->sidedefs[backsideId];
          backsideSector = backside.sectorNumber;
        }

        // WARNING: OVERFLOW!
        // TODO: If there is any problem that's the cause
        // BAM to degree
        float angle = (seg.angle << 16) * 8.38190317e-8;
        if (angle < 0) {
          angle += 360;
        }

        glSegments.push_back({seg.startingVectorNumber, seg.endingVectorNumber,
                              angle, seg.linedefNumber, seg.direction,
                              seg.offset, frontside.sectorNumber,
                              backsideSector});
      }
      gameLevel->segments = glSegments;
    } else if (lumpType == MapLumpTypes::eSSectors) {
      gameLevel->subsuctors = loadDefinitionsFromLump<mapssectors_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eNodes) {
      gameLevel->nodes = loadDefinitionsFromLump<mapnodes_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    } else if (lumpType == MapLumpTypes::eSectors) {
      gameLevel->sectors = loadDefinitionsFromLump<mapsectors_t>(
          buffer.data() + currentLump.filepos, currentLump.size);
    }
  }

  file.close();
  return gameLevel;
}