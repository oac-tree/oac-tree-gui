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

#ifndef SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
#define SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/operation/breakpoint_types.h>

#include <QObject>
#include <functional>

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;

/**
 * @brief The BreakpointController class stores information about breakpoints, and controls its
 * propagation to the domain runner.
 */

class BreakpointController : public QObject
{
  Q_OBJECT

public:
  using get_instruction_t = std::function<instruction_t*(const InstructionItem&)>;

  explicit BreakpointController(get_instruction_t callback, QObject* parent = nullptr);

  /**
   * @brief Save breakpoint information in internal cash.
   * @param procedure_item The procedure to explore for available breakpoints.
   *
   * @details The method is used to collect an information about all breakpoints over the
   * intrsuction tree, for later reuse.
   */
  void SaveBreakpoints(const ProcedureItem& procedure_item);

  /**
   * @brief Restore breakpoint information from internal cash.
   * @param procedure_item The procedure to restore breakpoints.
   *
   * @details The method is used to set breakpoints back to instructions in the container. It is
   * expected, that instructions do not have breakpoints already.
   */
  void RestoreBreakpoints(ProcedureItem& procedure_item);

  /**
   * @brief Sets breakpoints to the runner using breakpoint information from instruction tree.
   * @param procedure_item Expanded ProcedureItem
   *
   * @return Returns true if breakpoints have been successfully set (i.e. runner is paused, or not
   * running).
   *
   * @details It is expected that the runner doesn't have any breakpoints set, that it contains
   * a domain procedure, corresponding to given item.
   */
  bool PropagateBreakpointsToDomain(const ProcedureItem& procedure_item, runner_t& runner);

private:
  //! callback to retrieve domain instruction corresponding to given InstructionItem
  get_instruction_t m_get_domain_instruction;

  //! information
  std::vector<BreakpointInfo> m_breakpoints;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
