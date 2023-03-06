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

#include "anyvalueeditor/anyvalue_editor_actions.h"

#include <anyvalueeditor/anyvalue_editor_context.h>
#include <gtest/gtest.h>
#include <testutils/mock_callback_listener.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>

#include <sup/dto/anytype.h>
#include <sup/gui/core/anyvalue_conversion_utils.h>
#include <sup/gui/core/anyvalue_item.h>

using namespace anyvalueeditor;
using ::testing::_;

class AnyValueEditorActionsTest : public ::testing::Test
{
public:
  //! Creates context necessary for AnyValueEditActions to function.
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* item)
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [item]() { return item; };
    return {get_selected_callback, m_listener.CreateCallback()};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<AnyValueEditorActions> CreateActions(sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActions>(CreateContext(selection), &m_model, nullptr);
  }

  mvvm::ApplicationModel m_model;
  testutils::MockCallbackListener<sup::gui::MessageEvent> m_listener;
};

// -------------------------------------------------------------------------------------------------
// Adding structures
// -------------------------------------------------------------------------------------------------

//! Adding structure to the empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueStructToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueStruct(/*add to selected*/ false);

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueStructItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kStructTypeName);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding another AnyValueItem to "selected" item as field
  actions->OnAddAnyValueStruct(/*add to selected*/ true);

  // since our context doesn't report any selection, the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding structure as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueStructToAnotherStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueStruct(/*add to selected*/ true);  // no-op

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueStruct"));
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kStructTypeName);
};

// -------------------------------------------------------------------------------------------------
// Adding scalars
// -------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueScalarToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName, /*add to selected*/ false);

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueScalarItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding another scalar to "selected" item as field
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName, /*add to selected*/ true);  // no-op

  // since our context doesn't report any selection, the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding scalar as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueScalarToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueScalar(sup::dto::kInt32TypeName, /*add to selected*/ true);

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetDisplayName(), sup::dto::kInt32TypeName);
  EXPECT_EQ(inserted_item->GetAnyTypeName(), sup::dto::kInt32TypeName);
};

// -------------------------------------------------------------------------------------------------
// Adding array
// -------------------------------------------------------------------------------------------------

//! Adding a scalar to an empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueArrayToEmptyModel)
{
  // creating action for the context, when nothing is selected by the user
  auto actions = CreateActions(nullptr);

  // adding AnyValueItem struct as top level item
  actions->OnAddAnyValueArray(/*add to selected*/ false);

  // validating that model got top level item of the correct type
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  auto inserted_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(&m_model);
  ASSERT_NE(inserted_item, nullptr);
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kArrayTypeName);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding another array to "selected" item as field
  actions->OnAddAnyValueArray(/*add to selected*/ true);  // no-op

  // since our context doesn't report any selection, the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};

//! Adding array as a field to another structure (which is marked as selected).

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueArrayToStruct)
{
  auto parent = m_model.InsertItem<sup::gui::AnyValueStructItem>();

  // creating action for the context, when parent is selected
  auto actions = CreateActions(parent);

  // expecting no callbacks
  EXPECT_CALL(m_listener, OnCallback(_)).Times(0);

  // adding AnyValueItem struct as a field
  actions->OnAddAnyValueArray(/*add to selected*/ true);

  // validating that parent got new child
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
  ASSERT_EQ(parent->GetChildren().size(), 1);

  auto inserted_item = parent->GetChildren().at(0);
  EXPECT_EQ(inserted_item->GetType(), std::string("AnyValueArray"));
  EXPECT_EQ(inserted_item->GetDisplayName(), ::sup::gui::kArrayTypeName);
};
