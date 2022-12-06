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

#ifndef SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_
#define SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_

#include <functional>
#include <string>

namespace sequencergui
{
class VariableItem;
class WorkspaceItem;
}

namespace suppvmonitor
{

class WorkspaceEvent;
class MonitorModel;

//!

class WorkspaceItemController
{
public:
  WorkspaceItemController(MonitorModel* model);
  void ProcessDomainEvent(const WorkspaceEvent& event);

  sequencergui::VariableItem* GeVariableItemForName(const std::string& name);

  sequencergui::WorkspaceItem* GetWorkspaceItem();

  void SetCallback(const std::function<void(const WorkspaceEvent& event)>& callback);

  void OnDataChanged();

private:
  MonitorModel* m_model{nullptr};
  std::function<void(const WorkspaceEvent& event)> m_report_callback;
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_WORKSPACE_ITEM_CONRTOLLER_H_s
