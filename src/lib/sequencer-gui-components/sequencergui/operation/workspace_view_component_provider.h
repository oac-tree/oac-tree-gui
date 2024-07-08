/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_
#define SEQUENCERGUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_

#include <mvvm/providers/item_view_component_provider.h>

class QTreeView;

namespace mvvm
{
class FilterNameViewModel;
class ViewModel;
}  // namespace mvvm

namespace sequencergui
{

/**
 * @brief The WorkspaceViewComponentProvider class provides filtering capabilities on top of the
 * given model.
 */
class WorkspaceViewComponentProvider : public mvvm::ItemViewComponentProvider
{
  Q_OBJECT

public:
  explicit WorkspaceViewComponentProvider(std::unique_ptr<mvvm::ViewModel> view_model,
                                          QTreeView* view);

  /**
   * @brief Sets filtering pattern to the display name in the first column.
   *
   * Item will be filtered out if its display name doesn't contain a given pattern (case
   * insensitive).
   */
  void SetFilterPattern(const QString& pattern);

private:
  mvvm::FilterNameViewModel* m_filter_proxy_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_WORKSPACE_VIEW_COMPONENT_PROVIDER_H_
