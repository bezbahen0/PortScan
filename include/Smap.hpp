#ifndef Smap_HPP
#define Smap_HPP

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
//#define BOOST_ASIO_HAS_TIMERFD
#include <string>
#include <tuple>
#include <map>
#include <set>
#include <chrono>
#include <memory>
#include <netinet/in.h>

#include <boost/asio/io_context.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/basic_raw_socket.hpp>

#include "protocolraw.hpp"
#include "RouteTableIpv4.hpp"
#include "RouteTableIpv6.hpp"
#include "BinaryOptyon.hpp"

using boost::system::error_code; 
namespace chrono = std::chrono;

class Smap
{
    using bufferType = boost::asio::streambuf;
    using timerType = boost::asio::basic_waitable_timer<chrono::steady_clock>;
    using sharedTimer = std::shared_ptr<timerType>;
    using sharedBuffer = std::shared_ptr<bufferType>;

    struct scanInfo
    {
        int port;
        chrono::steady_clock::time_point sendTime;
        int seqNum;
        int ownPort;
    };

public:

    enum statePort
    {
        open,
        closed,
        filtered,
        aborted
    };

    enum
    {
        defaultTimeout = 3000,
        bufferSize = 1550
    };

    Smap(boost::asio::io_context& io_context, const std::string& host, protocolraw::socket::protocol_type protocol,int millisec);
    ~Smap();

    void startScan(int portn);
    std::map<int, statePort> const& portMap() const;

private:

    void startTimer(int millisec, scanInfo info, sharedTimer timer);
    void startReceive(scanInfo info, sharedTimer timer);
    void handleScan(error_code const& ec, std::size_t length, scanInfo info, sharedBuffer buffer);
    void handleReceive(error_code const& ec, std::size_t length, scanInfo info, sharedBuffer buffer, sharedTimer timer);
    void timeout(error_code const& ec, scanInfo info, sharedTimer timer);
    std::tuple<int, int> createSegment(bufferType& buffer, int port);
    std::tuple<int, int> createSegmentIpv4(bufferType& buffer, int port);
    std::tuple<int, int> createSegmentIpv6(bufferType& buffer, int port);
    void addMap(int keyPort, statePort stateport);

    int timeoutMilisecond_;
    std::set<int> timeoutPort_;
    boost::asio::io_context& io_;
    protocolraw::socket socket_;
    protocolraw::endpoint destination_;
    protocolraw::socket::protocol_type protocol_;
    std::map<int, statePort> portMap_;
    RouteTableIpv4 rtip4_;
    RouteTableIpv6 rtip6_;
};

#endif /*Smap_HPP*/
