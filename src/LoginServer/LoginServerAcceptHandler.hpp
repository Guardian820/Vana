/*
Copyright (C) 2008-2015 Vana Development Team

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

namespace Vana {
	class PacketReader;

	namespace LoginServer {
		class LoginServerAcceptedSession;

		namespace LoginServerAcceptHandler {
			auto registerChannel(ref_ptr_t<LoginServerAcceptedSession> session, PacketReader &reader) -> void;
			auto updateChannelPop(ref_ptr_t<LoginServerAcceptedSession> session, PacketReader &reader) -> void;
			auto removeChannel(ref_ptr_t<LoginServerAcceptedSession> session, PacketReader &reader) -> void;
		}
	}
}