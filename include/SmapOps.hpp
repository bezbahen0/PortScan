#ifndef SMAPOPS_HPP
#define SMAPOPS_HPP

#include <string>
#include <utility>
	
#include <boost/core/noncopyable.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class SmapOps : private boost::noncopyable
{
public:
    static SmapOps* instance();
    void deleteSingletone();
    void readOps(int argc, char* argv[]);
    std::pair<int, int> portRange();
private:
    SmapOps();
    
    po::options_description desc_;
    po::variables_map vm_;
    static SmapOps* smapOps_;
};

#endif /* SMAPOPS_HPP */
