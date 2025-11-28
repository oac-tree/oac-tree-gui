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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_TAB_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_TAB_WIDGET_H_

#include <oac_tree_gui/components/component_types.h>

#include <QWidget>

class QTabWidget;
class QToolBar;

namespace sup::gui
{
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;
class InstructionEditorWidget;
class WorkspaceEditorWidget;
class XmlPanel;
class NodeEditorWidget;

class ProcedureComposerTabWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureComposerTabWidget(sup::gui::IAppCommandService& command_service,
                                      QWidget* parent_widget = nullptr);
  ~ProcedureComposerTabWidget() override;

  ProcedureComposerTabWidget(const ProcedureComposerTabWidget&) = delete;
  ProcedureComposerTabWidget& operator=(const ProcedureComposerTabWidget&) = delete;
  ProcedureComposerTabWidget(ProcedureComposerTabWidget&&) = delete;
  ProcedureComposerTabWidget& operator=(ProcedureComposerTabWidget&&) = delete;

  void SetProcedure(ProcedureItem* procedure_item);

  ProcedureEditorType GetEditorType() const;

  void SetEditorType(ProcedureEditorType edit_type);

  /**
   * @brief Insert instruction from toolbox to the instruction container.
   */
  void InsertInstructionFromToolBox(const QString& name);

private:
  void UpdateToolbarButtons();
  void AddTab(QWidget* widget, const QString& label, const QString& tooltip);

  QToolBar* m_tool_bar{nullptr};
  InstructionEditorWidget* m_instruction_editor_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_editor_widget{nullptr};
  NodeEditorWidget* m_node_editor{nullptr};
  XmlPanel* m_xml_panel{nullptr};
  QTabWidget* m_tab_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_COMPOSER_TAB_WIDGET_H_
