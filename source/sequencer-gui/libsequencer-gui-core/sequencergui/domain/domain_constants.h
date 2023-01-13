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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_

//! @file domain_constants.h
//! Collection of constants used by domain objects. The existence of this file (and unavoidable data
//! duplication) is dictated by the fact that many Sequencer's headers are not exposed, and many
//! constants are literals in code.

#include <string>

namespace sequencergui::domainconstants
{
// instructions
const std::string kConditionInstructionType = "Condition";
const std::string kCopyInstructionType = "Copy";
const std::string kEqualsInstructionType = "Equals";
const std::string kFallbackInstructionType = "Fallback";
const std::string kForceSuccessInstructionType = "ForceSuccess";
const std::string kIncludeInstructionType = "Include";
const std::string kInputInstructionType = "Input";
const std::string kInverterInstructionType = "Inverter";
const std::string kListenInstructionType = "Listen";
const std::string kMessageInstructionType = "Message";
const std::string kOutputInstructionType = "Output";
const std::string kParallelInstructionType = "ParallelSequence";
const std::string kRepeatInstructionType = "Repeat";
const std::string kSequenceInstructionType = "Sequence";
const std::string kUserChoiceInstructionType = "UserChoice";
const std::string kWaitInstructionType = "Wait";

// variables
const std::string kLocalVariableType = "Local";
const std::string kFileVariableType = "File";
const std::string kChannelAccessVariableType = "ChannelAccessClient";
const std::string kPVClientVariableType = "PvAccessClient";
const std::string kPVServerVariableType = "PVServerVariable";

// attributes
const std::string kChannelAttribute = "channel";
const std::string kDescriptionAttribute = "description";
const std::string kFailureThresholdAttribute = "failureThreshold";
const std::string kFileAttribute = "file";
const std::string kInputAttribute = "input";
const std::string kIsRootAttribute = "isRoot";
const std::string kMaxCountAttribute = "maxCount";
const std::string kNameAttribute = "name";
const std::string kOutputAttribute = "output";
const std::string kPathAttribute = "path";
const std::string kSuccessThresholdAttribute = "successThreshold";
const std::string kTextAttribute = "text";
const std::string kTickTimeOutAttribute = "tickTimeout";
const std::string kTypeAttribute = "type";
const std::string kValueAttribute = "value";
const std::string kLeftHandAttribute = "lhs";
const std::string kRightHandAttribute = "rhs";
const std::string kInstanceAttribute = "instance";

// attributes (instruction specific)
const std::string kConditionVarNameAttribute = "varName";
const std::string kInputTargetAttribute = "output";
const std::string kOutputSourceAttribute = "from";
const std::string kWaitTimeoutAttribute = "timeout";
const std::string kListenVarNamesAttribute = "varNames";
const std::string kListenForceSuccessAttribute = "forceSuccess";

}  // namespace sequencergui::domainconstants

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
