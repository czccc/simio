//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_SOCKET_ADDR_H_
#define SIMIO_INCLUDE_SIMIO_BASE_SOCKET_ADDR_H_

#include <string>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

namespace simio {


//     /* Structure describing an Internet socket_ address.  */
//     struct sockaddr_in {
//         sa_family_t    sin_family; /* address family: AF_INET */
//         uint16_t       sin_port;   /* port in network byte order */
//         struct in_addr sin_addr;   /* internet address */
//     };

//     /* Internet address. */
//     typedef uint32_t in_addr_t;
//     struct in_addr {
//         in_addr_t       s_addr;     /* address in network byte order */
//     };

//     struct sockaddr_in6 {
//         sa_family_t     sin6_family;   /* address family: AF_INET6 */
//         uint16_t        sin6_port;     /* port in network byte order */
//         uint32_t        sin6_flowinfo; /* IPv6 flow information */
//         struct in6_addr sin6_addr;     /* IPv6 address */
//         uint32_t        sin6_scope_id; /* IPv6 scope-id_ */
//     };

class SocketAddr {
  public:
    explicit SocketAddr(const struct sockaddr &addr) {
        if (addr.sa_family == AF_INET) {
            is_v6 = false;
            auto *sin = reinterpret_cast<const sockaddr_in *>(&addr);
            addr_ = Address(*sin);
        } else if (addr.sa_family == AF_INET6) {
            is_v6 = true;
            auto *sin = reinterpret_cast<const sockaddr_in6 *>(&addr);
            addr_ = Address(*sin);
        }
        build_str();
    }
    explicit SocketAddr(const struct sockaddr_in &addr) : is_v6(false), addr_(addr) { build_str(); }
    explicit SocketAddr(const struct sockaddr_in6 &addr6) : is_v6(true), addr_(addr6) { build_str(); }
    explicit SocketAddr(const std::string &addr) : is_v6(false) {
        auto it = addr.rfind(':');
        std::string ip(addr.cbegin(), addr.cbegin() + it);
        std::string port_str(addr.cbegin() + it + 1, addr.cend());
        uint16_t port = std::stoi(port_str);
        sockaddr_in addr4{};
        addr4.sin_family = AF_INET;
        if (inet_pton(AF_INET, ip.c_str(), &addr4.sin_addr) <= 0) {
            std::cout << "Error when parse ip: " << ip << "to socket_ addr" << std::endl;
            abort();
        }
        addr4.sin_port = port;
        addr_ = Address(addr4);
        build_str();
    }
    SocketAddr(const std::string &ip, uint16_t port, bool is_v6 = false) : is_v6(is_v6) {
        if (is_v6) {
            sockaddr_in6 addr6{};
            addr6.sin6_family = AF_INET6;
            if (inet_pton(AF_INET6, ip.c_str(), &addr6.sin6_addr) <= 0) {
                std::cout << "Error when parse ip: " << ip << "to socket_ addr" << std::endl;
                abort();
            }
            addr6.sin6_port = port;
            addr_ = Address(addr6);
        } else {
            sockaddr_in addr4{};
            addr4.sin_family = AF_INET;
            if (inet_pton(AF_INET, ip.c_str(), &addr4.sin_addr) <= 0) {
                std::cout << "Error when parse ip: " << ip << "to socket_ addr" << std::endl;
                abort();
            }
            addr4.sin_port = port;
            addr_ = Address(addr4);
        }
        build_str();
    }

    bool is_ipv4() const { return !is_v6; }
    bool is_ipv6() const { return is_v6; }

    const sockaddr_in *get_addr4() const {
        return is_ipv4() ? &addr_.addr4_ : nullptr;
    }
    const sockaddr_in6 *get_addr6() const {
        return is_ipv6() ? &addr_.addr6_ : nullptr;
    }
    const sockaddr *get() const {
        if (is_ipv4()) {
            return reinterpret_cast<const sockaddr *>(get_addr4());
        } else {
            return reinterpret_cast<const sockaddr *>(get_addr6());
        }
    }

    std::string get_ip() const {
        if (is_ipv4()) {
            // std::string str(INET_ADDRSTRLEN, 0);
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr_.addr4_.sin_addr), str, INET_ADDRSTRLEN);
            std::string ret_str(str);
            return ret_str;
        } else {
            char str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr_.addr6_.sin6_addr), str, INET6_ADDRSTRLEN);
            std::string ret_str(str);
            return ret_str;
        }
    }
    uint16_t get_port() const {
        return is_ipv4() ? addr_.addr4_.sin_port : addr_.addr6_.sin6_port;
    }

    std::string build_str() const {
        if (str_.empty()) {
            std::ostringstream stream;
            stream << get_ip() << ":" << get_port();
            str_ = stream.str();
        }
        return str_;
    }

    friend std::ostream &operator<<(std::ostream &output,
                                    const SocketAddr &addr) {
        std::string s = addr.build_str();
        output << "Addr(" << s << ")";
        return output;
    }

  private:

    union Address {
        struct sockaddr_in addr4_;
        struct sockaddr_in6 addr6_;

        explicit Address(const struct sockaddr_in6 &addr6) : addr6_(addr6) {}
        explicit Address(const struct sockaddr_in &addr) : addr4_(addr) {}
        Address() = default;
    };

    bool is_v6;
    Address addr_{};
    mutable std::string str_{};
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_SOCKET_ADDR_H_
