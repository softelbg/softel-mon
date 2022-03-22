/*
 * MimeTypes.h
 *
 *  Created on: Dec 3, 2013
 *      Author: sgeorgiev
 */

#ifndef MIMETYPES_H_
#define MIMETYPES_H_

#include <string>
#include <map>

namespace SF {
namespace CORE {
namespace NET {

class MimeTypes {

public:
	static std::string getMimeType(const std::string& ext);
	static void init();

private:
	MimeTypes();

	static MimeTypes* m_pInstance;
	std::map<std::string, std::string>	m_mime_types;
};

} /* namespace NET */
} /* namespace CORE */
} /* namespace SF */
#endif /* MIMETYPES_H_ */
