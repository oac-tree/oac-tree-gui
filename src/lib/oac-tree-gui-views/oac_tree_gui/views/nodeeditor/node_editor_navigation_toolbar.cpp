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
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/style/style_helper.h>
#include <sup/gui/widgets/action_menu.h>

#include <mvvm/widgets/widget_utils.h>

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSlider>

namespace oac_tree_gui
{

namespace
{

/**  QAction* m_center_action{nullptr};

 * @brief Creates a vector with slider zoom value.
 */
std::vector<double> CreateSliderPoints()
{
  // dirst 200 points of the slider with be for zoom < 1.0, only 50 points for zoom >1.0
  std::vector<double> range1 = CreateZoomPoints(constants::kMinZoomFactor, 1.0, 200);
  const std::vector<double> range2 = CreateZoomPoints(1.0, constants::kMaxZoomFactor, 50);
  range1.insert(range1.end(), range2.begin(), range2.end());
  return range1;
}
}  // namespace

NodeEditorNavigationToolBar::NodeEditorNavigationToolBar(QWidget *parent_widget)
    : QToolBar(parent_widget)
    , m_zoom_slider(new QSlider)
    , m_zoom_label(new QLabel)
    , m_zoom_factor_converter(CreateSliderPoints())
    , m_center_action(new QAction(this))
    , m_fit_to_view_action(new QAction(this))
    , m_zoom_action(new sup::gui::ActionMenu(this))
    , m_zoom_menu(CreateZoomMenu())
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  InsertStrech();

  SetupActions();
  SetupSlider();
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

void NodeEditorNavigationToolBar::SetupSlider()
{
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
    const auto text = ZoomFactorConverter::GetZoomText(zoom_factor);
    m_zoom_label->setText(text);
    m_zoom_action->setText(text);
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

void NodeEditorNavigationToolBar::SetupActions()
{
  m_center_action->setText("Center");
  m_center_action->setIcon(FindIcon("camera-metering-center"));
  m_center_action->setToolTip("Center view");
  connect(m_center_action, &QAction::triggered, this,
          &NodeEditorNavigationToolBar::CenterViewRequest);
  addAction(m_center_action);

  m_fit_to_view_action->setText("Fit in view");
  m_fit_to_view_action->setIcon(FindIcon("fit-to-page-outline"));
  m_fit_to_view_action->setToolTip("Fit in view");
  connect(m_fit_to_view_action, &QAction::triggered, this,
          &NodeEditorNavigationToolBar::FitToViewRequest);
  addAction(m_fit_to_view_action);

  m_zoom_action->setMenu(m_zoom_menu.get());
  m_zoom_action->setToolTip("Zoom");
  addAction(m_zoom_action);
}

std::unique_ptr<QMenu> NodeEditorNavigationToolBar::CreateZoomMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  const std::vector<double> scales = {0.25, 0.5, 0.75, 1.0, 1.5, 2.0};
  std::map<double, QAction *> scale_to_action;
  for (auto scale : scales)
  {
    auto text = ZoomFactorConverter::GetZoomText(scale);
    auto action = result->addAction(text);
    scale_to_action[scale] = action;
    auto on_action = [this, scale, text]()
    {
      SetZoomFactor(scale);
      m_zoom_action->setText(text);
    };
    connect(action, &QAction::triggered, this, on_action);
  }

  result->setActiveAction(scale_to_action[1.0]);
  return result;
}

}  // namespace oac_tree_gui
