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

class QMenu;
class QWidgetAction;

namespace mvvm
{
class AllItemsTreeView;
}  // namespace mvvm

namespace sequencergui
{

class ProcedureItem;
class VariableItem;

//! List of workspace variables at the right of SequencerComposerView.
//! Defines set of actions to add/remove variables. Part of ComposerProcedureEditor.

class WorkspaceEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceEditorWidget(QWidget* parent = nullptr);
  ~WorkspaceEditorWidget() override;

  void SetProcedure(ProcedureItem* procedure);

  VariableItem* GetSelectedVariable() const;

signals:
  void VariableSelected(sequencergui::VariableItem* variable);
  void InsertAfterRequest(const QString& name);
  void RemoveSelectedRequest();
  void EditAnyvalueRequest();

private:
  void SetupActions();

  std::unique_ptr<QMenu> CreateInsertAfterMenu();

  std::unique_ptr<QMenu> m_insert_after_menu;
  QWidgetAction* m_insert_after_action{nullptr};
  QWidgetAction* m_remove_action{nullptr};
  QWidgetAction* m_edit_anyvalue_action{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
