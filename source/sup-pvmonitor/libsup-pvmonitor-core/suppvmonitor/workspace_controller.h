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

#ifndef SUPPVMONITOR_WORKSPACE_CONTROLLER_H_
#define SUPPVMONITOR_WORKSPACE_CONTROLLER_H_

#include <memory>

namespace suppvmonitor
{

class MonitorModel;
class SequencerWorkspaceListener;

class WorkspaceController
{
public:
  WorkspaceController(MonitorModel* model);

private:
  void InitController();
  MonitorModel* m_model{nullptr};
};

}  // namespace suppvmonitor

#endif  // SUPPVMONITOR_WORKSPACE_CONTROLLER_H_
