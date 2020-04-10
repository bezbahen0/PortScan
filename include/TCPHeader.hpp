#ifndef TCPHEADER_HPP
#define TCPHEADER_HPP


/*struct tcpheadr
{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
    uint16_t res1 : 4;
    uint16_t doff : 4;
    uint16_t fin : 1;
    uint16_t syn : 1;
    uint16_t rst : 1;
    uint16_t psh : 1;
    uint16_t ack : 1;
    uint16_t urg : 1;
    uint16_t res2 : 2;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};*/

class TCPHeader
{
    using headerType = struct tcphdr;
public:
    enum
    {
        defaultWindowValue = 4096
    };
    TCPHeader() : rep_{} {}
    uint16_t source() const
    { return ntohs(rep_.source); }

    uint16_t destination() const
    { return ntohs(rep_.dest); }

    uint32_t seq() const
    { return ntohl(rep_.seq); }

    uint32_t ackSeq() const
    { return ntohl(rep_.ack_seq); }

    uint16_t res1() const
    { return rep_.res1; }

    uint16_t doff() const
    { return rep_.doff; }

    uint16_t fin() const
    { return rep_.fin; }

    uint16_t syn() const
    { return rep_.syn; }

    uint16_t rst() const
    { return rep_.rst; }

    uint16_t psh() const 
    { return rep_.psh; }

    uint16_t ack() const
    { return rep_.ack; }

    uint16_t urg() const
    { return rep_.urg; }

    uint16_t res2() const
    { return rep_.res2; }

    uint16_t window() const
    { return ntohs(rep_.window); }

    uint16_t checksum() const
    { return ntohs(rep_.check); }

    uint16_t urgPtr() const
    { return ntohs(rep_.urg_ptr); }

    void source(uint16_t source)
    { rep_.source = ntohs(source); }

    void destination(uint16_t destination)
    { rep_.dest = htons(destination); }

    void seq(uint32_t seq)
    { rep_.seq = htonl(seq); }

    void ackSeq(uint32_t ackSeq)
    { rep_.ack_seq = htonl(ackSeq); }

    void res1(uint16_t res1)
    { rep_.res1 = res1; }

    void doff(uint16_t doff)
    { rep_.doff = doff; }

    void fin(bool fin)
    { rep_.fin = fin ? 1 : 0; }

    void syn(bool syn)
    { rep_.syn = syn ? 1 : 0; }

    void rst(bool rst)
    { rep_.rst = rst ? 1 : 0; }

    void psh(bool psh)
    { rep_.psh = psh ? 1 : 0; }

    void ack(bool ack)
    { rep_.ack = ack ? 1 : 0; }

    void urg(uint16_t urg)
    { rep_.urg = urg; }

    void res2(uint16_t res2)
    { rep_.res2 = res2; }

    void window(uint16_t window)
    { rep_.window = htons(window); }

    void checksum(uint16_t checksum)
    { rep_.check = htons(checksum); }

    void urgPtr(uint16_t urgPtr)
    { rep_.urg_ptr = htons(urgPtr); }

    std::size_t length()
    { return sizeof(rep_); }

    char* header()
    { return reinterpret_cast<char*>(&rep_); }

    void computeChecksum(uint32_t sourceAddress, uint32_t destinationAddress)
    {
        checksum(0);
        tcpChecksum tc = {{}, {}};
        tc.pseudo.ipSrc   = htonl(sourceAddress);
        tc.pseudo.ipDst   = htonl(destinationAddress);
        tc.pseudo.zero     = 0;
        tc.pseudo.protocol = IPPROTO_TCP;
        tc.pseudo.length   = htons(sizeof(tcphdr));
        tc.tcphdr = rep_;
        rep_.check = checksum(reinterpret_cast<uint16_t*>(&tc), sizeof(struct tcpChecksum));
    }

    void computeChecksum(const std::string &sourceAddress, const std::string &destinationAddress)
    {
        computeChecksum(
            boost::asio::ip::address_v4::from_string(sourceAddress).to_ulong(),
            boost::asio::ip::address_v4::from_string(destinationAddress).to_ulong()
        );
    }

    friend std::istream& operator>> (std::istream& is, TCPHeader& header)
    {
        return is.read(header.header(), header.length());
    }
    
    friend std::ostream& operator<< (std::ostream& os, TCPHeader& header)
    {
        return os.write(header.header(), header.length());
    }

private:
    struct tcphPseudo 
    {    
        uint32_t ipSrc;    
        uint32_t ipDst;    
        uint8_t zero;       
        uint8_t  protocol;  
        uint16_t length;    
    };

    struct tcpChecksum 
    {
        struct tcphPseudo pseudo;
        headerType tcphdr;
    };

    uint16_t checksum(uint16_t *buf, int bufsz)
    {
        unsigned long sum = 0;
        while (bufsz > 1) {
            sum += *buf++;
            bufsz -= 2;
        }
        if (bufsz == 1)
            sum += *(unsigned char *)buf;
        sum = (sum & 0xffff) + (sum >> 16);
        sum = (sum & 0xffff) + (sum >> 16);
        return ~sum;
    }

    headerType rep_;
};

#endif /*TCPHEADER_HPP*/
