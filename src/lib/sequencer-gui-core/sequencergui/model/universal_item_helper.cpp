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
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/interfaces/sessionmodel_interface.h>

namespace sequencergui
{

InstructionItem *InsertInstruction(const std::string &domain_type, mvvm::SessionItem *parent,
                                   const mvvm::TagIndex &tag_index)
{
  return InsertInstruction(sequencergui::CreateInstructionItem(domain_type), parent, tag_index);
}

InstructionItem *InsertInstruction(std::unique_ptr<sequencergui::InstructionItem> item,
                                   mvvm::SessionItem *parent, const mvvm::TagIndex &tag_index)
{
  auto child_ptr = item.get();

  if (auto model = parent->GetModel(); model)
  {
    model->InsertItem(std::move(item), parent, tag_index);
  }
  else
  {
    parent->InsertItem(std::move(item), tag_index);
  }

  return child_ptr;
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

}  // namespace sequencergui
