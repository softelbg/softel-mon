/*
 * MarketDataWidget.cc
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#include "MarketDataWidget.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include <softel/trade/idl/messages.pb.h>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/core/net/zmq/CGuardZMQMsg.hpp>
#include "common.h"


namespace SF {
namespace WT {

using namespace std;
using namespace SF::CORE;
using namespace Wt;

static const int NUM_IT = 10000;
// Symbols used for charting
static const std::vector<std::string> symbols = {"EUR/USD"};
static const int numPx = 400;

MarketDataWidget::MarketDataWidget(WContainerWidget *parent)
: WContainerWidget(parent)
{
	startButton_ = new Wt::WPushButton("Start", this);
	startButton_->clicked().connect(startButton_, &Wt::WPushButton::disable);
	startButton_->clicked().connect(this, &MarketDataWidget::start);
	startButton_->setMargin(2);

	stopButton_ = new Wt::WPushButton("Stop", this);
	stopButton_->clicked().connect([=] (const WMouseEvent& e) {
		isWorkerStopped_ = true;
	});

	table_ = new Wt::WTable(this);
	table_->setHeaderCount(1);
	table_->setWidth(Wt::WLength("400px"));
	//table_->setMargin(20);

	progress_ = new Wt::WProgressBar(this);
	progress_->setInline(false);
	progress_->setMinimum(0);
	progress_->setMaximum(NUM_IT);
	progress_->setMargin(2);

	for (size_t i = 0; i < symbols.size(); i++) {
		Wt::WStandardItemModel* model = new Wt::WStandardItemModel(numPx, 3, this);
		model->setHeaderData(0, Wt::WString("Date"));
		model->setHeaderData(1, Wt::WString("ASK " + symbols[i]));
		model->setHeaderData(2, Wt::WString("BID " + symbols[i]));

		Wt::Chart::WCartesianChart* chart = new Wt::Chart::WCartesianChart(this);
		chart->setBackground(Wt::WColor(220, 220, 220));
		chart->setModel(model);
		chart->setXSeriesColumn(0);
		chart->setLegendEnabled(true);
		chart->setType(Wt::Chart::ScatterPlot);
		chart->axis(Wt::Chart::XAxis).setScale(Wt::Chart::DateTimeScale);

		chart->setPlotAreaPadding(80, Wt::Left | Wt::Top | Wt::Bottom);
		chart->setPlotAreaPadding(160, Wt::Right);

		for (size_t j = 1; j < 3; ++j) {
			Wt::Chart::WDataSeries s(j, Wt::Chart::LineSeries);
			s.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
			chart->addSeries(s);
		}

		chart->resize(800, 400);
		chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
		// Enable pan and zoom
		//chart->setPanEnabled(true);
		//chart->setZoomEnabled(true);
		chart->axis(Wt::Chart::XAxis).setMaxZoom(16.0);
		chart->axis(Wt::Chart::XAxis).setZoom(15.0);

		//Wt::Chart::WAxisSliderWidget *sliderWidget = new Wt::Chart::WAxisSliderWidget(chart, 2, this);
		//sliderWidget->resize(800, 80);
		//sliderWidget->setPlotAreaPadding(40, Wt::Left | Wt::Right);
		//sliderWidget->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);

		Wt::WTableView *table = new Wt::WTableView(this);
		table->setModel(model);
		table->setSortingEnabled(false);
		table->setColumnResizeEnabled(true);
		table->setAlternatingRowColors(true);
		table->setColumnAlignment(0, Wt::AlignCenter);
		table->setHeaderAlignment(0, Wt::AlignCenter);
		table->setRowHeight(28);
		table->setHeaderHeight(28);
		table->setColumnWidth(0, 140);
		table->resize(783, 200);
		Wt::WItemDelegate *delegate = new Wt::WItemDelegate(table);
		delegate->setTextFormat("%.5f");
		table->setItemDelegate(delegate);
		table->setItemDelegateForColumn(0, new Wt::WItemDelegate(table));

		models_[symbols[i]] = model;
		charts_[symbols[i]]	= chart;
	}

}

MarketDataWidget::~MarketDataWidget() {
	isWorkerStopped_ = true;
	workThread_.join();
	Wt::log("info") << __FUNCTION__ << " ended";
}

void MarketDataWidget::start()
{
	isWorkerStopped_ = false;

	startButton_->setText("Wait...");
	progress_->setValue(0);

	Wt::WApplication *app = Wt::WApplication::instance();

	// Enable server push
	app->enableUpdates(true);

	table_->clear();
	table_->elementAt(0, 0)->addWidget(new Wt::WText("Currency"));
	table_->elementAt(0, 1)->addWidget(new Wt::WText("ASK"));
	table_->elementAt(0, 2)->addWidget(new Wt::WText("BID"));

	workThread_ = boost::thread(boost::bind(&MarketDataWidget::doWork, this, app));
}

void MarketDataWidget::doWork(Wt::WApplication *app)
{

	std::string url = "tcp://localhost:21981";
	ILOGF << " url = " << url << std::endl;
	SF::CORE::ZMQSubscription zmqSub(url);

	std::map<std::string, int> symbols;

	for (int j = 1; j < NUM_IT && !isWorkerStopped_; j++) {
		zmqSub.Receive([&](SF::CORE::CGuardZMQMsg& msg, const int size) {

			if (size == 0) {
				return;
			}

			SF::TRADE::MarketDataProto md;
			md.ParseFromArray(msg.Data(), msg.DataSize());

			ILOG << md.symbol() << " BID[" << md.bid() << "] ASK[" << md.ask() << "]" << std::endl;

			Wt::WApplication::UpdateLock uiLock(app);
			if (uiLock) {
				int row = 0;
				auto it = symbols.find(md.symbol());
				if (it != symbols.end()) {
					row = it->second;
				} else {
					row = symbols.size() + 1;
					symbols[md.symbol()] = row;

					table_->elementAt(row, 0)->addWidget(new Wt::WText(md.symbol()));
					table_->elementAt(row, 1)->addWidget(new Wt::WText(std::to_string(md.ask())));
					table_->elementAt(row, 2)->addWidget(new Wt::WText(std::to_string(md.bid())));
				}

				dynamic_cast<Wt::WText*>(table_->elementAt(row, 0)->widget(0))->setText(md.symbol());
				dynamic_cast<Wt::WText*>(table_->elementAt(row, 1)->widget(0))->setText(std::to_string(md.ask()));
				dynamic_cast<Wt::WText*>(table_->elementAt(row, 2)->widget(0))->setText(std::to_string(md.bid()));

				// Chart
				auto itModel = models_.find(md.symbol());
				if (itModel != models_.end()) {
					WDateTime dt;
					dt.setTime_t(time(NULL));

					Wt::WStandardItemModel* model = itModel->second;
					for (size_t i = model->rowCount() - 1; i > 0; --i) {
						for (size_t j = 0; j < model->columnCount(); j++) {
							model->setData(i, j, model->data(i - 1, j));
						}
					}

					model->setData(0, 0, dt);
					model->setData(0, 1, md.ask());
					model->setData(0, 2, md.bid());
				}

				progress_->setValue(j);

				app->triggerUpdate();
			}

		});
	}

	Wt::WApplication::UpdateLock uiLock(app);
	if (uiLock) {
		startButton_->enable();
		startButton_->setText("Start");

		app->triggerUpdate();

		// Disable server push
		app->enableUpdates(false);
	}
}


} /* namespace WT */
} /* namespace SF */
