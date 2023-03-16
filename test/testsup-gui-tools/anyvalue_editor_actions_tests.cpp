/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sup/gui/anyvalueeditor/anyvalue_editor_actions.h"

#include <gtest/gtest.h>
#include <testutils/folder_based_test.h>
#include <testutils/mock_callback_listener.h>
#include <testutils/test_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/utils/file_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

using namespace sup::gui;
using ::testing::_;

class AnyValueEditorActionsTest : public testutils::FolderBasedTest
{
public:
  AnyValueEditorActionsTest() : testutils::FolderBasedTest("test_AnyValueEditorAction")
  {
    m_model.RegisterItem<sup::gui::AnyValueStructItem>();
    m_model.RegisterItem<sup::gui::AnyValueArrayItem>();
    m_model.RegisterItem<sup::gui::AnyValueScalarItem>();
  }

  //! Creates context necessary for AnyValueEditActions to function.
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* item)
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [item]() { return item; };
    return {get_selected_callback, m_warning_listener.CreateCallback()};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<AnyValueEditorActions> CreateActions(sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActions>(CreateContext(selection), &m_model, nullptr);
  }

  mvvm::ApplicationModel m_model;
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_warning_listener;
};

//! Testing initial state of AnyValueEditorActions object.

TEST_F(AnyValueEditorActionsTest, InitialState)
{
  auto actions = CreateActions(nullptr);
  EXPECT_EQ(actions->GetTopItem(), nullptr);
  EXPECT_EQ(actions->GetSelectedItem(), nullptr);
}

//! Testing AnyValueEditorActions::SetInitialValue method.

TEST_F(AnyValueEditorActionsTest, SetInitialValue)
{
  AnyValueScalarItem item;
  item.SetAnyTypeName(sup::dto::kInt32TypeName);
  item.SetData(42);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  auto actions = CreateActions(nullptr);
  actions->SetInitialValue(item);
  ASSERT_NE(actions->GetTopItem(), nullptr);

  // validating
  auto copied_item = dynamic_cast<AnyValueScalarItem*>(actions->GetTopItem());
  ASSERT_NE(copied_item, nullptr);
  EXPECT_NE(copied_item, &item);
  EXPECT_EQ(copied_item->GetAnyTypeName(), item.GetAnyTypeName());
  EXPECT_EQ(copied_item->Data<int>(), item.Data<int>());
  //  EXPECT_EQ(copied_item->GetIdentifier(), item.GetIdentifier()); FIXME uncomment after
  //  implementing
}

// -------------------------------------------------------------------------------------------------
// Adding structures
// -------------------------------------------------------------------------------------------------

//! Adding structure to an empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueStructToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  EXPECT_EQ(actions->GetSelectedItem(), nullptr);

  // expecting no warnings
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueStruct();

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueStructItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kStructTypeName);
};

//! Attempt to add a structure to a non-empty model when nothing is selected.

TEST_F(AnyValueEditorActionsTest, AttemptToAddToNonEmptyModel)
{
  // non-empty model
  m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // expecting warning
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add another top level item
  actions->OnAddAnyValueStruct();

  // validating that there is still one item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding structure as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueStructToAnotherStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);
  EXPECT_EQ(actions->GetSelectedItem(), parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueStruct();

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueStruct"));
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kStructTypeName);
};

//! Attempt to add a structure as a field to a scalar.

TEST_F(AnyValueEditorActionsTest, AttemptToAddStructToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting error callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field to
  actions->OnAddAnyValueStruct();

  // validating that nothing can changed in the model
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//-------------------------------------------------------------------------------------------------
// Adding scalars
//-------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueScalarToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);

  // expecting warning callback further down
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding another scalar when nothing is selected should trigger the warning
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding scalar as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueScalarToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating an action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
};

//! Adding a scalar as an array element (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueScalarToArray)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  // creating an action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
};

//! Attempt to add scalar as a field to another scalar.

TEST_F(AnyValueEditorActionsTest, AttemptToAddScalarToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that nothing can changed in the model
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level scalar to the model.

TEST_F(AnyValueEditorActionsTest, AttemptToAddSecondTopLevelScalar)
{
  m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // expecting warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top level scalar
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // checking that model still have a single item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Attempt to add a scalar as an array element when array is contasining diffierent scalar types.

TEST_F(AnyValueEditorActionsTest, AttemptToAddScalarToArrayWhenTypeMismath)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueArrayItem>();
  m_model.InsertItem<sup::gui::AnyValueScalarItem>(parent)->SetAnyTypeName(
      sup::dto::kInt32TypeName);

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field. The type matches what is already in the array.
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName);

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 2);

  // expecting error callback
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add mismatching type
  actions->OnAddAnyValueScalar(sup::dto::kInt16TypeName);

  // array still has two element
  EXPECT_EQ(parent->GetChildren().size(), 2);
};

