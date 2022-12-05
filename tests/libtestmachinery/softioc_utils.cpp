/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
 *
 * Author        : Walter Van Herck (IO)
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

#include "softioc_utils.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <sstream>
namespace
{

//! Generates random file name for temp directory.
std::string GetTempFileName()
{
  std::random_device rndm_device;
  std::mt19937 gen(rndm_device());
  std::string alphabet = "abc123";
  std::shuffle(std::begin(alphabet), std::end(alphabet), gen);
  return "/tmp/sup_epics_tests_" + alphabet + ".out";
}

const std::string db_content = R"RAW(
record (bo,"CA-TESTS:BOOL")
{
    field(DESC,"Some EPICSv3 record")
    field(ONAM,"TRUE")
    field(OSV,"NO_ALARM")
    field(ZNAM,"FALSE")
    field(ZSV,"NO_ALARM")
    field(VAL,"0")
}

record (ao,"CA-TESTS:FLOAT")
{
    field(DESC,"Some EPICSv3 record")
    field(DRVH,"5.0")
    field(DRVL,"-5.0")
    field(VAL,"0")
}

record (stringout,"CA-TESTS:STRING")
{
    field(DESC,"Some EPICSv3 record")
    field(VAL,"undefined")
}

record (waveform,"CA-TESTS:CHARRAY")
{
    field(DESC,"Some EPICSv3 record")
    field(FTVL, "CHAR")
    field(NELM, "1024")
}
)RAW";

}  // unnamed namespace

std::string GetEPICSBinaryPath()
{
  return std::string(std::getenv("EPICS_BASE")) + "/bin/"
         + std::string(std::getenv("EPICS_HOST_ARCH")) + "/";
}

std::string GetEpicsDBContentString()
{
  return db_content;
}

void RemoveFile(const std::string &file_name)
{
  std::remove(file_name.c_str());
}

std::string GetPvGetOutput(const std::vector<std::string> &variable_names,
                           const std::string &file_name)
{
  std::string out_name = file_name.empty() ? GetTempFileName() : file_name;

  std::string variable_names_str;
  for (const auto &str : variable_names)
  {
    variable_names_str += str + " ";
  }

  std::string command(GetEPICSBinaryPath() + "pvget -v " + variable_names_str + " > " + out_name);
  if (std::system(command.c_str()) != 0)
  {
    return {};
  }

  std::stringstream sstr;
  sstr << std::ifstream(out_name).rdbuf();

  RemoveFile(out_name);

  return sstr.str();
}

std::string GetPvGetOutput(const std::string &variable_name, const std::string &file_name)
{
  return GetPvGetOutput(std::vector<std::string>({variable_name}), file_name);
}

std::string PvPut(const std::string &variable_name, const std::string &value)
{
  auto out_file_name = GetTempFileName();

  std::string command(GetEPICSBinaryPath() + "pvput " + variable_name + " " + value + " > "
                      + out_file_name);
  if (std::system(command.c_str()) != 0)
  {
    return {};
  }

  std::stringstream sstr;
  sstr << std::ifstream(out_file_name).rdbuf();

  RemoveFile(out_file_name);

  return sstr.str();
}
