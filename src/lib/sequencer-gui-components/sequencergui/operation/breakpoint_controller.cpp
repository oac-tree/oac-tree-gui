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

#include "breakpoint_controller.h"

#include "breakpoint_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>

#include <sup/sequencer/runner.h>

#include <QDebug>

namespace sequencergui
{

BreakpointController::BreakpointController(get_instruction_t callback, QObject *parent)
    : QObject(parent), m_get_domain_instruction(std::move(callback))
{
}

void BreakpointController::SaveBreakpoints(const ProcedureItem &procedure_item)
{
  m_breakpoints = std::move(CollectBreakpointInfo(*procedure_item.GetInstructionContainer()));
}

void BreakpointController::RestoreBreakpoints(ProcedureItem &procedure_item)
{
  SetBreakpointsFromInfo(m_breakpoints, *procedure_item.GetInstructionContainer());
}

bool BreakpointController::PropagateBreakpointsToDomain(const ProcedureItem &item, runner_t &runner)
{
  return false;
}

bool BreakpointController::UpdateDomainBreakpoint(const InstructionItem &item, runner_t &runner)
{
  if (runner.IsRunning())
  {
    qDebug() << "Can't propagate instruction breakpoint to the domain. Runner is still running.";
    return false;
  }

  auto domain_instruction = FindDomainInstruction(item);
  if (!domain_instruction)
  {
    qDebug() << "Can't find domain instruction corresponding to current InstructionItem.";
    return false;
  }

  auto breakpoint_status = GetBreakpointStatus(item);
  if (breakpoint_status == BreakpointStatus::kSet)
  {
    runner.EnableBreakpoint(domain_instruction);
  }
  else
  {
    // We do not used "disabled" breakpoints in the domain, InstructionItem's breakpoint marked as
    // disabled, will remove breakpoint from the domain
    runner.RemoveBreakpoint(domain_instruction);
  }

  return true;
}

instruction_t *BreakpointController::FindDomainInstruction(const InstructionItem &item)
{
  if (!m_get_domain_instruction)
  {
    throw RuntimeException("Callback is not set");
  }

  return m_get_domain_instruction(item);
}

}  // namespace sequencergui
