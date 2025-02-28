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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

/**
 * @brief The InstructionItem class is a base for all GUI items representind Sequencer domain
 * instructions.
 */
class InstructionItem : public mvvm::CompoundItem
{
public:
  explicit InstructionItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns domain type.
   */
  std::string GetDomainType() const;

  /**
   * @brief Inits given item from the domain instruction.
   */
  void InitFromDomain(const instruction_t* instruction);

  /**
   * @brief Creates domain counterpart.
   */
  std::unique_ptr<instruction_t> CreateDomainInstruction() const;

  /**
   * @brief Returns all child instrucitons.
   */
  std::vector<InstructionItem*> GetInstructions() const;

  /**
   * @brief Checks if this instruction has root flag set.
   */
  bool IsRoot() const;

  /**
   * @brief Sets root flag to a given value.
   */
  void SetIsRootFlag(bool value);

  /**
   * @brief Returns instruction name.
   *
   * This is where domain description information is stored.
   */
  std::string GetName() const;

  /**
   * @brief Sets the name to a given value.
   */
  void SetName(const std::string& name);

  /**
   * @brief Returns instruction status.
   */
  std::string GetStatus() const;

  /**
   * @brief Sets status to a given value.
   */
  void SetStatus(const std::string& status);

  /**
   * @brief Retuns x-coordinate of instruction on NodeEditor graphics scene.
   */
  double GetX() const;

  /**
   * @brief Set NodeEditor graphics scene x-coordinate to a given value.
   */
  void SetX(double value);

  /**
   * @brief Retuns y-coordinate of instruction on NodeEditor graphics scene.
   */
  double GetY() const;

  /**
   * @brief Set NodeEditor graphics scene y-coordinate to a given value.
   */
  void SetY(double value);

protected:
  /**
   * @brief Creates item properties common for all instructions.
   *
   * These are execution status, (x,y) coordinates and breakpoint information.
   */
  void RegisterCommonProperties();

private:
  /**
   * @brief Init this item from domain information.
   */
  virtual void InitFromDomainImpl(const instruction_t* instruction) = 0;

  /**
   * @brief Setup domain instruction using current property values.
   */
  virtual void SetupDomainImpl(instruction_t* instruction) const = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_ITEM_H_
