#pragma once

#include <cstdint>
#include <array>
#include <assert.h>
#include <string>

#include "format.h"

namespace base {
  namespace resources {
    struct magic_word {
      std::array<char, 4> signature = {'f', 'r', 's', 'p'};

      bool operator==(const magic_word& rhs) const noexcept {
        return (rhs.signature[0] == signature[0] && 
                rhs.signature[1] == signature[1] &&
                rhs.signature[2] == signature[2] &&
                rhs.signature[3] == signature[3]);
      }
      bool operator!=(const magic_word& rhs) const noexcept {
        return (rhs.signature[0] != signature[0] &&
          rhs.signature[1] != signature[1] &&
          rhs.signature[2] != signature[2] &&
          rhs.signature[3] != signature[3]);
      }
    };
    struct resource_name {
      static constexpr const size_t size = 128;
    private:
      std::array<char, size> _char_array = { 0 };

    public:
      resource_name() {}

      resource_name(const resource_name& other) {
        std::memcpy(_char_array.data(), other._char_array.data(), size);
      }

      resource_name(const char* pName) {
        assert(std::strlen(pName) < size);

        strcpy_s(this->_char_array.data(), std::strlen(pName), pName);
      }

      resource_name(const std::string& name) {
        for (size_t i = 0; i < name.length(); ++i) {
          if (i >= size) {
            break;
          }
          this->_char_array[i] = name[i];
        }
      }

      std::string string() const {
        return std::string(_char_array.data());
      }

      bool operator==(const resource_name& rhs) const {
        return (std::memcmp(_char_array.data(), rhs._char_array.data(), size) == 0);
      }
      bool operator!=(const resource_name& rhs) const {
        return (std::memcmp(_char_array.data(), rhs._char_array.data(), size) != 0);
      }
    };
    struct resource_pack_header {
      magic_word signature;
      uint32_t chunk_index = 0;
      uint32_t total_chunks = 0;
      content contentType = content::undef;
      uint32_t num_resources = 0;
      resource_name name;
    };
    /*
    * DATA FORMAT:
    * header header;
    * char bytes[header.size];
    */
    struct resource_item_header {
      resource_name name;
      uint32_t size = 0;
      uint32_t endSize = 0;
    };
    struct resource_texture_item_header : resource_item_header {
      uint32_t width = 0;
      uint32_t height = 0;
      uint32_t channels = 0;
    };
    struct resource_audio_item_header : resource_item_header {
      uint32_t channels = 0;
      uint32_t sampleRate = 0;
      uint32_t bytesPerSample = 0;
      uint64_t frameCount = 0;
    };
    struct resource_shader_item_header : resource_item_header {
    };
    struct resource_texture_array_item_header : resource_item_header {
      uint32_t count = 0;
    };
  }
}