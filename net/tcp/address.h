//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_NET_TCP_ADDRESS_H_
#define SIMIO_NET_TCP_ADDRESS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace simio {
namespace net {

//     /* Structure describing an Internet socket address.  */
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
//         uint32_t        sin6_scope_id; /* IPv6 scope-id */
//     };

class SocketAddr {
  public:
    explicit SocketAddr(const struct sockaddr_in &addr) : is_v6(false), addr(addr) {}
    explicit SocketAddr(const struct sockaddr_in6 &addr6) : is_v6(true), addr(addr6) {}
    SocketAddr(const std::string &ip, uint16_t port, bool is_v6 = false) : is_v6(is_v6) {

        if (is_v6) {
            sockaddr_in6 addr6{};
            addr6.sin6_family = AF_INET6;
            if (inet_pton(AF_INET6, ip.c_str(), &addr6.sin6_addr) <= 0) {
                // TODO: Add error check
            }
            addr6.sin6_port = port;
            addr = Address(addr6);
        } else {
            sockaddr_in addr4{};
            addr4.sin_family = AF_INET;
            if (inet_pton(AF_INET, ip.c_str(), &addr4.sin_addr) <= 0) {
                // TODO: Add error check
            }
            addr4.sin_port = port;
            addr = Address(addr4);
        }
    }

    bool is_ipv4() const { return !is_v6; }
    bool is_ipv6() const { return is_v6; }

    sockaddr_in *get_addr4() {
        // TODO: add check
        return &addr.addr_;
    }
    sockaddr_in6 *get_addr6() {
        // TODO: add check
        return &addr.addr6_;
    }
    const sockaddr *get() {
        if (is_ipv4()) {
            return reinterpret_cast<const sockaddr *>(get_addr4());
        } else {
            return reinterpret_cast<const sockaddr *>(get_addr6());
        }
    }

  private:

    union Address {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;

        explicit Address(const struct sockaddr_in6 &addr6) : addr6_(addr6) {}
        explicit Address(const struct sockaddr_in &addr) : addr_(addr) {}
        Address() = default;
    };

    const bool is_v6;
    Address addr{};
};

}
}

#endif //SIMIO_NET_TCP_ADDRESS_H_
