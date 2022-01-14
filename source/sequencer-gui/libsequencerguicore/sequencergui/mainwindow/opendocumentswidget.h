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

#ifndef SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H
#define SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace mvvm
{
class ViewModel;
}

class QListView;

namespace sequencergui
{
class SequencerModel;
class ProcedureItem;
class ExplorerNarrowToolBar;

//! Vertical panel located on the left of XMLTreeView

class OpenDocumentsWidget : public CollapsibleWidget
{
  Q_OBJECT

public:
  OpenDocumentsWidget(QWidget* parent = nullptr);
  ~OpenDocumentsWidget();

  void SetModel(SequencerModel* model);

  std::vector<ProcedureItem *> GetSelectedProcedures() const;
  ProcedureItem* GetSelectedProcedure();

  void SetSelectedProcedure(ProcedureItem* procedure);

signals:
  void createNewProcedureRequest();
  void procedureSelected(ProcedureItem* procedureItem);

private:
  void SetupToolBar();
  void onTreeSingleClick(const QModelIndex& index);
  void onRemoveSelectedRequest();

  QListView* m_list_view{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<mvvm::ViewModel> m_view_model;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MAINWINDOW_OPENDOCUMENTSWIDGET_H
