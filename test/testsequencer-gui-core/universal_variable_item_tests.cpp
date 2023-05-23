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

#include "sequencergui/model/universal_variable_item.h"

#include <sequencergui/domain/domain_utils.h>

#include <sup/sequencer/variable.h>

#include <gtest/gtest.h>

using namespace sequencergui;

//! Tests UniversalVariableItem.

class UniversalVariableItemTests : public ::testing::Test
{
};

TEST_F(UniversalVariableItemTests, InitialState)
{
  sequencergui::UniversalVariableItem item;

  EXPECT_TRUE(item.GetName().empty());
  //  EXPECT_TRUE(item.GetDisplayName().empty());
  EXPECT_TRUE(item.GetDomainType().empty());
  EXPECT_EQ(item.GetAnyValueItem(), nullptr);
  EXPECT_TRUE(item.IsAvailable());
}

//! Initialisation from default constructed domain variable.
TEST_F(UniversalVariableItemTests, InitFromDomain)
{
  auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);

  sequencergui::UniversalVariableItem item;
  item.InitFromDomain(local_variable.get());

  EXPECT_EQ(item.GetDomainType(), domainconstants::kLocalVariableType);
}
