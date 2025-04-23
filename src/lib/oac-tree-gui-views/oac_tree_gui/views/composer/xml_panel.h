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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_H_

#include <mvvm/signals/event_types.h>

#include <QWidget>

class QAction;

namespace sup::gui
{
class CodeView;
class VisibilityAgentBase;
class IMessageHandler;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;
class XmlPanelController;

/**
 * @brief The XmlPanel class represent a simple read-only text view to show XML content of the
 * procedure.
 */
class XmlPanel : public QWidget
{
  Q_OBJECT

public:
  explicit XmlPanel(QWidget* parent_widget = nullptr);
  ~XmlPanel() override;

  void SetProcedure(ProcedureItem* procedure);

signals:
  void ExportToFileRequest();

private:
  void SetupActions();
  void SetProcedureIntern(ProcedureItem* procedure);

  QAction* m_export_action{nullptr};
  sup::gui::CodeView* m_xml_view{nullptr};
  std::unique_ptr<XmlPanelController> m_panel_controller;
  ProcedureItem* m_procedure{nullptr};
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
  std::unique_ptr<sup::gui::IMessageHandler> m_message_handler;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_XML_PANEL_H_
