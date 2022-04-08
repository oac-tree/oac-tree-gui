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

#include "sequencergui/viewmodel/selectionmodel.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"

#include <gtest/gtest.h>

using namespace sequencergui;

class SelectionModelTest : public ::testing::Test
{
public:
  SelectionModelTest() : m_view_model(&m_model), m_selection_model(&m_view_model) {}

  mvvm::ApplicationModel m_model;
  mvvm::TopItemsViewModel m_view_model;
  sequencergui::SelectionModel m_selection_model;
};

TEST_F(SelectionModelTest, InitialState)
{
  EXPECT_EQ(m_selection_model.GetSelectedItem(), nullptr);
  EXPECT_TRUE(m_selection_model.GetSelectedItems().empty());
}
