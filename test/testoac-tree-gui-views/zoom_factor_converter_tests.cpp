/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/views/nodeeditor/zoom_factor_converter.h"

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of ZoomFactorConverter class.
 */
class ZoomFactorConverterTest : public ::testing::Test
{
};

TEST_F(ZoomFactorConverterTest, InitialState)
{
  const ZoomFactorConverter converter(1.0, 2.0);

  EXPECT_EQ(converter.GetZoomFactorMin(), 1.0);
  EXPECT_EQ(converter.GetZoomFactorMax(), 2.0);

  EXPECT_EQ(converter.GetSliderMinValue(), ZoomFactorConverter::kSliderMinValue);
  EXPECT_EQ(converter.GetSliderMaxValue(), ZoomFactorConverter::kSliderMaxValue);
}

TEST_F(ZoomFactorConverterTest, GetSliderText)
{
  const ZoomFactorConverter converter(1.0, 2.0);
  EXPECT_EQ(converter.GetSliderText(1.0), QString("100%"));
  EXPECT_EQ(converter.GetSliderText(0.99), QString(" 99%"));
  EXPECT_EQ(converter.GetSliderText(0.1), QString(" 10%"));
}

TEST_F(ZoomFactorConverterTest, GetSliderValue)
{
  const double zoom_factor_min{0.1};
  const double zoom_factor_max{2.0};

  const ZoomFactorConverter converter(zoom_factor_min, zoom_factor_max);

  EXPECT_EQ(converter.GetSliderValue(zoom_factor_min), converter.GetSliderMinValue());
  EXPECT_EQ(converter.GetSliderValue(zoom_factor_max), converter.GetSliderMaxValue());
}

TEST_F(ZoomFactorConverterTest, GetZoomFactor)
{
  const double zoom_factor_min{0.1};
  const double zoom_factor_max{2.0};

  const ZoomFactorConverter converter(zoom_factor_min, zoom_factor_max);

  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(converter.GetSliderMinValue()), zoom_factor_min);
  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(converter.GetSliderMaxValue()), zoom_factor_max);
}

}  // namespace oac_tree_gui::test
