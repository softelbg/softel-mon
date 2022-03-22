/*
 * MongoTable.h
 *
 *  Created on: Feb 12, 2016
 *      Author: stan
 */

#ifndef SOFTEL_WT_MONGOTABLE_H_
#define SOFTEL_WT_MONGOTABLE_H_

#include <softel/core/common.h>
#include "common.h"

class MongoTable : public Wt::WTable {
public:
	MongoTable(	const std::string collection,
				const std::string db,
				Wt::WContainerWidget *parent = 0);
	virtual ~MongoTable();

	void init();

	bsoncxx::builder::stream::document& filter() {
		return filter_;
	}

private:
	std::string collection_;
	std::string db_;
	bsoncxx::builder::stream::document filter_;
};

#endif /* SOFTEL_WT_MONGOTABLE_H_ */
