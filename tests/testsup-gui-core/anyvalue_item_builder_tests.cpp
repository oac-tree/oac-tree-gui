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

#include "sup/gui/dto/anyvalue_item_builder.h"

#include <gtest/gtest.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/gui/dto/anyvalue_item.h>
#include <sup/gui/dto/conversion_utils.h>
#include <testutils/folder_based_test.h>

#include <iostream>

using namespace sup::gui;

class AnyValueItemBuilderTests : public testutils::FolderBasedTest
{
public:
  AnyValueItemBuilderTests() : FolderBasedTest("test_AnyValueItemBuilder") {}

  std::unique_ptr<AnyValueItem> GetAnyValueItem(const sup::dto::AnyValue& value)
  {
    AnyValueItemBuilder builder;
    sup::dto::SerializeAnyValue(value, builder);
    return std::move(builder.MoveAnyValueItem());
  }

  //! Helper function to write AnyValue as JSON. Used to study how AnyValue stores itself.
  void WriteJson(const sup::dto::AnyValue& any_value, const std::string& file_name)
  {
    ::sup::dto::AnyValueToJSONFile(any_value, GetFilePath(file_name), /*pretty*/ true);
  }
};

//! Building AnyValueItem from AnyValue containing a scalar.

TEST_F(AnyValueItemBuilderTests, FromEmptyAnyValue)
{
  sup::dto::AnyValue anyvalue;
  WriteJson(anyvalue, "Empty.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetType(), AnyValueEmptyItem::Type);
  EXPECT_EQ(item->GetTotalItemCount(), 0);
  EXPECT_FALSE(item->IsScalar());
  EXPECT_FALSE(item->IsStruct());
  EXPECT_FALSE(item->IsArray());
}

//! Building AnyValueItem from AnyValue containing a scalar.

TEST_F(AnyValueItemBuilderTests, FromScalar)
{
  {  // bool
    sup::dto::AnyValue anyvalue{sup::dto::BooleanType, true};
    WriteJson(anyvalue, "ScalarBool.json");

    auto item = GetAnyValueItem(anyvalue);

    EXPECT_EQ(item->GetType(), AnyValueScalarItem::Type);
    EXPECT_EQ(item->GetTotalItemCount(), 0);
    EXPECT_EQ(mvvm::utils::TypeName(item->Data()), mvvm::constants::kBoolVariantName);
    EXPECT_TRUE(item->Data<bool>());
    EXPECT_TRUE(item->IsScalar());
    EXPECT_FALSE(item->IsStruct());
    EXPECT_FALSE(item->IsArray());
  }

  {  // int
    sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
    WriteJson(anyvalue, "ScalarInt32.json");

    auto item = GetAnyValueItem(anyvalue);

    EXPECT_EQ(item->GetType(), AnyValueScalarItem::Type);
    EXPECT_EQ(item->GetTotalItemCount(), 0);
    EXPECT_EQ(mvvm::utils::TypeName(item->Data()), mvvm::constants::kIntVariantName);
    EXPECT_EQ(item->Data<int>(), 42);
    EXPECT_TRUE(item->IsScalar());
    EXPECT_FALSE(item->IsStruct());
    EXPECT_FALSE(item->IsArray());
  }
}

//! Building AnyValueItem from AnyValue with an empty struct.

TEST_F(AnyValueItemBuilderTests, FromEmptyStruct)
{
  sup::dto::AnyValue anyvalue = ::sup::dto::EmptyStruct();
  WriteJson(anyvalue, "EmptyStruct.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetType(), AnyValueStructItem::Type);
  EXPECT_EQ(item->GetTotalItemCount(), 0);
  EXPECT_EQ(item->GetAnyTypeName(), std::string());
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(item->IsScalar());
  EXPECT_TRUE(item->IsStruct());
  EXPECT_FALSE(item->IsArray());
}

//! Building AnyValueItem from AnyValue with an empty struct.

TEST_F(AnyValueItemBuilderTests, FromEmptyNamedStruct)
{
  sup::dto::AnyValue anyvalue = ::sup::dto::EmptyStruct("mystruct");
  WriteJson(anyvalue, "EmptyNamedStruct.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetType(), AnyValueStructItem::Type);
  EXPECT_EQ(item->GetTotalItemCount(), 0);
  EXPECT_EQ(item->GetAnyTypeName(), std::string("mystruct"));
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(item->IsScalar());
  EXPECT_TRUE(item->IsStruct());
  EXPECT_FALSE(item->IsArray());
}

//! Building AnyValueItem from AnyValue with a struct containing a single scalar.

TEST_F(AnyValueItemBuilderTests, FromStructWithSingleScalar)
{
  sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};
  WriteJson(anyvalue, "StructWithSingleScalar.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 1);
  EXPECT_EQ(item->GetAnyTypeName(), std::string());
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));
  EXPECT_FALSE(item->IsScalar());
  EXPECT_TRUE(item->IsStruct());
  EXPECT_FALSE(item->IsArray());

  auto child = item->GetItem<AnyValueScalarItem>("", 0);
  EXPECT_EQ(child->GetType(), AnyValueScalarItem::Type);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "signed");
  EXPECT_TRUE(child->IsScalar());
  EXPECT_FALSE(child->IsStruct());
  EXPECT_FALSE(item->IsArray());
  EXPECT_EQ(child->Data<int>(), 42);
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kIntVariantName);
}

