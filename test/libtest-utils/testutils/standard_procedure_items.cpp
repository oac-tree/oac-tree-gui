/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "standard_procedure_items.h"

#include "test_utils.h"

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>

#include <mvvm/standarditems/container_item.h>

#include <sup/dto/anyvalue.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Creates procedure in a model.
 *
 * @param procedure The procedure.
 * @param model The model where the procedure will be created.
 */
oac_tree_gui::ProcedureItem* CreateProcedure(std::unique_ptr<oac_tree_gui::ProcedureItem> procedure,
                                             oac_tree_gui::SequencerModel* model)
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
oac_tree_gui::ProcedureItem* CreateProcedure(std::unique_ptr<oac_tree_gui::ProcedureItem> (*func)(),
                                             oac_tree_gui::SequencerModel* model)
{
  return CreateProcedure(func(), model);
}

}  // namespace

namespace test
{

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateInvalidProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto instruction =
      result->GetInstructionContainer()->InsertItem<InstructionItem>(mvvm::TagIndex::Append());
  return result;
}

oac_tree_gui::ProcedureItem* CreateInvalidProcedureItem(oac_tree_gui::SequencerModel* model)
{
  return CreateProcedure(&CreateInvalidProcedureItem, model);
}

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateSingleWaitProcedureItem(
    std::chrono::milliseconds timeout)
{
  auto result = std::make_unique<ProcedureItem>();
  auto wait =
      InsertInstruction(domainconstants::kWaitInstructionType, result->GetInstructionContainer());
  wait->SetAttribute(domainconstants::kTimeoutAttribute, GetTimeoutInSec(timeout));
  wait->SetAttribute(domainconstants::kBlockingAttribute, true);
  return result;
}

ProcedureItem* CreateSingleWaitProcedureItem(SequencerModel* model,
                                             std::chrono::milliseconds timeout)
{
  return CreateProcedure(CreateSingleWaitProcedureItem(timeout), model);
}

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateCopyProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto copy =
      InsertInstruction(domainconstants::kCopyInstructionType, result->GetInstructionContainer());
  copy->SetAttribute(domainconstants::kInputVariableNameAttribute, std::string("var0"));
  copy->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("var1"));

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

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateIncludeProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();
  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType,
                                    result->GetInstructionContainer());
  sequence->SetName("MySequence");
  InsertInstruction(domainconstants::kWaitInstructionType, sequence);

  auto repeat =
      InsertInstruction(domainconstants::kRepeatInstructionType, result->GetInstructionContainer());
  repeat->SetAttribute(domainconstants::kMaxCountAttribute, mvvm::int32{1});
  repeat->SetIsRootFlag(true);
  auto include = InsertInstruction(domainconstants::kIncludeInstructionType, repeat);
  include->SetAttribute(domainconstants::kPathAttribute, std::string("MySequence"));

  return result;
}

ProcedureItem* CreateIncludeProcedureItem(SequencerModel* model)
{
  return CreateProcedure(&CreateIncludeProcedureItem, model);
}

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateInputProcedureItem(
    const sup::dto::AnyValue& initial_value)
{
  auto result = std::make_unique<ProcedureItem>();
  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType,
                                    result->GetInstructionContainer());
  sequence->SetName("MySequence");

  auto input0 = InsertInstruction(domainconstants::kInputInstructionType, sequence);
  input0->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("var1"));
  input0->SetAttribute(domainconstants::kDescriptionAttribute, std::string("Your ID"));

  InsertInstruction(domainconstants::kWaitInstructionType, sequence);

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(initial_value, *var1);

  return result;
}

ProcedureItem* CreateInputProcedureItem(SequencerModel* model,
                                        const sup::dto::AnyValue& initial_value)
{
  return CreateProcedure(CreateInputProcedureItem(initial_value), model);
}

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateUserChoiceProcedureItem()
{
  // UserChoice instruction will offer the user a choice between two instructions to execute
  // 0) Wait instruction
  // 1) Copy instruction
  auto result = std::make_unique<ProcedureItem>();
  auto userchoice = InsertInstruction(domainconstants::kUserChoiceInstructionType,
                                      result->GetInstructionContainer());
  auto wait0 = InsertInstruction(domainconstants::kWaitInstructionType, userchoice);
  wait0->SetAttribute(domainconstants::kTimeoutAttribute, 10.0);

  auto copy = InsertInstruction(domainconstants::kCopyInstructionType, userchoice);
  copy->SetAttribute(domainconstants::kInputVariableNameAttribute, std::string("var0"));
  copy->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("var1"));

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

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateMessageProcedureItem(const std::string& text)
{
  auto result = std::make_unique<ProcedureItem>();
  auto message = InsertInstruction(domainconstants::kMessageInstructionType,
                                   result->GetInstructionContainer());
  message->SetAttribute(domainconstants::kTextAttribute, text);
  return result;
}

ProcedureItem* CreateMessageProcedureItem(oac_tree_gui::SequencerModel* model,
                                          const std::string& text)
{
  return CreateProcedure(CreateMessageProcedureItem(text), model);
}

std::unique_ptr<oac_tree_gui::ProcedureItem> CreateVariableResetProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();

  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType,
                                    result->GetInstructionContainer());

  auto copy0 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  copy0->SetAttribute(domainconstants::kInputVariableNameAttribute, std::string("a"));
  copy0->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("target"));

  auto reset = InsertInstruction(domainconstants::kVariableResetInstructionType, sequence);
  reset->SetAttribute(domainconstants::kGenericVariableNameAttribute, std::string("target"));

  auto copy1 = InsertInstruction(domainconstants::kCopyInstructionType, sequence);
  copy1->SetAttribute(domainconstants::kInputVariableNameAttribute, std::string("b"));
  copy1->SetAttribute(domainconstants::kOutputVariableNameAttribute, std::string("target"));

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

ProcedureItem* CreateVariableResetProcedureItem(oac_tree_gui::SequencerModel* model)
{
  return CreateProcedure(&CreateVariableResetProcedureItem, model);
}

std::unique_ptr<ProcedureItem> CreateIncrementProcedureItem()
{
  auto result = std::make_unique<ProcedureItem>();

  auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType,
                                    result->GetInstructionContainer());

  auto increment0 = InsertInstruction(domainconstants::kIncrementInstructionType, sequence);
  increment0->SetAttribute(domainconstants::kGenericVariableNameAttribute, std::string("var0"));

  auto var0 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var0->SetName("var0");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *var0);

  auto increment1 = InsertInstruction(domainconstants::kIncrementInstructionType, sequence);
  increment1->SetAttribute(domainconstants::kGenericVariableNameAttribute, std::string("var1"));

  auto var1 = result->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  var1->SetName("var1");
  SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 10}, *var1);

  return result;
}

ProcedureItem* CreateIncrementProcedureItem(oac_tree_gui::SequencerModel* model)
{
  return CreateProcedure(&CreateIncrementProcedureItem, model);
}

}  // namespace test

}  // namespace oac_tree_gui
