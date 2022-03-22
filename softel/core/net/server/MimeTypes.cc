/*
 * MimeTypes.cc
 *
 *  Created on: Dec 3, 2013
 *      Author: sgeorgiev
 */

#include "MimeTypes.h"

namespace SF {
namespace CORE {
namespace NET {

MimeTypes* MimeTypes::m_pInstance = NULL;

MimeTypes::MimeTypes() {
	m_mime_types.clear();

	m_mime_types["htm"] = "text/html";
	m_mime_types["html"] = "text/html";
	m_mime_types["cgi"] = "text/html";
	m_mime_types["sta"] = "text/html";
	m_mime_types["php"] = "text/html";

	m_mime_types["gif"] = "image/gif";
	m_mime_types["jpg"] = "image/jpeg";
	m_mime_types["png"] = "image/png";
}

/**
 * Search for the mime type by file extension
 */
std::string MimeTypes::getMimeType(const std::string& ext) {

	init();

	std::map<std::string, std::string>::const_iterator it =
			MimeTypes::m_pInstance->m_mime_types.find(ext);

	if (it != MimeTypes::m_pInstance->m_mime_types.end()) {
		return it->second;
	}

	return "text/plain";
}

void MimeTypes::init() {
	if (MimeTypes::m_pInstance == NULL) {
		MimeTypes::m_pInstance = new MimeTypes();
	}
}


} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */

