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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_

#include <sequencergui/jobsystem/i_job_item_manager.h>

#include <gmock/gmock.h>

namespace sequencergui
{
class JobItem;
class IJobHandler;
}  // namespace sequencergui

namespace testutils
{

/**
 * @brief The MockJobManager class plays the role of JobManager while testing OperationActionHandler
 * machinery.
 */
class MockJobManager : public sequencergui::IJobItemManager
{
public:
  MOCK_METHOD(void, SubmitJob, (sequencergui::JobItem*), (override));

  MOCK_METHOD(sequencergui::IJobHandler*, GetJobHandler, (sequencergui::JobItem*),
              (override));

  MOCK_METHOD(void, Start, (sequencergui::JobItem*), (override));
  MOCK_METHOD(void, Pause, (sequencergui::JobItem*), (override));
  MOCK_METHOD(void, Stop, (sequencergui::JobItem*), (override));
  MOCK_METHOD(void, Step, (sequencergui::JobItem*), (override));
  MOCK_METHOD(void, Reset, (sequencergui::JobItem*), (override));

  MOCK_METHOD(void, RemoveJobHandler, (sequencergui::JobItem*), (override));
  MOCK_METHOD(bool, HasRunningJobs, (), (const, override));
  MOCK_METHOD(void, StopAllJobs, (), (override));

  MOCK_METHOD(void, SetActiveJob, (sequencergui::JobItem*), (override));
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_
