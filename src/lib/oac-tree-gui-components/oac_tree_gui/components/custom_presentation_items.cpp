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
#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>

#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/presentation_item_helper.h>
#include <mvvm/providers/viewmodel_utils.h>
#include <mvvm/utils/container_utils.h>

namespace oac_tree_gui
{

namespace
{

//! Returns the presentation of the attribute value: a checkbox for a non-nullable bool.
std::unique_ptr<mvvm::AbstractPresentationItem> CreateValuePresentation(AttributeItem* item)
{
  if ((item != nullptr) && (mvvm::GetTypeCode(item->Data()) == mvvm::TypeCode::Bool)
      && !item->HasFlag(mvvm::Appearance::kNullable))
  {
    return std::make_unique<mvvm::BooleanDataPresentationItem>(item);
  }
  return std::make_unique<mvvm::DefaultDataPresentationItem>(item, mvvm::DataRole::kData);
}

}  // namespace

AttributePresentationItem::AttributePresentationItem(AttributeItem* item)
    : mvvm::AbstractPresentationItem(item)
    , m_attribute(item)
    , m_value_presentation(CreateValuePresentation(item))
{
}

AttributePresentationItem::~AttributePresentationItem() = default;

QVariant AttributePresentationItem::Data(mvvm::role_t qt_role) const
{
  if (!HasText())
  {
    return m_value_presentation->Data(qt_role);
  }

  if ((qt_role == mvvm::GetQtRole(Qt::DisplayRole)) || (qt_role == mvvm::GetQtRole(Qt::EditRole)))
  {
    return QString::fromStdString(m_attribute->GetText());
  }

  if (qt_role == mvvm::GetQtRole(Qt::ForegroundRole))
  {
    return mvvm::utils::TextColorRole(*GetItem());
  }

  if (qt_role == mvvm::GetQtRole(Qt::CheckStateRole))
  {
    return {};
  }

  return m_value_presentation->Data(qt_role);
}

bool AttributePresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  if (!HasText())
  {
    return m_value_presentation->SetData(data, qt_role);
  }

  if (qt_role == mvvm::GetQtRole(Qt::EditRole))
  {
    // empty text leaves the text state
    return m_attribute->SetText(data.toString().toStdString());
  }

  return IsPresentationOnlyRole(qt_role) && m_value_presentation->SetData(data, qt_role);
}

bool AttributePresentationItem::IsEditable() const
{
  return HasText() ? AbstractPresentationItem::IsEditable() : m_value_presentation->IsEditable();
}

bool AttributePresentationItem::IsCheckable() const
{
  return !HasText() && m_value_presentation->IsCheckable();
}

bool AttributePresentationItem::IsPresentationOnlyRole(mvvm::role_t qt_role) const
{
  return m_value_presentation->IsPresentationOnlyRole(qt_role);
}

QVector<mvvm::role_t> AttributePresentationItem::GetQtRolesToEmit(mvvm::role_t data_role) const
{
  if (data_role == itemconstants::kAttributeTextRole)
  {
    return {mvvm::GetQtRole(Qt::DisplayRole), mvvm::GetQtRole(Qt::EditRole),
            mvvm::GetQtRole(Qt::ForegroundRole), mvvm::GetQtRole(Qt::CheckStateRole)};
  }
  return m_value_presentation->GetQtRolesToEmit(data_role);
}

bool AttributePresentationItem::HasText() const
{
  return (m_attribute != nullptr) && (m_attribute->GetState() == AttributeState::kText);
}

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
    // the base class has already converted the check state into the boolean data role
    const bool changed_to_checked = GetItem()->Data<bool>();
    for (auto instruction : GetSiblings())
    {
      instruction->SetIsRootFlag(!changed_to_checked);
    }
  }
  EndMacro();
  return result;
}

std::vector<InstructionItem*> ExclusiveCheckStatePresentationItem::GetSiblings()
{
  if (auto container = GetInstructionContainer(); container != nullptr)
  {
    std::vector<InstructionItem*> result;
    const auto* presented_instruction = GetInstructionItem();
    for (auto* instruction : container->GetInstructions())
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

InstructionContainerItem* ExclusiveCheckStatePresentationItem::GetInstructionContainer()
{
  auto* instruction_item = GetInstructionItem();
  return (instruction_item == nullptr)
             ? nullptr
             : dynamic_cast<InstructionContainerItem*>(instruction_item->GetParent());
}

InstructionItem* ExclusiveCheckStatePresentationItem::GetInstructionItem()
{
  // Our GetItem() is PropertyItem carrying a boolean property, so we need to get its parent to get
  // InstructionItem
  return (GetItem() == nullptr) ? nullptr : GetInstructionFromProperty(*GetItem());
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
