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

#ifndef SEQUENCERGUI_VIEWMODEL_INSTRUCTION_TOOLKIT_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_INSTRUCTION_TOOLKIT_VIEWMODEL_H_

#include <QStandardItemModel>

#include <sequencergui/composer/composer_helper.h>

namespace sequencergui
{

/**
 * @brief The InstructionToolKitViewModel class provides a viewmodel with a tree of available
 * instructions sorted according to their plugin names.
 */
class InstructionToolKitViewModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit InstructionToolKitViewModel(QObject* parent = nullptr);

  QMimeData* mimeData(const QModelIndexList& index_list) const override;

  /**
   * @brief Populate model so it contains a draggable list of object type names.
   */
  void PopulateModel(const std::vector<std::string>& object_types);

  /**
   * @brief Populate model so it contains a draggable tree of object groups with names in it.
   */
  void PopulateModel(const std::vector<ObjectGroupInfo>& objects_group_info);

private:
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_INSTRUCTION_TOOLKIT_VIEWMODEL_H_
