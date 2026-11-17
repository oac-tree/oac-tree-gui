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
#include <map>
#include <set>
#include <string>

class QMenu;

namespace mvvm
{
class ModelListener;
}  // namespace mvvm

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
class GraphicsSceneComponentProvider;

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

  void SetSelectedInstructions(const std::vector<const InstructionItem *> &instructions) const;

  /**
   * @brief Shows the SubTree of the given top-level instruction.
   *
   * Loose instructions of the current SubTree are recorded and shown again on return. Editor mode
   * only.
   *
   * @param top_level The top-level instruction, or nullptr to show no SubTree.
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
  void OnAlignRequest();
  void SetupSceneComponentProvider();

  //! Sets the scope and its hidden set, and rebuilds the provider.
  void ApplyScope(const InstructionItem* top_level);

  //! Returns the top-level instructions not in the hidden set.
  std::vector<InstructionItem*> GetVisibleTopLevelInstructions() const;

  //! Aligns the visible instructions if required. Fits the view after a scope switch.
  void UpdateView();

  //! Aligns the visible instructions if they all are at the origin.
  void AlignInstructions();

  //! Falls back to the top-level ancestor, or to the root, if the scope is no longer top-level.
  void ValidateScope();

  //! Fills the SubTree selector menu.
  void FillSubTreeMenu(QMenu* menu);

  //! Updates the text of the SubTree selector.
  void UpdateSubTreeText();

  static std::unique_ptr<NodeGraphicsScene> CreateGraphicsScene();
  std::unique_ptr<GraphicsSceneComponentProvider> CreateGraphicsSceneComponentProvider(
      NodeEditorMode editor_mode);

  NodeEditorMode m_editor_mode;
  NodeGraphicsViewActions* m_view_actions{nullptr};
  std::unique_ptr<NodeGraphicsScene> m_graphics_scene;
  std::unique_ptr<GraphicsSceneComponentProvider> m_scene_component_provider;
  NodeGraphicsView* m_graphics_view{nullptr};
  NodeEditorNavigationToolBar* m_navigation_toolbar{nullptr};
  std::unique_ptr<sup::gui::IMessageHandler> m_graphics_view_message_handler;
  std::unique_ptr<sup::gui::VisibilityAgentBase> m_visibility_agent;

  ProcedureItem* m_procedure_item{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;

  //!< identifier of the top-level instruction of the current SubTree, empty if none
  std::string m_scope_identifier;

  //!< identifiers of top-level instructions not shown
  std::set<std::string> m_hidden_identifiers;

  //!< loose top-level instructions of each SubTree, recorded on leaving it
  std::map<std::string, std::set<std::string>> m_loose_identifiers;

  //!< true when a deferred view update has already been posted to the event loop
  bool m_view_update_scheduled{false};

  //!< true when the view should be fitted to the scope on the next view update
  bool m_fit_required{false};

  //!< true when a deferred scope validation has already been posted to the event loop
  bool m_scope_validation_scheduled{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
