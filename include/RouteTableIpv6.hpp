#ifndef ROUTETABLEIPV6_HPP
#define ROUTETABLEIPV6_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/asio/ip/address_v6.hpp>

struct RouteInfoIpv6 
{
    boost::asio::ip::address_v6 dest;
    uint16_t destPrefix;
    boost::asio::ip::address_v6 source;
    uint16_t sourcePrefix;
    boost::asio::ip::address_v6 nextHop;
    uint32_t metric;
    uint32_t refCnt;
    uint32_t use;
    uint32_t flags;
    std::string ifname;
};

class RouteTableIpv6
{
    using rilist = std::vector<RouteInfoIpv6>;
    using size_type = rilist::size_type;
    using const_iterator = rilist::const_iterator;

public:

    RouteTableIpv6();
    const_iterator defaultRouteIpv6() const;
    const_iterator find(boost::asio::ip::address_v6 target) const;

private:
    auto initStream(std::istream& is) -> decltype(is);
    auto readRouteInfo(std::ifstream& is, RouteInfoIpv6& ri) -> decltype(is);


    rilist rilist_;
    std::string const procRouteIpv6_ {"/proc/net/ipv6_route"};
};

#endif /*ROUTETABLEIPV6_HPP*/
