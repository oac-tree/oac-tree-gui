/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERACTIONS_H
#define SEQUENCERGUI_COMPOSER_COMPOSERACTIONS_H

#include "sequencergui/composer/composercontext.h"

#include <QObject>

#include <memory>

namespace sequencergui
{
class SequencerModel;
class MessageInterface;

//! Collection of actions

class ComposerActions : public QObject
{
  Q_OBJECT

public:
  explicit ComposerActions(SequencerModel* model, QObject* parent = nullptr);
  ~ComposerActions() override;

  void SetModel(SequencerModel* model);

  void SetContext(ComposerContext context);

  void InsertInstructionAfterRequest(const std::string& item_type);

  void InsertInstructionIntoRequest(const std::string& item_type);

private:
  SequencerModel* m_model{nullptr};
  ComposerContext m_context;
  std::unique_ptr<MessageInterface> m_message_reporter;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERACTIONS_H
