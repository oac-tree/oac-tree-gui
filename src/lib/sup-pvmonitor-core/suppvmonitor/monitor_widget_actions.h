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

#ifndef SUP_PVMONITOR_CORE_MONITOR_EDITOR_ACTIONS_H_
#define SUP_PVMONITOR_CORE_MONITOR_EDITOR_ACTIONS_H_

#include <suppvmonitor/monitor_widget_context.h>

#include <QObject>

namespace sequencergui
{
class VariableItem;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace suppvmonitor
{

class MonitorModel;

//! The MonitorWidgetActions class implements actions for MonitorWidget that can be triggered from
//! its main toolbar.

class MonitorWidgetActions : public QObject
{
  Q_OBJECT

public:
  explicit MonitorWidgetActions(MonitorWidgetContext context, MonitorModel* model, QObject* parent);

  void OnAddVariableRequest(const QString& variable_type_name);
  void OnRemoveVariableRequest();
  void OnEditAnyvalueRequest();

private:
  sequencergui::VariableItem* GetSelectedVariable();
  sup::gui::AnyValueItem* GetAnyValueItemToEdit();

  void SetupVariable(sequencergui::VariableItem* item);
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  std::string ProposeVariableName() const;

  MonitorModel* m_model{nullptr};
  MonitorWidgetContext m_context;
};

}  // namespace suppvmonitor

#endif  // SUP_PVMONITOR_CORE_MONITOR_EDITOR_ACTIONS_H_
