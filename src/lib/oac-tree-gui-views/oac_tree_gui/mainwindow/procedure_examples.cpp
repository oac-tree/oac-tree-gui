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

#include "procedure_examples.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/sequencer_item_helper.h>
#include <oac_tree_gui/model/sequencer_item_includes.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

namespace oac_tree_gui::Examples
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

  auto message = InsertInstruction(domainconstants::kMessageInstructionType, sequence);
  SetText("Hello World", message);

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

  auto input0 = InsertInstruction(domainconstants::kInputInstructionType, sequence);
  SetOutput("var1", input0);
  SetDescription("Your ID", input0);

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
  auto userchoice = InsertInstruction(domainconstants::kUserChoiceInstructionType,
                                      procedure_item->GetInstructionContainer());
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
  auto sequence_O = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence_O->SetDisplayName("O");

  auto sequence_E = model->InsertItem<SequenceItem>(sequence_O);
  sequence_E->SetDisplayName("E");
  auto wait_A = model->InsertItem<WaitItem>(sequence_E);
  wait_A->SetDisplayName("A");
  auto sequence_D = model->InsertItem<SequenceItem>(sequence_E);
  sequence_D->SetDisplayName("D");
  auto wait_B = model->InsertItem<WaitItem>(sequence_D);
  wait_B->SetDisplayName("B");
  auto wait_C = model->InsertItem<WaitItem>(sequence_D);
  wait_C->SetDisplayName("C");

  auto wait_F = model->InsertItem<WaitItem>(sequence_O);
  wait_F->SetDisplayName("F");

  auto sequence_N = model->InsertItem<SequenceItem>(sequence_O);
  sequence_N->SetDisplayName("N");
  auto wait_G = model->InsertItem<WaitItem>(sequence_N);
  wait_G->SetDisplayName("G");
  auto sequence_M = model->InsertItem<SequenceItem>(sequence_N);
  wait_G->SetDisplayName("M");
  model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("H");
  model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("I");
  model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("J");
  model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("K");
  model->InsertItem<WaitItem>(sequence_M)->SetDisplayName("L");

  return procedure_item;
}

ProcedureItem* AddComplexAlignmentProcedureV2(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());

  auto sequence0 = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  (void)InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  (void)InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  auto fallback0 = InsertInstruction(domainconstants::kFallbackInstructionType, sequence0);

  auto listen0 = model->InsertItem<SequenceItem>(fallback0);
  listen0->SetDisplayName("Listen");

  auto inverter0 = InsertInstruction(domainconstants::kInverterInstructionType, listen0);

  auto fallback1 = InsertInstruction(domainconstants::kFallbackInstructionType, inverter0);
  (void)model->InsertItem<WaitItem>(fallback1);
  (void)model->InsertItem<WaitItem>(fallback1);

  auto sequence1 = model->InsertItem<SequenceItem>(fallback0);
  auto force0 = InsertInstruction(domainconstants::kForceSuccessInstructionType, sequence1);
  (void)InsertInstruction(domainconstants::kCopyInstructionType, sequence0);
  (void)InsertInstruction(domainconstants::kCopyInstructionType, sequence0);

  auto sequence2 = model->InsertItem<SequenceItem>(force0);
  (void)model->InsertItem<WaitItem>(sequence2);
  (void)model->InsertItem<WaitItem>(sequence2);

  return procedure_item;
}

}  // namespace oac_tree_gui::Examples
