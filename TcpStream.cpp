#include "TcpStream.hpp"
#include <sys/socket.h>

TcpStream::TcpStream(int fd) : _fd(fd) {
    memset(&_event, 0, sizeof(Kevent));
};

size_t TcpStream::read(char* buff, size_t size) const {
    if (_fd < 0)
        return 0;
    return recv(_fd, buff, size, 0);
};

size_t TcpStream::write(const char* const buff, size_t size) const {
    if (_fd < 0)
        return 0;
    return send(_fd, buff, size, 0);
};

int TcpStream::get_raw_fd() const {
    return _fd;
}

Kevent TcpStream::get_kevent() const {
    return _event;
};

void TcpStream::set_kevent(Kevent kv) {
    memset(&_event, 0, sizeof(Kevent));
    _event = kv;
}

static void shutdown_helper(int fd) {
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

TcpStream::~TcpStream() {
    shutdown_helper(_fd);
}
