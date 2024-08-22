#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../engine/GameLevel.h"

struct wadinfo_t {
  unsigned char identification[4];
  int32_t numlumps;
  int32_t infotableofs;
};

struct filelump_t {
  int32_t filepos;
  int32_t size;
  unsigned char name[8];
};

struct wadfile_t {
  wadinfo_t info;
};

class WADLoader {
public:
  WADLoader();

  template <typename T>
  std::vector<T> loadDefinitionsFromLump(unsigned char *defsData,
                                         size_t defsSize) {
    std::vector<T> defs;
    T *currentThing = (T *)defsData;
    size_t defsCount = defsSize / sizeof(T);
    for (size_t i = 0; i < defsCount; i++) {
      defs.push_back(currentThing[i]);
    }

    return defs;
  }

  int getLumpTypeFromName(unsigned char lumpname[8]);
  GameLevel *loadFromFile(std::string filename, std::string mapName);
};