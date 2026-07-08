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

#include "custom_presentation_items.h"

#include <oac_tree_gui/components/component_helper.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/presentation_item_helper.h>

namespace oac_tree_gui
{

ChannelPresentationItem::ChannelPresentationItem(mvvm::SessionItem* item,
                                                 const QString& channel_name)
    : mvvm::DefaultDataPresentationItem(item, mvvm::DataRole::kDisplay)
    , m_channel_name(channel_name)
{
}

QVariant ChannelPresentationItem::Data(mvvm::role_t qt_role) const
{
  static const QColor kConnectedColor = GetConnectedVariableColor();
  static const QColor kDisonnectedColor = GetDisonnectedVariableColor();

  if (qt_role == mvvm::GetQtRole(Qt::DecorationRole))
  {
    return GetItem()->Data<bool>() ? kConnectedColor : kDisonnectedColor;
  }

  if (qt_role == mvvm::GetQtRole(Qt::DisplayRole))
  {
    return m_channel_name;
  }

  return {};
}

bool ChannelPresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  (void)data;
  (void)qt_role;
  return false;
}

QVector<mvvm::role_t> ChannelPresentationItem::GetQtRolesToEmit(mvvm::role_t data_role) const
{
  (void)data_role;
  // When IsAvailable status changes, the decoration role (green/gray box rectangle) should be
  // reported
  return {mvvm::GetQtRole(Qt::DecorationRole)};
}

ExclusiveCheckStatePresentationItem::ExclusiveCheckStatePresentationItem(mvvm::SessionItem* item)
    : BooleanDataPresentationItem(item,
                                  mvvm::BooleanDataPresentationItem::AppearanceMode::kCheckBoxOnly)
{
}

bool ExclusiveCheckStatePresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  BeginMacro();
  auto result = BooleanDataPresentationItem::SetData(data, qt_role);
  if (result)
  {
    const bool changed_to_checked = (static_cast<Qt::CheckState>(data.toInt()) == Qt::Checked);
    for (auto instruction : GetSiblings())
    {
      instruction->SetIsRootFlag(!changed_to_checked);
    }
  }
  EndMacro();
  return result;
}

std::vector<InstructionItem*> ExclusiveCheckStatePresentationItem::GetSiblings() const
{
  if (auto container = GetInstructionContainer(); container != nullptr)
  {
    std::vector<InstructionItem*> result;
    auto presented_instruction = GetInstructionItem();
    for (auto instruction : container->GetInstructions())
    {
      if (instruction != presented_instruction)
      {
        result.push_back(instruction);
      }
    }

    return result;
  }

  return {};
}

const InstructionContainerItem* ExclusiveCheckStatePresentationItem::GetInstructionContainer() const
{
  auto* instruction_item = GetInstructionItem();
  return (instruction_item == nullptr)
             ? nullptr
             : dynamic_cast<InstructionContainerItem*>(instruction_item->GetParent());
}

InstructionItem* ExclusiveCheckStatePresentationItem::GetInstructionItem() const
{
  // Our GetItem() is PropertyItem carrying a boolean property, so we need to get its parent to get
  // InstructionItem
  return (GetItem() == nullptr) ? nullptr : dynamic_cast<InstructionItem*>(GetItem()->GetParent());
}

mvvm::ISessionModel* ExclusiveCheckStatePresentationItem::GetModel()
{
  if (auto instruction_item = GetInstructionItem(); instruction_item != nullptr)
  {
    return instruction_item->GetModel();
  }
  return nullptr;
}

void ExclusiveCheckStatePresentationItem::BeginMacro()
{
  if (auto model = GetModel())
  {
    mvvm::utils::BeginMacro(*model, "Set root instruction");
  }
}

void ExclusiveCheckStatePresentationItem::EndMacro()
{
  if (auto model = GetModel())
  {
    mvvm::utils::EndMacro(*model);
  }
}

}  // namespace oac_tree_gui
