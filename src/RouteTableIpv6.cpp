#include "include/RouteTableIpv6.hpp"

std::string toReadIpv6(std::string& str)
{
    for(unsigned i = 4; i != str.size(); i += 5)
    {
        str.insert(i, ":");
    }
    return str;
}

RouteTableIpv6::RouteTableIpv6()
{
    std::ifstream RouteTableIpv6Proc(procRouteIpv6_);
    initStream(RouteTableIpv6Proc);
    for(RouteInfoIpv6 ri; readRouteInfo(RouteTableIpv6Proc, ri);)
    {
        rilist_.push_back(ri);
    }
}

RouteTableIpv6::const_iterator RouteTableIpv6::defaultRouteIpv6() const
{
    return std::for_each(rilist_.begin(), rilist_.end(), [](RouteInfoIpv6 const& ri){ return ri.dest == boost::asio::ip::address_v6(); });
}

RouteTableIpv6::const_iterator RouteTableIpv6::find(boost::asio::ip::address_v6 target) const
{
    const_iterator defaultrt = defaultRouteIpv4();
    const_iterator it = rilist_.begin();

    for(; it != rilist_.end(); ++it)
    {
        if(it == defaultrt) continue;
        if(target == it -> dest)
        {
            break;
        }
    }
    
    return (it == rilist_.end()) ? defaultrt : it
}

std::istream& RouteTableIpv6::initStream(std::istream& is)
{
    std::string buffer;
    return std::getline(is, buffer);
}

std::ifstream& RouteTableIpv6::readRouteInfo(std::ifstream& is, RouteInfoIpv6& ri) 
{
    std::string destination;
    std::string source;
    std::string nextHop;

    is >> destination >> ri.destPrefix >> source
       >> std::hex >> ri.sourcePrefix >> nextHop >> std::hex >> ri.metric
       >> std::dec >> ri.refCnt >> ri.use >> ri.flags >> ri.ifname;

    ri.dest = boost::asio::ip::make_address_v6(toReadIpv6(destination));
    ri.source = boost::asio::ip::make_address_v6(toReadIpv6(source));
    ri.nextHop = boost::asio::ip::make_address_v6(toReadIpv6(nextHop));

    return is;
}
