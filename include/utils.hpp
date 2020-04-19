#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <utility>
#include <netinet/in.h>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>

namespace utils
{

    uint16_t checksum(std::uint16_t *buf, int bufsz);

    std::string addrToString(struct in6_addr address);

    struct in6_addr stringToAddr(std::string address);

    boost::asio::ip::address_v4 getIfaddrIpv4(std::string const& ifname);

    boost::asio::ip::address_v6 getIfaddrIpv6(std::string const& ifname);

    std::string toReadIpv6(std::string& str);
};

#endif /*UTILS_HPP*/
