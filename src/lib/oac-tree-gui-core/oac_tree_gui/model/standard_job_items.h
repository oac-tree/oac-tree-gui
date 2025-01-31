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

#ifndef OAC_TREE_GUI_MODEL_STANDARD_JOB_ITEMS_H_
#define OAC_TREE_GUI_MODEL_STANDARD_JOB_ITEMS_H_

//! @file
//! Collection of items derived from JobItem class.

#include <oac_tree_gui/model/job_item.h>

namespace oac_tree_gui
{

/**
 * @brief The LocalJobItem class is intended to run the job based on a locally edited sequencer
 * procedure.
 *
 * It doesn't own the procedure, and is linked to it via LinkedItem mechanism.
 */
class LocalJobItem : public JobItem
{
public:
  LocalJobItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

/**
 * @brief The ImportedJobItem class is intended to run the job based on ProcedureItem imported from
 * somewhere.
 *
 * It carries the result of the import on board. Thus, imported procedure will be deleted, when
 * JobItem is removed from the model.
 */
class ImportedJobItem : public JobItem
{
public:
  static inline const std::string kImportedProcedure = "kImportedProcedure";
  ImportedJobItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

/**
 * @brief The RemoteJobItem class is intended to run and listen remote sequencer automation
 * procedure.
 */
class RemoteJobItem : public JobItem
{
public:
  RemoteJobItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns remote job index.
   *
   * It index wasn't set, will return -1.
   */
  std::size_t GetRemoteJobIndex() const;

  /**
   * @brief Sets remote job index.
   */
  void SetRemoteJobIndex(std::size_t value);

  /**
   * @brief Returns the remote server name.
   */
  std::string GetServerName() const;

  /**
   * @brief Sets the name of the remote server.
   */
  void SetServerName(const std::string& name);
};

/**
 * @brief The FileBasedJobItem class is intended to run the job which is linked to particular
 * Sequencer XML file on disk.
 */
class FileBasedJobItem : public JobItem
{
public:
  FileBasedJobItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& value);
};

/**
 * @brief Return types of all available job items.
 */
std::vector<std::string> GetJobItemTypes();

/**
 * @brief Creates job item intended for running local procedures.
 *
 * The procedure shall belong to some model, and it will be linked with this JobItem via the linked
 * item mechanism.
 *
 * @param procedure The procedure to run.
 * @param tick_timeout_msec Initial value of tick timeout during procedure execution.
 * @return New job item.
 */
std::unique_ptr<JobItem> CreateLocalJobItem(ProcedureItem* procedure, int tick_timeout_msec = 0);

/**
 * @brief Creates job item intended to run ProcedureItem imported from somewhere.
 *
 * The procedure will be inserted in dedicated tag on board of this item. Ownership will be taken.
 *
 * @param procedure The procedure to run.
 * @param tick_timeout_msec Initial value of tick timeout during procedure execution.
 * @return New job item.
 */
std::unique_ptr<JobItem> CreateImportedJobItem(std::unique_ptr<ProcedureItem> procedure,
                                               int tick_timeout_msec = 0);

/**
 * @brief Creates job item intended to control remote procedures.
 *
 * @param server_name The name of the automation server.
 * @param job_index Index of servers' job.
 * @return New job item.
 */
std::unique_ptr<RemoteJobItem> CreateRemoteJobItem(const std::string& server_name,
                                                   std::size_t job_index);

/**
 * @brief Creates job item intended to import and run particular Sequencer XML file on disk.
 *
 * @param file_name The full path to Sequencer XML file.
 * @param tick_timeout_msec Initial value of tick timeout during procedure execution.
 * @return New job item.
 */
std::unique_ptr<JobItem> CreateFileBasedJobItem(const std::string& file_name,
                                                int tick_timeout_msec = 0);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_STANDARD_JOB_ITEMS_H_
