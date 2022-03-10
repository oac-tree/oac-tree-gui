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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H
#define ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H

#include <QObject>

namespace mvvm
{
class ApplicationModel;
}

namespace anyvalueeditor
{

//! Actions for AnyValueEditor

class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent = nullptr);
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H
