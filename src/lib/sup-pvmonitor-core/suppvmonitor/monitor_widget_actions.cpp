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

namespace suppvmonitor
{

MonitorWidgetActions::MonitorWidgetActions(MonitorWidgetContext context, MonitorModel *model,
                                           QObject *parent)
    : QObject(parent), m_context(std::move(context)), m_model(model)
{
}

void MonitorWidgetActions::OnAddVariableRequest(const QString &variable_type_name)
{
  auto selected_item = m_context.get_selected_variable_callback();

  try
  {
    auto tagindex = selected_item ? selected_item->GetTagIndex().Next() : mvvm::TagIndex::Append();
    m_model->InsertItem(m_model->GetFactory()->CreateItem(variable_type_name.toStdString()),
                        m_model->GetWorkspaceItem(), tagindex);
  }
  catch (const std::exception &ex)
  {
    SendMessage("Can't add variable to the selection");
  }
}

void MonitorWidgetActions::SendMessage(const std::string &text, const std::string &informative,
                                       const std::string &details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

}  // namespace suppvmonitor
