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

#include "domain_runner_adapter_new.h"

#include <sequencergui/core/exceptions.h>

#include <sup/sequencer/procedure.h>
#include <sup/sequencer/runner.h>

#include <chrono>
#include <iostream>
#include <thread>

namespace sequencergui
{

DomainRunnerAdapterNew::DomainRunnerAdapterNew(
    procedure_t *procedure, userinterface_t *interface,
    std::function<void(RunnerStatus)> status_changed_callback)
    : m_procedure(procedure)
    , m_userinterface(interface)
    , m_status_changed_callback(status_changed_callback)
{
  m_domain_runner = std::make_unique<runner_t>(*m_userinterface);
  m_domain_runner->SetProcedure(m_procedure);
}

DomainRunnerAdapterNew::~DomainRunnerAdapterNew() = default;

void DomainRunnerAdapterNew::SetTickTimeout(int msec)
{
  m_tick_timeout_ms.store(msec);
}

bool DomainRunnerAdapterNew::IsBusy() const
{
  return false;
}

void DomainRunnerAdapterNew::StartRequest()
{
}

void DomainRunnerAdapterNew::PauseModeOnRequest()
{
}

void DomainRunnerAdapterNew::PauseModeOffRequest()
{
}

void DomainRunnerAdapterNew::StepRequest()
{
}

void DomainRunnerAdapterNew::StopRequest()
{
}

void DomainRunnerAdapterNew::OnStatusChange(RunnerStatus status)
{
  m_status_changed_callback(status);
}

}  // namespace sequencergui
