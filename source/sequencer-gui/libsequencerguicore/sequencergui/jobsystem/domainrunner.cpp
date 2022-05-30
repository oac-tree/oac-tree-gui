/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
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

#include "sequencergui/jobsystem/domainrunner.h"

#include "Procedure.h"
#include "Runner.h"
#include "sequencergui/jobsystem/functionrunner.h"

namespace sequencergui
{

DomainRunner::DomainRunner(procedure_t *procedure, userinterface_t *user_interface,
                           std::function<void(RunnerStatus)> status_changed_callback)
    : m_procedure(procedure), m_user_interface(user_interface)
{
  m_domain_runner = std::make_unique<sup::sequencer::Runner>(m_user_interface);
  m_domain_runner->SetProcedure(m_procedure);

  auto worker = [this]() { return ExecuteSingle(); };
  m_function_runner = std::make_unique<FunctionRunner>(worker, status_changed_callback);
}

bool DomainRunner::Start()
{
  return m_function_runner->Start();
}

bool DomainRunner::Stop()
{
  return m_function_runner->Stop();
}

bool DomainRunner::Pause()
{
  return m_function_runner->Pause();
}

bool DomainRunner::Step()
{
  return m_function_runner->Step();
}

bool DomainRunner::ExecuteSingle()
{
  m_domain_runner->ExecuteSingle();
  return !m_domain_runner->IsFinished();
}

DomainRunner::~DomainRunner() = default;

}  // namespace sequencergui
