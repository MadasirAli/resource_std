#pragma once
#pragma once
#include <cstdint>

namespace base {
  namespace resources {
    enum class content : uint32_t {
      undef,
      textures,
      audios,
      shaders,
      texture_arrays
    };
  }
}