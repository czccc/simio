//
// Created by Cheng on 2021/3/5.
//

#include "simio.h"
#include <cstring>
#include <iostream>

using simio::SocketAddr;
using namespace std;

int main() {
    // IPv4 demo of inet_ntop() and inet_pton()

    string ww = "127.0.0.1:1111";
    auto it = ww.rfind(':');
    string ip(ww.begin(), ww.begin() + it);
    string port(ww.begin() + it + 1, ww.end());
    cout << it << ":" << ip << " - " << port << endl;

    struct sockaddr_in ss{};
    struct sockaddr_in *sd;
    // memset(&sd, 0, sizeof(sd));
    char str[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(sd->sin_addr), str, INET_ADDRSTRLEN);
    // printf("%s\n", str);
    cout << str << endl;
    string ssd(str);
    cout << ssd << endl;

    inet_ntop(AF_INET, &(ss.sin_addr), str, INET_ADDRSTRLEN);
    // printf("%s\n", str);
    cout << str << endl;

    int ret = inet_pton(AF_INET, "192.0.2.133", &(sd->sin_addr));
    printf("%d\n", ret); // prints "192.0.2.33"
    int ret_1 = inet_pton(AF_INET, "192.0.2.123", &(ss.sin_addr));
    printf("%d\n", ret_1); // prints "192.0.2.33"

    if (ret == 0) {
        cout << "abort because ret" << endl;
        abort();
    }
    if (ret_1 == 0) {
        cout << "abort because ret_1" << endl;
        abort();
    }

    inet_ntop(AF_INET, &(sd->sin_addr), str, INET_ADDRSTRLEN);
    cout << str << endl;
    inet_ntop(AF_INET, &(ss.sin_addr), str, INET_ADDRSTRLEN);
    cout << str << endl;

    SocketAddr addr("1.2.3.4", 30);
    cout << addr << endl;

    // string s(INET_ADDRSTRLEN, 0);
    sockaddr_in sd1{};
    inet_pton(AF_INET, "192.0.2.133", &(sd1.sin_addr));
    sd1.sin_port = 2222;
    SocketAddr a1(sd1);
    cout << a1 << endl;

}