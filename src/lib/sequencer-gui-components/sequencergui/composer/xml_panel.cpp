
#include "xml_panel.h"

#include <sequencergui/components/visibility_agent_base.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/xml_utils.h>
#include <sup/gui/codeeditor/code_view.h>

#include <mvvm/model/application_model.h>
#include <mvvm/signals/model_listener.h>

#include <sup/dto/anyvalue.h>

#include <QVBoxLayout>
#include <iostream>

namespace sequencergui
{

XmlPanel::XmlPanel(QWidget *parent)
    : QWidget(parent), m_xml_view(new sup::gui::CodeView(sup::gui::CodeView::kXML))
{
  setWindowTitle("XML View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_xml_view);

  auto on_subscribe = [this]()
  {
//    m_listener = std::make_unique<listener_t>(m_model);
//    m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto &) { UpdateXml(); });
//    m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto &) { UpdateXml(); });
//    m_listener->Connect<mvvm::DataChangedEvent>(this, &XmlPanel::OnModelEvent);

//    UpdateXml();
  };

  auto on_unsubscribe = [this]() { m_listener.reset(); };

  // will be deleted as a child of QObject
  m_visibility_agent = new VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

void XmlPanel::SetModel(mvvm::SessionModelInterface *model)
{
  m_model = model;
}

void XmlPanel::SetProcedure(ProcedureItem *procedure)
{
  m_procedure = procedure;
  UpdateXml();
}

void XmlPanel::OnModelEvent(const mvvm::DataChangedEvent &event)
{
  auto [item, role] = event;

  auto [tag, index] = item->GetTagIndex();

  //   instruction node coordinates are not relevant for the XML of the procedure
  if (tag != itemconstants::kXpos && tag != itemconstants::kYpos)
  {
    UpdateXml();
  }
}

XmlPanel::~XmlPanel() = default;

void XmlPanel::UpdateXml()
{
  if (m_procedure && isVisible())
  {
    try
    {
      m_xml_view->SetContent(QString::fromStdString(ExportToXMLString(m_procedure)));
    }
    catch (const std::exception &ex)
    {
      // Procedure is in inconsistent state. For example, variable items all have the same names
      // which makes domain's Workspace unhappy.
      m_xml_view->ClearText();
    }
  }
  else
  {
    m_xml_view->ClearText();
  }
}

}  // namespace sequencergui
