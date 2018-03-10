#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

static constexpr int Port = 3000;

int main(int argc, char **argv)
{
	using namespace toolkit::net::ipv4;
	TCPServerSocket sock;
	sock.Listen(Endpoint(Address::Any(), Port));
	return 0;
}
