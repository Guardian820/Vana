/*
Copyright (C) 2008-2011 Vana Development Team

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

#include "AbstractSession.h"
#include "Decoder.h"
#include "Types.h"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/tr1/memory.hpp>
#include <queue>
#include <string>

using boost::asio::ip::tcp;
using boost::shared_array;
using std::queue;
using std::string;

class AbstractConnection;
class PacketCreator;

class Session : public AbstractSession, public boost::enable_shared_from_this<Session> {
public:
	friend class ConnectionAcceptor;
	Session(boost::asio::io_service &ioService, SessionManagerPtr sessionManager, AbstractConnection *connection, bool isServer, bool isEncrypted, const string &patchLocation = "");

	void disconnect();
	void send(const PacketCreator &packet);
	ip_t getIp() const;
protected:
	tcp::socket & getSocket() { return m_socket; }
	void start();
	void stop();
	void handleStart();
	void handleStop();

	void startReadHeader();
	void handleWrite(const boost::system::error_code &error, size_t bytesTransferred);
	void handleReadHeader(const boost::system::error_code &error, size_t bytesTransferred);
	void handleReadBody(const boost::system::error_code &error, size_t bytesTransferred);
	void send(const unsigned char *buf, int32_t len);
	void sendIv(const PacketCreator &packet);

	static const size_t headerLen = 4;
	static const size_t maxBufferLen = 65535;

	tcp::socket m_socket;
	Decoder m_decoder;
	std::tr1::shared_ptr<AbstractConnection> m_connection;
	shared_array<unsigned char> m_buffer;
	bool m_isServer;
	string m_patchLocation;

	// Packet sending
	shared_array<unsigned char> m_sendPacket;
	boost::mutex m_sendMutex;
};

typedef boost::shared_ptr<Session> SessionPtr;