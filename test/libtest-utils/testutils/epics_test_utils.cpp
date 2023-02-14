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

#include "epics_test_utils.h"

namespace
{

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

record (longin,"CA-TESTS:INT")
{
  field(DESC,"Test is running flag")
  field(VAL,"1")
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

namespace testutils
{

std::string GetEpicsDBContentString()
{
  return db_content;
}
}  // namespace testutils
