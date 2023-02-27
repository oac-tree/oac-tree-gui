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

#include <sup/dto/anytype.h>
#include <sup/gui/dto/anyvalue_item.h>

#include <stdexcept>

using namespace sup::gui;

class AnyValueItemUtilsTests : public ::testing::Test
{
public:
};

TEST_F(AnyValueItemUtilsTests, UpdateAnyValueItemScalarData)
{
  {  // empty items
    AnyValueEmptyItem source;
    AnyValueEmptyItem target;
    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(sup::dto::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(sup::dto::kInt8TypeName);

    EXPECT_THROW(UpdateAnyValueItemScalarData(source, target), std::logic_error);
  }

  {  // two scalars, same type
    AnyValueScalarItem source;
    source.SetAnyTypeName(sup::dto::kInt32TypeName);
    source.SetData(42);

    AnyValueScalarItem target;
    target.SetAnyTypeName(sup::dto::kInt32TypeName);
    target.SetData(0);
    EXPECT_EQ(target.Data<int>(), 0);

    EXPECT_NO_THROW(UpdateAnyValueItemScalarData(source, target));
    EXPECT_EQ(target.Data<int>(), 42);
  }
}
