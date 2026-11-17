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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_

#include <oac_tree_gui/model/component_types.h>

#include <mvvm/model/item_selection.h>

#include <QWidget>
#include <memory>

class QMenu;

namespace sup::gui
{
class IMessageHandler;
class VisibilityAgentBase;
}  // namespace sup::gui

namespace oac_tree_gui
{

class NodeGraphicsView;
class NodeGraphicsScene;
class InstructionItem;
class NodeGraphicsViewActions;
class ProcedureItem;
class NodeEditorNavigationToolBar;
class NodeEditorSceneController;

/**
 * @brief The NodeEditorWidget class is a main widget with node editor for sequence composition.
 *
 * Contains graphics scene with nodes, graphics view, and all action buttons. It is a central
 * part of SequencerComposerView.
 */
class NodeEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit NodeEditorWidget(NodeEditorMode editor_mode, QWidget* parent_widget = nullptr);
  ~NodeEditorWidget() override;

  NodeEditorWidget(const NodeEditorWidget&) = delete;
  NodeEditorWidget& operator=(const NodeEditorWidget&) = delete;
  NodeEditorWidget(NodeEditorWidget&&) = delete;
  NodeEditorWidget& operator=(NodeEditorWidget&&) = delete;

  void SetProcedure(ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<const InstructionItem*>& instructions) const;

  /**
   * @brief Shows the SubTree of the given top-level instruction. Editor mode only.
   */
  void SetScope(const InstructionItem* top_level);

  /**
   * @brief Returns the top-level instruction of the current SubTree, or nullptr.
   */
  const InstructionItem* GetScope() const;

signals:
  /**
   * @brief Notifies that the instruction selection on the graphics scene has changed.
   */
  void instructionSelectionChanged(const mvvm::ItemSelection& selection);

private:
  void SetupConnections();

  //! Fills the SubTree selector menu.
  void FillSubTreeMenu(QMenu* menu);

  //! Updates the text of the SubTree selector.
  void UpdateSubTreeText();

  NodeEditorMode m_editor_mode;
  NodeGraphicsViewActions* m_view_actions{nullptr};
  std::unique_ptr<NodeGraphicsScene> m_graphics_scene;
  NodeGraphicsView* m_graphics_view{nullptr};
  NodeEditorNavigationToolBar* m_navigation_toolbar{nullptr};
  std::unique_ptr<sup::gui::IMessageHandler> m_graphics_view_message_handler;
  std::unique_ptr<NodeEditorSceneController> m_scene_controller;
  std::unique_ptr<sup::gui::VisibilityAgentBase> m_visibility_agent;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
