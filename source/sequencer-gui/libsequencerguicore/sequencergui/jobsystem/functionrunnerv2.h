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

#ifndef SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNERV2_H
#define SEQUENCERGUI_JOBSYSTEM_FUNCTIONRUNNERV2_H

#include "sequencergui/jobsystem/abstractjob.h"

#include <memory>

namespace sequencergui
{

class FunctionRunnerV2 : public AbstractJob
{
public:
  FunctionRunnerV2();
  ~FunctionRunnerV2() override;

private:
  void Start() override;
  void Pause() override;
  void Step() override;
  void Stop() override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACTJOB_H
