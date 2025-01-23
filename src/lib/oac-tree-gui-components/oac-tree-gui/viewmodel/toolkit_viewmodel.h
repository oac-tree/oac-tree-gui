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

#ifndef SEQUENCERGUI_VIEWMODEL_TOOLKIT_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_TOOLKIT_VIEWMODEL_H_

#include <oac-tree-gui/domain/domain_object_group_helper.h>

#include <QStandardItemModel>

namespace sequencergui
{

/**
 * @brief The ToolKitViewModel class provides a viewmodel with a available object types, intended
 * for dragging.
 *
 * The model can be populated either with the list, or with the tree-like structure.
 */
class ToolKitViewModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit ToolKitViewModel(QObject* parent_object = nullptr);

  QMimeData* mimeData(const QModelIndexList& index_list) const override;

  /**
   * @brief Populate model so it contains a draggable list of object type names.
   *
   * This will clear the model from the previous content.
   */
  void PopulateModel(const std::vector<std::string>& object_types);

  /**
   * @brief Populate model so it contains a draggable tree of object groups with names in it.
   *
   * This will clear the model from the previous content.
   */
  void PopulateModel(const std::vector<ObjectGroupInfo>& objects_group_info);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_TOOLKIT_VIEWMODEL_H_
