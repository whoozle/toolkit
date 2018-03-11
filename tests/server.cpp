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
		while(true)
		{
			TCPSocket client;
			log.Info() << "connecting...";
			client.Connect(Endpoint(Address::Localhost(), Port));
			log.Info() << "connected";
		}
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
		std::unique_ptr<TCPSocket> _sock;

	public:
		Client(TCPSocket * sock): _sock(sock)
		{ }
		void HandleSocketEvent(int event)
		{ }
	};
}

int main(int argc, char **argv)
{
	Logger log("server:main");
	log.Info() << "starting test server";
	Poll poll;

	TCPServerSocket sock;
	sock.Listen(Endpoint(Address::Any(), Port));
	log.Info() << "listening...";

	std::thread spawner(&RunSpawn);
	while(true)
	{
		auto socket = sock.Accept();
		if (socket)
		{
			auto client = new Client(socket);
			poll.Add(*socket, *client, Poll::EventInput | Poll::EventOutput);
		}
		poll.Wait(1);
	}
	return 0;
}