//! Building AnyValueItem from AnyValue with a struct containing  two named scalars.

TEST_F(AnyValueItemBuilderTests, FromStructWithTwoScalars)
{
  sup::dto::AnyValue anyvalue = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}}};
  WriteJson(anyvalue, "StructWithTwoScalars.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetAnyTypeName(), std::string());
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));
  EXPECT_FALSE(item->IsScalar());
  EXPECT_TRUE(item->IsStruct());
  EXPECT_FALSE(item->IsArray());

  auto child = item->GetItem<AnyValueScalarItem>("", 0);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "signed");
  EXPECT_EQ(child->Data<int>(), 42);
  EXPECT_TRUE(child->IsScalar());
  EXPECT_FALSE(child->IsStruct());
  EXPECT_FALSE(item->IsArray());
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kIntVariantName);

  child = item->GetItem<AnyValueScalarItem>("", 1);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "bool");
  EXPECT_EQ(child->Data<bool>(), true);
  EXPECT_TRUE(child->IsScalar());
  EXPECT_FALSE(child->IsStruct());
  EXPECT_FALSE(item->IsArray());
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kBoolVariantName);
}

//! Building AnyValueItem from AnyValue containing a structure with nested structure.

TEST_F(AnyValueItemBuilderTests, FromNestedStruct)
{
  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, true}}}};
  sup::dto::AnyValue anyvalue{{
      {"scalars", two_scalars},
  }};
  WriteJson(anyvalue, "NestedStruct.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 1);
  EXPECT_EQ(item->GetAnyTypeName(), std::string());
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  auto child = item->GetItem("", 0);
  EXPECT_EQ(child->GetTotalItemCount(), 2);
  EXPECT_EQ(child->GetDisplayName(), "scalars");
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  auto grandchild0 = child->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntVariantName);

  auto grandchild1 = child->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "bool");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kBoolVariantName);
}

//! Building AnyValueItem from AnyValue containing a structure with two nested structures.

TEST_F(AnyValueItemBuilderTests, FromTwoNestedStruct)
{
  const std::string nested_name = "nested_struct";
  sup::dto::AnyValue two_scalars = {{{"signed", {sup::dto::SignedInteger8Type, 1}},
                                     {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}}};
  sup::dto::AnyValue anyvalue{{{"struct0", two_scalars},
                               {"struct1",
                                {{"first", {sup::dto::SignedInteger8Type, 0}},
                                 {"second", {sup::dto::SignedInteger8Type, 5}}}}},
                              nested_name};
  WriteJson(anyvalue, "TwoNestedStructs.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetAnyTypeName(), nested_name);
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // first branch
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 2);
  EXPECT_EQ(child0->GetDisplayName(), "struct0");
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  auto grandchild0 = child0->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntVariantName);

  auto grandchild1 = child0->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "unsigned");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntVariantName);

  // second branch
  auto child1 = item->GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 2);
  EXPECT_EQ(child1->GetDisplayName(), "struct1");
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  grandchild0 = child1->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "first");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntVariantName);

  grandchild1 = child1->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "second");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntVariantName);
}

//! Building AnyValueItem from AnyValue containing an array of integers.

