#ifndef ROUTETABLE_HPP
#define ROUTETABLE_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <boost/asio/ip/address_v4.hpp>


class RouteTable
{
public:
    struct RouteInfo
    {
        std::string ifname;
        boost::asio::ip::address_v4 dest, gateway, netmask;
        int refCnt, use, metric;
        unsigned flags, mtu, window, irtt;
    };

    using rilist = std::vector<RouteInfo>;
    using size_type = rilist::size_type;
    using const_reference = rilist::const_reference;
    using const_iterator = rilist::const_iterator;

    RouteTable()
    {
        std::ifstream routeTableProc(procRoute_);
        initStream(routeTableProc);
        for(RouteInfo ri; readRouteInfo(routeTableProc, ri);)
        {
            rilist_.push_back(ri);
        }
    }

    const_iterator defaultRoute() const
    {
        return std::find_if(rilist_.begin(), rilist_.end(), [](RouteInfo const& ri){ return ri.dest.to_ulong() == 0x00000000; });
    }

    const_iterator find(uint32_t target) const
    {
        const_iterator defaultrt = defaultRoute();
        const_iterator it = rilist_.begin();
        for(; it != rilist_.end(); ++it)
        {
            if(it == defaultrt) continue;
            if((target & it -> netmask.to_ulong()) == it -> dest.to_ulong())
            {
                break;
            }
        }
        return (it == rilist_.end()) ? defaultrt : it;
    }

private:
    auto initStream(std::istream& is) -> decltype(is)
    {
        std::string buffer;
        return std::getline(is, buffer);
    }

    auto readRouteInfo(std::ifstream& is, RouteInfo& ri) -> decltype(is)
    {
        uint32_t destination;
        uint32_t gateway;
        uint32_t netmask;
        is >> ri.ifname >> std::hex >> destination >> gateway >> std::dec
           >> ri.flags >> ri.refCnt >> ri.use >> ri.metric >> std::hex 
           >> netmask >> std::dec >> ri.mtu >> ri.window >> ri.irtt;
        ri.dest = boost::asio::ip::address_v4(ntohl(destination));
        ri.gateway = boost::asio::ip::address_v4(ntohl(gateway));
        ri.netmask = boost::asio::ip::address_v4(ntohl(netmask));
        return is;

    }

    std::string const procRoute_ {"/proc/net/route"};
    rilist rilist_;
};


#endif /* ROUTETABLE_HPP */
