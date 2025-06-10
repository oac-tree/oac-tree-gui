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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_XML_PANEL_CONTROLLER_H_
#define OAC_TREE_GUI_COMPONENTS_XML_PANEL_CONTROLLER_H_

#include <sup/gui/components/abstract_text_content_controller.h>

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The XmlPanelController class assists to XmlPanel in generation of XML when procedure
 * changes.
 */
class XmlPanelController : public sup::gui::AbstractTextContentController
{
public:
  /**
   * @brief XmlPanelController
   *
   * @param procedure Procedure to listen.
   * @param send_xml_func A function to send generated xml.
   * @param send_message_func A function to report errors.
   */
  XmlPanelController(ProcedureItem* procedure, send_text_func_t send_xml_func,
                     send_message_func_t send_message_func);

  ~XmlPanelController() override;
  XmlPanelController(const XmlPanelController&) = delete;
  XmlPanelController& operator=(const XmlPanelController&) = delete;
  XmlPanelController(XmlPanelController&&) = delete;
  XmlPanelController& operator=(XmlPanelController&&) = delete;

private:
  std::string GenerateText() override;
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event) override;

  ProcedureItem* m_procedure{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_XML_PANEL_CONTROLLER_H_
