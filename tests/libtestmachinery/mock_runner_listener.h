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

#ifndef MOCKRUNNERLISTENER_H
#define MOCKRUNNERLISTENER_H

#include "sequencergui/monitor/job_types.h"

#include <gmock/gmock.h>

#include <functional>

namespace testutils
{

//! Auxiliary class to listen for RunnerStatus changed events.

class MockRunnerListener
{
public:
  MOCK_METHOD1(StatusChanged, void(sequencergui::RunnerStatus status));

  //! Creates callback to listen for status change.
  //! The call will be propagated to StatusChanged mock method to benefit from gmock.
  std::function<void(sequencergui::RunnerStatus)> CreateCallback()
  {
    return [this](auto status) { StatusChanged(status); };
  }
};

}  // namespace testutils

#endif  //  MOCKRUNNERLISTENER_H
