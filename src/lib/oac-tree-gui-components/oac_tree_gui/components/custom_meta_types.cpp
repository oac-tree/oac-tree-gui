/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "custom_meta_types.h"

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/job_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/runner_status.h>

#include <sup/oac-tree/instruction.h>

#include <QDataStream>
#include <mutex>

namespace oac_tree_gui
{

void RegisterCustomMetaTypes()
{
  static std::once_flag register_once_flag;

  auto register_func = []()
  {
    (void)qRegisterMetaType<oac_tree_gui::InstructionItem*>("oac_tree_gui::InstructionItem*");
    (void)qRegisterMetaType<oac_tree_gui::ProcedureItem*>("oac_tree_gui::ProcedureItem*");
    (void)qRegisterMetaType<std::vector<oac_tree_gui::InstructionItem*>>(
        "std::vector<oac_tree_gui::InstructionItem*>");
    (void)qRegisterMetaType<const oac_tree_gui::InstructionItem*>(
        "const oac_tree_gui::InstructionItem*");
    (void)qRegisterMetaType<instruction_t*>("instruction_t*");
    (void)qRegisterMetaType<const instruction_t*>("const instruction_t*");
    (void)qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
    (void)qRegisterMetaType<oac_tree_gui::JobItem*>("oac_tree_gui::JobItem*");
    (void)qRegisterMetaType<LogEvent>("oac_tree_gui::LogEvent");
    (void)qRegisterMetaType<oac_tree_gui::RunnerStatus>("oac_tree_gui::RunnerStatus");
    (void)qRegisterMetaType<QList<int>>("QList<int>");
    (void)qRegisterMetaType<oac_tree_gui::GraphicsViewOperationMode>("oac_tree_gui::GraphicsViewOperationMode");

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qRegisterMetaTypeStreamOperators<QList<int>>("QList<int>");
#endif
  };

  std::call_once(register_once_flag, register_func);
}

}  // namespace oac_tree_gui
