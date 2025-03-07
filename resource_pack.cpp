#include "resource_pack.h"

#include <fstream>
#include <assert.h>
#include <stdexcept>

using namespace base::resources;

resource_pack::resource_pack(const std::string& name) :
  m_header(),
  m_name(name)
{
  std::ifstream file(name, std::ios::binary);

  if (file.is_open() == false) {
    throw std::runtime_error("Failed to open file\n" + m_name);
  }

  file.seekg(0, std::ios::end);
  const size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  constexpr const size_t headerSize = sizeof(resource_pack_header);

  if (fileSize == 0) {
    return;
  }
  assert(fileSize >= headerSize);

  file.read(reinterpret_cast<char*>(&m_header), headerSize);

  if (m_header.signature != magic_word()) {
    throw std::runtime_error("Signature didn't match for file\n" + m_name);
  }

  assert(m_header.contentType != content::undef);

  const size_t dataSize = fileSize - headerSize;

  m_pData = std::move(std::make_unique<char[]>(dataSize));
  file.read(m_pData.get(), dataSize);
  file.close();

  // creating references
  size_t offset = 0;
  for (size_t i = 0; i < m_header.num_resources; ++i) {

    const char* const itemStart = m_pData.get() + offset;
    const auto* pHeader = reinterpret_cast<const resource_item_header*>(itemStart);
    const char* const itemDataStart = itemStart + sizeof(resource_item_header);
    offset += pHeader->endSize;

    m_items.emplace_back(m_header.contentType, *pHeader, const_cast<char*>(itemDataStart));
  }
}