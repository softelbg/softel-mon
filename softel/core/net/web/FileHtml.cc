/*
 * FileHtml.cc
 *
 *  Created on: Apr 8, 2014
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2014 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "FileHtml.h"
#include <softel/core/util/CStringUtil.h>
#include <softel/core/system/CSystemTools.h>
#include <softel/core/net/server/ReplyHttpBase.h>
#include <softel/core/system/CShellCmd.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
using namespace SF::CORE::UTIL;
using namespace SF::CORE::SYSTEM;
using namespace SF::CORE::NET;
using namespace boost::filesystem;

namespace SF {
namespace WEB {

FileHtml::FileHtml(const string& pathBase, const RequestHttpBase& req) :
		m_path_base(pathBase), m_path(pathBase), m_request(req) {

	m_path_file = m_request.uri_file;

	m_path /= m_path_file;

	m_file_ext = extension(m_path);
	CStringUtil::remove('.', m_file_ext);
}

FileHtml::~FileHtml() {
	// TODO Auto-generated destructor stub
}

const string& FileHtml::getExtension() const {
	return m_file_ext;
}

bool FileHtml::validate() const {
	return (exists(m_path) && is_regular_file(m_path));
}

/**
 * Set Final HTTP Reply.
 * According to some special types of files should be handled different
 */
bool FileHtml::setHttpReply(string& reply) {

	if (m_file_ext.compare("sta") == 0) { // This STA handling is just for old softel dep...
		processSTA(reply);
	} else if (m_file_ext.compare("cgi") == 0) {
		processCGI(reply);
	} else if (m_file_ext.compare("php") == 0) {
		processPHP(reply);
	} else {
		return loadFile(reply);
	}

	return true;
}

bool FileHtml::loadFile(string& reply) {
	string full_path = m_path_base + m_path_file;
	return loadFile(full_path, reply);
}

/**
 * Load file to reply string...
 */
bool FileHtml::loadFile(const string& pathFile, string& reply) {
	// Open the file to send back.
	std::ifstream is(pathFile.c_str(), std::ios::in | std::ios::binary);
	if (!is) {
		return false;
	}

	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0) {
		reply.append(buf, is.gcount());
	}

	return true;
}

string FileHtml::toString() {
	stringstream ss;

	ss << endl << "FileHtml ---------------" << endl;

	ss << "   m_path_base: " << m_path_base << endl;
	ss << "   m_path_file: " << m_path_file << endl;
	ss << "   ext: " << getExtension() << endl;

	ss << endl << "FileHtml ---------------" << endl;

	return ss.str();
}

bool FileHtml::processPHP(std::string& reply) {
	stringstream cmd;

	cmd << "cd " << m_path_base << "; ";
	cmd << "php " << m_path_base << m_path_file << " ";
	for (size_t i = 0; i < m_request.m_token_arguments.size(); i++) {
		cmd << m_request.m_token_arguments[i] << " ";
	}

	cout << __FUNCTION__ << " cmd:" << cmd.str() << endl;

	CShellCmd shellCmd(cmd.str());
	shellCmd.run();

	reply = shellCmd.getResult();

	return true;
}

/**
 * Process legacy STA(N) executables...
 * Just for old web projects support
 *
 * Future web projects should account sofisticated plugins into the WebSrv
 *
 */
bool FileHtml::processSTA(std::string& reply) {

	stringstream cmd;
	stringstream pathResult;

	string cmdIp = "11.22.33.44"; // should have calling ip....

	cmd << m_path_base << m_path_file;

	cmd << " " << this << " " << m_path_base;
	cmd << " " << cmdIp << " " << m_path_file << " ";

	for (size_t i = 0; i < m_request.m_token_arguments.size(); i++) {
		cmd << m_request.m_token_arguments[i] << " ";
	}

	cmd << "callingip=" << cmdIp;

	cout << __FUNCTION__ << " cmd:" << cmd.str() << endl;

	CSystemTools::doSystem(cmd.str());

	pathResult << m_path_base << "/dir_sluj/CGIout_" << this << "_" << cmdIp
			<< ".htm";

	cout << __FUNCTION__ << " cmd:" << cmd.str() << endl;
	cout << __FUNCTION__ << " pathResult:" << pathResult.str() << endl;

	bool res = loadFile(pathResult.str(), reply);

	stringstream cmdDelResult;

	cmdDelResult << "rm -f " << m_path_file;

	cout << __FUNCTION__ << " cmdDelResult:" << cmdDelResult.str() << endl;
	CSystemTools::doSystem(cmdDelResult.str());

	return res;
}

/**
 * Process CGI executables, should be configurable if these are allowed...
 */
bool FileHtml::processCGI(std::string& reply) {

	stringstream cmd;

	cmd << m_path_base << m_path_file << " ";
	for (size_t i = 0; i < m_request.m_token_arguments.size(); i++) {
		cmd << m_request.m_token_arguments[i] << " ";
	}

	cout << __FUNCTION__ << " cmd:" << cmd.str() << endl;

	CShellCmd shellCmd(cmd.str());
	shellCmd.run();

	reply = shellCmd.getResult();

	return true;
}


} /* namespace WEB */
} /* namespace SF */
