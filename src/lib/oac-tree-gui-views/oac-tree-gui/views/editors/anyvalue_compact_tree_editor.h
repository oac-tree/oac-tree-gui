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

#ifndef SEQUENCERGUI_VIEWS_EDITORS_ANYVALUE_COMPACT_TREE_EDITOR_H_
#define SEQUENCERGUI_VIEWS_EDITORS_ANYVALUE_COMPACT_TREE_EDITOR_H_

#include <sup/gui/views/anyvalueeditor/abstract_anyvalue_editor.h>

class QLabel;
class QString;
class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
class ApplicationModel;
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace oac_tree_gui
{

/**
 * @brief The AnyValueCompactTreeEditor class provides a simplified AnyValueEditor in a form of a
 * tree.
 *
 * It is intended to type in values for struct and arrays, change of the type is not possible.
 */

class AnyValueCompactTreeEditor : public sup::gui::AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueCompactTreeEditor(QWidget* parent_widget = nullptr);
  ~AnyValueCompactTreeEditor() override;

  void SetDescription(const QString& text) override;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() override;

private:
  std::unique_ptr<mvvm::ApplicationModel> m_model;

  QLabel* m_label{nullptr};
  QTreeView* m_tree_view{nullptr};

  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_VIEWS_EDITORS_ANYVALUE_COMPACT_TREE_EDITOR_H_
