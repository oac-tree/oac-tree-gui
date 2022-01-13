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

#include "sequencergui/model/importutils.h"

#include "folderbasedtest.h"
#include "sequencergui/model/sequenceritems.h"
#include "test_utils.h"

//! Testing methods from importutils.h

class ImportUtilsTest : public FolderBasedTest
{
public:
  ImportUtilsTest() : FolderBasedTest("test_ImportUtilsTest") {}

  std::string CreateProcedureString(const std::string &body)
  {
    static const std::string header{
        R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">)RAW"};

    static const std::string footer{R"RAW(</Procedure>)RAW"};

    return header + body + footer;
  }
};

//! Importing xml Procedure containing a single instruction.

TEST_F(ImportUtilsTest, ProcedureWithSingleWait)
{
  const std::string body{R"(
  <Wait name="Only" timeout="42" />
)"};

  // writing procedure in file
  const auto file_name = GetFilePath("ProcedureWithSingleWait.xml");
  TestUtils::CreateTextFile(file_name, CreateProcedureString(body));

  sequencergui::ProcedureItem procedure_item;
  sequencergui::ImportFromFile(file_name, &procedure_item);

  auto container = procedure_item.GetInstructionContainer();
  auto wait_item = container->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Importing xml Procedure containing a single instruction.

TEST_F(ImportUtilsTest, ProcedureWithSingleVariable)
{
  const std::string body{R"(
  <Workspace>
    <Local name="severity" type='{"type":"uint32"}' value='7'/>
  </Workspace>
)"};

  // writing procedure in file
  const auto file_name = GetFilePath("ProcedureWithSingleVariable.xml");
  TestUtils::CreateTextFile(file_name, CreateProcedureString(body));

  sequencergui::ProcedureItem procedure_item;
  sequencergui::ImportFromFile(file_name, &procedure_item);

  auto variable_item = procedure_item.GetWorkspace()->GetItem<sequencergui::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), std::string(R"({"type":"uint32"})"));
  EXPECT_EQ(variable_item->GetJsonValue(), std::string("7"));
}
