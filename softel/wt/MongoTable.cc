/*
 * MongoTable.cc
 *
 *  Created on: Feb 12, 2016
 *      Author: stan
 */

#include "MongoTable.h"
#include <softel/core/common.h>

MongoTable::MongoTable(	const std::string collection,
						const std::string db,
						Wt::WContainerWidget *parent)
: Wt::WTable(parent), collection_(collection), db_(db) {
	setHeaderCount(1);
	DLOG << std::endl;
}

MongoTable::~MongoTable() {
	DLOG << std::endl;
}

void MongoTable::init()
{
	DLOG << std::endl;

	mongocxx::instance inst{};
	mongocxx::client conn{mongocxx::uri{}};
	auto db = conn[collection_];
	auto cursor = db[db_].find(filter_.view());

	bool isFirst = true;
	int i = 1;
	for (auto&& doc : cursor) {
		std::ostringstream ss;
		ss << i;

		int j = 0;

		for (auto&& pair : doc) {
			std::string key(pair.key());
			std::string val(bsoncxx::to_json(pair.get_value()));

			//Wt::log("info") << bsoncxx::to_json(pair);
			//Wt::log("info") << key << " = " << val;

			if (isFirst) {
				elementAt(0, j)->addWidget(new Wt::WText(key));
			}

			if (j == 0) {
				elementAt(i, j)->addWidget(new Wt::WText(ss.str()));
			} else {
				elementAt(i, j)->addWidget(new Wt::WText(val));
			}

			j++;
		}

		isFirst = false;

		Wt::log("info") << bsoncxx::to_json(doc);

		i++;
	}

	DLOG << std::endl;
}
