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

#ifndef OAC_TREE_GUI_VIEWS_NODEEDITOR_ZOOM_FACTOR_CONVERTER_H_
#define OAC_TREE_GUI_VIEWS_NODEEDITOR_ZOOM_FACTOR_CONVERTER_H_

#include <QString>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The ZoomFactorConverter class is a helper to convert QSLider linear integer values to
 * zoom factor and back.
 *
 * Zoom factor is used by NodeGraphicsView to zoom in and out. The value over 1.0 zooms into the
 * view (objects look large). Zoom value between 0.0 and 1.0 zooms out of the view (objects look
 * small).
 *
 * This class relies on a provided vector of sorted zoom values and simply uses this array to
 * convert zoom factor to index (corresponding to QSlider values) and back. No interpolation is
 * provided.
 */
class ZoomFactorConverter
{
public:
  /**
   * @brief Main c-tor with zoom values.
   *
   * @param zoom_values Sorted vector of all zoom values.
   */
  explicit ZoomFactorConverter(const std::vector<double>& zoom_values);

  /**
   * @brief Returns min value of slider's range.
   */
  int GetSliderMinValue() const;

  /**
   * @brief Returns max value of slider's range.
   */
  int GetSliderMaxValue() const;

  /**
   * @brief Returns minimum allowed value of the zoom factor.
   */
  double GetZoomFactorMin() const;

  /**
   * @brief Returns minimum allowed value of the zoom factor.
   */
  double GetZoomFactorMax() const;

  /**
   * @brief Returns slider value from the given zoom factor.
   */
  int GetSliderValue(double zoom_factor) const;

  /**
   * @brief Returns zoom factor from the given slider value.
   */
  double GetZoomFactor(int slider_value) const;

  /**
   * @brief Returns text for the label representing zoom factor.
   *
   * Example:
   * 1.0  -> "100.0%"
   * 0.93 ->"  9.3%".
   */
  static QString GetZoomText(double zoom_factor);

private:
  std::vector<double> m_zoom_values;
};

/**
 * @brief Creates vector of zoom values evenly distributed in the given interval.
 *
 * @param zoom_min Min available zoom value (corresponds to array index 0)
 * @param zoom_max Max available zoom value (corresponds to array index nbins-1)
 * @param nbins Length of array with values
 */
std::vector<double> CreateZoomPoints(double zoom_min, double zoom_max, size_t nbins);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_ZOOM_FACTOR_CONVERTER_H_
