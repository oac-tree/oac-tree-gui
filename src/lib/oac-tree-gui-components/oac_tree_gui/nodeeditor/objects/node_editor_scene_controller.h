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

#ifndef OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_EDITOR_SCENE_CONTROLLER_H_
#define OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_EDITOR_SCENE_CONTROLLER_H_

#include <oac_tree_gui/model/component_types.h>
#include <oac_tree_gui/nodeeditor/sub_tree_history.h>

#include <sup/gui/core/message_event.h>

#include <QObject>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace oac_tree_gui
{

class GraphicsSceneComponentProvider;
class InstructionContainerItem;
class InstructionItem;
class NodeGraphicsScene;
class ProcedureItem;
class SubTreeScope;

/**
 * @brief The NodeEditorSceneController class controls what the node editor scene shows.
 *
 * It owns GraphicsSceneComponentProvider and rebuilds it on scope switch. In editor mode the scene
 * shows one SubTree (a top-level instruction) with its loose instructions, as held by SubTreeScope.
 * In viewer mode it shows all top-level instructions, read-only. Scope switches made by the user
 * are recorded in SubTreeHistory for Back/Forward.
 */
class NodeEditorSceneController : public QObject
{
  Q_OBJECT

public:
  using send_message_t = std::function<void(const sup::gui::MessageEvent&)>;

  /**
   * @brief Main c-tor.
   *
   * @param editor_mode Editor or read-only viewer.
   * @param scene The scene to populate.
   * @param send_message_callback Callback to report errors.
   */
  NodeEditorSceneController(NodeEditorMode editor_mode, NodeGraphicsScene* scene,
                            send_message_t send_message_callback);
  ~NodeEditorSceneController() override;

  NodeEditorSceneController(const NodeEditorSceneController&) = delete;
  NodeEditorSceneController& operator=(const NodeEditorSceneController&) = delete;
  NodeEditorSceneController(NodeEditorSceneController&&) = delete;
  NodeEditorSceneController& operator=(NodeEditorSceneController&&) = delete;

  /**
   * @brief Sets the procedure. Shows the root SubTree, forgets loose instructions and the history.
   */
  void SetProcedure(ProcedureItem* procedure);

  /**
   * @brief Returns the procedure, or nullptr.
   */
  ProcedureItem* GetProcedure() const;

  /**
   * @brief Returns the instruction container of the procedure, or nullptr.
   */
  InstructionContainerItem* GetInstructionContainer() const;

  /**
   * @brief Creates or destroys the provider. Inactive, the scene is empty.
   */
  void SetActive(bool value);

  /**
   * @brief Shows the SubTree of the given top-level instruction. Editor mode only.
   *
   * Loose instructions of the current SubTree are recorded and shown again on return. The switch
   * is added to the history and starts a new trail.
   *
   * @param top_level The top-level instruction, or nullptr to show no SubTree.
   */
  void SetScope(const InstructionItem* top_level);

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
   * @brief Switches to the SubTree of the given instruction if this SubTree is hidden.
   */
  void ShowInstruction(const InstructionItem& instruction);

  /**
   * @brief Returns why the SubTree of the given instruction can't be opened.
   *
   * Only a local Include whose path resolves can be opened.
   *
   * @return The reason, or an empty string if it can be opened.
   */
  std::string GetOpenSubTreeError(const InstructionItem& instruction) const;

  /**
   * @brief Shows the SubTree that the given Include points to. Editor mode only.
   *
   * Switches to the top-level instruction of the target. A nested target is selected. If the
   * Include can't be opened, sends a message instead. The switch is added to the history and
   * extends the current trail.
   */
  void OpenSubTree(const InstructionItem& include);

  /**
   * @brief Shows the SubTree of the given instruction, selects the instruction and centers it.
   *
   * Editor mode only. A switch is added to the history and starts a new trail.
   */
  void GoToInstruction(const InstructionItem& instruction);

  /**
   * @brief Returns the history of shown SubTrees. Empty in viewer mode.
   */
  const SubTreeHistory& GetHistory() const;

  /**
   * @brief Returns the top-level instruction of the given history entry.
   *
   * @return The top-level ancestor of the entry's instruction, or nullptr if it was deleted.
   */
  const InstructionItem* FindHistoryScope(const SubTreeHistoryEntry& entry) const;

  /**
   * @brief Checks if there is an earlier SubTree to go back to.
   *
   * Entries of deleted SubTrees and of the current one are skipped.
   */
  bool CanGoBack() const;

  /**
   * @brief Checks if there is a later SubTree to go forward to.
   */
  bool CanGoForward() const;

  /**
   * @brief Shows the previous SubTree of the history.
   */
  void GoBack();

  /**
   * @brief Shows the next SubTree of the history.
   */
  void GoForward();

  /**
   * @brief Shows the SubTree of the history entry with the given index, and makes it current.
   *
   * Does nothing if the SubTree of the entry was deleted. The index must be in range.
   */
  void GoToHistoryEntry(std::size_t index);

  /**
   * @brief Returns the top-level instructions shown on the scene.
   */
  std::vector<InstructionItem*> GetVisibleTopLevelInstructions() const;

  /**
   * @brief Returns selected instructions. Empty if inactive.
   */
  std::vector<InstructionItem*> GetSelectedInstructions() const;

  /**
   * @brief Selects the shapes of the given instructions.
   *
   * An instruction without a shape (inside a collapsed parent) selects its nearest ancestor with a
   * shape. Instructions in hidden SubTrees are not selected.
   */
  void SetSelectedInstructions(const std::vector<const InstructionItem*>& instructions);

  /**
   * @brief Deletes selected shapes and connections.
   */
  void OnDeleteSelected();

  /**
   * @brief Aligns children of the single selected instruction.
   */
  void AlignSelected();

signals:
  void selectionChanged();
  void connectionStarted();
  void connectionFinished();

  /**
   * @brief Notifies that the result of GetScope has changed.
   */
  void scopeChanged();

  /**
   * @brief Notifies that a new scope is on the scene and aligned. Editor mode only.
   */
  void scopeShown();

  /**
   * @brief Requests the view to center on the selection set by OpenSubTree or GoToInstruction.
   */
  void centerOnSelectionRequested();

  /**
   * @brief Notifies that the history or its current entry has changed.
   */
  void historyChanged();

private:
  //! Creates the provider if active, and posts the view update if required.
  void SetupProvider();

  //! Creates the provider for the current hidden set.
  std::unique_ptr<GraphicsSceneComponentProvider> CreateProvider();

  //! Aligns the visible instructions if required, and emits scopeShown after a scope switch.
  void UpdateView();

  //! Connects scene signals to the provider.
  void SetupSceneConnections();

  //! Shows the SubTree of the given top-level instruction. Returns false if nothing changed.
  bool SwitchScope(const InstructionItem* top_level);

  //! Switches the scope and adds it to the history.
  void NavigateTo(const InstructionItem* top_level, const std::string& include_identifier);

  //! Checks if the entry's SubTree exists and is not the current one.
  bool IsHistoryTarget(const SubTreeHistoryEntry& entry) const;

  //! Opens the SubTree of an Include, selects the branch of other instructions.
  void OnInstructionDoubleClick(const InstructionItem* instruction);

  NodeEditorMode m_editor_mode;
  NodeGraphicsScene* m_scene{nullptr};
  send_message_t m_send_message_callback;
  ProcedureItem* m_procedure_item{nullptr};
  bool m_active{false};
  std::unique_ptr<GraphicsSceneComponentProvider> m_provider;
  std::unique_ptr<SubTreeScope> m_scope;
  SubTreeHistory m_history;

  //!< true when a deferred view update has already been posted to the event loop
  bool m_view_update_scheduled{false};

  //!< true when scopeShown should be emitted on the next view update
  bool m_scope_shown_required{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_EDITOR_SCENE_CONTROLLER_H_
