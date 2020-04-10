#include "include/SmapOps.hpp"

#include <iostream>
#include <exception>

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

void SmapOps::readOps(int argc, char* argv[])
{
    try
    {
        po::store(po::parse_command_line(argc, argv, desc_), vm_);
        po::notify(vm_);

        if(vm_.count("help"))
        {
            std::cout << desc_ << std::endl;
            return;
        }
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

SmapOps::SmapOps()
{
    desc_.add_options()
        ("help,h", "Print help")
        ("udp,u", "Start udp scanning")
        ("ipv6,6", "Enable Ipv6 scanning")
        ("out,o", po::value<std::string>(), "Create results.txt file")
        ("port-range,p", po::value<std::string>(), "Port range");
}
