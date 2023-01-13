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

#ifndef SUP_EPICS_SOFTIOCUTILS_H_
#define SUP_EPICS_SOFTIOCUTILS_H_

#include <string>
#include <vector>

namespace testutils
{

//! Returns path where EPICS binaries are located.
std::string GetEPICSBinaryPath();

//! Returns string representing EPICS database file with several testing variables.
std::string GetEpicsDBContentString();

//! Removes file with given name from disk.
void RemoveFile(const std::string& file_name);

//! Returns string representing PVGet output for given variable names.
std::string GetPvGetOutput(const std::vector<std::string>& variable_names,
                           const std::string& file_name = {});

//! Returns string representing `pvget` output for given variable name.
std::string GetPvGetOutput(const std::string& variable_name, const std::string& file_name = {});

//! Put the value into PV with `pvput` command, returns string representing command output.
std::string PvPut(const std::string& variable_name, const std::string& value);

}

#endif  // SUP_EPICS_SOFTIOCUTILS_H_
