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

#include "standard_procedures.h"

#include "test_utils.h"

#include <sequencergui/domain/domain_utils.h>

#include <sup/sequencer/execution_status.h>
#include <sup/sequencer/instruction.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/variable.h>

using namespace sequencergui;

namespace testutils
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
  copy->AddAttribute(domainconstants::kInputAttribute, "var0");
  copy->AddAttribute(domainconstants::kOutputAttribute, "var1");
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
  message->AddAttribute(sequencergui::domainconstants::kTextAttribute, text);
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
  wait0->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute,
                      std::to_string(GetTimeoutInSec(timeout1)));
  auto wait1 = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute,
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
  message0->AddAttribute(sequencergui::domainconstants::kTextAttribute, "abc");

  sequence->InsertInstruction(std::move(message0), 0);

  result->PushInstruction(std::move(sequence));
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithTwoMessagesProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto message0 = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message0->AddAttribute(sequencergui::domainconstants::kTextAttribute, "abc");
  auto message1 = CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message1->AddAttribute(sequencergui::domainconstants::kTextAttribute, "efg");

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
  input->AddAttribute(domainconstants::kOutputAttribute, "var0");
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
  wait0->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "10");  // 10 sec

  auto copy = CreateDomainInstruction(domainconstants::kCopyInstructionType);
  copy->AddAttribute(sequencergui::domainconstants::kInputAttribute, "var0");
  copy->AddAttribute(sequencergui::domainconstants::kOutputAttribute, "var1");

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
  auto result = std::make_unique<procedure_t>();

  // Sequence with wait instruction
  auto wait = CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait->AddAttribute(sequencergui::domainconstants::kTimeoutAttribute, "42");

  auto sequence = CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  sequence->AddAttribute(sequencergui::domainconstants::kNameAttribute, "MySequence");
  sequence->InsertInstruction(std::move(wait), 0);

  // Repeat with include instruction
  auto include = CreateDomainInstruction(domainconstants::kIncludeInstructionType);
  include->AddAttribute(sequencergui::domainconstants::kNameAttribute, "MyInclude");
  include->AddAttribute(sequencergui::domainconstants::kPathAttribute, "MySequence");

  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(sequencergui::domainconstants::kIsRootAttribute, "true");
  repeat->AddAttribute(sequencergui::domainconstants::kMaxCountAttribute, "10");
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
  increment->AddAttribute(sequencergui::domainconstants::kVarNameAttribute, "counter");

  // repeat instruction with increment inside
  auto repeat = CreateDomainInstruction(domainconstants::kRepeatInstructionType);
  repeat->AddAttribute(sequencergui::domainconstants::kMaxCountAttribute,
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

}  // namespace testutils
