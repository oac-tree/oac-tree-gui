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

#include "node_editor_widget.h"

#include "node_editor_navigation_toolbar.h"
#include "node_graphics_view.h"
#include "node_graphics_view_actions.h"

#include <oac_tree_gui/mainwindow/main_window_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/nodeeditor/objects/graphics_scene_component_provider.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>

#include <QPointF>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace
{
QList<QAction*> GetToolBarActions(oac_tree_gui::NodeGraphicsViewActions* actions)
{
  using ActionKey = oac_tree_gui::NodeGraphicsViewActions::ActionKey;
  return actions->GetActions({ActionKey::kPointer, ActionKey::kPan, ActionKey::kAlign});
}

constexpr std::chrono::milliseconds kMessageDurationTime{5000};

}  // namespace

namespace oac_tree_gui
{

NodeEditorWidget::NodeEditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_view_actions(new NodeGraphicsViewActions(this))
    , m_graphics_scene(CreateGraphicsScene())
    , m_graphics_view(new NodeGraphicsView(m_graphics_scene.get(), this))
    , m_navigation_toolbar(new NodeEditorNavigationToolBar)
    , m_graphics_view_message_handler(
          sup::gui::CreateWidgetOverlayMessageHandler(m_graphics_view, kMessageDurationTime))
{
  setWindowTitle("NodeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_graphics_view);
  layout->addWidget(m_navigation_toolbar);

  SetupConnections();

  auto on_subscribe = [this]() { SetupController(); };

  auto on_unsubscribe = [this]() { m_scene_component_provider.reset(); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);

  addActions(GetToolBarActions(m_view_actions));
}

NodeEditorWidget::~NodeEditorWidget() = default;

void NodeEditorWidget::SetProcedure(ProcedureItem* procedure)
{
  if (m_procedure_item == procedure)
  {
    return;
  }

  m_procedure_item = procedure;

  if (!m_procedure_item)
  {
    return;
  }

  SetupController();
}

std::vector<InstructionItem*> NodeEditorWidget::GetSelectedInstructions() const
{
  return m_scene_component_provider->GetSelectedInstructions();
}

void NodeEditorWidget::SetSelectedInstructions(
    const std::vector<InstructionItem*>& instructions) const
{
  if (isHidden())
  {
    return;
  }

  if (m_scene_component_provider)
  {
    m_scene_component_provider->SetSelectedInstructions(instructions);
  }
}

//! Provides node alignment on graphics view.

void NodeEditorWidget::OnAlignRequest()
{
  auto selected = mvvm::GetSelectedShapes<mvvm::ConnectableShape>(*m_graphics_scene);
  if (selected.size() != 1)
  {
    return;
  }

  auto view = selected.front();
  auto item = mvvm::GetUnderlyingItem<InstructionItem>(view);
  algorithm::AlignInstructionTreeWalker(view->pos(), item);
}

void NodeEditorWidget::SetupController()
{
  if (!m_procedure_item || !isVisible())
  {
    return;
  }

  auto container = m_procedure_item->GetInstructionContainer();

  m_scene_component_provider.reset();

  if (algorithm::RequiresInitialAlignment(container->GetInstructions()))
  {
    const QPointF reference_point = m_graphics_scene->sceneRect().center();
    algorithm::AlignInstructionTreeWalker(reference_point, container->GetInstructions());
  }

  m_scene_component_provider = CreateGraphicsSceneComponentProvider();
}

std::unique_ptr<NodeGraphicsScene> NodeEditorWidget::CreateGraphicsScene()
{
  return std::make_unique<NodeGraphicsScene>();
}

std::unique_ptr<GraphicsSceneComponentProvider>
NodeEditorWidget::CreateGraphicsSceneComponentProvider()
{
  auto message_callback = [this](const auto& message)
  { m_graphics_view_message_handler->SendMessage(message); };

  auto result = std::make_unique<GraphicsSceneComponentProvider>(
      message_callback, CreatePluginNameCallback(), m_graphics_scene.get(),
      m_procedure_item->GetInstructionContainer());

  // forward instruction selection from graphics scene
  connect(result.get(), &GraphicsSceneComponentProvider::selectionChanged, this,
          &NodeEditorWidget::selectionChanged);

  // change GraphicsView operation mode on connection start/finish
  connect(result.get(), &GraphicsSceneComponentProvider::connectionStarted, this, [this]()
          { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kSimpleSelection); });
  connect(result.get(), &GraphicsSceneComponentProvider::connectionFinished, this, [this]()
          { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kRubberSelection); });

  // propagates delete request from the graphics view
  connect(m_graphics_view, &NodeGraphicsView::deleteSelectedRequest, result.get(),
          &GraphicsSceneComponentProvider::OnDeleteSelected);

  // propagate drop request from GraphicsScene to GraphicsSceneComponentProvider
  connect(m_graphics_scene.get(), &NodeGraphicsScene::dropInstructionRequested, result.get(),
          [this](const QString& name, const QPointF& pos)
          { m_scene_component_provider->DropInstruction(name.toStdString(), {pos.x(), pos.y()}); });

  // propagate branch selection request from GraphicsScene to GraphicsSceneComponentProvider
  connect(m_graphics_scene.get(), &NodeGraphicsScene::instructionDoubleClick, result.get(),
          &GraphicsSceneComponentProvider::SelectInstructionBranch);

  // propagate double click on ports from GraphicsScene to GraphicsSceneComponentProvider
  connect(m_graphics_scene.get(), &NodeGraphicsScene::portDoubleClick, result.get(),
          &GraphicsSceneComponentProvider::DoubleClickPort);

  return result;
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
  connect(m_view_actions, &NodeGraphicsViewActions::alignSelectedRequest, this,
          &NodeEditorWidget::OnAlignRequest);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &NodeGraphicsView::OperationModeChanged, m_view_actions,
          &NodeGraphicsViewActions::UpdateButtonsToOperationMode);
}

}  // namespace oac_tree_gui
