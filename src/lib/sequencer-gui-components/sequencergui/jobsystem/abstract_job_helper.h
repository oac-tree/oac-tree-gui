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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HELPER_H_
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HELPER_H_

#include <sequencergui/jobsystem/job_types.h>

namespace sequencergui
{

bool CanStartJob(::sequencergui::RunnerStatus current_status);

bool CanPauseJob(::sequencergui::RunnerStatus current_status);

bool CanReleaseJob(::sequencergui::RunnerStatus current_status);

bool CanStopJob(::sequencergui::RunnerStatus current_status);


}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HELPER_H_
