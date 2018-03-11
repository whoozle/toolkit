#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>
#include <toolkit/io/Poll.h>
#include <toolkit/io/ISocketEventHandler.h>
#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>
#include <thread>
#include <memory>
#include <string>

static constexpr int Port = 3000;
static constexpr int Clients = 30;

using namespace toolkit::log;
using namespace toolkit::io;
using namespace toolkit::net::ipv4;

namespace
{
	void RunClient(int id)
	{
		Logger log("server:client:" + std::to_string(id));
		log.Info() << "ready";
		try
		{
			while(true)
			{
				TCPSocket client;
				log.Info() << "connecting...";
				client.Connect(Endpoint(Address::Localhost(), Port));
				log.Info() << "connected";
				log.Info() << "disconnecting";
			}
		}
		catch(const std::exception & ex)
		{ log.Error() << "failed: " << ex.what(); }
	}

	void RunSpawn()
	{
		Logger log("server:spawner");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		log.Info() << "launching threads...";
		for(int i = 0; i < Clients; ++i)
		{
			std::thread client(&RunClient, i + 1);
			client.detach();
		}
	}

	class Client : public toolkit::io::ISocketEventHandler
	{
		Logger						_log;
		Poll &						_poll;
		std::unique_ptr<TCPSocket>	_sock;

	public:
		Client(Poll & poll, TCPSocket * sock): _log("client:handler:" + std::to_string((long)this)), _poll(poll), _sock(sock)
		{
			_log.Info() << "creating";
			_poll.Add(*_sock, *this, Poll::EventInput);
		}

		void HandleSocketEvent(int events)
		{
			if (events & Poll::EventHangup)
			{
				_log.Info() << "destroying";
				_poll.Remove(*_sock);
				delete this;
			}
		}
	};

	class Server : public toolkit::io::ISocketEventHandler
	{
		Logger			_log;
		Poll &			_poll;
		TCPServerSocket _sock;
	public:
		Server(Poll & poll): _log("server:main"), _poll(poll)
		{
			_log.Info() << "starting test server";
			_sock.Listen(Endpoint(Address::Any(), Port));
			poll.Add(_sock, *this, Poll::EventInput);
			_log.Info() << "listening...";
		}

		void Tick()
		{ _log.Info() << "tick"; }

		void HandleSocketEvent(int events)
		{
			while(true)
			{
				auto socket = _sock.Accept();
				if (socket)
				{
					_log.Info() << "accepted client";
					new Client(_poll, socket);
				}
				else
					break;
			}
		}
	};

}

int main(int argc, char **argv)
{
	Poll poll;

	Server server(poll);
	std::thread spawner(&RunSpawn);
	while(true)
	{
		server.Tick();
		poll.Wait();
	}
	return 0;
}
