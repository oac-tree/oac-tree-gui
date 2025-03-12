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

#include "zoom_factor_converter.h"

#include <cmath>

namespace oac_tree_gui
{

namespace
{
const QString kPercentSign("%");
}

ZoomFactorConverter::ZoomFactorConverter(double zoom_factor_min, double zoom_factor_max)
    : m_zoom_factor_min(zoom_factor_min), m_zoom_factor_max(zoom_factor_max)
{
}

int ZoomFactorConverter::GetSliderMinValue() const
{
  return kSliderMinValue;
}

int ZoomFactorConverter::GetSliderMaxValue() const
{
  return kSliderMaxValue;
}

double ZoomFactorConverter::GetZoomFactorMin() const
{
  return m_zoom_factor_min;
}

double ZoomFactorConverter::GetZoomFactorMax() const
{
  return m_zoom_factor_max;
}

int ZoomFactorConverter::GetSliderValue(double zoom_factor) const
{
  const double ratio =
      (kSliderMaxValue - kSliderMinValue) / (m_zoom_factor_max - m_zoom_factor_min);
  return kSliderMinValue + ratio * (zoom_factor - m_zoom_factor_min);
}

double ZoomFactorConverter::GetZoomFactor(int slider_value) const
{
  const double ratio =
      (m_zoom_factor_max - m_zoom_factor_min) / (kSliderMaxValue - kSliderMinValue);
  return m_zoom_factor_min + ratio * (slider_value - kSliderMinValue);
}

QString ZoomFactorConverter::GetZoomText(double zoom_factor)
{
  const int kFieldCount = 3;
  const int percents = static_cast<int>(zoom_factor * 100);
  return QString("%1%2").arg(percents, kFieldCount).arg(kPercentSign);
}

}  // namespace oac_tree_gui
