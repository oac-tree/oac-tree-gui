/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWMODEL_WORKSPACE_FILTER_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_WORKSPACE_FILTER_VIEWMODEL_H_

#include <QSortFilterProxyModel>

namespace mvvm
{
class ViewModel;
class SessionItem;
}  // namespace mvvm

namespace oac_tree_gui
{

/**
 * @brief The WorkspaceFilterViewModel class is a proxy model that filters Workspace variables
 * according to the variable's name or channel name.
 *
 * Variable which passed the filter, is shown with all its properties.
 */
class WorkspaceFilterViewModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit WorkspaceFilterViewModel(QObject* parent_object = nullptr);

  void SetPattern(const QString& pattern);

  bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

  /**
   * @brief Checks if item should be shown in a view.
   *
   * Item will be shown if it is a) connectable variable with proper channel name, or variable name
   * b) it is a property with proper variable
   */
  bool IsItemAccepted(const mvvm::SessionItem* item) const;

  /**
   * @brief Check if given item name (or channel name) passes the filter.
   */
  bool IsValidName(const std::string& name) const;

private:
  mvvm::ViewModel* GetViewModel() const;
  QString m_pattern;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_VIEWMODEL_WORKSPACE_FILTER_VIEWMODEL_H_
