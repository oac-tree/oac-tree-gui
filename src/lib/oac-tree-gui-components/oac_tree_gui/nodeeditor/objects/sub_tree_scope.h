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

#ifndef OAC_TREE_GUI_NODEEDITOR_OBJECTS_SUB_TREE_SCOPE_H_
#define OAC_TREE_GUI_NODEEDITOR_OBJECTS_SUB_TREE_SCOPE_H_

#include <QObject>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace mvvm
{
class ModelListener;
}  // namespace mvvm

namespace oac_tree_gui
{

class InstructionContainerItem;
class InstructionItem;

/**
 * @brief The SubTreeScope class holds the SubTree shown in the node editor.
 *
 * A SubTree is a top-level instruction with its children. The scope is stored as the set of
 * hidden top-level instructions. Loose instructions of a SubTree are recorded on leaving it and
 * shown again on return. The container's model must outlive this object.
 */
class SubTreeScope : public QObject
{
  Q_OBJECT

public:
  explicit SubTreeScope(QObject* parent_object = nullptr);
  ~SubTreeScope() override;

  SubTreeScope(const SubTreeScope&) = delete;
  SubTreeScope& operator=(const SubTreeScope&) = delete;
  SubTreeScope(SubTreeScope&&) = delete;
  SubTreeScope& operator=(SubTreeScope&&) = delete;

  /**
   * @brief Sets the container and shows the given SubTree. Forgets loose instructions.
   *
   * @param container The instruction container, or nullptr.
   * @param top_level The top-level instruction to show, or nullptr.
   */
  void Reset(InstructionContainerItem* container, const InstructionItem* top_level);

  /**
   * @brief Removes the container. No scope, empty hidden set.
   */
  void Clear();

  /**
   * @brief Shows the SubTree of the given top-level instruction.
   *
   * Loose instructions of the current SubTree are recorded and shown again on return.
   *
   * @param top_level The top-level instruction, or nullptr to show no SubTree.
   * @return False if it is the current SubTree, or if there is no container.
   */
  bool SetScope(const InstructionItem* top_level);

  /**
   * @brief Returns the top-level instruction of the current SubTree.
   *
   * It is the top-level ancestor of the instruction given to SetScope. It follows that instruction
   * when it is connected to a visible tree.
   *
   * @return The top-level instruction, or nullptr if there is no scope, it was deleted, or it is
   * now inside a hidden tree.
   */
  const InstructionItem* GetScope() const;

  /**
   * @brief Returns identifiers of top-level instructions not shown.
   */
  const std::set<std::string>& GetHiddenIdentifiers() const;

signals:
  /**
   * @brief Notifies that the result of GetScope has changed.
   */
  void scopeChanged();

private:
  //! Sets the anchor and computes the hidden set.
  void ApplyScope(const InstructionItem* top_level);

  //! Emits scopeChanged if the result of GetScope differs from the last emitted one.
  void EmitScopeChangedIfNeeded();

  InstructionContainerItem* m_container{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;

  //!< identifier of the instruction given to SetScope, empty if none
  std::string m_anchor_identifier;

  //!< identifier of the scope at the last scopeChanged, empty if none
  std::string m_last_scope_identifier;

  //!< identifiers of top-level instructions not shown
  std::set<std::string> m_hidden_identifiers;

  //!< loose top-level instructions of each SubTree, recorded on leaving it
  std::map<std::string, std::set<std::string>> m_loose_identifiers;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_OBJECTS_SUB_TREE_SCOPE_H_
