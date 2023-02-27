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

#include "sup/gui/dto/anyvalue_item_utils.h"

#include <gtest/gtest.h>

#include <sup/gui/dto/anyvalue_item.h>

using namespace sup::gui;

class AnyValueItemUtilsTests : public ::testing::Test
{
public:
};

TEST_F(AnyValueItemUtilsTests, InitialState)
{

  EXPECT_EQ(1, 1);
}
