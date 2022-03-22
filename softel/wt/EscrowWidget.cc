/*
 * EscrowApplication.cc
 *
 *  Created on: Feb 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */

#include "EscrowWidget.h"

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include "common.h"

namespace SF {
namespace WT {

using namespace std;
using namespace SF::CORE;
using namespace Wt;

EscrowWidget::EscrowWidget(WContainerWidget *parent)
: WContainerWidget(parent)
{
	init();
}

EscrowWidget::~EscrowWidget() {
	// TODO Auto-generated destructor stub
}

void EscrowWidget::init() {

	WLineEdit* inName = new WLineEdit();
	WLineEdit* inTo = new WLineEdit();
	WLineEdit* inAmount = new WLineEdit();
	WLineEdit* inCurrency = new WLineEdit();

	Wt::WTemplate *t = new Wt::WTemplate(Wt::WString::tr("escrow"));
	addWidget(t);
	
	WPushButton *buttonSave = new WPushButton("Save");
	WPushButton *buttonCancel = new WPushButton("Cancel");

	t->bindWidget("name-edit", inName);
	t->bindWidget("to-edit", inTo);
	t->bindWidget("amount-edit", inAmount);
	t->bindWidget("currency-edit", inCurrency);
	t->bindWidget("save-button", buttonSave);
	t->bindWidget("cancel-button", buttonCancel);

	buttonCancel->clicked().connect([=] (const WMouseEvent& e) {
		Wt::log("info") << "Cancel: inName = " << inName->text();
		
		inName->setText("");
		inTo->setText("");
		inAmount->setText("");
		inCurrency->setText("");
	});
	
	buttonSave->clicked().connect([=] (const WMouseEvent& e) {
		Wt::log("info") << "Save: inName = " << inName->text();

		mongocxx::instance inst{};
		mongocxx::client conn{mongocxx::uri{}};
		auto collection = conn["testdb"]["escrow"];
		bsoncxx::builder::stream::document document{};
		document << "name" << inName->text().toUTF8();
		document << "to" << inTo->text().toUTF8();
		document << "amount" << inAmount->text().toUTF8();
		document << "currency" << inCurrency->text().toUTF8();
		collection.insert_one(document.view());

		bsoncxx::to_json(document);
		
		inName->setText("");
		inTo->setText("");
		inAmount->setText("");
		inCurrency->setText("");
	});

	initTable();
}

void EscrowWidget::initTable() {
	MongoTable* table = new MongoTable("testdb", "escrow", this);
	table->init();
}

} /* namespace WT */
} /* namespace SF */
