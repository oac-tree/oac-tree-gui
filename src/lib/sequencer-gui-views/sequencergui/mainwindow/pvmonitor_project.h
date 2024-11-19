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

#ifndef SEQUENCERGUI_MAINWINDOW_PVMONITOR_PROJECT_H_
#define SEQUENCERGUI_MAINWINDOW_PVMONITOR_PROJECT_H_

#include <mvvm/project/app_project.h>

namespace sequencergui
{

class MonitorModel;

/**
 * @brief The PvMonitorProject class is a main project for sup-pvmonitor application.
 */
class PvMonitorProject : public mvvm::AppProject
{
public:
  static inline const std::string kApplicationType = "SUP PV Monitor";

  explicit PvMonitorProject(const mvvm::ProjectContext &context);

  ~PvMonitorProject() override;

  MonitorModel* GetMonitorModel();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_PVMONITOR_PROJECT_H_
