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

#include "standard_procedure_items.h"

#include "test_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/universal_instruction_item.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

using namespace sequencergui;

namespace testutils
{

sequencergui::ProcedureItem* CreateInvalidProcedureItem(sequencergui::SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  // Uninitialised instruction doesn't have counterpart in SequencerDomain
  model->InsertItem<UniversalInstructionItem>(procedure_item->GetInstructionContainer());
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

  auto copy = InsertInstruction(domainconstants::kCopyInstructionType,
                                procedure_item->GetInstructionContainer());
  SetInput("var0", copy);
  SetOutput("var1", copy);

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

  auto input0 = InsertInstruction(domainconstants::kInputInstructionType, sequence);
  SetOutput("var1", input0);
  SetDescription("Your ID", input0);

  model->InsertItem<WaitItem>(sequence);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var1);

  return procedure_item;
}

ProcedureItem* CreateUserChoiceProcedureItem(SequencerModel* model)
{
  // UserChoice instruction will offer the user a choice between two instructions to execute
  // 0) Wait instruction
  // 1) Copy instruction
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto userchoice = InsertInstruction(domainconstants::kUserChoiceInstructionType,
                                      procedure_item->GetInstructionContainer());
  auto wait0 = model->InsertItem<WaitItem>(userchoice);
  wait0->SetTimeout(10.0);

  auto copy = InsertInstruction(domainconstants::kCopyInstructionType, userchoice);
  SetInput("var0", copy);
  SetOutput("var1", copy);

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42}, *var0);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 0}, *var1);

  return procedure_item;
}

ProcedureItem* CreateMessageProcedureItem(sequencergui::SequencerModel* model,
                                          const std::string& text)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto message = InsertInstruction(domainconstants::kMessageInstructionType,
                                   procedure_item->GetInstructionContainer());
  SetText(text, message);
  return procedure_item;
}

ProcedureItem* CreateVariableResetProcedureItem(sequencergui::SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());

  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());

  auto copy0 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("a", copy0);
  SetOutput("target", copy0);

  auto reset = InsertInstruction(domainconstants::kVariableResetInstructionType, sequence);
  SetVariableName("target", reset);

  auto copy1 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("b", copy1);
  SetOutput("target", copy1);

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("a");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var0);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("b");
  SetAnyValue(sup::dto::AnyValue{sup::dto::StringType, "abc"}, *var1);

  auto var2 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var2->SetName("target");

  return procedure_item;
}

}  // namespace testutils
