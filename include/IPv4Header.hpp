#ifndef IPV4_HPP
#define IPV4_HPP

#include <netinet/ip.h>

#include "utils.hpp"

/*struct ipv4hdr
{
    unsigned int version : 4;
    unsigned int headerLength : 4;
    uint8_t typeOfService;
    uint16_t totalLength;
    uint16_t id;
    uint16_t fragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t sourceAddress;
    uint32_t destinationAddress;
};*/


class IPv4Header
{

    using headerType = struct iphdr;
public:

    IPv4Header() : rep_{} {}

    uint8_t version() const
    { return rep_.version; }

    uint8_t headerLength() const
    { return rep_.ihl; }

    uint8_t typeOfService() const
    { return rep_.tos; }

    uint16_t totalLength() const
    { return ntohs(rep_.tot_len); }

    uint16_t id() const
    { return ntohs(rep_.id); }

    uint16_t fragmentOffset() const
    { return ntohs(rep_.frag_off); }
    
    uint8_t ttl() const
    { return rep_.ttl; }

    uint8_t protocol() const
    { return rep_.protocol; }

    uint16_t checksum() const
    { return ntohs(rep_.check); }

    boost::asio::ip::address_v4 sourceAddress() const
    {
        return boost::asio::ip::address_v4(ntohl(rep_.saddr));
    }
    boost::asio::ip::address_v4 destinationAddress() const
    {
        return boost::asio::ip::address_v4(ntohl(rep_.daddr));
    }

    void version(uint8_t version)
    { rep_.version = version; }

    void headerLength(uint8_t headerLength)
    { rep_.ihl = headerLength; }

    void typeOfService(uint8_t typeOfService)
    { rep_.tos = typeOfService; }

    void totalLength(uint16_t totalLength)
    { rep_.tot_len = htons(totalLength); }

    void id(uint16_t id)
    { rep_.id = htons(id); }

    void fragmentOffset(uint16_t fragmentOffset)
    { rep_.frag_off = htons(fragmentOffset); }

    void ttl(uint8_t ttl)
    { rep_.ttl = ttl; }

    void protocol(uint8_t protocol)
    { rep_.protocol = protocol; }

    void checksum(uint16_t check)
    { rep_.check= htons(check); }
    
    void checksum()
    {
        checksum(0);
        checksum( utils::checksum(reinterpret_cast<uint16_t*>(&rep_), length()) );
    }

    void sourceAddress(uint32_t sourceAddress)
    {
        rep_.saddr = htonl(sourceAddress);
    }

    void destinationAddress(uint32_t destinationAddress)
    {
        rep_.daddr = htonl(destinationAddress);
    }

    void sourceAddress(boost::asio::ip::address_v4 sourceAddress)
    {
        rep_.saddr = htonl(sourceAddress.to_ulong());
    }

    void destinationAddress(boost::asio::ip::address_v4 destinationAddress)
    {
        rep_.daddr = htonl(destinationAddress.to_ulong());
    }

    char* header()
    {
        return reinterpret_cast<char*>(&rep_);
    }

    std::size_t length()
    {
        return sizeof(rep_);
    }


    friend std::istream& operator>> (std::istream& is, IPv4Header& header)
    {
        return is.read(header.header(), header.length());
    }

    friend std::ostream& operator<< (std::ostream& os, IPv4Header& header)
    {
        return os.write(header.header(), header.length());
    }

private:

    headerType rep_;
};

#endif /*IPV4_HPP*/
