/*
 * FolderCacheHtml.cc
 *
 *  Created on: Apr 5, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "FolderCacheHtml.h"

#include <sstream>
#include <boost/format.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace SF {
namespace WEB {

#define DIRINTERNAL "/dir_sluj/"

FolderCacheHtml::FolderCacheHtml(const std::string& base,
		const std::string& request) :
	BFolderCache(base, request),
	m_path_base(base),
	m_path_request(request) {

	// Add last /
	if (*m_path_request.rbegin() != '/') {
		m_path_request += "/";
	}
}

FolderCacheHtml::~FolderCacheHtml() {
	// TODO Auto-generated destructor stub
}

void
FolderCacheHtml::toHtml(std::string& result) {

	stringstream ss;

	ss << "<html><head></head><body>" << endl;

	ss << "<P><P><h1>Index of " << m_path_request << "</h1>" << endl;
	ss << "<table BORDER= \"0\" CELLPADDING=\"2\" CELLSPACING=\"1\">" << endl;
	ss << "<tr><td><font size=\"3\" class=\"normal\" color=#0000FF> Name </font></td>" << endl;

	ss << "<td><font size=\"3\" class=\"normal\" color=#0000FF> Size </font></td></tr>" << endl;
	ss << "<tr><td></td><td></td></tr>" << endl;

	// Show previous directory
	ss << "<tr><td valign=\"middle\"><IMG SRC=\"" << DIRINTERNAL << "pic_UPfolder.GIF\" width=\"22\" height=\"20\">";
	ss << "<A HREF=\"" << path(m_path_request + "../").string() << "\"><font color=#FF0000> Previous Dir";
	ss << "</font></A>&nbsp;</td><td> </td><td> </td>";
	ss << "</tr>" << endl;

	const vector<path>& folders = getFolders();
	for(size_t i = 0; i < folders.size(); i++) {
		ss << "<tr><td valign=\"middle\"><IMG SRC=\"" << DIRINTERNAL << "pic_folder.GIF\" width=\"22\" height=\"20\">";

		ss << "<A HREF=\"" << getFolderPath(folders[i]) << "\"><font color=#FF0000>" << folders[i].filename().string();
		ss << "</font></A>&nbsp;</td><td> </td><td> </td>";
		ss << "</tr>" << endl;
	}

	const vector<path>& files = getFiles();
	for(size_t i = 0; i < files.size(); i++) {
		// TODO: Make icon by file ext type
		ss << "<tr><td><IMG SRC=" << DIRINTERNAL << "pic_unknown_file.GIF width=\"22\" height=\"22\" />";
		ss << "<A HREF=\"" << getFilePath(files[i]) << "\">";
		ss << "<font size=\"2\" class=\"normal\" color=#0000FF>" << files[i].filename().string() << "</font></A>&nbsp;";
		ss << "</td><td align=\"right\"><font size=\"2\"> " << getFileSize(files[i]) << " </font></td>" << endl;
		ss << "</tr>" << endl;
	}

	ss << "</table>";

	// Softel Logo
	ss << "<br><br><br><img src=\"" << DIRINTERNAL << "logoSoftel.png\"/>";

	ss << "</body></html>" << endl;

	result = ss.str();
}

string
FolderCacheHtml::FolderCacheHtml::getFolderPath(const boost::filesystem::path& folderPath) {
	return m_path_request + folderPath.filename().string();
}

string
FolderCacheHtml::FolderCacheHtml::getFilePath(const boost::filesystem::path& filePath) {
	return m_path_request + filePath.filename().string();
}

std::string FolderCacheHtml::getFileSize(
		const boost::filesystem::path& filePath) {

	stringstream ss;
	float fileSize = file_size(filePath);

	//ss.precision(2);
	if(fileSize > 1073741824) {
		ss << format("%.2f") % (fileSize/1073741824) << " GB";
	} else if(fileSize > 1048576) {
		ss << format("%.2f") % (fileSize/1048576) << " MB";
	} else if(fileSize > 1024) {
		ss << format("%.2f") % (fileSize/1024) << " KB";
	} else {
		ss << fileSize << " bytes";
	}

	return ss.str();
}

} /* namespace WEB */
} /* namespace SF */
