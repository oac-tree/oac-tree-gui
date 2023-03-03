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

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>

#include <sup/gui/core/anyvalue_item.h>

#include <anyvalueeditor/anyvalue_editor_context.h>
#include <gtest/gtest.h>

using namespace anyvalueeditor;

class AnyValueEditorActionsTest : public ::testing::Test
{
public:
  //! Creates context necessary for AnyValueEditActions to function.
  AnyValueEditorContext CreateContext(sup::gui::AnyValueItem* item)
  {
    // callback returns given item, pretending it is user's selection
    auto get_selected_callback = [item]() { return item; };
    return {get_selected_callback, {}};
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<AnyValueEditorActions> CreateActions(sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActions>(CreateContext(selection), &m_model, nullptr);
  }

  mvvm::ApplicationModel m_model;
};

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
  EXPECT_NE(inserted_item, nullptr);

  // adding another AnyValueItem to "selected" item as field
  actions->OnAddAnyValueStruct(/*add to selected*/ true);

  // since our context doesn't report any selection, the amount of items should stay the same
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
};
