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

#include <gtest/gtest.h>

#include <mvvm/model/application_model.h>

using namespace anyvalueeditor;

class AnyValueEditorActionsTest : public ::testing::Test
{
public:
  //! Creates a callback that mimics the selection of AnyValueItem by the user.
  //! This callback is necessary for AnyValueEditActions to function.
  static AnyValueEditorActions::callback_t CreateCallback(sup::gui::AnyValueItem* item)
  {
    return [item]() { return item; };
  }

  //! Creates AnyValueEditorActions for testing.
  std::unique_ptr<AnyValueEditorActions> CreateActions(sup::gui::AnyValueItem* selection)
  {
    return std::make_unique<AnyValueEditorActions>(&m_model, nullptr, CreateCallback(selection));
  }

  mvvm::ApplicationModel m_model;
};


//! Adding structure to empty model.

TEST_F(AnyValueEditorActionsTest, OnAddAnyValueStructToEmptyModel)
{
  // create
  auto actions = CreateActions(nullptr);
};
