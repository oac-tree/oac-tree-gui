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

#ifndef SEQUENCERGUI_JOBSYSTEM_REQUEST_TYPES_H_
#define SEQUENCERGUI_JOBSYSTEM_REQUEST_TYPES_H_

#include <sup/dto/anyvalue.h>

#include <string>
#include <vector>

namespace sequencergui
{

struct UserChoiceArgs
{
  std::vector<std::string> options;
  sup::dto::AnyValue metadata;
};

struct UserChoiceResult
{
  int index{0};
  bool processed{false};
};

struct UserInputArgs
{
  sup::dto::AnyValue value;
  std::string description;
};

struct UserInputResult
{
  sup::dto::AnyValue value;
  bool processed{false};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_REQUEST_TYPES_H_
