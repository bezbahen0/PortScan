#include "include/RouteTableIpv4.hpp"

RouteTableIpv4::RouteTableIpv4()
{
    std::ifstream RouteTableIpv4Proc(procRouteIpv4_);
    initStream(RouteTableIpv4Proc);
    for(RouteInfoIpv4 ri; readRouteInfo(RouteTableIpv4Proc, ri);)
    {
        rilist_.push_back(ri);
    }
}

RouteTableIpv4::const_iterator RouteTableIpv4::defaultRouteIpv4() const
{
    return std::find_if(rilist_.begin(), rilist_.end(), [](RouteInfoIpv4 const& ri){ return ri.dest == boost::asio::ip::address_v4(); });
}

RouteTableIpv4::const_iterator RouteTableIpv4::find(boost::asio::ip::address_v4 target) const
{
    const_iterator defaultrt = defaultRouteIpv4();
    const_iterator it = rilist_.begin();

    for(; it != rilist_.end(); ++it)
    {
        if(it == defaultrt) continue;
        if(boost::asio::ip::address_v4::broadcast(target, it -> netmask) == it -> dest)
        {
            break;
        }
    }
    
    return (it == rilist_.end()) ? defaultrt : it;
}

std::istream& RouteTableIpv4::initStream(std::istream& is)
{
    std::string buffer;
    return std::getline(is, buffer);
}

std::ifstream& RouteTableIpv4::readRouteInfo(std::ifstream& is, RouteInfoIpv4& ri)
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