TEST_F(AnyValueItemBuilderTests, FromArrayOfIntegers)
{
  sup::dto::AnyValue anyvalue =
      sup::dto::ArrayValue({{sup::dto::SignedInteger64Type, 1}, 2}, "my_array_t");
  WriteJson(anyvalue, "ArrayOfIntegers.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetAnyTypeName(), std::string("my_array_t"));
  EXPECT_EQ(item->GetDisplayName(), kArrayTypeName);
  EXPECT_EQ(item->GetType(), AnyValueArrayItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // first branch
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 0);
  EXPECT_EQ(child0->GetDisplayName(), "index0");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(child0->Data<int>(), 1);

  // second branch
  auto child1 = item->GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 0);
  EXPECT_EQ(child1->GetDisplayName(), "index1");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(child1->Data<int>(), 2);
}

//! Building AnyValueItem from AnyValue structure containing an array of integers.

TEST_F(AnyValueItemBuilderTests, StructWithArrayOfIntegers)
{
  sup::dto::AnyValue array =
      sup::dto::ArrayValue({{sup::dto::SignedInteger64Type, 1}, 2}, "my_array");

  sup::dto::AnyValue anyvalue{{{"field", array}}};
  WriteJson(anyvalue, "StructWithArrayOfIntegers.json");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 1);
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_EQ(item->GetType(), AnyValueStructItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // array branch
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 2);
  EXPECT_EQ(child0->GetDisplayName(), "field");
  EXPECT_EQ(child0->GetType(), AnyValueArrayItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(child0->Data()));

  // elements
  auto element0 = child0->GetItem("", 0);
  EXPECT_EQ(element0->GetTotalItemCount(), 0);
  EXPECT_EQ(element0->GetDisplayName(), "index0");
  EXPECT_EQ(mvvm::utils::TypeName(element0->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(element0->Data<int>(), 1);

  auto element1 = child0->GetItem("", 1);
  EXPECT_EQ(element1->GetTotalItemCount(), 0);
  EXPECT_EQ(element1->GetDisplayName(), "index1");
  EXPECT_EQ(mvvm::utils::TypeName(element1->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(element1->Data<int>(), 2);
}

//! Building AnyValueItem from AnyValue array containing two structures.

TEST_F(AnyValueItemBuilderTests, ArrayWithTwoStructureElements)
{
  sup::dto::AnyValue struct1 = {{{"first", {sup::dto::SignedInteger8Type, -43}},
                                 {"second", {sup::dto::UnsignedInteger8Type, 44}}},
                                "struct_name"};
  sup::dto::AnyValue struct2 = {{{"first", {sup::dto::SignedInteger8Type, 42}},
                                 {"second", {sup::dto::UnsignedInteger8Type, 43}}},
                                "struct_name"};

  auto array_value = sup::dto::ArrayValue({{struct1}, struct2}, "array_name");
  WriteJson(array_value, "ArrayWithTwoStructureElements.json");

  // top level item is AnyValueArrayItem with two elements
  auto item = GetAnyValueItem(array_value);
  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetDisplayName(), kArrayTypeName);
  EXPECT_EQ(item->GetType(), AnyValueArrayItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // first element in array is a structure
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 2);
  EXPECT_EQ(child0->GetDisplayName(), "index0");
  EXPECT_EQ(child0->GetType(), AnyValueStructItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(child0->Data()));

  auto grandchild0 = child0->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "first");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(grandchild0->Data<int>(), -43);

  auto grandchild1 = child0->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "second");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(grandchild1->Data<int>(), 44);

  // second element in array is a structure
  auto child1 = item->GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 2);
  EXPECT_EQ(child1->GetDisplayName(), "index1");
  EXPECT_EQ(child1->GetType(), AnyValueStructItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(child1->Data()));

  auto grandchild2 = child1->GetItem("", 0);
  EXPECT_EQ(grandchild2->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild2->GetDisplayName(), "first");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild2->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(grandchild2->Data<int>(), 42);

  auto grandchild3 = child1->GetItem("", 1);
  EXPECT_EQ(grandchild3->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild3->GetDisplayName(), "second");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild3->Data()), mvvm::constants::kIntVariantName);
  EXPECT_EQ(grandchild3->Data<int>(), 43);
}
