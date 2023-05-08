#pragma once
#include <unistd.h>
#include <array>
#include <cstring>
#include "listener_interface.hpp"
#include "tcpListener.hpp"

class TcpListener;
class TcpStream : public IListener {
    const int _fd;
    Kevent _event;

   public:
    TcpStream(int fd);
    std::string get_port() const;
    std::string get_host() const;
    virtual ~TcpStream();
    int get_raw_fd() const;
    Kevent get_kevent() const;
    void set_kevent(Kevent kv);
    std::string get_buffer_request() const;
    size_t read(char* buff, size_t size) const;
    size_t write(const char* const buff, size_t size) const;
};
