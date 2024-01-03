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
  using get_instruction_t = std::function<const instruction_t*(const InstructionItem&)>;

  explicit BreakpointController(get_instruction_t callback, QObject* parent = nullptr);

  /**
   * @brief Save breakpoint information in internal cash.
   *
   * @param procedure_item The procedure to explore available breakpoints.
   *
   * @details The method is used to collect information about all breakpoints over the
   * instruction tree. Later, breakpoints can be set to another instruction tree with the same
   * layout.
   */
  void SaveBreakpoints(const ProcedureItem& procedure_item);

  /**
   * @brief Restore breakpoint information from internal cash.
   * @param procedure_item The procedure to restore breakpoints.
   *
   * @details The method is used to set breakpoints back to instructions in the container.
   * Instructions should not have breakpoints already set. It is expected that instruction
   * tree has the same layout as the original, used to collect breakpoint information.
   */
  void RestoreBreakpoints(ProcedureItem& procedure_item);

  /**
   * @brief Sets breakpoints to the runner using breakpoint information from instruction tree.
   *
   * @param procedure_item Expanded ProcedureItem
   *
   * @return Returns true if breakpoints have been successfully set (i.e., the runner is paused or
   * not running).
   *
   * @details It is expected that the runner doesn't have any breakpoints set, that it contains
   * a domain procedure corresponding to a given item.
   */
  bool PropagateBreakpointsToDomain(const ProcedureItem& item, runner_t& runner);

  /**
   * @brief Updates domain breakpoint to the InstructionItem breakpoint.
   *
   * @param item Instruction item
   * @param runner Runner to use to set breakpoints
   *
   * @return Returns true if breakpoints have been successfully set (i.e., the runner is paused or
   * not running).
   *
   * @details We do not use disabled status in the domain. InstructionItem's breakpoint marked as
   * disabled will remove breakpoint from the domain.
   */
  bool UpdateDomainBreakpoint(const InstructionItem& item, runner_t& runner);

  /**
   * @brief Find domain instruction corresponding to a given item.
   */
  const instruction_t* FindDomainInstruction(const InstructionItem& item);

private:
  //! callback to retrieve domain instruction corresponding to given InstructionItem
  get_instruction_t m_get_domain_instruction;

  //! information
  std::vector<BreakpointInfo> m_breakpoints;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
