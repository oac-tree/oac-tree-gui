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

#ifndef SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_WIDGET_H_
#define SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_WIDGET_H_

#include <sequencergui/views/composer/procedure_list_actions.h>

#include <QWidget>

namespace mvvm
{
class ViewModel;
class ItemViewComponentProvider;
class ISessionModel;
}  // namespace mvvm

class QListView;

namespace sequencergui
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
  explicit ProcedureListWidget(QWidget* parent = nullptr);
  ~ProcedureListWidget() override;

  void SetModel(SequencerModel* model);

  ProcedureItem* GetSelectedProcedure();
  std::vector<ProcedureItem*> GetSelectedProcedures() const;

  void SetSelectedProcedure(ProcedureItem* procedure);

  QListView* GetListView();

  mvvm::ViewModel* GetViewModel();

  QList<QAction*> GetActions(const std::vector<ProcedureListActions::ActionKey>& action_keys);

signals:
  void ProcedureSelected(sequencergui::ProcedureItem* procedure_item);

private:  
  ProcedureListContext CreateContext();
  void OnContextMenuRequest(const QPoint& point);

  QListView* m_list_view{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  ProcedureListActions* m_actions{nullptr};
  ProcedureListActionHandler* m_action_handler{nullptr};

  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_WIDGET_H_
