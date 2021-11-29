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

#ifndef SEQUENCERGUI_MODEL_DOMAIN_CONSTANTS_H
#define SEQUENCERGUI_MODEL_DOMAIN_CONSTANTS_H

//! @file domain_constants.h
//! Collection of constants used by domain objects. The existence of this file (and unavoidable data
//! duplication) is dictated by the fact that many Sequencer's headers are not exposed, and many
//! constants are literals in code.

#include <string>

namespace sequi::DomainConstants
{
// instructions
const std::string kCopyInstructionType = "Copy";
const std::string kFallbackInstructionType = "Fallback";
const std::string kInputInstructionType = "Input";
const std::string kInverterInstructionType = "Inverter";
const std::string kOutputInstructionType = "Output";
const std::string kSequenceInstructionType = "Sequence";
const std::string kWaitInstructionType = "Wait";
const std::string kRepeatInstructionType = "Repeat";

// variables
const std::string kLocalVariableType = "Local";

// attributes
const std::string kNameAttribute = "name";
const std::string kDescriptionAttribute = "description";
const std::string kTypeAttribute = "type";
const std::string kValueAttribute = "value";
const std::string kMaxCountAttribute = "maxCount";
const std::string kInputAttribute = "input";
const std::string kOutputAttribute = "output";
const std::string kTickTimeOutAttribute = "tickTimeout";

// attributes (instruction specific)
const std::string kWaitTimeoutAttribute = "timeout";
const std::string kInputTargetAttribute = "output";
const std::string kOutputSourceAttribute = "from";

}  // namespace sequi::DomainConstants

#endif // SEQUENCERGUI_MODEL_DOMAIN_CONSTANTS_H
