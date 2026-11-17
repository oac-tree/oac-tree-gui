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

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/objects/graphics_scene_component_provider.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <sup/gui/widgets/message_handler_factory.h>
#include <sup/gui/widgets/visibility_agent_base.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/signals/model_listener.h>

#include <QMenu>
#include <QPointF>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>

namespace oac_tree_gui
{

namespace
{

constexpr std::chrono::milliseconds kMessageDurationTime{5000};

//! Returns toolbar actions for the given editor mode.
QList<QAction*> GetToolBarActions(NodeGraphicsViewActions* actions, NodeEditorMode editor_mode)
{
  using ActionKey = NodeGraphicsViewActions::ActionKey;
  if (editor_mode == NodeEditorMode::kNodeEditor)
  {
    return actions->GetActions(
        {ActionKey::kPointer, ActionKey::kPan, ActionKey::kAlign, ActionKey::kSubTree});
  }
  return actions->GetActions({ActionKey::kPointer, ActionKey::kPan, ActionKey::kAlign});
}

//! Returns the top-level instruction with the given identifier, or nullptr.
const InstructionItem* FindTopLevelInstruction(const InstructionContainerItem& container,
                                               const std::string& identifier)
{
  const auto instructions = container.GetInstructions();
  auto iter = std::find_if(instructions.begin(), instructions.end(),
                           [&identifier](const InstructionItem* instruction)
                           { return instruction->GetIdentifier() == identifier; });
  return iter == instructions.end() ? nullptr : *iter;
}

}  // namespace

NodeEditorWidget::NodeEditorWidget(NodeEditorMode editor_mode, QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_editor_mode(editor_mode)
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

  auto on_subscribe = [this]() { SetupSceneComponentProvider(); };
  auto on_unsubscribe = [this]() { m_scene_component_provider.reset(); };
  m_visibility_agent =
      std::make_unique<sup::gui::VisibilityAgentBase>(on_subscribe, on_unsubscribe);
  m_visibility_agent->SetTarget(this);

  addActions(GetToolBarActions(m_view_actions, m_editor_mode));
  UpdateSubTreeText();
}

NodeEditorWidget::~NodeEditorWidget()
{
  // we should reset visibility agent since on destruction widget becomes invisible (in
  // uncontrollable moment of time) and agent might try to call invalid callbacks.
  m_visibility_agent.reset();
}

void NodeEditorWidget::SetProcedure(ProcedureItem* procedure)
{
  if (m_procedure_item == procedure)
  {
    return;
  }

  m_procedure_item = procedure;
  m_listener.reset();
  m_loose_identifiers.clear();

  if (m_procedure_item == nullptr)
  {
    m_scope_identifier.clear();
    m_hidden_identifiers.clear();
    UpdateSubTreeText();
    return;
  }

  const InstructionItem* scope{nullptr};
  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    scope = FindRootInstruction(*m_procedure_item->GetInstructionContainer());

    if (m_procedure_item->GetModel() != nullptr)
    {
      // validate the scope when a top-level instruction is removed
      m_listener = std::make_unique<mvvm::ModelListener>(m_procedure_item->GetModel());
      m_listener->Connect<mvvm::ItemRemovedEvent>(
          [this](const mvvm::ItemRemovedEvent& event)
          {
            if ((event.item == m_procedure_item->GetInstructionContainer())
                && !m_scope_validation_scheduled)
            {
              m_scope_validation_scheduled = true;
              QTimer::singleShot(0, this, &NodeEditorWidget::ValidateScope);
            }
          });
    }
  }

  ApplyScope(scope);
}

std::vector<InstructionItem*> NodeEditorWidget::GetSelectedInstructions() const
{
  return m_scene_component_provider->GetSelectedInstructions();
}

void NodeEditorWidget::SetSelectedInstructions(
    const std::vector<const InstructionItem*>& instructions) const
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

void NodeEditorWidget::SetScope(const InstructionItem* top_level)
{
  if ((m_procedure_item == nullptr) || (m_editor_mode != NodeEditorMode::kNodeEditor))
  {
    return;
  }

  const auto identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  if (identifier == m_scope_identifier)
  {
    return;
  }

  // record loose instructions of the SubTree we are leaving
  auto& loose = m_loose_identifiers[m_scope_identifier];
  loose.clear();
  for (const auto* instruction : GetVisibleTopLevelInstructions())
  {
    if (instruction->GetIdentifier() != m_scope_identifier)
    {
      (void)loose.insert(instruction->GetIdentifier());
    }
  }

  ApplyScope(top_level);
}

