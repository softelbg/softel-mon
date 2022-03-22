#ifndef U_INFO_WIDGET_H
#define U_INFO_WIDGET_H

#include <fstream>
#include <memory>

#include <Wt/WApplication>
#include <Wt/WTemplate>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WTable>
#include <Wt/WString>

#include "UinfoStorage.h"

namespace SF {
namespace WT {

class UinfoWidget : public Wt::WContainerWidget
{
public:
    explicit UinfoWidget(Wt::WContainerWidget *parent = 0);
    virtual ~UinfoWidget();
private:
    void draw();
    void update(const bsoncxx::builder::stream::document& doc);
private:
    std::unique_ptr<Wt::WTable> m_table;
    std::unique_ptr<UinfoStorage> m_store;
    std::string m_curruser;
};
}}
#endif
