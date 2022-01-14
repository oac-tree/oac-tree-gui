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

#include "sequencergui/nodeeditor/graphicsscenecontroller.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/nodeeditor/connectableview.h"
#include "sequencergui/nodeeditor/connectableviewfactory.h"
#include "sequencergui/nodeeditor/graphicsscene.h"
#include "sequencergui/nodeeditor/viewfactoryinterface.h"

#include "mvvm/model/itemutils.h"

#include <QDebug>
#include <map>

namespace sequencergui
{
struct GraphicsSceneController::GraphicsSceneControllerImpl
{
  SequencerModel* m_model{nullptr};
  GraphicsScene* m_graphics_scene{nullptr};
  InstructionContainerItem* m_root_item{nullptr};
  std::map<const InstructionItem*, ConnectableView*> m_instruction_to_view;
  bool m_block_update{false};
  std::unique_ptr<ViewFactoryInterface> m_view_factory;
  std::function<void(InstructionContainerItem*)> m_align_strategy;

  GraphicsSceneControllerImpl(SequencerModel* model, GraphicsScene* graphics_scene)
      : m_model(model), m_graphics_scene(graphics_scene)
  {
    if (!m_model || !m_graphics_scene)
    {
      throw std::runtime_error("Error in controller initialisation");
    }
    m_view_factory = std::make_unique<ConnectableViewFactory>();
  }

  ConnectableView* FindView(const InstructionItem* instruction)
  {
    auto it = m_instruction_to_view.find(instruction);
    return it == m_instruction_to_view.end() ? nullptr : it->second;
  }

  void Iterate(InstructionItem* instruction, ConnectableView* parent_view)
  {
    for (auto instruction : instruction->GetInstructions())
    {
      auto next_parent_view = ProcessInstruction(instruction, parent_view);
      if (next_parent_view)
      {
        Iterate(instruction, next_parent_view);
      }
    }
  }

  ConnectableView* ProcessInstruction(InstructionItem* instruction, ConnectableView* parent_view)
  {
    auto instruction_view = FindView(instruction);

    if (!instruction_view)
    {
      auto instruction_view_unique = m_view_factory->CreateView(instruction);
      instruction_view = instruction_view_unique.get();
      m_instruction_to_view[instruction] = instruction_view;
      m_graphics_scene->addItem(instruction_view_unique.release());
    }

    if (parent_view)
    {
      parent_view->MakeChildConnected(instruction_view);
    }

    return instruction_view;
  }

  //! Inserts view(s) for the child instruction with a given index.
  void InsertView(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
  {
    auto new_child = parent->GetItem<InstructionItem>(tag_index.tag, tag_index.index);
    auto parent_view = FindView(dynamic_cast<InstructionItem*>(parent));
    auto next_parent_view = ProcessInstruction(new_child, parent_view);
    if (next_parent_view)
    {
      Iterate(new_child, next_parent_view);
    }
  }

  //! Removes a view corresponding to the instruction.

  void RemoveView(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index)
  {
    auto to_remove = parent->GetItem<InstructionItem>(tag_index.tag, tag_index.index);
    auto it = m_instruction_to_view.find(to_remove);
    if (it != m_instruction_to_view.end())
    {
      delete it->second;
      m_instruction_to_view.erase(it);
    }
  }

  void InitScene(InstructionContainerItem* root_item)
  {
    if (!root_item)
    {
      throw std::runtime_error("Instruction container items is not initialised");
    }

    m_graphics_scene->clear();
    m_instruction_to_view.clear();
    m_root_item = root_item;

    for (auto instruction : root_item->GetInstructions())
    {
      auto next_parent_view = ProcessInstruction(instruction, nullptr);
      if (next_parent_view)
      {
        Iterate(instruction, next_parent_view);
      }
    }

    if (m_align_strategy)
    {
      m_align_strategy(root_item);
    }
  }

  // Returns true if given item is either our root item, or is one of its descendant
  bool IsInScope(mvvm::SessionItem* item) const
  {
    return item == m_root_item || mvvm::utils::IsItemAncestor(item, m_root_item);
  }

  void OnDataChanged(mvvm::SessionItem* item, int role)
  {
    if (m_block_update)
    {
      return;
    }

    m_block_update = true;

    if (auto instruction = dynamic_cast<InstructionItem*>(item->GetParent()); item)
    {
      if (auto instruction_view = FindView(instruction); instruction_view)
      {
        instruction_view->UpdateViewFromItem();
      }
    }
    m_block_update = false;
  }
};

GraphicsSceneController::GraphicsSceneController(SequencerModel* model,
                                                 GraphicsScene* graphics_scene)
    : p_impl(std::make_unique<GraphicsSceneControllerImpl>(model, graphics_scene))
{
  model->Subscribe(this);
}

void GraphicsSceneController::SetAlignStrategy(
    std::function<void(InstructionContainerItem*)> strategy)
{
  p_impl->m_align_strategy = strategy;
}

GraphicsSceneController::~GraphicsSceneController() = default;

void GraphicsSceneController::OnItemInserted(mvvm::SessionItem* parent,
                                             const mvvm::TagIndex& tag_index)
{
  if (p_impl->IsInScope(parent))
  {
    p_impl->InsertView(parent, tag_index);
  }
}

void GraphicsSceneController::OnAboutToRemoveItem(mvvm::SessionItem* parent,
                                                  const mvvm::TagIndex& tag_index)
{
  if (p_impl->IsInScope(parent))
  {
    p_impl->RemoveView(parent, tag_index);
  }
}

void GraphicsSceneController::OnDataChanged(mvvm::SessionItem* item, int role)
{
  p_impl->OnDataChanged(item, role);
}

void GraphicsSceneController::Init(InstructionContainerItem* root_item)
{
  p_impl->InitScene(root_item);
}

}  // namespace sequencergui
