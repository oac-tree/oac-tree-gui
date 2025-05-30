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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_

#include <sup/gui/core/i_message_handler.h>

#include <QWidget>

class QKeyEvent;

namespace sup::gui
{
class IMessageHandler;
class VisibilityAgentBase;
}  // namespace sup::gui

namespace mvvm
{
class ApplicationModel;
}

namespace oac_tree_gui
{

class NodeGraphicsView;
class NodeGraphicsScene;
class ConnectableViewModelController;
class InstructionItem;
class NodeGraphicsViewActions;
class ProcedureItem;
class NodeEditorNavigationToolBar;

/**
 * @brief The NodeEditorWidget class is a main widget with node editor for sequence composition.
 *
 * Contains graphics scene with nodes, graphics view, and all action buttons. It is a central part
 * of SequencerComposerView.
 */
class NodeEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit NodeEditorWidget(QWidget* parent_widget = nullptr);
  ~NodeEditorWidget() override;

  void SetProcedure(ProcedureItem* procedure);

  std::vector<InstructionItem*> GetSelectedInstructions() const;

  void SetSelectedInstructions(const std::vector<InstructionItem*>& instructions) const;

signals:
  void InstructionSelected(oac_tree_gui::InstructionItem* selected);

private:
  void SetupConnections();
  void OnAlignRequest();
  void SetupController();

  NodeGraphicsViewActions* m_view_actions{nullptr};
  NodeGraphicsScene* m_graphics_scene{nullptr};
  NodeGraphicsView* m_graphics_view{nullptr};
  NodeEditorNavigationToolBar* m_navigation_toolbar{nullptr};
  std::unique_ptr<ConnectableViewModelController> m_scene_controller;
  std::unique_ptr<sup::gui::IMessageHandler> m_graphics_view_message_handler;
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};

  ProcedureItem* m_procedure_item{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_NODE_EDITOR_WIDGET_H_
