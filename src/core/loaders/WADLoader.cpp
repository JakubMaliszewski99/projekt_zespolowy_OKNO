#include "../include/core/loaders/WADLoader.h"
#include <cstring>

WADLoader::WADLoader() {}

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
  GameLevel* gameLevel = new GameLevel();
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
  size_t playpalIdx = -1;
  size_t texture1Idx = -1;
  size_t pnamesIdx = -1;
  size_t flat1StartIdx = -1;
  size_t flat1EndIdx = -1;
  for (size_t i = 0; i < wadFile->info.numlumps; i++) {
    if (strncmp("PLAYPAL", (const char*)lumpDirectory[i].name, 8) == 0) {
        playpalIdx = i;
    }
    if (strncmp("TEXTURE1", (const char*)lumpDirectory[i].name, 8) == 0) {
        texture1Idx = i;
    }
    if (strncmp("PNAMES", (const char*)lumpDirectory[i].name, 8) == 0) {
        pnamesIdx = i;
    }
    if (strncmp("F1_START", (const char*)lumpDirectory[i].name, 8) == 0) {
        flat1StartIdx = i;
    }
    if (strncmp("F1_END", (const char*)lumpDirectory[i].name, 8) == 0) {
        flat1EndIdx = i;
    }
    if (strncmp(mapName.c_str(), (const char *)lumpDirectory[i].name, 8) == 0) {
      selectedMapLumpIndex = i;
    }

  }

  filelump_t playpalLump = lumpDirectory[playpalIdx];
  size_t palletsCount = playpalLump.size / (16*16*3);
  for (size_t offset = 0; offset < palletsCount; offset++) {
      size_t palletOffset = playpalLump.filepos + offset * (16*16*3);
      color_t pallet[16][16];
      for (int i = 0; i < 16; i++) {
          for (int j = 0; j < 16; j++) {
              color_t color;
              color.r = buffer.data()[palletOffset + j*3 + 48 * i];
              color.g = buffer.data()[palletOffset + j*3 + 48 * i + 1];
              color.b = buffer.data()[palletOffset + j*3 + 48 * i + 2];
              pallet[i][j] = color;
          }
      }
      gameLevel->pallets.push_back((color_t*)pallet);
  }

  filelump_t texture1Lump = lumpDirectory[texture1Idx];
  maptexturelump_t* mapTextures = (maptexturelump_t*)&buffer.data()[texture1Lump.filepos];
  std::vector<maptexture_t*> wadTextures;
  for (size_t i = 0; i < mapTextures->numtextures; i++) {
      int32_t* pOffset = &mapTextures->offset;
      int32_t offset = pOffset[i];
      maptexture_t* texture = (maptexture_t*)&buffer.data()[texture1Lump.filepos + offset];
      gameLevel->textures.push_back(*texture);
      wadTextures.push_back(texture);
  }

  filelump_t pnamesLump = lumpDirectory[pnamesIdx];
  pnames_t* pnames = (pnames_t*)&buffer.data()[pnamesLump.filepos];
  for (size_t i = 0; i < pnames->nummappatches; i++) {
      name_t* name = &pnames->name_p_start;
      GameLevelPatch patch;
      patch.name = new int8_t[9];
      strncpy((char*)patch.name, (char*)name[i].value, 8);
      patch.name[8] = 0;
      gameLevel->patches.push_back(patch);
  }

  for (auto texture : wadTextures) {
      std::unique_ptr<GameLevelTexture> glTexture(new GameLevelTexture);
      glTexture->width = texture->width;
      glTexture->height = texture->height;
      glTexture->image.resize(texture->width);
      for (int i = 0; i < texture->width; i++) {
          glTexture->image[i].resize(texture->height);
      }
      strncpy((char*)glTexture->name, (char*)texture->name, 8);

      mappatch_t* texturePatches = &texture->patches;
      for (int i = 0; i < texture->patchcount; i++) {
          mappatch_t currentPatch = texturePatches[i];
          size_t patchIdx = currentPatch.patch;
          int8_t* patchName = gameLevel->patches[patchIdx].name;

          bool patchFound = false;
          size_t patchOffset = -1;
          for (size_t j = 0; j < wadFile->info.numlumps; j++) {
              if (strncmp((const char*)patchName, (const char*)lumpDirectory[j].name, 8) == 0) {
                  patchFound = true;
                  patchOffset = lumpDirectory[j].filepos;
              }
          }

          if (patchFound) {
              patchheader_t* patchHeader = (patchheader_t*)&buffer.data()[patchOffset];
              uint32_t* columnOffsets = &patchHeader->columnofs;
              uint32_t currentColumnOffsetIdx = 0;
              uint32_t currentColumnOffset = columnOffsets[currentColumnOffsetIdx];
              for (int x = 0; x < patchHeader->width; x++) {
                  uint8_t* post = (uint8_t*)&buffer.data()[patchOffset + currentColumnOffset];
                  uint8_t length = post[1];
                  for (int y = 0; y < length; y++) {
                      if (post[0] == 0xFF) {
                          break;
                      }
                      uint8_t colorIdx = post[3 + y];
                      if (currentPatch.originx + x >= texture->width || currentPatch.originy + y >= texture->height || currentPatch.originy < 0 || currentPatch.originx < 0) {
                          continue;
                      }
                      glTexture->image[currentPatch.originx + x][currentPatch.originy + y] = gameLevel->pallets[0][colorIdx];
                  }
                  currentColumnOffsetIdx++;
                  currentColumnOffset = columnOffsets[currentColumnOffsetIdx];
              }
          }
      }

      std::string strName((char*)texture->name, 8);
      gameLevel->textureImages[strName] = std::move(glTexture);
  }

  for (size_t i = flat1StartIdx + 1; i < flat1EndIdx; i++) {
      std::unique_ptr<GameLevelTexture> glTexture(new GameLevelTexture);
      glTexture->width = 64;
      glTexture->height = 64;
      glTexture->image.resize(glTexture->width);
      for (int i = 0; i < glTexture->width; i++) {
          glTexture->image[i].resize(glTexture->height);
      }
      strncpy((char*)glTexture->name, (char*)lumpDirectory[i].name, 8);
      for (int x = 0; x < 64; x++) {
          for (int y = 0; y < 64; y++) {
              glTexture->image[x][y] = { (unsigned char)x, (unsigned char)y, (unsigned char)x };
          }
      }

      std::string strName((char*)lumpDirectory[i].name, 8);
      gameLevel->flatImages[strName] = std::move(glTexture);
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