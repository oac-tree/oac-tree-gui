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

#include "procedure_examples.h"

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/sequencer_item_includes.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/sequencer_utils.h>
#include <sequencergui/model/universal_item_helper.h>
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

namespace sequencergui::Examples
{

ProcedureItem* AddCopyProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto repeat = model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
  repeat->SetRepeatCount(-1);
  repeat->SetIsRootFlag(true);
  auto sequence = model->InsertItem<SequenceItem>(repeat);

  auto copy0 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("var_const42", copy0);
  SetOutput("var0", copy0);

  model->InsertItem<WaitItem>(sequence);

  auto message = model->InsertItem<MessageItem>(sequence);
  message->SetText("Hello World");

  auto copy1 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("var_const0", copy1);
  SetOutput("var1", copy1);

  model->InsertItem<WaitItem>(sequence);

  auto copy2 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("var_const42", copy2);
  SetOutput("var1", copy2);

  model->InsertItem<WaitItem>(sequence);

  auto copy3 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("var_const0", copy3);
  SetOutput("var0", copy3);

  model->InsertItem<WaitItem>(sequence);

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 0}, *var0);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 0}, *var1);

  auto var_const42 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var_const42->SetName("var_const42");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42}, *var1);

  auto var_const0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var_const0->SetName("var_const0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 0}, *var1);

  return procedure_item;
}

ProcedureItem* AddLocalIncludeProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  model->InsertItem<WaitItem>(sequence)->SetTimeout(0.5);
  model->InsertItem<WaitItem>(sequence)->SetTimeout(1.0);
  model->InsertItem<WaitItem>(sequence)->SetTimeout(0.5);

  auto repeat = model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
  repeat->SetRepeatCount(-1);
  repeat->SetIsRootFlag(true);
  auto include = model->InsertItem<IncludeItem>(repeat);
  include->SetPath("MySequence");

  return procedure_item;
}

ProcedureItem* AddInputProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  auto input0 = model->InsertItem<InputItem>(sequence);
  input0->SetTargetVariableName("var1");
  input0->SetDescription("Your ID");

  model->InsertItem<WaitItem>(sequence);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42}, *var1);

  procedure_item->SetDisplayName("UserInput");
  return procedure_item;
}

ProcedureItem* AddUserChoiceProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto userchoice = model->InsertItem<UserChoiceItem>(procedure_item->GetInstructionContainer());
  model->InsertItem<WaitItem>(userchoice);
  model->InsertItem<WaitItem>(userchoice);

  procedure_item->SetDisplayName("UserChoice");
  return procedure_item;
}

// creating sequence representing same alignment as in article
// https://rachel53461.wordpress.com/2014/04/20/algorithm-for-drawing-trees/

ProcedureItem* AddComplexAlignmentProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence_O = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer())
                        ->SetDisplayName("O");

  auto sequence_E = model->InsertItem<SequenceItem>(sequence_O)->SetDisplayName("E");
  auto wait_A = model->InsertItem<WaitItem>(sequence_E)->SetDisplayName("A");
  auto sequence_D = model->InsertItem<SequenceItem>(sequence_E)->SetDisplayName("D");
  auto wait_B = model->InsertItem<WaitItem>(sequence_D)->SetDisplayName("B");
  auto wait_C = model->InsertItem<WaitItem>(sequence_D)->SetDisplayName("C");

  auto wait_F = model->InsertItem<WaitItem>(sequence_O)->SetDisplayName("F");

  auto sequence_N = model->InsertItem<SequenceItem>(sequence_O)->SetDisplayName("N");
  auto wait_G = model->InsertItem<WaitItem>(sequence_N)->SetDisplayName("G");
  auto sequence_M = model->InsertItem<SequenceItem>(sequence_N)->SetDisplayName("M");
  auto wait_H = model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("H");
  auto wait_I = model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("I");
  auto wait_J = model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("J");
  auto wait_K = model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("K");
  auto wait_L = model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("L");

  return procedure_item;
}

ProcedureItem* AddComplexAlignmentProcedureV2(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());

  auto sequence0 = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  auto copy0 = InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  auto copy1 = InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  auto fallback0 = model->InsertItem<FallbackItem>(sequence0);

  auto listen0 = model->InsertItem<SequenceItem>(fallback0)->SetDisplayName("Listen");
  auto inverter0 = model->InsertItem<InverterItem>(listen0);
  auto fallback1 = model->InsertItem<FallbackItem>(inverter0);
  auto wait0 = model->InsertItem<WaitItem>(fallback1);
  auto wait1 = model->InsertItem<WaitItem>(fallback1);

  auto sequence1 = model->InsertItem<SequenceItem>(fallback0);
  auto force0 = model->InsertItem<ForceSuccessItem>(sequence1);
  auto copy2 = InsertInstruction(domainconstants::kCopyInstructionType, sequence0);
  auto copy3 = InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  auto sequence2 = model->InsertItem<SequenceItem>(force0);
  auto wait2 = model->InsertItem<WaitItem>(sequence2);
  auto wait3 = model->InsertItem<WaitItem>(sequence2);

  return procedure_item;
}

}  // namespace sequencergui::Examples
