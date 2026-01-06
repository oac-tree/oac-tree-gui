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

#include "xml_panel_controller.h"

#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/xml_utils.h>

namespace oac_tree_gui
{

XmlPanelController::XmlPanelController(ProcedureItem* procedure, send_text_func_t send_xml_func,
                                       send_message_func_t send_message_func)
    : sup::gui::AbstractTextContentController(procedure, std::move(send_xml_func),
                                              std::move(send_message_func))
    , m_procedure(procedure)
{
  UpdateText();
}

XmlPanelController::~XmlPanelController() = default;

std::string XmlPanelController::GenerateText()
{
  if (m_procedure != nullptr)
  {
    auto result = ExportToXMLString(*m_procedure);
    return result;
  }

  return {};
}

void XmlPanelController::OnDataChangedEvent(const mvvm::DataChangedEvent& event)
{
  auto [item, role] = event;
  auto tag = item->GetTagIndex().GetTag();

  //   instruction node coordinates are not relevant for the XML of the procedure
  if (tag == itemconstants::kXpos || tag == itemconstants::kYpos)
  {
    return;
  }

  sup::gui::AbstractTextContentController::OnDataChangedEvent(event);
}

}  // namespace oac_tree_gui
