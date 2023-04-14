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

#ifndef SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_PVMONITOR_WORKSPACE_EDITOR_ACTIONS_H_

#include <sequencergui/pvmonitor/workspace_editor_context.h>

#include <QObject>

namespace mvvm
{
class SessionModelInterface;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

class VariableItem;
class WorkspaceItem;

//! The MonitorWidgetActions class implements actions for MonitorWidget that can be triggered from
//! its main toolbar.

class WorkspaceEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit WorkspaceEditorActions(WorkspaceEditorContext context, QObject* parent = nullptr);

  void OnAddVariableRequest(const QString& variable_type_name);
  void OnRemoveVariableRequest();
  void OnEditAnyvalueRequest();

private:
  mvvm::SessionModelInterface* GetModel() const;
  WorkspaceItem* GetWorkspaceItem() const;
  VariableItem* GetSelectedVariable();
  sup::gui::AnyValueItem* GetSelectedAnyValueItem();
  sup::gui::AnyValueItem* GetAnyValueItemToEdit();

  void SetupVariable(VariableItem* item);
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  std::string ProposeVariableName() const;

  WorkspaceItem* m_workspace{nullptr};
  WorkspaceEditorContext m_context;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_WIDGET_ACTIONS_H_
