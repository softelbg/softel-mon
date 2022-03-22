/**
 * BFolderCache.h
 *
 * Directory representation with the containing objects (files, folders...)
 * Currently implemented on top of boost::filesystem::path
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef BFolderCache_H_
#define BFolderCache_H_

#include <softel/core/CObject.h>
#include <softel/core/error/CErrorCommon.h>

#include <boost/config.hpp>
#include <boost/filesystem/config.hpp>
#include <boost/filesystem.hpp>

namespace SF {
namespace CORE {
namespace NET {

class BFolderCache : public SF::CORE::CObject {

public:
   BFolderCache(const std::string& path);
   BFolderCache(const std::string& path1, const std::string& path2);
   virtual ~BFolderCache() {}

   void list();
   
   size_t size() const;
   bool empty() const;
   bool validate() const;
   
   const boost::filesystem::path& getPath() const;
   
   const boost::filesystem::path& get(size_t pos) const;
   bool get(size_t pos, boost::filesystem::path& result) const;

   const std::vector<boost::filesystem::path>& getFiles() const;
   const std::vector<boost::filesystem::path>& getFolders() const;
   const std::vector<boost::filesystem::path>& getOthers() const;
   
   std::string toString();
   
private:
   bool validatePos(size_t pos) const;
   void clear();
   
   boost::filesystem::path m_boost_path;
   
   std::vector<boost::filesystem::path> m_dir_list;
   
   std::vector<boost::filesystem::path> m_files;
   std::vector<boost::filesystem::path> m_folders;
   std::vector<boost::filesystem::path> m_others;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */


#endif /* BFolderCache_H_ */
