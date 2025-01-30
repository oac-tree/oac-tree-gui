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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_editor.h"

#include "graphics_view.h"
#include "node_editor_actions.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/nodeeditor/connectable_instruction_adapter.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/graphics_scene.h>
#include <oac_tree_gui/nodeeditor/graphics_scene_controller.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/application_model.h>

#include <QDebug>
#include <QPointF>
#include <QVBoxLayout>
#include <QWidgetAction>

namespace
{
QList<QAction *> GetToolBarActions(oac_tree_gui::NodeEditorActions *actions)
{
  using ActionKey = oac_tree_gui::NodeEditorActions::ActionKey;
  return actions->GetActions({ActionKey::kPointer, ActionKey::kPan, ActionKey::kZoom,
                              ActionKey::kCenter, ActionKey::kAlign});
}
}  // namespace

namespace oac_tree_gui
{

NodeEditor::NodeEditor(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_actions(new NodeEditorActions)
    , m_graphics_scene(new GraphicsScene([this](const auto &message)
                                         { m_graphics_view_message_handler->SendMessage(message); },
                                         this))
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
    , m_graphics_view_message_handler(sup::gui::CreateWidgetOverlayMessageHandler(m_graphics_view))
{
  setWindowTitle("NodeEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_graphics_view);

  SetupConnections();

  auto on_subscribe = [this]() { SetupController(); };

  auto on_unsubscribe = [this]() { m_scene_controller.reset(); };

  // will be deleted as a child of QObject
  m_visibility_agent = new sup::gui::VisibilityAgentBase(this, on_subscribe, on_unsubscribe);

  addActions(GetToolBarActions(m_actions));
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

  SetupController();
}

std::vector<InstructionItem *> NodeEditor::GetSelectedInstructions() const
{
  return m_graphics_scene->GetSelectedInstructions();
}

void NodeEditor::SetSelectedInstructions(const std::vector<InstructionItem *> &instructions) const
{
  if (isHidden())
  {
    return;
  }

  m_graphics_scene->SetSelectedInstructions(instructions);
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

void NodeEditor::SetupController()
{
  if (!m_procedure_item || !isVisible())
  {
    return;
  }

  auto container = m_procedure_item->GetInstructionContainer();

  m_scene_controller.reset();

  if (algorithm::RequiresInitialAlignment(container->GetInstructions()))
  {
    const QPointF reference_point = m_graphics_scene->sceneRect().center();
    algorithm::AlignInstructionTreeWalker(reference_point, container->GetInstructions());
  }

  m_scene_controller =
      std::make_unique<GraphicsSceneController>(m_procedure_item->GetModel(), m_graphics_scene);

  m_scene_controller->Init(m_procedure_item->GetInstructionContainer());

  static bool is_first_recenter = true;
  if (is_first_recenter)
  {
    is_first_recenter = false;
    m_graphics_view->onCenterView();
  }
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
  connect(m_actions, &NodeEditorActions::selectionMode, m_graphics_view,
          &GraphicsView::onSelectionMode);

  // Center view from toolBar to GraphicsView
  connect(m_actions, &NodeEditorActions::centerView, m_graphics_view, &GraphicsView::onCenterView);

  // Propagate zoom request from a toolbar to GraphicsView
  connect(m_actions, &NodeEditorActions::changeScale, m_graphics_view,
          &GraphicsView::onChangeScale);

  // alignment request from a toolbar
  connect(m_actions, &NodeEditorActions::alignSelectedRequest, this, &NodeEditor::OnAlignRequest);

  // Propagate selection mode change from GraphicsView to a toolBar
  connect(m_graphics_view, &GraphicsView::selectionModeChanged, m_actions,
          &NodeEditorActions::onViewSelectionMode);
}

}  // namespace oac_tree_gui
