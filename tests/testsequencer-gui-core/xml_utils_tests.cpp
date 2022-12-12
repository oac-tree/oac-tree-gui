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

#include "sequencergui/model/xml_utils.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <testutils/folder_based_test.h>
#include <testutils/test_utils.h>

//! Testing methods from importutils.h

class XmlUtilsTest : public FolderBasedTest
{
public:
  XmlUtilsTest() : FolderBasedTest("test_ImportUtilsTest") {}

  static std::string CreateProcedureString(const std::string& body, bool schema = true)
  {
    static const std::string header_with_schema{
        R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">)RAW"};

    static const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>)RAW"};

    static const std::string footer{R"RAW(</Procedure>
)RAW"};

    return (schema ? header_with_schema : header) + body + footer;
  }
};

//! Importing xml Procedure containing a single instruction.

TEST_F(XmlUtilsTest, ImportFromFileProcedureWithSingleWait)
{
  const std::string body{R"(
  <Wait name="Only" timeout="42" />
)"};

  // writing procedure in file
  const auto file_name = GetFilePath("ProcedureWithSingleWait.xml");
  testutils::CreateTextFile(file_name, CreateProcedureString(body));

  auto procedure_item = sequencergui::ImportFromFile(file_name);

  auto container = procedure_item->GetInstructionContainer();
  auto wait_item = container->GetItem<sequencergui::WaitItem>("");
  EXPECT_EQ(wait_item->GetTimeout(), 42.0);
}

//! Importing xml Procedure containing a single instruction.

TEST_F(XmlUtilsTest, ImportFromFileProcedureWithSingleVariable)
{
  const std::string body{R"(
  <Workspace>
    <Local name="severity" type='{"type":"uint32"}' value='7'/>
  </Workspace>
)"};

  // writing procedure in file
  const auto file_name = GetFilePath("ProcedureWithSingleVariable.xml");
  testutils::CreateTextFile(file_name, CreateProcedureString(body));

  auto procedure_item = sequencergui::ImportFromFile(file_name);

  auto variable_item = procedure_item->GetWorkspace()->GetItem<sequencergui::LocalVariableItem>("");
  EXPECT_EQ(variable_item->GetJsonType(), std::string(R"({"type":"uint32"})"));
  EXPECT_EQ(variable_item->GetJsonValue(), std::string("7"));
}

//! Exporting xml Procedure containing a single instruction.

TEST_F(XmlUtilsTest, ExportToXMLStringProcedureWithSingleWait)
{
  sequencergui::ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto wait0 = container->InsertItem<sequencergui::WaitItem>({"", -1});
  wait0->SetTimeout(42.0);

  const std::string body{R"(
  <Wait isRoot="false" name="" timeout="42.0"/>
  <Workspace/>
)"};

  auto expected_string = CreateProcedureString(
      body, /*schema*/ false);  // current ExportToXMLString doesn't know how export with schema
  EXPECT_EQ(sequencergui::ExportToXMLString(&procedure_item), expected_string);
}
