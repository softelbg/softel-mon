/*
 * BaseMachine.h
 *
 *  Base Trade Machine (de-facto the base trading server).
 *  Currently holds the users(traders), its owner user and update subscriptions.
 *
 *  Created on: Mar 10, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#ifndef SOFTEL_WT_BASEMACHINE_H_
#define SOFTEL_WT_BASEMACHINE_H_

#include <softel/core/common.h>
#include <softel/core/commonDefs.h>
#include <softel/trade/CommonHelpers.h>
#include <softel/core/util/Options.h>
#include <softel/core/net/zmq/ZMQSubscription.h>
#include <softel/trade/idl/Protobuf.h>

namespace SF {
namespace WT {


typedef std::map<std::string, SF::TRADE::THRIFT::TMSpec> TMachines;
typedef std::map<std::string, std::vector<std::string>> TMachineIdx;
typedef std::vector<std::string> TTypes;

class UpdateSubscriber {

};

struct UpdateSubscription {
    UpdateSubscription(const std::string& id, UpdateSubscriber *c, const std::function<void()>& f)
    : sessionId(id),
      client(c),
      function(f)
    { }

    std::string sessionId;
    UpdateSubscriber *client;
    std::function<void()> function;
    std::map<std::string, std::function<void()>> components_;
};

class BaseMachine {
public:
    BaseMachine();
    explicit BaseMachine(const SF::TRADE::THRIFT::TMSpec& tm);
    virtual ~BaseMachine() noexcept (false);

    void setOwner(const SF::TRADE::THRIFT::User& owner);
    void getOwner(SF::TRADE::THRIFT::User& owner) const;

    void setUsers(const SF::TRADE::THRIFT::TUsers& users);
    void getUsers(SF::TRADE::THRIFT::TUsers& result) const;
    SF::TRADE::THRIFT::User getUser(size_t idx) const;

    void subscribe(const std::string& sessionId, UpdateSubscriber* client, const std::function<void()>& callback);
    void subscribe(const std::string& sessionId,
            UpdateSubscriber* client,
            const std::function<void()>& callback,
            const std::map<std::string, std::function<void()>>& components);
    void unsubscribe(UpdateSubscriber *client);

    void update();
    void update(const std::string& component);

    virtual void start() {}

protected:
    void connectMachine();

    SF::TRADE::THRIFT::TUsers users_; // TODO: make some caching... Currently keeping as NewMachine still use it...

    std::map<UpdateSubscriber*, UpdateSubscription> subscriptions_;
    std::atomic<int> id_;

    SF::TRADE::THRIFT::User owner_;
    SF::TRADE::THRIFT::TMSpec tm_;

    mutable std::shared_ptr<SF::CORE::ZMQSubscription> zmqSub_;
    mutable std::shared_ptr<SF::TRADE::THRIFT::BaseMachineClient> client_;
    mutable SF::CORE::LOCK::Mutex mxThriftClient_;

    mutable SF::CORE::LOCK::RWMutex rw_;
    mutable tbb::task_group tg_;
};


} /* namespace WT */
} /* namespace SF */

#endif /* SOFTEL_WT_BASEMACHINE_H_ */
