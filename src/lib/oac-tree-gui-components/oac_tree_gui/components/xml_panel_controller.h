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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_CONTROLLER_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_CONTROLLER_H_

#include <sup/gui/core/message_event.h>

#include <mvvm/signals/event_types.h>

#include <functional>
#include <memory>

namespace mvvm
{
class ModelListener;
}

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The XmlPanelController class assists to XmlPanel in generation of XML when procedure
 * changes.
 */
class XmlPanelController
{
public:
  using send_xml_func_t = std::function<void(const std::string&)>;
  using send_message_func_t = std::function<void(const sup::gui::MessageEvent& message)>;

  /**
   * @brief XmlPanelController
   *
   * @param procedure Procedure to listen.
   * @param send_xml_func A function to send generated xml.
   * @param send_message_func A function to report errors.
   */
  XmlPanelController(ProcedureItem* procedure, send_xml_func_t send_xml_func,
                     send_message_func_t send_message_func);

  ~XmlPanelController();

private:
  void SetupListener();
  void UpdateXml();
  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);
  /**
   * @brief Notifies the user that XML generation went wrong.
   */
  void SendMessage(const std::string& what) const;

  ProcedureItem* m_procedure{nullptr};
  send_xml_func_t m_send_xml_func;
  send_message_func_t m_send_message_func;
  std::unique_ptr<mvvm::ModelListener> m_listener;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_CONTROLLER_H_
