/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/composer/composertreewidget.h"

#include "sequencergui/composer/composertreetoolbar.h"
#include "sequencergui/model/domainutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/widgets/propertytreeview.h"
#include "mvvm/widgets/topitemstreeview.h"
#include "mvvm/widgets/allitemstreeview.h"
#include "mvvm/widgets/widgetutils.h"

#include <QSplitter>
#include <QTabWidget>
#include <QTreeView>
#include <QVBoxLayout>

namespace sequi
{
ComposerTreeWidget::ComposerTreeWidget(QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new ComposerTreeToolBar)
    , m_tab_widget(new QTabWidget)
    , m_instruction_tree(new ModelView::TopItemsTreeView)
    , m_workspace_tree(new ModelView::AllItemsTreeView)
    , m_property_tree(new ModelView::PropertyTreeView)
    , m_splitter(new QSplitter)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  m_splitter->setOrientation(Qt::Vertical);

  m_tab_widget->addTab(m_instruction_tree, "Instructions");
  m_tab_widget->addTab(m_workspace_tree, "Workspace");
  m_tab_widget->setTabPosition(QTabWidget::South);
  m_splitter->addWidget(m_tab_widget);
  m_splitter->addWidget(m_property_tree);
  m_splitter->setSizes(QList<int>() << 300 << 200);
  layout->addWidget(m_splitter);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  SetupConnections();
}

ComposerTreeWidget::~ComposerTreeWidget() = default;

void ComposerTreeWidget::SetModel(SequencerModel* model, ProcedureItem* procedure)
{
  m_model = model;
  m_procedure = procedure;

  m_instruction_tree->SetApplicationModel(m_model);
  m_workspace_tree->SetApplicationModel(m_model);
  if (m_procedure)
  {
    m_instruction_tree->SetRootSessionItem(procedure->GetInstructionContainer());
    m_workspace_tree->SetRootSessionItem(procedure->GetWorkspace());
  }
}

void ComposerTreeWidget::SetSelected(InstructionItem* instruction)
{
  m_instruction_tree->SetSelected(instruction);
}

void ComposerTreeWidget::SetupConnections()
{
  auto on_selection_changed = [this](auto item) { m_property_tree->SetItem(item); };
  connect(m_instruction_tree, &ModelView::TopItemsTreeView::itemSelected, on_selection_changed);

  auto on_insert_after = [this](auto name)
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->InsertNewItem(name.toStdString(), item->GetParent(), item->GetTagIndex().Next());
    }
    else
    {
      m_model->InsertNewItem(name.toStdString(), m_procedure->GetInstructionContainer(), {"", -1});
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::insertAfterRequest, this, on_insert_after);

  auto on_insert_into = [this](auto name)
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->InsertNewItem(name.toStdString(), item, {"", -1});
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::insertIntoRequest, this, on_insert_into);

  auto on_remove = [this]()
  {
    if (auto item = m_instruction_tree->GetSelectedItem(); item)
    {
      m_model->RemoveItem(item);
    }
  };
  connect(m_tool_bar, &ComposerTreeToolBar::removeSelectedRequest, this, on_remove);
}

}  // namespace sequi
