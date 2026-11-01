#pragma once

// Minimal OSC (Open Sound Control) sender over UDP for Windows.
// Header-only, no dependencies besides Winsock.
//
// NOTE: include this header BEFORE <opencv2/opencv.hpp> in any file that uses both,
// so winsock2.h is included before windows.h.

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

class OscSender
{
public:
    OscSender(const std::string& ip, unsigned short port)
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            throw std::runtime_error("WSAStartup failed");

        sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock_ == INVALID_SOCKET)
            throw std::runtime_error("Could not create UDP socket");

        std::memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) != 1)
            throw std::runtime_error("Invalid IP address: " + ip);
    }

    ~OscSender()
    {
        closesocket(sock_);
        WSACleanup();
    }

    OscSender(const OscSender&) = delete;
    OscSender& operator=(const OscSender&) = delete;

    // Send one 32-bit integer, e.g. sendInt("/marker/id", 7)
    void sendInt(const std::string& address, int32_t value)
    {
        std::vector<char> buf;
        padString(buf, address);
        padString(buf, ",i");
        pushUint32(buf, static_cast<uint32_t>(value));
        transmit(buf);
    }

    // Send one or more 32-bit floats in a single message,
    // e.g. sendFloats("/marker/rotation", {rx, ry, rz})
    void sendFloats(const std::string& address, const std::vector<float>& values)
    {
        std::vector<char> buf;
        padString(buf, address);
        padString(buf, "," + std::string(values.size(), 'f'));
        for (float v : values)
        {
            uint32_t bits;
            std::memcpy(&bits, &v, sizeof(bits));
            pushUint32(buf, bits);
        }
        transmit(buf);
    }

private:
    // OSC strings: null-terminated, then padded with nulls to a multiple of 4 bytes.
    static void padString(std::vector<char>& buf, const std::string& s)
    {
        buf.insert(buf.end(), s.begin(), s.end());
        buf.push_back('\0');
        while (buf.size() % 4 != 0)
            buf.push_back('\0');
    }

    // OSC numbers are big-endian (network byte order).
    static void pushUint32(std::vector<char>& buf, uint32_t v)
    {
        uint32_t n = htonl(v);
        char bytes[4];
        std::memcpy(bytes, &n, 4);
        buf.insert(buf.end(), bytes, bytes + 4);
    }

    void transmit(const std::vector<char>& buf)
    {
        sendto(sock_, buf.data(), static_cast<int>(buf.size()), 0,
               reinterpret_cast<const sockaddr*>(&addr_), sizeof(addr_));
    }

    SOCKET sock_ = INVALID_SOCKET;
    sockaddr_in addr_{};
};
