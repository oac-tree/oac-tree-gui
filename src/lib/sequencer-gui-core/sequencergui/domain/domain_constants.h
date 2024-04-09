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

#ifndef SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
#define SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_

//! Collection of constants used by domain objects. The existence of this file (and unavoidable data
//! duplication) is dictated by the fact that many Sequencer's headers are not exposed, and many
//! constants are literals in code.

#include <sup/sequencer/constants.h>

#include <string>

namespace sequencergui::domainconstants
{

// plugins
const std::string kCorePluginName = ""; //!< fictional name when only sequencer core libs are loaded

// instructions
const std::string kChoiceInstructionType = "Choice";
const std::string kConditionInstructionType = "Condition";
const std::string kCopyInstructionType = "Copy";
const std::string kDecrementInstructionType = "Decrement";
const std::string kFallbackInstructionType = "Fallback";
const std::string kForceSuccessInstructionType = "ForceSuccess";
const std::string kIncludeInstructionType = "Include";
const std::string kIncludeProcedureInstructionType = "IncludeProcedure";
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
const std::string kDescriptionAttribute = sup::sequencer::Constants::DESCRIPTION_ATTRIBUTE_NAME;
const std::string kFailureThresholdAttribute =
    sup::sequencer::Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME;
const std::string kFileNameAttribute = sup::sequencer::Constants::FILENAME_ATTRIBUTE_NAME;
const std::string kInputVariableNameAttribute =
    sup::sequencer::Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kIsRootAttribute = sup::sequencer::Constants::IS_ROOT_ATTRIBUTE_NAME;
const std::string kLeftVariableNameAttribute =
    sup::sequencer::Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kMaxCountAttribute = sup::sequencer::Constants::MAX_COUNT_ATTRIBUTE_NAME;
const std::string kNameAttribute = sup::sequencer::Constants::NAME_ATTRIBUTE_NAME;
const std::string kOutputVariableNameAttribute =
    sup::sequencer::Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kPathAttribute = sup::sequencer::Constants::PATH_ATTRIBUTE_NAME;
const std::string kRightVariableNameAttribute =
    sup::sequencer::Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kSuccessThresholdAttribute =
    sup::sequencer::Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME;
const std::string kTextAttribute = sup::sequencer::Constants::TEXT_ATTRIBUTE_NAME;
const std::string kTickTimeOutAttribute = "tickTimeout";
const std::string kTimeoutAttribute = sup::sequencer::Constants::TIMEOUT_SEC_ATTRIBUTE_NAME;
const std::string kTypeAttribute = sup::sequencer::Constants::TYPE_ATTRIBUTE_NAME;
const std::string kValueAttribute = sup::sequencer::Constants::VALUE_ATTRIBUTE_NAME;
const std::string kGenericVariableNameAttribute =
    sup::sequencer::Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kServiceAttribute = "service";
const std::string kRequestAttribute = "requestVar";
const std::string kCommandAttribute = "command";
const std::string kSeverityAttribute = sup::sequencer::Constants::SEVERITY_ATTRIBUTE_NAME;
const std::string kMessageAttribute = sup::sequencer::Constants::MESSAGE_ATTRIBUTE_NAME;
const std::string kPrettyJsonAttribute = sup::sequencer::Constants::PRETTY_JSON_ATTRIBUTE_NAME;
const std::string kDynamicTypeAttribute = sup::sequencer::Constants::IS_DYNAMIC_TYPE_ATTRIBUTE_NAME;
const std::string kShowCollapsedAttribute =
    sup::sequencer::Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME;

}  // namespace sequencergui::domainconstants

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
