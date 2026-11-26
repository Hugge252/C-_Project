#pragma once


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <string>
#include <vector>

class OscSender
{
public:
    OscSender(const std::string& ip, unsigned short port);
    ~OscSender();

    OscSender(const OscSender&) = delete;
    OscSender& operator=(const OscSender&) = delete;

    // Send one 32-bit integer, sendInt("/marker/id", 7)
    void sendInt(const std::string& address, int32_t value);

    // Send one or more 32-bit floats in a single message,
    // sendFloats("/marker/rotation", {rx, ry, rz})
    void sendFloats(const std::string& address, const std::vector<float>& values);

private:
    void transmit(const std::vector<char>& buffer);

    SOCKET sock_ = INVALID_SOCKET;
    sockaddr_in addr_{};
};
