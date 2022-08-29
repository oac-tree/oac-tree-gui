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

#include "anyvalueeditor/domain_anyvalue_builder.h"

#include <anyvalueeditor/anyvalue_item.h>
#include <gtest/gtest.h>
#include <sup/dto/anyvalue.h>

using namespace anyvalueeditor;

class DomainAnyValueBuilderTest : public ::testing::Test
{
public:
  sup::dto::AnyValue CreateAnyValue(const AnyValueItem& item)
  {
    DomainAnyValueBuilder builder(item);
    return builder.GetAnyValue();
  }
};

//! Build from empty AnyValueItem.

TEST_F(DomainAnyValueBuilderTest, EmptyValue)
{
  AnyValueItem item;

  auto any_value = CreateAnyValue(item);
  EXPECT_TRUE(sup::dto::IsEmptyValue(any_value));
}

//! Build AnyValue from scalar.

TEST_F(DomainAnyValueBuilderTest, FromScalar)
{
  AnyValueItem item;

  sup::dto::AnyValue expected_anyvalue{sup::dto::SignedInteger32Type, 42};
}
