#include "include/utils.hpp"

uint16_t utils::checksum(std::uint16_t *buf, int bufsz)
{
    unsigned long sum = 0;
    while(bufsz > 1)
    {
        sum += *buf++;
        bufsz -= 2;
    }
    if(bufsz == 1)
        sum += *(unsigned char *)buf;

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}


std::string utils::addrToString(struct in6_addr address)
{
    char str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(address), str, INET6_ADDRSTRLEN);
    return std::string(str);
}

struct in6_addr utils::stringToAddr(std::string address)
{
    struct in6_addr sa;
    inet_pton(AF_INET6, address.c_str(), &(sa));
    return sa;
}

boost::asio::ip::address_v4 utils::getIfaddrIpv4(std::string const& ifname)
{
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    return boost::asio::ip::address_v4(ntohl(((struct sockaddr_in *) &ifr.ifr_addr) -> sin_addr.s_addr));
}

boost::asio::ip::address_v6 utils::getIfaddrIpv6(std::string ifname)
{
    int fd = socket(AF_INET6, SOCK_DGRAM, 0);
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET6;
    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    return boost::asio::ip::make_address_v6(addrToString(((struct sockaddr_in6 *) &ifr.ifr_addr) -> sin6_addr));
}

std::string utils::toReadIpv6(std::string& str)
{
    for(unsigned i = 4; i != str.size(); i += 5)
    {
        str.insert(i, ":");
    }
    return str;
}
