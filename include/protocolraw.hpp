#ifndef PROTOCOLRAW_HPP
#define PROTOCOLRAW_HPP

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/basic_raw_socket.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ip/basic_resolver_iterator.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>
#include <boost/asio/detail/push_options.hpp>

class protocolraw
{
public:
    using endpoint = boost::asio::ip::basic_endpoint<protocolraw>;
    using socket = boost::asio::basic_raw_socket<protocolraw>;
    using resolver = boost::asio::ip::basic_resolver<protocolraw>;
    
    static protocolraw v4()
    {
        return protocolraw(BOOST_ASIO_OS_DEF(AF_INET));
    }

    static protocolraw v6()
    {
        return protocolraw(BOOST_ASIO_OS_DEF(AF_INET6));
    }

    int type() const
    {
        return BOOST_ASIO_OS_DEF(SOCK_RAW);
    }

    int protocol() const
    {
        return protocol_;
    }

    void protocol(int protocol)
    {
        protocol_ = protocol;
    }

    int family() const
    {
        return family_;
    }

    friend bool operator==(const protocolraw& p1, const protocolraw& p2)
    {
        return p1.family_ == p2.family_;
    }

    friend bool operator!=(const protocolraw& p1, const protocolraw& p2)
    {
        return p1.family_ != p2.family_;
    }

private:
    explicit protocolraw(int protocol_family)
        : family_(protocol_family)
    {
    }

    int family_;
    int protocol_;
};

#endif /*PROTOCOLRAW_HPP*/
