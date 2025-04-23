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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_

#include <oac_tree_gui/jobsystem/i_job_item_manager.h>

#include <gmock/gmock.h>

namespace oac_tree_gui
{

class JobItem;
class IJobHandler;

namespace test
{

/**
 * @brief The MockJobManager class plays the role of JobManager while testing OperationActionHandler
 * machinery.
 */
class MockJobManager : public oac_tree_gui::IJobItemManager
{
public:
  MOCK_METHOD(std::size_t, GetJobCount, (), (const, override));
  MOCK_METHOD(std::vector<oac_tree_gui::JobItem *>, GetJobItems, (), (const, override));
  MOCK_METHOD(void, SubmitJob, (oac_tree_gui::JobItem *), (override));

  MOCK_METHOD(oac_tree_gui::IJobHandler *, GetJobHandler, (oac_tree_gui::JobItem *), (override));

  MOCK_METHOD(void, Start, (oac_tree_gui::JobItem *), (override));
  MOCK_METHOD(void, Pause, (oac_tree_gui::JobItem *), (override));
  MOCK_METHOD(void, Stop, (oac_tree_gui::JobItem *), (override));
  MOCK_METHOD(void, Step, (oac_tree_gui::JobItem *), (override));
  MOCK_METHOD(void, Reset, (oac_tree_gui::JobItem *), (override));

  MOCK_METHOD(void, RemoveJobHandler, (oac_tree_gui::JobItem *), (override));
  MOCK_METHOD(bool, HasRunningJobs, (), (const, override));
  MOCK_METHOD(void, StopAllJobs, (), (override));

  MOCK_METHOD(void, SetActiveJob, (oac_tree_gui::JobItem *), (override));
};

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_JOB_MANAGER_H_
