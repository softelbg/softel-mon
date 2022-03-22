/*
 * MarketDataWidget.h
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#ifndef SOFTEL_WT_MARKETDATAWIDGET_H_
#define SOFTEL_WT_MARKETDATAWIDGET_H_

#include <Wt/WContainerWidget>
#include <softel/core/common.h>

namespace SF {
namespace WT {

class MarketDataWidget : public Wt::WContainerWidget {
public:
	explicit MarketDataWidget(Wt::WContainerWidget *parent = 0);
	virtual ~MarketDataWidget();

private:
	void start();
	void doWork(Wt::WApplication *app);

	std::map<std::string, Wt::Chart::WCartesianChart*> charts_;
	std::map<std::string, Wt::WStandardItemModel*> models_;

	Wt::WTable *table_;
	Wt::WProgressBar *progress_;
	Wt::WPushButton *startButton_;
	Wt::WPushButton *stopButton_;

	boost::thread workThread_;
	std::atomic<bool> isWorkerStopped_;
};

} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_MARKETDATAWIDGET_H_ */
