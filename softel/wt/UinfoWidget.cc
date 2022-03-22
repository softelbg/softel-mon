#include "UinfoWidget.h"

#include <thread>
#include <ctime>
#include <chrono>

#include <softel/core/commonDefs.h>
#include <softel/core/common.h>
#include <softel/wt/common.h>


namespace SF {
namespace WT {

using namespace Wt;

UinfoWidget::UinfoWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    DLOGF << "Initialize Uinfo " << std::endl;
    WApplication::instance()->enableUpdates(true);
    
    setContentAlignment(AlignCenter);
    
    m_store.reset(new UinfoStorage("history", "users"));

    new WText("<h2>User transactions history</h2>", this);
    
    m_table.reset(new WTable(this));
    m_table->setStyleClass("tableview");
    draw();
            
}

void UinfoWidget::draw()
{
    new WText("User", m_table->elementAt(0, 0));
    new WText("Type", m_table->elementAt(0, 1));
    new WText("Item", m_table->elementAt(0, 2));
    new WText("Amount", m_table->elementAt(0, 3));
    new WText("Price", m_table->elementAt(0, 4));
    new WText("Balance", m_table->elementAt(0, 5));
    new WText("Date", m_table->elementAt(0, 6));
    m_table->setHeaderCount(1);

    
    uint32_t row = m_table->rowCount();

    bsoncxx::builder::stream::document doc{};
    
    auto out = m_store->fetch(doc);
    
    for (const auto& elem : out)
    {
        new WText(bsoncxx::to_json(elem["user"].get_value()),m_table->elementAt(row, 0));
        new WText(bsoncxx::to_json(elem["type"].get_value()),m_table->elementAt(row, 1));
        new WText(bsoncxx::to_json(elem["item"].get_value()),m_table->elementAt(row, 2));
        new WText(bsoncxx::to_json(elem["amount"].get_value()),m_table->elementAt(row, 3));
        new WText(bsoncxx::to_json(elem["price"].get_value()),m_table->elementAt(row, 4));
        new WText(bsoncxx::to_json(elem["balance"].get_value()),m_table->elementAt(row, 5));
        new WText(bsoncxx::to_json(elem["date"].get_value()), m_table->elementAt(row, 6));
        ++row;
    }
    
    m_store->Register<DBObservers::InsertEvent>([this](const bsoncxx::builder::stream::document& doc)->void {
            update(doc);
    });
}

void UinfoWidget::update(const bsoncxx::builder::stream::document& doc)
{
    uint32_t row = m_table->rowCount();
    
    new WText(bsoncxx::to_json(doc.view()["user"].get_value()),m_table->elementAt(row, 0));
    new WText(bsoncxx::to_json(doc.view()["type"].get_value()),m_table->elementAt(row, 1));
    new WText(bsoncxx::to_json(doc.view()["item"].get_value()),m_table->elementAt(row, 2));
    new WText(bsoncxx::to_json(doc.view()["amount"].get_value()),m_table->elementAt(row, 3));
    new WText(bsoncxx::to_json(doc.view()["price"].get_value()),m_table->elementAt(row, 4));
    new WText(bsoncxx::to_json(doc.view()["balance"].get_value()),m_table->elementAt(row, 5));
    new WText(bsoncxx::to_json(doc.view()["date"].get_value()), m_table->elementAt(row, 6));

}
UinfoWidget::~UinfoWidget()
{
    Wt::WApplication::instance()->enableUpdates(false);
    
    DLOGF << "Destroy Uinfo " << std::endl;
}
}}
