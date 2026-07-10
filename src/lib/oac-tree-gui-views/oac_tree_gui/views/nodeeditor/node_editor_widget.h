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

#include <QWidget>

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

  std::vector<const InstructionItem *> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<const InstructionItem *> &instructions) const;

signals:
  void selectionChanged();

private:
  void SetupConnections();
  void OnAlignRequest();
  void SetupSceneComponentProvider();
  void AlignInstructions();
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

  //!< true when a deferred initial alignment has already been posted to the event loop
  bool m_alignment_scheduled{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
