/*
 * FolderCacheHtml.h
 *
 *  Created on: Apr 5, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef FOLDERCACHEHTML_H_
#define FOLDERCACHEHTML_H_

#include <softel/core/net/server/BFolderCache.h>

namespace SF {
namespace WEB {

class FolderCacheHtml: public SF::CORE::NET::BFolderCache {
public:
	FolderCacheHtml(const std::string& base, const std::string& request);
	virtual ~FolderCacheHtml();

	void toHtml(std::string& result);

private:
	std::string getFolderPath(const boost::filesystem::path& folderPath);
	std::string getFilePath(const boost::filesystem::path& filePath);
	std::string getFileSize(const boost::filesystem::path& filePath);

	std::string m_path;
	std::string m_path_base;
	std::string m_path_request;
};

} /* namespace WEB */
} /* namespace SF */
#endif /* FOLDERCACHEHTML_H_ */
