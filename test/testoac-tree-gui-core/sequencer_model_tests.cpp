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

#include "oac-tree-gui/model/sequencer_model.h"

#include <oac-tree-gui/model/instruction_container_item.h>
#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/standard_instruction_items.h>
#include <oac-tree-gui/model/universal_variable_item.h>
#include <oac-tree-gui/model/workspace_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/item_factory.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace sequencergui::test
{

//! Tests for SequencerModel class.
class SequencerModelTest : public test::FolderTest
{
public:
  SequencerModelTest() : FolderTest("SequencerModelTest") {}
};

TEST_F(SequencerModelTest, InitialState)
{
  SequencerModel model;

  // container is already created
  EXPECT_NE(model.GetProcedureContainer(), nullptr);

  EXPECT_TRUE(model.GetProcedures().empty());

  // trying to insert procedure to make sure that catalogue is there
  auto item = model.InsertItem(mvvm::GetGlobalItemFactory().CreateItem(ProcedureItem::Type),
                               model.GetRootItem(), mvvm::TagIndex::Append());
  EXPECT_EQ(item->GetType(), ProcedureItem::Type);
  EXPECT_NE(dynamic_cast<ProcedureItem*>(item), nullptr);
}

TEST_F(SequencerModelTest, GetProcedures)
{
  SequencerModel model;
  auto procedure0 = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto procedure1 = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());

  EXPECT_EQ(model.GetProcedures(), std::vector<ProcedureItem*>({procedure0, procedure1}));
}

TEST_F(SequencerModelTest, XmlDocumentSaveLoad)
{
  const auto file_path = GetFilePath("XmlDocumentSaveLoad.xml");

  SequencerModel model;
  auto procedure_item = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto sequence0 = model.InsertItem<SequenceItem>(procedure_item->GetInstructionContainer());

  mvvm::XmlDocument document({&model});

  auto original_procedure_container = model.GetProcedureContainer();

  // saving model in file
  document.Save(file_path);

  // loading model from file
  document.Load(file_path);

  EXPECT_NE(model.GetProcedureContainer(), original_procedure_container);
  ASSERT_EQ(model.GetProcedures().size(), 1);
  EXPECT_EQ(model.GetProcedures().at(0)->GetInstructionContainer()->GetInstructions().size(), 1);
}

//! Checking that domain name of universal instruction item outsurvive serialization (real-life
//! bug).
TEST_F(SequencerModelTest, UniversalInstructionSerialization)
{
  const auto file_path = GetFilePath("UniversalInstructionSaveLoad.xml");

  SequencerModel model;
  auto procedure_item = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto wait_item =
      model.InsertItem<UniversalInstructionItem>(procedure_item->GetInstructionContainer());
  wait_item->SetDomainType(domainconstants::kWaitInstructionType);

  mvvm::XmlDocument document({&model});

  auto original_procedure_container = model.GetProcedureContainer();

  // saving model in file
  document.Save(file_path);

  // loading model from file
  document.Load(file_path);

  EXPECT_NE(model.GetProcedureContainer(), original_procedure_container);
  ASSERT_EQ(model.GetProcedures().size(), 1);
  EXPECT_EQ(model.GetProcedures().at(0)->GetInstructionContainer()->GetInstructions().size(), 1);
  auto new_wait_item =
      model.GetProcedures().at(0)->GetInstructionContainer()->GetInstructions().at(0);

  EXPECT_EQ(new_wait_item->GetDomainType(), domainconstants::kWaitInstructionType);
}

//! Checking that domain name of universal variable item outsurvive serialization (real-life bug).
TEST_F(SequencerModelTest, UniversalVariableSerialization)
{
  const auto file_path = GetFilePath("UniversalVariableSaveLoad.xml");

  SequencerModel model;
  auto procedure_item = model.InsertItem<ProcedureItem>(model.GetProcedureContainer());
  auto variable_item = model.InsertItem<UniversalVariableItem>(procedure_item->GetWorkspace());
  variable_item->SetDomainType(domainconstants::kLocalVariableType);

  mvvm::XmlDocument document({&model});

  auto original_procedure_container = model.GetProcedureContainer();

  // saving model in file
  document.Save(file_path);

  // loading model from file
  document.Load(file_path);

  EXPECT_NE(model.GetProcedureContainer(), original_procedure_container);
  ASSERT_EQ(model.GetProcedures().size(), 1);
  EXPECT_EQ(model.GetProcedures().at(0)->GetWorkspace()->GetVariables().size(), 1);
  auto new_variable_item = model.GetProcedures().at(0)->GetWorkspace()->GetVariables().at(0);

  EXPECT_EQ(new_variable_item->GetDomainType(), domainconstants::kLocalVariableType);
}

//! Checking that command container is cleaned when model is cleared.
TEST_F(SequencerModelTest, ClearAndCommandFramework)
{
  SequencerModel model;
  model.SetUndoEnabled(true);

  // initially created model already has a container, command stack is empty
  ASSERT_NE(model.GetProcedureContainer(), nullptr);
  ASSERT_EQ(model.GetCommandStack()->GetCommandCount(), 0);

  // inserting procedures
  model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());
  EXPECT_EQ(model.GetProcedures().size(), 1);
  ASSERT_EQ(model.GetCommandStack()->GetCommandCount(), 1);

  // clearing the model, container should still exist, command stack is empty
  model.Clear();
  ASSERT_NE(model.GetProcedureContainer(), nullptr);
  EXPECT_EQ(model.GetProcedures().size(), 0);
  ASSERT_EQ(model.GetCommandStack()->GetCommandCount(), 0);
}

}  // namespace sequencergui::test
