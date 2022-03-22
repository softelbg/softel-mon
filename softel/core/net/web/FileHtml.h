/*
 * FileHtml.h
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef FILEHTML_H_
#define FILEHTML_H_

#include <boost/config.hpp>
#include <boost/filesystem/config.hpp>
#include <boost/filesystem.hpp>
#include <softel/core/net/server/RequestHttpBase.h>

#include <softel/core/CObject.h>

#include <string>
#include <map>

namespace SF {
namespace WEB {

class FileHtml: public SF::CORE::CObject {
public:
	FileHtml(const std::string& pathBase, const SF::CORE::NET::RequestHttpBase& req);
	virtual ~FileHtml();

	bool validate() const;

	const std::string& getExtension() const;

	bool setHttpReply(std::string& reply);

	std::string toString();

private:
	void doArguments();
	bool loadFile(const std::string& pathFile, std::string& reply);
	bool loadFile(std::string& reply);

	bool processSTA(std::string& reply);
	bool processCGI(std::string& reply);
	bool processPHP(std::string& reply);

	std::string m_path_base;
	std::string m_path_file;
	std::string m_file_ext;

	boost::filesystem::path m_path;
	SF::CORE::NET::RequestHttpBase m_request;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* FILEHTML_H_ */
