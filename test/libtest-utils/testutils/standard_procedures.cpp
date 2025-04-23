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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "standard_procedures.h"

#include "test_utils.h"

#include <oac_tree_gui/domain/domain_helper.h>

#include <sup/oac-tree/execution_status.h>
#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/procedure.h>
#include <sup/oac-tree/sequence_parser.h>
#include <sup/oac-tree/variable.h>

namespace oac_tree_gui::test
{

std::unique_ptr<procedure_t> CreateSingleWaitProcedure(std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<procedure_t>();
  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout)));  // expects in sec
  result->PushInstruction(std::move(wait0));
  result->AddAttribute(domainconstants::kTickTimeOutAttribute, "0.01");  // 10 msec
  return result;
}

std::unique_ptr<procedure_t> CreateCopyProcedure()
{
  auto result = std::make_unique<procedure_t>();

  auto copy = CreateDomainInstruction(domainconstants::kCopyInstructionType);
  copy->AddAttribute(domainconstants::kInputVariableNameAttribute, "var0");
  copy->AddAttribute(domainconstants::kOutputVariableNameAttribute, "var1");
  result->PushInstruction(std::move(copy));

  auto var0 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "42");
  result->AddVariable("var0", std::move(var0));

  auto var1 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var1->AddAttribute(domainconstants::kNameAttribute, "var1");
  var1->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var1->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var1", std::move(var1));

  return result;
}

std::unique_ptr<procedure_t> CreateMessageProcedure(const std::string &text)
{
  auto result = std::make_unique<procedure_t>();
  auto message = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message->AddAttribute(oac_tree_gui::domainconstants::kNameAttribute,
                        domainconstants::kMessageInstructionType);
  message->AddAttribute(oac_tree_gui::domainconstants::kTextAttribute, text);
  result->PushInstruction(std::move(message));

  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithWaitProcedure(std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout)));  // expects in sec

  sequence->InsertInstruction(std::move(wait0), 0);

  result->PushInstruction(std::move(sequence));
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithTwoWaitsProcedure(std::chrono::milliseconds timeout1,
                                                                 std::chrono::milliseconds timeout2)
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(oac_tree_gui::domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout1)));
  auto wait1 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(oac_tree_gui::domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout2)));

  sequence->InsertInstruction(std::move(wait0), 0);
  sequence->InsertInstruction(std::move(wait1), 1);

  result->PushInstruction(std::move(sequence));
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithSingleMessageProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto message0 = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message0->AddAttribute(oac_tree_gui::domainconstants::kTextAttribute, "abc");

  sequence->InsertInstruction(std::move(message0), 0);

  result->PushInstruction(std::move(sequence));
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithTwoMessagesProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto message0 = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message0->AddAttribute(oac_tree_gui::domainconstants::kTextAttribute, "abc");
  auto message1 = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message1->AddAttribute(oac_tree_gui::domainconstants::kTextAttribute, "efg");

  sequence->InsertInstruction(std::move(message0), 0);
  sequence->InsertInstruction(std::move(message1), 1);

  result->PushInstruction(std::move(sequence));
  return result;
}

std::unique_ptr<procedure_t> CreateInputProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto input = CreateDomainInstruction(domainconstants::kInputInstructionType);
  input->AddAttribute(domainconstants::kOutputVariableNameAttribute, "var0");
  input->AddAttribute(domainconstants::kDescriptionAttribute, "description");
  sequence->InsertInstruction(std::move(input), 0);
  result->PushInstruction(std::move(sequence));

  auto var0 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var0", std::move(var0));

  return result;
}

std::unique_ptr<procedure_t> CreateUserChoiceProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto userchoice = CreateDomainInstruction(domainconstants::kUserChoiceInstructionType);
  userchoice->AddAttribute(domainconstants::kDescriptionAttribute, "it's your choice");

  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(oac_tree_gui::domainconstants::kTimeoutAttribute, "10");  // 10 sec

  auto copy = CreateDomainInstruction(domainconstants::kCopyInstructionType);
  copy->AddAttribute(oac_tree_gui::domainconstants::kInputVariableNameAttribute, "var0");
  copy->AddAttribute(oac_tree_gui::domainconstants::kOutputVariableNameAttribute, "var1");

  userchoice->InsertInstruction(std::move(wait0), 0);
  userchoice->InsertInstruction(std::move(copy), 1);

  auto var0 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "42");
  result->AddVariable("var0", std::move(var0));

  auto var1 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var1->AddAttribute(domainconstants::kNameAttribute, "var1");
  var1->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var1->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var1", std::move(var1));

  result->PushInstruction(std::move(userchoice));
  return result;
}

