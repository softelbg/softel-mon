/*
 * FFT.h
 *
 *  Created on: May 6, 2015
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2015 Softel ltd Haskovo. All rights reserved.
 *
 */

#ifndef FFT_H_
#define FFT_H_

#include <softel/core/CObject.h>

namespace SF {
namespace DSP {

class FFT: public SF::CORE::CObject {
public:
	FFT();
	virtual ~FFT();
};

} /* namespace DSP */
} /* namespace SF */
#endif /* FFT_H_ */
