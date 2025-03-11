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

#include "node_editor_navigation_toolbar.h"

#include <oac_tree_gui/nodeeditor/scene_constants.h>

#include <sup/gui/style/style_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QSlider>

namespace oac_tree_gui
{

namespace
{

const int kSliderMinValue = 0;
const int kSliderMaxValue = 100;
const QString kPercentSign("%");

/**
 * @brief Returns slider value from the given zoom factor.
 */
int GetSliderValue(double zoom_factor)
{
  const double ratio =
      (kSliderMaxValue - kSliderMinValue) / (constants::kMaxZoomFactor - constants::kMinZoomFactor);
  qDebug() << "ratio " << ratio << zoom_factor << kSliderMinValue + ratio * (zoom_factor - constants::kMinZoomFactor);
  return kSliderMinValue + ratio * (zoom_factor - constants::kMinZoomFactor);
}

/**
 * @brief Returns zoom factor from the given slider value.
 */
double GetZoomFactor(int slider_value)
{
  const double ratio =
      (constants::kMaxZoomFactor - constants::kMinZoomFactor) / (kSliderMaxValue - kSliderMinValue);
  return constants::kMinZoomFactor + ratio * (slider_value - kSliderMinValue);
}

/**
 * @brief Returns text for the label representing zoom factor.
 */
QString GetZoomText(double zoom_factor)
{
  const int kFieldCount = 3;
  const int percents = static_cast<int>(zoom_factor * 100);
  return QString(" %1%2").arg(percents, kFieldCount).arg(kPercentSign);
}

}  // namespace

NodeEditorNavigationToolBar::NodeEditorNavigationToolBar(QWidget *parent_widget)
    : QToolBar(parent_widget), m_zoom_slider(new QSlider), m_zoom_label(new QLabel)
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  InsertStrech();

  m_zoom_slider->setRange(kSliderMinValue, kSliderMaxValue);
  m_zoom_slider->setOrientation(Qt::Horizontal);
  m_zoom_slider->setMaximumWidth(mvvm::utils::UnitSize(10));
  m_zoom_slider->setToolTip("Zoom (also Ctrl+scroll)");
  m_zoom_label->setFixedWidth(mvvm::utils::UnitSize(3));

  addWidget(m_zoom_slider);
  addWidget(m_zoom_label);

  auto on_slider_changed = [this](int value)
  {
    qDebug() << "on_slider_value_changed" << value << GetZoomFactor(value) << m_is_interactive
             << GetZoomText(GetZoomFactor(value));
    m_zoom_label->setText(GetZoomText(GetZoomFactor(value)));
    if (m_is_interactive)
    {
      qDebug() << "on_slider_value_changed emiting";
      emit ZoomFactorRequest(GetZoomFactor(value));
    }
  };
  connect(m_zoom_slider, &QSlider::valueChanged, this, on_slider_changed);

  connect(m_zoom_slider, &QSlider::sliderPressed, this, [this]() { m_is_interactive = true; });
  connect(m_zoom_slider, &QSlider::sliderReleased, this, [this]() { m_is_interactive = false; });

  SetZoomFactor(1.0);
}

void NodeEditorNavigationToolBar::SetZoomFactor(double value)
{
  qDebug() << "XZXX " << value << GetSliderValue(value);
  m_zoom_slider->setValue(GetSliderValue(value));
}

void NodeEditorNavigationToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace oac_tree_gui
