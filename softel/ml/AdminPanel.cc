/*
 * AdminPanel.cc
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "AdminPanel.h"
#include "MLMachine.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include <softel/wt/common.h>
#include <thread>

#include <Wt/WServer>
#include <Wt/WTextArea>

using namespace std;
using namespace SF::CORE;
using namespace Wt;

namespace SF {
namespace WT {

RNNWidget::RNNWidget(const std::string& name, Wt::WContainerWidget *parent)
: WContainerWidget(parent), name_(name)
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;

    WTextArea * wtxt = new WTextArea(this);
    wtxt->setColumns(120);
    wtxt->setRows(40);
    
    // Enable server push
    Wt::WApplication::instance()->enableUpdates(true);
    
    std::function<void()> fn = [wtxt, this] () {
        std::deque<std::string> msg = SF::ML::Machines::m->getMsg(name_);
        size_t sz = msg.size();
        std::string strmsg;
        for (size_t i = 0; i < sz; i++) {
            strmsg += msg[i] + "\n";
        }

        wtxt->setText(WString::fromUTF8(strmsg));
        
        Wt::WApplication::instance()->triggerUpdate();
    };
    
    SF::ML::Machines::m->subscribe(Wt::WApplication::instance()->sessionId(), this, fn);
    
    fn();
}

RNNWidget::~RNNWidget() noexcept (true)
{
    Wt::log("info") << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__;
    SF::ML::Machines::m->unsubscribe(this);
    Wt::WApplication::instance()->enableUpdates(false);
}

AdminPanel::AdminPanel(const Wt::WEnvironment& env)
: WApplication(env) {

    setTitle("ML Admin Panel");

    useStyleSheet("style/main.css");

    messageResourceBundle().use(appRoot() + "admlayout");

    WTabWidget* tab = new WTabWidget(root());

    std::vector<std::string> names = SF::ML::Machines::m->getNames();

    // Set tabs
    for (auto & name : names) {
        tab->addTab(new RNNWidget(name), name, Wt::WTabWidget::PreLoading);
    }
    
    tab->setStyleClass("tabwidget");
}

AdminPanel::~AdminPanel() {
    // TODO Auto-generated destructor stub
}


} /* namespace WT */
} /* namespace SF */
