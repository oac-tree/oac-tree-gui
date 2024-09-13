/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "xml_panel.h"

#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/xml_utils.h>
#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/style_utils.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/signals/model_listener.h>

#include <QAction>
#include <QVBoxLayout>

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

  SetupActions();

  SetupListener();
}

XmlPanel::~XmlPanel() = default;

void XmlPanel::SetModel(mvvm::ISessionModel *model)
{
  m_model = model;
}

void XmlPanel::SetProcedure(ProcedureItem *procedure)
{
  m_procedure = procedure;
  UpdateXml();
}

void XmlPanel::SetupActions()
{
  m_export_action = new QAction(this);
  m_export_action->setText("Save procedure to XML");
  m_export_action->setIcon(sup::gui::utils::GetIcon("export.svg"));
  m_export_action->setToolTip("Save procedure to Sequencer XML file");
  connect(m_export_action, &QAction::triggered, this, &XmlPanel::ExportToFileRequest);
  addAction(m_export_action);
}

void XmlPanel::SetupListener()
{
  auto on_subscribe = [this]()
  {
    m_listener = std::make_unique<mvvm::ModelListener>(m_model);
    m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto &) { UpdateXml(); });
    m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto &) { UpdateXml(); });
    m_listener->Connect<mvvm::DataChangedEvent>(this, &XmlPanel::OnDataChangedEvent);

    UpdateXml();
  };

  auto on_unsubscribe = [this]() { m_listener.reset(); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

void XmlPanel::OnDataChangedEvent(const mvvm::DataChangedEvent &event)
{
  auto [item, role] = event;

  auto [tag, index] = item->GetTagIndex();

  //   instruction node coordinates are not relevant for the XML of the procedure
  if (tag != itemconstants::kXpos && tag != itemconstants::kYpos)
  {
    UpdateXml();
  }
}

void XmlPanel::UpdateXml()
{
  if (m_procedure && isVisible())
  {
    try
    {
      m_xml_view->SetContent(QString::fromStdString(ExportToXMLString(*m_procedure)));
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