const InstructionItem* NodeEditorWidget::GetScope() const
{
  if ((m_procedure_item == nullptr) || m_scope_identifier.empty())
  {
    return nullptr;
  }
  return FindTopLevelInstruction(*m_procedure_item->GetInstructionContainer(), m_scope_identifier);
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

void NodeEditorWidget::SetupSceneComponentProvider()
{
  if ((m_procedure_item == nullptr) || !isVisible())
  {
    return;
  }

  m_scene_component_provider.reset();

  const bool requires_alignment =
      algorithm::RequiresInitialAlignment(GetVisibleTopLevelInstructions());

  // The provider is created first (a read-only operation on the model); shapes appear at the
  // instructions' current coordinates and follow later coordinate changes on their own.
  m_scene_component_provider = CreateGraphicsSceneComponentProvider(m_editor_mode);

  if ((requires_alignment || m_fit_required) && !m_view_update_scheduled)
  {
    // Initial alignment mutates instruction X/Y coordinates, i.e. it creates a model command. This
    // method can be reached synchronously from a model notification (e.g. when the expanded
    // procedure is regenerated on board of a JobItem while the node editor is visible), and the
    // undo/redo framework forbids starting a command from inside a running one. We therefore post
    // the alignment to the next event-loop cycle, once the triggering command has finished.
    m_view_update_scheduled = true;
    QTimer::singleShot(0, this, &NodeEditorWidget::UpdateView);
  }
}

void NodeEditorWidget::ApplyScope(const InstructionItem* top_level)
{
  m_scope_identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  m_hidden_identifiers.clear();

  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    const auto& loose = m_loose_identifiers[m_scope_identifier];
    for (const auto* instruction : m_procedure_item->GetInstructionContainer()->GetInstructions())
    {
      const auto& identifier = instruction->GetIdentifier();
      if ((identifier != m_scope_identifier) && (loose.find(identifier) == loose.end()))
      {
        (void)m_hidden_identifiers.insert(identifier);
      }
    }
    m_fit_required = true;
  }

  UpdateSubTreeText();
  SetupSceneComponentProvider();
}

std::vector<InstructionItem*> NodeEditorWidget::GetVisibleTopLevelInstructions() const
{
  std::vector<InstructionItem*> result;
  if (m_procedure_item == nullptr)
  {
    return result;
  }

  const auto instructions = m_procedure_item->GetInstructionContainer()->GetInstructions();
  (void)std::copy_if(instructions.begin(), instructions.end(), std::back_inserter(result),
                     [this](const InstructionItem* instruction)
                     {
                       return m_hidden_identifiers.find(instruction->GetIdentifier())
                              == m_hidden_identifiers.end();
                     });
  return result;
}

void NodeEditorWidget::UpdateView()
{
  m_view_update_scheduled = false;

  // preconditions may have changed while the update was queued
  if ((m_procedure_item == nullptr) || !isVisible())
  {
    return;
  }

  AlignInstructions();

  if (m_fit_required)
  {
    m_fit_required = false;
    FitScope();
  }
}

void NodeEditorWidget::AlignInstructions()
{
  const auto instructions = GetVisibleTopLevelInstructions();
  if (!algorithm::RequiresInitialAlignment(instructions))
  {
    return;
  }

  const auto center = GetGraphicsViewportCenter();
  // alignment reference point slightly above the center of the scene
  const QPointF reference_point{center.x(), center.y() - (3 * ConnectableViewRectangle().height())};
  algorithm::AlignInstructionTreeWalker(reference_point, instructions);
}

void NodeEditorWidget::FitScope()
{
  auto rect = m_graphics_scene->sceneRect();
  if (!m_graphics_scene->items().isEmpty())
  {
    const double margin = GetGraphicsViewportSize().width() / 2.0;
    rect = m_graphics_scene->itemsBoundingRect().adjusted(-margin, -margin, margin, margin);
  }
  m_graphics_view->setSceneRect(rect);

  m_graphics_view->FitView();
  if (m_graphics_view->GetZoomFactor() > 1.0)
  {
    m_graphics_view->SetZoomFactor(1.0);
    m_graphics_view->CenterView();
  }
}

void NodeEditorWidget::ValidateScope()
{
  m_scope_validation_scheduled = false;

  if ((m_procedure_item == nullptr) || (m_procedure_item->GetModel() == nullptr)
      || m_scope_identifier.empty() || (GetScope() != nullptr))
  {
    return;
  }

  // the scope instruction was moved into another tree, or removed
  const auto* container = m_procedure_item->GetInstructionContainer();
  const auto* new_scope = FindRootInstruction(*container);
  const auto* item =
      dynamic_cast<InstructionItem*>(m_procedure_item->GetModel()->FindItem(m_scope_identifier));
  if (item != nullptr)
  {
    const auto* top_level = GetTopLevelInstruction(*item);
    if ((top_level != nullptr) && (top_level->GetParent() == container))
    {
      new_scope = top_level;
    }
  }

  SetScope(new_scope);
}

