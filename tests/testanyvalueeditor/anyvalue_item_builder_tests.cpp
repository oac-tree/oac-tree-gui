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

#include "anyvalueeditor/anyvalue_item_builder.h"
#include "folder_based_test.h"

#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/anyvalue_utils.h>
#include <gtest/gtest.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

using namespace anyvalueeditor;

class AnyValueItemBuilderTests : public FolderBasedTest
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
    EXPECT_EQ(mvvm::utils::TypeName(item->Data()), mvvm::constants::kBoolTypeName);
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
    EXPECT_EQ(mvvm::utils::TypeName(item->Data()), mvvm::constants::kIntTypeName);
    EXPECT_EQ(item->Data<int>(), 42);
    EXPECT_TRUE(item->IsScalar());
    EXPECT_FALSE(item->IsStruct());
    EXPECT_FALSE(item->IsArray());
  }
}

//! Building AnyValueItem from AnyValue with a struct containing  two named scalars.

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

//! Building AnyValueItem from AnyValue with a struct containing a single scalar.

TEST_F(AnyValueItemBuilderTests, FromStructWithSingleScalar)
{
  sup::dto::AnyValue anyvalue = {{{"signed", {sup::dto::SignedInteger32Type, 42}}}};

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
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kIntTypeName);
}

//! Building AnyValueItem from AnyValue with a struct containing  two named scalars.

TEST_F(AnyValueItemBuilderTests, FromStructWithTwoScalars)
{
  sup::dto::AnyValue anyvalue = {
      {{"signed", {sup::dto::SignedInteger32Type, 42}}, {"bool", {sup::dto::BooleanType, true}}}};

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
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kIntTypeName);

  child = item->GetItem<AnyValueScalarItem>("", 1);
  EXPECT_EQ(child->GetTotalItemCount(), 0);
  EXPECT_EQ(child->GetDisplayName(), "bool");
  EXPECT_EQ(child->Data<bool>(), true);
  EXPECT_TRUE(child->IsScalar());
  EXPECT_FALSE(child->IsStruct());
  EXPECT_FALSE(item->IsArray());
  EXPECT_EQ(mvvm::utils::TypeName(child->Data()), mvvm::constants::kBoolTypeName);
}

//! Building AnyValueItem from AnyValue containing a structure with nested structure.

TEST_F(AnyValueItemBuilderTests, FromNestedStruct)
{
  sup::dto::AnyValue two_scalars = {
      {{"signed", {sup::dto::SignedInteger8Type, 1}}, {"bool", {sup::dto::BooleanType, 12}}}};
  sup::dto::AnyValue anyvalue{{
      {"scalars", two_scalars},
  }};

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
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  auto grandchild1 = child->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "bool");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kBoolTypeName);
}

//! Building AnyValueItem from AnyValue containing a structure with two nested structures.

TEST_F(AnyValueItemBuilderTests, FromTwoNestedStruct)
{
  const std::string nested_name = "nested_struct";
  sup::dto::AnyValue two_scalars = {{{"signed", {sup::dto::SignedInteger8Type, 1}},
                                     {"unsigned", {sup::dto::UnsignedInteger8Type, 12}}}};
  sup::dto::AnyValue anyvalue{{{"scalars", two_scalars},
                               {"single",
                                {{"first", {sup::dto::SignedInteger8Type, 0}},
                                 {"second", {sup::dto::SignedInteger8Type, 5}}}}},
                              nested_name};

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetAnyTypeName(), std::string());
  EXPECT_EQ(item->GetDisplayName(), kStructTypeName);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // first branch
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 2);
  EXPECT_EQ(child0->GetDisplayName(), "scalars");
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  auto grandchild0 = child0->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "signed");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  auto grandchild1 = child0->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "unsigned");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntTypeName);

  // second branch
  auto child1 = item->GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 2);
  EXPECT_EQ(child1->GetDisplayName(), "single");
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  grandchild0 = child1->GetItem("", 0);
  EXPECT_EQ(grandchild0->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild0->GetDisplayName(), "first");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild0->Data()), mvvm::constants::kIntTypeName);

  grandchild1 = child1->GetItem("", 1);
  EXPECT_EQ(grandchild1->GetTotalItemCount(), 0);
  EXPECT_EQ(grandchild1->GetDisplayName(), "second");
  EXPECT_EQ(mvvm::utils::TypeName(grandchild1->Data()), mvvm::constants::kIntTypeName);
}

//! Building AnyValueItem from AnyValue containing an array of integers.

TEST_F(AnyValueItemBuilderTests, FromArrayOfIntegers)
{
  sup::dto::AnyValue anyvalue =
      sup::dto::ArrayValue({{sup::dto::SignedInteger64Type, 1}, 2}, "my_array_t");

  auto item = GetAnyValueItem(anyvalue);

  EXPECT_EQ(item->GetTotalItemCount(), 2);
  EXPECT_EQ(item->GetDisplayName(), AnyValueArrayItem::Type);
  EXPECT_FALSE(mvvm::utils::IsValid(item->Data()));

  // first branch
  auto child0 = item->GetItem("", 0);
  EXPECT_EQ(child0->GetTotalItemCount(), 0);
  EXPECT_EQ(child0->GetDisplayName(), "index0");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(child0->Data<int>(), 1);

  // second branch
  auto child1 = item->GetItem("", 1);
  EXPECT_EQ(child1->GetTotalItemCount(), 0);
  EXPECT_EQ(child1->GetDisplayName(), "index1");
  EXPECT_EQ(mvvm::utils::TypeName(child0->Data()), mvvm::constants::kIntTypeName);
  EXPECT_EQ(child1->Data<int>(), 2);
}
