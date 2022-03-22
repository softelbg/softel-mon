/**
 * BFolderCache.cc
 *
 *  Created on: 20.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "BFolderCache.h"
#include <sstream>

using namespace std;
using namespace SF::CORE::ERROR;
using namespace boost::filesystem;

namespace SF {
namespace CORE {
namespace NET {

BFolderCache::BFolderCache(const string& path) :
   m_boost_path(path) {

   list();
}

BFolderCache::BFolderCache(const std::string& path1, const std::string& path2) :
   m_boost_path(path1) {

   m_boost_path /= path2;
   
   list();
}

void
BFolderCache::list() {

   clear();
   
   if (validate()) {
      directory_iterator end_iter;
      for(directory_iterator dir_iter(m_boost_path); dir_iter != end_iter; ++dir_iter) {        
         m_dir_list.push_back(*dir_iter);
         
         // dispatch by type
         if (is_regular_file(dir_iter->status())) {
            m_files.push_back(*dir_iter);
         } else if (is_directory(dir_iter->status())) {
            m_folders.push_back(*dir_iter);
         } else {
            m_others.push_back(*dir_iter);
         }
      }
   }
}

bool
BFolderCache::validate() const {
   return (exists(m_boost_path) && is_directory(m_boost_path));
}

size_t
BFolderCache::size() const {
   return m_dir_list.size();
}

bool
BFolderCache::empty() const {
   return m_dir_list.empty();
}

void
BFolderCache::clear() {
   m_dir_list.clear();
}
   
const path&
BFolderCache::getPath() const {
   return m_boost_path;
}

const path&
BFolderCache::get(size_t pos) const {
   return m_dir_list[pos];
}

bool
BFolderCache::get(size_t pos, path& result) const {
   if (validatePos(pos)) {
      result = get(pos);
      return true;
   } else {
      return false;
   }
}

bool
BFolderCache::validatePos(size_t pos) const {
   if (pos >= 0 && !m_dir_list.empty() && pos < m_dir_list.size()) {
      return true;
   } else {
      return false;
   }
}

const vector<path>&
BFolderCache::getFiles() const {
   return m_files;
}

const vector<path>&
BFolderCache::getFolders() const {
   return m_folders;
}

const vector<path>&
BFolderCache::getOthers() const {
   return m_others;
}
   
std::string
BFolderCache::toString() {
   stringstream ss;
   
   ss << endl << "BFolderCache ---------------" << endl;
   
   if (validate()) {
      
      ss << "Folders: ";
      for(int i = 0; i < m_folders.size(); i++) {
         ss << "[" << m_folders[i].filename() << "] ";
      }
      
      ss << endl << "Files: ";
      for(int i = 0; i < m_files.size(); i++) {
         ss << "[" << m_files[i].filename() << "]";
         ss << "[" << m_files[i].string() << "] ";
      }
   
   } else {
      ss << "INVALID FOLDER: " << m_boost_path.string();
   }
   
   ss << endl << "BFolderCache ---------------" << endl;
   
   return ss.str();
}

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */



