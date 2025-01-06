/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_PVMONITOR_MONITOR_MODEL_H_
#define SEQUENCERGUI_PVMONITOR_MONITOR_MODEL_H_

#include <mvvm/model/application_model.h>

namespace sequencergui
{
class WorkspaceItem;
}

namespace sequencergui
{

class MonitorModel : public mvvm::ApplicationModel
{
public:
  MonitorModel();

  sequencergui::WorkspaceItem* GetWorkspaceItem() const;

  void Clear() override;

private:
  void PopulateModel();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_PVMONITOR_MONITOR_MODEL_H_
