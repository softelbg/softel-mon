#pragma once

#include "MapBase.hpp"
#include "TextFile.h"

namespace SF {
namespace CORE {
namespace UTIL {
    

class Options : public MapBase<std::string, std::string>
{
public:
    typedef std::map<std::string, std::string> TStr2Str;

    Options();
    Options(int argc, char *argv[], const std::string& sep);
    Options(const TStr2Str::const_iterator& begin, const TStr2Str::const_iterator& end);
    virtual ~Options();

    void merge(const Options& opt);
    void merge(const TStr2Str::const_iterator& begin, const TStr2Str::const_iterator& end);
    bool has(const std::string& key) const;

    template <class IT> void initOptions(IT begin, IT end) {
    	clear();
    	for (IT it = begin; it != end; ++it) {
    		fillSingle(*it, sep_, map_);
    	}
    	toUpper();
    }

    std::string& operator[] (const std::string& key);
    TStr2Str::const_iterator find(const std::string& key) const;

    // Get Always result
    std::string get(const std::string& key, const std::string& defVal="") const;
    int getInt(const std::string& key) const;

    // Static helper functions
    // TODO: could belong to some algo name space...
    static void fill(int argc, char *argv[], const std::string& sep, std::map<std::string, std::string>& m);
    static void fillSingle(const std::string& line, const std::string& sep, std::map<std::string, std::string>& m);

    static std::string getBasePath(); // TODO: move to proper component
	static std::string getExecutableName();
	static std::string getDllFileName();

    std::string toString();

protected:
	static std::tuple<std::string, std::string> getPathComponents();
    void fillMinusOptions(int argc, char *argv[]);
	void toUpper();

    std::string sep_;
};

class OptionsFromFile : virtual public TextFile, public Options
{
public:
    OptionsFromFile(const std::string& path, const std::string& sep);
    virtual ~OptionsFromFile() {}

    virtual void processLine(const std::string& line);
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

