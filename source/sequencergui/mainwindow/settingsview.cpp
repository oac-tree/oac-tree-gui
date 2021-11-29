/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
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

namespace sequi
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
  //  layout->addSpacing(ModelView::Utils::WidthOfLetterM() / 2);
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
  auto view = new ModelView::AllItemsTreeView(m_model);
  view->GetTreeView()->setAlternatingRowColors(true);
  m_tab_widget->addTab(view, QString::fromStdString(m_model->GetType()));
  m_stacked_widget->addWidget(m_tab_widget);
}

void SettingsView::SetupListSelector()
{
  const int width = ModelView::Utils::WidthOfLetterM() * 15;
  m_list_widget->setFixedWidth(width);
  m_list_widget->setIconSize(
      QSize(ModelView::Utils::WidthOfLetterM() * 1.2, ModelView::Utils::WidthOfLetterM() * 1.2));

  auto item = new QListWidgetItem(QIcon(":/icons/card-bulleted-outline.svg"), "All models");
  m_list_widget->addItem(item);

  item = new QListWidgetItem(QIcon(":/icons/cog-outline.svg"), "Miscellaneous");
  m_list_widget->addItem(item);

  connect(m_list_widget, &QListWidget::currentRowChanged,
          [this](int row) { m_stacked_widget->setCurrentIndex(row); });
}

void SettingsView::SetupOtherSettings()
{
  m_stacked_widget->addWidget(new QWidget);
}

}  // namespace sequi
