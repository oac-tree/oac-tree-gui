/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
#define SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

namespace mvvm
{
class AllItemsTreeView;
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

class ProcedureItem;
class VariableItem;
class WorkspaceEditorContext;
class WorkspaceEditorActionHandler;
class WorkspaceEditorActions;

//! List of workspace variables at the right of SequencerComposerView.
//! Defines set of actions to add/remove variables. Part of ComposerProcedureEditor.

class WorkspaceEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceEditorWidget(QWidget* parent = nullptr);
  ~WorkspaceEditorWidget() override;

  void SetProcedure(ProcedureItem* procedure);

  mvvm::SessionItem* GetSelectedItem() const;

private:
  void SetupConnections();
  WorkspaceEditorContext CreateWorkspaceEditorContext();

  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  ProcedureItem* m_procedure{nullptr};

  WorkspaceEditorActions* m_editor_actions{nullptr};
  std::unique_ptr<WorkspaceEditorActionHandler> m_action_handler;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
