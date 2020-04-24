#ifndef BINARYOPTYON_HPP
#define BINARYOPTYON_HPP

#include <netinet/in.h>
#include <sys/socket.h>

template<int Level, int Name, bool Init = true>
class BinaryOption
{
public:
    BinaryOption() = default;
    BinaryOption(bool optval) : optval_(optval){}
    ~BinaryOption() = default;

    template<typename Protocol>
    int level(Protocol const&) const
    { return Level; }

    template<typename Protocol>
    int name(Protocol const&) const
    { return Name; }

    template<typename Protocol>
    void* data(Protocol const&)
    { return reinterpret_cast<void*>(&optval_); }

    template<typename Protocol>
    void const* data(Protocol const&) const
    { return reinterpret_cast<void const*>(&optval_); }

    template<typename Protocol>
    int size(Protocol const&) const
    { return sizeof(optval_); }

    void set_value(bool value)
    { optval_ = value; }

    bool get_value() const
    { return optval_; }

private:
    bool optval_ = Init;
};

using iphdrincl = BinaryOption<SOL_IP, IP_HDRINCL, true>;

#endif /* BINARYOPTYON_HPP */
