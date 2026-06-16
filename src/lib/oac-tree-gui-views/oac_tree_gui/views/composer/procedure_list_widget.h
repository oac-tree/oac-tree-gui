/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_WIDGET_H_

#include <oac_tree_gui/views/composer/procedure_list_actions.h>

#include <QWidget>

class QTreeView;

namespace mvvm
{
class ViewModel;
class ItemViewComponentProvider;
class ItemSelection;
}  // namespace mvvm

namespace sup::gui
{
class IAppCommandService;
class FlatListView;
}  // namespace sup::gui

namespace oac_tree_gui
{

class SequencerModel;
class ProcedureItem;
class ProcedureListActionHandler;
class ProcedureListContext;

/**
 * @brief The ProcedureListWidget class represent a simple list with procedures with editable names.
 *
 * It is located in left-bottom corner of SequencerExplorerView and left-top corner of
 * SequencerComposerView. It contains a list of actions to add/remove/copy procedures.
 */
class ProcedureListWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureListWidget(sup::gui::IAppCommandService& command_service,
                               QWidget* parent_widget = nullptr);
  ~ProcedureListWidget() override;

  ProcedureListWidget(const ProcedureListWidget&) = delete;
  ProcedureListWidget& operator=(const ProcedureListWidget&) = delete;
  ProcedureListWidget(ProcedureListWidget&&) = delete;
  ProcedureListWidget& operator=(ProcedureListWidget&&) = delete;

  void SetModel(SequencerModel* model);

  QTreeView* GetTreeView();

  mvvm::ViewModel* GetViewModel();

  QList<QAction*> GetActions(const std::vector<ProcedureListActions::ActionKey>& action_keys);

  mvvm::ItemSelection GetSelection() const;

  void SetSelection(const mvvm::ItemSelection& selection);

signals:
  void procedureSelectionChanged(const mvvm::ItemSelection& selection);

private:
  ProcedureListContext CreateContext();
  void OnContextMenuRequest(const QPoint& point);
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateProvider() const;
  void SetupConnections();

  sup::gui::IAppCommandService& m_command_service;
  sup::gui::FlatListView* m_list_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  ProcedureListActions* m_actions{nullptr};
  ProcedureListActionHandler* m_action_handler{nullptr};

  SequencerModel* m_model{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_WIDGET_H_
