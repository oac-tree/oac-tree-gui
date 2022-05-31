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

#include "test_procedures.h"

#include "ExecutionStatus.h"
#include "Instruction.h"
#include "Procedure.h"
#include "Variable.h"
#include "sequencergui/model/domainutils.h"

using namespace sequencergui;

namespace
{
std::string GetTimeoutInSec(std::chrono::milliseconds timeout)
{
  auto value = std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
  return std::to_string(static_cast<double>(value) / 1000);
}
}  // namespace

namespace testutils
{

std::unique_ptr<procedure_t> CreateSingleWaitProcedure(std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<procedure_t>();
  auto wait0 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kWaitTimeoutAttribute,
                      GetTimeoutInSec(timeout));  // expects in sec
  result->PushInstruction(wait0.release());
  result->AddAttribute(domainconstants::kTickTimeOutAttribute, "0.01");  // 10 msec
  return result;
}

std::unique_ptr<procedure_t> CreateCopyProcedure()
{
  auto result = std::make_unique<procedure_t>();

  auto copy = DomainUtils::CreateDomainInstruction(domainconstants::kCopyInstructionType);
  copy->AddAttribute(domainconstants::kInputAttribute, "var0");
  copy->AddAttribute(domainconstants::kOutputAttribute, "var1");
  result->PushInstruction(copy.release());

  auto var0 = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "42");
  result->AddVariable("var0", var0.release());

  auto var1 = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  var1->AddAttribute(domainconstants::kNameAttribute, "var1");
  var1->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var1->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var1", var1.release());

  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithWaitProcedure(std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto wait0 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(domainconstants::kWaitTimeoutAttribute,
                      GetTimeoutInSec(timeout));  // expects in sec

  sequence->InsertInstruction(wait0.release(), 0);

  result->PushInstruction(sequence.release());
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithTwoWaitsProcedure(std::chrono::milliseconds timeout1,
                                                                 std::chrono::milliseconds timeout2)
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto wait0 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute,
                      GetTimeoutInSec(timeout1));
  auto wait1 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait1->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute,
                      GetTimeoutInSec(timeout2));

  sequence->InsertInstruction(wait0.release(), 0);
  sequence->InsertInstruction(wait1.release(), 1);

  result->PushInstruction(sequence.release());
  return result;
}

std::unique_ptr<procedure_t> CreateSequenceWithTwoMessagesProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto message0 = DomainUtils::CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message0->AddAttribute(sequencergui::domainconstants::kTextAttribute, "abc");
  auto message1 = DomainUtils::CreateDomainInstruction(domainconstants::kMessageInstructionType);
  message1->AddAttribute(sequencergui::domainconstants::kTextAttribute, "efg");

  sequence->InsertInstruction(message0.release(), 0);
  sequence->InsertInstruction(message1.release(), 1);

  result->PushInstruction(sequence.release());
  return result;
}

std::unique_ptr<procedure_t> CreateInputProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto sequence = DomainUtils::CreateDomainInstruction(domainconstants::kSequenceInstructionType);
  auto input = DomainUtils::CreateDomainInstruction(domainconstants::kInputInstructionType);
  input->AddAttribute(domainconstants::kInputTargetAttribute, "var0");
  input->AddAttribute(domainconstants::kDescriptionAttribute, "description");
  sequence->InsertInstruction(input.release(), 0);
  result->PushInstruction(sequence.release());

  auto var0 = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var0", var0.release());

  return result;
}

std::unique_ptr<procedure_t> CreateUserChoiceProcedure()
{
  auto result = std::make_unique<procedure_t>();
  auto userchoice =
      DomainUtils::CreateDomainInstruction(domainconstants::kUserChoiceInstructionType);
  userchoice->AddAttribute(domainconstants::kDescriptionAttribute, "it's your choice");

  auto wait0 = DomainUtils::CreateDomainInstruction(domainconstants::kWaitInstructionType);
  wait0->AddAttribute(sequencergui::domainconstants::kWaitTimeoutAttribute, "10");  // 10 sec

  auto copy = DomainUtils::CreateDomainInstruction(domainconstants::kCopyInstructionType);
  copy->AddAttribute(sequencergui::domainconstants::kInputAttribute, "var0");
  copy->AddAttribute(sequencergui::domainconstants::kOutputAttribute, "var1");

  userchoice->InsertInstruction(wait0.release(), 0);
  userchoice->InsertInstruction(copy.release(), 1);

  auto var0 = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  var0->AddAttribute(domainconstants::kNameAttribute, "var0");
  var0->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var0->AddAttribute(domainconstants::kValueAttribute, "42");
  result->AddVariable("var0", var0.release());

  auto var1 = DomainUtils::CreateDomainVariable(domainconstants::kLocalVariableType);
  var1->AddAttribute(domainconstants::kNameAttribute, "var1");
  var1->AddAttribute(domainconstants::kTypeAttribute, R"RAW({"type":"uint32"})RAW");
  var1->AddAttribute(domainconstants::kValueAttribute, "0");
  result->AddVariable("var1", var1.release());

  result->PushInstruction(userchoice.release());
  return result;
}

}  // namespace testutils
