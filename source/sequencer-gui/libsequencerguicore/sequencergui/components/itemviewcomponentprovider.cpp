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

#include "sequencergui/components/itemviewcomponentprovider.h"

#include "sequencergui/core/exceptions.h"

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <QAbstractItemView>

namespace sequencergui
{

ItemViewComponentProvider::ItemViewComponentProvider(create_viewmodel_t model_func,
                                                     QAbstractItemView *view)
    : m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
    , m_selection_model(std::make_unique<mvvm::ItemSelectionModel>())
    , m_create_viewmodel(std::move(model_func))
    , m_view(view)
{
  connect(m_selection_model.get(), &mvvm::ItemSelectionModel::SelectedItemChanged, this,
          [this](auto item) { emit SelectedItemChanged(const_cast<mvvm::SessionItem *>(item)); });
}

ItemViewComponentProvider::~ItemViewComponentProvider() = default;

void ItemViewComponentProvider::SetApplicationModel(mvvm::ApplicationModel *model)
{
  if (!model)
  {
    Reset();
    return;
  }

  InitViewModel(model);
}

void ItemViewComponentProvider::SetItem(mvvm::SessionItem *item)
{
  if (!item)
  {
    Reset();
    return;
  }

  auto application_model = dynamic_cast<mvvm::ApplicationModel *>(item->GetModel());
  if (!application_model)
  {
    throw RuntimeException("Wrong model type");
  }

  InitViewModel(application_model);
  m_view_model->SetRootSessionItem(item);
}

QAbstractItemView *ItemViewComponentProvider::GetView() const
{
  return m_view;
}

mvvm::ItemSelectionModel *ItemViewComponentProvider::GetSelectionModel() const
{
  return m_selection_model.get();
}

void ItemViewComponentProvider::Reset()
{
  m_view->setModel(nullptr);
  m_selection_model->setModel(nullptr);
  m_view_model.reset();
}

void ItemViewComponentProvider::InitViewModel(mvvm::ApplicationModel *model)
{
  m_view_model = m_create_viewmodel(model);
  m_selection_model->SetViewModel(m_view_model.get());
  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());
  m_view->setSelectionModel(m_selection_model.get());
}

}  // namespace sequencergui
