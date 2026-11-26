#include "OscSender.hpp"

#include <cstring>
#include <stdexcept>

//private helper functions, message formatting
namespace
{
    // OSC strings: null-terminated, then padded with nulls to a multiple of 4 bytes.
    void padString(std::vector<char>& buf, const std::string& s)
    {
        buf.insert(buf.end(), s.begin(), s.end());
        buf.push_back('\0');
        while (buf.size() % 4 != 0)
            buf.push_back('\0');
    }

	// OSC 32-bit integers and floats are sent in big-endian byte order.
    // modifies buffer in place, appending 4 bytes.
    void pushUint32(std::vector<char>& buffer, uint32_t v)
    {
        uint32_t n = htonl(v);
        char bytes[4];
        std::memcpy(bytes, &n, 4);
        buffer.insert(buffer.end(), bytes, bytes + 4);
    }
}

OscSender::OscSender(const std::string& ip, unsigned short port)
{
	// Initialize Winsock version 2.2
	//throws std::runtime_error if WSAStartup fails = code returned isnt 0
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        throw std::runtime_error("WSAStartup failed");

	// IPv4 UDP socket.
    // UDP = no delivery gurantee, good for live sensor data
    sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ == INVALID_SOCKET)
        throw std::runtime_error("Could not create UDP socket");

	// Set up the destination address structure.
    std::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) != 1)
        throw std::runtime_error("Invalid IP address: " + ip);
}

// ~ means destructor, called when object goes out of scope or deleted.
// close the socket and clean up Winsock. if missing, the socket will remain open until the program exits
OscSender::~OscSender()
{
    closesocket(sock_);
    WSACleanup();
}


void OscSender::sendInt(const std::string& address, int32_t value)
{
	// Format OSC message and send it.
    std::vector<char> buffer;
    padString(buffer, address);
    padString(buffer, ",i");
    pushUint32(buffer, static_cast<uint32_t>(value));
    transmit(buffer);
}

// Send more 32-bit floats in a single message,
void OscSender::sendFloats(const std::string& address, const std::vector<float>& values)
{
	std::vector<char> buffer; //list of bytes to send
    padString(buffer, address);
    padString(buffer, "," + std::string(values.size(), 'f'));
	// Append each float as a 32-bit big-endian value. is a foreach loop
    for (float v : values)
    {
        uint32_t bits;
        std::memcpy(&bits, &v, sizeof(bits));
        pushUint32(buffer, bits); 
    }
    transmit(buffer);
}

void OscSender::transmit(const std::vector<char>& buffer)
{
    sendto(sock_, buffer.data(), static_cast<int>(buffer.size()), 0,
           reinterpret_cast<const sockaddr*>(&addr_), sizeof(addr_));
}
