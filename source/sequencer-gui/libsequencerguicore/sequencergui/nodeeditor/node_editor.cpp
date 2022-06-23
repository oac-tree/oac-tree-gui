/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/node_editor.h"

#include "sequencergui/nodeeditor/node_editor_toolbar.h"

#include <mvvm/model/application_model.h>
#include <sequencergui/core/message_handler_factory.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/nodeeditor/connectable_instruction_adapter.h>
#include <sequencergui/nodeeditor/connectable_view.h>
#include <sequencergui/nodeeditor/graphics_scene.h>
#include <sequencergui/nodeeditor/graphics_scene_controller.h>
#include <sequencergui/nodeeditor/graphics_view.h>
#include <sequencergui/nodeeditor/scene_utils.h>
#include <sequencergui/utils/sequencer_align_utils.h>

#include <QDebug>
#include <QPointF>
#include <QToolBar>
#include <QVBoxLayout>

namespace sequencergui
{

NodeEditor::NodeEditor(QWidget *parent)
    : QWidget(parent)
    , m_graphics_scene(new GraphicsScene(this))
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
    , m_graphics_view_message_handler(CreateWidgetOverlayMessageHandler(m_graphics_view))
{
  setWindowTitle("NodeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);
  layout->addWidget(m_graphics_view);

  m_graphics_scene->SetMessageHandler(CreateMessageHandler());

  SetupConnections();
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
  auto model = dynamic_cast<mvvm::ApplicationModel *>(
      procedure->GetModel());  // FIXME find solution without cast
  m_scene_controller = std::make_unique<GraphicsSceneController>(model, m_graphics_scene);

  auto align_strategy = [this](auto container)
  {
    const QPointF reference_point = m_graphics_scene->sceneRect().center();
    algorithm::AlignInstructionTreeWalker(reference_point, container);
  };
  m_scene_controller->SetAlignStrategy(align_strategy);

  m_scene_controller->Init(instruction_container);

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

std::unique_ptr<MessageHandlerInterface> NodeEditor::CreateMessageHandler()
{
  return CreateMessageHandlerDecorator(m_graphics_view_message_handler.get());
}

std::unique_ptr<QToolBar> NodeEditor::CreateToolBar()
{
  auto result = std::make_unique<NodeEditorToolBar>();

  // Propagate selection mode change from toolbar to GraphicsView
  connect(result.get(), &NodeEditorToolBar::selectionMode, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Center view from toolBar to GraphicsView
  connect(result.get(), &NodeEditorToolBar::centerView, m_graphics_view,
          &GraphicsView::onCenterView);

  // Propagate zoom request from a toolbar to GraphicsView
  connect(result.get(), &NodeEditorToolBar::changeScale, m_graphics_view,
          &GraphicsView::onChangeScale);

  auto on_align = [this]()
  {
    auto selected = m_graphics_scene->GetSelectedViewItems<ConnectableView>();
    if (selected.size() != 1)
    {
      return;
    }

    auto view = selected.front();
    auto item = view->GetConnectableItem()->GetInstruction();
    algorithm::AlignInstructionTreeWalker(view->pos(), item);
  };
  connect(result.get(), &NodeEditorToolBar::alignSelectedRequest, this, on_align);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &GraphicsView::selectionModeChanged, result.get(),
          &NodeEditorToolBar::onViewSelectionMode);

  return result;
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
}

}  // namespace sequencergui
