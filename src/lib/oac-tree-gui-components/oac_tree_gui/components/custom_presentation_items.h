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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
#define OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_

#include <mvvm/viewmodel/standard_presentation_items.h>

namespace oac_tree_gui
{

/**
 * @brief The ChannelPresentationItem class is used to show IsAvailable status of InstructionItem
 * together with channel name.
 */
class MVVM_VIEWMODEL_EXPORT ChannelPresentationItem : public mvvm::DataPresentationItem
{
public:
  /**
   * @brief Main constructor.
   *
   * @param item The item representing IsAvailable property of the instruction item.
   */
  explicit ChannelPresentationItem(mvvm::SessionItem* item, const QString& channel_name);

  QVariant Data(int qt_role) const override;

  bool SetData(const QVariant& data, int qt_role) override;

  QVector<int> GetQtRoles(int data_role) const override;

private:
  QString m_channel_name;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_CUSTOM_PRESENTATION_ITEMS_H_
