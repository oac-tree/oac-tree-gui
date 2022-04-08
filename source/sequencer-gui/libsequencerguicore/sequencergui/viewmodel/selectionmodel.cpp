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

#include "sequencergui/viewmodel/selectionmodel.h"

#include "mvvm/model/itemutils.h"
#include "mvvm/utils/containerutils.h"
#include "mvvm/viewmodel/viewmodel.h"

namespace sequencergui
{

SelectionModel::SelectionModel(mvvm::ViewModel *view_model, QObject *parent)
    : QItemSelectionModel(view_model, parent)
{
  connect(view_model, &mvvm::ViewModel::modelAboutToBeReset, [this]() { clearSelection(); });
}

void SelectionModel::SetViewModel(mvvm::ViewModel *view_model)
{
  setModel(view_model);
}

const mvvm::SessionItem *SelectionModel::GetSelectedItem() const
{
  auto selected = GetSelectedItems();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<const mvvm::SessionItem *> SelectionModel::GetSelectedItems() const
{
  std::vector<const mvvm::SessionItem *> result;

  for (auto index : selectedIndexes())
  {
    auto item = GetViewModel()->GetSessionItemFromIndex(index);

    result.push_back(item);
  }

  return mvvm::utils::UniqueWithOrder(result);
}

void SelectionModel::SetSelectedItem(const mvvm::SessionItem *item)
{
  SetSelectedItems({item});
}

void SelectionModel::SetSelectedItems(std::vector<const mvvm::SessionItem *> items)
{
  clearSelection();
  QItemSelection selection;
  for (auto item : items)
  {
    for (auto index : GetViewModel()->GetIndexOfSessionItem(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }
  //  auto flags = QItemSelectionModel::Select; // not clear, which one to use
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;
  select(selection, flags);
}

const mvvm::ViewModel *SelectionModel::GetViewModel() const
{
  return dynamic_cast<const mvvm::ViewModel *>(model());
}

}  // namespace sequencergui
