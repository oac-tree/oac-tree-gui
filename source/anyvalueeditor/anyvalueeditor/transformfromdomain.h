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

#ifndef ANYVALUEEDITOR_TRANSFORMFROMDOMAIN_H
#define ANYVALUEEDITOR_TRANSFORMFROMDOMAIN_H

namespace sup::dto
{
class AnyValue;
}

namespace anyvalueeditor
{

class AnyValueItem;

void PopulateItem(const sup::dto::AnyValue* domain, AnyValueItem* item);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_TRANSFORMFROMDOMAIN_H
