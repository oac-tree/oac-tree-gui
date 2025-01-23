/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Supervision and Automation (SUP) oac-tree component
 *
 * Description   : UserInterface implementation
 *
 * Author        : B.Bauvir (IO)
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
 ******************************************************************************/

#include <oac-tree-gui/domain/domain_helper.h>

#include <sup/epics-test/softioc_runner.h>

#include <gtest/gtest.h>
#include <testutils/sequencer_test_utils.h>

#include <chrono>
#include <thread>

class IOCEnvironment : public ::testing::Environment
{
public:
  IOCEnvironment();
  ~IOCEnvironment() override;

  void SetUp() override;
  void TearDown() override;

  sup::epics::test::SoftIocRunner m_softioc_service;
};

::testing::Environment* const ioc_environment =
    ::testing::AddGlobalTestEnvironment(new IOCEnvironment);

IOCEnvironment::IOCEnvironment() : m_softioc_service{"SequencerGUIChannelAccessTests"} {}

IOCEnvironment::~IOCEnvironment() = default;

void IOCEnvironment::SetUp()
{
  if (sequencergui::IsSequencerPluginEpicsAvailable())
  {
    m_softioc_service.Start(sequencergui::test::GetEpicsDBContentString());
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

void IOCEnvironment::TearDown()
{
  if (sequencergui::IsSequencerPluginEpicsAvailable())
  {
    m_softioc_service.Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}
