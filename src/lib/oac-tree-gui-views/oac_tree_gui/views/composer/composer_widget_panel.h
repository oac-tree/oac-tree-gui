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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_

#include <QWidget>

namespace sup::gui
{
class ItemStackWidget;
}

namespace oac_tree_gui
{

class InstructionEditorWidget;
class InstructionItem;
class ProcedureItem;
class WorkspaceEditorWidget;
class NodeEditorWidget;
class XmlPanel;

/**
 * @brief The ComposerWidgetPanel class is a main vertical panel with a stack of widget for
 * procedure editing.
 *
 * It is located both at the center and on the right of SequencerComposerView (a second main view).
 * Depending on the current selection can show
 * - instruction tree editor
 * - workspace editor
 * - node editor
 * - XML viewer
 */
class ComposerWidgetPanel : public QWidget
{
  Q_OBJECT

public:
  enum WidgetType
  {
    kInstructionTree,
    kWorkspace,
    kNodeEditor,
    kXmlPanel
  };

  /**
   * @brief Main c-tor.
   *
   * @param settings_group_name Unique name of the panel for persistent settings.
   * @param widget_type The type of the panel to show at first start.
   * @param parent_widget The parent widget.
   */
  explicit ComposerWidgetPanel(const QString& settings_group_name, WidgetType widget_type,
                               QWidget* parent_widget = nullptr);
  ~ComposerWidgetPanel() override;

  void SetProcedure(ProcedureItem* procedure);

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions);

  std::vector<InstructionItem*> GetSelectedInstructions() const;
  InstructionItem* GetSelectedInstruction() const;

  /**
   * @brief Read settings from persistent storage.
   */
  void ReadSettings();

  /**
   * @brief Write settings to persistent storage.
   */
  void WriteSettings();

signals:
  void InstructionSelected(oac_tree_gui::InstructionItem* selected);
  void ExportToFileRequest();

private:
  void SetupConnections();

  InstructionEditorWidget* m_instruction_editor_widget{nullptr};
  WorkspaceEditorWidget* m_workspace_editor_widget{nullptr};
  NodeEditorWidget* m_node_editor{nullptr};
  XmlPanel* m_xml_panel{nullptr};
  sup::gui::ItemStackWidget* m_stack_widget{nullptr};

  ProcedureItem* m_procedure{nullptr};
  bool m_block_selection_notification{false};  //!< do not notify outside if selection changed
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_COMPOSER_WIDGET_PANEL_H_
