/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#ifndef OAC_TREE_GUI_MAINWINDOW_PVMONITOR_PROJECT_H_
#define OAC_TREE_GUI_MAINWINDOW_PVMONITOR_PROJECT_H_

#include <mvvm/project/app_project.h>

namespace oac_tree_gui
{

class MonitorModel;

/**
 * @brief The PvMonitorProject class is a main project for sup-pvmonitor application.
 */
class PvMonitorProject : public mvvm::AppProject
{
public:
  static inline const std::string kApplicationType = "SUP PV Monitor";

  explicit PvMonitorProject(const mvvm::ProjectContext& context);

  ~PvMonitorProject() override;

  MonitorModel* GetMonitorModel();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_PVMONITOR_PROJECT_H_
