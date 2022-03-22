#pragma once


namespace SF
{
namespace CORE
{


struct IZeroMQHelper;


class ZMQBase
{
public:
    virtual ~ZMQBase();

    void connect();
    void bind();


    std::shared_ptr<IZeroMQHelper> getHelper() {
        return zmqHelper_;
    }

    std::shared_ptr<void> getContext() {
        return zmqContext_;
    }

    boost::shared_ptr<void> getSocket() {
        return zmqSocket_;
    }

    // Helper socket configuration readers
    int getRecvMore() const;

    static void destroy();

protected:
    explicit ZMQBase(const std::string& url);

    void init();

    std::string url_;

    static std::shared_ptr<IZeroMQHelper> zmqHelper_;
    static std::shared_ptr<void> zmqContext_;

    boost::shared_ptr<void> zmqSocket_;
};


}
}