//-------------------------------------------------------------------------------------------------
// Adding array
//-------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueArrayToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueArray();

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kArrayTypeName);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top-level item
  actions->OnAddAnyValueArray();

  // the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding array as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueArrayToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueArray();

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueArray"));
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kArrayTypeName);
};

//! Attempt to add array as a field to a scalar.

TEST_F(AnyValueEditorActionsTest, AttemptToAddArrayToScalar)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueScalarItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting error callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // adding AnyValueItem struct as a field to
  actions->OnAddAnyValueArray();

  // validating that nothing can changed in the model
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 0);
};

//! Attempt to add second top level array to the model.

TEST_F(AnyValueEditorActionsTest, AttemptToAddSecondTopLevelArray)
{
  m_model.InsertItem<sup::gui::AnyValueArrayItem>();

  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // expecting warning callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // attempt to add second top level array
  actions->OnAddAnyValueArray();

  // checking that model still have a single item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//-------------------------------------------------------------------------------------------------
// Remove item
//-------------------------------------------------------------------------------------------------

//! Remove item when nothing is selected.

TEST_F(AnyValueEditorActionsTest, RemoveItemWhenNothingIsSelected)
{
  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  EXPECT_NO_FATAL_FAILURE(actions->OnRemoveSelected());

  // validating that still has an item
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Remove selected item.

TEST_F(AnyValueEditorActionsTest, RemoveSelectedItem)
{
  auto struct_item = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  // creating action for the context, pretending item is selected
  auto actions = CreateActions(struct_item);

  actions->OnRemoveSelected();

  // validating that there is no item anymore
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
};

//-------------------------------------------------------------------------------------------------
// Import/export
//-------------------------------------------------------------------------------------------------

//! Validates import of JSON from file.

TEST_F(AnyValueEditorActionsTest, ImportFromFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = GetAnyValueToJSONString(&anyvalue);
  testutils::CreateTextFile(file_path, json_content);

  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  actions->OnImportFromFileRequest(file_path);

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);

  // attempt to import again
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  actions->OnImportFromFileRequest(file_path);
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Validates import of JSON from file, where imported value goes as a field to selected
//! structure.

TEST_F(AnyValueEditorActionsTest, ImportFromFileToStructField)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalar.xml");
  sup::dto::AnyValue anyvalue{sup::dto::SignedInteger32Type, 42};
  auto json_content = GetAnyValueToJSONString(&anyvalue);
  testutils::CreateTextFile(file_path, json_content);

  // creating a
  auto structure = m_model.InsertItem<sup::gui::AnyValueStructItem>();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  // creating action for the context, making structure selected
  auto actions = CreateActions(structure);

  // expecting no callbacks
  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  actions->OnImportFromFileRequest(file_path);

  // testing new child of the structure
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(structure->GetChildren().size(), 1);
  auto inserted_item = dynamic_cast<sup::gui::AnyValueScalarItem*>(structure->GetChildren().at(0));
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::gui::kScalarTypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->Data<int>(), 42);
};

//! Validates export of top level item to JSON file.

TEST_F(AnyValueEditorActionsTest, ExportToFile)
{
  // preparing scalar
  auto scalar = m_model.InsertItem<sup::gui::AnyValueScalarItem>();
  scalar->SetAnyTypeName(sup::dto::kInt32TypeName);
  scalar->SetData(99);

  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResults.xml");

  // creating action when nothing is selected
  auto actions = CreateActions(nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(0);

  // exporting file
  actions->OnExportToFileRequest(file_path);

  // model should be the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  // reading our exported file for the validation
  auto exported_value = sup::gui::AnyValueFromJSONFile(file_path);

  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 99};
  EXPECT_EQ(exported_value, expected_anyvalue);
};

//! Attempt to export to JSON file from epmpty model.

TEST_F(AnyValueEditorActionsTest, AttemptToExportEmptyModelToFile)
{
  // preparing file with content for further import
  const auto file_path = GetFilePath("AnyValueScalarExportResultsV2.xml");

  // creating action when nothing is selected
  auto actions = CreateActions(nullptr);

  EXPECT_CALL(m_warning_listener, OnCallback(_)).Times(1);

  // exporting file
  actions->OnExportToFileRequest(file_path);

  // model empty as it was, file wasn't created
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_FALSE(mvvm::utils::IsExists(file_path));
};
