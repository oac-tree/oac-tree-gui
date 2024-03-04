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

#ifndef SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_SERVICE_H_
#define SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_SERVICE_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/experimental/domain_events.h>

#include <QObject>
#include <memory>

namespace sequencergui
{

class DomainRunner;
class DomainEventQueue;
class DomainEventDispatcher;
class DomainEventDispatcherContext;

/**
 * @brief The DomainRunnerService class is a wrapper around DomainRunner which provide all event
 * queue/dispatcher wiring.
 */
class DomainRunnerService : public QObject
{
public:
  explicit DomainRunnerService(procedure_t& procedure);
  ~DomainRunnerService() override;

private:
  void SetupDomainRunner(procedure_t& procedure);
  DomainEventDispatcherContext CreateDispatcherContext();

  void OnInstructionStatusChanged(const InstructionStatusChanged&);
  void OnJobStateChanged(const JobStateChanged&);

  std::unique_ptr<DomainEventQueue> m_event_queue;
  std::unique_ptr<DomainEventDispatcher> m_event_dispatcher;
  std::unique_ptr<DomainRunner> m_domain_runner;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_DOMAIN_RUNNER_ADAPTER_H_
