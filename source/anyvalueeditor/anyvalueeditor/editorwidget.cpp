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

#include "anyvalueeditor/anyvalueitem.h"

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace anyvalueeditor
{

EditorWidget::EditorWidget(QWidget *parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_tree_view);

  PopulateModel();

  m_view_model = std::make_unique<mvvm::AllItemsViewModel>(m_model.get());
  m_tree_view->setModel(m_view_model.get());
  m_tree_view->setItemDelegate(m_delegate.get());
}

EditorWidget::~EditorWidget() = default;

void EditorWidget::PopulateModel()
{
  m_model->RegisterItem<AnyValueItem>();

  m_model->InsertItem<AnyValueItem>();
}

}  // namespace anyvalueeditor
