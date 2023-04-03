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

#include "standard_procedure_items.h"

#include "test_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_transform_helper.h>

#include <mvvm/standarditems/container_item.h>
#include <sup/dto/anyvalue.h>

using namespace sequencergui;

namespace testutils
{

sequencergui::ProcedureItem* CreateInvalidProcedureItem(sequencergui::SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  // InstructionItem that doesn't have counterpart in SequencerDomain
  auto item = model->InsertItem<UnknownInstructionItem>(procedure_item->GetInstructionContainer());
  return procedure_item;
}

ProcedureItem* CreateSingleWaitProcedureItem(SequencerModel* model,
                                             std::chrono::milliseconds timeout)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto wait = model->InsertItem<WaitItem>(procedure_item->GetInstructionContainer());
  wait->SetTimeout(GetTimeoutInSec(timeout));
  return procedure_item;
}

ProcedureItem* CreateCopyProcedureItem(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto copy = model->InsertItem<CopyItem>(procedure_item->GetInstructionContainer());
  copy->SetInput("var0");
  copy->SetOutput("var1");

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var0);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}, *var1);

  return procedure_item;
}

ProcedureItem* CreateIncludeProcedureItem(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  model->InsertItem<WaitItem>(sequence);

  auto repeat = model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
  repeat->SetRepeatCount(1);
  repeat->SetIsRootFlag(true);
  auto include = model->InsertItem<IncludeItem>(repeat);
  include->SetPath("MySequence");

  return procedure_item;
}

ProcedureItem* CreateInputProcedureItem(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  auto input0 = model->InsertItem<InputItem>(sequence);
  input0->SetTargetVariableName("var1");
  input0->SetDescription("Your ID");

  auto wait0 = model->InsertItem<WaitItem>(sequence);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var1);

  return procedure_item;
}

ProcedureItem* CreateUserChoiceProcedureItem(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto userchoice = model->InsertItem<UserChoiceItem>(procedure_item->GetInstructionContainer());
  auto wait0 = model->InsertItem<WaitItem>(userchoice);
  wait0->SetTimeout(10.0);

  auto copy = model->InsertItem<CopyItem>(userchoice);
  copy->SetInput("var0");
  copy->SetOutput("var1");

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("var0");
  var0->SetJsonType(R"({"type":"uint32"})");
  var0->SetJsonValue("42");

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  var1->SetJsonType(R"({"type":"uint32"})");
  var1->SetJsonValue("0");

  return procedure_item;
}

ProcedureItem* CreateMessageProcedureItem(sequencergui::SequencerModel* model,
                                          const std::string& text)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto message = model->InsertItem<MessageItem>(procedure_item->GetInstructionContainer());
  message->SetText(text);
  return procedure_item;
}

}  // namespace testutils
