/*
 * common.h
 *
 *  Created on: Feb 13, 2016
 *      Author: stan
 */

#ifndef SOFTEL_CORE_COMMON_H_
#define SOFTEL_CORE_COMMON_H_

// TODO: improve...
#define DLOG std::cout
#define ILOG std::cout
#define WLOG std::cout
#define ELOG std::cout
#define DLOGF DLOG << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " "
#define ILOGF ILOG << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " "
#define ELOGF ELOG << __FILE__ << "::" << __FUNCTION__ << "::" << __LINE__ << " "

// Try/Catch
#define TRY(expr)   \
    try {           \
        expr;       \
    } catch (const std::runtime_error& e) { \
        ELOGF << "Exception: " << e.what() << std::endl; \
    }  catch (const std::exception& e) { \
        ELOGF << "Exception: " << e.what() << std::endl; \
    }


// STD
#include <iostream>
#include <exception>
#include <map>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <atomic>
#include <queue>
#include <locale>
#include <functional>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <time.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <assert.h>


// BOOST
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/exception/all.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/utility.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/shared_array.hpp>
#include <boost/atomic/atomic.hpp>


// WT
/*
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WPanel>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WTemplate>
*/

// BSON
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
// Mongo
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>


// ZeroMQ
#include <zmq.h>


// TBB
#include <tbb/tbb.h>
#include <tbb/task_group.h>
#include <tbb/spin_rw_mutex.h>
#include <tbb/concurrent_vector.h>




namespace SF {
namespace CORE {



} /* namespace CORE */
} /* namespace SF */

#endif /* SOFTEL_CORE_COMMON_H_ */
