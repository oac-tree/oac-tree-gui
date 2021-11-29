/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/mainwindow/opendocumentswidget.h"

#include "sequencergui/mainwindow/explorernarrowtoolbar.h"
#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencermodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include <QLabel>
#include <QListView>
#include <QSplitter>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

namespace sequi
{
OpenDocumentsWidget::OpenDocumentsWidget(QWidget *parent)
    : CollapsibleWidget(parent), m_list_view(new QListView)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_view);

  SetupToolBar();

  connect(m_list_view, &QListView::clicked, this, &OpenDocumentsWidget::onTreeSingleClick);
}

OpenDocumentsWidget::~OpenDocumentsWidget() = default;

void OpenDocumentsWidget::SetModel(SequencerModel *model)
{
  m_model = model;
  m_view_model = std::make_unique<ModelView::TopItemsViewModel>(model);
  m_view_model->SetRootSessionItem(model->GetProcedureContainer());
  m_list_view->setModel(m_view_model.get());
}

ProcedureItem *OpenDocumentsWidget::GetSelectedProcedure()
{
  auto selected = GetSelectedProcedures();
  return selected.empty() ? nullptr : selected.front();
}

void OpenDocumentsWidget::SetSelectedProcedure(ProcedureItem *procedure)
{
  auto indexes = m_view_model->GetIndexOfSessionItem(procedure);
  if (!indexes.empty())
  {
    m_list_view->selectionModel()->select(indexes.at(0), QItemSelectionModel::SelectCurrent);
  }
}

std::vector<ProcedureItem *> OpenDocumentsWidget::GetSelectedProcedures() const
{
  std::vector<ModelView::SessionItem *> result;

  if (!m_list_view->selectionModel())
  {
    return {};
  }
  for (auto index : m_list_view->selectionModel()->selectedIndexes())
  {
    auto procedure_item =
        ModelView::Utils::GetContext<ModelView::SessionItem>(m_view_model->itemFromIndex(index));
    result.push_back(const_cast<ModelView::SessionItem *>(procedure_item));
  }
  return ModelView::Utils::CastedItems<ProcedureItem>(result);
}

void OpenDocumentsWidget::SetupToolBar()
{
  SetText("PROCEDURES");

  auto tool_bar = GetToolBar();

  auto new_procedure_button = new QToolButton;
  new_procedure_button->setIcon(QIcon(":/icons/file-plus-outline.svg"));
  connect(new_procedure_button, &QToolButton::clicked, this,
          &OpenDocumentsWidget::createNewProcedureRequest);
  tool_bar->AddWidget(new_procedure_button);

  auto remove_selected_button = new QToolButton;
  remove_selected_button->setIcon(QIcon(":/icons/beaker-remove-outline.svg"));
  connect(remove_selected_button, &QToolButton::clicked, this,
          &OpenDocumentsWidget::onRemoveSelectedRequest);
  tool_bar->AddWidget(remove_selected_button);
}

void OpenDocumentsWidget::onTreeSingleClick(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  if (auto item = ModelView::Utils::GetItem<ProcedureItem>(m_view_model->itemFromIndex(index));
      item)
  {
    emit procedureSelected(const_cast<ProcedureItem *>(item));
  }
}

void OpenDocumentsWidget::onRemoveSelectedRequest()
{
  for (auto procedure_item : GetSelectedProcedures())
  {
    m_model->RemoveItem(procedure_item);
  }
}

}  // namespace sequi
