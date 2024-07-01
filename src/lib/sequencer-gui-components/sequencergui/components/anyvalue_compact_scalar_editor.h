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

#ifndef SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_
#define SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_

#include <sup/gui/anyvalueeditor/abstract_anyvalue_editor.h>

class QLabel;
class QString;
class QDataWidgetMapper;
class QBoxLayout;

namespace mvvm
{
class ApplicationModel;
class ViewModelDelegate;
class ViewModel;
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace sequencergui
{

/**
 * @brief The AnyValueCompactScalarEditor class provides a simplified AnyValueEditor for scalar
 * types.
 *
 * It has only a single cell editor occupying horizontal space.
 *
 * The implementation looks bulky for what it does. The are following reasons:
 * a) We want to re-use cell-editor machinery since it has all proper limits for all scalar types.
 * This requires viewmodel and delegate to be in place.
 * b) We might consider to extend this editor to show more than one cell-editor in a grid view, i.e.
 * for all leaves of a simple struct.
 */
class AnyValueCompactScalarEditor : public sup::gui::AbstractAnyValueEditor
{
  Q_OBJECT

public:
  explicit AnyValueCompactScalarEditor(QWidget* parent = nullptr);
  ~AnyValueCompactScalarEditor() override;

  void SetDescription(const QString& text) override;

  void SetInitialValue(const sup::gui::AnyValueItem* item) override;

  std::unique_ptr<sup::gui::AnyValueItem> GetResult() override;

private:
  std::unique_ptr<mvvm::ApplicationModel> m_model;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
  std::unique_ptr<mvvm::ViewModel> m_view_model;
  std::unique_ptr<QDataWidgetMapper> m_widget_mapper;

  QBoxLayout* m_layout{nullptr};
  QLabel* m_label{nullptr};
  QWidget* m_editor{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_ANYVALUE_COMPACT_SCALAR_EDITOR_H_
