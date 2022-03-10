/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalueeditor/editorwidget.h"

#include "AnyValue.h"
#include "AnyValueHelper.h"
#include "anyvalueeditor/anyvalueeditoractions.h"
#include "anyvalueeditor/anyvalueeditortoolbar.h"
#include "anyvalueeditor/anyvalueitem.h"
#include "anyvalueeditor/transformfromanyvalue.h"

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"
#include "mvvm/widgets/allitemstreeview.h"

#include <QDebug>
#include <QTreeView>
#include <QVBoxLayout>

namespace anyvalueeditor
{

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_actions(new AnyValueEditorActions(m_model.get(), this))
    , m_tool_bar(new AnyValueEditorToolBar(m_actions))
    , m_tree_view(new QTreeView)
    , m_all_items_tree_view(new mvvm::AllItemsTreeView)
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tool_bar);
  layout->addWidget(m_all_items_tree_view);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->setMargin(0);

  m_model->RegisterItem<AnyValueItem>();
  PopulateModel();

  //  m_view_model = std::make_unique<mvvm::AllItemsViewModel>(m_model.get());
  //  m_tree_view->setModel(m_view_model.get());
  //  m_tree_view->setItemDelegate(m_delegate.get());

  //  m_actions->SetSelectionModel(m_tree_view->selectionModel());
  //  m_actions->SetViewModel(m_view_model.get());

  m_all_items_tree_view->SetApplicationModel(m_model.get());
  auto on_selected = [this](auto item)
  { m_actions->SetSelectedItem(dynamic_cast<AnyValueItem *>(item)); };
  connect(m_all_items_tree_view, &mvvm::AllItemsTreeView::itemSelected, this, on_selected);
}

void EditorWidget::ImportAnyValueFromFile(const QString &file_name)
{
  qDebug() << file_name;
  auto anyvalue = sup::dto::AnyValueFromJSONFile(file_name.toStdString());
  auto item = m_model->InsertItem<AnyValueItem>();
  PopulateItem(&anyvalue, item);
}

EditorWidget::~EditorWidget() = default;

void EditorWidget::PopulateModel()
{
  {  // two members
    sup::dto::AnyValue anyvalue = {
        {{"signed", {sup::dto::SignedInteger32, 42}}, {"bool", {sup::dto::Boolean, true}}}};
    auto item = m_model->InsertItem<AnyValueItem>();
    PopulateItem(&anyvalue, item);
  }

  {  // Nested structure
    sup::dto::AnyValue two_scalars = {
        {{"signed", {sup::dto::SignedInteger8, 1}}, {"bool", {sup::dto::Boolean, 12}}}};
    sup::dto::AnyValue anyvalue{{
        {"scalars", two_scalars},
    }};
    auto item = m_model->InsertItem<AnyValueItem>();
    PopulateItem(&anyvalue, item);
  }
}

}  // namespace anyvalueeditor
