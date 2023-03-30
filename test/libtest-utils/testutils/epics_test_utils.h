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

#ifndef LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_

#include <string>
#include <vector>

namespace testutils
{

//! Returns string representing EPICS database file with several testing variables.
std::string GetEpicsDBContentString();

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
