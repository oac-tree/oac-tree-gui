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

#ifndef OAC_TREE_GUI_MODEL_INSTRUCTION_INFO_ITEM_H_
#define OAC_TREE_GUI_MODEL_INSTRUCTION_INFO_ITEM_H_

#include <oac_tree_gui/model/instruction_item.h>

namespace oac_tree_gui
{

/**
 * @brief The InstructionInfoItem is a simplified instruction with status and description used to
 * build instruction tree from JobInfo domain information.
 */
class InstructionInfoItem : public InstructionItem
{
public:
  InstructionInfoItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  void InitFromDomainInfo(const sup::oac_tree::InstructionInfo& info);

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;

  void SetupFromDomain(const sup::oac_tree::InstructionInfo& info);

};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_INSTRUCTION_INFO_ITEM_H_
