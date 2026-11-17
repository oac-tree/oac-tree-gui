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

#include "node_editor_widget.h"

#include "node_editor_navigation_toolbar.h"
#include "node_graphics_view.h"
#include "node_graphics_view_actions.h"
#include "sub_tree_navigation_actions.h"

#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>

#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

constexpr std::chrono::milliseconds kMessageDurationTime{5000};

//! Returns toolbar actions of the node graphics view.
QList<QAction*> GetToolBarActions(NodeGraphicsViewActions* actions)
{
  using ActionKey = NodeGraphicsViewActions::ActionKey;
  return actions->GetActions({ActionKey::kPointer, ActionKey::kPan, ActionKey::kAlign});
}

}  // namespace

NodeEditorWidget::NodeEditorWidget(NodeEditorMode editor_mode, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_editor_mode(editor_mode)
    , m_view_actions(new NodeGraphicsViewActions(this))
    , m_graphics_scene(std::make_unique<NodeGraphicsScene>())
    , m_graphics_view(new NodeGraphicsView(m_graphics_scene.get(), this))
    , m_navigation_toolbar(new NodeEditorNavigationToolBar)
    , m_graphics_view_message_handler(
          sup::gui::CreateWidgetOverlayMessageHandler(m_graphics_view, kMessageDurationTime))
    , m_scene_controller(std::make_unique<NodeEditorSceneController>(
          editor_mode, m_graphics_scene.get(),
          [this](const auto& message) { m_graphics_view_message_handler->SendMessage(message); }))
{
  setWindowTitle("NodeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_graphics_view);
  layout->addWidget(m_navigation_toolbar);

  SetupConnections();

  auto on_subscribe = [this]() { m_scene_controller->SetActive(true); };
  auto on_unsubscribe = [this]() { m_scene_controller->SetActive(false); };
  m_visibility_agent =
      std::make_unique<sup::gui::VisibilityAgentBase>(on_subscribe, on_unsubscribe);
  m_visibility_agent->SetTarget(this);

  addActions(GetToolBarActions(m_view_actions));
  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    m_navigation_actions = new SubTreeNavigationActions(m_scene_controller.get(), this);
    addActions(m_navigation_actions->GetActions());
  }
}

NodeEditorWidget::~NodeEditorWidget()
{
  // we should reset visibility agent since on destruction widget becomes invisible (in
  // uncontrollable moment of time) and agent might try to call invalid callbacks.
  m_visibility_agent.reset();
}

void NodeEditorWidget::SetProcedure(ProcedureItem* procedure)
{
  m_scene_controller->SetProcedure(procedure);
}

std::vector<InstructionItem*> NodeEditorWidget::GetSelectedInstructions() const
{
  return m_scene_controller->GetSelectedInstructions();
}

void NodeEditorWidget::SetSelectedInstructions(
    const std::vector<const InstructionItem*>& instructions) const
{
  if (isHidden())
  {
    return;
  }

  m_scene_controller->SetSelectedInstructions(instructions);
}

void NodeEditorWidget::SetScope(const InstructionItem* top_level)
{
  m_scene_controller->SetScope(top_level);
}

const InstructionItem* NodeEditorWidget::GetScope() const
{
  return m_scene_controller->GetScope();
}

void NodeEditorWidget::SetupConnections()
{
  // Propagate selection mode change from toolbar to GraphicsView
  connect(m_view_actions, &NodeGraphicsViewActions::OperationModeChangeRequest, m_graphics_view,
          &NodeGraphicsView::SetOperationMode);

  // Propagate zoom results from GraphicsView to bottom toolbar
  connect(m_graphics_view, &NodeGraphicsView::ZoomFactorChanged, m_navigation_toolbar,
          &NodeEditorNavigationToolBar::SetZoomFactor);

  // Propagate zoom request from a bottom navigation panel to GraphicsView
  connect(m_navigation_toolbar, &NodeEditorNavigationToolBar::ZoomFactorRequest, m_graphics_view,
          &NodeGraphicsView::SetZoomFactor);

  // alignment request from a bottom toolbar
  connect(m_navigation_toolbar, &NodeEditorNavigationToolBar::CenterViewRequest, m_graphics_view,
          &NodeGraphicsView::CenterView);
  connect(m_navigation_toolbar, &NodeEditorNavigationToolBar::FitToViewRequest, m_graphics_view,
          &NodeGraphicsView::FitView);

  // alignment request from a toolbar
  connect(m_view_actions, &NodeGraphicsViewActions::alignSelectedRequest, m_scene_controller.get(),
          &NodeEditorSceneController::AlignSelected);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &NodeGraphicsView::OperationModeChanged, m_view_actions,
          &NodeGraphicsViewActions::UpdateButtonsToOperationMode);

  // forward instruction selection from graphics scene
  connect(m_scene_controller.get(), &NodeEditorSceneController::selectionChanged, this, [this]()
          { emit instructionSelectionChanged(mvvm::ItemSelection(GetSelectedInstructions())); });

  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    // change GraphicsView operation mode on connection start/finish
    connect(m_scene_controller.get(), &NodeEditorSceneController::connectionStarted, this, [this]()
            { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kSimpleSelection); });
    connect(m_scene_controller.get(), &NodeEditorSceneController::connectionFinished, this, [this]()
            { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kRubberSelection); });

    // propagates delete request from the graphics view
    connect(m_graphics_view, &NodeGraphicsView::deleteSelectedRequest, m_scene_controller.get(),
            &NodeEditorSceneController::OnDeleteSelected);

    // fit the view to a new scope
    connect(m_scene_controller.get(), &NodeEditorSceneController::scopeShown, m_graphics_view,
            &NodeGraphicsView::FocusOnItems);
  }
}

}  // namespace oac_tree_gui
