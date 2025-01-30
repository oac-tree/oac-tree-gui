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

#ifndef SEQUENCERGUI_TRANSFORM_I_PROCEDURE_ITEM_BUILDER_H_
#define SEQUENCERGUI_TRANSFORM_I_PROCEDURE_ITEM_BUILDER_H_

#include <cstddef>

namespace oac_tree_gui
{

class ProcedureItem;
class InstructionItem;
class VariableItem;

/**
 * @brief The IProcedureItemBuilder class is an interface to build ProcedureItem from its sequencer
 * domain counterpart.
 */
class IProcedureItemBuilder
{
public:
  virtual ~IProcedureItemBuilder() = default;

  /**
   * @brief Returns instruction item for given automation index.
   */
  virtual InstructionItem* GetInstruction(std::size_t index) const = 0;

  /**
   * @brief Returns automation index corresponding to a given instruction item.
   */
  virtual size_t GetIndex(const InstructionItem* item) const = 0;

  /**
   * @brief Returns variable item for a given automation index.
   */
  virtual VariableItem* GetVariable(std::size_t index) const = 0;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_TRANSFORM_I_PROCEDURE_ITEM_BUILDER_H_
