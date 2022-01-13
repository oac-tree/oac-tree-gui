/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/nodeeditor/sceneutils.h"

#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/sequencerutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QDebug>
#include <QLinearGradient>
#include <QRectF>
#include <numeric>

namespace
{

//! Returns true if instruction has not been positioned yet.
bool HasNoPosition(sequencergui::InstructionItem* instruction)
{
  const bool zero_x = std::abs(instruction->GetX()) < std::numeric_limits<double>::epsilon();
  const bool zero_y = std::abs(instruction->GetY()) < std::numeric_limits<double>::epsilon();
  return zero_x && zero_y;
}

}  // namespace

namespace sequencergui
{

double GetAlignmentGridWidth()
{
  return sequencergui::ConnectableViewRectangle().width() * 1.4;
}

double GetAlignmentGridHeight()
{
  return sequencergui::ConnectableViewRectangle().height() * 1.4;
}

QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect)
{
  QLinearGradient result(rect.x() + rect.width() / 2, rect.y(), rect.x() + rect.width() / 2,
                         rect.y() + rect.height());
  result.setColorAt(0, color);
  result.setColorAt(0.5, color.lighter(150));
  result.setColorAt(1, color);
  return result;
}

QRectF ConnectableViewRectangle()
{
  // make size of rectangle depending on 'M'-letter size to address scaling issues
  static QRectF result = QRectF(0, 0, mvvm::utils::WidthOfLetterM() * 8.2,
                                mvvm::utils::HeightOfLetterM() * 8.2);
  return result;
}

std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width)
{
  std::vector<QPointF> result;
  if (n_points < 0)
  {
    throw std::runtime_error("Error in GetPositions: invalid number of points");
  }

  double xpos = reference.x() - (n_points - 1) * width / 2;
  for (int i = 0; i < n_points; ++i)
  {
    result.emplace_back(QPointF(xpos, reference.y()));
    xpos += width;
  }
  return result;
}

void AlignTree(const QPointF& reference, InstructionContainerItem* container, bool force)
{
  auto positions =
      GetPositions(reference, container->GetInstructions().size(), GetAlignmentGridWidth());
  int index{0};
  for (const auto child : container->GetInstructions())
  {
    if (HasNoPosition(child))
    {
      child->SetX(positions[index].x());
      child->SetY(positions[index].y());
    }
    AlignInstructionTree(positions[index], child, force);
    ++index;
  }
}

void AlignInstructionTree(const QPointF& reference, sequencergui::InstructionItem* instruction, bool force)
{
  auto positions = sequencergui::GetPositions(reference, instruction->GetInstructions().size(),
                                       sequencergui::GetAlignmentGridWidth());
  int index{0};
  for (const auto child : instruction->GetInstructions())
  {
    QPointF child_pos(positions[index].x(), positions[index].y() + sequencergui::GetAlignmentGridHeight());
    if (HasNoPosition(child) || force)
    {
      child->SetX(child_pos.x());
      child->SetY(child_pos.y());
    }
    AlignInstructionTree(child_pos, child, force);
    ++index;
  }
}

QColor GetBaseColor(const InstructionItem* instruction)
{
  if (instruction->GetType() == UnknownInstructionItem::Type)
  {
    return {"chartreuse"};
  }

  if (IsDecoratorInstruction(instruction))
  {
    return {"lightseagreen"};
  }

  if (IsCompoundInstruction(instruction))
  {
    return {"royalblue"};
  }

  return {Qt::lightGray};
}

}  // namespace sequi
