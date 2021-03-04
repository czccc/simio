//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_H_
#define SIMIO_INCLUDE_SIMIO_BASE_H_

#include <cinttypes>

#include <functional>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace simio {

using Token = uint64_t;

class Interest {
  public:
    uint8_t inner;

    explicit Interest(uint8_t val) : inner(val) {}
    ~Interest() = default;

    bool is_readable() const { return inner & READABLE_RAW; }
    bool is_writable() const { return inner & WRITABLE_RAW; }

    Interest add(const Interest &other) const { return Interest(inner | other.inner); }
    Interest remove(const Interest &other) const {
        // TODO: check inner is zero
        return Interest(inner & !other.inner);
    }

    static Interest READABLE() {
        return Interest(READABLE_RAW);
    }
    static Interest WRITABLE() {
        return Interest(WRITABLE_RAW);
    }

  private:

    // These must be unique.
    const static unsigned char READABLE_RAW = 0b0001;
    const static unsigned char WRITABLE_RAW = 0b0010;

};

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
    // explicit SocketAddr(sockaddr &sockaddr) : addr(reinterpret_cast<sockaddr_in> (sockaddr)) {
    //     if (sockaddr.sa_family == AF_INET) {
    //         is_v6 = false;
    //     }
    // }

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

    bool is_v6;
    Address addr{};
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_H_
