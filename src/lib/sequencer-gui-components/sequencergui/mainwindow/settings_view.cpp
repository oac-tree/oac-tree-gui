/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_view.h"

#include <sequencergui/model/application_models.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/widgets/dots_toolbar.h>
#include <sequencergui/widgets/style_utils.h>

#include <mvvm/model/application_model.h>
#include <mvvm/widgets/all_items_tree_view.h>
#include <mvvm/widgets/widget_utils.h>

#include <QHBoxLayout>
#include <QListView>
#include <QListWidget>
#include <QStackedWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequencergui
{

SettingsView::SettingsView(QWidget *parent)
    : QWidget(parent)
    , m_tool_bar(new DotsToolBar)
    , m_list_widget(new QListWidget)
    , m_stacked_widget(new QStackedWidget)
{
  m_tool_bar->AddDotsMenu();

  auto hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(0, 0, 0, 0);
  hlayout->setSpacing(0);

  hlayout->addWidget(m_list_widget);
  hlayout->addWidget(m_stacked_widget);

  m_stacked_widget->setCurrentIndex(0);

  auto layout = new QVBoxLayout(this);
  layout->addLayout(hlayout);
}

void SettingsView::SetApplicationModels(ApplicationModels *models)
{
  m_models = models;
  SetupListSelector();
  SetupModelWidgets();
  SetupGeneralSettingWidgets();
}

//! Initialize tabs with model content.
//! Each model will be represented by a single tree (with all items shown) in a tab.

void SettingsView::SetupModelWidgets()
{
  for (auto model : m_models->GetModels())
  {
    auto view = new mvvm::AllItemsTreeView(model);
    view->GetTreeView()->expandAll();
    view->GetTreeView()->setAlternatingRowColors(true);
    view->GetTreeView()->resizeColumnToContents(0);

    m_stacked_widget->addWidget(view);

    auto item = new QListWidgetItem(styleutils::GetIcon("card-bulleted-outline.svg"),
                                    QString::fromStdString(model->GetType()));
    m_list_widget->addItem(item);
  }
}

void SettingsView::SetupListSelector()
{
  const int width = mvvm::utils::UnitSize(15.0);
  m_list_widget->setFixedWidth(width);
  m_list_widget->setIconSize(QSize(mvvm::utils::UnitSize(1.2), mvvm::utils::UnitSize(1.2)));

  connect(m_list_widget, &QListWidget::currentRowChanged, this,
          [this](int row) { m_stacked_widget->setCurrentIndex(row); });
}

void SettingsView::SetupGeneralSettingWidgets()
{
  m_stacked_widget->addWidget(new QWidget);
  auto item = new QListWidgetItem(styleutils::GetIcon("card-bulleted-outline.svg"), "UI settings");
  m_list_widget->addItem(item);
}

}  // namespace sequencergui
