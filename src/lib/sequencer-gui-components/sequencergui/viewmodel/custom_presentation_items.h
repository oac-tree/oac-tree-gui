/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H_
#define SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H_

#include <mvvm/viewmodel/standard_presentation_items.h>

namespace sequencergui
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

  virtual QVector<int> GetQtRoles(int data_role) const override;

private:
  QString m_channel_name;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H_
