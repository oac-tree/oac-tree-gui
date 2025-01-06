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

#include "test_utils.h"

#include <string>
#include <thread>

namespace
{
const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<root>)RAW"};

const std::string footer{R"RAW(</root>
)RAW"};

using clock = std::chrono::high_resolution_clock;
}  // namespace

namespace testutils
{

bool WaitFor(std::function<bool()> predicate, std::chrono::milliseconds timeout)
{
  const std::chrono::milliseconds timeout_precision_msec(5);
  const std::chrono::milliseconds wait_time(timeout);
  auto time_end = clock::now() + timeout;
  while (clock::now() < time_end)
  {
    if (predicate())
    {
      return true;
    }
    std::this_thread::sleep_for(timeout_precision_msec);
  }
  return false;
}

double GetTimeoutInSec(std::chrono::milliseconds timeout)
{
  auto value = std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count();
  return static_cast<double>(value) / 1000;
}

std::string CreateProcedureString(const std::string &body, bool schema)
{
  static const std::string header_with_schema{
      R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure xmlns="http://codac.iter.org/sup/sequencer" version="1.0"
           name="Trivial procedure for testing purposes"
           xmlns:xs="http://www.w3.org/2001/XMLSchema-instance"
           xs:schemaLocation="http://codac.iter.org/sup/sequencer sequencer.xsd">)RAW"};

  static const std::string header{R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>)RAW"};

  static const std::string footer{R"RAW(</Procedure>
)RAW"};

  return (schema ? header_with_schema : header) + body + footer;
}

}  // namespace testutils
