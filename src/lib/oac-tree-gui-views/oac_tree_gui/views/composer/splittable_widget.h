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

#ifndef OAC_TREE_GUI_VIEWS_SPLITTABLE_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_SPLITTABLE_WIDGET_H_

#include <QWidget>
#include <functional>
#include <memory>

class QSplitter;

namespace oac_tree_gui
{

/**
 * @brief The SplittableWidget class holds a splitter and logic to dynamically add/remove splitter's
 * content.
 */
class SplittableWidget : public QWidget
{
  Q_OBJECT

public:
  using CreateWidgetCallback = std::function<std::unique_ptr<QWidget>()>;

  explicit SplittableWidget(CreateWidgetCallback callback, QWidget* parent_widget = nullptr);
  ~SplittableWidget() override;

  /**
   * @brief Gets the internal splitter.
   */
  QSplitter* GetSplitter() const;

  /**
   * @brief Adds a new widget to the splitter.
   *
   * @param after_widget If nullptr, the new widget is added at the end. Otherwise, it is added
   * after the specified widget.
   */
  void AddWidget(QWidget* after_widget = nullptr);

  /**
   * @brief Closes and deletes the specified widget in the splitter.
   */
  static void CloseWidget(QWidget* widget_to_close);

private:
  CreateWidgetCallback m_callback;
  QSplitter* m_splitter{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_SPLITTABLE_WIDGET_H_
