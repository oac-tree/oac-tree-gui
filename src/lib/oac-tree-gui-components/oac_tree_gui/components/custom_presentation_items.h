/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
#define OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_

#include <mvvm/providers/boolean_data_presentation_item.h>
#include <mvvm/providers/default_data_presentation_item.h>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

class InstructionItem;
class InstructionContainerItem;

/**
 * @brief The ChannelPresentationItem class is used to show IsAvailable status of InstructionItem
 * together with channel name.
 */
class ChannelPresentationItem : public mvvm::DefaultDataPresentationItem
{
public:
  /**
   * @brief Main constructor.
   *
   * @param item The item representing IsAvailable property of the instruction item.
   */
  explicit ChannelPresentationItem(mvvm::SessionItem* item, const QString& channel_name);

  QVariant Data(mvvm::role_t qt_role) const override;

  bool SetData(const QVariant& data, mvvm::role_t qt_role) override;

  QVector<int> GetQtRolesToEmit(mvvm::role_t data_role) const override;

private:
  QString m_channel_name;
};

/**
 * @brief The ExclusiveCheckStatePresentationItem class is used to show IsRoot status of
 * InstructionItem.
 *
 * This item is exclusive because only one instruction in procedure can be root, so it has to reset
 * other root checkboxes when set.
 *
 * It is assumed that all instructions for which this presentation is constructed,
 * are direct children of the same instruction container.
 */
class ExclusiveCheckStatePresentationItem : public mvvm::BooleanDataPresentationItem
{
public:
  explicit ExclusiveCheckStatePresentationItem(mvvm::SessionItem* item);

  bool SetData(const QVariant& data, mvvm::role_t qt_role) override;

  std::vector<InstructionItem*> GetSiblings() const;

  const InstructionContainerItem* GetInstructionContainer() const;

  InstructionItem* GetInstructionItem() const;

private:
  mvvm::ISessionModel* GetModel();
  void BeginMacro();
  void EndMacro();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
