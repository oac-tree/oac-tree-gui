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

#include "monitor_widget_actions.h"

#include "monitor_model.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sequencergui/model/workspace_item.h>
#include <sequencergui/transform/variable_item_transform_utils.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_item.h>

namespace suppvmonitor
{

MonitorWidgetActions::MonitorWidgetActions(MonitorWidgetContext context, MonitorModel *model,
                                           QObject *parent)
    : QObject(parent), m_context(std::move(context)), m_model(model)
{
}

void MonitorWidgetActions::OnAddVariableRequest(const QString &variable_type_name)
{
  auto selected_item = GetSelectedVariable();

  try
  {
    auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
    auto inserted =
        m_model->InsertItem(m_model->GetFactory()->CreateItem(variable_type_name.toStdString()),
                            m_model->GetWorkspaceItem(), tagindex);
    SetupVariable(dynamic_cast<sequencergui::VariableItem *>(inserted));
  }
  catch (const std::exception &ex)
  {
    SendMessage("Can't add variable to the selection");
  }
}

void MonitorWidgetActions::OnRemoveVariableRequest()
{
  if (auto selected = GetSelectedVariable(); selected)
  {
    m_model->RemoveItem(selected);
  }
}

void suppvmonitor::MonitorWidgetActions::OnEditAnyvalueRequest()
{
  auto selected_anyvalue = GetSelectedAnyValueItem();
  if (!selected_anyvalue)
  {
    SendMessage("Please select AnyValue you want to modify",
                "You can also select workspace variable itself");
    return;
  }

  auto edited_anyvalue = m_context.get_anyvalue_callback(*selected_anyvalue);
  if (edited_anyvalue)
  {
    m_model->RemoveItem(selected_anyvalue);
    m_model->InsertItem(std::move(edited_anyvalue), GetSelectedVariable(), {});
  }
}

sequencergui::VariableItem *MonitorWidgetActions::GetSelectedVariable()
{
  return m_context.get_selected_variable_callback ? m_context.get_selected_variable_callback()
                                                  : nullptr;
}

sup::gui::AnyValueItem *MonitorWidgetActions::GetSelectedAnyValueItem()
{
  return GetSelectedVariable() ? GetSelectedVariable()->GetAnyValueItem() : nullptr;
}

//! Set reasonlable initial values for just created variable.
//! Might be changed in the future.

void MonitorWidgetActions::SetupVariable(sequencergui::VariableItem *item)
{
  if (!item)
  {
    return;
  }

  item->SetName(ProposeVariableName());
  // By default we always set scalar anyvalue to any VariableItem added to the WorkspaceItem.
  // If user wants something else, he has to start AnyValueEditor.
  sequencergui::SetAnyValue(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 0}, *item);
}

void MonitorWidgetActions::SendMessage(const std::string &text, const std::string &informative,
                                       const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

std::string MonitorWidgetActions::ProposeVariableName() const
{
  return "var" + std::to_string(m_model->GetWorkspaceItem()->GetVariableCount() - 1);
}

}  // namespace suppvmonitor
