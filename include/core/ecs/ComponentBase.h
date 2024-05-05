#pragma once
#include <bitset>
#include <stdint.h>

using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;