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

#ifndef OAC_TREE_GUI_COMPONENTS_TEXT_EDIT_CONTROLLER_H_
#define OAC_TREE_GUI_COMPONENTS_TEXT_EDIT_CONTROLLER_H_

#include <mvvm/signals/item_controller.h>

#include <QObject>

class QPlainTextEdit;
class QCheckBox;

namespace oac_tree_gui
{

/**
 * @brief The TextControllerContext is a collection of data to initialize TextEditController.
 */
struct TextControllerContext
{
  QCheckBox* check_box{nullptr};
  QPlainTextEdit* text_edit{nullptr};
};

class TextEditItem;

/**
 * @brief The TextEditController class is a controller to synchronize properties of TextEditItem
 * with QCheckBox and QTextEdit.
 *
 * The task of the controller is to enable/disable QTextEdit depending on the state of QCheckBox
 * and to synchronize values of both widgets with corresponding properties of TextEditItem.
 */
class TextEditController : public mvvm::ItemController<TextEditItem>
{
public:
  explicit TextEditController(const TextControllerContext& context);
  ~TextEditController() override;

  TextEditController(const TextEditController&) = delete;
  TextEditController& operator=(const TextEditController&) = delete;
  TextEditController(TextEditController&&) = delete;
  TextEditController& operator=(TextEditController&&) = delete;

protected:
  void Subscribe() override;
  void Unsubscribe() override;

  /**
   * @brief Connects Qt signals.
   */
  void SetQtConnected();

  /**
   * @brief Disconnects Qt signals.
   */
  void SetQtDisonnected();

private:
  /**
   * @brief Update Qt widgets.
   */
  void OnPropertyChangedEvent(const mvvm::PropertyChangedEvent& event);

  void UpdateWidgetStateFromItem();
  TextControllerContext m_context;
  std::unique_ptr<QMetaObject::Connection> m_text_edit_connection;
  std::unique_ptr<QMetaObject::Connection> m_checkbox_connection;
  bool m_do_not_update_widgets{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_TEXT_EDIT_CONTROLLER_H_
