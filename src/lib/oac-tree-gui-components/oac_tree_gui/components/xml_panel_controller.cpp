/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "xml_panel_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/xml_utils.h>

#include <mvvm/signals/model_listener.h>

namespace oac_tree_gui
{

XmlPanelController::XmlPanelController(ProcedureItem *procedure, send_xml_func_t send_xml_func,
                                       send_message_func_t send_message_func)
    : m_procedure(procedure)
    , m_send_xml_func(std::move(send_xml_func))
    , m_send_message_func(std::move(send_message_func))
{
  if (!m_procedure)
  {
    throw RuntimeException("No procedure defined");
  }

  if (!m_send_xml_func)
  {
    throw RuntimeException("Undefined callback to send XML");
  }

  if (!m_send_message_func)
  {
    throw RuntimeException("Undefined callback to report errors");
  }

  SetupListener();
  UpdateXml();
}

XmlPanelController::~XmlPanelController()
{
  qDebug() << "~XmlPanelController::~XmlPanelController()";
}

void XmlPanelController::SetupListener()
{
  m_listener = std::make_unique<mvvm::ModelListener>(m_procedure->GetModel());
  m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto &) { UpdateXml(); });
  m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto &) { UpdateXml(); });
  m_listener->Connect<mvvm::DataChangedEvent>(this, &XmlPanelController::OnDataChangedEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(this,
                                                    &XmlPanelController::OnAboutToRemoveItemEvent);
}

void XmlPanelController::UpdateXml()
{
  if (m_procedure)
  {
    try
    {
      const std::string xml = ExportToXMLString(*m_procedure);
      m_send_xml_func(xml);
    }
    catch (const std::exception &ex)
    {
      // Procedure is in inconsistent state. For example, variable items all have the same names
      // which makes domain's Workspace unhappy.
      m_send_xml_func(std::string());
      SendMessage(ex.what());
    }
  }
}

void XmlPanelController::OnDataChangedEvent(const mvvm::DataChangedEvent &event)
{
  auto [item, role] = event;

  auto tag = item->GetTagIndex().GetTag();

  //   instruction node coordinates are not relevant for the XML of the procedure
  if (tag != itemconstants::kXpos && tag != itemconstants::kYpos)
  {
    UpdateXml();
  }
}

void XmlPanelController::OnAboutToRemoveItemEvent(const mvvm::AboutToRemoveItemEvent &event)
{
  if (event.item->GetItem(event.tag_index) == m_procedure)
  {
    m_send_xml_func(std::string());
    m_procedure = nullptr;
    m_listener.reset();
  }
}

void XmlPanelController::SendMessage(const std::string &what) const
{
  const std::string title("XML generation failed");
  const std::string text("The current procedure is in inconsistent state:");
  const sup::gui::MessageEvent message{title, text, what, ""};
  m_send_message_func(message);
}

}  // namespace oac_tree_gui
