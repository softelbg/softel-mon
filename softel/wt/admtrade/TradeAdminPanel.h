/*
 * TradeAdminPanel.h
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_ADMTRADE_TRADEADMINPANEL_H_
#define SOFTEL_WT_ADMTRADE_TRADEADMINPANEL_H_

#include <Wt/WApplication>

namespace SF {
namespace WT {

class TradeAdminPanel : public Wt::WApplication {
public:
    explicit TradeAdminPanel(const Wt::WEnvironment& env);
    virtual ~TradeAdminPanel();
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_ADMTRADE_TRADEADMINPANEL_H_ */
