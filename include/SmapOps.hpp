#ifndef SMAPOPS_HPP
#define SMAPOPS_HPP

#include <string>
#include <utility>

#include <boost/core/noncopyable.hpp>
#include <boost/program_options.hpp>

#include "include/protocolraw.hpp"

namespace po = boost::program_options;

class SmapOps : private boost::noncopyable
{
public:
    static SmapOps* instance();
    void deleteSingletone();
    void readOps(int argc, char* argv[]);

    std::pair<int, int> portRange();

    std::string host()
    { return host_; }

    int family()
    { return fa_; }

    protocolraw::socket::protocol_type protocol();

private:
    SmapOps();

    std::string host_;
    std::pair<int, int> portRange_;
    po::options_description desc_;
    po::variables_map vm_;

    int fa_;
    bool udpscan_;
    protocolraw::socket::protocol_type pr_;

    static SmapOps* smapOps_;
};

#endif /* SMAPOPS_HPP */
