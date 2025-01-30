/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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
//! duplication) is dictated by the fact that many oac-tree's headers are not exposed, and many
//! constants are literals in code.

#include <sup/oac-tree/constants.h>

#include <string>

namespace oac_tree_gui::domainconstants
{

// plugins

//!< fictional name when only sequencer core libs are loaded
const std::string kCorePluginName = "";

const std::string kEpicsCAPluginName = "liboac-tree-ca.so";
const std::string kEpicsPVXSPluginName = "liboac-tree-pvxs.so";
const std::string kEpicsMiscPluginName = "liboac-tree-misc.so";
const std::string kControlPluginName = "liboac-tree-control.so";
const std::string kSupConfigPluginName = "libsequencer-sup-config.so";
const std::string kSupTimingPluginName = "libsequencer-sup-timing.so";
const std::string kSupPulseCounterPluginName = "libsequencer-sup-pulse-counter.so";
const std::string kMathExprPluginName = "liboac-tree-mathexpr.so";
const std::string kSystemPluginName = "liboac-tree-system.so";
const std::string kStringPluginName = "liboac-tree-strings.so";

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
const std::string kDescriptionAttribute = sup::oac_tree::Constants::DESCRIPTION_ATTRIBUTE_NAME;
const std::string kFailureThresholdAttribute =
    sup::oac_tree::Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME;
const std::string kFileNameAttribute = sup::oac_tree::Constants::FILENAME_ATTRIBUTE_NAME;
const std::string kInputVariableNameAttribute =
    sup::oac_tree::Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kIsRootAttribute = sup::oac_tree::Constants::IS_ROOT_ATTRIBUTE_NAME;
const std::string kLeftVariableNameAttribute =
    sup::oac_tree::Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kMaxCountAttribute = sup::oac_tree::Constants::MAX_COUNT_ATTRIBUTE_NAME;
const std::string kNameAttribute = sup::oac_tree::Constants::NAME_ATTRIBUTE_NAME;
const std::string kOutputVariableNameAttribute =
    sup::oac_tree::Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kPathAttribute = sup::oac_tree::Constants::PATH_ATTRIBUTE_NAME;
const std::string kRightVariableNameAttribute =
    sup::oac_tree::Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kSuccessThresholdAttribute =
    sup::oac_tree::Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME;
const std::string kTextAttribute = sup::oac_tree::Constants::TEXT_ATTRIBUTE_NAME;
const std::string kTickTimeOutAttribute = "tickTimeout";
const std::string kTimeoutAttribute = sup::oac_tree::Constants::TIMEOUT_SEC_ATTRIBUTE_NAME;
const std::string kTypeAttribute = sup::oac_tree::Constants::TYPE_ATTRIBUTE_NAME;
const std::string kValueAttribute = sup::oac_tree::Constants::VALUE_ATTRIBUTE_NAME;
const std::string kGenericVariableNameAttribute =
    sup::oac_tree::Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME;
const std::string kServiceAttribute = "service";
const std::string kRequestAttribute = "requestVar";
const std::string kCommandAttribute = "command";
const std::string kSeverityAttribute = sup::oac_tree::Constants::SEVERITY_ATTRIBUTE_NAME;
const std::string kMessageAttribute = sup::oac_tree::Constants::MESSAGE_ATTRIBUTE_NAME;
const std::string kPrettyJsonAttribute = sup::oac_tree::Constants::PRETTY_JSON_ATTRIBUTE_NAME;
const std::string kDynamicTypeAttribute = sup::oac_tree::Constants::IS_DYNAMIC_TYPE_ATTRIBUTE_NAME;
const std::string kShowCollapsedAttribute =
    sup::oac_tree::Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME;

}  // namespace oac_tree_gui::domainconstants

#endif  // SEQUENCERGUI_DOMAIN_DOMAIN_CONSTANTS_H_
