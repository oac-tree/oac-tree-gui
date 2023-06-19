/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_editor.h"

#include "sequencergui/nodeeditor/node_editor_toolbar.h"

#include <sequencergui/components/message_handler_factory.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/graphics_scene.h>
#include <sequencergui/nodeeditor/graphics_scene_controller.h>
#include <sequencergui/nodeeditor/graphics_view.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/model/application_model.h>

#include <QDebug>
#include <QPointF>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace sequencergui
{

NodeEditor::NodeEditor(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new NodeEditorToolBar)
    , m_tool_bar_action(new QWidgetAction(this))
    , m_graphics_scene(new GraphicsScene(this))
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
    , m_graphics_view_message_handler(CreateWidgetOverlayMessageHandler(m_graphics_view))
{
  setWindowTitle("NodeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_graphics_view);

  m_graphics_scene->SetMessageHandler(CreateMessageHandler());

  SetupConnections();
  SetupToolBar();
}

NodeEditor::~NodeEditor() = default;

void NodeEditor::SetProcedure(ProcedureItem *procedure)
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

  auto instruction_container = procedure->GetInstructionContainer();
  m_graphics_scene->SetInstructionContainer(instruction_container);
  m_scene_controller =
      std::make_unique<GraphicsSceneController>(procedure->GetModel(), m_graphics_scene);

  m_scene_controller->Init(instruction_container);

  if (algorithm::RequiresInitialAlignment(instruction_container->GetInstructions()))
  {
    const QPointF reference_point = m_graphics_scene->sceneRect().center();
    algorithm::AlignInstructionTreeWalker(reference_point,
                                          instruction_container->GetInstructions());
  }

  m_graphics_view->onCenterView();
}

std::vector<InstructionItem *> NodeEditor::GetSelectedInstructions() const
{
  return m_graphics_scene->GetSelectedInstructions();
}

void NodeEditor::SetSelectedInstructions(const std::vector<InstructionItem *> &instructions) const
{
  m_graphics_scene->SetSelectedInstructions(instructions);
}

//! Creates message handler that can be used to publish messages at the lower right corner of
//! graphics view.

std::unique_ptr<sup::gui::MessageHandlerInterface> NodeEditor::CreateMessageHandler()
{
  return CreateMessageHandlerDecorator(m_graphics_view_message_handler.get());
}

//! Setup a toolbar so it can be used via widget's action mechanism.

void NodeEditor::SetupToolBar()
{
  // remove extra spacing so it can be embedded into another toolbar
  m_tool_bar->layout()->setContentsMargins(0, 0, 0, 0);
  m_tool_bar->layout()->setSpacing(0);

  // add toolbar to the list of widgert's action
  m_tool_bar_action->setDefaultWidget(m_tool_bar);
  addAction(m_tool_bar_action);
}

//! Provides node alignment on graphics view.

void NodeEditor::OnAlignRequest()
{
  auto selected = m_graphics_scene->GetSelectedViewItems<ConnectableView>();
  if (selected.size() != 1)
  {
    return;
  }

  auto view = selected.front();
  auto item = view->GetConnectableItem()->GetInstruction();
  algorithm::AlignInstructionTreeWalker(view->pos(), item);
}

void NodeEditor::SetupConnections()
{
  // Propagates delete request from the graphics view to the scene.
  connect(m_graphics_view, &GraphicsView::deleteSelectedRequest, m_graphics_scene,
          &GraphicsScene::OnDeleteSelectedRequest);

  // Forward instruction selection from graphics scene
  connect(m_graphics_scene, &GraphicsScene::InstructionSelected, this,
          &NodeEditor::InstructionSelected);

  // Propagate selection request from GraphicsScene to GraphicsView
  connect(m_graphics_scene, &GraphicsScene::selectionModeChangeRequest, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Propagate selection mode change from toolbar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::selectionMode, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Center view from toolBar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::centerView, m_graphics_view, &GraphicsView::onCenterView);

  // Propagate zoom request from a toolbar to GraphicsView
  connect(m_tool_bar, &NodeEditorToolBar::changeScale, m_graphics_view,
          &GraphicsView::onChangeScale);

  // alignment request from a toolbar
  connect(m_tool_bar, &NodeEditorToolBar::alignSelectedRequest, this, &NodeEditor::OnAlignRequest);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &GraphicsView::selectionModeChanged, m_tool_bar,
          &NodeEditorToolBar::onViewSelectionMode);
}

}  // namespace sequencergui
