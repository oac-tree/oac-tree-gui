/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/sequencerutils.h"

#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/taggeditems.h"
#include "mvvm/utils/containerutils.h"

#include <vector>

namespace
{

// FIXME remove duplication with InstructionItem.cpp
const std::string kChildInstructions = "kChildInstructions";

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{sequi::DomainConstants::kForceSuccessInstructionType,
                                  sequi::DomainConstants::kIncludeInstructionType,
                                  sequi::DomainConstants::kRepeatInstructionType,
                                  sequi::DomainConstants::kInverterInstructionType};
  return result;
}
}  // namespace

namespace sequi
{

bool IsCompoundInstruction(const InstructionItem *instruction)
{
  return instruction->GetTaggedItems()->HasTag(kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem *instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return ModelView::Utils::Contains(domain_names, instruction->GetDomainType());
}

}  // namespace sequi
