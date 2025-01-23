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

#ifndef SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
#define SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_

#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <oac-tree-gui/operation/breakpoint_types.h>

namespace sequencergui
{

class ProcedureItem;

/**
 * @brief The BreakpointController class stores information about breakpoints, and controls its
 * propagation to the domain runner.
 */
class BreakpointController
{
public:
  BreakpointController();

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

private:
  //! information
  std::vector<BreakpointInfo> m_breakpoints;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
