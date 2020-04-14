#include <iostream>
#include <exception>

#include "include/Smap.hpp"
#include "include/SmapOps.hpp"

int main(int argc, char* argv[])
{
    try
    {
        auto ops = SmapOps::instance();
        ops -> readOps(argc, argv);

        std::pair<int, int> portRange = ops -> portRange();

        boost::asio::io_context io_context;
        Smap smap(io_context, ops -> host(), Smap::defaultTimeout);

        for(int beginPort = portRange.first; beginPort <= portRange.second; ++beginPort)
        {
            smap.startScan(beginPort);
        }
        std::cout << "Now starting scan to " << ops -> host()  << std::endl;
        io_context.run();

        std::cout << "PORT\tSTATE" << std::endl;
        for (auto pair : smap.portMap()) 
        {
            using pstate = Smap::statePort ;

            static std::map<pstate, std::string> const pstr = {
                {pstate::open,     "open"},     {pstate::closed,  "closed"},
                {pstate::filtered, "filtered"}, {pstate::aborted, "aborted"}
            };

            std::cout << pair.first << '\t' << pstr.at(pair.second) << std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Exeption: "<< e.what() << std::endl;
    }
    return 0;
}
