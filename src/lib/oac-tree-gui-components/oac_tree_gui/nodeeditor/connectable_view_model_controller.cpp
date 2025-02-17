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

#include "connectable_view_model_controller.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/nodeeditor/connectable_view.h>
#include <oac_tree_gui/nodeeditor/connectable_view_factory.h>
#include <oac_tree_gui/nodeeditor/connectable_view_map.h>
#include <oac_tree_gui/nodeeditor/i_connectable_view_factory.h>
#include <oac_tree_gui/nodeeditor/node_graphics_scene.h>

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

namespace oac_tree_gui
{

struct ConnectableViewModelController::ConnectableViewModelControllerImpl
{
  mvvm::ISessionModel* m_model{nullptr};
  NodeGraphicsScene* m_graphics_scene{nullptr};
  InstructionContainerItem* m_root_item{nullptr};
  ConnectableViewMap m_instruction_to_view;
  bool m_block_update{false};
  std::unique_ptr<IConnectableViewFactory> m_view_factory;
  std::unique_ptr<mvvm::ModelListener> m_listener;

  ConnectableViewModelControllerImpl(mvvm::ISessionModel* model, NodeGraphicsScene* graphics_scene)
      : m_model(model)
      , m_graphics_scene(graphics_scene)
      , m_listener(std::make_unique<mvvm::ModelListener>(model))
  {
    if (!m_model || !m_graphics_scene)
    {
      throw std::runtime_error("Error in controller initialisation");
    }
    m_view_factory = std::make_unique<ConnectableViewFactory>();
  }

  ConnectableView* FindView(const InstructionItem* instruction)
  {
    return m_instruction_to_view.FindView(instruction);
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
      m_instruction_to_view.Insert(instruction, instruction_view);
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
    auto new_child = parent->GetItem<InstructionItem>(tag_index);
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
    auto item_to_remove = parent->GetItem<InstructionItem>(tag_index);
    // Removing view of item, and all views of child items.
    for (auto view : m_instruction_to_view.FindRelatedViews(item_to_remove))
    {
      m_instruction_to_view.Remove(view);
      delete view;  // removing from scene
    }
  }

  void InitScene(InstructionContainerItem* root_item)
  {
    if (!root_item)
    {
      throw std::runtime_error("Instruction container items is not initialised");
    }

    m_graphics_scene->clear();
    m_instruction_to_view.Clear();
    m_root_item = root_item;

    for (auto instruction : root_item->GetInstructions())
    {
      auto next_parent_view = ProcessInstruction(instruction, nullptr);
      if (next_parent_view)
      {
        Iterate(instruction, next_parent_view);
      }
    }
  }

  // Returns true if given item is either our root item, or is one of its descendant
  bool IsInScope(mvvm::SessionItem* item) const
  {
    return item == m_root_item || mvvm::utils::IsItemAncestor(item, m_root_item);
  }

  // Returns true if given item is a top level item (ProcedureItem) of our container.
  bool IsProcedure(mvvm::SessionItem* item) const { return item == m_root_item->GetParent(); }

  void OnDataChanged(mvvm::SessionItem* item, int role)
  {
    (void)role;
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

ConnectableViewModelController::ConnectableViewModelController(mvvm::ISessionModel* model,
                                                               NodeGraphicsScene* graphics_scene)
    : p_impl(std::make_unique<ConnectableViewModelControllerImpl>(model, graphics_scene))
{
  p_impl->m_listener->Connect<mvvm::DataChangedEvent>(
      this, &ConnectableViewModelController::OnModelEvent);

  p_impl->m_listener->Connect<mvvm::ItemInsertedEvent>(
      this, &ConnectableViewModelController::OnModelEvent);
  p_impl->m_listener->Connect<mvvm::AboutToRemoveItemEvent>(
      this, &ConnectableViewModelController::OnModelEvent);
}

ConnectableViewModelController::~ConnectableViewModelController() = default;

void ConnectableViewModelController::OnModelEvent(const mvvm::ItemInsertedEvent& event)
{
  if (event.tag_index.GetTag() == itemconstants::kAnyValueTag)
  {
    return;
  }

  if (p_impl->IsInScope(event.item))
  {
    p_impl->InsertView(event.item, event.tag_index);
  }
}

void ConnectableViewModelController::OnModelEvent(const mvvm::AboutToRemoveItemEvent& event)
{
  auto [parent, tag_index] = event;
  auto item_to_remove = parent->GetItem(tag_index);
  if (tag_index.GetTag() == itemconstants::kAnyValueTag)
  {
    return;
  }

  // Special case when user removes procedure owning our instruction container.
  if (item_to_remove == p_impl->m_root_item->GetParent())
  {
    // We can't continue, all objects should be removed from the scene.
    p_impl->m_graphics_scene->ResetContext();
    return;
  }

  if (p_impl->IsInScope(parent))
  {
    p_impl->RemoveView(parent, tag_index);
  }
}

void ConnectableViewModelController::OnModelEvent(const mvvm::DataChangedEvent& event)
{
  p_impl->OnDataChanged(event.item, event.data_role);
}

void ConnectableViewModelController::Init(InstructionContainerItem* root_item)
{
  p_impl->InitScene(root_item);
}

}  // namespace oac_tree_gui
