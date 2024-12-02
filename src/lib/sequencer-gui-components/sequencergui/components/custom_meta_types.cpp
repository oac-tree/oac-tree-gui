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

#include "custom_meta_types.h"

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/runner_status.h>

#include <sup/sequencer/instruction.h>

namespace sequencergui
{
void RegisterCustomMetaTypes()
{
  qRegisterMetaType<sequencergui::InstructionItem*>("sequencergui::InstructionItem*");
  qRegisterMetaType<sequencergui::ProcedureItem*>("sequencergui::ProcedureItem*");
  qRegisterMetaType<std::vector<sequencergui::InstructionItem*>>(
      "std::vector<sequencergui::InstructionItem*>");
  qRegisterMetaType<const sequencergui::InstructionItem*>("const sequencergui::InstructionItem*");
  qRegisterMetaType<instruction_t*>("instruction_t*");
  qRegisterMetaType<const instruction_t*>("const instruction_t*");
  qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
  qRegisterMetaType<sequencergui::JobItem*>("sequencergui::JobItem*");
  qRegisterMetaType<LogEvent>("sequencergui::LogEvent");
  qRegisterMetaType<sequencergui::RunnerStatus>("sequencergui::RunnerStatus");
}

}  // namespace sequencergui
