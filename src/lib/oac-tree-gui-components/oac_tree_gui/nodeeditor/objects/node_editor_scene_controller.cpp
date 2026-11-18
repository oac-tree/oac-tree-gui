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
#include "sub_tree_scope.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <sup/gui/core/message_event.h>

#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/graphics_scene_helper.h>
#include <mvvm/nodeeditor/i_node_port.h>

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
    , m_scope(std::make_unique<SubTreeScope>())
{
  connect(m_scope.get(), &SubTreeScope::scopeChanged, this,
          &NodeEditorSceneController::scopeChanged);

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

  if ((m_procedure_item == nullptr) || (m_editor_mode == NodeEditorMode::kNodeViewer))
  {
    m_scope->Clear();
  }
  else
  {
    auto container = GetInstructionContainer();
    m_scope->Reset(container, FindRootInstruction(*container));
    m_scope_shown_required = true;
  }

  SetupProvider();
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
  if (m_scope->SetScope(top_level))
  {
    m_scope_shown_required = true;
    SetupProvider();
  }
}

const InstructionItem* NodeEditorSceneController::GetScope() const
{
  return m_scope->GetScope();
}

void NodeEditorSceneController::ShowInstruction(const InstructionItem& instruction)
{
  const auto* top_level = GetTopLevelInstruction(instruction);
  if ((top_level == nullptr) || (top_level->GetParent() != GetInstructionContainer()))
  {
    return;
  }

  const auto& hidden = m_scope->GetHiddenIdentifiers();
  if (hidden.find(top_level->GetIdentifier()) != hidden.end())
  {
    SetScope(top_level);
  }
}

std::string NodeEditorSceneController::GetOpenSubTreeError(const InstructionItem& instruction) const
{
  if (!IsIncludeInstruction(instruction))
  {
    return "Not an Include instruction.";
  }

  if (instruction.GetDomainType() == domainconstants::kIncludeProcedureInstructionType)
  {
    return "IncludeProcedure refers to another procedure file.";
  }

  if (!IsLocalInclude(instruction))
  {
    return "Include refers to another file.";
  }

  const auto* container = GetInstructionContainer();
  if ((container == nullptr) || (FindIncludeTarget(instruction, *container) == nullptr))
  {
    const auto path = instruction.GetAttribute<std::string>(domainconstants::kPathAttribute);
    return path.empty() ? std::string("Include path is empty.")
                        : "Include path '" + path + "' is not found.";
  }

  return {};
}

void NodeEditorSceneController::OpenSubTree(const InstructionItem& include)
{
  if (m_editor_mode != NodeEditorMode::kNodeEditor)
  {
    return;
  }

  if (const auto error = GetOpenSubTreeError(include); !error.empty())
  {
    m_send_message_callback(sup::gui::CreateInvalidOperationMessage("Can't open SubTree", error));
    return;
  }

  const auto* target = FindIncludeTarget(include, *GetInstructionContainer());
  const auto* top_level = GetTopLevelInstruction(*target);
  SetScope(top_level);

  if (target != top_level)
  {
    SetSelectedInstructions({target});
    emit centerOnSelectionRequested();
  }
}

std::vector<InstructionItem*> NodeEditorSceneController::GetVisibleTopLevelInstructions() const
{
  std::vector<InstructionItem*> result;
  if (m_procedure_item == nullptr)
  {
    return result;
  }

  const auto& hidden = m_scope->GetHiddenIdentifiers();
  const auto instructions = GetInstructionContainer()->GetInstructions();
  (void)std::copy_if(instructions.begin(), instructions.end(), std::back_inserter(result),
                     [&hidden](const InstructionItem* instruction)
                     { return hidden.find(instruction->GetIdentifier()) == hidden.end(); });
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
      m_send_message_callback, m_scene, GetInstructionContainer(), m_scope->GetHiddenIdentifiers());

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
          &NodeEditorSceneController::OnInstructionDoubleClick);

  connect(m_scene, &NodeGraphicsScene::portDoubleClick, this,
          [this](const mvvm::INodePort* port)
          {
            if (m_provider)
            {
              m_provider->DoubleClickPort(port);
            }
          });
}

void NodeEditorSceneController::OnInstructionDoubleClick(const InstructionItem* instruction)
{
  if ((instruction != nullptr) && IsIncludeInstruction(*instruction))
  {
    OpenSubTree(*instruction);
  }
  else if (m_provider)
  {
    m_provider->SelectInstructionBranch(instruction);
  }
}

}  // namespace oac_tree_gui