std::unique_ptr<procedure_t> CreateRepeatSequenceProcedure(int count,
                                                           std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<procedure_t>();
  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(domainconstants::kMaxCountAttribute,
                       std::to_string(count));  // number of prepetitions

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto wait0 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout)));  // expects in sec

  sequence->InsertInstruction(std::move(wait0), 0);
  repeat->InsertInstruction(std::move(sequence), 0);

  result->PushInstruction(std::move(repeat));
  return result;
}

std::unique_ptr<procedure_t> CreateLocalIncludeProcedure()
{
  // <Sequence name="CountTwice">
  //     <Wait/>
  // </Sequence>
  // <Repeat isRoot="true" maxCount="10">
  //     <Include name="Counts" path="CountTwice"/>
  // </Repeat>

  auto result = std::make_unique<procedure_t>();

  // Sequence with wait instruction
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(oac_tree_gui::domainconstants::kTimeoutAttribute, "42");

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  sequence->AddAttribute(oac_tree_gui::domainconstants::kNameAttribute, "MySequence");
  sequence->InsertInstruction(std::move(wait), 0);

  // Repeat with include instruction
  auto include = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  include->AddAttribute(oac_tree_gui::domainconstants::kNameAttribute, "MyInclude");
  include->AddAttribute(oac_tree_gui::domainconstants::kPathAttribute, "MySequence");

  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(oac_tree_gui::domainconstants::kIsRootAttribute, "true");
  repeat->AddAttribute(oac_tree_gui::domainconstants::kMaxCountAttribute, "10");
  repeat->InsertInstruction(std::move(include), 0);

  // procedure with two instructions
  result->PushInstruction(std::move(sequence));
  result->PushInstruction(std::move(repeat));

  return result;
}

std::unique_ptr<procedure_t> CreateCounterProcedure(int n_repetitions)
{
  auto result = std::make_unique<procedure_t>();

  auto increment = CreateDomainInstruction(domainconstants::kIncrementInstructionType);
  increment->AddAttribute(oac_tree_gui::domainconstants::kGenericVariableNameAttribute, "counter");

  // repeat instruction with increment inside
  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(oac_tree_gui::domainconstants::kMaxCountAttribute,
                       std::to_string(n_repetitions));
  repeat->InsertInstruction(std::move(increment), 0);

  result->PushInstruction(std::move(repeat));

  auto var0 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "counter");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("counter", std::move(var0));

  return result;
}

std::unique_ptr<procedure_t> CreateRepeatSequencerProcedure(int n_repetitions, int max_counter)
{
  auto result = std::make_unique<procedure_t>();

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);

  auto less_than = CreateDomainInstruction(domainconstants::kLessThanInstructionType);
  less_than->AddAttribute(oac_tree_gui::domainconstants::kLeftVariableNameAttribute, "counter");
  less_than->AddAttribute(oac_tree_gui::domainconstants::kRightVariableNameAttribute,
                          "max_counter");
  sequence->InsertInstruction(std::move(less_than), 0);

  auto increment = CreateDomainInstruction(domainconstants::kIncrementInstructionType);
  increment->AddAttribute(oac_tree_gui::domainconstants::kGenericVariableNameAttribute, "counter");
  sequence->InsertInstruction(std::move(increment), 0);

  // repeat instruction with increment inside
  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(oac_tree_gui::domainconstants::kMaxCountAttribute,
                       std::to_string(n_repetitions));
  repeat->InsertInstruction(std::move(sequence), 0);

  result->PushInstruction(std::move(repeat));

  auto var0 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "counter");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("counter", std::move(var0));

  auto var1 = CreateDomainVariable(domainconstants::kLocalVariableType);
  var1->AddAttribute(domainconstants::kNameAttribute, "max_counter");
  var1->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var1->AddAttribute(domainconstants::kValueAttribute, std::to_string(max_counter));
  result->AddVariable("max_counter", std::move(var1));

  return result;
}

std::unique_ptr<procedure_t> CreateRepeatIncrementAndCompare()
{
  // Will continue incrementing, while to_continue variable is equal to one.
  const std::string procedure_xml = R"RAW(
<Procedure>
  <Repeat maxCount="-1">
    <Sequence>
      <Equals leftVar="to_continue" rightVar="one"/>
      <Increment varName="counter"/>
    </Sequence>
  </Repeat>
  <Workspace>
    <Local name="counter" type='{"type":"int32"}' value="0"/>
    <Local name="one" type='{"type":"int32"}' value="1"/>
    <Local name="to_continue" type='{"type":"int32"}' value="1"/>
  </Workspace>
</Procedure>
)RAW";
  return sup::oac_tree::ParseProcedureString(procedure_xml);
}

}  // namespace testutils
