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

#ifndef SEQUENCERGUI_COMPONENTS_ITEMVIEWCONTROLLER_H
#define SEQUENCERGUI_COMPONENTS_ITEMVIEWCONTROLLER_H

#include "mvvm/utils/containerutils.h"

#include <QObject>
#include <functional>
#include <memory>

class QAbstractItemView;

namespace mvvm
{
class SessionItem;
class ItemSelectionModel;
class ViewModel;
class ViewModelDelegate;
class ApplicationModel;
}  // namespace mvvm

namespace sequencergui
{

//! Provides QAbstractItemView with custom components: viewmodel, delegate
//! and selection model. It owns all components, but doesn't own a view.

class ItemViewComponentProvider : public QObject
{
  Q_OBJECT

public:
  using create_viewmodel_t =
      std::function<std::unique_ptr<mvvm::ViewModel>(mvvm::ApplicationModel*)>;

  ItemViewComponentProvider(create_viewmodel_t model_func, QAbstractItemView* view);
  ~ItemViewComponentProvider() override;

  void SetApplicationModel(mvvm::ApplicationModel* model);

  void SetItem(mvvm::SessionItem* item);

  QAbstractItemView* GetView() const;

  mvvm::ItemSelectionModel* GetSelectionModel() const;

  mvvm::ViewModel* GetViewModel() const;

  mvvm::SessionItem* GetSelectedItem() const;
  void SetSelectedItem(mvvm::SessionItem* item);

  template <typename T = mvvm::SessionItem>
  std::vector<T*> GetSelectedItems() const;

signals:
  void SelectedItemChanged(mvvm::SessionItem*);

private:
  void Reset();
  void InitViewModel(mvvm::ApplicationModel* model);
  std::vector<mvvm::SessionItem*> GetSelectedItemsIntern() const;

  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  std::unique_ptr<mvvm::ItemSelectionModel> m_selection_model;
  std::unique_ptr<mvvm::ViewModel> m_view_model;
  create_viewmodel_t m_create_viewmodel;  //!< to create new ViewModel on ApplicationModel change

  QAbstractItemView* m_view{nullptr};
};

template <typename T>
std::vector<T*> ItemViewComponentProvider::GetSelectedItems() const
{
  return ::mvvm::utils::CastItems<T>(GetSelectedItemsIntern());
}

template <typename T>
std::unique_ptr<T> CreateViewModel(mvvm::ApplicationModel* model)
{
  return std::make_unique<T>(model);
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ITEMVIEWCONTROLLER_H
