#pragma once

#include "Options.h"

namespace SF {
namespace CORE {
namespace UTIL {


class IParam
{
public:
    ~IParam() {}
    virtual std::string toString() const = 0;
    virtual void init(const Options& opt) = 0;
};

template <class T>
class OptionParam : virtual public IParam
{
public:
    explicit OptionParam(const std::string& key) : key_(key), valueDimension_("") {}
    OptionParam(const std::string& key, const T& defaultValue)
        : key_(key), value_(defaultValue), defaultValue_(defaultValue), valueDimension_("") {}

    OptionParam(const std::string& key, const T& defaultValue, const std::string& valueDimension)
        : key_(key), value_(defaultValue), defaultValue_(defaultValue), valueDimension_(valueDimension) {}

    OptionParam(const Options& opt, const std::string& key)
        : key_(key), valueDimension_("")
    {
        init(opt);
    }

    OptionParam(const Options& opt, const std::string& key, const T& defaultValue)
        : key_(key), value_(defaultValue), defaultValue_(defaultValue), valueDimension_("")
    {
        init(opt);
    }

    OptionParam(const Options& opt, const std::string& key, const T& defaultValue, const std::string& valueDimension)
        : key_(key), value_(defaultValue), defaultValue_(defaultValue), valueDimension_(valueDimension)
    {
        init(opt);
    }

    virtual ~OptionParam() {}

    T getValue() const {
        return value_;
    }

    bool isFound() const {
        return isFound_;
    }

    std::string toString() const {
        std::ostringstream os;
        os << key_ << "=" << value_ << (valueDimension_.empty() ? "" : " <" + valueDimension_ + ">");
        return os.str();
    }

    void init(const Options& opt) {
        isFound_ = opt.has(key_);

        if (isFound_) {
            try {
                value_ = boost::lexical_cast<T>(opt.get(key_));
            }
            catch (boost::bad_lexical_cast&) {}
        }
        else {
            value_ = defaultValue_;
        }
    }

private:
    std::string key_;
    T value_;
    T defaultValue_;
    std::string valueDimension_;
    bool isFound_;
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */
