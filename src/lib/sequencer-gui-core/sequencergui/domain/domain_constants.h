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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_

//! Collection of constants used by domain objects. The existence of this file (and unavoidable data
//! duplication) is dictated by the fact that many Sequencer's headers are not exposed, and many
//! constants are literals in code.

#include <string>

namespace sequencergui::domainconstants
{
// instructions
const std::string kChoiceInstructionType = "Choice";
const std::string kConditionInstructionType = "Condition";
const std::string kCopyInstructionType = "Copy";
const std::string kDecrementInstructionType = "Decrement";
const std::string kFallbackInstructionType = "Fallback";
const std::string kForceSuccessInstructionType = "ForceSuccess";
const std::string kIncludeInstructionType = "Include";
const std::string kIncrementInstructionType = "Increment";
const std::string kInputInstructionType = "Input";
const std::string kInverterInstructionType = "Inverter";
const std::string kListenInstructionType = "Listen";
const std::string kMessageInstructionType = "Message";
const std::string kOutputInstructionType = "Output";
const std::string kParallelInstructionType = "ParallelSequence";
const std::string kRepeatInstructionType = "Repeat";
const std::string kSequenceInstructionType = "Sequence";
const std::string kUserChoiceInstructionType = "UserChoice";
const std::string kVariableResetInstructionType = "ResetVariable";
const std::string kWaitInstructionType = "Wait";
const std::string kUserConfirmationInstructionType = "UserConfirmation";

// equality instructions
const std::string kLessThanInstructionType = "LessThan";
const std::string kLessThanOrEqualInstructionType = "LessThanOrEqual";
const std::string kEqualsInstructionType = "Equals";
const std::string kGreaterThanOrEqualInstructionType = "GreaterThanOrEqual";
const std::string kGreaterThanInstructionType = "GreaterThan";

// sup-sequencer-plugin-epics instructions
const std::string kChannelAccessReadInstructionType = "ChannelAccessRead";
const std::string kChannelAccessWriteInstructionType = "ChannelAccessWrite";
const std::string kPvAccessReadInstructionType = "PvAccessRead";
const std::string kPvAccessWriteInstructionType = "PvAccessWrite";
const std::string kRPCClientInstructionType = "RPCClient";
const std::string kSystemCallInstructionType = "SystemCall";
const std::string kLogInstructionType = "Log";

// sup-sequencer-plugin-control instructions
const std::string kWaitForConditionInstructionType = "WaitForCondition";
const std::string kExecuteWhileInstructionType = "ExecuteWhileInstruction";
const std::string kAchieveConditionInstructionType = "AchieveConditionInstruction";

// variables
const std::string kLocalVariableType = "Local";
const std::string kFileVariableType = "File";
const std::string kChannelAccessVariableType = "ChannelAccessClient";
const std::string kPvAccessClientVariableType = "PvAccessClient";
const std::string kPvAccessServerVariableType = "PvAccessServer";
const std::string kSystemClockVariableType = "SystemClock";

// attributes
const std::string kChannelAttribute = "channel";
const std::string kDescriptionAttribute = "description";
const std::string kFailureThresholdAttribute = "failureThreshold";
const std::string kFileNameAttribute = "file";
const std::string kInputAttribute = "inputVar";
const std::string kIsRootAttribute = "isRoot";
const std::string kLeftHandAttribute = "leftVar";
const std::string kMaxCountAttribute = "maxCount";
const std::string kNameAttribute = "name";
const std::string kOutputAttribute = "outputVar";
const std::string kPathAttribute = "path";
const std::string kRightHandAttribute = "rightVar";
const std::string kSuccessThresholdAttribute = "successThreshold";
const std::string kTextAttribute = "text";
const std::string kTickTimeOutAttribute = "tickTimeout";
const std::string kTimeoutAttribute = "timeout";
const std::string kTypeAttribute = "type";
const std::string kValueAttribute = "value";
const std::string kVarNameAttribute = "varName";
const std::string kServiceAttribute = "service";
const std::string kRequestAttribute = "requestVar";
const std::string kCommandAttribute = "command";
const std::string kSeverityAttribute = "severity";
const std::string kMessageAttribute = "message";
const std::string kPrettyJsonAttribute = "pretty";
const std::string kDynamicTypeAttribute = "dynamicType";

}  // namespace sequencergui::domainconstants

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
