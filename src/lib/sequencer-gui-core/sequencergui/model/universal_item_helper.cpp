/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "universal_item_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/aggregate_factory.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/model/iterate_helper.h>
#include <sequencergui/transform/attribute_item_transform_helper.h>
#include <sequencergui/transform/transform_from_domain.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/i_sessionmodel.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>

namespace sequencergui
{

InstructionItem *InsertInstruction(const std::string &domain_type, mvvm::SessionItem *parent,
                                   const mvvm::TagIndex &tag_index)
{
  return InsertInstruction(sequencergui::CreateInstructionItem(domain_type), parent, tag_index);
}

InstructionItem *InsertAggregate(const std::string &domain_type, mvvm::SessionItem *parent,
                                 const mvvm::TagIndex &tag_index)
{
  static ::sequencergui::AggregateFactory factory;
  return InsertInstruction(factory.Create(domain_type), parent, tag_index);
}

InstructionItem *InsertInstruction(std::unique_ptr<sequencergui::InstructionItem> item,
                                   mvvm::SessionItem *parent, const mvvm::TagIndex &tag_index)
{
  return static_cast<InstructionItem *>(
      mvvm::utils::InsertItem(std::move(item), parent, tag_index));
}

bool IsAggregateName(const std::string &name)
{
  static ::sequencergui::AggregateFactory factory;
  return factory.Contains(name);
}

void SetInput(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kInputVariableNameAttribute, value);
}

void SetOutput(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kOutputVariableNameAttribute, value);
}

void SetDescription(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kDescriptionAttribute, value);
}

void SetText(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kTextAttribute, value);
}

void SetVariableName(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kGenericVariableNameAttribute, value);
}

void AddShowCollapsedProperty(InstructionItem &item)
{
  static const std::vector<std::string> collapsed_by_default{
      domainconstants::kIncludeInstructionType, domainconstants::kIncludeProcedureInstructionType};

  if (mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute))
  {
    auto property = item.GetItem<sup::gui::AnyValueItem>(domainconstants::kShowCollapsedAttribute);
    property->SetDisplayName("Show collapsed");
    property->SetToolTip("Show child branch collapsed duing procedure execution");
    if (mvvm::utils::Contains(collapsed_by_default, item.GetDomainType()))
    {
      property->SetData(true);
      SetAttributeExposedFlag(true, *property);
    }
  }
}

bool IsCollapsed(const InstructionItem &item)
{
  return mvvm::utils::HasTag(item, domainconstants::kShowCollapsedAttribute)
             ? item.Property<bool>(domainconstants::kShowCollapsedAttribute)
             : false;
}

std::vector<const InstructionItem *> GetCollapsedItems(const InstructionContainerItem &container)
{
  std::vector<const InstructionItem *> result;

  auto func = [&result](const InstructionItem *item)
  {
    if (IsCollapsed(*item))
    {
      result.push_back(item);
    }
  };

  IterateInstructionContainer<const InstructionItem *>(container.GetInstructions(), func);

  return result;
}

sup::gui::AnyValueItem *GetAnyValueItem(const InstructionItem &item)
{
  return mvvm::utils::HasTag(item, sequencergui::itemconstants::kAnyValueTag)
             ? item.GetItem<sup::gui::AnyValueItem>(sequencergui::itemconstants::kAnyValueTag)
             : nullptr;
}

}  // namespace sequencergui
