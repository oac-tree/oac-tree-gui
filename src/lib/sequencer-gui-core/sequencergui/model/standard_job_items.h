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

#ifndef SEQUENCERGUI_MODEL_STANDARD_JOB_ITEMS_H_
#define SEQUENCERGUI_MODEL_STANDARD_JOB_ITEMS_H_

//! @file
//! Collection of items derived from JobItem class.

#include <sequencergui/model/job_item.h>

namespace sequencergui
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
  static inline const std::string Type = "LocalJob";

  using JobItem::JobItem;
  LocalJobItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

/**
 * @brief The ImportedJobItem class is intended to run the job based on procedure imported from
 * disk.
 *
 * It carries the result of the import on board. Thus, imported procedure will be deleted, when
 * JobItem is removed from the model.
 */
class ImportedJobItem : public JobItem
{
public:
  static inline const std::string Type = "ImportedJob";
  static inline const std::string kImportedProcedure = "kImportedProcedure";

  using JobItem::JobItem;
  ImportedJobItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

/**
 * @brief The RemoteJobItem class is intended to run and listen remote sequencer automation
 * procedure.
 */
class RemoteJobItem : public JobItem
{
public:
  static inline const std::string Type = "RemoteJob";

  using JobItem::JobItem;
  RemoteJobItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  /**
   * @brief Returns remote job index.
   *
   * It index wasn't set, will return -1.
   */
  size_t GetRemoteJobIndex() const;

  /**
   * @brief Sets remote job index.
   */
  void SetRemoteJobIndex(size_t value);

  /**
   * @brief Returns the remote server name.
   */
  std::string GetServerName() const;

  /**
   * @brief Sets the name of the remote server.
   */
  void SetServerName(const std::string& name);
};

std::unique_ptr<JobItem> CreateLocalJobItem(ProcedureItem* procedure);

std::unique_ptr<JobItem> CreateImportedJobItem(std::unique_ptr<ProcedureItem> procedure);

std::unique_ptr<JobItem> CreateRemoteJobItem(const std::string& server_name, size_t job_index);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_JOB_ITEMS_H_
