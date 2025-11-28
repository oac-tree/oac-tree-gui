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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "node_editor_navigation_toolbar.h"

#include <oac_tree_gui/nodeeditor/scene_constants.h>
#include <oac_tree_gui/nodeeditor/scene_utils.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/widgets/action_menu.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QAction>
#include <QMenu>
#include <QSlider>

namespace oac_tree_gui
{

namespace
{

void AppendRange(std::vector<double>& values, double zmin, double zmax, int npoints)
{
  const std::vector<double> range = CreateZoomPoints(zmin, zmax, npoints);
  values.insert(values.end(), range.begin(), range.end());
}

/**  QAction* m_center_action{nullptr};
 * @brief Creates a vector with slider zoom value.
 */
std::vector<double> CreateSliderPoints()
{
  static const std::vector<double> kZoomStops = oac_tree_gui::GetDefaultZoomStops();
  std::vector<double> result;
  for (size_t i_point = 0; i_point < kZoomStops.size() - 1; ++i_point)
  {
    // make slider for zoom<1.0 longer than for zoom >1.0
    const int npoints = kZoomStops[i_point] < 1.0 ? 100 : 50;
    AppendRange(result, kZoomStops[i_point], kZoomStops[i_point + 1], npoints);
  }
  return result;
}

}  // namespace

NodeEditorNavigationToolBar::NodeEditorNavigationToolBar(QWidget* parent_widget)
    : QToolBar(parent_widget)
    , m_zoom_slider(new QSlider)
    , m_zoom_factor_converter(CreateSliderPoints())
    , m_center_action(new QAction(this))
    , m_fit_to_view_action(new QAction(this))
    , m_zoom_action(new sup::gui::ActionMenu(this))
    , m_zoom_menu(CreateZoomMenu())
{
  setIconSize(NarrowToolBarIconSize());

  InsertStretch();

  SetupActions();
  SetupSlider();

  addAction(m_action_map.GetAction(ActionKey::kCenter));
  addAction(m_action_map.GetAction(ActionKey::kFitToView));
  addWidget(m_zoom_slider);
  addAction(m_action_map.GetAction(ActionKey::kFixedZoomLevel));
}

void NodeEditorNavigationToolBar::SetZoomFactor(double zoom_factor)
{
  m_zoom_slider->setValue(m_zoom_factor_converter.GetSliderValue(zoom_factor));
}

void NodeEditorNavigationToolBar::InsertStretch()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

void NodeEditorNavigationToolBar::SetupSlider()
{
  m_zoom_slider->setRange(m_zoom_factor_converter.GetSliderMinValue(),
                          m_zoom_factor_converter.GetSliderMaxValue());

  m_zoom_slider->setOrientation(Qt::Horizontal);
  m_zoom_slider->setMaximumWidth(mvvm::style::UnitSize(10));
  m_zoom_slider->setToolTip("Zoom (also Ctrl+scroll)");

  auto on_slider_changed = [this](int value)
  {
    const double zoom_factor = m_zoom_factor_converter.GetZoomFactor(value);
    const auto text = ZoomFactorConverter::GetZoomText(zoom_factor);
    m_zoom_action->setText(text);
    emit ZoomFactorRequest(zoom_factor);
  };
  connect(m_zoom_slider, &QSlider::valueChanged, this, on_slider_changed);

  SetZoomFactor(1.0);
}

void NodeEditorNavigationToolBar::SetupActions()
{
  m_center_action->setText("Center");
  m_center_action->setIcon(FindIcon("camera-metering-center"));
  m_center_action->setToolTip("Center view (zoom unchanged, move where items are)");
  connect(m_center_action, &QAction::triggered, this,
          &NodeEditorNavigationToolBar::CenterViewRequest);
  m_action_map.Add(ActionKey::kCenter, m_center_action);

  m_fit_to_view_action->setText("Fit in view");
  m_fit_to_view_action->setIcon(FindIcon("fit-to-page-outline"));
  m_fit_to_view_action->setToolTip("Fit in view");
  connect(m_fit_to_view_action, &QAction::triggered, this,
          &NodeEditorNavigationToolBar::FitToViewRequest);
  m_action_map.Add(ActionKey::kFitToView, m_fit_to_view_action);

  m_zoom_action->setMenu(m_zoom_menu.get());
  m_zoom_action->setToolTip("Zoom");
  m_action_map.Add(ActionKey::kFixedZoomLevel, m_zoom_action);
}

std::unique_ptr<QMenu> NodeEditorNavigationToolBar::CreateZoomMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  std::map<double, QAction*> zoom_factor_to_action;
  for (auto zoom_factor : oac_tree_gui::GetDefaultZoomStops())
  {
    auto text = ZoomFactorConverter::GetZoomText(zoom_factor);
    auto action = result->addAction(text);
    zoom_factor_to_action[zoom_factor] = action;
    auto on_action = [this, zoom_factor, text]() { SetZoomFactor(zoom_factor); };
    connect(action, &QAction::triggered, this, on_action);
  }

  result->setActiveAction(zoom_factor_to_action[1.0]);
  return result;
}

}  // namespace oac_tree_gui
