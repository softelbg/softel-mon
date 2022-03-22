/*
 * CSharedCounter.h
 *
 *  Created on: 21.07.2012
 *      Author: STAN
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2010 Softel ltd Haskovo. All rights reserved.
 *
 */


#ifndef CSharedCounter_H
#define CSharedCounter_H

namespace SF {
namespace CORE {
namespace UTIL {

class CSharedCounter {

public:

	CSharedCounter();
	~CSharedCounter();

	void IncCounter();
	void DecCounter();
	int GetCounter();

	void resetCounter();

	void incCounter(int pos, int step);
	void decCounter(int pos, int step);
	int getCounter(int pos);
	void resetCounter(int pos);
	bool isCounterIndexValid(int pos);

	void incPosition();
	void decPosition();
	int getPosition();

	void setLock(bool val);
	bool isLocked();

private:

	int m_SharedMemID;

	volatile struct SharedMem {

		int count;

		int position;

		bool locked;

		int arr_count[64];

	}*m_pSharedMem;
};

} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

#endif

