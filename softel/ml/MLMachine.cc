/*
 * MLMachine.cc
 *
 *  Created on: Mar 7, 2016
 *      Author: stan
 *
 *      www.softel.bg
 *
 *      Copyright (c) 2006 - 2016 Softel ltd Haskovo. All rights reserved.
 *
 */
#include "MLMachine.h"

using namespace std;
using namespace SF::CORE::LOCK;
using namespace SF::TRADE;
using namespace SF::TRADE::THRIFT;


namespace SF {
namespace ML {

std::shared_ptr<MLMachine> Machines::m;

MLMachine::MLMachine()
: BaseMachine()
{
    init();
}

MLMachine::~MLMachine() {
    tg_.cancel();
    tg_.wait();
}

void MLMachine::process(const std::string& name, std::string& last_line)
{
    std::ifstream infile("/home/sgeorgiev/ml/char-rnn/generated_" + name + ".txt");
    {
        WriteSynchronized lock(rw_msg_);

        std::string line;
        int i = 0;
        while (std::getline(infile, line)) {
            if (i == 5) {
                if (last_line == line) {
                    DLOG << "Same last_line = " << last_line << std::endl << " line = " << line << std::endl;
                    break;
                }

                last_line = line;
                line = line.substr(40);
            }
            if (i++ > 4) {
                msg_[name].push_back(line);

                if (msg_[name].size() > 1000) {
                    msg_[name].pop_front();
                }
            }
        }
    }
}

void MLMachine::init() {

    names_ = {"DanBrown", "TerryPratchett"}; // TODO make proper names query...

    tbb::task_arena limited(4, 1);
    limited.execute([&] {
        tg_.run([&]() {
            std::map<std::string, std::string> last_line;
            while(!tbb::task::self().is_cancelled()) {
                
                for (auto & name : names_) {
                    process(name, last_line[name]);
                }
                
                update();
               
                sleep(100);
            }
        });
    });
}


} /* namespace WT */
} /* namespace SF */
