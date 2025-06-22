#pragma once
#include <iostream>
#include <bitset>

using Entity = uint32_t;
using ComponentID = uint8_t;
const size_t MAX_ENTITIES = 100000;  // arbitrary max entities
const size_t MAX_COMPONENTS = 32;  // arbitrary max components
