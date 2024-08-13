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

#ifndef SEQUENCERGUI_AUTOMATION_AUTOMATION_MANAGER_H_
#define SEQUENCERGUI_AUTOMATION_AUTOMATION_MANAGER_H_

#include <string>
#include <memory>

namespace sequencergui
{

class RemoteJobObserver;

class AutomationManager
{
public:
  explicit AutomationManager(const std::string& server_name);
  ~AutomationManager();

private:
  std::unique_ptr<RemoteJobObserver> m_job_observer;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_AUTOMATION_AUTOMATION_MANAGER_H_
