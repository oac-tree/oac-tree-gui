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

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/widgets/itemselectionmodel.h"

#include <QAbstractItemView>

namespace sequencergui
{

ItemViewComponentProvider::ItemViewComponentProvider(QAbstractItemView *view) : m_view(view) {}

ItemViewComponentProvider::~ItemViewComponentProvider() = default;

void ItemViewComponentProvider::SetApplicationModel(mvvm::ApplicationModel *model) {}

void ItemViewComponentProvider::SetItem(mvvm::SessionItem *item) {}

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

}  // namespace sequencergui
