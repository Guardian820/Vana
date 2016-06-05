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

namespace vana {
	enum class salt_modify_policy {
		none = 0,
		xor_byte_cipher,
		xor_cipher,
		bit_rotate_cipher,
		overall_bit_rotate_cipher,
		byte_rotate_cipher,
		bit_reverse_cipher,
		reverse_cipher,
		overwrite_cipher,
		add_cipher,
		subtract_cipher,
		multiply_cipher,
		divide_cipher,
		append,
		prepend,
		intersperse,
		// If more constants are added, please add them to ConfigFile.cpp as well
	};
}