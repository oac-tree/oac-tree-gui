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

#include "composer_combo_panel_toolbar.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/style/style_helper.h>

#include <sup/gui/style/style_helper.h>

#include <mvvm/style/mvvm_style_helper.h>

#include <QAction>
#include <QComboBox>
#include <QMenu>
#include <QToolButton>

namespace oac_tree_gui
{

namespace
{
constexpr auto kNoProcedureSelected = " < no procedure >";
}

ComposerComboPanelToolBar::ComposerComboPanelToolBar(const ProceduresCallback& procedure_callback,
                                                     QWidget* parent_widget)
    : QToolBar(parent_widget)
    , m_available_procedures(procedure_callback)
    , m_select_procedure_menu(CreateSelectProcedureMenu())
    , m_select_procedure_button(new QToolButton(this))
    , m_split_horizontally_button(new QToolButton(this))
    , m_close_current_view_button(new QToolButton(this))
{
  setIconSize(sup::gui::utils::NarrowToolBarIconSize());

  m_select_procedure_button->setText(kNoProcedureSelected);
  // m_select_procedure_action->setIcon(FindIcon("file-plus-outline"));
  m_select_procedure_button->setMenu(m_select_procedure_menu.get());
  m_select_procedure_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_select_procedure_button->setPopupMode(QToolButton::MenuButtonPopup);
  m_select_procedure_button->setToolTip(
      "Select on of the existing procedures for editing in this widget");
  m_select_procedure_button->setAutoRaise(true);
  m_select_procedure_button->setFocusPolicy(Qt::NoFocus);
  m_select_procedure_button->setMaximumWidth(mvvm::style::UnitSize(25));
  addWidget(m_select_procedure_button);

  InsertStretch();

  m_split_horizontally_button->setToolTip("Split current view to the right");
  m_split_horizontally_button->setIcon(FindIcon("view-split-left-right-16"));
  m_split_horizontally_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  connect(m_split_horizontally_button, &QToolButton::clicked, this,
          &ComposerComboPanelToolBar::splitViewRequest);
  addWidget(m_split_horizontally_button);

  m_close_current_view_button->setToolTip("Close this view");
  m_close_current_view_button->setIcon(FindIcon("dialog-close-16"));
  m_close_current_view_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  connect(m_close_current_view_button, &QToolButton::clicked, this,
          &ComposerComboPanelToolBar::closeViewRequest);
  addWidget(m_close_current_view_button);

  // make buttons closer to each other
  // setStyleSheet("QToolButton { padding: 0px; margin:0px; }");
}

ComposerComboPanelToolBar::~ComposerComboPanelToolBar() = default;

void ComposerComboPanelToolBar::InsertStretch()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

std::unique_ptr<QMenu> ComposerComboPanelToolBar::CreateSelectProcedureMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);
  connect(result.get(), &QMenu::aboutToShow, this, &ComposerComboPanelToolBar::OnAboutToShowMenu);
  return result;
}

void ComposerComboPanelToolBar::OnAboutToShowMenu()
{
  if (!m_available_procedures)
  {
    return;
  }

  auto menu = m_select_procedure_menu.get();
  menu->clear();

  // action when no procedures is selected
  auto action = menu->addAction(kNoProcedureSelected);
  auto on_action = [this]()
  {
    m_select_procedure_button->setText(kNoProcedureSelected);
    emit SelectProcedureRequest(nullptr);
  };
  connect(action, &QAction::triggered, this, on_action);

  // build a menu containing list of procedures for later job submission
  for (auto procedure : m_available_procedures())
  {
    auto action = menu->addAction(QString::fromStdString(procedure->GetDisplayName()));
    auto on_action = [this, procedure]()
    {
      m_select_procedure_button->setText(QString::fromStdString(procedure->GetDisplayName()));
      emit SelectProcedureRequest(procedure);
    };
    connect(action, &QAction::triggered, this, on_action);
  }
}

}  // namespace oac_tree_gui
