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

#include "sequencergui/model/procedureexamples.h"

#include "sequencergui/model/procedureitems.h"
#include "sequencergui/model/sequencermodel.h"
#include "mvvm/standarditems/containeritem.h"

namespace sequencergui::Examples
{

ProcedureItem* AddCopyProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto repeat = model->InsertItem<RepeatItem>(procedure_item->GetInstructionContainer());
  repeat->SetRepeatCount(-1);
  repeat->SetIsRootFlag(true);
  auto sequence = model->InsertItem<SequenceItem>(repeat);

  auto copy0 = model->InsertItem<CopyItem>(sequence);
  copy0->SetInput("var_const42");
  copy0->SetOutput("var0");

  model->InsertItem<WaitItem>(sequence);

  auto message = model->InsertItem<MessageItem>(sequence);
  message->SetText("Hello World");

  auto copy1 = model->InsertItem<CopyItem>(sequence);
  copy1->SetInput("var_const0");
  copy1->SetOutput("var1");

  model->InsertItem<WaitItem>(sequence);

  auto copy2 = model->InsertItem<CopyItem>(sequence);
  copy2->SetInput("var_const42");
  copy2->SetOutput("var1");
  model->InsertItem<WaitItem>(sequence);

  auto copy3 = model->InsertItem<CopyItem>(sequence);
  copy3->SetInput("var_const0");
  copy3->SetOutput("var0");
  model->InsertItem<WaitItem>(sequence);

  //    auto equals = m_model->InsertItem<EqualsItem>(sequence);
  //    equals->SetLeftHandSide("var_const42");
  //    equals->SetRightHandSide("var_const0");

  auto var0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var0->SetName("var0");
  var0->SetJsonType(R"({"type":"uint32"})");
  var0->SetJsonValue("0");

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  var1->SetJsonType(R"({"type":"uint32"})");
  var1->SetJsonValue("0");

  auto var_const42 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var_const42->SetName("var_const42");
  var_const42->SetJsonType(R"({"type":"uint32"})");
  var_const42->SetJsonValue("42");

  auto var_const0 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var_const0->SetName("var_const0");
  var_const0->SetJsonType(R"({"type":"uint32"})");
  var_const0->SetJsonValue("0");

  return procedure_item;
}

ProcedureItem* AddLocalIncludeProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  model->InsertItem<WaitItem>(sequence);
  model->InsertItem<WaitItem>(sequence);
  model->InsertItem<WaitItem>(sequence);
  model->InsertItem<WaitItem>(sequence);

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

  auto wait0 = model->InsertItem<WaitItem>(sequence);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  //  var1->SetJsonType(R"({"type":"string"})");
  //  var1->SetJsonValue(R"("abc")");
  var1->SetJsonType(R"({"type":"uint32"})");
  var1->SetJsonValue(R"(42)");

  procedure_item->SetDisplayName("UserInput");
  return procedure_item;
}

ProcedureItem* AddUserChoiceProcedure(SequencerModel* model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto userchoice = model->InsertItem<UserChoiceItem>(procedure_item->GetInstructionContainer());
  auto wait0 = model->InsertItem<WaitItem>(userchoice);
  auto wait1 = model->InsertItem<WaitItem>(userchoice);

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
  auto copy0 = model->InsertItem<CopyItem>(sequence0);
  auto copy1 = model->InsertItem<CopyItem>(sequence0);
  auto fallback0 = model->InsertItem<FallbackItem>(sequence0);

  auto listen0 = model->InsertItem<UnknownInstructionItem>(fallback0)->SetDisplayName("Listen");
  auto inverter0 = model->InsertItem<InverterItem>(listen0);
  auto fallback1 = model->InsertItem<FallbackItem>(inverter0);
  auto equals0 = model->InsertItem<EqualsItem>(fallback1);
  auto equals1 = model->InsertItem<EqualsItem>(fallback1);

  auto sequence1 = model->InsertItem<SequenceItem>(fallback0);
  auto force0 = model->InsertItem<ForceSuccessItem>(sequence1);
  auto copy2 = model->InsertItem<CopyItem>(sequence1);
  auto copy3 = model->InsertItem<CopyItem>(sequence1);

  auto sequence2 = model->InsertItem<SequenceItem>(force0);
  auto equals2 = model->InsertItem<EqualsItem>(sequence2);
  auto equals3 = model->InsertItem<EqualsItem>(sequence2);

  return procedure_item;
}

}  // namespace sequencergui::Examples
