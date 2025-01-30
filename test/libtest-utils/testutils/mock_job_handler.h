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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_

#include <oac_tree_gui/jobsystem/i_job_handler.h>

#include <gmock/gmock.h>

namespace oac_tree_gui
{

class JobItem;

namespace test
{

/**
 * @brief The MockJobHandlerListener class helps us listening calls from JobManager to the
 * IJobHandler interface.
 */
class MockJobHandlerListener
{
public:
  MOCK_METHOD(void, Start, (const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(void, Pause, (const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(void, Step, (const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(void, Stop, (const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(void, Reset, (const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(bool, IsRunning, (const oac_tree_gui::IJobHandler*), (const));
  MOCK_METHOD(oac_tree_gui::RunnerStatus, GetRunnerStatus, (const oac_tree_gui::IJobHandler*),
              (const));
  MOCK_METHOD(oac_tree_gui::JobLog*, GetJobLog, (const oac_tree_gui::IJobHandler*), (const));
  MOCK_METHOD(void, OnToggleBreakpointRequest,
              (oac_tree_gui::InstructionItem*, const oac_tree_gui::IJobHandler*), ());
  MOCK_METHOD(oac_tree_gui::ProcedureItem*, GetExpandedProcedure,
              (const oac_tree_gui::IJobHandler*), (const));
};

/**
 * @brief The MockJobHandler implements IJobHandler interface and forwards all calls to mock
 * listener.
 *
 * It reports also own "this" so MockJobHandlerListener could distinguish which job handler calls
 * what.
 */
class MockJobHandler : public oac_tree_gui::IJobHandler
{
public:
  explicit MockJobHandler(MockJobHandlerListener& listener, oac_tree_gui::JobItem* item)
      : m_listener(listener), m_job_item(item)
  {
  }

  void Start() override;

  void Pause() override;

  void Step() override;

  void Stop() override;

  void Reset() override;

  bool IsRunning() const override;

  oac_tree_gui::RunnerStatus GetRunnerStatus() const override;

  oac_tree_gui::JobLog* GetJobLog() const override;

  void OnToggleBreakpointRequest(oac_tree_gui::InstructionItem* instruction) override;

  oac_tree_gui::JobItem* GetJobItem() override;

  oac_tree_gui::ProcedureItem* GetExpandedProcedure() const override;

  MockJobHandlerListener& m_listener;
  oac_tree_gui::JobItem* m_job_item{nullptr};
};

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_JOB_HANDLER_H_
