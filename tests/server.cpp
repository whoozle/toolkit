#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>
#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>
#include <thread>

static constexpr int Port = 3000;

int main(int argc, char **argv)
{
	using namespace toolkit::log;
	using namespace toolkit::net::ipv4;

	Logger log("server:main");
	log.Info() << "starting test server";
	TCPServerSocket sock;
	sock.Listen(Endpoint(Address::Any(), Port));
	log.Info() << "listening...";
	while(true)
	{
		auto socket = sock.Accept();
		log.Info() << "accepted: " << socket;
	}
	return 0;
}
