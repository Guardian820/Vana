/*
Copyright (C) 2008-2016 Vana Development Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#pragma once

#include "Common/Types.hpp"
#include <algorithm>

namespace vana {
	namespace ext {
		template <typename TKey, typename TValue>
		inline
		auto is_element(const hash_map<TKey, TValue> &map, const TKey &key) -> bool {
			return map.find(key) != std::end(map);
		}

		template <typename TKey, typename TValue, typename THash, typename TOperation>
		inline
		auto find_value_ptr(const hash_map<TKey, TValue, THash, TOperation> &map, const TKey &key) -> const TValue * const {
			auto kvp = map.find(key);
			if (kvp != std::end(map)) {
				return &kvp->second;
			}
			return nullptr;
		}

		template <typename TKey, typename TValue>
		inline
		auto find_value_ptr(const hash_map<TKey, TValue> *map, const TKey &key) -> const TValue * const {
			if (map == nullptr) {
				return nullptr;
			}
			return find_value_ptr(*map, key);
		}

		template <typename TKey, typename TValue>
		inline
		auto find_value_ptr(const ord_map<TKey, TValue> &map, const TKey &key) -> const TValue * const {
			auto kvp = map.find(key);
			if (kvp != std::end(map)) {
				return &kvp->second;
			}
			return nullptr;
		}

		template <typename TKey, typename TValue>
		inline
		auto find_value_ptr(const ord_map<TKey, TValue> *map, const TKey &key) -> const TValue * const {
			if (map == nullptr) {
				return nullptr;
			}
			return find_value_ptr(*map, key);
		}

		template <typename TValue>
		inline
		auto find_value_ptr(const vector<TValue> &map, size_t index) -> const TValue * const {
			if (index >= map.size()) {
				return nullptr;
			}
			return &map.at(index);
		}

		template <typename TValue>
		inline
		auto find_value_ptr(const vector<TValue> *map, size_t index) -> const TValue * const {
			if (map == nullptr) {
				return nullptr;
			}
			return find_value_ptr(*map, index);
		}

		template <typename TKey, typename TValue>
		inline
		auto find_value_or_default(const hash_map<TKey, TValue> &map, const TKey &key, TValue default) -> TValue {
			auto kvp = map.find(key);
			if (kvp != std::end(map)) {
				return kvp->second;
			}
			return default;
		}

		template <typename TContainer, typename TPred>
		inline
		auto any_of(const TContainer &c, TPred pred) -> bool {
			return std::any_of(std::cbegin(c), std::cend(c), pred);
		}

		template <typename TValue>
		inline
		auto remove_element(vector<TValue> &map, const TValue &value) -> decltype(std::begin(map)) {
			return map.erase(std::remove(std::begin(map), std::end(map), value), std::end(map));
		}

		template <typename TElement>
		auto in_range_inclusive(const TElement val, const TElement min, const TElement max) -> bool {
			return !(val < min || val > max);
		}

		template <typename TElement>
		auto constrain_range(const TElement val, const TElement min, const TElement max) -> TElement {
			return std::min(std::max(val, min), max);
		}
	}
}