/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/mainwindow/settingsview.h"

#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/widgetutils.h"

#include <QHBoxLayout>
#include <QListView>
#include <QListWidget>
#include <QStackedWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QTreeView>

namespace sequencergui
{

SettingsView::SettingsView(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar{new QToolBar}
    , m_list_widget(new QListWidget)
    , m_stacked_widget(new QStackedWidget)
    , m_tab_widget(new QTabWidget)
{
  m_tool_bar->setIconSize(StyleUtils::ToolBarIconSize());

  auto hlayout = new QHBoxLayout;
  hlayout->setMargin(0);
  hlayout->setSpacing(0);
  //  hlayout->setContentsMargins(0, 0, 0, 0);

  hlayout->addWidget(m_list_widget);
  //  layout->addSpacing(mvvm::Utils::WidthOfLetterM() / 2);
  hlayout->addWidget(m_stacked_widget);

  m_stacked_widget->setCurrentIndex(0);

  auto layout = new QVBoxLayout(this);
  //  layout->addWidget(m_tool_bar);
  layout->addLayout(hlayout);
}

void SettingsView::SetModel(SequencerModel *model)
{
  m_model = model;
  SetupListSelector();
  SetupModelSettings();
  SetupOtherSettings();
}

//! Initialize tabs with model content.
//! Each model will be represented by a single tree (with all items shown) in a tab.

void SettingsView::SetupModelSettings()
{
  auto view = new mvvm::AllItemsTreeView(m_model);
  view->GetTreeView()->setAlternatingRowColors(true);
  m_tab_widget->addTab(view, QString::fromStdString(m_model->GetType()));
  m_stacked_widget->addWidget(m_tab_widget);
}

void SettingsView::SetupListSelector()
{
  const int width = mvvm::utils::WidthOfLetterM() * 15;
  m_list_widget->setFixedWidth(width);
  m_list_widget->setIconSize(
      QSize(mvvm::utils::WidthOfLetterM() * 1.2, mvvm::utils::WidthOfLetterM() * 1.2));

  auto item = new QListWidgetItem(StyleUtils::GetIcon("card-bulleted-outline.svg"), "All models");
  m_list_widget->addItem(item);

  item = new QListWidgetItem(StyleUtils::GetIcon("cog-outline.svg"), "Miscellaneous");
  m_list_widget->addItem(item);

  connect(m_list_widget, &QListWidget::currentRowChanged,
          [this](int row) { m_stacked_widget->setCurrentIndex(row); });
}

void SettingsView::SetupOtherSettings()
{
  m_stacked_widget->addWidget(new QWidget);
}

}  // namespace sequi
