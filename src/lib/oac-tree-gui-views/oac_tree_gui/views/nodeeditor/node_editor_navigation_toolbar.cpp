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

NodeEditorNavigationToolBar::NodeEditorNavigationToolBar(QWidget *parent_widget)
    : QToolBar(parent_widget)
    , m_zoom_slider(new QSlider)
    , m_zoom_label(new QLabel)
    , m_zoom_factor_converter(constants::kMinZoomFactor, constants::kMaxZoomFactor)
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  InsertStrech();

  m_zoom_slider->setRange(m_zoom_factor_converter.GetSliderMinValue(),
                          m_zoom_factor_converter.GetSliderMaxValue());

  m_zoom_slider->setOrientation(Qt::Horizontal);
  m_zoom_slider->setMaximumWidth(mvvm::utils::UnitSize(10));
  m_zoom_slider->setToolTip("Zoom (also Ctrl+scroll)");
  m_zoom_label->setFixedWidth(mvvm::utils::UnitSize(3));

  addWidget(m_zoom_slider);
  addWidget(m_zoom_label);

  auto on_slider_changed = [this](int value)
  {
    const double zoom_factor = m_zoom_factor_converter.GetZoomFactor(value);
    m_zoom_label->setText(ZoomFactorConverter::GetZoomText(zoom_factor));
    if (m_is_interactive)
    {
      emit ZoomFactorRequest(zoom_factor);
    }
  };
  connect(m_zoom_slider, &QSlider::valueChanged, this, on_slider_changed);

  connect(m_zoom_slider, &QSlider::sliderPressed, this, [this]() { m_is_interactive = true; });
  connect(m_zoom_slider, &QSlider::sliderReleased, this, [this]() { m_is_interactive = false; });

  SetZoomFactor(1.0);
}

void NodeEditorNavigationToolBar::SetZoomFactor(double zoom_factor)
{
  m_zoom_slider->setValue(m_zoom_factor_converter.GetSliderValue(zoom_factor));
}

void NodeEditorNavigationToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

}  // namespace oac_tree_gui
