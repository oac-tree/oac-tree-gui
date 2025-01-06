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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_

#include <sequencergui/jobsystem/i_job_handler.h>

#include <gmock/gmock.h>

namespace sequencergui
{
class JobItem;
}  // namespace sequencergui

namespace testutils
{

/**
 * @brief The MockJobHandlerListener class helps us listening calls from JobManager to the
 * IJobHandler interface.
 */
class MockJobHandlerListener
{
public:
  MOCK_METHOD(void, Start, (const sequencergui::IJobHandler*), ());
  MOCK_METHOD(void, Pause, (const sequencergui::IJobHandler*), ());
  MOCK_METHOD(void, Step, (const sequencergui::IJobHandler*), ());
  MOCK_METHOD(void, Stop, (const sequencergui::IJobHandler*), ());
  MOCK_METHOD(void, Reset, (const sequencergui::IJobHandler*), ());
  MOCK_METHOD(bool, IsRunning, (const sequencergui::IJobHandler*), (const));
  MOCK_METHOD(sequencergui::RunnerStatus, GetRunnerStatus, (const sequencergui::IJobHandler*),
              (const));
  MOCK_METHOD(sequencergui::JobLog*, GetJobLog, (const sequencergui::IJobHandler*), (const));
  MOCK_METHOD(void, OnToggleBreakpointRequest,
              (sequencergui::InstructionItem*, const sequencergui::IJobHandler*), ());
  MOCK_METHOD(sequencergui::ProcedureItem*, GetExpandedProcedure,
              (const sequencergui::IJobHandler*), (const));
};

/**
 * @brief The MockJobHandler implements IJobHandler interface and forwards all calls to mock
 * listener.
 *
 * It reports also own "this" so MockJobHandlerListener could distinguish which job handler calls
 * what.
 */
class MockJobHandler : public sequencergui::IJobHandler
{
public:
  explicit MockJobHandler(MockJobHandlerListener& listener, sequencergui::JobItem* item)
      : m_listener(listener), m_job_item(item)
  {
  }

  void Start() override;

  void Pause() override;

  void Step() override;

  void Stop() override;

  void Reset() override;

  bool IsRunning() const override;

  sequencergui::RunnerStatus GetRunnerStatus() const override;

  sequencergui::JobLog* GetJobLog() const override;

  void OnToggleBreakpointRequest(sequencergui::InstructionItem* instruction) override;

  sequencergui::JobItem* GetJobItem() override;

  sequencergui::ProcedureItem* GetExpandedProcedure() const override;

  MockJobHandlerListener& m_listener;
  sequencergui::JobItem* m_job_item{nullptr};
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_
