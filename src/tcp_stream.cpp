#include "net/tcp_stream.hpp"

using namespace net;

TcpStream::TcpStream(TcpSocket&& s) noexcept :
    socket_{std::move(s)}
{ }
