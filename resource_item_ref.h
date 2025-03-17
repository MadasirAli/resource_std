#pragma once
#include <cstdint>
#include <assert.h>
#include <vector>

#include "format.h"
#include "resource_pack_header.h"

namespace base {
	namespace resources
	{
		class resource_item_ref {

		public:
			resource_item_ref(content content, const resource_item_header& header, char* pData) :
				_content(content),
				_header(header),
				_pData(pData)
			{
				assert(content != content::undef);
				assert(_pData != nullptr);
				assert(header.size > 0);
			}


			content content_type() const noexcept {
				return _content;
			}
			const resource_item_header& get_header() const noexcept {
				return _header;
			}
			char* get_data() const noexcept {
				return _pData;
			}

		private:
			content _content = content::undef;
			resource_item_header _header;
			mutable char* _pData = nullptr;
		};

		class resource_texture_item_ref {
		public:
			resource_texture_item_ref(const resource_item_ref& itemRef) :
				_pData(itemRef.get_data())
			{
				assert(itemRef.content_type() == content::textures);

				auto* pItemStart = _pData - sizeof(resource_item_header);
				_header = *((resource_texture_item_header*)pItemStart);
				_pData = pItemStart + sizeof(resource_texture_item_header);
			}

			const resource_texture_item_header& get_header() const noexcept {
				return _header;
			}
			char* get_data() const noexcept {
				return _pData;
			}

		private:
			resource_texture_item_header _header;
			char* _pData = nullptr;
		};
		class resource_audio_item_ref {
		public:
			resource_audio_item_ref(const resource_item_ref& itemRef) :
				_pData(itemRef.get_data())
			{
				assert(itemRef.content_type() == content::audios);

				auto* pItemStart = _pData - sizeof(resource_item_header);
				_header = *((resource_audio_item_header*)pItemStart);
				_pData = pItemStart + sizeof(resource_audio_item_header);
			}

			const resource_audio_item_header& get_header() const noexcept {
				return _header;
			}
			char* get_data() const noexcept {
				return _pData;
			}

		private:
			resource_audio_item_header _header;
			char* _pData = nullptr;
		};
		class resource_shader_item_ref {
		public:
			resource_shader_item_ref(const resource_item_ref& itemRef) :
				_pData(itemRef.get_data())
			{
				assert(itemRef.content_type() == content::shaders);

				auto* pItemStart = _pData - sizeof(resource_item_header);
				_header = *((resource_shader_item_header*)pItemStart);
				_pData = pItemStart + sizeof(resource_shader_item_header);
			}

			const resource_shader_item_header& get_header() const noexcept {
				return _header;
			}
			char* get_data() const noexcept {
				return _pData;
			}

		private:
			resource_shader_item_header _header;
			char* _pData = nullptr;
		};

		class resource_texture_array_item_ref {
		public:
			resource_texture_array_item_ref(const resource_item_ref& itemRef) :
				_pData(itemRef.get_data())
			{
				assert(itemRef.content_type() == content::texture_arrays);

				auto* pItemStart = _pData - sizeof(resource_item_header);
				_header = *((resource_texture_array_item_header*)pItemStart);
				_pData = pItemStart + sizeof(resource_texture_array_item_header);

				_textures.reserve(_header.count);

				// creating references
				size_t offset = 0;
				for (size_t i = 0; i < _header.count; ++i) {

					const char* const itemStart = _pData + offset;
					const auto* pHeader = reinterpret_cast<const resource_item_header*>(itemStart);
					const char* const itemDataStart = itemStart + sizeof(resource_item_header);
					offset += pHeader->endSize;

					resource_item_ref item{ content::textures, *pHeader, (char*)(itemDataStart) };
					_textures.emplace_back(item);
				}
			}

			const std::vector<resource_texture_item_ref>& get_textures() const {
				return _textures;
			}

			const resource_texture_array_item_header& get_header() const noexcept {
				return _header;
			}
			char* get_data() const noexcept {
				return _pData;
			}

		private:
			resource_texture_array_item_header _header;
			char* _pData = nullptr;
			std::vector<resource_texture_item_ref> _textures;
		};
	}
}