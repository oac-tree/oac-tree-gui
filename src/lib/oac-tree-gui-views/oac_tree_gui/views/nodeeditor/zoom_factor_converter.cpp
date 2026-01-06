/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "zoom_factor_converter.h"

#include <oac_tree_gui/core/exceptions.h>

#include <algorithm>
#include <cmath>

namespace oac_tree_gui
{

namespace
{
const QString kPercentSign("%");
}

ZoomFactorConverter::ZoomFactorConverter(const std::vector<double>& zoom_values)
    : m_zoom_values(zoom_values)
{
  if (m_zoom_values.empty())
  {
    throw RuntimeException("Empty zoom values");
  }
}

int ZoomFactorConverter::GetSliderMinValue() const
{
  return 0;
}

int ZoomFactorConverter::GetSliderMaxValue() const
{
  return static_cast<int>(m_zoom_values.size() - 1);
}

double ZoomFactorConverter::GetZoomFactorMin() const
{
  return m_zoom_values.front();
}

double ZoomFactorConverter::GetZoomFactorMax() const
{
  return m_zoom_values.back();
}

int ZoomFactorConverter::GetSliderValue(double zoom_factor) const
{
  auto iter = std::upper_bound(m_zoom_values.begin(), m_zoom_values.end(), zoom_factor);
  return iter == m_zoom_values.begin()
             ? 0
             : static_cast<int>(std::distance(m_zoom_values.begin(), std::prev(iter)));
}

double ZoomFactorConverter::GetZoomFactor(int slider_value) const
{
  const int fixed_slider_value = std::clamp(slider_value, GetSliderMinValue(), GetSliderMaxValue());
  return m_zoom_values.at(static_cast<size_t>(fixed_slider_value));
}

QString ZoomFactorConverter::GetZoomText(double zoom_factor)
{
  const int kFieldCount = 3;
  const int percents = static_cast<int>(zoom_factor * 100);
  const int base = 10;
  return QString("%1%2").arg(percents, kFieldCount, base, QChar(' ')).arg(kPercentSign);
}

std::vector<double> CreateZoomPoints(double zoom_min, double zoom_max, size_t nbins)
{
  if (nbins == 0)
  {
    return {};
  }

  std::vector<double> result;
  result.reserve(nbins);

  const double dx = (zoom_max - zoom_min) / static_cast<double>((nbins - 1));

  for (size_t i = 0; i < nbins; ++i)
  {
    result.push_back(zoom_min + (static_cast<double>(i) * dx));
  }

  return result;
}

}  // namespace oac_tree_gui
