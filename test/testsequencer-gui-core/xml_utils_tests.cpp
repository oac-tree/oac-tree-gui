/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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
#include <sequencergui/transform/transform_helpers.h>

#include <mvvm/test/test_helper.h>

#include <sup/dto/anyvalue.h>

#include <testutils/folder_test.h>
#include <testutils/test_utils.h>

//! Testing methods from importutils.h

class XmlUtilsTest : public testutils::FolderTest
{
public:
  XmlUtilsTest() : FolderTest("XmlUtilsTest") {}
};

//! Importing xml Procedure containing a single instruction.

TEST_F(XmlUtilsTest, ImportFromFileProcedureWithSingleWait)
{
  const std::string body{R"(
  <Wait name="Only" timeout="42" />
)"};

  // writing procedure in file
  const auto file_name = GetFilePath("ProcedureWithSingleWait.xml");
  mvvm::test::CreateTextFile(file_name, testutils::CreateProcedureString(body));

  auto procedure_item = sequencergui::ImportFromFile(file_name);

  EXPECT_EQ(procedure_item->GetFileName(), file_name);

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
  mvvm::test::CreateTextFile(file_name, testutils::CreateProcedureString(body));

  auto procedure_item = sequencergui::ImportFromFile(file_name);

  auto variable_item = procedure_item->GetWorkspace()->GetItem<sequencergui::LocalVariableItem>("");

  const sup::dto::AnyValue expected_anyvalue(sup::dto::UnsignedInteger32Type, 7);
  EXPECT_EQ(expected_anyvalue, GetAnyValue(*variable_item));
}

//! Exporting xml Procedure containing a single instruction.

TEST_F(XmlUtilsTest, ExportToXMLStringProcedureWithSingleWait)
{
  sequencergui::ProcedureItem procedure_item;
  auto container = procedure_item.GetInstructionContainer();

  auto wait0 = container->InsertItem<sequencergui::WaitItem>({"", -1});
  wait0->SetTimeout(42.0);

  const std::string body{R"(
  <Wait timeout="42.0"/>
  <Workspace/>
)"};

  // current ExportToXMLString doesn't know how export with schema
  auto expected_string = testutils::CreateProcedureString(body, /*schema*/ false);
  EXPECT_EQ(sequencergui::ExportToXMLString(procedure_item), expected_string);
}

TEST_F(XmlUtilsTest, ReplaceQuotationMarks)
{
  using sequencergui::ReplaceQuotationMarks;

  // string stays the same
  EXPECT_EQ(ReplaceQuotationMarks(""), std::string());
  EXPECT_EQ(ReplaceQuotationMarks("abc"), std::string("abc"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW("type")RAW"), std::string(R"RAW("type")RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW(value='0')RAW"), std::string(R"RAW(value='0')RAW"));

  // html quotes replaced
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW("&quot;")RAW"), std::string(R"RAW('"')RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW(type="{&quot;abc&quot;}")RAW"),
            std::string(R"RAW(type='{"abc"}')RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW("&quot;abc&quot;&quot;def&quot;")RAW"),
            std::string(R"RAW('"abc""def"')RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW(type="{&quot;type&quot;:&quot;uint64&quot;}")RAW"),
            std::string(R"RAW(type='{"type":"uint64"}')RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW(value="&quot;My variable&quot;")RAW"),
            std::string(R"RAW(value='"My variable"')RAW"));
  EXPECT_EQ(ReplaceQuotationMarks(R"RAW(value="&quot;My variable&quot;")RAW"),
            std::string(R"RAW(value='"My variable"')RAW"));
  const std::string long_str(
      R"RAW(<Local name="cancel_text" dynamicType="false" type="{&quot;type&quot;:&quot;string&quot;}" value="&quot;Negative&quot;"/>)RAW");
  const std::string expected(
      R"RAW(<Local name="cancel_text" dynamicType="false" type='{"type":"string"}' value='"Negative"'/>)RAW");
  EXPECT_EQ(ReplaceQuotationMarks(long_str), expected);
}
