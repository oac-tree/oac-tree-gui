/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_CONSTANTS_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_CONSTANTS_H_

//! Collection of constants used by domain objects. The existence of this file (and unavoidable data
//! duplication) is dictated by the fact that many oac-tree's headers are not exposed, and many
//! constants are literals in code.

namespace oac_tree_gui::domainconstants
{

// plugins

//!< fictional name when only sequencer core libs are loaded
constexpr auto kCorePluginName = "";

//!< Plugin names must coincide with shared plugin library names, with preffix "lib" and suffixes
//! ".so" or ".dylib" stripped off. Windows is not yet supported.
constexpr auto kEpicsCAPluginName = "oac-tree-ca";
constexpr auto kEpicsPVXSPluginName = "oac-tree-pvxs";
constexpr auto kControlPluginName = "oac-tree-control";
constexpr auto kSupConfigPluginName = "sequencer-sup-config";
constexpr auto kSupTimingPluginName = "sequencer-sup-timing";
constexpr auto kSupPulseCounterPluginName = "sequencer-sup-pulse-counter";
constexpr auto kMathExprPluginName = "oac-tree-mathexpr";
constexpr auto kSystemPluginName = "oac-tree-system";
constexpr auto kStringPluginName = "oac-tree-strings";

// instructions
constexpr auto kChoiceInstructionType = "Choice";
constexpr auto kConditionInstructionType = "Condition";
constexpr auto kCopyInstructionType = "Copy";
constexpr auto kDecrementInstructionType = "Decrement";
constexpr auto kFallbackInstructionType = "Fallback";
constexpr auto kForceSuccessInstructionType = "ForceSuccess";
constexpr auto kIncludeInstructionType = "Include";
constexpr auto kIncludeProcedureInstructionType = "IncludeProcedure";
constexpr auto kIncrementInstructionType = "Increment";
constexpr auto kInputInstructionType = "Input";
constexpr auto kInverterInstructionType = "Inverter";
constexpr auto kListenInstructionType = "Listen";
constexpr auto kMessageInstructionType = "Message";
constexpr auto kOutputInstructionType = "Output";
constexpr auto kParallelInstructionType = "ParallelSequence";
constexpr auto kRepeatInstructionType = "Repeat";
constexpr auto kSequenceInstructionType = "Sequence";
constexpr auto kUserChoiceInstructionType = "UserChoice";
constexpr auto kVariableResetInstructionType = "ResetVariable";
constexpr auto kWaitInstructionType = "Wait";
constexpr auto kUserConfirmationInstructionType = "UserConfirmation";
constexpr auto kSucceedInstructionType = "Succeed";
constexpr auto kFailedInstructionType = "Fail";
constexpr auto kLogInstructionType = "Log";

// equality instructions
constexpr auto kLessThanInstructionType = "LessThan";
constexpr auto kLessThanOrEqualInstructionType = "LessThanOrEqual";
constexpr auto kEqualsInstructionType = "Equals";
constexpr auto kGreaterThanOrEqualInstructionType = "GreaterThanOrEqual";
constexpr auto kGreaterThanInstructionType = "GreaterThan";

// sup-sequencer-plugin-epics instructions
constexpr auto kChannelAccessReadInstructionType = "ChannelAccessRead";
constexpr auto kChannelAccessWriteInstructionType = "ChannelAccessWrite";
constexpr auto kPvAccessReadInstructionType = "PvAccessRead";
constexpr auto kPvAccessWriteInstructionType = "PvAccessWrite";
constexpr auto kRPCClientInstructionType = "RPCClient";

// sup-sequencer-plugin-control instructions
constexpr auto kWaitForConditionInstructionType = "WaitForCondition";
constexpr auto kExecuteWhileInstructionType = "ExecuteWhileInstruction";
constexpr auto kAchieveConditionInstructionType = "AchieveConditionInstruction";

// variables
constexpr auto kLocalVariableType = "Local";
constexpr auto kFileVariableType = "File";
constexpr auto kChannelAccessVariableType = "ChannelAccessClient";
constexpr auto kPvAccessClientVariableType = "PvAccessClient";
constexpr auto kPvAccessServerVariableType = "PvAccessServer";
constexpr auto kSystemClockVariableType = "SystemClock";

// attributes
constexpr auto kChannelAttribute = "channel";
constexpr auto kDescriptionAttribute = "description";
constexpr auto kFailureThresholdAttribute = "failureThreshold";
constexpr auto kFileNameAttribute = "file";
constexpr auto kInputVariableNameAttribute = "inputVar";
constexpr auto kIsRootAttribute = "isRoot";
constexpr auto kLeftVariableNameAttribute = "leftVar";
constexpr auto kMaxCountAttribute = "maxCount";
constexpr auto kNameAttribute = "name";
constexpr auto kOutputVariableNameAttribute = "outputVar";
constexpr auto kPathAttribute = "path";
constexpr auto kRightVariableNameAttribute = "rightVar";
constexpr auto kSuccessThresholdAttribute = "successThreshold";
constexpr auto kTextAttribute = "text";
constexpr auto kTickTimeOutAttribute = "tickTimeout";
constexpr auto kTimeoutAttribute = "timeout";
constexpr auto kTypeAttribute = "type";
constexpr auto kValueAttribute = "value";
constexpr auto kGenericVariableNameAttribute = "varName";
constexpr auto kServiceAttribute = "service";
constexpr auto kRequestAttribute = "requestVar";
constexpr auto kCommandAttribute = "command";
constexpr auto kSeverityAttribute = "severity";
constexpr auto kMessageAttribute = "message";
constexpr auto kPrettyJsonAttribute = "pretty";
constexpr auto kDynamicTypeAttribute = "dynamicType";
constexpr auto kShowCollapsedAttribute = "showCollapsed";
constexpr auto kFromVariableAttributeName = "fromVar";
constexpr auto kEqualVariableAttributeName = "equalsVar";
constexpr auto kBlockingAttribute = "blocking";

}  // namespace oac_tree_gui::domainconstants

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_CONSTANTS_H_
