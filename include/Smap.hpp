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

#include "tcp.hpp"
#include "RouteTable.hpp"
#include "BinaryOptyon.hpp"

using boost::system::error_code; 
namespace chrono = std::chrono;
namespace asio = boost::asio;

class Smap
{
    using bufferType = asio::streambuf;
    using timerType = asio::basic_waitable_timer<chrono::steady_clock>;
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

    Smap(boost::asio::io_context& io_context, const std::string& host, int millisec);
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
    void addMap(int keyPort, statePort stateport);

    int timeoutMilisecond_;
    std::set<int> timeoutPort_;
    boost::asio::io_context& io_;
    tcp::socket socket_;
    tcp::endpoint destination_;
    std::map<int, statePort> portMap_;
    RouteTable rt_;
};

#endif /*Smap_HPP*/
