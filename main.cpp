#include <iostream>
#include "TcpStream.hpp"
#include "kqueue.hpp"
#include "listener_interface.hpp"
#include "socket_header.hpp"
#include "tcpListener.hpp"
#include "tools.hpp"

using std::cout;
using std::endl;
using std::string;

#define Server TcpListener;
#define Client TcpStream;
#define Event TcpListener;
#define loop for (;;)

int main(int ac, char** av) {
    if (ac < 2)
        exit(1);

    TcpListener* server = new TcpListener("localhost", av[1]);
    int server_fd = server->get_raw_fd();
    cout << G(DEBUG) << " Server is connected "
         << "port:" << av[1] << " fd:" << server_fd << endl;
    Kqueue kq;
    char buff[1024] = {0};
    kq.attach(server);

    loop {
        cout << G(INFO) << " Waiting  ..." << endl;
        IListener& event = kq.get_event();
        if (event.get_raw_fd() == server_fd) {
            TcpStream& client = dynamic_cast<TcpListener*>(&event)->accept();
            kq.attach(&client);
            cout << G(DEBUG)
                 << " Client connected fd == " << client.get_raw_fd() << endl;
        } else {
            TcpStream* client = dynamic_cast<TcpStream*>(&event);
            bzero(buff, 1024);
            if (client->read(buff, client->get_kevent().data) == 0) {
                cout << G(DEBUG)
                     << " Client detached fd == " << client->get_raw_fd()
                     << endl;
                kq.detach(client);
                delete client;
            } else {
                cout << G(INFO) << " Message : " << tools::trim(buff, "\n")
                     << endl;
            }
        }
    }

    return 0;
}

// std::map<uintptr_t, IListener*> listeners = kq.get_listeners();
// std::map<uintptr_t, IListener*>::iterator it =
//     listeners.begin();

// while (it != listeners.end()) {
//     if (it->first == (uintptr_t)server_fd ||
//         it->first == (uintptr_t)client->get_raw_fd()) {
//         ++it;
//         continue;
//     }
//     dynamic_cast<TcpStream*>(it->second)
//         ->write(buff, strlen(buff));
//     it++;
// }
