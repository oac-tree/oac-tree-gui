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

#ifndef OAC_TREE_GUI_COMPOSER_WIDGET_FOCUS_HANDLER_H_
#define OAC_TREE_GUI_COMPOSER_WIDGET_FOCUS_HANDLER_H_

#include <QDebug>
#include <algorithm>
#include <vector>

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The ComponentFocusHandler handles the logic of which widget in the list is currently in
 * the user's focus.
 *
 * It is used in the context of multiple split views, when we need to decide which panel is
 * currently active to show new content in the appropriate place.
 *
 * It reports change in current widget in focus via callbacks.
 */
template <typename WidgetT>
class WidgetFocusHandler
{
public:
  /**
   * @brief Returns number of managed objects.
   */
  std::size_t GetCount() const;

  /**
   * @brief Adds widget to the list of managed objects.
   *
   * It will become the one in focus.
   */
  void AddWidget(WidgetT* widget);

  /**
   * @brief Removes widget from the list of managed objects.
   *
   * The last widget in the list will become the one in focus.
   */
  void RemoveWidget(WidgetT* widget);

  /**
   * @brief Sets the widget in focus.
   *
   * Previous widget in focus and new widget in focus will benotified about the change.
   */
  void SetInFocus(WidgetT* focus);

  /**
   * @brief Returns the widget in focus.
   */
  WidgetT* GetInFocus() const;

private:
  std::vector<WidgetT*> m_editors;
  WidgetT* m_in_focus{nullptr};
};

template <typename WidgetT>
inline std::size_t WidgetFocusHandler<WidgetT>::GetCount() const
{
  return m_editors.size();
}

template <typename WidgetT>
inline void WidgetFocusHandler<WidgetT>::AddWidget(WidgetT* widget)
{
  auto iter = std::find(m_editors.begin(), m_editors.end(), widget);
  if (iter != m_editors.end())
  {
    return;
  }

  SetInFocus(widget);

  if (m_editors.size() == 1)
  {
    m_editors.at(0)->ShowAsLastEditor(false);
  }

  m_editors.push_back(widget);
  if (m_editors.size() == 1)
  {
    m_editors.at(0)->ShowAsLastEditor(true);
  }
}

template <typename WidgetT>
inline void WidgetFocusHandler<WidgetT>::RemoveWidget(WidgetT* widget)
{
  auto iter = std::find(m_editors.begin(), m_editors.end(), widget);
  if (iter == m_editors.end())
  {
    return;
  }

  if (m_in_focus == widget)
  {
    m_in_focus = nullptr;
  }
  m_editors.erase(iter);

  if (!m_editors.empty())
  {
    SetInFocus(m_editors.back());
  }

  if (m_editors.size() == 1)
  {
    m_editors.at(0)->ShowAsLastEditor(true);
  }
}

template <typename WidgetT>
inline void WidgetFocusHandler<WidgetT>::SetInFocus(WidgetT* focus)
{
  qDebug() << "WidgetFocusHandler<WidgetT>::SetInFocus 1.1" << focus;
  if (m_in_focus == focus)
  {
    return;
  }

  // notify previous focus object
  if (m_in_focus)
  {
    qDebug() << "WidgetFocusHandler<WidgetT>::SetInFocus 1.2";
    m_in_focus->ShowAsActive(false);
  }

  m_in_focus = focus;

  // notify new focus object
  if (m_in_focus)
  {
    qDebug() << "WidgetFocusHandler<WidgetT>::SetInFocus 1.3";
    m_in_focus->ShowAsActive(true);
  }
}

template <typename WidgetT>
inline WidgetT* WidgetFocusHandler<WidgetT>::GetInFocus() const
{
  return m_in_focus;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_WIDGET_FOCUS_HANDLER_H_
