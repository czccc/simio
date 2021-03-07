//
// Created by Cheng on 2021/3/5.
//

#include "simio.h"
#include "assert.h"

using namespace simio;
using namespace std;

int main() {
    simio::TcpSocketPtr s = simio::TcpSocket::new_v4();
    cout << s << endl;
    s->set_reuse_addr(true);
    assert(s->get_reuse_addr());
    s->set_reuse_port(true);
    assert(s->get_reuse_port());
    s->set_linger(1);
    assert(s->get_linger() >= 0);
    s->set_send_buffer_size(30000);
    int ret = s->get_send_buffer_size();
    assert(s->get_send_buffer_size() == 60000);
    s->set_recv_buffer_size(30000);
    assert(s->get_recv_buffer_size() == 60000);
    s->set_keepalive_time(2);
    ret = s->get_keepalive_time();
    assert(s->get_keepalive_time() == 2);
    s->set_keepalive_retries(2);
    assert(s->get_keepalive_retries() == 2);
    s->set_keepalive_interval(2);
    assert(s->get_keepalive_interval() == 2);
    TcpListener listener = TcpListener::bind("127.0.0.1:6764");
    cout << listener << endl;

    TcpStream stream = TcpStream::connect(SocketAddr("127.0.0.1:6764"));
    cout << stream << endl;
}