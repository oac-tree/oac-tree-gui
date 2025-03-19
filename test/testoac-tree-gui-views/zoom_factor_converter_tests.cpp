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

#include <oac_tree_gui/core/exceptions.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Tests of ZoomFactorConverter class.
 */
class ZoomFactorConverterTest : public ::testing::Test
{
};

TEST_F(ZoomFactorConverterTest, CeateZoomPoints)
{
  const double zoom_min{0.1};
  const double zoom_max{1.0};
  const size_t nbins{10};

  EXPECT_TRUE(CreateZoomPoints(zoom_min, zoom_max, 0).empty());

  auto values = CreateZoomPoints(zoom_min, zoom_max, 10);
  EXPECT_EQ(values.size(), nbins);
  EXPECT_DOUBLE_EQ(values[0], zoom_min);
  EXPECT_DOUBLE_EQ(values[nbins - 1], zoom_max);
  EXPECT_DOUBLE_EQ(values[4], 0.5);
}

TEST_F(ZoomFactorConverterTest, InitialState)
{
  // zoom value   :  0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
  // slider index :  0   1   2   3   4   5   6   7   8   9
  const double zoom_min{0.1};
  const double zoom_max{1.0};
  const size_t nbins{10};

  EXPECT_THROW(ZoomFactorConverter({}), RuntimeException);

  const ZoomFactorConverter converter(CreateZoomPoints(zoom_min, zoom_max, nbins));

  EXPECT_DOUBLE_EQ(converter.GetZoomFactorMin(), zoom_min);
  EXPECT_DOUBLE_EQ(converter.GetZoomFactorMax(), zoom_max);

  EXPECT_EQ(converter.GetSliderMinValue(), 0);
  EXPECT_EQ(converter.GetSliderMaxValue(), nbins - 1);

  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(0), zoom_min);
  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(nbins - 1), zoom_max);
  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(4), 0.5);

  // values outside of allowed slider range are forced to zoom min and max
  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(-1), zoom_min);
  EXPECT_DOUBLE_EQ(converter.GetZoomFactor(999), zoom_max);
}

TEST_F(ZoomFactorConverterTest, GetZoomText)
{
  const ZoomFactorConverter converter({1.0, 2.0});
  EXPECT_EQ(converter.GetZoomText(2.0), QString("200%"));
  EXPECT_EQ(converter.GetZoomText(1.0), QString("100%"));
  EXPECT_EQ(converter.GetZoomText(0.99), QString(" 99%"));
  EXPECT_EQ(converter.GetZoomText(0.1), QString(" 10%"));
}

TEST_F(ZoomFactorConverterTest, GetSliderValue)
{
  const double zoom_min{0.1};
  const double zoom_max{1.0};
  const size_t nbins{10};

  EXPECT_THROW(ZoomFactorConverter({}), RuntimeException);

  const ZoomFactorConverter converter(CreateZoomPoints(zoom_min, zoom_max, nbins));

  EXPECT_EQ(converter.GetSliderValue(zoom_min), converter.GetSliderMinValue());
  EXPECT_EQ(converter.GetSliderValue(0.5), 4);
  EXPECT_EQ(converter.GetSliderValue(zoom_max), converter.GetSliderMaxValue());

  // zoom values outside of the allowed range reports closest slider position
  EXPECT_EQ(converter.GetSliderValue(0.05), converter.GetSliderMinValue());
  EXPECT_EQ(converter.GetSliderValue(2.0), converter.GetSliderMaxValue());
}

}  // namespace oac_tree_gui::test
