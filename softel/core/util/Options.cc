
#include <softel/core/common.h>
#include "Options.h"

#ifdef WIN32
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#else
#include <unistd.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

namespace CN = SF::CORE::UTIL;

CN::Options::Options()
: sep_("=")
{
}

CN::Options::Options(int argc, char *argv[], const std::string& sep)
: sep_(sep)
{
	map_["exe"] = argv[0]; // Add the argv[0] which is the executable name.
	fillMinusOptions(argc, argv);
	fill(argc, argv, sep, map_);
	toUpper();
}

CN::Options::Options(const TStr2Str::const_iterator& begin, const TStr2Str::const_iterator& end)
{
	merge(begin, end);
	toUpper();
}

CN::Options::~Options()
{
}

void CN::Options::merge(const Options& opt)
{
	merge(opt.cbegin(), opt.cend());
}

void CN::Options::merge(const TStr2Str::const_iterator& begin, const TStr2Str::const_iterator& end)
{
	map_.insert(begin, end);
}

bool CN::Options::has(const std::string& key) const
{
	bool result = false;
	std::string keyToUpper = boost::algorithm::to_upper_copy(key);

	if (find(keyToUpper) != cend()) {
		result = true;
	}

	return result;
}

std::string CN::Options::get(const std::string& key, const std::string& defVal) const
{
    std::string result = defVal;

	std::string keyToUpper = boost::algorithm::to_upper_copy(key);

	auto it = find(keyToUpper);

	if (it != cend()) {
		result = it->second;
	}

	return result;
}

int CN::Options::getInt(const std::string& key) const
{
	try {
		return boost::lexical_cast<int>(get(key));
	}
	catch (boost::bad_lexical_cast &) {
		return 0;
	}
}

std::string CN::Options::toString()
{
	std::stringstream ss;

	ss << __FUNCTION__ << " -------------------" << std::endl;
	ss << "size = " << size() << std::endl;

	for (auto it = cbegin(); it != cend(); ++it) {
		ss << it->first << " = " << it->second << std::endl;
	}

	ss << "--------------------------------------" << std::endl;

	return ss.str();
}

void
CN::Options::fillMinusOptions(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg[0] == '-') {
			map_[arg] = arg;
		}
	}
}

void
CN::Options::fillSingle(const std::string& line, const std::string& sep, std::map<std::string, std::string>& m)
{
	// Simple tokenizer for "key=val" template.
	size_t pos = line.find_first_of(sep); // Using multiple delimiter chars, but only a single one is enough
	if (pos != std::string::npos) {
		m[line.substr(0, pos)] = line.substr(pos + 1, std::string::npos);
	}
}

void
CN::Options::fill(int argc, char *argv[], const std::string& sep, std::map<std::string, std::string>& m)
{
	for (int i = 1; i < argc; ++i) {
		fillSingle(argv[i], sep, m);
	}
}

std::tuple<std::string, std::string> CN::Options::getPathComponents()
{
	char cbinPath[MAX_PATH] = { 0 };
#ifdef WIN32
	GetModuleFileName(NULL, cbinPath, MAX_PATH);
#else
	getcwd(cbinPath, MAX_PATH);
#endif

	std::string binPath(cbinPath);

	size_t pos = binPath.find_last_of("\\");
	std::string pathBase = binPath.substr(0, pos);
	std::string exe = binPath.substr(pos + 1);

	return std::make_tuple(pathBase, exe);
}

std::string CN::Options::getExecutableName()
{
	return std::get<1>(getPathComponents());
}

#ifdef WIN32
std::string CN::Options::getDllFileName()
{
	CHAR dllFullPath[MAX_PATH] = { 0 };
	GetModuleFileName((HINSTANCE)&__ImageBase, dllFullPath, _countof(dllFullPath));
	boost::filesystem::path path(dllFullPath);
	return path.filename().string();
}
#endif

std::string
CN::Options::getBasePath()
{
	return std::get<0>(getPathComponents());
}

void CN::Options::toUpper()
{
	TStr2Str newMap;

	for (TStr2Str::iterator it = map_.begin(); it != map_.end(); ++it)	{
		std::string keyToUpper = boost::algorithm::to_upper_copy(it->first);
		newMap[keyToUpper] = it->second;
	}

	std::swap(map_, newMap);
}

std::string& CN::Options::operator[] (const std::string& key)
{
    return map_[boost::to_upper_copy(key)];
}

CN::Options::TStr2Str::const_iterator CN::Options::find(const std::string& key) const
{
    return map_.find(boost::to_upper_copy(key));
}

CN::OptionsFromFile::OptionsFromFile(const std::string& path, const std::string& sep)
: TextFile(path)
{
	sep_ = sep;
	init();
	toUpper();
}

void
CN::OptionsFromFile::processLine(const std::string& line)
{
	fillSingle(line, sep_, map_);
}
