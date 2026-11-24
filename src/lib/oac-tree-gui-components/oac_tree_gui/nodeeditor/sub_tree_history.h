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

#ifndef OAC_TREE_GUI_NODEEDITOR_SUB_TREE_HISTORY_H_
#define OAC_TREE_GUI_NODEEDITOR_SUB_TREE_HISTORY_H_

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The SubTreeHistoryEntry struct is a SubTree visited in the node editor.
 */
struct SubTreeHistoryEntry
{
  std::string scope_identifier;    //!< top-level instruction shown, empty if none
  std::string include_identifier;  //!< Include that led here, empty if none
};

/**
 * @brief The SubTreeHistory class holds the SubTrees visited in the node editor, for Back/Forward.
 *
 * A trail is a run of entries reached by Include hops, shown in the breadcrumb. An entry without
 * an Include starts a new trail.
 */
class SubTreeHistory
{
public:
  using predicate_t = std::function<bool(const SubTreeHistoryEntry&)>;

  /**
   * @brief Removes all entries.
   */
  void Clear();

  /**
   * @brief Removes all entries and adds the given one.
   */
  void Reset(const SubTreeHistoryEntry& entry);

  /**
   * @brief Adds an entry after the current one and makes it current. Drops forward entries.
   */
  void Push(const SubTreeHistoryEntry& entry);

  /**
   * @brief Returns all entries.
   */
  const std::vector<SubTreeHistoryEntry>& GetEntries() const&;

  /**
   * @brief Returns the index of the current entry, 0 if empty.
   */
  std::size_t GetCurrentIndex() const;

  /**
   * @brief Makes the entry with the given index current.
   *
   * @throw RuntimeException if the index is out of range.
   */
  void SetCurrentIndex(std::size_t index);

  /**
   * @brief Returns the index of the nearest entry before the current one that matches.
   */
  std::optional<std::size_t> FindPrevious(const predicate_t& predicate) const;

  /**
   * @brief Returns the index of the nearest entry after the current one that matches.
   */
  std::optional<std::size_t> FindNext(const predicate_t& predicate) const;

  /**
   * @brief Returns the index of the first entry of the current trail, 0 if empty.
   *
   * It is the last entry up to the current one without an Include.
   */
  std::size_t GetTrailStart() const;

private:
  std::vector<SubTreeHistoryEntry> m_entries;
  std::size_t m_current_index{0};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_SUB_TREE_HISTORY_H_
