#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

auto SOCK_PATH = "/tmp/blaadpapers.sock";

int main() {
    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un addr = {AF_UNIX};
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    if(connect(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == 0) {
        close(fd);
        return 1;
    }
    daemon(0, 0);
    bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    listen(fd, 1);

    signal(
        SIGINT,
        [](int) {
            unlink(SOCK_PATH);
            exit(0);
        }
    );

    while(true) {
        const int cli = accept(fd, nullptr, nullptr);
        char cmd;
        read(cli, &cmd, 1);
        if(cmd == 'x') break;
        close(cli);
    }

    unlink(SOCK_PATH);
    close(fd);
}