void NodeEditorWidget::FillSubTreeMenu(QMenu* menu)
{
  menu->clear();
  UpdateSubTreeText();

  if (m_procedure_item == nullptr)
  {
    return;
  }

  const auto* container = m_procedure_item->GetInstructionContainer();
  const auto* root = FindRootInstruction(*container);

  auto add_action = [this, menu](const InstructionItem& instruction)
  {
    auto action = menu->addAction(QString::fromStdString(instruction.GetDisplayName()));
    action->setCheckable(true);
    action->setChecked(instruction.GetIdentifier() == m_scope_identifier);
    connect(action, &QAction::triggered, this,
            [this, identifier = instruction.GetIdentifier()]()
            {
              const auto* container = m_procedure_item->GetInstructionContainer();
              if (const auto* top_level = FindTopLevelInstruction(*container, identifier);
                  top_level != nullptr)
              {
                SetScope(top_level);
              }
            });
    return action;
  };

  if (root != nullptr)
  {
    // root first, in bold
    auto action = add_action(*root);
    auto font = action->font();
    font.setBold(true);
    action->setFont(font);
    (void)menu->addSeparator();
  }

  for (const auto* instruction : container->GetInstructions())
  {
    if (instruction != root)
    {
      (void)add_action(*instruction);
    }
  }
}

void NodeEditorWidget::UpdateSubTreeText()
{
  const auto* scope = GetScope();
  m_view_actions->SetSubTreeText(
      (scope != nullptr) ? QString::fromStdString(scope->GetDisplayName()) : QString("(none)"));
}

std::unique_ptr<NodeGraphicsScene> NodeEditorWidget::CreateGraphicsScene()
{
  return std::make_unique<NodeGraphicsScene>();
}

std::unique_ptr<GraphicsSceneComponentProvider>
NodeEditorWidget::CreateGraphicsSceneComponentProvider(NodeEditorMode editor_mode)
{
  auto message_callback = [this](const auto& message)
  { m_graphics_view_message_handler->SendMessage(message); };

  auto result = std::make_unique<GraphicsSceneComponentProvider>(
      message_callback, m_graphics_scene.get(), m_procedure_item->GetInstructionContainer(),
      m_hidden_identifiers);

  // forward instruction selection from graphics scene
  connect(result.get(), &GraphicsSceneComponentProvider::selectionChanged, this, [this]()
          { emit instructionSelectionChanged(mvvm::ItemSelection(GetSelectedInstructions())); });

  if (editor_mode == NodeEditorMode::kNodeEditor)
  {
    // change GraphicsView operation mode on connection start/finish
    connect(result.get(), &GraphicsSceneComponentProvider::connectionStarted, this, [this]()
            { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kSimpleSelection); });
    connect(result.get(), &GraphicsSceneComponentProvider::connectionFinished, this, [this]()
            { m_graphics_view->SetOperationMode(GraphicsViewOperationMode::kRubberSelection); });

    // propagates delete request from the graphics view
    connect(m_graphics_view, &NodeGraphicsView::deleteSelectedRequest, result.get(),
            &GraphicsSceneComponentProvider::OnDeleteSelected);

    // propagate drop request from GraphicsScene to GraphicsSceneComponentProvider
    connect(
        m_graphics_scene.get(), &NodeGraphicsScene::dropInstructionRequested, result.get(),
        [this](const QString& name, const QPointF& pos)
        { m_scene_component_provider->DropInstruction(name.toStdString(), {pos.x(), pos.y()}); });

    // propagate branch selection request from GraphicsScene to GraphicsSceneComponentProvider
    connect(m_graphics_scene.get(), &NodeGraphicsScene::instructionDoubleClick, result.get(),
            &GraphicsSceneComponentProvider::SelectInstructionBranch);

    // propagate double click on ports from GraphicsScene to GraphicsSceneComponentProvider
    connect(m_graphics_scene.get(), &NodeGraphicsScene::portDoubleClick, result.get(),
            &GraphicsSceneComponentProvider::DoubleClickPort);
  }
  else
  {
    result->SetReadOnly();
  }

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

  // SubTree selector
  connect(m_view_actions, &NodeGraphicsViewActions::subTreeMenuAboutToShow, this,
          &NodeEditorWidget::FillSubTreeMenu);
}

}  // namespace oac_tree_gui
