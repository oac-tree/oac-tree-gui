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

namespace oac_tree_gui
{

/**
 * @brief The ZoomFactorConverter class is a helper to convert the slider's linear limits to zoom
 * factor.
 *
 * Zoom factor is used by NodeGraphicsView to zoom in and out. The value over 1.0 zooms into the
 * view (objects look large). Zoom value between 0.0 and 1.0 zooms out of the view (objects look
 * small).
 */
class ZoomFactorConverter
{
public:
  inline static const int kSliderMinValue = 10;
  inline static const int kSliderMaxValue = 200;

  /**
   * @brief Main c-tor with zoom limits defined.
   *
   * @param zoom_factor_min The minimum possible value of the zoom factor.
   * @param zoom_factor_max The maximum possible value of the zoom factor.
   */
  ZoomFactorConverter(double zoom_factor_min, double zoom_factor_max);

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
   * Example: "100.0%", "  9.3%".
   */
  static QString GetZoomText(double zoom_factor);

private:
  double m_zoom_factor_min{0};
  double m_zoom_factor_max{0};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_NODEEDITOR_ZOOM_FACTOR_CONVERTER_H_
