#include <iostream>
#include <ostream>
#include <istream>
#include <chrono>
#include <functional>


#include "include/Smap.hpp"
#include "include/TCPHeader.hpp"
#include "include/IPv4Header.hpp"

using std::placeholders::_1;
using std::placeholders::_2;
using namespace boost;

uint32_t getIfaddrIpv4(std::string const& ifname)
{   
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    return ntohl(((struct sockaddr_in *) &ifr.ifr_addr) -> sin_addr.s_addr);
 }

Smap::Smap(boost::asio::io_context& io, const std::string& host, int millisec) :
    timeoutMilisecond_(millisec), 
    io_(io),
    socket_(io, tcp::v4())
{
    tcp::resolver resolver(io);
    tcp::resolver::query query(tcp::v4(), host, "", boost::asio::ip::resolver_query_base::numeric_service);
    destination_ = *resolver.resolve(query);
    socket_.set_option(iphdrincl(true));
}

Smap::~Smap()
{

}

void Smap::startScan(int portn)
{
    auto buffer = std::make_shared<bufferType>();
    createSegment(*buffer, portn);
    auto sendTime = chrono::steady_clock::now();
    socket_.async_send_to(buffer -> data(), destination_,
        std::bind(&Smap::handleScan, this, _1, _2, Smap::scanInfo{portn, sendTime}, buffer)
    );
}

std::map<int, Smap::statePort> const& Smap::portMap() const
{
    return portMap_;
}

void Smap::startTimer(int millisec, scanInfo info, sharedTimer timer)
{
    timer -> expires_from_now(chrono::milliseconds(millisec));
    timer -> async_wait(std::bind(&Smap::timeout, this, _1, info, timer));
}

void Smap::startReceive(scanInfo info, sharedTimer timer)
{
    auto&& buffer = std::make_shared<bufferType>();
    socket_.async_receive(buffer -> prepare(bufferSize), 
        std::bind(&Smap::handleReceive, this, _1, _2, info, buffer, timer)
    );
}


void Smap::handleScan(error_code const& ec, std::size_t length, scanInfo info, sharedBuffer buffer)
{
    if(ec)
    {
        std::cerr << "Scan error: " << ec.message() << std::endl;
    }
    else
    {
        sharedTimer timer = std::make_shared<timerType>(io_);
        startTimer(timeoutMilisecond_, info, timer);
        startReceive(info, timer);
    }
}


void Smap::handleReceive(error_code const& ec, std::size_t length, scanInfo info, sharedBuffer buffer, sharedTimer timer)
{
    if(ec == asio::error::operation_aborted)
    {
        if(timeoutPort_.find(info.port) == timeoutPort_.end())
        {
            startReceive(info, timer);
        }
        else
        {
            addMap(info.port, statePort::filtered);
        }
        return;
    }
    else if(ec)
    {
        std::cerr << "Receive error: " << ec.message() << std::endl;
        addMap(info.port, statePort::aborted);
    }
    else
    {
        buffer -> commit(length);

        IPv4Header ipv4;
        TCPHeader tcp;

        std::istream is(&(*buffer));
        is >> ipv4 >> tcp;

        if(tcp.syn() && tcp.ack())
        {
            portMap_[tcp.source()] = statePort::open;
        }
        else if(tcp.rst() && tcp.ack())
        {
            portMap_[tcp.source()] = statePort::closed;
        }
        else
        {
            startReceive(info, timer);
            return;
        }
    }
    timer -> cancel();

}

void Smap::timeout(error_code const& ec, scanInfo info, sharedTimer timer)
{
    if(ec == asio::error::operation_aborted);
    else if(ec)
    {
        std::cerr << "Error: timer handler: " << ec.message() << std::endl;
    }
    else
    {
        timeoutPort_.insert(info.port);
        socket_.cancel();
    }
}

std::tuple<int, int> Smap::createSegment(bufferType& buffer, int port)
{
    buffer.consume(buffer.size());
    std::ostream os(&buffer);

    IPv4Header ipv4Header;
    ipv4Header.version(4);
    ipv4Header.headerLength(ipv4Header.length() / 4);
    ipv4Header.typeOfService(0x10);
    ipv4Header.fragmentOffset(IP_DF);
    ipv4Header.ttl(IPDEFTTL);
    ipv4Header.protocol(IPPROTO_TCP);
    uint32_t daddr = destination_.address().to_v4().to_ulong();
    ipv4Header.sourceAddress(getIfaddrIpv4(rtip4_.find(daddr) -> ifname));
    ipv4Header.destinationAddress(daddr);

    uint16_t source = rand();
    uint32_t seq = rand();
    TCPHeader tcpHeader;
    tcpHeader.source(source);
    tcpHeader.destination(port);
    tcpHeader.seq(seq);
    tcpHeader.doff(20/4);
    tcpHeader.syn(true);
    tcpHeader.window(TCPHeader::defaultWindowValue);
    tcpHeader.computeChecksum(ipv4Header.sourceAddress().to_ulong(), ipv4Header.destinationAddress().to_ulong());

    ipv4Header.totalLength(ipv4Header.length() + tcpHeader.length());
    ipv4Header.checksum();


    if(!(os << ipv4Header << tcpHeader))
    {
        throw std::runtime_error("cannot receive packet");
    }
    return std::make_tuple(source, seq);
}

void Smap::addMap(int keyPort, statePort stateport)
{
    if(portMap_.find(keyPort) == portMap_.end())
    {
        portMap_[keyPort] = stateport;
    }
}
