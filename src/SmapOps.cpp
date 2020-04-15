#include "include/SmapOps.hpp"

#include <iostream>
#include <exception>

#include <netinet/in.h>

#include <boost/lexical_cast.hpp>

SmapOps* SmapOps::smapOps_ = nullptr;

SmapOps* SmapOps::instance()
{
    if(!smapOps_)
    {
        smapOps_ = new SmapOps();
    }
    return smapOps_;
}

void SmapOps::deleteSingletone()
{
    vm_.clear();
    delete smapOps_;
}

protocolraw::socket::protocol_type SmapOps::protocol()
{
    if(udpscan_)
    {
        pr_.protocol(IPPROTO_UDP);
    }
    else
    {
        pr_.protocol(IPPROTO_TCP);
    }
    return pr_;
}

void SmapOps::readOps(int argc, char* argv[])
{
    try
    {
        po::positional_options_description p;
        p.add("hosts", -1);
        po::store(po::command_line_parser(argc, argv).
                  options(desc_).positional(p).run(), vm_);
        po::notify(vm_);

        if(vm_.count("help"))
        {
            std::cout << desc_ << std::endl;
            return;
        }
        if(vm_.count("ipv6"))
        {
            fa_ = AF_INET6;
            pr_ = protocolraw::socket::protocol_type::v6();
        }
        if(vm_.count("udp"))
        {
            udpscan_ = true;
        }
        if(vm_.count("hosts"))
        {
            host_ = vm_["hosts"].as<std::string>();
        }



        portRange_ = portRange();
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

std::pair<int, int> SmapOps::portRange()
{
    if(!vm_.count("port-range"))
    {
        return std::make_pair(1, 1024);
    }

    std::string rangePort =  vm_["port-range"].as<std::string>();
    std::string::size_type delimeterPos = rangePort.find('-');

    int beginPort = boost::lexical_cast<int>(rangePort.substr(0, delimeterPos));
    int endPort = boost::lexical_cast<int>(rangePort.substr(delimeterPos + 1));
    
    if(beginPort < 1 || beginPort > 65535 || endPort < 1 || endPort > 65535)
    {
        throw std::runtime_error("Ports specified must be between 0 and 65535 inclusive");
    }

    return std::make_pair(beginPort, endPort);
}

SmapOps::SmapOps() : fa_(AF_INET), pr_(protocolraw::v4())
{
    desc_.add_options()
            ("help,h", "Print help")
            ("udp,u", "Start udp scanning")
            ("hosts", po::value<std::string>(), "hosts range or host")
            ("ipv6,6", "Enable Ipv6 scanning")
            ("out,o", po::value<std::string>(), "Create results.txt file")
            ("port-range,p", po::value<std::string>(), "Port range");
}
