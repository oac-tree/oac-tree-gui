/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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
#include <sequencergui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

namespace sequencergui
{

namespace
{

/**
 * @brief Creates procedure in a model.
 *
 * @param procedure The procedure.
 * @param model The model where the procedure will be created.
 */
sequencergui::ProcedureItem* CreateProcedure(std::unique_ptr<sequencergui::ProcedureItem> procedure,
                                             sequencergui::SequencerModel* model)
{
  auto procedure_ptr = procedure.get();
  model->InsertItem(std::move(procedure), model->GetProcedureContainer(), mvvm::TagIndex::Append());
  return procedure_ptr;
}

/**
 * @brief Creates procedure in a model.
 *
 * @param func Pointer to the procedure creation function.
 * @param model The model where the procedure will be created.
 */
sequencergui::ProcedureItem* CreateProcedure(std::unique_ptr<sequencergui::ProcedureItem> (*func)(),
                                             sequencergui::SequencerModel* model)
{
  return CreateProcedure(func(), model);
}

}  // namespace

namespace test
{

std::unique_ptr<sequencergui::ProcedureItem> CreateInvalidProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  result->GetInstructionContainer()->InsertItem<UniversalInstructionItem>(mvvm::TagIndex::Append());
  return result;
}

sequencergui::ProcedureItem* CreateInvalidProcedureItem(sequencergui::SequencerModel* model)
{
  return CreateProcedure(&CreateInvalidProcedureItem, model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateSingleWaitProcedureItem(
    std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<ProcedureItem>();
  auto wait = result->GetInstructionContainer()->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait->SetTimeout(GetTimeoutInSec(timeout));
  return result;
}

ProcedureItem* CreateSingleWaitProcedureItem(SequencerModel* model,
                                             std::chrono::milliseconds timeout)
{
  return CreateProcedure(CreateSingleWaitProcedureItem(timeout), model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateCopyProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto copy =
      InsertInstruction(domainconstants::kCopyInstructionType, result->GetInstructionContainer());
  SetInput("var0", copy);
  SetOutput("var1", copy);

  auto var0 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var0);

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 43}, *var1);

  return result;
}

ProcedureItem* CreateCopyProcedureItem(SequencerModel* model)
{
  return CreateProcedure(&CreateCopyProcedureItem, model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateIncludeProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto sequence =
      result->GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  sequence->SetName("MySequence");
  sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  auto repeat = result->GetInstructionContainer()->InsertItem<RepeatItem>(mvvm::TagIndex::Append());
  repeat->SetRepeatCount(1);
  repeat->SetIsRootFlag(true);
  auto include = repeat->InsertItem<IncludeItem>(mvvm::TagIndex::Append());
  include->SetPath("MySequence");

  return result;
}

ProcedureItem* CreateIncludeProcedureItem(SequencerModel* model)
{
  return CreateProcedure(&CreateIncludeProcedureItem, model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateInputProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto sequence =
      result->GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());
  sequence->SetName("MySequence");

  auto input0 = InsertInstruction(domainconstants::kInputInstructionType, sequence);
  SetOutput("var1", input0);
  SetDescription("Your ID", input0);

  sequence->InsertItem<WaitItem>(mvvm::TagIndex::Append());

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var1);

  return result;
}

ProcedureItem* CreateInputProcedureItem(SequencerModel* model)
{
  return CreateProcedure(&CreateInputProcedureItem, model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateUserChoiceProcedureItem()
{
  // UserChoice instruction will offer the user a choice between two instructions to execute
  // 0) Wait instruction
  // 1) Copy instruction
  auto result = std::make_unique<ProcedureItem>();
  auto userchoice = InsertInstruction(domainconstants::kUserChoiceInstructionType,
                                      result->GetInstructionContainer());
  auto wait0 = userchoice->InsertItem<WaitItem>(mvvm::TagIndex::Append());
  wait0->SetTimeout(10.0);

  auto copy = InsertInstruction(domainconstants::kCopyInstructionType, userchoice);
  SetInput("var0", copy);
  SetOutput("var1", copy);

  auto var0 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 42}, *var0);

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::UnsignedInteger32Type, 0}, *var1);

  return result;
}

ProcedureItem* CreateUserChoiceProcedureItem(SequencerModel* model)
{
  return CreateProcedure(&CreateUserChoiceProcedureItem, model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateMessageProcedureItem(const std::string& text)
{
  auto result = std::make_unique<ProcedureItem>();
  auto message = InsertInstruction(domainconstants::kMessageInstructionType,
                                   result->GetInstructionContainer());
  SetText(text, message);
  return result;
}

ProcedureItem* CreateMessageProcedureItem(sequencergui::SequencerModel* model,
                                          const std::string& text)
{
  return CreateProcedure(CreateMessageProcedureItem(text), model);
}

std::unique_ptr<sequencergui::ProcedureItem> CreateVariableResetProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();

  auto sequence =
      result->GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  auto copy0 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("a", copy0);
  SetOutput("target", copy0);

  auto reset = InsertInstruction(domainconstants::kVariableResetInstructionType, sequence);
  SetVariableName("target", reset);

  auto copy1 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  SetInput("b", copy1);
  SetOutput("target", copy1);

  auto var0 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("a");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42}, *var0);

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("b");
  SetAnyValue(sup::dto::AnyValue{sup::dto::StringType, "abc"}, *var1);

  auto var2 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var2->SetName("target");

  return result;
}

ProcedureItem* CreateVariableResetProcedureItem(sequencergui::SequencerModel* model)
{
  return CreateProcedure(&CreateVariableResetProcedureItem, model);
}

std::unique_ptr<ProcedureItem> CreateIncrementProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();

  auto sequence =
      result->GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  auto increment0 = InsertInstruction(domainconstants::kIncrementInstructionType, sequence);
  SetVariableName("var0", increment0);

  auto var0 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);

  auto increment1 = InsertInstruction(domainconstants::kIncrementInstructionType, sequence);
  SetVariableName("var1", increment1);

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 10}, *var1);

  return result;
}

ProcedureItem* CreateIncrementProcedureItem(sequencergui::SequencerModel* model)
{
  return CreateProcedure(&CreateIncrementProcedureItem, model);
}

}  // namespace test

}  // namespace sequencergui
