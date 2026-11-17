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

#include "node_editor_scene_controller.h"

#include "graphics_scene_component_provider.h"
#include "node_graphics_scene.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/signals/model_listener.h>

#include <QPointF>
#include <QTimer>
#include <algorithm>

namespace oac_tree_gui
{

NodeEditorSceneController::NodeEditorSceneController(NodeEditorMode editor_mode,
                                                     NodeGraphicsScene* scene,
                                                     send_message_t send_message_callback)
    : m_editor_mode(editor_mode)
    , m_scene(scene)
    , m_send_message_callback(std::move(send_message_callback))
{
  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    SetupSceneConnections();
  }
}

NodeEditorSceneController::~NodeEditorSceneController() = default;

void NodeEditorSceneController::SetProcedure(ProcedureItem* procedure)
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
    UpdateResolvedScope();
    return;
  }

  const InstructionItem* scope{nullptr};
  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    scope = FindRootInstruction(*GetInstructionContainer());

    if (m_procedure_item->GetModel() != nullptr)
    {
      // the scope may change when instructions are inserted, moved or removed
      m_listener = std::make_unique<mvvm::ModelListener>(m_procedure_item->GetModel());
      m_listener->Connect<mvvm::ItemInsertedEvent>([this](const auto&) { UpdateResolvedScope(); });
      m_listener->Connect<mvvm::ItemRemovedEvent>([this](const auto&) { UpdateResolvedScope(); });
    }
  }

  ApplyScope(scope);
}

InstructionContainerItem* NodeEditorSceneController::GetInstructionContainer() const
{
  return (m_procedure_item != nullptr) ? m_procedure_item->GetInstructionContainer() : nullptr;
}

void NodeEditorSceneController::SetActive(bool value)
{
  m_active = value;
  if (m_active)
  {
    SetupProvider();
  }
  else
  {
    m_provider.reset();
    m_scene->clear();
  }
}

void NodeEditorSceneController::SetScope(const InstructionItem* top_level)
{
  if ((m_procedure_item == nullptr) || (m_editor_mode != NodeEditorMode::kNodeEditor))
  {
    return;
  }

  const auto* current = GetScope();
  const auto current_identifier =
      (current != nullptr) ? current->GetIdentifier() : m_scope_identifier;
  const auto identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  if (identifier == current_identifier)
  {
    return;
  }

  // record loose instructions of the SubTree we are leaving
  auto& loose = m_loose_identifiers[current_identifier];
  loose.clear();
  for (const auto* instruction : GetVisibleTopLevelInstructions())
  {
    if (instruction->GetIdentifier() != current_identifier)
    {
      (void)loose.insert(instruction->GetIdentifier());
    }
  }

  ApplyScope(top_level);
}

const InstructionItem* NodeEditorSceneController::GetScope() const
{
  if ((m_procedure_item == nullptr) || (m_procedure_item->GetModel() == nullptr)
      || m_scope_identifier.empty())
  {
    return nullptr;
  }

  const auto* anchor = dynamic_cast<const InstructionItem*>(
      m_procedure_item->GetModel()->FindItem(m_scope_identifier));
  if (anchor == nullptr)
  {
    return nullptr;
  }

  const auto* top_level = GetTopLevelInstruction(*anchor);
  if ((top_level == nullptr) || (top_level->GetParent() != GetInstructionContainer())
      || (m_hidden_identifiers.find(top_level->GetIdentifier()) != m_hidden_identifiers.end()))
  {
    return nullptr;
  }
  return top_level;
}

std::vector<InstructionItem*> NodeEditorSceneController::GetVisibleTopLevelInstructions() const
{
  std::vector<InstructionItem*> result;
  if (m_procedure_item == nullptr)
  {
    return result;
  }

  const auto instructions = GetInstructionContainer()->GetInstructions();
  (void)std::copy_if(instructions.begin(), instructions.end(), std::back_inserter(result),
                     [this](const InstructionItem* instruction)
                     {
                       return m_hidden_identifiers.find(instruction->GetIdentifier())
                              == m_hidden_identifiers.end();
                     });
  return result;
}

std::vector<InstructionItem*> NodeEditorSceneController::GetSelectedInstructions() const
{
  return m_provider ? m_provider->GetSelectedInstructions() : std::vector<InstructionItem*>{};
}

void NodeEditorSceneController::SetSelectedInstructions(
    const std::vector<const InstructionItem*>& instructions)
{
  if (m_provider)
  {
    m_provider->SetSelectedInstructions(instructions);
  }
}

void NodeEditorSceneController::OnDeleteSelected()
{
  if (m_provider)
  {
    m_provider->OnDeleteSelected();
  }
}

