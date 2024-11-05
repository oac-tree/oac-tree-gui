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

#include "sequencergui/jobsystem/remote_job_handler.h"
#include "test_automation_server.h"

#include <gtest/gtest.h>
#include <thread>

namespace sequencergui
{

class RemoteJobHandlerTest : public ::testing::Test
{
public:
};

TEST_F(RemoteJobHandlerTest, SimpleProcedure)
{
  const std::string kSequenceTwoWaitsBody{
      R"RAW(
  <Repeat maxCount="3">
    <Sequence>
       <Increment varName="var0"/>
    </Sequence>
  </Repeat>
  <Workspace>
    <Local name="var0" type='{"type":"uint32"}' value='0'/>
  </Workspace>
)RAW"};

  const std::string server_name("RemoteJobHandlerTestServer");

  testutils::TestAutomationServer server;

  server.Start(server_name, kSequenceTwoWaitsBody);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  server.Stop();

}

}  // namespace sequencergui
