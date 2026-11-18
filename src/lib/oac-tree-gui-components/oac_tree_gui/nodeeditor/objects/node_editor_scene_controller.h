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

#include <sup/gui/core/message_event.h>

#include <QObject>
#include <functional>
#include <memory>
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
 * In viewer mode it shows all top-level instructions, read-only.
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
   * @brief Sets the procedure. Shows the root SubTree and forgets loose instructions.
   */
  void SetProcedure(ProcedureItem* procedure);

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
   * Loose instructions of the current SubTree are recorded and shown again on return.
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

private:
  //! Creates the provider if active, and posts the view update if required.
  void SetupProvider();

  //! Creates the provider for the current hidden set.
  std::unique_ptr<GraphicsSceneComponentProvider> CreateProvider();

  //! Aligns the visible instructions if required, and emits scopeShown after a scope switch.
  void UpdateView();

  //! Connects scene signals to the provider.
  void SetupSceneConnections();

  NodeEditorMode m_editor_mode;
  NodeGraphicsScene* m_scene{nullptr};
  send_message_t m_send_message_callback;
  ProcedureItem* m_procedure_item{nullptr};
  bool m_active{false};
  std::unique_ptr<GraphicsSceneComponentProvider> m_provider;
  std::unique_ptr<SubTreeScope> m_scope;

  //!< true when a deferred view update has already been posted to the event loop
  bool m_view_update_scheduled{false};

  //!< true when scopeShown should be emitted on the next view update
  bool m_scope_shown_required{false};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_OBJECTS_NODE_EDITOR_SCENE_CONTROLLER_H_
