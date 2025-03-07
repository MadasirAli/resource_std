#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "resource_pack_header.h"
#include "resource_item_ref.h"

namespace base {
  namespace resources {
    class resource_pack {
    public:
      resource_pack(const std::string& name);

      const resource_pack_header& get_header() const noexcept {
        return m_header;
      }

      const std::vector<resource_item_ref>& get_resources() const noexcept {
        return m_items;
      }

    private:
      resource_pack_header m_header;
      std::string m_name;
      std::unique_ptr<char[]> m_pData;
      std::vector<resource_item_ref> m_items;
    };
  }
}