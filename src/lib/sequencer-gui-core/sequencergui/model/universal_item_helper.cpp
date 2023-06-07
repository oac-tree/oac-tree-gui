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
#include <sequencergui/model/universal_instruction_item.h>

#include <mvvm/interfaces/sessionmodel_interface.h>

namespace sequencergui
{

UniversalInstructionItem *InsertInstruction(const std::string &domain_type,
                                            mvvm::SessionModelInterface *model)
{
  auto result = model->InsertItem<UniversalInstructionItem>();
  result->SetDomainType(domain_type);
  return result;
}

UniversalInstructionItem *InsertInstruction(const std::string &domain_type,
                                            mvvm::SessionItem *parent)
{
  auto model = parent->GetModel();
  if (!model)
  {
    throw RuntimeException("Item doesn't belong to the model");
  }

  auto result = model->InsertItem<UniversalInstructionItem>(parent);
  result->SetDomainType(domain_type);
  return result;
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

void SetOutputTarget(const std::string &value, InstructionItem *item)
{
  item->SetProperty(domainconstants::kOutputAttribute, value);
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
