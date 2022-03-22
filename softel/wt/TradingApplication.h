/*
 * TradingApplication.h
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#ifndef SOFTEL_WT_TRADINGAPPLICATION_H_
#define SOFTEL_WT_TRADINGAPPLICATION_H_

#include <Wt/WApplication>
#include <Wt/WContainerWidget>

namespace SF {
namespace WT {

class TradingApplication : public Wt::WApplication {
public:
	explicit TradingApplication(const Wt::WEnvironment& env);
	virtual ~TradingApplication();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_TRADINGAPPLICATION_H_ */
