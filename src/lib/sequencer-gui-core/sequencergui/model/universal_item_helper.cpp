/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/item_constants.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
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
  item->SetProperty(domainconstants::kInputAttribute, value);
}

void SetOutput(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kOutputAttribute, value);
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
  item->SetProperty(domainconstants::kVarNameAttribute, value);
}

void AddShowCollapsedProperty(InstructionItem &item)
{
  static const std::vector<std::string> collapsed_by_default{
      domainconstants::kIncludeInstructionType, domainconstants::kIncludeProcedureInstructionType};

  if (mvvm::utils::HasTag(item, itemconstants::kChildInstructions))
  {
    auto property = item.AddProperty(itemconstants::kShowCollapsed, false)
                        ->SetDisplayName("Show collapsed")
                        ->SetToolTip("Show child branch collapsed duing procedure execution");

    if (mvvm::utils::Contains(collapsed_by_default, item.GetDomainType()))
    {
      property->SetData(true);
    }
  }
}

bool IsCollapsed(const InstructionItem &item)
{
  return mvvm::utils::HasTag(item, itemconstants::kShowCollapsed)
             ? item.Property<bool>(itemconstants::kShowCollapsed)
             : false;
}

}  // namespace sequencergui
