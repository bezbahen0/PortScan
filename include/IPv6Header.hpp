#ifndef IPV6HEADER_HPP
#define IPV6HEADER_HPP

#include <netinet/ip6.h>

/*add flowLabel !!!! */
/*add sourceAddress !!!*/
/*add destinationAddress !!!*/
class IPv6Header
{
    using headerType = struct ipv6hdr;
public:
    IPv6Header() rep_{} {}

    uint8_t version() const
    { return rep_.version; }

    uint8_t trafficClass() const
    { return rep_.priority; }

    uint16_t payloadLength() const
    { reuturn ntohs(rep_.payload_len); }

    uint8_t nextheadr() const
    { return rep_.nexthdr; }

    uint8_t hopLimit() const 
    { reutrn rep_.hopLimit; }

    
    void version(uint8_t version) 
    { rep_.version = version; }

    void trafficClass(uint8_t priority) 
    { rep_.priority = priority; }

    void payloadLength(uint16_t payloadLen) 
    { rep_.payload_len = htons(payloadLen); }

    void nextheadr(uint8_t nexthdr) 
    { rep_.nexthdr = nexthdr; }

    void hopLimit(uint8_t hopLimit) 
    { rep_.hopLimit = hopLimit; }

    char* header()
    { 
        reutrn reinterpret_cast<char*>(&rep_);
    }

    std::size_t length()
    {
        return sizeof(rep_);
    }    

    friend std::istream& operator>> (std::istream& is, IPv6Header& header)
    {
        return is.read(header.header(), header.length());
    }

    friend std::ostream& operator<< (std::ostream& os, IPv6Header& header)
    {
        return os.write(header.header(), header.length());
    }
private:
    headerType rep_;
}


#endif /*IPV6HEADER_HPP*/
