/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/model/sequencerutils.h"

#include "sequencergui/model/domain_constants.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/taggeditems.h"
#include "mvvm/utils/containerutils.h"

#include <vector>

namespace
{

// FIXME remove duplication with InstructionItem.cpp
const std::string kChildInstructions = "kChildInstructions";

std::vector<std::string> GetDomainDecoratorNames()
{
  std::vector<std::string> result{sequi::DomainConstants::kForceSuccessInstructionType,
                                  sequi::DomainConstants::kIncludeInstructionType,
                                  sequi::DomainConstants::kRepeatInstructionType,
                                  sequi::DomainConstants::kInverterInstructionType};
  return result;
}
}  // namespace

namespace sequi
{

bool IsCompoundInstruction(const InstructionItem *instruction)
{
  return instruction->GetTaggedItems()->HasTag(kChildInstructions);
}

bool IsDecoratorInstruction(const InstructionItem *instruction)
{
  static const std::vector<std::string> domain_names = GetDomainDecoratorNames();
  return ModelView::Utils::Contains(domain_names, instruction->GetDomainType());
}

void AddCopyExampleProcedure(SequencerModel *model)
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
}

void AddLocalIncludeExampleProcedure(SequencerModel *model)
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
}

void AddUserInputExampleProcedure(SequencerModel *model)
{
  auto procedure_item = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
  auto sequence = model->InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());
  sequence->SetName("MySequence");
  auto input0 = model->InsertItem<InputItem>(sequence);
  input0->SetTargetVariableName("var1");
  input0->SetDescription("Your name");

  auto wait0 = model->InsertItem<WaitItem>(sequence);

  auto var1 = model->InsertItem<LocalVariableItem>(procedure_item->GetWorkspace());
  var1->SetName("var1");
  var1->SetJsonType(R"({"type":"string"})");
  var1->SetJsonValue("abc");
}

}  // namespace sequi
