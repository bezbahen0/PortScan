#ifndef ROUTETABLEIPV4_HPP
#define ROUTETABLEIPV4_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/asio/ip/address_v4.hpp>

struct RouteInfoIpv4
{
    std::string ifname;
    boost::asio::ip::address_v4 dest; 
    boost::asio::ip::address_v4 gateway;
    boost::asio::ip::address_v4 netmask;
    int refCnt;
    int use;
    int metric;
    unsigned flags;
    unsigned mtu;
    unsigned window;
    unsigned irtt;
};

class RouteTableIpv4
{
    using rilist = std::vector<RouteInfoIpv4>;
    using size_type = rilist::size_type;
    using const_iterator = rilist::const_iterator;

public:
    RouteTableIpv4();
    
    const_iterator defaultRouteIpv4() const;
    const_iterator find(boost::asio::ip::address_v4 target) const;

private:
    std::istream& initStream(std::istream& is);
    std::ifstream& readRouteInfo(std::ifstream& is, RouteInfoIpv4& ri);

    rilist rilist_;
    std::string const procRouteIpv4_ {"/proc/net/route"};
};

#endif /*ROUTETABLEIPV4_HPP*/
