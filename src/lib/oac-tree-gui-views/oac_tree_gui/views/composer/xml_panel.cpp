/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "xml_panel.h"

#include <oac_tree_gui/components/xml_panel_controller.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/xml_utils.h>
#include <oac_tree_gui/style/style_helper.h>
#include <oac_tree_gui/views/operation/procedure_action_handler.h>

#include <sup/gui/views/codeeditor/code_view.h>
#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/session_item.h>

#include <QAction>
#include <QVBoxLayout>

namespace oac_tree_gui
{

XmlPanel::XmlPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_xml_view(new sup::gui::CodeView(sup::gui::CodeView::kXML))
    , m_message_handler(sup::gui::CreateWidgetOverlayMessageHandler(m_xml_view))
{
  setWindowTitle("XML View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_xml_view);

  SetupActions();

  auto on_subscribe = [this]() { SetProcedureIntern(m_procedure); };
  auto on_unsubscribe = [this]() { SetProcedureIntern(nullptr); };
  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);
}

XmlPanel::~XmlPanel() = default;

void XmlPanel::SetProcedure(ProcedureItem* procedure)
{
  if (m_procedure == procedure)
  {
    return;
  }

  m_procedure = procedure;

  if ((m_procedure != nullptr) && isVisible())
  {
    SetProcedureIntern(m_procedure);
  }
}

void XmlPanel::SetupActions()
{
  m_export_action = new QAction(this);
  m_export_action->setText("to XML");
  m_export_action->setIcon(FindIcon("export"));
  m_export_action->setToolTip("Save procedure to oac-tree XML file");
  connect(m_export_action, &QAction::triggered, this, &XmlPanel::OnExportToFileRequest);
  addAction(m_export_action);
}

void XmlPanel::SetProcedureIntern(ProcedureItem* procedure)
{
  if (procedure != nullptr)
  {
    auto on_xml_update = [this](const auto& xml)
    {
      m_message_handler->ClearMessages();
      m_xml_view->SetContent(QString::fromStdString(xml));
    };

    auto on_message = [this](const auto& message)
    {
      m_xml_view->ClearText();
      m_message_handler->SendMessage(message);
    };

    m_panel_controller = std::make_unique<XmlPanelController>(procedure, on_xml_update, on_message);
  }
  else
  {
    m_panel_controller.reset();
    m_xml_view->ClearText();
  }
}

void XmlPanel::OnExportToFileRequest()
{
  ProcedureActionHandler handler;
  handler.OnExportToXmlRequest(m_procedure);
}

}  // namespace oac_tree_gui