void NodeEditorSceneController::AlignSelected()
{
  auto selected = mvvm::GetSelectedShapes<mvvm::ConnectableShape>(*m_scene);
  if (selected.size() != 1)
  {
    return;
  }

  auto shape = selected.front();
  algorithm::AlignInstructionTreeWalker(shape->pos(),
                                        mvvm::GetUnderlyingItem<InstructionItem>(shape));
}

void NodeEditorSceneController::ApplyScope(const InstructionItem* top_level)
{
  m_scope_identifier = (top_level != nullptr) ? top_level->GetIdentifier() : std::string();
  m_hidden_identifiers.clear();

  if (m_editor_mode == NodeEditorMode::kNodeEditor)
  {
    const auto& loose = m_loose_identifiers[m_scope_identifier];
    for (const auto* instruction : GetInstructionContainer()->GetInstructions())
    {
      const auto& identifier = instruction->GetIdentifier();
      if ((identifier != m_scope_identifier) && (loose.find(identifier) == loose.end()))
      {
        (void)m_hidden_identifiers.insert(identifier);
      }
    }
    m_scope_shown_required = true;
  }

  UpdateResolvedScope();
  SetupProvider();
}

void NodeEditorSceneController::SetupProvider()
{
  if ((m_procedure_item == nullptr) || !m_active)
  {
    return;
  }

  m_provider.reset();

  // The provider is created first (a read-only operation on the model); shapes appear at the
  // instructions' current coordinates and follow later coordinate changes on their own.
  m_provider = CreateProvider();

  const bool requires_alignment =
      algorithm::RequiresInitialAlignment(GetVisibleTopLevelInstructions());

  if ((requires_alignment || m_scope_shown_required) && !m_view_update_scheduled)
  {
    // Initial alignment mutates instruction X/Y coordinates, i.e. it creates a model command. This
    // method can be reached synchronously from a model notification (e.g. when the expanded
    // procedure is regenerated on board of a JobItem while the node editor is visible), and the
    // undo/redo framework forbids starting a command from inside a running one. We therefore post
    // the alignment to the next event-loop cycle, once the triggering command has finished.
    m_view_update_scheduled = true;
    QTimer::singleShot(0, this, &NodeEditorSceneController::UpdateView);
  }
}

std::unique_ptr<GraphicsSceneComponentProvider> NodeEditorSceneController::CreateProvider()
{
  auto result = std::make_unique<GraphicsSceneComponentProvider>(
      m_send_message_callback, m_scene, GetInstructionContainer(), m_hidden_identifiers);

  connect(result.get(), &GraphicsSceneComponentProvider::selectionChanged, this,
          &NodeEditorSceneController::selectionChanged);
  connect(result.get(), &GraphicsSceneComponentProvider::connectionStarted, this,
          &NodeEditorSceneController::connectionStarted);
  connect(result.get(), &GraphicsSceneComponentProvider::connectionFinished, this,
          &NodeEditorSceneController::connectionFinished);

  if (m_editor_mode == NodeEditorMode::kNodeViewer)
  {
    result->SetReadOnly();
  }

  return result;
}

void NodeEditorSceneController::UpdateView()
{
  m_view_update_scheduled = false;

  // preconditions may have changed while the update was queued
  if ((m_procedure_item == nullptr) || !m_active)
  {
    return;
  }

  const auto instructions = GetVisibleTopLevelInstructions();
  if (algorithm::RequiresInitialAlignment(instructions))
  {
    const auto center = GetGraphicsViewportCenter();
    // alignment reference point slightly above the center of the scene
    const QPointF reference_point{center.x(),
                                  center.y() - (3 * ConnectableViewRectangle().height())};
    algorithm::AlignInstructionTreeWalker(reference_point, instructions);
  }

  if (m_scope_shown_required)
  {
    m_scope_shown_required = false;
    emit scopeShown();
  }
}

void NodeEditorSceneController::UpdateResolvedScope()
{
  const auto* scope = GetScope();
  const auto identifier = (scope != nullptr) ? scope->GetIdentifier() : std::string();
  if (identifier != m_resolved_scope_identifier)
  {
    m_resolved_scope_identifier = identifier;
    emit scopeChanged();
  }
}

void NodeEditorSceneController::SetupSceneConnections()
{
  connect(m_scene, &NodeGraphicsScene::dropInstructionRequested, this,
          [this](const QString& name, const QPointF& pos)
          {
            if (m_provider)
            {
              m_provider->DropInstruction(name.toStdString(), {pos.x(), pos.y()});
            }
          });

  connect(m_scene, &NodeGraphicsScene::instructionDoubleClick, this,
          [this](const InstructionItem* instruction)
          {
            if (m_provider)
            {
              m_provider->SelectInstructionBranch(instruction);
            }
          });

  connect(m_scene, &NodeGraphicsScene::portDoubleClick, this,
          [this](const mvvm::INodePort* port)
          {
            if (m_provider)
            {
              m_provider->DoubleClickPort(port);
            }
          });
}

}  // namespace oac_tree_gui
