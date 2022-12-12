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

#include "sequencergui/model/sequencer_model.h"

#include <gtest/gtest.h>
#include <mvvm/serialization/xml_document.h>
#include <mvvm/standarditems/container_item.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <testutils/folder_based_test.h>

using namespace sequencergui;

//! Tests for SequencerModel class.

class SequencerModelTest : public FolderBasedTest
{
public:
  SequencerModelTest() : FolderBasedTest("test_SequencerModel") {}
};

TEST_F(SequencerModelTest, InitialState)
{
  SequencerModel model;

  // container is already created
  EXPECT_NE(model.GetProcedureContainer(), nullptr);

  EXPECT_TRUE(model.GetProcedures().empty());

  // trying to insert procedure to make sure that catalogue is there
  auto item = model.InsertItem(model.GetFactory()->CreateItem(ProcedureItem::Type),
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
