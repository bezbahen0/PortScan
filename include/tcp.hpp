#ifndef TCP_HPP
#define TCP_HPP


#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/basic_raw_socket.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ip/basic_resolver_iterator.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>
#include <boost/asio/detail/push_options.hpp>

class tcp
{
public:
  using endpoint = boost::asio::ip::basic_endpoint<tcp>;
  using socket = boost::asio::basic_raw_socket<tcp>;
  using resolver = boost::asio::ip::basic_resolver<tcp>;

  static tcp v4()
  {
    return tcp(BOOST_ASIO_OS_DEF(AF_INET));
  }

  static tcp v6()
  {
    return tcp(BOOST_ASIO_OS_DEF(AF_INET6));
  }

  int type() const
  {
    return BOOST_ASIO_OS_DEF(SOCK_RAW);
  }

  int protocol() const
  {
    return BOOST_ASIO_OS_DEF(IPPROTO_TCP);
  }

  int family() const
  {
    return family_;
  }

  friend bool operator==(const tcp& p1, const tcp& p2)
  {
    return p1.family_ == p2.family_;
  }

  friend bool operator!=(const tcp& p1, const tcp& p2)
  {
    return p1.family_ != p2.family_;
  }

private:
  explicit tcp(int protocol_family)
    : family_(protocol_family)
  {
  }

  int family_;
};

#endif /*TCP_HPP*/
