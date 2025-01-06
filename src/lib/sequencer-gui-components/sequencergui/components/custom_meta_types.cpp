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

#include <QDataStream>
#include <mutex>

namespace sequencergui
{

void RegisterCustomMetaTypes()
{
  static std::once_flag register_once_flag;

  auto register_func = []()
  {
    (void)qRegisterMetaType<sequencergui::InstructionItem*>("sequencergui::InstructionItem*");
    (void)qRegisterMetaType<sequencergui::ProcedureItem*>("sequencergui::ProcedureItem*");
    (void)qRegisterMetaType<std::vector<sequencergui::InstructionItem*>>(
        "std::vector<sequencergui::InstructionItem*>");
    (void)qRegisterMetaType<const sequencergui::InstructionItem*>(
        "const sequencergui::InstructionItem*");
    (void)qRegisterMetaType<instruction_t*>("instruction_t*");
    (void)qRegisterMetaType<const instruction_t*>("const instruction_t*");
    (void)qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
    (void)qRegisterMetaType<sequencergui::JobItem*>("sequencergui::JobItem*");
    (void)qRegisterMetaType<LogEvent>("sequencergui::LogEvent");
    (void)qRegisterMetaType<sequencergui::RunnerStatus>("sequencergui::RunnerStatus");
    (void)qRegisterMetaType<QList<int>>("QList<int>");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");
#endif
  };

  std::call_once(register_once_flag, register_func);
}

}  // namespace sequencergui
