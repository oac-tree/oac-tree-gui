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

#include <sup/gui/style/style_helper.h>

#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QSlider>

namespace oac_tree_gui
{

namespace
{

const int kZoomMinValue = 1;
const int kZoomMaxValue = 200;
const int kZoomInitialValue = 100;
const QString kPercentSign("%");

/**
 * @brief Returns slider value from the given zoom factor.
 */
int GetSliderValue(double zoom_factor)
{
  return static_cast<int>(zoom_factor * 100.0);
}

/**
 * @brief Returns zoom factor from the given slider value.
 */
double GetZoomFactor(int slider_value)
{
  return slider_value / 100.0;
}

/**
 * @brief Returns text for the label representing zoom factor.
 */
QString GetZoomText(double zoom_factor)
{
  const int kFieldCount = 3;
  int zoom_value = static_cast<int>(zoom_factor*100);
  return QString(" %1%2").arg(zoom_value, kFieldCount).arg(kPercentSign);
}

}  // namespace

NodeEditorNavigationToolBar::NodeEditorNavigationToolBar(QWidget *parent_widget)
    : QToolBar(parent_widget), m_zoom_slider(new QSlider), m_zoom_label(new QLabel)
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  InsertStrech();

  m_zoom_slider->setRange(kZoomMinValue, kZoomMaxValue);
  m_zoom_slider->setOrientation(Qt::Horizontal);
  m_zoom_slider->setRange(kZoomMinValue, kZoomMaxValue);
  m_zoom_slider->setMaximumWidth(mvvm::utils::UnitSize(10));
  m_zoom_slider->setToolTip("Zoom (also Ctrl+scroll)");
  m_zoom_label->setFixedWidth(mvvm::utils::UnitSize(3));

  addWidget(m_zoom_slider);
  addWidget(m_zoom_label);

  auto on_slider_changed = [this](int value)
  {
    qDebug() << "on_slider_value_changed" << value << GetZoomFactor(value) << GetZoomText(GetZoomFactor(value));
    m_zoom_label->setText(GetZoomText(GetZoomFactor(value)));
    emit ZoomFactorRequest(GetZoomFactor(value));
  };
  connect(m_zoom_slider, &QSlider::valueChanged, on_slider_changed);

  SetZoomFactor(1.0);
}

void NodeEditorNavigationToolBar::SetZoomFactor(double value)
{
  m_zoom_slider->setValue(GetSliderValue(value));
}

void NodeEditorNavigationToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace oac_tree_gui
